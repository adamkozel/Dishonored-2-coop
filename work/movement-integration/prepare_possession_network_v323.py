from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';P=R/'work/natural-moveset'
p=P/'Test-PossessionNetwork-v323.ps1';assert not p.exists()
s=(P/'Test-SwarmNetwork-v270.ps1').read_text().replace('Swarm','Possession').replace('swarm','possession').replace('devouring_possession','possession').replace('SWARM','POSSESSION')
s=s.replace("'coop_possession_fixture off','coop_movement_fixture off'", "'coop_swarm_fixture off','coop_possession_fixture off','coop_movement_fixture off'")
s=s.replace('possession-banks-v267','possession-banks-v323').replace('version-eq49','version-eq51').replace('version-ne49','version-ne51').replace('bank49/304','bank51/315').replace('bank49','bank51')
s=s.replace('23,24,25','26,27').replace('All three exact native phases with reciprocal Corvo arm/body ownership','Both observed native phases with reciprocal Corvo left-arm/equipment ownership')
start=s.index(" $sourceEntities=Q host '--entities'");end=s.index(' $scenes|ConvertTo-Json',start)
s=s[:start]+''' if([Math]::Abs($manaBefore.currentCandidate-$manaAfter.currentCandidate)-gt.001){throw 'Empty-target cast unexpectedly consumed mana'}
 $peerInventory=Q client '--player-inventory'
 $peerPower=@($peerInventory.items|Where-Object name -eq 'models/powers/possession/possession.inventoryitem')
 if($peerPower.Count-ne1-or$peerPower[0].quantity-ne0-or$peerPower[0].component-ne'0x0'){throw 'Remote cosmetic body acquired native Possession'}
 $checks.Add('Empty-target source keeps mana and peer acquires no native Possession component; animation executes no native transfer')
'''+s[end:]
s=s.replace("  $entities=Q host '--entities';if(@($entities.entities|Where-Object {$_.definitionName-match'^models/characters/animals/rat/possession(_big)?\\.def$'}).Count){throw 'Spawned possession remains after checkpoint'}\n",'')
s=s.replace('and retire spawned Possession entities','and clear the native Possession acquisition')
s=s.replace('Standing armed native Corvo Possession phases drive remote full-body bank51 poses through native Steam in isolated AI-held Dunwall. Local first-person remains native; peer animation does not execute rat, VFX or damage events. Remote world/rat behavior, alternate contexts and visual acceptance remain pending. Checkpoint and protected14 restoration mandatory.', 'Standing armed native Corvo empty-target Possession entry333ms/aim200ms drive remote full-body bank51/315 through native Steam protocol48. Source scene retirement drives idle recovery; no invented successful-target release. Local first-person remains native; peer animation executes no transfer, VFX or damage event. Valid-target transfer, disappearance/reappearance, VFX, alternate contexts and visual acceptance remain pending. Checkpoint and protected14 restoration mandatory.')
# Check the immutable bank identity before copying it into the receiver.
anchor=' Copy-Item "$PSScriptRoot/possession-banks-v323/corvo-possession-bank.bin"';assert anchor in s
s=s.replace(anchor,''' $bankPin=Get-Content "$PSScriptRoot/possession-bank-manifest-v323.json" -Raw|ConvertFrom-Json|Where-Object role -eq 'corvo'
 if((Get-FileHash "$PSScriptRoot/possession-banks-v323/corvo-possession-bank.bin").Hash-ne$bankPin.sha256){throw 'Candidate bank hash differs'}
'''+anchor)
p.write_text(s)
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text().replace("'PossessionObserve','SwarmObserve'", "'PossessionNetwork','PossessionObserve','SwarmObserve'")
anchor="  'PossessionObserve'{";assert s.count(anchor)==1
s=s.replace(anchor,"  'PossessionNetwork'{& \"$root/work/natural-moveset/Test-PossessionNetwork-v323.ps1\" -Take $Take -Pitch $Pitch}\n"+anchor);p.write_text(s)
# Archive source/binaries and exact immutable data provenance after checks.
s=(W/'archive_possession_fixture_v322.py').read_text().replace('v322','v323').replace('candidate-possession-fixture-v323','candidate-possession-body-v323').replace('prepare_possession_scene_v323.py','prepare_possession_body_v323.py').replace('Observe-NativePossession-v323.ps1','Test-PossessionNetwork-v323.ps1')
s=s.replace("W/'prepare_possession_body_v323.py'", "W/'prepare_possession_body_v323.py',W/'prepare_possession_network_v323.py',R/'work/natural-moveset/build_possession_bank_v323.py',R/'work/natural-moveset/possession-bank-manifest-v323.json',R/'work/natural-moveset/possession-bank-source-manifest-v323.json'")
s=s.replace("runtimeVerified=False,scope=", "runtimeVerified=False,protocol=48,bankVersion=51,clips=315,scope=")
start=s.index("scope='Candidate322:");end=s.index("')\n",start)
s=s[:start]+"scope='Candidate323: exact observed Corvo standing armed empty-target Possession entry333/aim200 scene reader drives semantic protocol48 and full-body bank51/315. Native first-person and network root remain authoritative. Source scene retirement returns remote body to idle. No successful-target out phase, transfer, visibility, VFX, damage or alternate-context claim. All previous313 clip payloads retained. Native runtime and visuals pending."+s[end:]
s=s.replace("for b in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/b,A/b)", "shutil.copytree(R/'work/natural-moveset/possession-banks-v323',A/'banks')\nfor b in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/b,A/b)")
(W/'archive_possession_body_v323.py').write_text(s)
print('Prepared323 paired native network body test, pins and archive; no target transfer or release phase implied')
