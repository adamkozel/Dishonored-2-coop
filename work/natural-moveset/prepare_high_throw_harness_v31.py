from pathlib import Path
P=Path(__file__).resolve().parent
s=(P/'Record-CarrySettledStance-v30.ps1').read_text()
s=s.replace("'carry-pickup-bank-manifest-v1.json'","'high-throw-bank-manifest-v31.json'")
s=s.replace("if(!$Paired){throw 'Full paired case required'}", "if(!$Paired-or$CrouchedStart-or$CrouchedDrop){throw 'This experiment requires standing paired high throw'}")
s=s.replace("I X 500;$cancelMs=$clock.ElapsedMilliseconds", "& \"$lab/Send-D2Input.ps1\" -Role $Role -RightTrigger 255 -DurationMs 500|Out-Null;$cancelMs=$clock.ElapsedMilliseconds")
s=s.replace("nativeDrop=$true;", "nativeDrop=$false;nativeThrow=$true;")
s=s.replace("'Source native pickup, carry walk and drop completed'", "'Source native pickup, carry walk and high throw completed'")
s=s.replace("Source walks and requests native drop after holding;", "Source walks and requests native high throw using RT; source-clock remote release queues a single native event149 after physics handoff;")
s=s.replace("$nativeBefore=Q $Role '--player-states';", "$attachmentBefore=J $peer '.carry-attachment.json';$attachmentBefore|ConvertTo-Json -Depth 8|Set-Content \"$out/attachment-before.json\"\n $nativeBefore=Q $Role '--player-states';")
s=s.replace("$scope=if($Paired)", "(J $peer '.carry-attachment.json')|ConvertTo-Json -Depth 8|Set-Content \"$out/attachment-after.json\"\n $scope=if($Paired)")
(P/'Record-HighThrow-v31.ps1').write_text(s)
print('High-throw paired harness prepared')
