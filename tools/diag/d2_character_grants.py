"""List character-gated story grants in the campaign maps' scripts (read-only; output is game-derived, keep it local
except the node table compiled into the mod).

A map script gives Corvo and Emily different items through one pattern (confirmed static, Dunwall's safe room):
CompareEntityDef(player, models/characters/player/<corvo|emily>/body.def) feeds an ActionTestBool, whose True and
False outputs go to two player-scoped action nodes (ActionPlayerInventory, ActionPlayerLoadout, Mana). The Kiscule
action mirror replays the host's node in the partner's world, so the partner needs the other branch instead.

Usage: python -I d2_character_grants.py <base-dir> [--json OUT]
Prints one row per gated pair: map, entity, Corvo node/input, Emily node/input and both nodes' item parameters.
"""
import argparse
import json
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import d2_resource_index as r  # noqa: E402
import d2_mission_route as route  # noqa: E402
from d2_kiscule_graph import parse_nodes  # noqa: E402

MIRRORED = ('arkKisculeNodeActionPlayerInventory', 'arkKisculeNodeActionPlayerLoadout', 'arkKisculeNodeMana')


def entity_files(base):
    seen = set()
    for ix in r.containers(base):
        for e in r.entries(ix):
            if e['type'] == 'File' and re.match(r'maps/campaign/.*\.entities$', e['source']) and e['source'] not in seen:
                seen.add(e['source'])
                yield e['source']


def pairs_in(body):
    nodes = list(parse_nodes(body))
    cls = {nid: c for nid, c, _, _ in nodes}
    params = {nid: dict(p) for nid, _, p, _ in nodes}
    names = {(nid, pid): pname for nid, _, _, pts in nodes for pid, pname, _, _ in pts}
    outs = {nid: [(pname, en, ec) for pid, pname, ptype, ends in pts if ptype.startswith('Output') for en, ec in ends]
            for nid, _, _, pts in nodes}
    for nid, c, _, _ in nodes:
        if c != 'arkKisculeNodeCompareEntityDef':
            continue
        b = params[nid].get('m_b', '')
        m = re.search(r'player/(corvo|emily)/body\.def', b)
        if not m:
            continue
        for pname, tb, tbin in outs.get(nid, []):
            if cls.get(tb) != 'arkKisculeNodeActionTestBool':
                continue
            equal = pname.replace('&gt;', '>').strip() in ('A == B',)
            true_is_corvo = (m.group(1) == 'corvo') == equal
            branch = {}
            for bname, target, tin in outs.get(tb, []):
                if bname in ('True', 'False') and cls.get(target, '').replace('Parameters', '') in MIRRORED:
                    who = 'corvo' if (bname == 'True') == true_is_corvo else 'emily'
                    branch[who] = (target, tin, names.get((target, tin), tin))
            if 'corvo' in branch and 'emily' in branch:
                yield {'compare': nid, 'test': tb, 'corvo': branch['corvo'], 'emily': branch['emily'],
                       'class': cls[branch['corvo'][0]].replace('arkKisculeNode', ''),
                       'corvoItem': params[branch['corvo'][0]].get('m_inventoryItem') or params[branch['corvo'][0]].get('m_loadout'),
                       'emilyItem': params[branch['emily'][0]].get('m_inventoryItem') or params[branch['emily'][0]].get('m_loadout')}


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('base')
    ap.add_argument('--json')
    a = ap.parse_args()
    rows = []
    for src in entity_files(a.base):
        _, text = route.load_entities(a.base, re.escape(src) + '$')
        for ent, body in route.split_entities(text).items():
            for p in pairs_in(body):
                row = {'map': src[len('maps/'):-len('.entities')], 'entity': ent, **p}
                rows.append(row)
                print(json.dumps(row), flush=True)
    if a.json:
        Path(a.json).write_text(json.dumps(rows, indent=1) + '\n', encoding='utf-8')


if __name__ == '__main__':
    main()
