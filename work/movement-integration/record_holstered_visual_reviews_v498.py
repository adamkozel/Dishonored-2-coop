from pathlib import Path
import json, datetime
R=Path(__file__).resolve().parents[2]; N=R/'work/natural-moveset'
now=datetime.datetime.now().astimezone().isoformat()
for take in ('native-doppelganger-holstered-standing-v493','native-doppelganger-holstered-crouched-v494',
             'doppelganger-holstered-standing-v496','doppelganger-holstered-crouched-v497'):
    native=take.startswith('native-'); crouched='crouched' in take
    for suffix in (('',) if native else ('','-source')):
        source=native or bool(suffix)
        sheet='source-sheet.jpg' if source else 'remote-sheet.jpg'
        assert (N/take/sheet).exists()
        notes=(['Native first-person left-hand gesture, white placement preview and summon burst are visible; the sword stays outside the hand.',
                'The native clone emerges separately. First-person view remains the original game presentation.'] if source else
               ['Remote caster changes casting-arm and torso pose, with empty right hand and no visible drawn sword.',
                'Native summon burst produces a separate clone; the caster recovers to its prior ground stance.',
                'Crouched caster is visibly low during the gesture and upright by10s after the native stand input.' if crouched else 'Standing caster returns to its empty-handed idle by5–6s.'])
        value=dict(inspected=True,at=now,method='Viewed labelled temporal contact sheet extracted from the completed actual game video with ffmpeg; no visual edits.',
                   sheet=sheet,video=take+suffix+'.mp4',observations=notes,complete=False,visualAccepted=False,
                   limitations=['Temporal samples establish the visible states but do not establish continuous-motion polish.',
                                'The remote caster is dark and fairly small; fine finger, foot-contact and cloth quality remain unaccepted.' if not source else 'Native first-person reference, not third-person animation acceptance.'])
        (N/take/('visual-review'+suffix+'.json')).write_text(json.dumps(value,indent=2),encoding='utf-8')
print('Saved six inspected source/remote reviews with explicit visual limitations.')
