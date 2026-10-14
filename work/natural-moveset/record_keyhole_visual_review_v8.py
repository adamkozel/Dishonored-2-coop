import json
from pathlib import Path
P=Path(__file__).resolve().parent
reviewed={
 'keyhole-corvo-right-crouch-v24-v9':('contact-1.jpg','Clean checkpoint repeat removes the obstructing guard from the view. Both boots, knees, low entry, kneeling hold and low recovery are visible; native sword redraw follows recovery. Contact-2.jpg was also inspected for holstered entry. Exact hand/door contact and broader geometry remain unaccepted.'),
 'keyhole-corvo-left-early-v24-v9':('contact-1.jpg','Cancellation during entry returns the full body to standing without a fabricated kneeling exit; native sword recovery is visible. Fine hand contact and temporal smoothness remain unaccepted.'),
 'keyhole-emily-left-early-crouch-v24-v9':('contact-1.jpg','Entry cancellation returns to crouched movement with the full body low and boots visible. Native sword recovery is visible. Continuous temporal quality remains to assess.'),
 'keyhole-corvo-pause-v24-v8':('contact.jpg','Native scene and equipment recover after menu pause, but retiring presentation temporarily reveals native standing/hidden avatar states. Functional ownership recovery passed; visual pause continuity remains unfinished.'),
 'keyhole-emily-crouch-pause-v24-v9':('contact.jpg','Crouched scene recovers after menu pause, but temporary standing/hidden avatar frames occur while presentation ownership is suspended. This exposes a remaining pause-pose continuity issue.'),
 'keyhole-corvo-left-stand-v24-v6':('contact-1.jpg','Full body enters, kneels at the door and returns to standing; sword recovers after the native exit. Dark scene limits finger/contact inspection.'),
 'keyhole-corvo-left-crouch-v24-v6':('contact-2.jpg','Corrected low entry and exit stay crouched instead of using the original stand-up donor. Full body is visible; dark lighting limits finger inspection.'),
 'keyhole-emily-left-stand-v24-v6':('contact-1.jpg','Whole-body entry, kneeling hold, and standing recovery are visible. Native coat hangs behind the hips; lighting limits fine contact assessment.'),
 'keyhole-emily-left-crouch-v24-v6':('contact-2.jpg','Low entry/exit are visible with the full body. Some final contact-sheet frames include later test cleanup; native stance recovery is established by the timestamped state evidence.'),
 'keyhole-emily-right-stand-v24-v6':('contact-1.jpg','Wider balcony view shows whole-body lowering, hold, rise and sword recovery. The still frames do not establish continuous temporal quality.'),
 'keyhole-emily-right-crouch-v24-v6':('contact-1.jpg','Crouched phases are visible but a nearby held NPC obstructs part of the frame. This capture is evidence of the tested behavior, not final visual acceptance.'),
 'keyhole-corvo-right-stand-v24-v7':('contact-1.jpg','Wider balcony view shows both boots and knees through entry, kneeling hold and standing recovery. Sword holsters and redraws; exact door contact still needs broader geometry testing.'),
}
for take,(contact,notes) in reviewed.items():
 assert (P/take/contact).is_file()
 (P/take/'visual-review.json').write_text(json.dumps(dict(inspected=True,accepted=False,method='Actual game frames at recorded timestamps, inspected as a contact sheet; no retiming or recoloring.',contact_sheets=[f'{take}/{contact}'],notes=notes,remaining='Continuous video review, other door geometry, arbitrary interruption and full lifecycle coverage.'),indent=2),encoding='utf-8')
print(*reviewed,sep='\n')
