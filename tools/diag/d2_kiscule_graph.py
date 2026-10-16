"""Print one entity's Kiscule graph (read-only; game-derived output stays local).

Each node is listed with its parameters, followed by its connections to
other nodes: output connection point -> (node, input). Use it to read the
gate in front of a scripted event when d2_mission_route.py shows only roots.

Usage:
  python -I d2_kiscule_graph.py <base-dir> <map .entities regex> <entity name> [--grep REGEX]
Example:
  python -I d2_kiscule_graph.py work/asset-game/base "institute_p_lowchaos[.]entities$" mdnt_lab_mdnt_hyp_transfo_throwplayer
"""
import argparse
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import d2_mission_route as route  # noqa: E402


def parse_nodes(body):
    """Yield (id, class, params, connection points) for each Kiscule node."""
    starts = [m for m in re.finditer(r'\n\t{5}item\[\d+\] = \{\s*m_id = (\d+);\s*m_name = "(ark[A-Za-z<> *:]+)";', body)]
    for i, m in enumerate(starts):
        end = starts[i + 1].start() if i + 1 < len(starts) else len(body)
        seg = body[m.end():end]
        cut = seg.find('m_connectionPoints')
        params = re.findall(r'\b(m_(?!nodePos|id\b|name\b)[A-Za-z]+) = ("?[^;\n{]*"?);', seg[:cut if cut > 0 else len(seg)])
        points = []
        for cp in re.finditer(r'm_id = (\d+);\s*m_name = "([^"]*)";\s*m_type = "([A-Za-z]+)";', seg[cut:] if cut > 0 else ''):
            tail = seg[cut:][cp.end():]
            nxt = re.search(r'm_id = \d+;\s*m_name = "[^"]*";\s*m_type = ', tail)
            block = tail[:nxt.start()] if nxt else tail
            ends = re.findall(r'm_endNodeID = (\d+);\s*m_endConnectionPointID = (\d+);', block)
            points.append((cp.group(1), cp.group(2), cp.group(3), ends))
        yield m.group(1), m.group(2), params, points


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('base')
    ap.add_argument('map')
    ap.add_argument('entity')
    ap.add_argument('--grep', default='')
    a = ap.parse_args()
    _, text = route.load_entities(a.base, a.map)
    ents = route.split_entities(text)
    if a.entity not in ents:
        raise SystemExit('no entity ' + a.entity)
    rx = re.compile(a.grep, re.I) if a.grep else None
    nodes = list(parse_nodes(ents[a.entity]))
    names = {nid: cls for nid, cls, _, _ in nodes}
    for nid, cls, params, points in nodes:
        line = f'#{nid} {cls.replace("arkKisculeNode", "")} ' + ' '.join(f'{k}={v}' for k, v in params)
        outs = []
        for _pid, pname, ptype, ends in points:
            if ptype.startswith('Output'):
                for en, ec in ends:
                    outs.append(f'{pname} -> #{en} {names.get(en, "?").replace("arkKisculeNode", "")}')
        if rx and not rx.search(line + ' '.join(outs)):
            continue
        print(line)
        for o in outs:
            print('    ' + o)


if __name__ == '__main__':
    main()
