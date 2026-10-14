from pathlib import Path
import json,datetime,subprocess,sys
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
cand={'use-interaction-v1214':'candidate-npc-combat-v1213','use-interaction-v1214b':'candidate-npc-combat-v1213','use-interaction-v1214c':'candidate-npc-combat-v1213b'}
cases=[]
for take,c in cand.items():
    r=rd(N/take/'result.json');assert rd(N/take/'restoration.json')['passed'],take
    cases.append(dict(take=f'../natural-moveset/{take}',candidate=c,passed=r['passed'],failure=r.get('failure'),cases=r.get('cases')))
assert cases[-1]['passed']
C=rd(W/'candidate-npc-combat-v1213b/identity.json')
review={('use-interaction-v1214','use-host-observed.mp4'):(False,'Client view of remote Corvo (host source, 1213): Corvo leans into the window with head lowered while it swings open, then straightens (strip inspected). Backlit; hand contact not verifiable.'),
        ('use-interaction-v1214c','use-client-observed.mp4'):(True,'Host view of remote Emily (client source, 1213b): Emily steps to the window and extends her arm to it as it swings open, then steps back (strip inspected). Generic reach stand-in; hand not targeted at the handle.'),
        ('use-interaction-v1214c','use-host-observed.mp4'):(False,'Client view of remote Corvo (host source, 1213b): presentation ended at 24% of the clip (source ended the use early).')}
videos=rd(N/'runtime-video-manifest.json')
for take in cand:
    T=N/take
    for f in sorted(T.glob('*.mp4')):
        meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=avg_frame_rate','-of','json',str(f)],text=True))['streams'][0];a,b=map(float,meta['avg_frame_rate'].split('/'))
        path=f'{take}/{f.name}';src='host' if '-host-' in f.name else 'client';obs='client' if src=='host' else 'host'
        acc,rv=review.get((take,f.name),(False,'Raw native recording.'))
        videos=[x for x in videos if x['video']!=path]
        videos.append(dict(video=path,role=obs,character='corvo' if obs=='host' else 'emily',label=f'Interaction / native use -> authored reach (source {src}) / {take} / {f.name}',travelling=False,gameplay_bound=True,actual_capture_hz=round(a/b,2) if b else None,container_fps=meta['avg_frame_rate'],timing='Native wall-time recording, no speedup',
            scope='Observer window of the remote partner while the source player natively uses the Dunwall window handle (player state 75).',review=rv,candidate=cand[take],dll=rd(W/cand[take]/'identity.json')['dll'],runtime_test_passed=rd(T/'result.json')['passed'],visual_accepted=acc,requirements=['lever_button_crank_wheel'] if acc else []))
wr(N/'runtime-video-manifest.json',videos)
at=datetime.datetime.now().astimezone().isoformat()
wr(W/'use-interaction-audit-v1215.json',dict(at=at,candidate='candidate-npc-combat-v1213b',dll=C['dll'],bank=dict(version=71,clips=374,manifest='../natural-moveset/use-bank-manifest-v1212.json',added='369 button/generic, 370 lever, 371 crank, 372 wheel/valve, 373 key (mechanisms-v4 authored sequences)'),cases=cases,
  findings=['Confirmed runtime (1214c, both roles): a native use (player state 75, one engine frame) is detected at the source with the focused entity (interaction sensor +0x80) and plays the authored generic interaction (clip 369) on the partner body through protocol 74; Emily reach visually accepted.',
            '1213 sampled state 75 only at the 33 ms packet cadence and missed uses (1214b); 1213b samples every engine frame.',
            'Host-source presentation in 1214c ended at 24% of the clip (the source ends a use on any other action or >0.6 m movement); cause not isolated.'],
  complete=False,outstanding=['Lever/crank/wheel/key variants are classified by entity name and loaded (clips 370-373) but untested on real levers/cranks/wheels/locks.','Early end of the host-source use (24%).','Hand targeting to the actual handle/lever geometry (clips are stand-ins).']))
p=N/'review-evidence-annotations.json';an=rd(p)
it=an['items']['lever_button_crank_wheel'];it['runtime_binding']='partial_native_use_state75_generic_reach_both_roles'
it['notes']=(it.get('notes') or '')+' 1213b/1214c (audit 1215): native use (state 75 + focused entity) drives the authored interaction on the partner body through Steam (protocol 74, bank 71 clips 369-373), both roles; generic reach on a Dunwall window handle verified, Emily visual accepted. Lever/crank/wheel variants classified by name but untested on real mechanisms; host-source clip ended early once.'
it.setdefault('evidence',[]).append('../movement-integration/use-interaction-audit-v1215.json')
it=an['items']['key_use'];it['notes']=(it.get('notes') or '')+' 1213b (audit 1215): key/lock uses map to clip 373 by focused entity name through the same native use source; not yet runtime-tested on a real lock.'
it.setdefault('evidence',[]).append('../movement-integration/use-interaction-audit-v1215.json')
wr(p,an)
C.update(runtimeVerified=True,scopedUseInteractionVerified=True,runtimeEvidence='../use-interaction-audit-v1215.json',result='1214c pass both roles: native use -> authored generic interaction on the partner body.')
wr(W/'candidate-npc-combat-v1213b/identity.json',C)
s=rd(R/'work/current-task-state.json');s['UpdatedAt']=at
s['MovementExecutionPriority']['current']='1213b live (protocol 74, bank 71/374): native use (state 75) drives authored interactions on the partner body, both roles (1214c). 1203f product presentation recovery and lifecycle verified. Window traversal/counter/knockdown investigated (audits 1207, 1211).'
s['MovementExecutionPriority']['next']='Stage 3: real lever/crank/wheel/lock tests for variants 370-373; cutscene_enter_exit; counter/knockdown provocation; remaining unbound items. Release banks/product package must move to bank 71/protocol 74.'
wr(R/'work/current-task-state.json',s)
out=subprocess.run([sys.executable,'-X','utf8',str(N/'build_review_catalog.py')],cwd=N,capture_output=True,text=True);print(out.stdout[-200:],out.stderr[-300:]);assert out.returncode==0
print('ok')
