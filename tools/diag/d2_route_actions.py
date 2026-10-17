"""Timed host actions for the route harness (lab only; drives the host lab instance).

Memories of Training (#73, held take route-tut-v1310b) needs inputs timed to
an NPC: a choke behind a walking Corvo, a pickpocket as he arrives at his
stand, crossbow shots aimed from the native eye, and the combat lessons'
block/parry/choke/fatality input cycles. Each action exits 0 on success and
prints one JSON line per attempt.

Usage: python -I d2_route_actions.py <repo> <action> '<json args>' | @<json file>
  chase_hold  {"npc": str, "dist": 0.85, "up": 1.0, "button": "RightShoulder", "ms": 3500, "ly": 0,
               "crouch": true, "until": task, "tries": 6}
              stand behind the NPC along its walking heading and hold a button (ly: walk after it)
  pickpocket  {"npc": str, "stand": [x, y], "radius": 0.9, "dist": 0.7, "up": 1.0, "pitch": 0,
               "until": task, "tries": 6}
              as the NPC walks into its stand, press X from behind while walking after it;
              "then": {"stand": [x, y, eyeZ], "yaw": deg, "until": task} uses a target at once after the theft
  shoot       {"target": entity, "stand": [x, y, eyeZ], "up": 0.08, "until": task?}
              aim from the native eye at the entity and fire the left-hand weapon (LT); with until,
              the task must leave the active list within 6 s
  cycle       {"face": str, "seq": [{"b": ["RightShoulder"], "ms": 1200}, {"ms": 320}], "until": task,
               "untilLogic": false, "ms": 180000, "faceEvery": 5}
              repeat an input sequence facing the NPC until the task leaves the active list
              (or, with untilLogic, the host leaves gameplay: a fatality scene)
Task names come from the gameflow name cache (work/natural-moveset/gameflow-names.local.json).
Pitch is set with Set-ObserverPitch.ps1 (setviewpos ignores its pitch argument).
"""
import datetime
import json
import math
import os
import struct
import subprocess
import sys
import time
import uuid

PWSH = r'C:\Users\adako\.cache\codex-runtimes\codex-primary-runtime\dependencies\native\powershell\pwsh.exe'
BUTTONS = {'RightShoulder': 0x200, 'LeftShoulder': 0x100, 'A': 0x1000, 'B': 0x2000, 'X': 0x4000, 'Y': 0x8000}
REPO = ''


def ps(command):
    return subprocess.run([PWSH, '-NoProfile', '-Command', command], capture_output=True, text=True)


def pid():
    return json.load(open(f'{REPO}/work/lab-instances/host/launch.json', encoding='utf-8-sig'))['pid']


def probe(*args):
    """A native read; retried like the harness's Q (a read can fail for a moment during a fade or restart)."""
    for attempt in range(12):
        r = subprocess.run([f'{REPO}/outputs/d2-coop-lab/bin/d2_runtime_probe.exe', str(pid()), *map(str, args)],
                           capture_output=True, text=True)
        if r.returncode == 0:
            try:
                return json.loads(r.stdout)
            except ValueError:
                pass
        time.sleep(0.25)
    raise RuntimeError(f'native read failed: {args}')


def status(suffix=''):
    for _ in range(40):
        try:
            return json.load(open(f'{REPO}/work/lab-instances/host/coop-status.json{suffix}', encoding='utf-8-sig'))
        except (OSError, ValueError):
            time.sleep(0.03)
    return {}


NAMES = None


def active_tasks():
    global NAMES
    if NAMES is None:
        try:
            NAMES = json.load(open(f'{REPO}/work/natural-moveset/gameflow-names.local.json', encoding='utf-8-sig'))
        except (OSError, ValueError):
            NAMES = {}
    nodes = status('.missionend.json').get('gameflowNodes') or []
    return [NAMES.get(str(n[0]), '#%d' % n[0]) for n in nodes if n[1] == 2]


def logic():
    return status().get('localGameLogicState')


def command(text):
    ps(f"& '{REPO}/outputs/d2-coop-lab/Send-D2Command.ps1' -Role host -Command '{text}' -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null")


def teleport(x, y, z, yaw, pitch=0.0):
    command(f'setviewpos {x:.3f} {y:.3f} {z:.3f} {yaw:.3f} 0')
    if pitch:
        time.sleep(0.25)
        ps(f"& '{REPO}/work/natural-moveset/Set-ObserverPitch.ps1' -Role host -Pitch {max(-70, min(70, pitch)):.2f} -Quick|Out-Null")


