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
  pickpocket  {"npc": str, "stand": [x, y], "radius": 0.9, "dist": 0.7, "up": 1.0, "pitch": 0, "taps": 6, "next": task?,
               "until": task, "tries": 6}
              as the NPC walks into its stand, press X from behind while walking after it;
              "then": {"stand": [x, y, eyeZ], "yaw": deg, "until": task} uses a target at once after the theft
  shoot       {"target": entity, "stand": [x, y, eyeZ], "up": 0.08, "until": task?}
              aim from the native eye at the entity and fire the left-hand weapon (LT); with until,
              the task must be complete within 6 s
  cycle       {"face": str, "seq": [{"b": ["RightShoulder"], "ms": 1200}, {"ms": 320}], "until": task,
               "untilLogic": false, "ms": 180000, "faceEvery": 5}
              repeat an input sequence facing the NPC until the task is complete
              (or, with untilLogic, the host leaves gameplay: a fatality scene)
  grab        {"face": str, "until": task, "ms": 300000, "press": 700, "gap": 150, "hold": 5500, "state": 41}
              block in short RB presses; a press that becomes a grab (player state 41, the choke)
              is held on until the choke ends (choke an off-balance enemy)
Every action takes "role": "host" (default) or "client" (#197: a partner completing a non-lethal outcome).
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
ROLE = 'host'


def ps(command):
    return subprocess.run([PWSH, '-NoProfile', '-Command', command], capture_output=True, text=True)


def pid():
    return json.load(open(f'{REPO}/work/lab-instances/{ROLE}/launch.json', encoding='utf-8-sig'))['pid']


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
            return json.load(open(f'{REPO}/work/lab-instances/{ROLE}/coop-status.json{suffix}', encoding='utf-8-sig'))
        except (OSError, ValueError):
            time.sleep(0.03)
    return {}


NAMES = None


def task_states():
    """{name: native state} for the named gameflow nodes (2 active, 3 complete, 5 failed or cancelled)."""
    active_tasks()
    nodes = status('.missionend.json').get('gameflowNodes') or []
    return {NAMES.get(str(n[0]), '#%d' % n[0]): n[1] for n in nodes}


def completed(task):
    return task_states().get(task) == 3


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
    ps(f"& '{REPO}/outputs/d2-coop-lab/Send-D2Command.ps1' -Role {ROLE} -Command '{text}' -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null")


def teleport(x, y, z, yaw, pitch=0.0):
    command(f'setviewpos {x:.3f} {y:.3f} {z:.3f} {yaw:.3f} 0')
    if pitch:
        time.sleep(0.25)
        ps(f"& '{REPO}/work/natural-moveset/Set-ObserverPitch.ps1' -Role {ROLE} -Pitch {max(-70, min(70, pitch)):.2f} -Quick|Out-Null")


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
    folder = f'{REPO}/work/lab-instances/{ROLE}'
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
    """The nearest walking matching NPC and its heading (unit vector) from two samples; None if none walks.

    #197: Dunwall has two Ramsey entities (the audience one stands still), so each match is followed by its handle."""
    end = time.time() + timeout
    while time.time() < end:
        a = npcs(sub)
        time.sleep(0.25)
        b = npcs(sub)
        me = status().get('localPosition') or [0, 0, 0]
        before = {e['handle']: e['position'] for e in a}
        for e in sorted(b, key=lambda e: math.hypot(e['position'][0] - me[0], e['position'][1] - me[1])):
            q = before.get(e['handle'])
            if not q:
                continue
            dx, dy = e['position'][0] - q[0], e['position'][1] - q[1]
            n = math.hypot(dx, dy)
            if n > moving:
                return e, (dx / n, dy / n)
    return None, None


def crouched():
    """True or False; None while the player state is unreadable (a takedown or scene in progress)."""
    try:
        return 6 in [s['id'] for s in probe('--player-states')['active']]
    except RuntimeError:
        return None


def crouch():
    end = time.time() + 20
    while time.time() < end:
        c = crouched()
        if c:
            return True
        if c is False:
            packet(['B'], 150)
        time.sleep(0.9)
    return False


def done(task):
    """Completed (state 3); a task that only left the active list may have failed in a restart."""
    return completed(task)


def close_modal(limit=20.0):
    """A tutorial page (logic 12) pauses the game: close it as the route harness's settle does (B, then A).

    campaign-training-vm-v1697a/b: after the first choke attempt failed, the stealth lesson restarted under a tutorial
    page in both worlds, the trainer never walked again and every later try found no walking NPC."""
    end, n = time.time() + limit, 0
    while time.time() < end and status().get('localGameLogicState') == 12:
        packet(['B'] if n % 2 == 0 else ['A'], 300)
        n += 1
        time.sleep(1.5)
    if n:
        print(json.dumps({'closedModal': n, 'logic': status().get('localGameLogicState')}), flush=True)


def attempts(a, one):
    """Up to a['tries'] attempts; an attempt whose native reads stay unavailable is logged and retried."""
    for k in range(a.get('tries', 6)):
        close_modal()
        try:
            result = one(a, k)
        except RuntimeError as e:
            print(json.dumps({'try': k, 'error': str(e)}), flush=True)
            time.sleep(2.0)
            continue
        if result is not None:
            return result
    return 1


def chase_try(a, k):
    """None to try again, 0 when the task is done."""
    if a.get('crouch', True):
        crouch()
    npc, h = heading(a['npc'])
    if not npc:
        return None
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
    return None


def chase_hold(a):
    return attempts(a, chase_try)


def pickpocket_try(a, k):
    """None to try again; 0 (or the follow-up use's result) when the theft is done."""
    sx, sy = a['stand']
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
        return None
    p, h = hit
    d = a.get('dist', 0.7)
    teleport(p[0] - d * h[0], p[1] - d * h[1], p[2] + a.get('up', 1.0), math.degrees(math.atan2(h[1], h[0])), a.get('pitch', 0))
    # Several X taps while walking after him: a tap counts only once the use focus is on him.
    for _ in range(a.get('taps', 6)):
        packet(['X'], 120, ly=12000)
        time.sleep(0.12)
        packet([], 100, ly=12000)
        time.sleep(0.1)
    time.sleep(1.0)
    # Done means the next task is active (a stealth restart also drops the theft task for a moment).
    ok = any(a['next'] == x for x in active_tasks()) if a.get('next') else done(a['until'])
    print(json.dumps({'try': k, 'at': [round(v, 2) for v in p], 'done': ok, 'active': active_tasks()}), flush=True)
    if ok:
        if not a.get('then') or then_use(a['then']) == 0:
            return 0
        time.sleep(4.0)  # the follow-up use failed (a restart): steal the key again
        return None
    time.sleep(4.0)
    return None


def pickpocket(a):
    return attempts(a, pickpocket_try)


def interaction_sensor():
    """The player's interaction sensor component (Record-Coverage's FocusInit); None when it is not unique."""
    player = probe('--player')
    me = [e for e in probe('--entities')['entities'] if e.get('pointer') == player.get('player')]
    if len(me) != 1:
        return None
    sensors = [c for c in probe('--components', me[0]['handle'])['components']
               if c.get('type') == '.?AVarkCpntPlayerSensorInteraction@@']
    return sensors[0]['pointer'] if len(sensors) == 1 else None


def use_focus(sensor, ents):
    """(handle, name) of the sensor's target, as Record-Coverage's Focus reads it (word 0x80)."""
    words = [w for w in probe('--object', sensor)['words'] if w.get('offset') == '0x80']
    if len(words) != 1:
        return None, None
    h = int(words[0]['word'], 16) >> 32
    names = {int(e['handle'], 16): e['name'] for e in ents}
    return '0x%x' % h, names.get(h)


def pickpocket_focus_try(a, k):
    """1760 (#171): pickpocket_try that walks after him reading the use focus and presses X only while it is on him.

    With a['partner'] ({pos, yaw, crouch}) the VM partner is placed out of the trainer's sight first: he fails his lesson
    when he notices either player, and each lesson restart puts her back at the lesson start (campaign-training-vm-v1760a)."""
    sx, sy = a['stand']
    hide = a.get('partner')
    if hide:
        x, y, z = hide['pos']
        r = ps(f"& '{REPO}/work/natural-moveset/Hide-RoutePartner-v1760.ps1' -X {x} -Y {y} -Z {z} -Yaw {hide.get('yaw', 0)}"
               + (' -Crouch' if hide.get('crouch') else ''))
        print(json.dumps({'try': k, 'partner': (r.stdout or r.stderr).strip()[-200:]}), flush=True)
    # With a['wait_at'] ([x, y, z, yaw]) the host waits for the trainer there too: in the partner's game the host's avatar
    # waiting at the lesson start was noticed, and that restart reached both games (campaign-training-vm-v1760c).
    if a.get('wait_at'):
        teleport(*a['wait_at'])
        time.sleep(0.5)
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
            hit = (q1, ((q1[0] - q0[0]) / step, (q1[1] - q0[1]) / step), p1[-1]['name'])
    if not hit:
        return None
    p, h, npc = hit
    d = a.get('dist', 0.7)
    teleport(p[0] - d * h[0], p[1] - d * h[1], p[2] + a.get('up', 1.0), math.degrees(math.atan2(h[1], h[0])), a.get('pitch', 0))
    try:
        sensor = interaction_sensor()
    except RuntimeError:
        sensor = None
    trace, presses, walk = [], 0, time.time() + a.get('walk', 6.0)
    while sensor and time.time() < walk:
        packet([], a.get('step_ms', 900), ly=a.get('ly', 12000))
        try:
            ents = probe('--entities')['entities']
            fh, fname = use_focus(sensor, ents)
            q = [e for e in ents if e['name'] == npc]
            me = probe('--player')['candidatePosition']
        except RuntimeError as e:
            trace.append({'error': str(e)})
            continue
        dist = round(math.hypot(q[0]['position'][0] - me[0], q[0]['position'][1] - me[1]), 2) if q else None
        on = bool(fname and a['npc'] in fname)
        trace.append({'t': round(walk - time.time(), 1), 'focus': fname or fh, 'dist': dist, 'npc': bool(q)})
        if on and presses < a.get('presses', 3):
            packet(['X'], 150, ly=a.get('ly', 12000))
            presses += 1
            time.sleep(0.4)
            trace[-1]['pressed'] = True
            if any(a['next'] == x for x in active_tasks()):
                break
    time.sleep(1.0)
    ok = any(a['next'] == x for x in active_tasks())
    print(json.dumps({'try': k, 'npc': npc, 'at': [round(v, 2) for v in p], 'done': ok, 'presses': presses, 'sensor': sensor,
                      'active': active_tasks(), 'trace': trace}), flush=True)
    if ok:
        if not a.get('then') or then_use(a['then']) == 0:
            return 0
        time.sleep(4.0)
        return None
    time.sleep(4.0)
    return None


def pickpocket_focus(a):
    return attempts(a, pickpocket_focus_try)


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
        state = task_states().get(t['until'])
        if state == 3:
            print(json.dumps({'then': t['until'], 'done': True}), flush=True)
            return 0
        if state != 2:  # failed or cancelled: the lesson restarted
            break
    print(json.dumps({'then': t['until'], 'done': False, 'state': task_states().get(t['until']), 'active': active_tasks()}), flush=True)
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
    try:
        found = npcs(sub)
    except RuntimeError:
        return
    if not found:
        return
    me = status().get('localPosition') or [0, 0, 0]
    # #197: the nearest match (Dunwall has an audience Ramsey far from the one in the throne room).
    p = min(found, key=lambda e: math.hypot(e['position'][0] - me[0], e['position'][1] - me[1]))['position']
    command(f'setviewpos {me[0]:.3f} {me[1]:.3f} {me[2] + 1.7:.3f} {math.degrees(math.atan2(p[1] - me[1], p[0] - me[0])):.2f} 0')
    # 1760 (#171): setviewpos keeps the pitch; campaign-training-vm-v1760c swung 22 times at the ceiling (the crossbow
    # lesson left the view 33 degrees up), so a view more than 10 degrees off level is levelled first.
    try:
        f = probe('--player')['nativeViewOrientation']
        if abs(math.degrees(math.asin(max(-1.0, min(1.0, f[2]))))) > 10:
            ps(f"& '{REPO}/work/natural-moveset/Set-ObserverPitch.ps1' -Role {ROLE} -Pitch 0 -Quick|Out-Null")
    except (RuntimeError, KeyError, IndexError):
        pass


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


def player_state(sid):
    try:
        return sid in [x['id'] for x in probe('--player-states')['active']]
    except RuntimeError:
        return False


def grab(a):
    """Block in short presses; when a press turns into a grab (choke state), keep the button held until the choke ends."""
    start, n, grabs = time.time(), 0, 0
    face(a['face'])
    while time.time() - start < a.get('ms', 300000) / 1000:
        packet(['RightShoulder'], a.get('press', 700) + 100)
        t = time.time()
        grabbed = False
        while time.time() - t < a.get('press', 700) / 1000:
            if player_state(a.get('state', 41)):
                grabbed = True
                break
        if grabbed:
            grabs += 1
            packet(['RightShoulder'], a.get('hold', 5500))  # replaces the press while it is still down: no release
            time.sleep(a.get('hold', 5500) / 1000 + 0.5)
        packet([], a.get('gap', 150))
        time.sleep(a.get('gap', 150) / 1000)
        n += 1
        if done(a['until']):
            print(json.dumps({'presses': n, 'grabs': grabs, 'seconds': round(time.time() - start, 1), 'active': active_tasks()}), flush=True)
            return 0
        if n % a.get('faceEvery', 6) == 0:
            face(a['face'])
    print(json.dumps({'presses': n, 'grabs': grabs, 'timeout': True, 'active': active_tasks()}), flush=True)
    return 1


def vulnerable_windows():
    """Off-balance windows applied to this player's replicas so far (1214kc npc-stream vulnerableApplies, summed)."""
    return sum(int(e.get('vulnerableApplies', 0)) for e in status('.npc-stream.json').get('entries', []))


def grab_react(a):
    """Block in presses as grab does, but react to the stagger as a player would: when a new off-balance window reaches
    this player's replica (vulnerableApplies grows, 1214kc), release, face the target and tap grab until the choke
    starts or the window passes (route-dunwall-partnergrab-v1890b: three windows, no press landed in one)."""
    start, n, grabs, windows, taps = time.time(), 0, 0, 0, 0
    seen = vulnerable_applies_at_start = vulnerable_windows()
    face(a['face'])

    def tap_window():
        nonlocal taps
        face(a['face'])
        t = time.time()
        while time.time() - t < a.get('window', 1600) / 1000:
            packet([], 60)
            time.sleep(0.06)
            packet(['RightShoulder'], a.get('tap', 250) + 100)
            taps += 1
            t2 = time.time()
            while time.time() - t2 < a.get('tap', 250) / 1000:
                if player_state(a.get('state', 41)):
                    return True
        return False

    while time.time() - start < a.get('ms', 300000) / 1000:
        packet(['RightShoulder'], a.get('press', 700) + 100)
        t = time.time()
        grabbed = False
        while time.time() - t < a.get('press', 700) / 1000:
            if player_state(a.get('state', 41)):
                grabbed = True
                break
            now = vulnerable_windows()
            if now > seen:
                seen = now
                windows += 1
                grabbed = tap_window()
                break
        if grabbed:
            grabs += 1
            packet(['RightShoulder'], a.get('hold', 5500))  # replaces the press while it is still down: no release
            time.sleep(a.get('hold', 5500) / 1000 + 0.5)
        packet([], a.get('gap', 150))
        time.sleep(a.get('gap', 150) / 1000)
        n += 1
        if done(a['until']):
            print(json.dumps({'presses': n, 'grabs': grabs, 'windows': windows, 'taps': taps, 'seconds': round(time.time() - start, 1), 'active': active_tasks()}), flush=True)
            return 0
        if n % a.get('faceEvery', 6) == 0:
            face(a['face'])
    print(json.dumps({'presses': n, 'grabs': grabs, 'windows': windows, 'taps': taps, 'windowsBefore': vulnerable_applies_at_start, 'timeout': True, 'active': active_tasks()}), flush=True)
    return 1


def main():
    global REPO, ROLE
    REPO = sys.argv[1]
    raw = sys.argv[3] if len(sys.argv) > 3 else '{}'
    args = json.load(open(raw[1:], encoding='utf-8-sig')) if raw.startswith('@') else json.loads(raw)
    ROLE = args.get('role', 'host')
    if ROLE not in ('host', 'client'):
        raise SystemExit('role must be host or client')
    return {'chase_hold': chase_hold, 'pickpocket': pickpocket, 'pickpocket_focus': pickpocket_focus, 'shoot': shoot, 'cycle': cycle,
            'grab': grab, 'grab_react': grab_react}[sys.argv[2]](args)


if __name__ == '__main__':
    sys.exit(main())
