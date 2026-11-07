# 2142 (#271): is the native tall clip sword_gun_neutral a "pistol held, not aimed" pose that could replace sword_gun_idle
# (bank clip 201, a guard's side-on duel stance) as the remote avatar's standing pistol idle? Run in Blender (background):
# it reads the offline native samples (native-samples/*.edgepose, base_tall rig, nothing from the game or the lab) and
# measures, in the native source rig with its origin joint kept, the clip's facing (origin joint 0, and center_root 2), the
# head's facing (head joint 12's bind forward carried by its world rotation, because head 12 to nose 92 is not forward), and
# each arm (upper arm 7/14 to wrist 9/16): yaw from the origin facing (positive = to the character's left) and drop (wrist z
# minus upper-arm z, metres). It also compares sword_gun_neutral with sword_neutral (the additive reference that
# retarget_native.py uses for sword_*_add) and with sword_gun_additive composed on it (local Q = ref Q * delta Q, T = ref T +
# delta T). It then retargets the clips onto Corvo and Emily in memory with retarget_native.py's method (checked against
# native-retargeted/f45ecaa1f1f7a6f0-*.bin) and renders a still per clip in render_library.py's scene and style
# (1152x720 Workbench, ffmpeg captions, scaled to 640x400). Writes s4-review-v2135/source-<clip>.png (only for clips whose
# still does not exist yet), gun-neutral-measure-v2142.json and gun-candidates-v2142.json; temporary frames go to --tmp.
# Usage: blender -b --python preview_gun_neutral_v2142.py -- --tmp <scratch folder>
import bpy, sys, json, math, struct, hashlib, subprocess, argparse
from pathlib import Path
from mathutils import Matrix, Quaternion, Vector
P = Path(__file__).resolve().parent
sys.path.insert(0, str(P))
parser = argparse.ArgumentParser(); parser.add_argument('--tmp', required=True); parser.add_argument('--force-idle', action='store_true')
args = parser.parse_args(sys.argv[sys.argv.index('--') + 1:] if '--' in sys.argv else [])
TMP = Path(args.tmp); TMP.mkdir(parents=True, exist_ok=True)
from rig_math import read, qts, NativeRig
from edge_data import poses
from preview_skin import correct_calf_helpers

manifest = read(P / 'native-allcatalog-sample-manifest.json')['rows']
def row_for(alias):
    r = next(r for r in manifest if alias in r['aliases'] and r['status'] == 'native_pose_sampled_offline' and 'base_tall' in r['skeleton'])
    sp = poses(P / r['sample_file']); assert sp['sha256'] == r['sample_sha256']
    return r, sp

SR = NativeRig(read(P / 'base-tall.json')['joints']); SJ = SR.joints; SBG = SR.global_bind
names = [j['nativeName'] for j in SJ]; N = {n: i for i, n in enumerate(names)}
def flat(v): return Vector((v.x, v.y, 0.0)).normalized()
def yawd(v): return math.degrees(math.atan2(v.y, v.x))
def wrap(d): return (d + 180.0) % 360.0 - 180.0
# Bind forward: the toes' direction from the ankles (both feet), horizontal.
FWD = flat((SBG[N['left_toes_jnt']].translation - SBG[N['left_ankle_jnt']].translation) + (SBG[N['right_toes_jnt']].translation - SBG[N['right_ankle_jnt']].translation))
LEFT_CHECK = wrap(yawd(flat(SBG[N['left_arm_jnt']].translation - SBG[N['right_arm_jnt']].translation)) - yawd(FWD))
def carried(g, j): return flat(g[j].to_3x3() @ SBG[j].to_3x3().inverted() @ FWD)

def measure(local):
    g = SR.fk(local); face = yawd(carried(g, 0)); out = dict(origin_yaw=round(face, 2))
    out['center_root_from_facing'] = round(wrap(yawd(carried(g, N['center_root_jnt'])) - face), 2)
    out['head_from_facing'] = round(wrap(yawd(carried(g, N['center_head_jnt'])) - face), 2)
    out['nose_from_facing'] = round(wrap(yawd(flat(g[N['center_nose_jnt']].translation - g[N['center_head_jnt']].translation)) - face), 2)
    for side, a, w in (('left', 'left_arm_jnt', 'left_wrist_jnt'), ('right', 'right_arm_jnt', 'right_wrist_jnt')):
        A, W = g[N[a]].translation, g[N[w]].translation; d = W - A
        out[side + '_arm'] = dict(yaw_from_facing=round(wrap(yawd(flat(d)) - face), 2), drop_m=round(W.z - A.z, 3),
                                  pitch_deg=round(math.degrees(math.atan2(d.z, Vector((d.x, d.y)).length)), 1),
                                  reach_m=round(Vector((d.x, d.y)).length, 3), wrist_z=round(W.z, 3), arm_z=round(A.z, 3))
    return out

