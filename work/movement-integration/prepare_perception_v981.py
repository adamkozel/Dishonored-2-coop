from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
s=(N/'Test-NpcNativePerception-v978.ps1').read_text(encoding='utf-8-sig').replace("'npc-native-perception-v978'","'npc-native-perception-v981'")
a=' C client ("setviewpos {0} {1} {2} {3} 0"-f$x,$y,($t[2]+1.7),$yaw)'
assert a in s;s=s.replace(a," C client 'setviewpos -42.9563 -122 62.7 90 0'\n # The same settled position that proved local visibility; no forward-axis assumption.")
a=' Start-Sleep -Milliseconds 600\n'
assert s.count(a)==1;s=s.replace(a,a+''' & C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/python/python.exe -X utf8 "$root/work/movement-integration/read_observable_v981.py" > "$out/observable-read.log"
 if($LASTEXITCODE){throw 'Visibility snapshot failed'}
''')
(N/'Test-NpcNativePerception-v981.ps1').write_text(s)
s=(R/'work/movement-integration/read_observable_v980.py').read_text().replace("out=W/'observable-readonly-v980'","out=W/'observable-readonly-v981'")
(R/'work/movement-integration/read_observable_v981.py').write_text(s)
