from pathlib import Path
import shutil
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab/src';W=R/'work/movement-integration';N=R/'work/natural-moveset';B=W/'trap-recovery-pre-v575';B.mkdir(exist_ok=False)
def edit(name,fn):
 p=S/name;shutil.copy2(p,B/name);p.write_text(fn(p.read_text()))
edit('native_player_trap_scene.h',lambda s:s.replace('        if(duration==333&&asset==prefix+"ooa.midnightscene")return 6;', '        if(duration==333&&asset==prefix+"ooa.midnightscene")return 6;\n        if(duration==866&&asset==prefix+"reload.midnightscene")return 7; // Native Emily572 binds arm+player+item even after stock reaches0.')
 .replace('Reload/ready-out/slide','Ready-out/slide').replace('s.variant==5?1u:0u','(s.variant==5||s.variant==7)?1u:0u').replace('(s.variant==5||s.variant==6)?','(s.variant==5||s.variant==6||s.variant==7)?'))
edit('trap_presentation.h',lambda s:s.replace('300,333};return v<=6?', '300,333,866};return v<=7?').replace('s.variant<=6','s.variant<=7').replace('variant>6','variant>7'))
edit('trap_prop.h',lambda s:s.replace('variant>6','variant>7'))
edit('shadow_action_protocol.h',lambda s:s.replace('version=61','version=62').replace('p.version==61','p.version==62'))
edit('movement_bank.h',lambda s:s.replace('32+347*','32+348*').replace('340,347};return v<=61?','340,347,348};return v<=62?').replace('v<=61;++v','v<=62;++v').replace('bool hasTrap()const{return version>=61;}','bool hasTrap()const{return version>=62;}'))
edit('shadow_action_protocol_tests.cpp',lambda s:s.replace('for(unsigned v=1;v<=6;++v){auto phase=trap;', 'for(unsigned v=1;v<=7;++v){auto phase=trap;').replace('bad.version=60;check(!validShadowAction(bad,c,h),"pre-trap wire refused")','bad.version=61;check(!validShadowAction(bad,c,h),"pre-recovery trap wire refused")'))
s=(N/'build_trap_bank_v569.py').read_text().replace('trap-banks-v569','trap-banks-v575').replace('trap-bank-manifest-v569','trap-bank-manifest-v575').replace('D2MOVE61','D2MOVE62').replace('61,n,347','62,n,348').replace("'place','ooa'","'place','ooa','reload'").replace('version=61,clips=347','version=62,clips=348').replace('[2][7][120]','[2][8][120]').replace('Bank61/347','Bank62/348')
# Recreate only the bank/track data. The already installed static asset bytes stay identical.
s=s.replace(' assert bank[32:len(old)]==old[32:];'," prior=(P/'trap-banks-v569'/f'{role}-trap-bank.bin').read_bytes();assert bank[32:len(prior)]==prior[32:];assert bank[32:len(old)]==old[32:];")
(N/'build_trap_bank_v575.py').write_text(s)
b=(W/'Build-Traps-v571.ps1').read_text().replace('v571.log','v575.log').replace('Build571','Build575').replace('trap-banks-v569','trap-banks-v575').replace('347 clip','348 clip');(W/'Build-Traps-v575.ps1').write_text(b)
s=(W/'stage_traps_v571.py').read_text().replace("A=W/'candidate-trap-network-v571';P=W/'candidate-trap-network-v569'", "A=W/'candidate-trap-network-v575';P=W/'candidate-trap-network-v571'").replace("f'shadowwalk-body-{tag}-v571.log'","f'shadowwalk-body-{tag}-v575.log'").replace('Build-Traps-v571.ps1','Build-Traps-v575.ps1').replace('trap-banks-v569','trap-banks-v575').replace('protocol=61,bankVersion=61,clips=347','protocol=62,bankVersion=62,clips=348').replace('Corrects clock tuple start to15c/local160 following failed570. Runtime572 verification pending.','Preserves corrected native clock and all prior347 clips; adds observed866ms reload/recovery branch from Emily572. Runtime576 verification pending.')
s=s.replace("W/'implement_traps_v569.py'", "W/'extend_trap_recovery_v575.py'")
(W/'stage_traps_v575.py').write_text(s)
s=(W/'Deploy-Traps-v571.ps1').read_text().replace('v571','v575').replace('TrapDeploy571','TrapDeploy575');(W/'Deploy-Traps-v575.ps1').write_text(s)
s=(N/'Test-TrapNetwork-v570.ps1').read_text().replace('TrapNetwork570','TrapNetwork576').replace('trap-network-v570','trap-network-v576').replace('trap-bank-manifest-v569','trap-bank-manifest-v575').replace('-eq61','-eq62').replace('-ne61','-ne62').replace('bank61/347','bank62/348').replace('Reload, ready-out, surface contact','Ready-out, surface contact')
s=s.replace(' foreach($variant in 0,1,2,3,4,5,6){', ''' # Require every recovery branch actually played by the native source. Do
 # not demand an empty-stock scene when the game chose its reload recovery.
 $recoveries=@();if(@($scenes|Where-Object asset -like '*_ooa.midnightscene').Count){$recoveries+=6};if(@($scenes|Where-Object asset -like '*_reload.midnightscene').Count){$recoveries+=7}
 if(!$recoveries.Count){throw 'Native post-placement recovery not observed'}
 foreach($variant in (@(0,1,2,3,4,5)+$recoveries)){''')
(N/'Test-TrapNetwork-v576.ps1').write_text(s)
print('Added actual observed866ms native recovery, preserved prior347 clips, and require whichever native recovery scenes actually occur.')
