"""Builds the release's decl recipes (#30): our 45 decls in game1voidmod.resources as lines copied from the player's
own decls plus our text, so the release ships no game decl text. d2coop_setup applies them at install time and
checks each result's SHA-256.

Usage: python -I build_decl_recipes.py <lab base dir> <output recipes.bin> [--report report.json]

A decl is rebuilt line by line. Each line is its indentation, its body, its trailing whitespace and its line ending
(indentation and trailing whitespace are spaces, tabs and CRs). Bodies come from ops: copies of a source decl's line
bodies, or our text. Indentation, trailing whitespace and line endings are our formatting and are stored as runs; our
decls differ from the game's in all three (LF against CRLF, re-indented copies, CR CR LF endings).

Recipe file (integers little-endian, strings as u32 length + bytes):
  "D2RCP004", u32 entry count; per entry:
    u32 id, str type, str source, str destination, u32 size, 32-byte SHA-256 of the decl,
    u32 line count, u8 final newline,
    u32 ending runs, per run (u32 lines, u8 CRLF),
    u32 indent runs, per run (u32 lines, str indent),
    u32 trail runs, per run (u32 lines, str trail),
    u32 source count, per source (str type, str name, 32-byte SHA-256 of its unpacked bytes),
    u32 op count, per op u8 kind: 0 = text (str: bodies joined by LF), 1 = copy (u32 source, u32 first, u32 end).
A source's line bodies are its lines without the line ending and without leading and trailing spaces, tabs and CRs.
"""
import argparse
import difflib
import hashlib
import json
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'diag'))
import d2_resource_index as r  # noqa: E402

OURS = ('/d2coop', 'd2coop_')
MIN_RATIO = 0.3     # below this the decl is ours and ships as text
MIN_BLOCK = 4       # a residual block this long tries a second source
LF = b'\n'


def split(b):
    """(lines without ending, CRLF flags, final newline)."""
    out, crlf, at = [], [], 0
    while at < len(b):
        n = b.find(LF, at)
        end = len(b) if n < 0 else n
        cr = n >= 0 and end > at and b[end - 1:end] == b'\r'
        out.append(b[at:end - 1] if cr else b[at:end])
        crlf.append(cr)
        at = end + 1
    return out, crlf, b.endswith(LF)


def body_of(line):
    return line.strip(b' \t\r')


def bodies(b):
    return [body_of(l) for l in split(b)[0]]


def runs(values):
    out = []
    for v in values:
        if out and out[-1][1] == v:
            out[-1][0] += 1
        else:
            out.append([1, v])
    return out


def rebuild(content_bodies, indents, trails, crlf, final):
    parts = []
    for k, (ind, body, trail) in enumerate(zip(indents, content_bodies, trails)):
        parts.append(ind + body + trail)
        if k + 1 < len(content_bodies) or final:
            parts.append(b'\r\n' if crlf[k] else LF)
    return b''.join(parts)


def game_decls(base):
    """(type, source) -> (index, entry) of the newest game entry (our own entries excluded)."""
    found = {}
    for ix in r.containers(base):
        for e in r.entries(ix):
            if not any(o in e['source'] for o in OURS):
                found[(e['type'], e['source'])] = (ix, e)
    return found


def best_source(decls, kind, want_bodies, exclude=()):
    want = set(b.strip() for b in want_bodies if b.strip())
    size = sum(map(len, want_bodies))
    best = (0.0, None)
    for (t, s), (ix, e) in decls.items():
        if t != kind or s in exclude or e['size'] > 8 * max(1, size) or e['size'] * 8 < size:
            continue
        try:
            g = r.read(ix, e)
        except Exception:
            continue
        have = set(b.strip() for b in bodies(g) if b.strip())
        if have:
            j = len(want & have) / len(want | have)
            if j > best[0]:
                best = (j, (s, g))
    return best[1]


def ops_for(target, sources):
    """Copy ops from sources[0], a second source for long residual blocks, text for the rest."""
    if not sources:
        return [('text', LF.join(target))]
    ops = []
    for tag, i1, i2, j1, j2 in difflib.SequenceMatcher(None, bodies(sources[0][1]), target, autojunk=False).get_opcodes():
        if tag == 'equal':
            ops.append(('copy', 0, i1, i2))
        elif tag in ('insert', 'replace'):
            block = target[j1:j2]
            if len(sources) > 1 and len(block) >= MIN_BLOCK:
                for t2, a1, a2, b1, b2 in difflib.SequenceMatcher(None, bodies(sources[1][1]), block, autojunk=False).get_opcodes():
                    if t2 == 'equal':
                        ops.append(('copy', 1, a1, a2))
                    elif t2 in ('insert', 'replace'):
                        ops.append(('text', LF.join(block[b1:b2])))
            else:
                ops.append(('text', LF.join(block)))
    # Lines a sequence diff leaves as text (reordered copies) are looked up in the sources one by one.
    where = {}
    for k, src in enumerate(sources):
        for i, b in enumerate(bodies(src[1])):
            if len(b.strip()) > 2:
                where.setdefault(b, (k, i))
    looked = []
    for o in ops:
        if o[0] != 'text':
            looked.append(o)
            continue
        for line in o[1].split(LF):
            hit = where.get(line)
            if hit and looked and looked[-1][0] == 'copy' and looked[-1][1] == hit[0] and looked[-1][3] == hit[1]:
                looked[-1] = ('copy', hit[0], looked[-1][2], hit[1] + 1)
            elif hit:
                looked.append(('copy', hit[0], hit[1], hit[1] + 1))
            else:
                looked.append(('text', line))
    ops = looked
    merged = []
    for o in ops:  # join neighbouring text ops
        if merged and o[0] == 'text' and merged[-1][0] == 'text':
            merged[-1] = ('text', merged[-1][1] + LF + o[1])
        else:
            merged.append(o)
    return merged


