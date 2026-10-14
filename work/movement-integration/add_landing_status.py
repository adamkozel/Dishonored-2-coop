from pathlib import Path
p=Path('outputs/d2-coop-lab/src/movement_playback.h');s=p.read_text(encoding='utf-8')
old=r'\"secondJumpTransition\":%.6f}'
assert old in s
s=s.replace(old,r'\"secondJumpTransition\":%.6f,\"landingCrouchMix\":%.6f}')
old='p.secondJump.progress,p.secondJump.transition);';assert old in s
s=s.replace(old,'p.secondJump.progress,p.secondJump.transition,p.landingCrouchMix);')
p.write_text(s,encoding='utf-8')
