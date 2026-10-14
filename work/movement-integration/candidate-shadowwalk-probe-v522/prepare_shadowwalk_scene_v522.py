from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';S=R/'outputs/d2-coop-lab/src';N=R/'work/natural-moveset'
def once(s,a,b):
    assert s.count(a)==1,a
    return s.replace(a,b)
p=S/'runtime_probe.cpp';s=p.read_text(encoding='utf-8-sig')
s=once(s,'#include "native_player_shadowwalk_state.h"','#include "native_player_shadowwalk_state.h"\n#include "native_player_shadowwalk_scene.h"')
needle='            std::cout<<"}\\n";if(!ms)return valid?0:69;Sleep(15);'
s=once(s,needle,r'''            NativePlayerPowerScene scene{};const bool sceneValid=valid&&readNativePlayerShadowWalkScene(reader,base,s.states,scene);
            std::cout<<",\"sceneValid\":"<<(sceneValid?"true":"false");
            if(sceneValid)std::cout<<",\"variant\":"<<scene.variant<<",\"scene\":"<<scene.handle<<",\"timeMs\":"<<scene.timeMs<<",\"durationMs\":"<<scene.durationMs;
'''+needle)
p.write_text(s,encoding='utf-8')
p=S/'world_identity_tests.cpp';s=p.read_text(encoding='utf-8-sig')
s=once(s,'#include "native_player_shadowwalk_state.h"','#include "native_player_shadowwalk_state.h"\n#include "native_player_shadowwalk_scene.h"')
needle='                        form.d(pc+0xe4,4);'
s=once(s,needle,needle+r'''
                        {const auto rd=[&](auto a,auto b,auto n){return form.read(a,b,n);};NativePlayerStates states{};NativePlayerPowerScene scene{};
                         require(readNativePlayerStates(rd,base,states),"Shadow Walk form fixture state readable");
                         require(!readNativePlayerShadowWalkScene(rd,base,states,scene),"unobserved armed Shadow Walk form rejected");
                         auto holstered=form;holstered.q(inv+0x78,0);holstered.d(equipment+0x1c,0x20000002);
                        }
                        require(nativeShadowWalkVariant(3,"midnight/gameplay/player/base/powers/power_shadow_walk_trans_in.midnightscene","emily",1000)==32,"exact native Shadow Walk entry resource and clock");
                        require(nativeShadowWalkVariant(5,"midnight/gameplay/player/base/powers/power_shadow_walk_trans_out.midnightscene","Emily",1133)==34,"exact native Shadow Walk exit resource and clock");
                        require(!nativeShadowWalkVariant(4,"midnight/gameplay/player/base/powers/power_shadow_walk_trans_in.midnightscene","emily",1000),"entry resource cannot impersonate sustained form");
                        require(!nativeShadowWalkVariant(3,"midnight/gameplay/player/base/powers/power_shadow_walk_trans_in_crouch.midnightscene","emily",1000),"unobserved crouched entry not silently admitted");
''')
# Equipment storage names differ in this shared fixture; leave positive scene
# ownership to the native runtime comparison until its full synthetic fixture exists.
s=s.replace('                         auto holstered=form;holstered.q(inv+0x78,0);holstered.d(equipment+0x1c,0x20000002);\n','')
p.write_text(s,encoding='utf-8')
b=(W/'Build-ShadowWalkState-v519.ps1').read_text(encoding='utf-8-sig').replace('v519','v522').replace('probe519','probe522')
(W/'Build-ShadowWalkScene-v522.ps1').write_text(b,encoding='utf-8')
s=(N/'Observe-NativeShadowWalk-v520.ps1').read_text(encoding='utf-8-sig')
s=s.replace("'-t','28'","'-t','14'").replace("'27000'","'13000'").replace('-lt26000','-lt12000').replace('$back=$false;$entityNext=2000','$back=$false;$ended=$false;$entityNext=2000')
needle='  $entities=$null;if($w.ElapsedMilliseconds-ge$entityNext)'
s=once(s,needle,'''  if($form-and!$ended-and$w.ElapsedMilliseconds-ge7000){I client 255 220;$ended=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native LT220 End Shadow Walk after bounded movement'})}
'''+needle)
s=s.replace('$_.effects.records','$_.effects.effects')
needle=" $checks.Add('Strict native mode reader observes all recorded phases including exact alternate ShadowWalk form and retirement')"
s=once(s,needle,needle+'''
 $sceneRows=@($strict|Where-Object sceneValid)
 foreach($variant in 32,33,34){if(!@($sceneRows|Where-Object variant -eq $variant).Count){throw "Exact native scene variant $variant not observed"}}
 $sceneRows|Group-Object variant,durationMs|Select-Object Name,Count|ConvertTo-Json|Set-Content "$out/strict-scene-observations.json"
 if(!$ended){throw 'End Shadow Walk input was not issued'}
 $checks.Add('Owned native seven-binding entry and exit scenes plus sustained form observed; native LT request ends form')
''')
s=s.replace('natural exit','requested exit').replace('Natural Shadow Walk exit','Requested Shadow Walk exit')
(N/'Observe-NativeShadowWalk-v522.ps1').write_text(s,encoding='utf-8')
w=(W/'Run-ShadowWalkObserve-v520.ps1').read_text(encoding='utf-8-sig').replace('native-shadowwalk-state-v520','native-shadowwalk-scenes-v522').replace('Observe-NativeShadowWalk-v520','Observe-NativeShadowWalk-v522')
(W/'Run-ShadowWalkObserve-v522.ps1').write_text(w,encoding='utf-8')
print('Prepared native scene reader and short normal-input exit observation522; no production movement changes.')
