"""Adapt verified native Midnight API binding to one cosmetic Mesmerize model."""
from pathlib import Path
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab/src'
s=(S/'carry_victim_scene.h').read_text()
a=s.index('struct CarryVictimScene');s=s[a:]
api_start=s.index('bool carryVictimApis()');api_end=s.index('bool carryVictimInspect(')
s=s[:api_start]+s[api_end:]
s=s.replace('CarryVictimScene','MesmerizeVisualScene').replace('carryVictimInspect','mesmerizeVisualSceneInspect').replace('startCarryVictimScene','startMesmerizeVisualScene').replace('stopCarryVictimScene','stopMesmerizeVisualScene').replace('carryVictimResource','mesmerizeVisualResource').replace('const NativeWorldNpc& target','const MesmerizeVisualTarget& target')
s=s.replace('text!="AIGroup1"','text!="Mesmerize"').replace('||!memoryRead(base+0x32ce5d0,&aiGroup,8)||aiGroup!=groupName','')
s=s.replace('Binding binding{aiGroup,','Binding binding{groupName,')
s=s.replace('s.variant=variant;','s.variant=variant;')
s=s.replace('unsigned variant=2','unsigned variant=1')
s=s.replace('unsigned handle=0,variant=2','unsigned handle=0,variant=1')
s=s.replace('one NPC group','one cosmetic model group').replace('exactly one NPC','exactly one cosmetic').replace('Native stop has retired this exact handle.','Native stop has retired this exact handle.')
s=s.replace('groups=0,groupName=0,spawn=0,aiGroup=0','groups=0,groupName=0,spawn=0')
s='#pragma once\n// Bounded local model experiment. No gameplay/AI or native scene event tracks.\nstruct MesmerizeVisualTarget {std::uintptr_t actor=0;unsigned handle=0;};\nconst char* mesmerizeVisualResource(unsigned variant){\n return variant==1?"midnight/d2coop/mesmerize_visual_in.midnightscene":variant==2?"midnight/d2coop/mesmerize_visual_loop.midnightscene":variant==3?"midnight/d2coop/mesmerize_visual_out.midnightscene":nullptr;\n}\n'+s
(S/'mesmerize_visual_scene.h').write_text(s)
print('Generated cosmetic-only scene binding candidate')
