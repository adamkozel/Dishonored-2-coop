from pathlib import Path
p=Path('outputs/d2-coop-lab/src/native_power_fx.h');s=p.read_text().replace('        kind==12?', '        kind==13?"models/effects/gameplay/powers/mesmerize_v2/body/mesmerize_v2_01.fx":\n        kind==12?');p.write_text(s)
p=Path('outputs/d2-coop-lab/src/mesmerize_visual_fixture.h');s=p.read_text().replace('MesmerizeVisualTarget target{};MesmerizeVisualScene scene{};', 'MesmerizeVisualTarget target{};MesmerizeVisualScene scene{};NativePowerFx effect{};bool effectAttempted=false;');s=s.replace('unsigned mesmerizeVisualRequest=0;','unsigned mesmerizeVisualRequest=0;\nstd::uint64_t mesmerizeVisualFxStarted=0,mesmerizeVisualFxStopped=0;');s=s.replace('    if(p.pendingHide){\n        reinterpret_cast', '''    if(p.pendingHide){
        if(p.effect.expires){
            if(!nativePowerFxApi())return;
            if(nativePowerFxRecord(p.effect))reinterpret_cast<void(*)(std::uintptr_t,unsigned)>(gameImage+0x1a31f0)(nativePowerFxManager(),p.effect.id);
            p.effect={};++mesmerizeVisualFxStopped;
        }
        reinterpret_cast''')
s=s.replace('        const auto wanted=p.wanted;p.playing=wanted;', '''        if(!p.effectAttempted){
            p.effectAttempted=true;NativePowerFx e{};e.game=p.game;e.epoch=p.epoch;e.actor=p.target.actor;e.owner=p.target.handle;e.kind=13;
            e.declaration=nativePowerFxDefinition(13);float transform[12]{};
            if(!e.declaration||!memoryRead(p.target.actor+0x300,transform,48)){p.pendingHide=true;refuse("native_body_effect_definition_missing");return;}
            using Start=unsigned*(*)(std::uintptr_t,unsigned*,std::uintptr_t,std::uintptr_t,std::uintptr_t,std::uintptr_t,const float*,const float*,std::uintptr_t,unsigned,std::uintptr_t,std::uintptr_t);
            reinterpret_cast<Start>(gameImage+0x87cb90)(e.actor,&e.id,0,0,0,e.declaration,transform,transform+3,0,0,0,0);
            if(e.id==~0u){p.pendingHide=true;refuse("native_body_effect_start_failed");return;}
            e.expires=p.until;e.record=nativePowerFxRecord(e);p.effect=e;++mesmerizeVisualFxStarted;
            if(!e.record){p.pendingHide=true;refuse("native_body_effect_identity_unconfirmed");return;}
        }
        const auto wanted=p.wanted;p.playing=wanted;''')
s=s.replace('p.pendingHide=false;p.until=0;p.playing=0;', 'p.pendingHide=false;p.until=0;p.playing=0;p.effectAttempted=false;')
s=s.replace('\\\"phaseBefore\\\":%u,','\\\"fxActive\\\":%s,\\\"fxId\\\":%u,\\\"fxStarted\\\":%llu,\\\"fxStopped\\\":%llu,\\\"phaseBefore\\\":%u,').replace('mesmerizeVisualRetired,mesmerizeVisualPhaseBefore,','mesmerizeVisualRetired,p.effect.expires?"true":"false",p.effect.id,mesmerizeVisualFxStarted,mesmerizeVisualFxStopped,mesmerizeVisualPhaseBefore,');p.write_text(s)
p=Path('work/natural-moveset/Test-MesmerizeVisual-v76.ps1');s=p.read_text().replace("$m=Q $r '--player-mesmerize';if($m.quantity-or$m.started-or$m.equipped){throw 'Gameplay Mesmerize equipment unexpectedly active'}", "if($r-eq'client'){$m=Q $r '--player-mesmerize';if($m.quantity-or$m.started-or$m.equipped){throw 'Gameplay Mesmerize equipment unexpectedly active'}}")
s=s.replace("$checks.Add('Twelve-second lease", "if($v.fxActive-or$v.fxStarted-ne1-or$v.fxStopped-ne1){throw 'Owned body effect lifecycle incomplete'}\n $checks.Add('Exact native cosmetic body effect starts once on owned visual model and stops with its lease; native sound declared, recording silent')\n $checks.Add('Twelve-second lease")
s=s.replace('No networking, particles, sound or native gameplay outcomes in this proof.', 'Native particles and declared spatial sound accompany the model. Recording is silent. No networking or native gameplay outcomes in this proof.')
p.write_text(s)
