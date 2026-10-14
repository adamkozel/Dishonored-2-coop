"""Index read-only native references without claiming remote animation playback."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parent.parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest().upper()
def write(p,value):
    temp=p.with_suffix(p.suffix+'.tmp');temp.write_text(json.dumps(value,indent=2),encoding='utf-8');temp.replace(p)
manifest=read(P/'runtime-video-manifest.json');cases={}
for character in ['corvo','emily']:
    take=f'native-carry-{character}-reader-v2';folder=P/take;result=read(folder/'result.json')
    assert result['passed'] and not result['remote_gameplay_bound'] and not result['visual_accepted']
    assert result['video_sha256']==sha(folder/'native.mp4') and result['actual_capture_hz']>=24
    review=read(folder/'source-review.json');assert review['inspected'] and not review['remote_visual_accepted']
    assert review['sheet_sha256']==sha(folder/'native-sheet.png')
    video=take+'/native.mp4'
    row=dict(video=video,character=character,label=character.title()+' · Native carry source reference · remote binding pending',alias='native_carry_source_reference',native_source_only=True,
             actual_capture_hz=result['actual_capture_hz'],timing='Actual elapsed capture timing; no speedup',travelling=False,gameplay_bound=False,visual_accepted=False,
             scope='Original first-person native pickup, standing/crouched carrying and release. Validates scene ownership and timing for the future remote port. This recording does not show the authored remote movement or synchronized corpse attachment.',
             requirements=['carry_body_lift_idle_move_drop_throw'],evidence=take+'/result.json',source_review=take+'/source-review.json',proxy_sha256=result['identity']['proxySha256'],probe_sha256=result['identity']['probeSha256'])
    manifest=[r for r in manifest if r['video']!=video];manifest.append(row)
    cases[take]=dict(result=take+'/result.json',source_review=take+'/source-review.json',samples=result['samples'],accepted_samples=result['accepted_samples'],checks=len(result['checks']),variants=result['variants'],video=video,reader_sha256=result['identity']['readerSha256'])
findings=dict(complete=False,remote_gameplay_bound=False,visual_accepted=False,cases=cases,
 confirmed=['Exact native CarryCorpse45 and DropCorpse46 ownership chains decoded and checked against live scenes for both characters.',
            'Carry45+F8 is the source corpse generation; +110 phases1/2/3 are equipment preparation/pickup/carry; +24 owns the current scene.',
            'Native carry walking changes when horizontal velocity squared exceeds1; its1600ms scene is distinct from the3000ms idle.',
            'Corvo loop/walk uses Scene_2; Emily uses Scene_1. Both native standing and crouched carry walking were recorded.',
            'Drop46 copies the corpse to+38 and owns a separate scene. Open-space Corvo low_stand(kind0) and Emily low_sneak(kind1) passed reader replay.',
            'Near-wall Emily observation selected fetal_pose(kind4); this variant is decoded and unit-tested but not included in the final two replay cases.',
            'One Corvo idle scene retired between serial source and independent scene probes; the following sample owns walk. This boundary is recorded and excluded from matching coverage.'],
 remaining=['Remote body/corpse identity mapping, native third-person attachment and authoritative physical handoff.',
            'Whole-body pickup/contact alignment, crouched carrying clips, directional carrying and equipment transitions.',
            'High/soft releases and throwing require native observation and dedicated runtime bindings.',
            'Shared-target contention, interruption, death, reload, map transition and peer-loss cleanup.',
            'Full-body visual comparison and final acceptance for both characters.'],
 ghidra=dict(program='Dishonored2.exe',carry_vtable='141c77748',drop_vtable='141c790c8',carry_enter='140829030',carry_update='14082a110',carry_speed_test='1408297b0',drop_enter='140831fe0',drop_select='140832770'),
 source_reader='outputs/d2-coop-lab/src/native_player_carry_scene.h',tests='work/movement-integration/world-tests-carry-reader-v2.log',archive='work/movement-integration/verified-native-carry-reader-v2')
annotations=read(P/'review-evidence-annotations.json');item=annotations['items']['carry_body_lift_idle_move_drop_throw']
item['native_source_evidence']={'findings':'native-carry-reader-findings-v2.json','cases':list(cases),'remote_gameplay_bound':False}
note='Native first-person source reference for pickup, standing/crouched carry-walking and release is now recorded for both characters; ownership/timing reader verified. Remote corpse attachment and authored carry playback are still incomplete.'
if note not in item['notes']:item['notes']+=' '+note
item['runtime_binding']='not_verified'
state=read(ROOT/'work/current-task-state.json');movement=state['MovementIntegration']
movement['NativeCarrySource']=dict(complete=False,remoteGameplayBound=False,visualAccepted=False,readerVerified=True,findings='work/natural-moveset/native-carry-reader-findings-v2.json',sourceCases=list(cases),sourceChecks=sum(c['checks'] for c in cases.values()),next='Bind authored carrier and native corpse as one owned remote pair; preserve first-person controls and network-owned gameplay outcomes.')
movement['LatestContinuation']['actual_game_videos']=len(manifest)
movement['LatestContinuation']['next']='Carry source reader verified for both roles. Continue remote paired-body ownership and authored carry binding, then remaining movement contract. Native reference videos are not remote playback proof; complete movement remains false.'
# Avoid presenting native first-person evidence as pose publication or network playback.
html=P/'review.html';text=html.read_text(encoding='utf-8-sig')
old="$('coverage').textContent=r.travelling?"
new="$('coverage').textContent=r.native_source_only?'Native first-person source reference. Remote body and corpse playback are not shown.':r.travelling?"
if new not in text:
    assert text.count(old)==1;text=text.replace(old,new)
    temp=html.with_suffix('.html.tmp');temp.write_text(text,encoding='utf-8');temp.replace(html)
write(P/'runtime-video-manifest.json',manifest);write(P/'native-carry-reader-findings-v2.json',findings)
write(P/'review-evidence-annotations.json',annotations);write(ROOT/'work/current-task-state.json',state)
print(json.dumps(dict(cases=len(cases),source_checks=sum(c['checks'] for c in cases.values()),source_samples=sum(c['samples'] for c in cases.values()),game_recordings=len(manifest),remote_gameplay_bound=False)))
