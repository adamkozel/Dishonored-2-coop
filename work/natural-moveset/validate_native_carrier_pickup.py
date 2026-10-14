"""Verify source-driven carrier playback, without treating it as paired pickup."""
import hashlib,json,subprocess,sys
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
for take in sys.argv[1:]:
    f=P/take;rows=read(f/'observations.json');identity=read(f/'identity.json');result=read(f/'result.json')
    source_lift=[r for r in rows if r['native'].get('carryScene',{}).get('variant')==1]
    # Retain separately timestamped receive state from before slow native reads
    # and attachment setup. No inference from playback or fabrication of packets.
    prep=[r for r in rows if r['observerStream']['carryRemote']==7 or r.get('observerStreamAtBegin',{}).get('carryRemote')==7]
    lift=[r for r in rows if r['movement']['carryVariant']==1 and r['movement']['carryWeight']>.5]
    loop=[r for r in rows if r['movement']['carryVariant']==2 and r['movement']['carryWeight']>.9]
    checks=dict(harness_passed=result['passed'],preparation_transmitted=bool(prep),native_hoist_observed=bool(source_lift),
        stream_ready_throughout=all(r['observerStream']['ready'] and r['sourceStream']['ready']for r in rows),
        no_stream_rearm=len({r['observerStream']['token']for r in rows})==1,
        steam_connected=all(r['observerRoot']['connectionState']==3 and r['sourceRoot']['connectionState']==3 for r in rows),
        bank_active_throughout=all(r['movement']['active'] and r['movement']['version']==identity.get('bankVersion',33) for r in rows),
        no_bank_refusals=len({r['movement']['refused']for r in rows})==1,
        unique_mapped_body=all(r['observerStream']['carryLocalHandle']==int(identity['corpse'],0)for r in rows if r['observerStream']['carryRemote']),
        one_source_corpse_generation=len({r['observerStream']['carryRemoteIncarnation']for r in rows if r['observerStream']['carryRemote']})==1,
        loaded_pose_reached=len(loop)>=5,
        health_preserved=all(identity['healthBefore'][k]['current']==identity['healthAfter'][k]['current']for k in ['source','observer','avatar']))
    if not identity['crouched'] or identity.get('bankVersion',33)>=35:
        gather=[r for r in rows if r['movement']['carryVariant']==7 and r['movement']['carryWeight']>.1]
        # Received semantics can already be at hoist while delayed body playback
        # still presents gather. Check playback order, not same-row packet state.
        checks.update(gather_played=bool(gather),
            gather_precedes_hoist=bool(gather)and bool(lift)and gather[0]['beginMs']<lift[0]['beginMs'],
            hoist_played=len(lift)>=5,semantic_holster_followed=bool(lift) and all(not r['movement']['presentedDrawn']for r in lift),
            native_holster_observed=any(r['equipment']['remoteDriven'] and r['equipment']['active'] and r['equipment']['result']=='native_holstered'for r in lift),
            authored_hoist_advances=bool(lift) and max(r['movement']['carryAuthoredAmount']for r in lift)>.9,
            hoist_never_rewinds=all(b['movement']['carryAuthoredAmount']+.001>=a['movement']['carryAuthoredAmount']for a,b in zip(lift,lift[1:])))
    else:checks['standing_lift_not_used_while_crouched']=not lift
    if identity.get('bankVersion',33)>=34 and (not identity['crouched'] or identity.get('bankVersion',33)>=35):
        values=[r['movement']['carrySettle']for r in loop]
        checks.update(supported_settle_observed=bool(values)and min(values)<.95,
            supported_settle_finishes=bool(values)and values[-1]==1,
            no_settle_replay_on_native_loop=bool(values)and all(b+.00001>=a for a,b in zip(values,values[1:])))
    if identity['crouched'] and identity.get('bankVersion',33)>=35:
        checks.update(native_crouched_hoist=bool(source_lift) and all(r['native']['stance']==1 for r in source_lift),
            authored_crouched_hoist=bool(lift) and all(r['movement'].get('carryPlaybackStance')==1 for r in lift),
            both_stance_hoists_have_separate_bank_clips=all(r['movement']['version']>=35 for r in lift))
    probe=json.loads(subprocess.check_output(['ffprobe','-v','error','-show_entries','stream=avg_frame_rate','-of','json',str(f/'native.mp4')]))
    a,b=map(int,probe['streams'][0]['avg_frame_rate'].split('/'));hz=a/b;checks['capture_at_least_24hz']=hz>=24
    transitions=[];prior=None
    for r in rows:
        state=(r['observerStream']['carryRemote'],r['movement']['carryVariant'])
        if state!=prior:transitions.append(dict(ms=r['beginMs'],remote=state[0],playing=state[1],amount=r['movement']['carrySampleAmount']));prior=state
    out=dict(passed=all(checks.values()),checks=checks,samples=len(rows),preparation_samples=len(prep),lift_samples=len(lift),loop_samples=len(loop),
        actual_capture_hz=hz,transitions=transitions,video_sha256=hashlib.sha256((f/'native.mp4').read_bytes()).hexdigest().upper(),
        scope=identity['scope'],complete=False,visual_accepted=False)
    (f/'validation.json').write_text(json.dumps(out,indent=2));print(json.dumps(dict(take=take,**out)))
    if not out['passed']:sys.exit(1)
