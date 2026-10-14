from pathlib import Path
R=Path(__file__).resolve().parents[2]; P=R/'work/natural-moveset'; W=R/'work/movement-integration'; S=R/'outputs/d2-coop-lab/src'
def edit(path,old,new):
 s=path.read_text();assert s.count(old)==1,(path,old);path.write_text(s.replace(old,new))
edit(S/'movement_bank.h','315,316,318};return v<=53?counts[v]:0;','315,316,318,319};return v<=54?counts[v]:0;')
edit(S/'movement_bank.h','v<=53;++v','v<=54;++v')
edit(S/'movement_bank_tests.cpp','(argc==1?1:53);format<=53','(argc==1?1:54);format<=54')
edit(S/'movement_bank_tests.cpp','        check(bank.hasPossessionRelease()', '''        check(bank.hasPossessionCrouchRelease()==(bank.version>=54),"crouched release requires bank54");
        if(bank.hasPossessionCrouchRelease())for(float t:{0.f,.5f,1.f}){
            for(unsigned j:{2u,9u,16u,17u,20u,24u})for(float low:{0.f,.5f,1.f})
                for(float x:bank.samplePossession(28,j,t,low))check(std::isfinite(x),"crouched release stays finite");
            if(argc==3){
                const auto a=bank.samplePossession(28,2,t,0),b=bank.samplePossession(28,2,t,1);
                check(b[6]<a[6]-.15f&&b[6]>.5f,"release preserves complete crouched pelvis");
                check(std::fabs(bank.clips[318].angle-.4f)<1e-6f,"crouched release uses native400ms");
            }
        }
        check(bank.hasPossessionRelease()''')
edit(S/'shadow_action_protocol_tests.cpp','        state.active[0].vtable++;check(!nativeHumanPossessionContext(state,base,1),"wrong native class refused");', '''        for(unsigned stance:{0u,1u}){
            state.player.current=stance;
            check(nativeHumanPossessionEntryContext(state,base)&&nativeHumanPossessionContext(state,base,1),"observed standing/crouched host entry accepted");
            auto bad=state;bad.player.current=2;check(!nativeHumanPossessionEntryContext(bad,base),"airborne host entry remains unverified");
            bad=state;bad.defaultMode=true;check(!nativeHumanPossessionEntryContext(bad,base),"default locomotion cannot authorize release");
            bad=state;bad.active.push_back({6,2,base+0x1c84258});check(!nativeHumanPossessionEntryContext(bad,base),"extra state cannot authorize host entry");
            bad=state;bad.active[0].vtable++;check(!nativeHumanPossessionEntryContext(bad,base),"wrong entry class refused");
        }
        state.active[0].vtable++;check(!nativeHumanPossessionContext(state,base,1),"wrong native class refused");''')
# Extend existing fully-owned dual-scene fixture with observed crouched timing.
edit(S/'world_identity_tests.cpp','require(!inspectEquipment(bad),"unobserved crouched dual draw remains refused");', '''require(!inspectEquipment(bad),"standing draw declaration cannot impersonate crouched exit");
                        bad.str(strings,"midnight/gameplay/player/base/combat/sword_equip_sneak.midnightscene");bad.d(scene+0x28,1133);
                        require(inspectEquipment(bad)&&result.variant==3,"crouched Corvo exit validates independent sword1133 and power333");
                        auto low=bad;low.d(scene+0x28,1066);require(!inspectEquipment(low),"crouched exit refuses standing timing");
                        low=bad;low.q(pb+8,arm);require(!inspectEquipment(low),"crouched exit refuses borrowed right arm");
                        low=bad;low.d(ps+0x24,123);require(!inspectEquipment(low),"crouched exit refuses aliased power scene");''')
