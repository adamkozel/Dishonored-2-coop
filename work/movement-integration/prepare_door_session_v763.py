from pathlib import Path
R=Path(__file__).resolve().parents[2]; W=R/'work/movement-integration'; N=R/'work/natural-moveset'
p=R/'outputs/d2-coop-lab/src/door_sync.h';t=p.read_text();old=r'fprintf(f,"{\"experimental\":true';assert t.count(old)==1;p.write_text(t.replace(old,r'fprintf(f,"\"experimental\":true'))
for kind in ['Build','Deploy']:
    t=(W/f'{kind}-DoorSide-v755.ps1').read_text().replace('DoorSide','DoorSession').replace('door-side','door-session').replace('door_side','door_session').replace('755','763')
    (W/f'{kind}-DoorSession-v763.ps1').write_text(t)
t=(W/'stage_door_side_v755.py').read_text().replace('DoorSide','DoorSession').replace('door-side','door-session').replace('door_side','door_session').replace('755','763')
t=t.replace("P=W/'candidate-checkpoint-retry-v742'","P=W/'candidate-door-side-v755'")
start=t.index("scope='");end=t.index("')",start)
t=t[:start]+"scope='Explicit ordinary-door session keeps ID-only intent across verified coordinated same-map checkpoint loads. Old native bindings retire before load; both-complete and restored local role/counterpart plus fresh paired epochs allow a new catalog and new arms. Session mode does not pause AI or depend on movement isolation. Legacy one-world fixture mode retained. Session change, uncoordinated world change or failed checkpoint retires intent. Same protocol69/bank67/357 and exact native queue. Runtime pending."+t[end:]
(W/'stage_door_session_v763.py').write_text(t)
print('Prepared candidate763 build/deploy')
