from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration'
s=(P/'Test-PossessionReleaseNetwork-v335.ps1').read_text()
needle=" $checks.Add('Pinned bank52/316 and protocol49 native Steam stream ready;315 prior clip payloads retained')"
assert s.count(needle)==1
s=s.replace(needle," $visibilityBefore=S client '.possession-body.json';$visibilityBefore|ConvertTo-Json|Set-Content \"$out/visibility-before.json\"\n $checks.Add('Pinned bank52/316 and protocol50 native Steam stream ready;315 prior clip payloads retained')")
s=s.replace("sourceStream=(S host '.shadow-stream.json')", "sourceStream=(S host '.shadow-stream.json');visibility=(S client '.possession-body.json')")
needle=" $checks.Add('Exact native400ms release scene reaches peer over Steam and drives bank52 full-body release with holstered sword; occupied-host stream retirement remains explicit')"
assert s.count(needle)==1
s=s.replace(needle,""" $checks.Add('Exact native400ms release scene reaches peer over Steam and drives bank52 full-body release with holstered sword')
 $hidden=@($rows|Where-Object {$_.stream.remoteState-eq17-and$_.visibility.active-and$_.visibility.hidden})
 $visibilityAfter=S client '.possession-body.json';$visibilitySource=S host '.possession-body.json'
 [ordered]@{before=$visibilityBefore;after=$visibilityAfter;source=$visibilitySource;hiddenSamples=$hidden.Count;hiddenRows=$hidden}|ConvertTo-Json -Depth 16|Set-Content "$out/visibility-evidence.json"
 if($hidden.Count-lt3-or$visibilityAfter.active-or$visibilityAfter.restoring-or$visibilityAfter.hidden-or
    $visibilityAfter.hideCalls-$visibilityBefore.hideCalls-ne1-or$visibilityAfter.showCalls-$visibilityBefore.showCalls-ne1-or
    $visibilityAfter.lost-ne$visibilityBefore.lost-or$visibilityAfter.refused-ne$visibilityBefore.refused-or$visibilitySource.hideCalls){throw 'Remote Corvo body hide/restore ownership or continuity failed'}
 $checks.Add('Native transfer/occupied context maintains fresh Corvo-only semantic17; exactly one native clone hide and show, with verified restoration and no local-player/Emily visibility mutation')
""")
s=s.replace('Existing stream deliberately expires during unsupported host occupation;', 'Human transfer/internal1 and occupation/internal8 maintain a finite cosmetic clone visibility lease;')
s=s.replace('remote body hiding, target control, VFX and shared gameplay remain unimplemented.', 'remote target control, VFX and shared gameplay remain unimplemented. Body visibility uses native API on the exact held Corvo clone only; interruption/alternate contexts pending.')
(P/'Test-PossessionVisibilityNetwork-v336.ps1').write_text(s,encoding='utf-8')
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text().replace('Test-PossessionReleaseNetwork-v335.ps1','Test-PossessionVisibilityNetwork-v336.ps1');p.write_text(s,encoding='utf-8')
# Archive actual source/binaries before deployment, keeping334 intact.
s=(W/'archive_possession_release_body_v334.py').read_text().replace('v334','v336')
s=s.replace("P/'possession-banks-v336'", "P/'possession-banks-v334'").replace("W/'prepare_possession_release_body_v336.py',W/'prepare_possession_network_v336.py'", "W/'prepare_possession_visibility_v336.py',W/'prepare_possession_visibility_test_v336.py'")
s=s.replace("W/'possession-release-build-v336.log'", "W/'possession-visibility-build-v336.log'").replace("W/'possession-release-bank-v336.log'", "W/'possession-visibility-tests-v336.log'")
s=s.replace("P/'build_possession_bank_v336.py'", "P/'build_possession_bank_v334.py'").replace("P/'Test-PossessionReleaseNetwork-v336.ps1'", "P/'Test-PossessionVisibilityNetwork-v336.ps1'")
s=s.replace('possession-bank-manifest-v336','possession-bank-manifest-v334').replace('possession-bank-source-manifest-v336','possession-bank-source-manifest-v334').replace('protocol=49','protocol=50')
a=s.index("scope='Native observed Corvo");b=s.index("')\n",a)
s=s[:a]+"scope='Cosmetic native human Possession transfer/occupied presentation semantic17 over protocol50, finite exactly-owned Corvo clone native visibility lease and cleanup. Release bank52/316 unchanged from334. Source contexts1/8 require exact native classes, physics, generation-owned human target and dormant acquired power. No NPC control/damage/VFX/local-player visibility mutations. Native runtime/visual validation pending."+s[b:]
s=s.replace("commands=['cmake build Release d2_lab_proxy d2_proxy_smoke d2_runtime_probe d2_shadow_action_tests d2_movement_bank_tests','d2_shadow_action_tests.exe','d2_movement_bank_tests.exe corvo-possession-bank.bin emily-possession-bank.bin','d2_proxy_smoke.exe dinput8.dll']", "commands=['cmake build Release d2_lab_proxy d2_shadow_action_tests','d2_shadow_action_tests.exe','d2_proxy_smoke.exe dinput8.dll']")
s=s.replace('Compilation, semantic protocol49, actual two bank52 payloads, proxy/Steam pose/skip protocol smoke; native deployment pending', 'Compilation, semantic protocol50/context/freshness guards and proxy/Steam pose/skip protocol smoke. Actual bank52 tests passed334; unchanged bank hashes archived. Probe unchanged330. Native deployment pending')
(W/'archive_possession_visibility_v336.py').write_text(s,encoding='utf-8')
print('Prepared native visibility comparison, exact one-hide/one-show assertions and candidate archive')
