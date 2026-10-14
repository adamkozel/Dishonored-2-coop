from pathlib import Path
P=Path(__file__).resolve().parent
s=(P/'build_bendtime_contexts_v41.py').read_text(encoding='utf-8-sig')
s=s.replace('bendtime-contexts-v41','bendtime-balanced-v42').replace('bendtime-context-banks-v41','bendtime-balanced-banks-v42').replace('bendtime-context-bank.bin','bendtime-balanced-bank.bin').replace('bendtime-context-source-manifest-v41','bendtime-balanced-source-manifest-v42').replace('bendtime-context-bank-manifest-v41','bendtime-balanced-bank-manifest-v42')
a='    p[17]=rest[17].copy()'
assert s.count(a)==1
s=s.replace(a,a+'''
    if crouched:
     # Local stance offsets previously bent both torso and shoulders,
     # lifting the trailing elbow. Keep the donor's changing actor-space
     # arm directions, reparented to moving crouched shoulders. Rotations
     # alone preserve native lengths; no fixed wrist targets are used.
     def ease(x):
      x=max(0.,min(1.,x));return x*x*(3-2*x)
     weight=ease(t/.25)if phase=='in'else 1-ease(t)if phase=='out'else 1.
     for j in(7,8,9,14,15,16):
      current=rig.fk(p);rig.orient(p,j,current[j].to_quaternion().slerp(g[j].to_quaternion(),weight).to_matrix())
''')
s=s.replace('native_bendtime_{phase}_','native_bendtime_balanced_{phase}_')
s=s.replace('Full-body Bend Time stance adaptation with common start reference across phases.','Full-body Bend Time stance adaptation with animated actor-space arm orientations, preventing the doubled crouched shoulder offset. Common phase reference and moving body, no first-person wrist anchors.')
(P/'build_bendtime_balance_v42.py').write_text(s)
h=P/'Test-BendTimeContexts.ps1';s=h.read_text(encoding='utf-8-sig')
assert s.count('[switch]$Moving)')==1
s=s.replace('[switch]$Moving)',"[switch]$Moving,[ValidatePattern('^[a-z0-9-]+\\.json$')][string]$BankManifest='bendtime-context-bank-manifest-v41.json')")
a='function C($r,$c)';assert s.count(a)==1
s=s.replace(a,'$entry=Get-Content "$PSScriptRoot/$BankManifest" -Raw|ConvertFrom-Json|Where-Object role -eq $character;$bank=Join-Path $PSScriptRoot $entry.bank;if((Get-FileHash $bank).Hash-ne$entry.sha256){throw "Bank hash mismatch"}\n'+a)
s=s.replace('"$PSScriptRoot/bendtime-context-banks-v41/$character-bendtime-context-bank.bin"','$bank')
h.write_text(s)
print('Generated balanced pose builder and explicit bank manifest selection')
