from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
s=(N/'Test-DoorRearm-v764.ps1').read_text(encoding='utf-8-sig')
s=s.replace('native-door-session-reload-client-v764','native-door-menu-reload-client-v776').replace('candidate-door-session-v763','candidate-door-menu-v769').replace('Expected archived763','Expected archived769').replace('NativeDoorRearm764','NativeDoorMenuReload776')
(N/'Test-DoorMenuReload-v776.ps1').write_text(s,encoding='utf-8')
print('Prepared existing shared-checkpoint renewal regression against candidate769; no invented load or menu event')
