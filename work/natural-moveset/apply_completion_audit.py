"""#11: applies a movement completion audit to review-catalog.json.

An audit (completion-audit-v*.json) lists, per checklist item, the decision ("complete" or "open"), the scope it covers,
the coverage takes and cases it relies on, the reviewed observer videos with their verdicts, and what remains when open.
This tool verifies every cited case passed in its take's result.json and every cited video exists, then records the
decision on the item (phase_status, complete, completion) and refuses the whole audit if anything does not check out.
Finally it regenerates the movement requirements document.

Usage: apply_completion_audit.py AUDIT.json [--dry-run]"""
import json, subprocess, sys
from pathlib import Path

N = Path(__file__).resolve().parent
ROOT = N.parents[1]
CATALOG = N / 'review-catalog.json'

def case_results(take):
    result = json.loads((N / take / 'result.json').read_text(encoding='utf-8-sig'))
    # Use-variant takes name a case by its target mechanism instead of a case name.
    return {f"{c['source']}-{c.get('case') or c.get('target') or c.get('window')}": c for c in result.get('cases', [])}

def main():
    audit_path = Path(sys.argv[1]); dry = '--dry-run' in sys.argv
    audit = json.loads(audit_path.read_text(encoding='utf-8'))
    catalog = json.loads(CATALOG.read_text(encoding='utf-8'))
    items = {i['id']: i for i in catalog['items']}
    problems = []; cache = {}
    for entry in audit['items']:
        iid = entry['id']
        if iid not in items:
            problems.append(f'{iid}: not in the catalog'); continue
        if entry['decision'] not in ('complete', 'open'):
            problems.append(f'{iid}: decision {entry["decision"]}')
        for ref in entry.get('takes', []):
            take = ref['take']
            if take not in cache:
                try:
                    cache[take] = case_results(take)
                except FileNotFoundError:
                    cache[take] = None
            if cache[take] is None:
                problems.append(f'{iid}: {take} has no result.json'); continue
            for case in ref['cases']:
                row = cache[take].get(case)
                if not row or not row.get('passed'):
                    problems.append(f'{iid}: {take} {case} did not pass')
        for v in entry.get('visual', []):
            if not (N / v['video']).exists():
                problems.append(f'{iid}: video {v["video"]} missing')
        if entry['decision'] == 'complete' and entry.get('remaining'):
            problems.append(f'{iid}: complete but lists remaining work')
        if entry['decision'] == 'open' and not entry.get('remaining'):
            problems.append(f'{iid}: open without remaining work')
    if problems:
        print('AUDIT_REFUSED'); print('\n'.join(problems)); sys.exit(1)
    for entry in audit['items']:
        item = items[entry['id']]
        record = {'audit': audit_path.name, 'decision': entry['decision'], 'scope': entry['scope'],
                  'takes': entry.get('takes', []), 'visual': entry.get('visual', []), 'lifecycle': entry.get('lifecycle', '')}
        if entry.get('remaining'):
            record['remaining'] = entry['remaining']
        item['completion'] = record
        if entry['decision'] == 'complete':
            item['phase_status'] = 'complete'; item['complete'] = True
        for ref in entry.get('takes', []):
            ev = f"{ref['take']}/result.json"
            if ev not in item.setdefault('evidence', []):
                item['evidence'].append(ev)
    done = sum(1 for i in catalog['items'] if i.get('phase_status') == 'complete')
    print(json.dumps({'audit': audit_path.name, 'items': len(audit['items']), 'complete_now': done,
                      'remaining': sum(1 for i in catalog['items'] if i.get('phase_status') == 'work_remaining')}))
    if dry:
        return
    CATALOG.write_bytes(json.dumps(catalog, indent=2).encode('utf-8'))  # the catalog's existing format (LF, ASCII)
    subprocess.run([sys.executable, str(ROOT / 'work' / 'regen_movement_requirements.py')], check=True)

if __name__ == '__main__':
    main()
