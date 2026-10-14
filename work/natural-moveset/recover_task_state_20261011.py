"""Restore the full prior status snapshot from this task's own command log."""
import json, hashlib, datetime
from pathlib import Path
P=Path(__file__).resolve().parent
log=Path(r'C:/Users/adako/.codex/sessions/2026/10/08/rollout-2026-10-08T09-23-15-01a11a65-16bb-74c3-a717-9821b03e3461.jsonl')
text=None
for i,line in enumerate(log.open(encoding='utf-8')):
    if i==68312:
        record=json.loads(line)
        assert record['type']=='event_msg'
        text=record['payload']['item']['stdout'];break
assert text
state,end=json.JSONDecoder().raw_decode(text)
assert state['UpdatedAt']=='2026-10-11T09:22:15.9648179+02:00'
assert isinstance(state['MovementIntegration'],dict) and len(state)>45
backup=P/'task-state-recovery-20261011'
backup.mkdir(exist_ok=False)
target=P.parent/'current-task-state.json'
(backup/'failed-update.json').write_bytes(target.read_bytes())
(backup/'prior-full-status.json').write_text(text[:end],encoding='utf-8')
temporary=target.with_suffix('.recovering.json')
temporary.write_text(json.dumps(state,indent=2),encoding='utf-8')
assert json.loads(temporary.read_text())==state
temporary.replace(target)
(backup/'provenance.json').write_text(json.dumps(dict(recovered_at=datetime.datetime.now().astimezone().isoformat(),
    source='This task command log, event 68312 full stdout; first complete JSON object.',
    snapshot_time=state['UpdatedAt'],snapshot_sha256=hashlib.sha256(text[:end].encode()).hexdigest(),
    recovered_top_level_keys=len(state),pending='Replay index_ranged_matrix_v29.py and index_crossbow_native_state_v26.py; update current candidate.'),indent=2))
print('Recovered',len(state),'top-level status fields; movement keys:',list(state['MovementIntegration']))
