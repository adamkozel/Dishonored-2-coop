from pathlib import Path
import datetime, hashlib, json, shutil, struct
R=Path(__file__).resolve().parents[2]; W=R/'work/movement-integration'; N=R/'work/natural-moveset'; L=R/'outputs/d2-coop-lab'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
exe=R/'work/asset-game/Dishonored2.exe'
assert sha(exe)=='B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180'
b=exe.read_bytes(); pe=struct.unpack_from('<I',b,0x3c)[0]; count=struct.unpack_from('<H',b,pe+6)[0]; opt=struct.unpack_from('<H',b,pe+20)[0]
sections=pe+24+opt; rva=0x3c22f0; offsets=[]
for i in range(count):
 p=sections+i*40; size,va,raw_size,raw=struct.unpack_from('<IIII',b,p+8)
 if va<=rva and rva+14<=va+raw_size:offsets.append(raw+rva-va)
assert len(offsets)==1
actual=b[offsets[0]:offsets[0]+14]; expected=bytes.fromhex('48895c241844884c242088542410'); assert actual==expected
(W/'possession-partner-prologue-v374.json').write_text(json.dumps(dict(executable=str(exe),sha256=sha(exe),rva=hex(rva),fileOffset=hex(offsets[0]),bytes=actual.hex(),wholeInstructions=3,length=14,confirmed=True),indent=2),encoding='utf-8')
s=(N/'Observe-PossessionPartner-v373.ps1').read_text(encoding='utf-8-sig')
s=s.replace('$cast=$false;$cancelled=$false','$cast=$false;$released=$false')
s=s.replace("$w.Restart();$cast=", "$filterBefore=S host '.possession-partner-filter.json';if(!$filterBefore.installed){throw 'Partner filter not installed'}\n $w.Restart();$cast=")
s=s.replace('I host 255 8000','I host 255 2500').replace('Native Corvo LT held toward exact partner, cancellation before release','Native Corvo LT held toward exact partner, then normal release')
start=s.index('  if($cast-and!$cancelled-and$w.ElapsedMilliseconds-ge3600){')
end=s.index("  $cp=Q host '--object'",start)
s=s[:start]+'''  if($cast-and!$released-and$w.ElapsedMilliseconds-ge3900){
   I host;$released=$true
   $events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Neutral after timed LT release; no crouch/cancel input'})
  }
'''+s[end:]
s=s.replace("$checks.Add('Native selected-target references and partner component membership recorded; eligibility remains an observation, not an exclusion claim')",'''$filterAfter=S host '.possession-partner-filter.json'
 [ordered]@{before=$filterBefore;after=$filterAfter}|ConvertTo-Json -Depth 8|Set-Content "$out/filter-observations.json"
 if($filterAfter.blocked-le$filterBefore.blocked-or$filterAfter.readFailed-ne$filterBefore.readFailed){throw 'Partner not blocked consistently by exact identity filter'}
 foreach($row in $selected){
  $targetWord=($row.component.words|Where-Object offset -eq '0xb0').word
  $selectedHandle=if($targetWord){[uint32]([Convert]::ToUInt64($targetWord.Substring(2),16)-band0xffffffffL)}else{0}
  if($selectedHandle-eq[uint32]$row.partnerHandle-or$row.target){throw 'Partner or other target selected in partner-only corridor'}
 }
 if(@($rows|Where-Object {!$_.player.defaultMode-or$_.player.active.id-contains26-or$_.player.physicsType-eq18}).Count){throw 'Native player entered transfer/occupied mode'}
 if([Math]::Abs([double]$manaAfter.currentCandidate-[double]$manaBefore.currentCandidate)-gt.01){throw 'Blocked partner cast consumed mana'}
 $checks.Add('Exact current partner blocked before native target/FX selection; no identity refusals, transfer, occupied state or mana cost')''')
s=s.replace("if(!$cancelled){throw 'Native cancellation not sent'}", "if(!$released){throw 'Normal LT release not sent'}")
s=s.replace("throw 'Native cancel did not retain standing player'", "throw 'Blocked partner attempt did not retain standing player'")
s=s.replace('Partner aim observation with native human upgrade and B cancellation before release. No partner exclusion implemented by this test.', 'Partner exclusion with native human upgrade, ordinary LT hold/release, target references, no mana cost, no transfer, unchanged health and full checkpoint restoration. Ordinary NPC positive control is separate.')
s=s.replace('PASS_NATIVE_POSSESSION_OBSERVATION','PASS_NATIVE_POSSESSION_PARTNER_EXCLUSION')
(N/'Test-PossessionPartner-v374.ps1').write_text(s,encoding='utf-8')
runner=W/'Invoke-NativeLabOperation.ps1'; s=runner.read_text(encoding='utf-8-sig')
s=s.replace("Observe-PossessionPartner-v373.ps1", "Test-PossessionPartner-v374.ps1")
runner.write_text(s,encoding='utf-8')
A=W/'candidate-possession-partner-v374'; assert not A.exists()
logs=[W/f'possession-partner-{k}-v374.log' for k in ('build','world-tests','action-tests','bank-tests','smoke')]
for p in logs:
 t=p.read_text(encoding='utf-8-sig'); assert t and not any(x in t for x in ('error C','error LNK','FAIL:','FAIL ','Traceback')),p
A.mkdir();shutil.copytree(L/'src',A/'src');shutil.copytree(N/'possession-crouch-release-banks-v371',A/'banks')
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/name,A/name)
for p in [Path(__file__),*logs,W/'possession-partner-prologue-v374.json',W/'Build-PossessionPartner-v374.ps1',runner,N/'Test-PossessionPartner-v374.ps1']:shutil.copy2(p,A/p.name)
x=json.loads((W/'candidate-possession-crouch-release-v371/identity.json').read_text(encoding='utf-8-sig'))
x.update(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),runtimeVerified=False,banks=[dict(path=str(p),sha256=sha(p)) for p in sorted((A/'banks').glob('*.bin'))],scope='Exact current partner exclusion at native Possession ranked-selector return. Calls native selector once; retains ordinary targets. Selected power/sensor/membership/generation/current avatar proof required. Build and synthetic tests passed; runtime pending. Bank54/319 unchanged from371.')
x.pop('runtimeVerificationScope',None)
(A/'identity.json').write_text(json.dumps(x,indent=2),encoding='utf-8')
for name in ('dinput8.dll','d2_runtime_probe.exe'):
 shutil.copy2(A/name,L/'bin'/name);assert sha(L/'bin'/name)==sha(A/name)
print(json.dumps(dict(archive=str(A),dll=x['dll'],probe=x['probe'],prologue=actual.hex())))
