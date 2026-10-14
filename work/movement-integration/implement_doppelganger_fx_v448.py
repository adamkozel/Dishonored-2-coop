from pathlib import Path
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab/src'
p=S/'doppelganger_fx_scene.h';assert not p.exists()
s=(S/'mesmerize_visual_scene.h').read_text(encoding='utf-8-sig').replace('MesmerizeVisual','DoppelgangerFx').replace('mesmerizeVisual','doppelgangerFx')
s=s.replace('// Bounded local model experiment. No gameplay/AI or native scene event tracks.','// Cosmetic441 native FX/material tracks only; body pose/root stay network-owned.')
s=s.replace('struct DoppelgangerFxTarget {std::uintptr_t actor=0;unsigned handle=0;};','using DoppelgangerFxTarget=NativeDoppelgangerVisual;')
start=s.index(' return variant==1?');end=s.index('\n}',start)
s=s[:start]+' return variant==1?"midnight/d2coop/doppelganger_fx_spawn.midnightscene":variant==2?"midnight/d2coop/doppelganger_fx_despawn.midnightscene":nullptr;'+s[end:]
s=s.replace('"Mesmerize"','"AIGroup1"').replace('elapsed>700','elapsed>=doppelgangerFxDuration(variant)')
s=s.replace('if(!s.handle||!s.declaration||worldIdentityEntity(memoryRead,observedGame,target.handle)!=target.actor)return false;',
    'if(!s.handle||!s.declaration||!currentDoppelgangerVisual(target,observedGame))return false;')
s=s.replace('if(s.handle||!resource||elapsed>=doppelgangerFxDuration(variant)||!carryVictimApis()',
    'if(s.handle||!resource||elapsed>=doppelgangerFxDuration(variant)||!currentDoppelgangerVisual(target,observedGame)||!carryVictimApis()')
p.write_text(s,encoding='utf-8')

p=S/'doppelganger_world_stream.h';s=p.read_text(encoding='utf-8-sig')
old='r.sequence,r.event,r.digest,r.handle,r.generation,r.present);'
new='r.sequence,r.event,r.digest,r.handle,r.generation,r.present,r.fxKnown,r.fxScene,r.fxVariant,r.fxTimeMs);'
assert s.count(old)==1;s=s.replace(old,new)
old=r'\"present\":%u}",i?",":"",r.sequence';new=r'\"present\":%u,\"fxKnown\":%u,\"fxScene\":%u,\"fxVariant\":%u,\"fxTimeMs\":%u}",i?",":"",r.sequence'
assert s.count(old)==1;s=s.replace(old,new)
old='fprintf(f,"],\\\"poseReadMaxMs'
# Add independently measured scene-reader cost without rewriting the existing pose status.
anchor='    fprintf(f,"],\\\"poseReadMaxMs';assert s.count(anchor)==1
s=s.replace(anchor,'    fprintf(f,"],\\\"sceneReadMaxMs\\\":%.4f,\\\"sceneReadLine\\\":%u,\\\"sceneUnknown\\\":%llu,\\\"fxKnown\\\":%u,\\\"fxScene\\\":%u,\\\"fxVariant\\\":%u,\\\"fxTimeMs\\\":%u",doppelgangerSceneReadMaxMs,doppelgangerSceneReadLine,doppelgangerSceneUnknown,p.fxKnown,p.fxScene,p.fxVariant,p.fxTimeMs);\n    fprintf(f,",\\\"poseReadMaxMs')
p.write_text(s,encoding='utf-8')

p=S/'doppelganger_visual.h';s=p.read_text(encoding='utf-8-sig')
anchor='void applyDoppelgangerCloneBody';assert s.count(anchor)==1
s=s.replace(anchor,'#include "doppelganger_fx_scene.h"\n#include "doppelganger_fx_presentation.h"\n'+anchor)
anchor='    if(!active){clearDoppelgangerCloneLease();';assert s.count(anchor)==1
s=s.replace(anchor,'    if(!active){serviceDoppelgangerFx(tick,p.target,p.game,p.epoch,false);clearDoppelgangerCloneLease();')
anchor='    doppelgangerVisualMaxMs=(std::max)';assert s.count(anchor)==1
s=s.replace(anchor,'    serviceDoppelgangerFx(tick,p.target,p.game,p.epoch,true);\n'+anchor)
anchor=r'doppelgangerClonePublication.sequence.load());const bool ok=!ferror(f);'
assert s.count(anchor)==1
s=s.replace(anchor,r'doppelgangerClonePublication.sequence.load());writeDoppelgangerFx(f);fputs("}\n",f);const bool ok=!ferror(f);')
old=r'\"publicationSequence\":%llu}\n"';new=r'\"publicationSequence\":%llu"'
assert s.count(old)==1;s=s.replace(old,new)
p.write_text(s,encoding='utf-8')
print('448 native scene helper and source/receiver status wiring prepared')