def local_of(frame): return [qts(v) for v in frame]
def compose(ref, delta):
    out = []
    for a, b in zip(ref, delta):
        q = Quaternion((a[3], *a[:3])) @ Quaternion((b[3], *b[:3])); v = list(b); v[:4] = [q.x, q.y, q.z, q.w]; v[4:7] = [a[k] + b[k] for k in range(4, 7)]; out.append(v)
    return out
def pose_gap(fa, fb, joints=range(2, 27)):
    ga, gb = SR.fk(local_of(fa)), SR.fk(local_of(fb)); worst = (0, None)
    for j in joints:
        ang = math.degrees(ga[j].to_quaternion().rotation_difference(gb[j].to_quaternion()).angle); ang = min(ang, 360.0 - ang)
        if ang > worst[0]: worst = (ang, names[j])
    return dict(max_world_angle_deg=round(worst[0], 2), joint=worst[1])

report = dict(script='preview_gun_neutral_v2142.py', label='confirmed static (offline native samples, no runtime)', rig='base_tall (native source)',
              conventions=dict(facing='origin joint 0 bind forward (toes from ankles) carried by its rotation', yaw_positive='to the character\'s left',
                               left_arm_check_deg=round(LEFT_CHECK, 1), drop='wrist z minus upper-arm z, m (negative = wrist below shoulder)',
                               head='center_head_jnt bind forward carried by its world rotation; nose_from_facing is head 12 to nose 92 for reference'), clips={})
