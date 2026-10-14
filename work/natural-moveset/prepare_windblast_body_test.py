"""Prepare an armed-standing Corvo test; world effect work remains separate."""
from pathlib import Path
P=Path(__file__).resolve().parent;s=(P/'Test-DarkVisionContexts.ps1').read_text(encoding='utf-8-sig')
def rep(a,b):
 global s
 assert s.count(a)==1,(a[:90],s.count(a));s=s.replace(a,b)
rep("[ValidateSet('host','client')][string]$Source='host'","[ValidateSet('host')][string]$Source='host'")
rep(",[switch]$Crouched,[switch]$Holstered,[switch]$Moving)",")\n$Crouched=$false;$Holstered=$false;$Moving=$false")
s=s.replace("-v26-","-v27-").replace('v26','v27').replace('version-eq26','version-eq27').replace('version-ne26','version-ne27')
rep("'darkvision-corvo-crouched-v27-v1'","'windblast-corvo-body-v27-v1'")
rep("$before.quantity-or$before.equipped-or$before.managerStarted","$before.quantity-or$before.equipped-or$before.started")
rep("$peerPower=Q $peer '--player-darkvision'","$peerPower=Q $peer '--player-darkvision'") # Explicitly retain the untouched observer probe below.
rep("if($power.managerStarted){$activated=$true}elseif($activated-and$next-eq2){$deactivated=$true}","")
rep("if(!$activated-or!$deactivated){throw 'Native Dark Vision did not toggle on and off'};$checks.Add('Native source vision overlay manager toggles on/off; observer vision and loadout remain untouched')","")
rep("if($peerPower.quantity-or$peerPower.managerStarted){throw 'Observer received caster-only Dark Vision'}","if($peerPower.quantity-or$peerPower.managerStarted){throw 'Observer native vision changed'}")
rep("quantity_selection_and_effect_inactive","quantity_and_selection_restored")
rep("$after.quantity-or$after.equipped-or$after.started-or$after.managerStarted","$after.quantity-or$after.equipped-or$after.started")
s=s.replace('darkvision-context-banks-v1/$character-darkvision-context-bank.bin','windblast-banks-v1/$character-windblast-bank.bin')
s=s.replace('coop_darkvision_fixture','coop_windblast_fixture').replace('.darkvision-fixture.json','.windblast-fixture.json').replace('native_darkvision_ready','native_windblast_ready')
s=s.replace("Q $Source '--player-darkvision'","Q $Source '--player-windblast'")
s=s.replace('.variant-eq10','.variant-eq11').replace('.powerVariant-eq10','.powerVariant-eq11')
s=s.replace('Dark Vision','Windblast').replace('DARKVISION','WINDBLAST')
rep("Native cleanup restores quantity, selection and inactive vision","Native cleanup restores original power quantity and selection")
rep("scope='Scoped native Windblast on/off casts in the explicitly recorded stance, sword and travel context. Source POV and remote whole-body observer, native private reveal, controls and camera. No remote damage, mana or VFX events. Airborne/other contexts and broader lifecycle remain unverified.'", "scope='Standing armed Corvo Windblast body integration only. Native source controller casts and scene timing drive the remote whole body through Steam. Native source effects and physics are recorded; remote world VFX, impacts, other contexts and broader lifecycle are not yet implemented or certified.'")
(P/'Test-WindblastBody.ps1').write_text(s,encoding='utf-8-sig');print('Prepared bounded Corvo body test')
