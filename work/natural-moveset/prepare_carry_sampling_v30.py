from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'Record-CarrySettledStance-v29.ps1').read_text(encoding='utf-8-sig')
s=s.replace("$begin=$clock.ElapsedMilliseconds;$st=Q $Role '--player-states'", "$begin=$clock.ElapsedMilliseconds;$streamAtBegin=J $peer '.shadow-stream.json';$st=Q $Role '--player-states'")
s=s.replace('beginMs=$begin;endMs=$clock.ElapsedMilliseconds;native=$st;observerStream=', 'beginMs=$begin;endMs=$clock.ElapsedMilliseconds;observerStreamAtBegin=$streamAtBegin;native=$st;observerStream=')
(p/'Record-CarrySettledStance-v30.ps1').write_text(s)
