"""Compare the story flags of the two isolated lab games (read-only).

Reads every story flag variable (storyflags/*.kiscule instances on the worldspawn) in the host and client lab
games with d2_kiscule_vars.py and reports the flags whose value bytes differ, or that only one world has. The
flags are where the campaign records decisions that later maps read (for example Stilton saved or not), so a
difference after a decision is a decision that did not reach the other world.

Writes only the differences (and counts) as JSON; the full flag dumps are game-derived and stay local.

Usage:
  python -I d2_storyflags_diff.py [--out FILE] [--grep REGEX]
"""
import argparse
import json
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
VARS = Path(__file__).resolve().parent / 'd2_kiscule_vars.py'
LINE = re.compile(r'^(?P<label>\S+) (?P<name>\S+): default (?P<default>-?\d+) value (?P<value>-?\d+) \(bytes (?P<bytes>[0-9a-f]+)\)$')


def flags(role):
    pid = json.loads((ROOT / f'work/lab-instances/{role}/launch.json').read_text(encoding='utf-8-sig'))['pid']
    out = subprocess.run([sys.executable, '-I', str(VARS), str(pid), 'storyflags'], capture_output=True, text=True, timeout=120)
    if out.returncode:
        raise SystemExit(f'{role}: {out.stderr.strip()[:300]}')
    result = {}
    for line in out.stdout.splitlines():
        m = LINE.match(line.strip())
        if m:
            result[f"{m['label']}/{m['name']}"] = {'value': int(m['value']), 'default': int(m['default']), 'bytes': m['bytes']}
    return result


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--out', default='')
    ap.add_argument('--grep', default='')
    a = ap.parse_args()
    host, client = flags('host'), flags('client')
    rx = re.compile(a.grep, re.I) if a.grep else None
    diffs = []
    for key in sorted(set(host) | set(client)):
        if rx and not rx.search(key):
            continue
        h, c = host.get(key), client.get(key)
        # Bytes 8 and 9 are a bool flag's default and current value; the rest of the 16 bytes is padding.
        if h is None or c is None or h['bytes'][16:20] != c['bytes'][16:20]:
            diffs.append({'flag': key, 'host': h, 'client': c})
    report = {'readOnly': True, 'hostFlags': len(host), 'clientFlags': len(client), 'differences': len(diffs), 'diffs': diffs}
    text = json.dumps(report, indent=1)
    if a.out:
        Path(a.out).write_text(text + '\n', encoding='utf-8')
    print(text if len(text) < 6000 else json.dumps({k: report[k] for k in ('hostFlags', 'clientFlags', 'differences')}))


if __name__ == '__main__':
    main()