def apply_ops(ops, sources):
    split_sources = [bodies(s[1]) for s in sources]
    out = []
    for o in ops:
        out += o[1].split(LF) if o[0] == 'text' else split_sources[o[1]][o[2]:o[3]]
    return out


def text_bytes(ops):
    return sum(len(o[1]) + 1 for o in ops if o[0] == 'text')


def s(x):
    b = x.encode('utf-8') if isinstance(x, str) else x
    return struct.pack('<I', len(b)) + b


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('base')
    ap.add_argument('out')
    ap.add_argument('--report')
    a = ap.parse_args()
    base = Path(a.base)
    ours = [e for e in r.entries(base / 'game1voidmod.index') if any(o in e['source'] for o in OURS)]
    decls = game_decls(base)
    out = bytearray(b'D2RCP004' + struct.pack('<I', len(ours)))
    report = []
    for e in ours:
        body = r.read(base / 'game1voidmod.index', e)
        raw, crlf, final = split(body)
        target = [body_of(l) for l in raw]
        indents = [l[:len(l) - len(l.lstrip(b' \t\r'))] for l in raw]
        trails = [l[len(i) + len(t):] for l, i, t in zip(raw, indents, target)]
        assert all(i + t + tr == l for l, i, t, tr in zip(raw, indents, target, trails))
        assert all(LF not in t for t in target)
        sources = []
        first = best_source(decls, e['type'], target)
        if first and (difflib.SequenceMatcher(None, bodies(first[1]), target, autojunk=False).ratio() >= MIN_RATIO or
                      text_bytes(ops_for(target, [first])) * 2 < sum(map(len, target))):
            sources = [first]
            rest = [l for o in ops_for(target, sources) if o[0] == 'text' for l in o[1].split(LF)]
            if len(rest) >= MIN_BLOCK:
                second = best_source(decls, e['type'], rest, exclude=(first[0],))
                if second and text_bytes(ops_for(target, sources + [second])) < text_bytes(ops_for(target, sources)):
                    sources.append(second)
        ops = ops_for(target, sources)
        assert rebuild(apply_ops(ops, sources), indents, trails, crlf, final) == body, e['source']
        out += struct.pack('<I', e['id']) + s(e['type']) + s(e['source']) + s(e['destination']) + struct.pack('<I', len(body))
        out += hashlib.sha256(body).digest() + struct.pack('<IB', len(target), 1 if final else 0)
        endings = runs(crlf)
        out += struct.pack('<I', len(endings)) + b''.join(struct.pack('<IB', n, 1 if v else 0) for n, v in endings)
        indent_runs = runs(indents)
        out += struct.pack('<I', len(indent_runs)) + b''.join(struct.pack('<I', n) + s(v) for n, v in indent_runs)
        trail_runs = runs(trails)
        out += struct.pack('<I', len(trail_runs)) + b''.join(struct.pack('<I', n) + s(v) for n, v in trail_runs)
        out += struct.pack('<I', len(sources))
        for name, data in sources:
            out += s(e['type']) + s(name) + hashlib.sha256(data).digest()
        out += struct.pack('<I', len(ops))
        for o in ops:
            out += (b'\x00' + s(o[1])) if o[0] == 'text' else (b'\x01' + struct.pack('<III', o[1], o[2], o[3]))
        text = text_bytes(ops)
        report.append({'source': e['source'], 'size': len(body), 'sources': [n for n, _ in sources], 'ourTextBytes': text,
                       'copiedBodyBytes': sum(len(b) for b in target) - min(text, sum(len(b) for b in target)), 'ops': len(ops),
                       'indentRuns': len(indent_runs), 'endingRuns': len(endings)})
        print(json.dumps(report[-1]), flush=True)
    Path(a.out).write_bytes(bytes(out))
    if a.report:
        Path(a.report).write_bytes((json.dumps(report, indent=1) + '\n').encode('utf-8'))
    print(json.dumps({'entries': len(ours), 'bytes': len(out), 'ourText': sum(x['ourTextBytes'] for x in report),
                      'copied': sum(x['copiedBodyBytes'] for x in report)}))


if __name__ == '__main__':
    main()
