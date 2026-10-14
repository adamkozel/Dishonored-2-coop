from pathlib import Path
R=Path(__file__).resolve().parents[2]; N=R/'work/natural-moveset'
s=(N/'Observe-NativeDoor-v640.ps1').read_text().replace('native-unlocked-door-corvo-v640','native-unlocked-door-corvo-v641').replace('NativeDoor640','NativeDoor641')
# PowerShell hex ffffffff is signed -1; it retained adjacent changing flags.
# Explicit unsigned decimal mask reads only the native generation DWORD.
s=s.replace('-band 0xffffffff','-band [uint64]4294967295')
s=s.replace("if($o.type-ne'.?AVarkCpntOpenable@@'-or(W $o '0x10')-ne$d.owner-or((W $o '0x18')-band [uint64]4294967295)-ne$d.generation){throw 'Door component lifetime changed'}", "if($o.type-ne'.?AVarkCpntOpenable@@'-or(W $o '0x10')-ne$d.owner-or((W $o '0x18')-band [uint64]4294967295)-ne$d.generation){$o|ConvertTo-Json -Depth 7|Set-Content \"$out/failed-door-$r.json\";throw 'Door component lifetime changed'}")
s=s.replace("$peerCapture.WaitForExit(15000)","$peerCapture.WaitForExit(35000)")
s=s.replace("}catch{$failure=$_.Exception.Message}finally{", "}catch{$failure=$_.Exception.Message;if($samples){$samples|ConvertTo-Json -Depth 17|Set-Content \"$out/partial-observations.json\"};if($inputs){$inputs|ConvertTo-Json|Set-Content \"$out/partial-inputs.json\"}}finally{")
p=N/'Observe-NativeDoor-v641.ps1'
if p.exists(): raise SystemExit('Unique script exists')
p.write_text(s)
print('641 fixes unsigned generation/state masks and retains partial diagnostics. No production code changed.')