src = {}
for alias in ('sword_gun_idle', 'sword_gun_neutral', 'sword_neutral', 'sword_idle', 'sword_gun_additive'):
    r, sp = row_for(alias); src[alias] = (r, sp); n = len(sp['frames'])
    frames = sorted({0, n // 2, n - 1}) if alias != 'sword_gun_additive' else []
    report['clips'][alias] = dict(id=r['id'], asset=r['asset'], frames=n, fps=sp['fps'], sample_file=r['sample_file'],
                                  samples={str(f): measure(local_of(sp['frames'][f])) for f in frames})
# Is sword_gun_additive a delta layer (near-identity locals) and what does it do on top of sword_gun_neutral?
add = src['sword_gun_additive'][1]['frames']; gn = src['sword_gun_neutral'][1]['frames'][0]
def max_delta(frame):
    angs = [(math.degrees(2 * math.acos(min(1.0, abs(v[3])))), names[i]) for i, v in enumerate(frame[:27])]
    return max(angs)
report['clips']['sword_gun_additive']['delta_check'] = {str(f): dict(max_local_angle_deg=round(max_delta(add[f])[0], 2), joint=max_delta(add[f])[1],
    max_translation_m=round(max(Vector(v[4:7]).length for v in add[f][2:27]), 4)) for f in range(len(add))}
report['clips']['sword_gun_additive']['on_sword_gun_neutral'] = {str(f): measure(local_of(compose(gn, add[f]))) for f in range(len(add))}
report['clips']['sword_gun_additive']['on_sword_neutral'] = {str(f): measure(local_of(compose(src['sword_neutral'][1]['frames'][0], add[f]))) for f in range(len(add))}
report['comparisons'] = dict(
    gun_neutral_f0_vs_sword_neutral_f0=pose_gap(gn, src['sword_neutral'][1]['frames'][0]),
    gun_neutral_f0_vs_f1=pose_gap(gn, src['sword_gun_neutral'][1]['frames'][1]),
    gun_neutral_f0_vs_gun_idle_f0=pose_gap(gn, src['sword_gun_idle'][1]['frames'][0]),
    gun_neutral_f0_vs_sword_idle_f0=pose_gap(gn, src['sword_idle'][1]['frames'][0]))

# Further candidates: every alias with gun or pistol in it, classified by name.
cands = []
for r in manifest:
    al = [a for a in r['aliases'] if 'gun' in a or 'pistol' in a]
    if not al: continue
    name = ' '.join(al); bad = [w for w in ('fire', 'reload', 'hit', 'death', 'die', 'equip', 'unequip', 'snapshot', 'turn', 'giveup', 'start', 'stop', 'in', 'out', 'outofsight', 'wakeup', 'check') if ('_' + w) in name or name.endswith(w)]
    cands.append(dict(aliases=al, asset=r['asset'], status=r['status'], frames=r.get('sampling', {}).get('frames'), role=r['source_role'],
                      loop_candidate=not bad, excluded_by=bad))
(P / 'gun-candidates-v2142.json').write_text(json.dumps(cands, indent=1))

# In-memory retarget, retarget_native.py's non-additive path.
def helpers(rig, local):
    J = rig.joints; BG = rig.global_bind; lookup = {j['nativeName']: i for i, j in enumerate(J)}; g = []
    for i, j in enumerate(J):
        n = j['nativeName']; side = 'left' if n.startswith('left') else 'right' if n.startswith('right') else ''; driver = None; p = j['parent']
        if side and 'helper' in n:
            if 'upper_leg' in n or 'thigh' in n: driver = side + '_femur_jnt'
            elif 'knee' in n: driver = side + '_tibia_jnt'
            elif 'arm01' in n and 'forearm' not in n: driver = side + '_arm_jnt'
            elif 'elbow' in n or 'forearm' in n: driver = side + '_elbow_jnt'
            elif 'wrist' in n: driver = side + '_wrist_jnt'
            elif 'ankle' in n: driver = side + '_ankle_jnt'
        if driver and lookup[driver] < i:
            k = lookup[driver]; local[i] = g[p].inverted() @ (g[k] @ BG[k].inverted() @ BG[i])
        g.append(local[i] if p < 0 else g[p] @ local[i])
def retarget(frame, role):
    target = NativeRig(read(P.parent / f'mixamo-crouch/native-{role}.json')['joints']); J = target.joints; B = target.bind; BG = target.global_bind
    scale = (BG[19].translation - BG[21].translation).length / (SBG[19].translation - SBG[21].translation).length
    mapping = {i: N[j['nativeName']] for i, j in enumerate(J) if j['nativeName'] in N}
    sl = local_of(frame); sl[0] = Matrix.Identity(4); sl[1] = Matrix.Identity(4); sg = SR.fk(sl)
    local = [m.copy() for m in B]; g = []
    for i, j in enumerate(J):
        p = j['parent']
        if i in mapping and i >= 2 and 'helper' not in j['nativeName']:
            k = mapping[i]; rotation = sg[k].to_quaternion().to_matrix() @ SBG[k].to_quaternion().to_matrix().inverted() @ BG[i].to_3x3()
            local[i] = (g[p].to_3x3().inverted() @ rotation).to_4x4(); local[i].translation = B[i].translation
            if i == 2: local[i].translation += (sl[k].translation - SR.bind[k].translation) * scale
        g.append(local[i] if p < 0 else g[p] @ local[i])
    local[0] = Matrix.Identity(4); local[1] = Matrix.Identity(4); helpers(target, local)
    return local
def row7(m):
    q = m.to_quaternion().normalized(); return [q.x, q.y, q.z, q.w, *m.translation]
check = {}
for role in ('corvo', 'emily'):
    meta = read(P / f'native-retargeted/f45ecaa1f1f7a6f0-{role}.json'); b = (P / meta['binary']).read_bytes(); n = meta['joints']
    assert hashlib.sha256(b).hexdigest() == meta['sha256']
    mine = [row7(m) for m in retarget(src['sword_gun_idle'][1]['frames'][0], role)]
    ref = [struct.unpack_from('<7f', b, 28 + j * 28) for j in range(n)]
    check[role] = round(max(min(max(abs(x - y) for x, y in zip(a[:4], c[:4])), max(abs(x + y) for x, y in zip(a[:4], c[:4]))) + max(abs(x - y) for x, y in zip(a[4:], c[4:])) for a, c in zip(mine, ref)), 7)
report['retarget_check_vs_native_retargeted_gun_idle_f0'] = check
assert all(v < 1e-4 for v in check.values()), check

# Render render_library.py's scene for one frame per clip.
srcp = (P / 'preview_natural_walk.py').read_text()
exec(compile(srcp.split("for name in ('idle'")[0], str(P / 'preview_natural_walk.py'), 'exec'), globals())
scene.render.resolution_x = 1152; scene.render.resolution_y = 720
scene.display.shading.color_type = 'MATERIAL'; scene.display.shading.show_shadows = True; scene.display.render_aa = 'FXAA'
verts = []; faces = []
for x in range(-12, 13):
    for y in range(-12, 13):
        at = len(verts); verts.extend([(x, y, -.02), (x + 1, y, -.02), (x + 1, y + 1, -.02), (x, y + 1, -.02)]); faces.append((at, at + 1, at + 2, at + 3))
grid = bpy.data.meshes.new('contact_grid'); grid.from_pydata(verts, [], faces); scene.collection.objects.link(bpy.data.objects.new('contact_grid', grid))
for i in range(2):
    mat = bpy.data.materials.new('floor' + str(i)); v = .17 if i else .21; mat.diffuse_color = (v, v, v, 1); grid.materials.append(mat)
for i, face in enumerate(grid.polygons): face.material_index = (i // 25 + i % 25) % 2
stills = {}
for alias in ('sword_gun_neutral', 'sword_gun_idle'):
    out = P / 's4-review-v2135' / f'source-{alias}.png'
    if out.exists() and not (alias == 'sword_gun_idle' and args.force_idle): stills[alias] = str(out) + ' (existing, not re-rendered)'; continue
    if alias == 'sword_gun_idle': out = TMP / 'source-sword_gun_idle-v2142.png'
    frame = src[alias][1]['frames'][0]; view = Vector((5.5, -7, 3.7)); forward = -view.normalized(); right = forward.cross(Vector((0, 0, 1))).normalized(); up = right.cross(forward).normalized()
    projected = []; posed = []
    for role, rig, J, B, sword in rigs:
        local = retarget(frame, role); correct_calf_helpers(J, B, local); offset = Vector((0, -.95 if role == 'corvo' else .95, 0)); rig.matrix_world = Matrix.Translation(offset); g = []
        for i, j in enumerate(J):
            rig.pose.bones[j['nativeName']].matrix_basis = B[i].inverted() @ local[i]; p = j['parent']; g.append(local[i] if p < 0 else g[p] @ local[i])
            if i in (2, 7, 9, 12, 14, 16, 19, 21, 22, 23, 25, 26):
                point = g[i].translation + offset; projected.append((point.dot(right), point.dot(up)))
        q = read(P.parent / 'fullbody-proof/inputs/client-target-sword.json' if role == 'corvo' else P.parent / 'crouch-grip-reference/host-target-sword.json')['quaternion']
        sword.matrix_world = rig.matrix_world @ g[17] @ Quaternion((q[3], -q[0], -q[1], -q[2])).normalized().to_matrix().to_4x4(); sword.hide_render = False
    minx = min(v[0] for v in projected); maxx = max(v[0] for v in projected); miny = min(v[1] for v in projected); maxy = max(v[1] for v in projected)
    camera.data.ortho_scale = max(4.25, (maxx - minx + .8), (maxy - miny + .8) * 1152 / 720)
    target = right * ((minx + maxx) * .5) + up * ((miny + maxy) * .5); camera.location = target + view
    camera.rotation_euler = (target - camera.location).to_track_quat('-Z', 'Y').to_euler()
    bpy.context.view_layer.update(); raw = TMP / f'{alias}-raw.png'; scene.render.filepath = str(raw); bpy.ops.render.render(write_still=True)
    vf = ("drawtext=fontfile='C\\:/Windows/Fonts/arial.ttf':text='" + alias + "  |  CORVO / EMILY':fontcolor=white:fontsize=22:x=20:y=20,"
          "drawtext=fontfile='C\\:/Windows/Fonts/arial.ttf':text='AUTHORING PREVIEW - native source frame 0 of " + str(len(src[alias][1]['frames'])) +
          " - gameplay binding pending':fontcolor=white:fontsize=16:x=20:y=h-28,scale=640:400")
    run = subprocess.run(['ffmpeg', '-hide_banner', '-loglevel', 'error', '-y', '-i', str(raw), '-vf', vf, '-frames:v', '1', str(out)], capture_output=True, text=True)
    if run.returncode: raise RuntimeError(run.stderr)
    stills[alias] = str(out)
report['stills'] = stills
(P / 'gun-neutral-measure-v2142.json').write_text(json.dumps(report, indent=1))
print('GUN_NEUTRAL_V2142_DONE', json.dumps(stills), flush=True)
