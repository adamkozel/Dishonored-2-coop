from pathlib import Path
import json, datetime
N=Path(__file__).resolve().parents[2]/'work/natural-moveset'
now=datetime.datetime.now().astimezone().isoformat()
for take in ('native-doppelganger-cancel-standing-v501','doppelganger-cancel-standing-v503','doppelganger-cancel-crouched-holstered-v504'):
    native=take.startswith('native-'); crouched='crouched' in take
    for suffix in (('',) if native else ('','-source')):
        source=native or bool(suffix); sheet='source-sheet.jpg' if source else 'remote-sheet.jpg'
        assert (N/take/sheet).exists()
        observations=(['Native left-hand aim and white placement preview clear after cancellation, without a summon burst or clone.',
                       'The source remains crouched and the sword remains sheathed.' if crouched else 'The native sword remains drawn through recovery.'] if source else
                      ['Casting arm rises during aim and lowers to the prior idle by the 3.7-second sample. No clone or summon burst is visible.',
                       'The low crouched full-body pose recovers with sword sheathed.' if crouched else 'The standing body recovers with the sword still held.'])
        value=dict(inspected=True,at=now,method='Viewed labelled temporal contact sheets extracted from the completed actual-game recordings; no visual edits.',sheet=sheet,video=take+suffix+'.mp4',observations=observations,complete=False,visualAccepted=False,
                   limitations=['Temporal samples do not establish continuous-motion polish.','Native first-person reference, not third-person quality acceptance.' if source else 'Remote body is dark and small; fine fingers, feet and cloth remain unaccepted.'])
        (N/take/('visual-review'+suffix+'.json')).write_text(json.dumps(value,indent=2),encoding='utf-8')
print('Saved five inspected cancellation reviews; final visual acceptance remains separate.')
