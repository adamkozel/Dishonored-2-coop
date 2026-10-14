from pathlib import Path
import json,datetime
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
at=datetime.datetime.now().astimezone().isoformat()
for take in ['native-window-input-v828d','native-window-api-v830b','shared-window-family-v832b','shared-window-recovery-v833b']:
    for p in (N/take).glob('*.mp4'):
        sheet=p.with_name(p.stem+'-sheet.jpg');assert sheet.exists(),sheet
        if take.endswith('v828d'):
            note='Inspected contact sheet: host native Window/Use view opens to sea and closes after raised aim; client remains in fixed Dunwall room. Local ordinary-use reference, no relay. Peer image is not a view of its window.'
        elif take.endswith('v830b'):
            note='Inspected both native first-person contact sheets: window opens/closes using indexed native API. Source grips and camera remain native. Exact observed state/queued index establishes reversal; sparse contact sheets alone do not prove timing. Local API experiment, no relay.'
        elif take.endswith('v832b'):
            note='Inspected both native views of window4 opening/closing during ordinary host/client use and later batch activity. Four-window owned-state records establish off-camera members; this camera does not visually prove all four windows. No remote actor animation or traversal approval.'
        elif p.stem.startswith('menu-'):
            note='Inspected native pause menu on host and client window opening while host remains paused; host returns to matching open window after resume. Native records confirm no receiver API call during held menu and retained catalog arms.'
        elif p.stem.startswith('reload-'):
            note='Inspected native loading screens and return to Dunwall with unarmed local hands. Runtime records establish one additional load/completion per role, counterpart restoration and four fresh window channels. Fixed room camera does not frame reloaded windows.'
        else:
            note='Inspected both fresh-after-load native views showing open window and sea; opening occurs near capture startup. Native ordinary-client input/state records establish the transition. This sparse sheet is evidence of final appearance, not complete transition timing.'
        if take.endswith('v833b'):
            note+=' Raw harness result remains failed: final inventory check compared against pre-reload sword ownership; exact sword1->0 difference matches the earlier checkpoint. Functional window checks and final complete inventory restoration are verified separately in audit834.'
        value=dict(at=at,inspected=True,inspectionMethod='Visually inspected recorded contact sheets against native snapshots',visualAccepted=False,complete=False,sheets=[sheet.name],observations=[note])
        p.with_name(p.stem+'-review-v834.json').write_text(json.dumps(value,indent=2))
print('Saved12 inspected game-video reviews with scope and raw recovery failure retained')
