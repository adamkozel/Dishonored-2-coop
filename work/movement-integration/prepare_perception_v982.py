from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
s=(N/'Test-NpcNativePerception-v981.ps1').read_text(encoding='utf-8-sig').replace('npc-native-perception-v981','npc-native-perception-v982').replace('read_observable_v981','read_observable_v982')
a=" foreach($r in 'host','client'){C $r 'coop_shadow_stream on'}"
assert s.count(a)==1
s=s.replace(a,a+'''\n # Native SetTransform requeues observable removal/addition on every call.
 # Freeze only the owned remote presentation after arrival to test whether
 # repeated teleports starve native perception. AI remains safely held.
 C host 'coop_probe_follow off'
 Save "$out/paused-owned-follow.json" (S host)
''')
s=s.replace("nativeOverrideUsed=$false;scope=", "nativeOverrideUsed=$false;followPausedForDiagnostic=$true;scope=")
s=s.replace("'Bounded native enemy perception of remote player, no custom faction override or enemy damage relay.'", "'Diagnostic only: owned remote follow paused after placement; no relationship override or enemy damage relay. Does not accept continuously moving co-op perception.'")
(N/'Test-NpcNativePerception-v982.ps1').write_text(s)
s=(R/'work/movement-integration/read_observable_v981.py').read_text().replace("out=W/'observable-readonly-v981'","out=W/'observable-readonly-v982'")
(R/'work/movement-integration/read_observable_v982.py').write_text(s)
