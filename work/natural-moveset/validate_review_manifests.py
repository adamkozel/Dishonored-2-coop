"""Validate recorded candidate bytes with the production review decoder."""
import json, subprocess, hashlib, argparse
from pathlib import Path
P=Path(__file__).resolve().parent
p=argparse.ArgumentParser();p.add_argument('manifests',nargs='+');p.add_argument('--out',required=True);args=p.parse_args()
exe=P.parent/'build/Release/d2_review_clip_tests.exe';reports=[]
for name in args.manifests:
 rows=json.loads((P/name).read_text(encoding='utf-8-sig'));failures=[]
 for row in rows:
  path=P/row['binary'];raw=path.read_bytes();assert hashlib.sha256(raw).hexdigest()==row['sha256']
  r=subprocess.run([str(exe),str(path)],capture_output=True,text=True)
  if r.returncode:failures.append(dict(id=row['id'],duration=row['duration'],frames=row['frames'],error=(r.stdout+r.stderr).strip()))
 reports.append(dict(manifest=name,clips=len(rows),passed=not failures,failures=failures))
(P/args.out).write_text(json.dumps(reports,indent=2));print(json.dumps(reports,indent=2))
raise SystemExit(int(any(not r['passed']for r in reports)))
