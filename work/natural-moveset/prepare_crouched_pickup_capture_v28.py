from pathlib import Path
P=Path(__file__).resolve().parent
t=(P/'Record-PickupAndDrop-v27.ps1').read_text()
t=t.replace('[switch]$CrouchedDrop,','[switch]$CrouchedDrop,[switch]$CrouchedStart,')
old=" $nativeBefore=Q $Role '--player-states';if($nativeBefore.carrySceneValid){throw 'Source already holds a body'}"
new=""" if($CrouchedStart){
  $attempts=[Collections.Generic.List[object]]::new()
  for($attempt=0;$attempt-lt3;$attempt++){
   $st=Q $Role '--player-states';$attempts.Add($st);if($st.stance-eq1){break}
   I B 180;Start-Sleep -Milliseconds 650
  }
  $st=Q $Role '--player-states';if($st.stance-ne1){throw 'Native crouch entry failed'}
  $st|ConvertTo-Json -Depth 12|Set-Content "$out/crouched-before.json"
  $attempts|ConvertTo-Json -Depth 12|Set-Content "$out/crouch-entry-attempts.json"
 }
"""+old
assert old in t;t=t.replace(old,new)
t=t.replace('$CrouchedDrop-and!$crouchSent','($CrouchedDrop-ne$CrouchedStart)-and!$crouchSent')
t=t.replace('crouched=$false;nativeDrop=', 'crouched=[bool]$CrouchedStart;nativeDrop=')
(P/'Record-PickupAndDrop-v28.ps1').write_text(t)
p=P/'validate_native_carrier_pickup.py';t=p.read_text()
t=t.replace("if not identity['crouched']:","if not identity['crouched'] or identity.get('bankVersion',33)>=35:")
t=t.replace("if identity.get('bankVersion',33)>=34 and not identity['crouched']:","if identity.get('bankVersion',33)>=34 and (not identity['crouched'] or identity.get('bankVersion',33)>=35):")
old="    probe=json.loads(subprocess.check_output"
new="""    if identity['crouched'] and identity.get('bankVersion',33)>=35:
        checks.update(native_crouched_hoist=bool(source_lift) and all(r['native']['stance']==1 for r in source_lift),
            authored_crouched_hoist=bool(lift) and all(r['movement'].get('carryPlaybackStance')==1 for r in lift),
            both_stance_hoists_have_separate_bank_clips=all(r['movement']['version']>=35 for r in lift))
"""+old
assert old in t;t=t.replace(old,new);p.write_text(t)
p=P/'validate_pickup_drop.py';t=p.read_text().replace("scope='One continuous native-input standing pickup,", "scope='One continuous native-input '+('crouched' if i['crouched'] else 'standing')+' pickup,")
p.write_text(t)
print('CROUCHED_CAPTURE_AND_VALIDATORS_READY')