_last = [0]


def packet(buttons=(), ms=150, lt=0, rt=0, ly=0):
    """The Send-D2Input.ps1 packet (D2CI v1, 40 bytes), published by same-directory rename."""
    mask = 0
    for b in buttons:
        mask |= BUTTONS[b]
    seq = max(int(time.time() * 1000) % 4294967296, _last[0] + 1)
    _last[0] = seq
    expires = datetime.datetime.now(datetime.timezone.utc) + datetime.timedelta(milliseconds=ms)
    filetime = int((expires - datetime.datetime(1601, 1, 1, tzinfo=datetime.timezone.utc)).total_seconds() * 10_000_000)
    data = struct.pack('<IHHIQHBBhhhhQ', 0x49433244, 1, 40, seq, filetime, mask, lt, rt, 0, ly, 0, 0, 0)
    folder = f'{REPO}/work/lab-instances/host'
    temp = os.path.join(folder, 'packet-' + uuid.uuid4().hex + '.tmp')
    with open(temp, 'wb') as f:
        f.write(data)
    for _ in range(15):
        try:
            os.replace(temp, os.path.join(folder, 'input.bin'))
            return
        except PermissionError:
            time.sleep(0.01)
    os.remove(temp)


def npcs(sub):
    return [e for e in probe('--entities')['entities'] if sub in e['name'] and 'arkNPC' in e.get('type', '')]


def heading(sub, moving=0.2, timeout=60.0):
    """Last matching NPC and its walking heading (unit vector) from two samples; None if it never walks."""
    end = time.time() + timeout
    while time.time() < end:
        a = npcs(sub)
        time.sleep(0.25)
        b = npcs(sub)
        if a and b:
            dx, dy = b[-1]['position'][0] - a[-1]['position'][0], b[-1]['position'][1] - a[-1]['position'][1]
            n = math.hypot(dx, dy)
            if n > moving:
                return b[-1], (dx / n, dy / n)
    return None, None


def crouched():
    return 6 in [s['id'] for s in probe('--player-states')['active']]


def crouch():
    for _ in range(4):
        if crouched():
            return True
        packet(['B'], 150)
        time.sleep(0.9)
    return crouched()


def done(task):
    return not any(task == a for a in active_tasks())


def chase_hold(a):
    for k in range(a.get('tries', 6)):
        if a.get('crouch', True):
            crouch()
        npc, h = heading(a['npc'])
        if not npc:
            continue
        p, d = npc['position'], a.get('dist', 0.85)
        teleport(p[0] - d * h[0], p[1] - d * h[1], p[2] + a.get('up', 1.0), math.degrees(math.atan2(h[1], h[0])))
        time.sleep(0.12)
        packet([a.get('button', 'RightShoulder')], a.get('ms', 3500), ly=a.get('ly', 0))
        time.sleep(a.get('ms', 3500) / 1000 + 2.0)
        ok = done(a['until'])
        print(json.dumps({'try': k, 'npc': npc['name'], 'at': [round(v, 2) for v in p], 'done': ok}), flush=True)
        if ok:
            return 0
        time.sleep(3.0)
    return 1


def pickpocket(a):
    sx, sy = a['stand']
    for k in range(a.get('tries', 6)):
        crouch()
        hit = None
        end = time.time() + 60
        while time.time() < end and not hit:
            p0 = npcs(a['npc'])
            time.sleep(0.1)
            p1 = npcs(a['npc'])
            if not p0 or not p1:
                continue
            q0, q1 = p0[-1]['position'], p1[-1]['position']
            step = math.hypot(q1[0] - q0[0], q1[1] - q0[1])
            d0, d1 = math.hypot(q0[0] - sx, q0[1] - sy), math.hypot(q1[0] - sx, q1[1] - sy)
            if step > 0.03 and d1 < a.get('radius', 0.9) and d1 < d0:
                hit = (q1, ((q1[0] - q0[0]) / step, (q1[1] - q0[1]) / step))
        if not hit:
            continue
        p, h = hit
        d = a.get('dist', 0.7)
        teleport(p[0] - d * h[0], p[1] - d * h[1], p[2] + a.get('up', 1.0), math.degrees(math.atan2(h[1], h[0])), a.get('pitch', 0))
        packet(['X'], 400, ly=12000)
        time.sleep(1.2)
        ok = done(a['until'])
        print(json.dumps({'try': k, 'at': [round(v, 2) for v in p], 'done': ok, 'active': active_tasks()}), flush=True)
        if ok:
            return then_use(a['then']) if a.get('then') else 0
        time.sleep(4.0)
    return 1


