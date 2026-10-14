from pathlib import Path
W=Path(__file__).resolve().parent
for kind in ('Build','Deploy'):
    s=(W/f'{kind}-DoorSession-v763.ps1').read_text(encoding='utf-8-sig')
    s=s.replace('DoorSession','DoorMenu').replace('door-session','door-menu').replace('door_session','door_menu').replace('763','769')
    (W/f'{kind}-DoorMenu-v769.ps1').write_text(s,encoding='utf-8')
s=(W/'stage_door_session_v763.py').read_text(encoding='utf-8-sig')
s=s.replace("A=W/'candidate-door-session-v763'; P=W/'candidate-door-side-v755'", "A=W/'candidate-door-menu-v769'; P=W/'candidate-door-session-v763'")
s=s.replace('DoorSession-v763','DoorMenu-v769').replace('stage_door_session_v763','stage_door_menu_v769').replace('-v763.log','-v769.log')
a=s.index("scope='");b=s.index("')\n(A/",a)
s=s[:a]+"scope='Same-world native pause/read-note/wheel retention for an explicitly armed ordinary-door session. Suspends door traffic and native mutation; resumes with fresh Hello on original arms, excludes paused time from pending native deadlines, republishes host state and outstanding client intent. Checkpoint, changed-world and invalid-phase retirement preserved. Runtime pending."+s[b:]
(W/'stage_door_menu_v769.py').write_text(s,encoding='utf-8')
print('Prepared isolated candidate769 build/deploy/archive scripts')
