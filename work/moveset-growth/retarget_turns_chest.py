"""Turn comparison with a chest-relative sword-ready constraint.

The blade follows the turning torso rather than holding its orientation in the
actor frame. Keep v1 and the component-feedback comparison for inspection.
"""
from pathlib import Path
P=Path(__file__).resolve().parent
source=(P/'retarget_turns.py').read_text().replace("OUT=P/'turns'","OUT=P/'turns-chest'").replace("P/'donors-feedback'","P/'donors-component-feedback'")
source=source.replace("self.hand=rg[16];self.binding", "self.hand=rg[16];self.chest=rg[5];self.binding")
old="self.solve(local,14,15,16,self.hand.translation,Vector((.02,-.64,1.2)));self.orient(local,16,self.hand.to_3x3())"
new="g=self.fk(local);delta=g[5]@self.chest.inverted();hand=delta@self.hand;pole=delta@Vector((.02,-.64,1.2))\n  self.solve(local,14,15,16,hand.translation,pole);self.orient(local,16,hand.to_3x3())"
assert old in source
source=source.replace(old,new)
exec(compile(source,str(P/'retarget_turns.py'),'exec'),{'__file__':str(P/'retarget_turns.py'),'__name__':'__main__'})
