from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
s=(N/'Test-ChokeNetwork-v589.ps1').read_text(encoding='utf-8-sig')
s=s.replace('ChokeNetwork589','ChokeCancelObserve590').replace('I $source RightShoulder 6500','I $source RightShoulder 1200').replace('held6500ms','held1200ms')
start=s.index('   foreach($v in 1,2,3){');end=s.index("   $checks.Add('Ordinary guard",start)
s=s[:start]+'''   $cancel=@($paired|Where-Object {$_.asset-like'*/choke_cancel.midnightscene'-and$_.subscene-eq'Npc'-and$_.sceneDurationMs-eq400})
   if(!$cancel.Count){throw 'Native cancel400 phase not observed after ordinary input release'}
   $checks.Add('Ordinary input release captured native choke_cancel/Npc400 scene')
   C $peer 'coop_movement_bank off';foreach($r in 'host','client'){C $r 'coop_shadow_stream off'}
'''+s[end:]
s=s.replace('Source-timed remote actor body through native Steam. Remote victim animation/alignment and shared knockout outcomes are not implemented by this binding.', 'Native cancel source observation only. Existing actor binding does not yet represent cancellation; no remote cancel completion or shared victim outcome is claimed.')
s=s.replace('PASS_CHOKE_NETWORK','PASS_NATIVE_CHOKE_CANCEL')
(N/'Observe-ChokeCancel-v590.ps1').write_text(s)
print('Native cancel observation590 prepared, not executed.')
