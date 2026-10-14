from pathlib import Path
R=Path(__file__).resolve().parents[2]
s=(R/'work/natural-moveset/Observe-NativeGrenadeContexts-v60.ps1').read_text(encoding='utf-8-sig')
def replace(a,b):
    global s
    assert a in s,a
    s=s.replace(a,b)
replace("$Stance='crouched'", "$Stance='standing'")
replace("$fired=$false;$scenes=@{}", "$fired=$false;$jumped=$false;$released=$false;$jumpAt=0;$scenes=@{}")
replace("  $midnight=Q $Source '--midnight';", """  if($fired-and!$jumped-and$watch.ElapsedMilliseconds-ge950){
   I $Source A 350 255 $Move;$jumped=$true;$jumpAt=$watch.ElapsedMilliseconds
   $events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native jump while cooking; trigger still held'})
  }
  if($jumped-and!$released-and$watch.ElapsedMilliseconds-ge($jumpAt+350)){
   I $Source Neutral 100 0 $Move;$released=$true
   $events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native trigger release after jump; no velocity or gameplay calls'})
  }
  $midnight=Q $Source '--midnight';""")
replace(" $checks.Add('Actual native cooking entry, cooking idle and throw scene observed')", """ $checks.Add('Actual native cooking entry, cooking idle and throw scene observed')
 if(!$jumped-or!$released){throw 'Airborne input sequence did not execute'}
 if(!@($rows|Where-Object {@($_.player.active|Where-Object {$_.id-in@(2,3)}).Count}).Count){throw 'Native jump/fall was not observed; input is not airborne coverage'}
 $checks.Add('Native jump/fall state observed independently of controller input')""")
replace("scope='Source native one-grenade action observation only.","scope='Source native one-grenade jump/cook/release observation only. Input alone does not establish airborne overlap; inspect exact state/scene rows.")
(R/'work/natural-moveset/Observe-NativeGrenadeAir-v63.ps1').write_text(s,encoding='utf-8')
p=R/'work/movement-integration/Invoke-NativeLabOperation.ps1';t=p.read_text(encoding='utf-8-sig')
t=t.replace("'GrenadeObserve','GrenadeProp'", "'GrenadeObserve','GrenadeAirObserve','GrenadeProp'")
t=t.replace("  'GrenadeProp'{", "  'GrenadeAirObserve'{& \"$root/work/natural-moveset/Observe-NativeGrenadeAir-v63.ps1\" -Source $Source -Take $Take -Pitch $Pitch -Stance standing -Move $Move}\n  'GrenadeProp'{")
p.write_text(t,encoding='utf-8');print('Prepared native airborne grenade observer; no runtime mutation or airborne recognition change')
