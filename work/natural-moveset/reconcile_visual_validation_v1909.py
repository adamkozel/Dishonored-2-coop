"""Final audit (goal criterion 1): reconciles review-catalog.json's visual_validation with the completion audits.

apply_completion_audit.py recorded each audit's reviewed videos and verdicts in the item's completion record, but never
updated visual_validation, so 23 complete items kept a pre-audit value (16 not_accepted, 7 bounded_actual_game_*).
For each complete item whose visual_validation is not accepted_*, this checks that its completion record cites at least
one reviewed video with a verdict and that every cited video exists, then sets visual_validation to
accepted_completion_audit and keeps the old value in visual_validation_before. It refuses the whole change if any item
does not check out, and regenerates the movement requirements document.
The same holds for runtime_binding: 8 complete items kept not_verified although their completion record cites runtime
takes whose cases passed. Each cited case is re-checked in its take's result.json (as apply_completion_audit.py does)
and runtime_binding becomes native_runtime_completion_audit, the old value kept in runtime_binding_before.

Usage: reconcile_visual_validation_v1909.py [--dry-run]"""
import json, subprocess, sys
from pathlib import Path

N = Path(__file__).resolve().parent
ROOT = N.parents[1]
CATALOG = N / 'review-catalog.json'
sys.path.insert(0, str(N))
from apply_completion_audit import case_results


def main():
    dry = '--dry-run' in sys.argv
    catalog = json.loads(CATALOG.read_text(encoding='utf-8'))
    changed, problems, bound, cache = [], [], [], {}
    for item in catalog['items']:
        if item.get('phase_status') != 'complete' or item.get('runtime_binding') != 'not_verified':
            continue
        takes = (item.get('completion') or {}).get('takes') or []
        if not takes:
            problems.append(f'{item["id"]}: complete without runtime takes')
        for ref in takes:
            if ref['take'] not in cache:
                try:
                    cache[ref['take']] = case_results(ref['take'])
                except FileNotFoundError:
                    cache[ref['take']] = None
            if cache[ref['take']] is None:
                problems.append(f'{item["id"]}: {ref["take"]} has no result.json'); continue
            for case in ref['cases']:
                row = cache[ref['take']].get(case)
                if not row or not row.get('passed'):
                    problems.append(f'{item["id"]}: {ref["take"]} {case} did not pass')
        bound.append((item, sum(len(r['cases']) for r in takes)))
    for item in catalog['items']:
        vv = item.get('visual_validation', '')
        if item.get('phase_status') != 'complete' or str(vv).startswith('accepted'):
            continue
        visual = (item.get('completion') or {}).get('visual') or []
        if not visual:
            problems.append(f'{item["id"]}: complete without reviewed videos')
            continue
        for v in visual:
            if not v.get('verdict'):
                problems.append(f'{item["id"]}: {v.get("video")} has no verdict')
            if not (N / v['video']).exists():
                problems.append(f'{item["id"]}: video {v["video"]} missing')
        changed.append((item, vv, len(visual)))
    if problems:
        print('RECONCILE_REFUSED'); print('\n'.join(problems)); sys.exit(1)
    for item, vv, n in changed:
        print(f'{item["id"]}: {vv} -> accepted_completion_audit ({n} reviewed videos, {item["completion"]["audit"]})')
        item['visual_validation_before'] = vv
        item['visual_validation'] = 'accepted_completion_audit'
    for item, n in bound:
        print(f'{item["id"]}: not_verified -> native_runtime_completion_audit ({n} passing cases, {item["completion"]["audit"]})')
        item['runtime_binding_before'] = 'not_verified'
        item['runtime_binding'] = 'native_runtime_completion_audit'
    print(json.dumps({'reconciled': len(changed), 'bound': len(bound)}))
    if dry:
        return
    CATALOG.write_bytes(json.dumps(catalog, indent=2).encode('utf-8'))  # the catalog's existing format (LF, ASCII)
    subprocess.run([sys.executable, str(ROOT / 'work' / 'regen_movement_requirements.py')], check=True)


if __name__ == '__main__':
    main()
