"""Publish the chaos-follows-host takes (1291e failed harness, 1291f pass; candidate 1214ba): audit JSON and task state."""
import json
from datetime import datetime, timezone, timedelta
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
NM = ROOT / 'work/natural-moveset'
now = datetime.now(timezone(timedelta(hours=2))).isoformat()
e = json.loads((NM / 'mission-end-chaos-v1291e/result.json').read_text(encoding='utf-8-sig'))
f = json.loads((NM / 'mission-end-chaos-v1291f/result.json').read_text(encoding='utf-8-sig'))
chaos = {r: json.loads((NM / f'mission-end-chaos-v1291f/missionend-{r}.json').read_text(encoding='utf-8-sig'))['chaos'] for r in ('host', 'client')}
case = f['cases'][0]
audit = {
    'at': now,
    'candidate': 'candidate-npc-combat-v1214ba',
    'takes': [
        {'take': 'mission-end-chaos-v1291e', 'passed': e['passed'], 'failure': e['failure'],
         'note': 'Harness defect: $chaos collided with the -Chaos parameter (PowerShell variables are case-insensitive); failed after load, lab restored.'},
        {'take': 'mission-end-chaos-v1291f', 'passed': f['passed'], 'failure': f['failure']},
    ],
    'complete': False,
    'scope': '#16 stage 6: chaos follows the host, so both worlds load the same next-map variant.',
    'findings': [
        'Confirmed runtime: the player arkCpntPlayerChaos component (vtable +0x1b63248) holds the chaos level at +0x20 and the points at +0x24 (0/0 on the Dunwall fixture).',
        'Confirmed static: the mission-end level is 0 below the mission high threshold, 1 from it, 2 from very high (0x1404ea390; Edge of the World 55/128).',
        '1214ba: the host publishes its points about once a second (D2CH); the partner keeps its own equal in gameplay or scenes.',
        'Take 1291f: the lab set the host to 60 points; the client followed (%d writes, from %d). Both computed level %d at the end and both loaded %s (same map: %s).' % (
            chaos['client']['writes'], chaos['client']['before'], chaos['client']['localLevel'], case['final']['host']['map'], case['sameMap']),
        'Before 1214ba each world used its own points, so a host-only chaos change would have sent the partner to the other variant.',
    ],
    'next': [
        'Client-originated mission end; product mode across the VM.',
        'Hand-offs without a result screen and in-mission LevelTransition map changes (chapter 97).',
    ],
}
(ROOT / 'work/movement-integration/chaos-follows-host-audit-v1294.json').write_text(json.dumps(audit, indent=2) + '\n', encoding='utf-8')
state_path = ROOT / 'work/current-task-state.json'
state = json.loads(state_path.read_text(encoding='utf-8-sig'))
state['UpdatedAt'] = now
state['MissionTransitions'].update({
    'Audit': 'work/movement-integration/chaos-follows-host-audit-v1294.json',
    'Candidate': 'candidate-npc-combat-v1214ba',
    'ChaosFollowsHost': True,
    'NextAction': ' '.join(audit['next']),
})
state_path.write_text(json.dumps(state, indent=2) + '\n', encoding='utf-8')
print('ok')
