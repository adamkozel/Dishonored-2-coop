"""Offline native-rig math. No first-person positional constraints or scene state."""
import json, math, re
from mathutils import Matrix, Quaternion, Vector

def read(path):
    return json.loads(path.read_text(encoding='utf-8-sig'))

def qts(q):
    m = Quaternion((q[3], *q[:3])).normalized().to_matrix().to_4x4()
    m.translation = Vector(q[4:7])
    return m

def ease(t):
    t = max(0, min(1, t))
    return t*t*(3-2*t)

class NativeRig:
    def __init__(self, joints):
        self.joints = joints
        self.bind = [qts(j['bindLocalQTS']) for j in joints]
        self.global_bind = self.fk(self.bind)

    def fk(self, local):
        out = []
        for j, m in zip(self.joints, local):
            p = j['parent']
            out.append(m if p < 0 else out[p] @ m)
        return out

    def orient(self, local, i, rotation):
        g = self.fk(local)
        p = self.joints[i]['parent']
        m = (g[p].to_3x3().inverted() @ rotation).to_4x4()
        m.translation = local[i].translation
        local[i] = m

    def solve(self, local, a, b, c, goal, pole):
        g = self.fk(local)
        p = g[a].translation
        l1, l2 = self.bind[b].translation.length, self.bind[c].translation.length
        v = goal-p
        if v.length < 1e-7:
            return
        dist = max(.0001, min(v.length, l1+l2-.0005))
        axis = v.normalized()
        side = (pole-p)-axis*(pole-p).dot(axis)
        if side.length < 1e-7:
            return
        side.normalize()
        along = (dist*dist+l1*l1-l2*l2)/(2*dist)
        middle = p+axis*along+side*math.sqrt(max(0, l1*l1-along*along))
        for i, child, dst in ((a,b,middle),(b,c,goal)):
            g = self.fk(local)
            rotation = (g[child].translation-g[i].translation).rotation_difference(dst-g[i].translation).to_matrix() @ g[i].to_3x3()
            self.orient(local, i, rotation)

def header_floats(text, name):
    body = re.search(name+r'[^=]*=\{(.*?)\};', text, re.S).group(1)
    return [float(v) for v in body.replace('{','').replace('}','').replace('f','').split(',') if v.strip()]

def preview_helpers(rig, local):
    """Approximate native skin helpers for offline review; D2 computes its own."""
    J=rig.joints; BG=rig.global_bind; lookup={j['nativeName']:i for i,j in enumerate(J)}; g=[]
    for i,j in enumerate(J):
        name=j['nativeName']; side='left' if name.startswith('left') else 'right' if name.startswith('right') else ''; driver=None; p=j['parent']
        if side and 'helper' in name:
            if 'upper_leg' in name or 'thigh' in name:driver=side+'_femur_jnt'
            elif 'knee' in name:driver=side+'_tibia_jnt'
            elif 'arm01' in name and 'forearm' not in name:driver=side+'_arm_jnt'
            elif 'elbow' in name or 'forearm' in name:driver=side+'_elbow_jnt'
            elif 'wrist' in name:driver=side+'_wrist_jnt'
            elif 'ankle' in name:driver=side+'_ankle_jnt'
        if driver and lookup[driver]<i:
            k=lookup[driver];local[i]=g[p].inverted()@(g[k]@BG[k].inverted()@BG[i])
        g.append(local[i] if p<0 else g[p]@local[i])
    from preview_skin import correct_calf_helpers
    correct_calf_helpers(J,rig.bind,local)
