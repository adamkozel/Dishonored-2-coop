"""Static mission route finder (read-only; game-derived output stays local).

For each gameflow task, objective or mission node set by a map's Kiscule
scripts (arkKisculeNodeActionGameflowTask / Objective / Mission), walk the
script graph backwards to the nodes that start the chain: trigger volumes,
usables, custom events, pickups, NPC deaths. Their entities are resolved to
spawn positions from the same .entities file. Also lists the map's
LevelTransition nodes with their roots.

Usage:
  python -I d2_mission_route.py <base-dir> <map .entities source regex> [--depth N] [--json out.json]
  python -I d2_mission_route.py <base-dir> <map regex> --event NAME [--event NAME ...]
    (where each custom event is sent from, and what starts those senders)
Example:
  python -I d2_mission_route.py work/asset-game/base "dunwall_escape_tower_p[.]entities$"
"""
import argparse
import json
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import d2_resource_index as r  # noqa: E402

ROOT_KINDS = ('MidnightControl', 'TriggerNotification', 'UsableNotification', 'GetCustomEvent', 'LoadingEnd', 'PickupNotification',
              'DeathNotification', 'AIStateNotification', 'InventoryNotification', 'ConversationNotification',
              'GameflowTaskNotification', 'GameflowObjectiveNotification', 'Timer', 'EntitySpawned', 'Notification')
TARGETS = ('arkKisculeNodeActionGameflowTask', 'arkKisculeNodeActionGameflowObjective', 'arkKisculeNodeActionGameflowMission',
           'arkKisculeNodeLevelTransition')


def load_entities(base, pattern):
    rx = re.compile(pattern, re.I)
    for ix in r.containers(base):
        for e in r.entries(ix):
            if e['type'] == 'File' and rx.search(e['source']):
                return e['source'], r.read(ix, e).decode('utf-8', 'replace')
    raise SystemExit('no entities file matches ' + pattern)


def split_entities(text):
    out = {}
    for m in re.finditer(r'\n\tentityDef ([A-Za-z0-9_]+) \{', text):
        start = m.end()
        end = text.find('\n\tentityDef ', start)
        out[m.group(1)] = text[start:end if end > 0 else len(text)]
    return out


def position(body):
    m = re.search(r'spawnPosition = \{\s*x = ([-\d.e]+);\s*y = ([-\d.e]+);\s*z = ([-\d.e]+);', body)
    return [round(float(v), 3) for v in m.groups()] if m else None


def graph(body):
    nodes = {}
    for m in re.finditer(r'm_id = (\d+);\s*m_name = "(ark[A-Za-z]+)";', body):
        nid, name = m.groups()
        nxt = re.search(r'\n\t{5}item\[\d+\] = \{\s*m_id = \d+;\s*m_name = "ark', body[m.end():])
        chunk = body[m.end(): m.end() + (nxt.start() if nxt else 20000)]
        head = chunk.split('m_connectionPoints')[0]
        params = {k: v.strip('"') for k, v in re.findall(r'(m_[A-Za-z]+) = ("[^"]*"|[-\d.]+|true|false);', head) if v != '""' and k != 'm_class'}
        nodes[nid] = {'name': name, 'params': params, 'ends': re.findall(r'm_endNodeID = (\d+);', chunk)}
    incoming = {}
    for nid, n in nodes.items():
        for e in n['ends']:
            incoming.setdefault(e, []).append(nid)
    return nodes, incoming


def roots(nodes, incoming, start, depth):
    found, seen, stack = [], set(), [(start, 0)]
    while stack:
        nid, d = stack.pop()
        if nid in seen or d > depth:
            continue
        seen.add(nid)
        n = nodes.get(nid)
        if not n:
            continue
        if d and any(k in n['name'] for k in ROOT_KINDS):
            found.append((d, nid, n['name'].replace('arkKisculeNode', ''), n['params']))
        for src in incoming.get(nid, []):
            stack.append((src, d + 1))
    return sorted(found)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('base')
    ap.add_argument('map')
    ap.add_argument('--depth', type=int, default=6)
    ap.add_argument('--json')
    ap.add_argument('--event',action='append',default=[])
    a = ap.parse_args()
    source, text = load_entities(a.base, a.map)
    ents = split_entities(text)
    pos = {name: position(body) for name, body in ents.items()}
    report = {'source': source, 'targets': []}
    if a.event:
        print(source)
        for ent, body in ents.items():
            if 'm_kiscule' not in body:
                continue
            nodes, incoming = graph(body)
            for nid, n in nodes.items():
                if n['name'] == 'arkKisculeNodeSendCustomEvent' and n['params'].get('m_eventName') in a.event:
                    print(f"[send {n['params']['m_eventName']}] ({ent}#{nid})")
                    for d, rid, kind, params in roots(nodes, incoming, nid, a.depth):
                        ref = params.get('m_listenedEntity') or params.get('m_eventName') or params.get('m_declMdnt') or ''
                        print(f'    d{d} {kind}: {ref} {pos.get(ref) or ""}')
        return
    for ent, body in ents.items():
        if 'm_kiscule' not in body:
            continue
        nodes, incoming = graph(body)
        for nid, n in nodes.items():
            if n['name'] not in TARGETS:
                continue
            label = n['params'].get('m_taskName') or n['params'].get('m_objectiveName') or n['params'].get('m_missionName') or n['params'].get('m_mapName', '')
            rs = []
            for d, rid, kind, params in roots(nodes, incoming, nid, a.depth):
                ref = params.get('m_listenedEntity') or params.get('m_eventName') or ''
                rs.append({'depth': d, 'kind': kind, 'ref': ref, 'position': pos.get(ref), 'params': {k: v for k, v in params.items() if k in ('m_maxNotificationCount', 'm_global')}})
            report['targets'].append({'script': ent, 'node': nid, 'kind': n['name'].replace('arkKisculeNode', ''), 'label': label, 'roots': rs})
    if a.json:
        Path(a.json).write_text(json.dumps(report, indent=1), encoding='utf-8')
    print(source)
    for t in report['targets']:
        print(f"[{t['kind']}] {t['label']}  ({t['script']}#{t['node']})")
        for x in t['roots'][:8]:
            print(f"    d{x['depth']} {x['kind']}: {x['ref']} {x['position'] or ''}")


if __name__ == '__main__':
    main()
