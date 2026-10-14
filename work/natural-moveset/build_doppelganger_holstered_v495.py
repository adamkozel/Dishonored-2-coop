"""Append six complete-body, hands-free Doppelganger phase candidates.

Native source observations493/494 must pass before production binding.
Keep the previous325 clips byte-identical. Local first-person is untouched.
"""
import sys, struct, json, hashlib
from pathlib import Path
P = Path(__file__).resolve().parent
sys.path.insert(0, str(P))
from clip_io import *
from rig_math import ease

L = Library(['doppelganger-bank-source-manifest-v407.json'], 'doppelganger-holstered-sources-v495')
OUT = P / 'doppelganger-holstered-banks-v495'
OUT.mkdir(exist_ok=True)
reports = []
for role in ('corvo', 'emily'):
    data = (P/'doppelganger-crouch-banks-v490'/f'{role}-doppelganger-crouch-bank.bin').read_bytes()
    magic, version, n, count, fps, layout = struct.unpack_from('<8sIIIIQ', data)
    assert (magic, version, count) == (b'D2MOVE56', 56, 325)
    bank = bytearray(struct.pack('<8sIIIIQ', b'D2MOVE57', 57, n, 331, fps, layout) + data[32:])
    entry = 24 + 121*n*28
    rig = rig_for(role)
    _, initial = L.load(role, 'native_doppelganger_in')
    reference = initial[0]
    refG = rig.fk(reference)
    right, clips = [], []
    for j in range(n):
        k = j
        while k >= 0 and k != 16:
            k = rig.joints[k]['parent']
        if k == 16 and j != 16 and 'helper' not in rig.joints[j]['nativeName']:
            right.append(j)
    for phaseIndex, (phase, duration) in enumerate((('in',466),('loop',1200),('out',666))):
        meta, frames = L.load(role, 'native_doppelganger_'+phase)
        for context, (restid, crouched) in enumerate(((59, False), (76, True))):
            cid = 325 + phaseIndex*2 + context
            rest = [qts(struct.unpack_from('<7f', data, 32+restid*entry+24+j*28)) for j in range(n)]
            restG = rig.fk(rest)
            poses, errors, heights = [], [], []
            for f in range(120):
                t = f/119
                source = sample(frames,t)
                sourceG = rig.fk(source)
                pose = copy_pose(source)
                for j in range(2,27):
                    if j in (10,17):
                        continue
                    if crouched or j in (14,15,16):
                        gain = .55 if crouched and j in (2,18) else .8 if crouched and j in (3,4,5) else 1.
                        delta = reference[j].to_quaternion().inverted() @ source[j].to_quaternion()
                        translation = pose[j].translation.copy()
                        pose[j] = (rest[j].to_quaternion() @ Quaternion().slerp(delta,gain)).normalized().to_matrix().to_4x4()
                        pose[j].translation = translation
                if crouched:
                    pose[2].translation = rest[2].translation + (source[2].translation-reference[2].translation)*.4
                for j in right:
                    pose[j] = rest[j].copy()
                pose[17] = rest[17].copy()
                if crouched:
                    weight = ease(t/.25) if phase=='in' else 1-ease(t) if phase=='out' else 1.
                    for j in (7,8,9):
                        g = rig.fk(pose)
                        rig.orient(pose,j,g[j].to_quaternion().slerp(sourceG[j].to_quaternion(),weight).to_matrix())
                targets = [restG[j].translation+(sourceG[j].translation-refG[j].translation)*.4 if crouched else sourceG[j].translation for j in (21,25)]
                rotations = [restG[j].to_3x3()@refG[j].to_3x3().inverted()@sourceG[j].to_3x3() if crouched else sourceG[j].to_3x3() for j in (21,25)]
                pin_feet(rig,pose,targets,rotations)
                g = rig.fk(pose)
                errors.append(max((g[j].translation-targets[k]).length for k,j in enumerate((21,25))))
                heights.append(pose[2].translation.z)
                assert max((pose[j].translation-rig.bind[j].translation).length for j in range(3,27) if j!=17) < 1e-5
                poses.append(pose)
            assert max(errors)<.002 and min(heights)>.5, (role, phase, crouched, max(errors),min(heights))
            bank.extend(struct.pack('<IIIIff',cid,121,0,0,0.,duration/1000))
            for pose in poses+[poses[0]]:
                for matrix in pose:
                    bank.extend(struct.pack('<7f',*row(matrix)))
            alias = f'native_doppelganger_{phase}_{"crouched" if crouched else "standing"}_empty'
            L.save(rig,role,alias,poses,[identity() for _ in poses],'powers',
                   'Complete-body hands-free Doppelganger acting from407 body donor with independent balanced stance, changing casting arm and supported feet. Native first-person supplies phase timing/context only. No fixed wrists, root tracks, gameplay or effect events. Corvo offline rig comparison only; runtime and visual evidence required.',
                   [meta['sha256']],native_duration_ms=duration,stance=int(crouched),armed=False,
                   max_foot_target_error=max(errors),minimum_pelvis_height=min(heights))
            L.reports[-1]['equipment_policy'] = 'hands_free'
            (L.out/(L.reports[-1]['id']+'.json')).write_text(json.dumps(L.reports[-1],indent=2),encoding='utf-8')
            clips.append(dict(id=cid,variant=29+phaseIndex,alias=alias,crouched=crouched,armed=False,
                              max_foot_error=max(errors),minimum_pelvis_height=min(heights)))
    path = OUT/f'{role}-doppelganger-holstered-bank.bin'
    path.write_bytes(bank)
    assert bank[32:len(data)] == data[32:]
    reports.append(dict(role=role,bank=str(path.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest().upper(),
                        format='D2MOVE57',clips=331,prior_payload_unchanged=True,sources=clips,
                        visual_accepted=False,runtime_verified=False))
L.finish('doppelganger-holstered-source-manifest-v495.json')
(P/'doppelganger-holstered-bank-manifest-v495.json').write_text(json.dumps(reports,indent=2),encoding='utf-8')
print('DOPPELGANGER_HOLSTERED_BANKS_GENERATED',flush=True)
