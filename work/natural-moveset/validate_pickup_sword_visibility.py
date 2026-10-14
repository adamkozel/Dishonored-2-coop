"""Verify native sword visibility around a completed paired pickup capture."""
import json,sys,hashlib
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
for take in sys.argv[1:]:
 f=P/take;i=read(f/'identity.json');a=read(f/'sword-before.json');h=read(f/'held-sword.json');z=read(f/'sword-restored.json');s=read(f/'stopped.json');rows=read(f/'observations.json')
 checks=dict(carrier_passed=read(f/'validation.json')['passed'],paired_passed=read(f/'paired-validation.json')['passed'],
  exact_child_preserved=all(x['weaponHandle']==a['weaponHandle']and x['entity']==a['entity']and x['inventory']==a['inventory']for x in [h,z]),
  drawn_visible_before=a['equipped']and a['started']and not a['explicitHidden'],
  holstered_hidden_while_held=not h['equipped']and not h['started']and h['explicitHidden']and h['attachedTag']=='holster_wpn',
  native_hide_observed=any(r['equipment']['hiddenNow']and r['equipment']['nativeHideCalls']>0 for r in rows),
  native_show_on_cleanup=s['equipment']['nativeShowCalls']>0,
  original_equipment_and_visibility_restored=all(z[k]==a[k]for k in ['equipped','started','explicitHidden','attachedTag']),
  ownership_retired=not s['equipment']['active']and not s['equipment']['restoring'],
  no_lost_or_failed_visibility=s['equipment']['lost']==0 and s['equipment']['visibilityRefused']==0)
 out=dict(passed=all(checks.values()),checks=checks,dll=i['dllSha256'],video_sha256=hashlib.sha256((f/'native.mp4').read_bytes()).hexdigest().upper(),
  scope='Exact clone inventory child only: native hide while holstered, show on original-state restoration. Visual confirmation recorded separately. No physical folding claim.',complete=False)
 (f/'sword-visibility-validation.json').write_text(json.dumps(out,indent=2));print(json.dumps(dict(take=take,**out)))
 if not out['passed']:raise SystemExit(1)
