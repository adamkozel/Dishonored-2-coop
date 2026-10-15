"""Audit 1288 (#14): a real native grenade thrown at a bound guard in product mode, in both roles (takes grenade-real-v1273b,
v1273c), with the lab item grant (1214au). Writes the audit, updates chapter 95 and the current state. Takes are never
rewritten."""
from pathlib import Path
import json, datetime
R = Path(__file__).resolve().parents[2]; W = R / 'work/movement-integration'; N = R / 'work/natural-moveset'
D = R / 'documentation/handoff-2026-10-11'
rd = lambda p: json.loads(p.read_text(encoding='utf-8-sig'))
def edit(path, old, new):
    s = path.read_bytes().decode('utf-8')
    if '\r\n' in s: old = old.replace('\n', '\r\n'); new = new.replace('\n', '\r\n')
    assert s.count(old) == 1, (path.name, s.count(old), old[:80]); path.write_bytes(s.replace(old, new).encode('utf-8'))
at = datetime.datetime.now().astimezone().isoformat()
rows = []
for t in ['grenade-real-v1273b-host', 'grenade-real-v1273b-client', 'grenade-real-v1273c-host', 'grenade-real-v1273c-client']:
    r = rd(N / t / 'result.json'); tr = rd(N / t / 'throws.json'); tr = tr if isinstance(tr, list) else [tr]
    rows.append(dict(take=t, passed=r.get('passed'), failure=r.get('failure'), throws=[dict(attempt=x.get('attempt'), before=x.get('before'), after=x.get('after')) for x in tr]))
findings = [
    'Lab fixture (1214au, lab only): coop_grant_item grenade|health|mana adds one native item through the native inventory add (+0x618de0) and selects a grenade with the native equip (+0x614b20) in product mode; the isolated grenade fixture refused product mode (1273a: unsupported_scope_or_item_owned). Takes restore the inventory baseline.',
    'Confirmed runtime (1273b, both roles): granted and equipped, both grenades exploded against the office door between the thrower and the guard and blew the door apart; the guard was untouched (40 -> 40). Whether the destroyed door is shared between the worlds was not filmed (open, physics/destructibles).',
    'Confirmed runtime (1273c, host thrower, 3.5 m in front of the guard): the guard dropped 40 -> 7.49 in both worlds (host-native pass-through of explosion_grenade, HostWorld commit).',
    'Confirmed runtime (1273c, client thrower): the guard died (40 -> 0) in both worlds through the PeerNative request the host applies.',
    'Visual (1273c-client, host free camera): the grenade explodes at the guard and the guard collapses; the window behind overexposes the view.']
(W / 'real-grenade-audit-v1288.json').write_text(json.dumps(dict(at=at, candidate='candidate-npc-combat-v1214au', takes=rows, findings=findings, complete=True,
    scope='#14: real grenade on a bound guard in product mode, both roles.'), indent=2), encoding='utf-8')
edit(D / '95-shared-combat-authority.md', "## Open\n",
"""## Real grenades in product mode (1214au, audit 1288)

The lab-only `coop_grant_item grenade|health|mana` adds one native item in product mode. It uses the native inventory add (+0x618de0) and, for a grenade, the native equip (+0x614b20). The isolated grenade fixture refused product mode (take 1273a).

| Take | Thrower | Result |
|---|---|---|
| [grenade-real-v1273b-host](../../work/natural-moveset/grenade-real-v1273b-host) | host | No hit: the grenade blew apart the office door between thrower and guard (whether the door break is shared is open) |
| [grenade-real-v1273c-host](../../work/natural-moveset/grenade-real-v1273c-host) | host | Pass: guard 40 → 7.49 in both worlds (host-native pass-through, `HostWorld` commit) |
| [grenade-real-v1273c-client](../../work/natural-moveset/grenade-real-v1273c-client) | client | Pass: guard dead in both worlds (`PeerNative` request applied by the host) |

## Open
""")
edit(D / '95-shared-combat-authority.md',
"- Grenades and powers in both roles are verified through the lab fixture only (1274, 1275). Real sleep darts and chokes on a mission are verified (1276i, 1276m); a real grenade or power is not.",
"- Real grenades in both roles are verified in product mode (1273c, audit 1288); real sleep darts and chokes on a mission (1276i, 1276m). A real power on a guard and destroyed doors and props remain open.")
edit(D / '01-current-state.md', "Live candidate-npc-combat-v1214at ([identity](../../work/movement-integration/candidate-npc-combat-v1214at/identity.json))",
     "Live candidate-npc-combat-v1214au ([identity](../../work/movement-integration/candidate-npc-combat-v1214au/identity.json))")
edit(D / '01-current-state.md', "protocol 75, bank 71/374 (bank 1266). Latest:\n",
"""protocol 75, bank 71/374 (bank 1266). Latest:
- **Real grenades (#14, 1214au, [audit 1288](../../work/movement-integration/real-grenade-audit-v1288.json)):** a real grenade thrown at a guard in product mode lands in both worlds, from either player ([chapter 95](95-shared-combat-authority.md#real-grenades-in-product-mode-1214au-audit-1288)). The lab can grant one native grenade, health or mana potion in product mode.
""")
s = rd(R / 'work/current-task-state.json'); s['UpdatedAt'] = at; m = s['MovementExecutionPriority']
m['current'] = m['current'].replace('1214at live', '1214au live') + ' Real grenades verified in both roles in product mode (audit 1288).'
(R / 'work/current-task-state.json').write_bytes(json.dumps(s, indent=2, ensure_ascii=True).replace('\n', '\r\n').encode('utf-8'))
print('ok')