def then_use(t):
    """Right after a pickpocket: stand at a use target and press X (longer each time) until its task is done."""
    end = time.time() + 6
    while time.time() < end and not any(t['until'] == x for x in active_tasks()):
        time.sleep(0.3)
    if not any(t['until'] == x for x in active_tasks()):
        print(json.dumps({'then': t['until'], 'error': 'task never active', 'active': active_tasks()}), flush=True)
        return 1
    teleport(*t['stand'], t.get('yaw', 0), t.get('pitch', 0))
    time.sleep(0.5)
    for ms in t.get('press', [250, 900, 900, 1500]):
        packet(['X'], ms)
        time.sleep(ms / 1000 + 1.2)
        if done(t['until']):
            print(json.dumps({'then': t['until'], 'done': True}), flush=True)
            return 0
    print(json.dumps({'then': t['until'], 'done': False, 'active': active_tasks()}), flush=True)
    return 1


def shoot(a):
    target = [e for e in probe('--entities')['entities'] if e['name'] == a['target']]
    if not target:
        print(json.dumps({'target': a['target'], 'error': 'absent'}))
        return 1
    p = target[0]['position']
    sx, sy, sz = a['stand']
    command(f'setviewpos {sx:.3f} {sy:.3f} {sz:.3f} {math.degrees(math.atan2(p[1] - sy, p[0] - sx)):.3f} 0')
    time.sleep(0.4)
    eye = probe('--player')['nativeViewPosition']
    yaw = math.degrees(math.atan2(p[1] - eye[1], p[0] - eye[0]))
    pitch = math.degrees(math.atan2(eye[2] - (p[2] + a.get('up', 0.08)), math.hypot(p[0] - eye[0], p[1] - eye[1])))
    teleport(sx, sy, sz, yaw, pitch)
    time.sleep(0.3)
    packet([], 300, lt=255)
    time.sleep(1.5)
    ok = True
    if a.get('until'):
        end = time.time() + 6
        while not done(a['until']) and time.time() < end:
            time.sleep(0.5)
        ok = done(a['until'])
    print(json.dumps({'target': a['target'], 'yaw': round(yaw, 2), 'pitch': round(pitch, 2), 'eye': [round(v, 2) for v in eye], 'done': ok}), flush=True)
    return 0 if ok else 1


def face(sub):
    found = npcs(sub)
    if not found:
        return
    p = found[-1]['position']
    me = status().get('localPosition') or [0, 0, 0]
    command(f'setviewpos {me[0]:.3f} {me[1]:.3f} {me[2] + 1.7:.3f} {math.degrees(math.atan2(p[1] - me[1], p[0] - me[0])):.2f} 0')


def cycle(a):
    start, n = time.time(), 0
    face(a['face'])
    while time.time() - start < a.get('ms', 180000) / 1000:
        for item in a['seq']:
            b = item.get('b') or []
            packet([b] if isinstance(b, str) else b, item['ms'] + 50, rt=item.get('rt', 0))
            time.sleep(item['ms'] / 1000)
        n += 1
        if (a.get('untilLogic') and logic() != 1) or done(a['until']):
            print(json.dumps({'cycles': n, 'seconds': round(time.time() - start, 1), 'logic': logic(), 'active': active_tasks()}), flush=True)
            return 0
        if n % a.get('faceEvery', 5) == 0:
            face(a['face'])
    print(json.dumps({'cycles': n, 'timeout': True, 'active': active_tasks()}), flush=True)
    return 1


def main():
    global REPO
    REPO = sys.argv[1]
    raw = sys.argv[3] if len(sys.argv) > 3 else '{}'
    args = json.load(open(raw[1:], encoding='utf-8-sig')) if raw.startswith('@') else json.loads(raw)
    return {'chase_hold': chase_hold, 'pickpocket': pickpocket, 'shoot': shoot, 'cycle': cycle}[sys.argv[2]](args)


if __name__ == '__main__':
    sys.exit(main())
