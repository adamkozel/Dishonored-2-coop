import sys
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
from clip_io import *
L=Library(['library-retarget-manifest.json'],'vault-donor-inspection');rig=rig_for('corvo')
for alias in ('sword_vaultraise_walkleft_in','sword_vaultraise_walk_out','sword_mantle_vault_walk_out','sword_vaultthrough_walkleft_in','sword_vaultthrough_walk_out'):
 m,fs=L.load('corvo',alias);print(alias,m['frames'],m['duration'],flush=True)
 for f in sorted(set((0,len(fs)//4,len(fs)//2,3*len(fs)//4,len(fs)-1))):
  g=rig.fk(fs[f]);root=qts(m['root_track'][f]);points={str(j):[round(v,3)for v in (root@g[j]).translation]for j in (2,9,16,21,25)}
  print(f,[round(v,3)for v in root.translation],points,flush=True)
