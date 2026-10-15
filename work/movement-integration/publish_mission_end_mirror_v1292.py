"""Publish the mission-end mirror take (1291b, candidate 1214ax): audit JSON and task state."""
import json
from datetime import datetime, timezone, timedelta
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
TAKE = ROOT / 'work/natural-moveset/mission-end-mirror-v1291b'
now = datetime.now(timezone(timedelta(hours=2))).isoformat()

result = json.loads((TAKE / 'result.json').read_text(encoding='utf-8-sig'))
case = result['cases'][0]
mirror = {r: json.loads((TAKE / f'missionend-{r}.json').read_text(encoding='utf-8-sig')) for r in ('host', 'client')}
audit = {
    'at': now,
    'candidate': 'candidate-npc-combat-v1214ax',
    'takes': [{'take': TAKE.name, 'passed': result['passed'], 'failure': result['failure']}],
    'complete': False,
    'scope': '#16 stage 6: the mission end reaches the next mission in both worlds (Edge of the World -> The Good Doctor), lab mode, host-originated.',
    'findings': [
        'Confirmed static: gameflow set-state 0x1404e9bd0 (manager game+0x1f7780, &node id, state); mission requests 0x14040f760 allow 1->2, 2->3 (complete), 2->4, 2->5; a mission reaching 3 with its result screen enabled switches logic to 11 (0x1404ea020 -> SwitchState 0x1407885b0).',
        'Confirmed runtime (1214ax): the host entered its result screen and sent D2ME (sent %d); the client found its own active mission among %d mission nodes, completed it natively (applied %d, state %d).' % (mirror['host']['sent'], mirror['client']['missions'], mirror['client']['applied'], mirror['client']['appliedState']),
        'Host states: ' + ' -> '.join(case['hostStates']) + '.',
        'Client states: ' + ' -> '.join(case['clientStates']) + '.',
        'The client result screen opened in the same 400 ms sample as the host one (36.4 s); both reached gameplay on institute_p_lowchaos 1.6 s apart; the Steam connection stayed fresh.',
        'Visual (accepted): Corvo and Emily each saw their own native "Edge of the World - Mission 02 Complete" screen with their own portrait, then the Good Doctor intro and gameplay.',
        'Side effect: the host instance unlocked the Steam achievement "Ghostly" on the shared lab account.',
    ],
    'next': [
        'Partner loads the host chaos variant (each world picks institute_p_low/highchaos from its own chaos).',
        'Both-ready hold at the next mission start (each player presses Continue independently).',
        'Client-originated mission end; product mode across the VM; boat and in-mission LevelTransition boundaries (chapter 97).',
    ],
}
(ROOT / 'work/movement-integration/mission-end-mirror-audit-v1292.json').write_text(json.dumps(audit, indent=2) + '\n', encoding='utf-8')

state_path = ROOT / 'work/current-task-state.json'
state = json.loads(state_path.read_text(encoding='utf-8-sig'))
state['UpdatedAt'] = now
state['MissionTransitions'].update({
    'Audit': 'work/movement-integration/mission-end-mirror-audit-v1292.json',
    'Candidate': 'candidate-npc-combat-v1214ax',
    'PartnerFollowsMissionEnd': True,
    'NextAction': audit['next'][0] + ' ' + audit['next'][1],
})
state_path.write_text(json.dumps(state, indent=2) + '\n', encoding='utf-8')
print('ok')
