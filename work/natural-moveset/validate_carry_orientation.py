"""Catch carried-body yaw drift during a fixed-heading native carry walk."""
import json,math,sys
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def yaw(s):return math.degrees(math.atan2(s['root'][4],s['root'][3]))
def diff(a,b):return (a-b+180)%360-180
for take in sys.argv[1:]:
 f=P/take;i=read(f/'identity.json');a=read(f/'carrier-held.json');b=read(f/'victim-held.json')
 final=read(f/'loaded-stance-contact.json')['pair'];c=final['carrier'];v=final['victim']
 before=diff(yaw(b),yaw(a));after=diff(yaw(v),yaw(c));drift=diff(after,before)
 checks=dict(completed_carrier_publication=final['carrierFinalPublicationComplete'],
  sequential_read_under_50ms=final['readEndTick']-final['readBeginTick']<=50,
  fixed_carrier_heading=abs(diff(yaw(c),yaw(a)))<1,
  held_body_yaw_drift_under_5deg=abs(drift)<5)
 out=dict(passed=all(checks.values()),checks=checks,relative_yaw_before_deg=before,relative_yaw_after_deg=after,
  drift_deg=drift,dll=i['dllSha256'],bank_sha256=i['bankSha256'],complete=False,
  scope='Two stable actor-root observations across one bounded fixed-heading native carry walk. Not turning, arbitrary corpses or visual acceptance.')
 (f/'orientation-validation.json').write_text(json.dumps(out,indent=2));print(json.dumps(dict(take=take,**out)))
 if not out['passed']:raise SystemExit(1)