# Preserve all existing318 clip payloads; append one400ms complete crouch release.
s=(P/'build_possession_crouch_v363.py').read_text()
s=s.replace("Library(['possession-bank-source-manifest-v323.json'],'possession-crouch-sources-v363')", "Library(['possession-bank-source-manifest-v323.json','possession-bank-source-manifest-v334.json'],'possession-crouch-release-sources-v371')")
s=s.replace("OUT=P/'possession-crouch-banks-v363'", "OUT=P/'possession-crouch-release-banks-v371'")
s=s.replace("P/'possession-banks-v334'/f'{role}-possession-bank.bin'", "P/'possession-crouch-banks-v363'/f'{role}-possession-crouch-bank.bin'")
s=s.replace("magic==b'D2MOVE52' and version==52 and count==316", "magic==b'D2MOVE53' and version==53 and count==318")
s=s.replace("b'D2MOVE53',53,n,318", "b'D2MOVE54',54,n,319")
s=s.replace("[('in',333),('loop',200)]", "[('out',400)]")
s=s.replace("   for j in right:pose[j]=rest[j].copy()", "   # Release is empty-handed; retain authored finger motion rather than armed grip.")
s=s.replace('316+offset','318+offset').replace('26+offset','28+offset')
s=s.replace("f'{role}-possession-crouch-bank.bin'", "f'{role}-possession-crouch-release-bank.bin'")
s=s.replace("P/'possession-crouch-banks-v363'/f'{role}-possession-crouch-release-bank.bin'", "P/'possession-crouch-banks-v363'/f'{role}-possession-crouch-bank.bin'")
s=s.replace("format='D2MOVE53',clips=318", "format='D2MOVE54',clips=319")
s=s.replace('possession-crouch-source-manifest-v363','possession-crouch-release-source-manifest-v371').replace('possession-crouch-bank-manifest-v363','possession-crouch-release-bank-manifest-v371')
s=s.replace('armed=True','armed=False').replace('Native361 supplies timing/context only.','Native370 supplies release400ms/crouched context only.').replace('sword grip/socket retained','empty-hand finger animation/socket retained')
s=s.replace('"""Whole-body crouched Possession entry/aim; original316 payloads retained.\n\nNative361 observes Sneaking6+UsePower63 with the same333/200ms arm scenes.\nOnly these observed phases are added. Successful release still uses the\nprevious standing native host-entry context. No root/gameplay/FX events.\n"""','"""Append observed400ms complete-body crouched release; original318 payloads retained. No root/gameplay/FX events."""')
(P/'build_possession_crouch_release_v371.py').write_text(s)
# New take keeps the prior failed result immutable; native exit retains crouch.
s=(P/'Test-PossessionCrouchTarget-v365.ps1').read_text()
s=s.replace('possession-crouch-bank-manifest-v363','possession-crouch-release-bank-manifest-v371').replace('possession-crouch-banks-v363/corvo-possession-crouch-bank.bin','possession-crouch-release-banks-v371/corvo-possession-crouch-release-bank.bin')
s=s.replace('version-eq53','version-eq54').replace('version-ne53','version-ne54').replace('bank53/318','bank54/319').replace('bank53','bank54').replace('316 prior clip','318 prior clip')
s=s.replace('$returned.stance-eq0','$returned.stance-eq1').replace('$_.movement.equipmentVariant-eq1','$_.movement.equipmentVariant-eq3')
s=s.replace('Native paired sword1066/power333','Native paired crouched sword1133/power333').replace('subsequent standing native exit','subsequent crouched native exit')
s=s.replace('Where-Object valid','Where-Object {$_.valid}')
s=s.replace("$_.variant-eq28-and$_.durationMs-eq400", "$_.stance-eq1-and$_.variant-eq28-and$_.durationMs-eq400")
(P/'Test-PossessionCrouchTarget-v371.ps1').write_text(s)
edit(W/'Invoke-NativeLabOperation.ps1','Test-PossessionCrouchTarget-v365.ps1','Test-PossessionCrouchTarget-v371.ps1')
s=(W/'Build-PossessionDraw-v370.ps1').read_text().replace('v370','v371').replace('Build370','Build371').replace('possession-draw','possession-crouch-release').replace('possession-crouch-banks-v363','possession-crouch-release-banks-v371').replace('possession-crouch-bank.bin','possession-crouch-release-bank.bin').replace('actual318','actual319')
(W/'Build-PossessionCrouchRelease-v371.ps1').write_text(s)
print('371 source, offline generator, owned-scene/context tests and runtime harness prepared')
