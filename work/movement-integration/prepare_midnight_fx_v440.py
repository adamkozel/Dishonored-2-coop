from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
s=(N/'Test-MidnightFx-v439.ps1').read_text(encoding='utf-8-sig')
# PowerShell enumerates a null for each non-FX track's missing records member.
# Five actual records plus four absent members counted nine in439.
s=s.replace('@($_.bindingsResearch[0].fxResearch.tracks.records).Count','@($_.bindingsResearch[0].fxResearch.tracks.records|Where-Object {$null-ne$_}).Count')
s=s.replace('$fxRecords=@($proof.bindingsResearch[0].fxResearch.tracks.records)','$fxRecords=@($proof.bindingsResearch[0].fxResearch.tracks.records|Where-Object {$null-ne$_})')
target=N/'Test-MidnightFx-v440.ps1';assert not target.exists();target.write_text(s,encoding='utf-8')
s=(W/'Run-MidnightFx-v439.ps1').read_text(encoding='utf-8-sig').replace('midnight-fx-v439','midnight-fx-v440').replace('Test-MidnightFx-v439','Test-MidnightFx-v440')
(W/'Run-MidnightFx-v440.ps1').write_text(s,encoding='utf-8')
print('440 fixes research assertion only; unchanged DLL427/probe438')
