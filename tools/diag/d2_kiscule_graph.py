"""Trace a Kiscule script graph in an extracted map script (maps/**/*.entities, d2_resource_index.py; read-only).

An entityDef's edit.m_kiscule holds the designer comments (m_comments) and the nodes: items with m_id, m_name (the node
class, arkKisculeNode...), m_parameters.m_object and m_connectionPoints. Each connection point lists its links as
{m_endNodeID, m_endConnectionPointID}. The file is game-derived: run it on a scratch copy and keep the output local.

Usage:
  python -I d2_kiscule_graph.py FILE --find TEXT [--entity NAME] [--up N] [--down N]
    every node whose class or parameters contain TEXT, with its links traced N levels upstream and downstream
  python -I d2_kiscule_graph.py FILE --id NODE [--entity NAME] [--up N] [--down N]
  python -I d2_kiscule_graph.py FILE --comments [--entity NAME]
    the designer comments, by position
"""
import argparse
import re

LEAF = re.compile(r'^(\S+(?: \S+)?) = (.*);$')


def parse(path):
    root = {'_children': []}
    stack = [root]
    with open(path, encoding='latin1') as f:
        for raw in f:
            line = raw.strip()
            if not line:
                continue
            if line == '}':
                if len(stack) > 1:
                    stack.pop()
                continue
            m = LEAF.match(line)
            if m and not line.endswith('{'):
                stack[-1].setdefault('_leaves', {})[m.group(1)] = m.group(2).strip('"')
                continue
            if line.endswith('{'):
                key = line[:-1].strip().rstrip('=').strip()
                node = {'_key': key, '_children': []}
                stack[-1]['_children'].append(node)
                stack.append(node)
    return root


def child(n, key):
    for c in n.get('_children', []):
        if c['_key'] == key:
            return c
    return None


def items(n):
    return [c for c in n.get('_children', []) if c['_key'].startswith('item[')] if n else []


def flatten(n, prefix=''):
    out = {}
    for k, v in n.get('_leaves', {}).items():
        out[prefix + k] = v
    for c in n.get('_children', []):
        out.update(flatten(c, prefix + c['_key'] + '.'))
    return out


def graphs(root, entity=None):
    def walk(n):
        if n.get('_key', '').startswith('entityDef '):
            name = n['_key'].split(' ', 1)[1]
            edit = child(n, 'edit')
            k = child(edit, 'm_kiscule') if edit else None
            if k and (entity is None or name == entity):
                yield name, k
            return
        for c in n.get('_children', []):
            yield from walk(c)
    yield from walk(root)


def nodes_of(k):
    out = {}
    for block in k.get('_children', []):
        for it in items(block):
            lv = it.get('_leaves', {})
            if lv.get('m_name', '').startswith('arkKiscule'):
                params = flatten(child(it, 'm_parameters') or {})
                points = {}
                for cp in items(child(it, 'm_connectionPoints')):
                    cl = cp.get('_leaves', {})
                    links = [(l.get('_leaves', {}).get('m_endNodeID'), l.get('_leaves', {}).get('m_endConnectionPointID'))
                             for l in items(child(cp, 'm_connections'))]
                    points[cl.get('m_id')] = (cl.get('m_name'), cl.get('m_type'), links)
                pos = child(it, 'm_nodePos')
                out[lv.get('m_id')] = {'cls': lv['m_name'], 'params': params, 'points': points,
                                        'pos': (pos.get('_leaves', {}).get('x'), pos.get('_leaves', {}).get('y')) if pos else None}
    return out


def label(nodes, nid):
    n = nodes.get(nid)
    if not n:
        return f'#{nid} (not in this graph)'
    keep = {k.split('.')[-1]: v for k, v in n['params'].items()
            if v not in ('', 'NULL', '0', 'false', 'true', '-1') and not k.endswith('m_class') and 'num' != k.split('.')[-1]}
    return f"#{nid} {n['cls'].replace('arkKisculeNode', '')} {keep}"[:260]


def trace(nodes, nid, depth, up, seen, indent=''):
    if depth <= 0 or nid in seen:
        return
    seen.add(nid)
    n = nodes.get(nid)
    if not n:
        return
    for pid, (pname, ptype, links) in n['points'].items():
        if not links or (up and not ptype.startswith('Input')) or (not up and not ptype.startswith('Output')):
            continue
        for end, endp in links:
            other = nodes.get(end)
            ename = other['points'].get(endp, ('?',))[0] if other else '?'
            print(f"{indent}{'<-' if up else '->'} {pname} {'from' if up else 'to'} [{ename}] {label(nodes, end)}")
            trace(nodes, end, depth - 1, up, seen, indent + '   ')


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('file')
    ap.add_argument('--entity')
    ap.add_argument('--find')
    ap.add_argument('--id', help='trace from this node id instead of --find')
    ap.add_argument('--up', type=int, default=3)
    ap.add_argument('--down', type=int, default=1)
    ap.add_argument('--comments', action='store_true')
    a = ap.parse_args()
    root = parse(a.file)
    for name, k in graphs(root, a.entity):
        if a.comments:
            for it in items(child(k, 'm_comments')):
                pos = child(it, 'm_pos')
                text = flatten(child(it, 'm_parameters') or {}).get('m_insideText', '')
                print(f"{name} {pos.get('_leaves', {}).get('x') if pos else ''},{pos.get('_leaves', {}).get('y') if pos else ''}: {text}")
            continue
        if not a.find and not a.id:
            continue
        nodes = nodes_of(k)
        for nid, n in nodes.items():
            if (a.id and nid == a.id) or (a.find and (a.find in n['cls'] or any(a.find in v for v in n['params'].values()))):
                print(f"== {name} {label(nodes, nid)} at {n['pos']}")
                trace(nodes, nid, a.up, True, set(), '   ')
                trace(nodes, nid, a.down, False, set(), '   ')


if __name__ == '__main__':
    main()
