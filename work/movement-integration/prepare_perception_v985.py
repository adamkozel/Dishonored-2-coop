from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
s=(N/'Test-NpcNativePerception-v984.ps1').read_text().replace('npc-native-perception-v984','npc-native-perception-v985')
s=s.replace("$boundHandles.Contains([Convert]::ToUInt32($_.handle.Replace('0x',''),16))", "([Convert]::ToUInt32($_.handle.Replace('0x',''),16) -in $boundHandles)")
a=' $avatar=(S host).probeActorHandle;';assert a in s;s=s.replace(a," if($enemyHandles.Count-lt3){throw 'At least three active native enemy identities required'}\n Save \"$out/watched-enemies.json\" $enemyHandles\n"+a)
a=" C client 'setviewpos -42.9563 -122 62.7 90 0'\n # The same settled position that proved local visibility; no forward-axis assumption.\n Start-Sleep -Milliseconds 600\n & C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/python/python.exe -X utf8 \"$root/work/movement-integration/read_observable_v984.py\" > \"$out/observable-read.log\"\n if($LASTEXITCODE){throw 'Visibility snapshot failed'}\n"
assert a in s;s=s.replace(a,'')
a=' $watch.Restart();$selected=0;$attacking=0';assert a in s;s=s.replace(a," C client 'setviewpos -42.9563 -122 62.7 90 0'\n"+a)
s=s.replace('if($selected-lt3)','if($selected-lt1)')
s=s.replace('No native enemy sustained remote-avatar combat target in this fixture','No native enemy selected the remote avatar during this bounded encounter')
(N/'Test-NpcNativePerception-v985.ps1').write_text(s)
