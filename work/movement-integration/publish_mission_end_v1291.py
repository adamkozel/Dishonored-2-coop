"""Publish the Edge of the World mission-end take (1291a): audit JSON and task state."""
import json
from datetime import datetime, timezone, timedelta
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
TAKE = ROOT / 'work/natural-moveset/mission-end-carriage-v1291a'
now = datetime.now(timezone(timedelta(hours=2))).isoformat()

result = json.loads((TAKE / 'result.json').read_text(encoding='utf-8-sig'))
case = result['cases'][0]
audit = {
    'at': now,
    'candidate': 'candidate-npc-combat-v1214aw',
    'takes': [{'take': TAKE.name, 'passed': result['passed'], 'failure': result['failure']}],
    'complete': False,
    'scope': '#16 stage 6: native mission end on Edge of the World (host), partner behaviour observed, lab mode, follow off.',
    'findings': [
        'Confirmed static (chapter 89): the mission ends through the carriage usable and scene, then the gameflow mission completion (result screen, institute.mapinfo); its LevelTransition nodes are debug only.',
        'Confirmed runtime: the host used the carriage from the east door (cinematic epoch 20 -> 22).',
        'Host logic sequence: ' + ' -> '.join(case['hostStates']) + '.',
        'Logic 9 is the leave-or-stay prompt (A leaves); logic 11 is the native result screen (A continues); 15/5 is the map transition; the Good Doctor intro scene (5) follows.',
        'Client logic never left 1 on eotw_p_lowchaos: the shared usable and mirrored triggers do not carry the mission end to the partner world.',
        'The Steam connection stayed fresh with the peers on different maps (remoteFresh true, connection 3 on both).',
        'Lab SLOT81 checkpoints unchanged; Dunwall fixture and protected files restored.',
        'Side effect: the host instance unlocked the Steam achievement "Alternative Approach" on the lab account (no casualties); lab instances share the main Steam account.',
    ],
    'next': [
        'Carry the mission end to the partner world natively: mirror gameflow task/objective/mission state changes (arkMsgGameflowTaskChangeState, arkMsgGameflowObjectiveChangeState, arkMsgGameflowMissionChangeState) so the partner completes its own mission, sees its own result screen and loads the next map with its own character state.',
        'Partner follows the host\'s chaos variant; both-ready barrier before the next mission starts.',
    ],
}
(ROOT / 'work/movement-integration/mission-end-audit-v1291.json').write_text(json.dumps(audit, indent=2) + '\n', encoding='utf-8')

state_path = ROOT / 'work/current-task-state.json'
state = json.loads(state_path.read_text(encoding='utf-8-sig'))
state['UpdatedAt'] = now
state['MissionTransitions'] = {
    'Card': 16,
    'Audit': 'work/movement-integration/mission-end-audit-v1291.json',
    'Plan': 'documentation/handoff-2026-10-11/97-campaign-map-sequence.md',
    'HostNativeMissionEnd': True,
    'PartnerFollowsMissionEnd': False,
    'NextAction': audit['next'][0],
}
state_path.write_text(json.dumps(state, indent=2) + '\n', encoding='utf-8')
print('ok')
