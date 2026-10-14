from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
for name in ['Build-DoorApi-v710.ps1','Deploy-DoorApi-v710.ps1','stage_door_api_v710.py']:
    text=(W/name).read_text(encoding='utf-8-sig').replace('710','713')
    if name.startswith('stage_'):
        text=text.replace("P=W/'candidate-door-api-v707'","P=W/'candidate-door-api-v710'")
        text=text.replace('709 exact native base AnimBase VT1b7f408, not character VT1b80408. Strict native Openable reader and explicit isolated ordinary-door native request proof.','712 analysis: fresh Moving target is -1; interrupt API74d7a0 incorrectly finalizes it before starting.713 uses native74a400 queue and component resume, matching ordinary interaction and opposite close clip. Strict native identity retained.')
    (W/name.replace('710','713')).write_text(text,encoding='utf-8')
text=(N/'Test-NativeDoorApi-v711.ps1').read_text(encoding='utf-8-sig').replace('v711','v714').replace('NativeDoorApi711','NativeDoorApi714').replace('candidate-door-api-v710','candidate-door-api-v713').replace('archived710','archived713')
text=text.replace("state90=('0x{0:x}'-f(W $o '0x90'));", "state90=('0x{0:x}'-f(W $o '0x90'));movingTarget=('0x{0:x}'-f(W $o '0xb0'));")
text=text.replace('Openable74d7a0 request proof: open0 and close3 with native direction.','Openable74a400 request proof with ordinary native open/close parameters and component resume.')
(N/'Test-NativeDoorApi-v714.ps1').write_text(text,encoding='utf-8')
print('Prepared713 build/deploy and714 isolated proof')
