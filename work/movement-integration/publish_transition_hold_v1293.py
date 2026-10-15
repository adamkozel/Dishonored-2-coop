"""Publish the both-ready transition hold takes (1291c on 1214ay, 1291d on 1214az): audit JSON and task state."""
import json
from datetime import datetime, timezone, timedelta
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
NM = ROOT / 'work/natural-moveset'
now = datetime.now(timezone(timedelta(hours=2))).isoformat()


def take(name):
    d = NM / name
    r = json.loads((d / 'result.json').read_text(encoding='utf-8-sig'))
    hold = {k: json.loads((d / f'missionend-{k}.json').read_text(encoding='utf-8-sig'))['hold'] for k in ('host', 'client')}
    timeline = json.loads((d / 'timeline.json').read_text(encoding='utf-8-sig'))
    stalls = {k: sum(1 for x in timeline if x[k].get('responding') is False) for k in ('host', 'client')}
    return r, hold, stalls, len(timeline)


c, ch, cs, cn = take('mission-end-hold-v1291c')
d, dh, ds, dn = take('mission-end-hold-v1291d')
audit = {
    'at': now,
    'candidates': ['candidate-npc-combat-v1214ay', 'candidate-npc-combat-v1214az'],
    'takes': [
        {'take': 'mission-end-hold-v1291c', 'candidate': 'candidate-npc-combat-v1214ay', 'passed': c['passed'], 'acceptance': False,
         'failure': 'host window Not Responding in %d of %d samples during the hold' % (cs['host'], cn)},
        {'take': 'mission-end-hold-v1291d', 'candidate': 'candidate-npc-combat-v1214az', 'passed': d['passed'], 'acceptance': True, 'failure': None},
    ],
    'complete': False,
    'scope': '#16 stage 6: both-ready hold at the next mission start after a mirrored mission end; Emily presses Continue 20 s after Corvo.',
    'findings': [
        'Confirmed runtime: the next-mission load ends with the native FinishLoad switch 15 -> 1 (caller 0x7879cd) while the intro scene is pending (5).',
        '1214ay (1291c): Corvo held %d ms until Emily reported %s; Emily released at once (%d ms). Both entered the intro in the same sample and reached gameplay together.' % (ch['host']['lastWaitMs'], ch['host']['peerMap'], ch['client']['lastWaitMs']),
        '1291c defect: the holding thread owns the game window, so Windows flagged it Not Responding (%d of %d samples).' % (cs['host'], cn),
        '1214az (1291d): peeking the queue without removing every 500 ms while holding (%d peeks) kept the window responsive (%d of %d samples not responding); hold %d ms; both reached gameplay in the same sample.' % (dh['host']['peeks'], ds['host'], dn, dh['host']['lastWaitMs']),
        'Visual (accepted): while held, Corvo sees the native black load-end frame with the skip hint; from the intro on, both screens match.',
    ],
    'next': [
        'Partner loads the host chaos variant (each world picks institute_p_low/highchaos from its own chaos).',
        'Client-originated mission end; product mode across the VM; boat and in-mission LevelTransition boundaries (chapter 97).',
    ],
}
(ROOT / 'work/movement-integration/transition-hold-audit-v1293.json').write_text(json.dumps(audit, indent=2) + '\n', encoding='utf-8')

state_path = ROOT / 'work/current-task-state.json'
state = json.loads(state_path.read_text(encoding='utf-8-sig'))
state['UpdatedAt'] = now
state['MissionTransitions'].update({
    'Audit': 'work/movement-integration/transition-hold-audit-v1293.json',
    'Candidate': 'candidate-npc-combat-v1214az',
    'BothReadyHold': True,
    'NextAction': ' '.join(audit['next']),
})
state_path.write_text(json.dumps(state, indent=2) + '\n', encoding='utf-8')
print('ok')
