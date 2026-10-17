import json, os, sys
T = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, T)
H = json.load(open(os.path.join(T, 'bankhashes.json')))
ORIG = json.load(open(os.path.join(T, 'origins.json')))
import classify  # noqa: E402  (re-runs classification) -- provides ORIGIN
ORIGIN = classify.ORIGIN
BS = chr(92)
NM = r"C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\natural-moveset"
BASE = """native-gaits stance-banks-v1 sprint-banks-v1 lean-banks-v1 slide-banks-v1 air-banks-v1 combat-banks-v1 equipment-banks-v3 unarmed-special-banks-v1 vertical-peek-banks-v1 low-mantle-banks-v1 low-mantle-banks-v2 crouch-mantle-banks-v1 vault-banks-v1 power-banks-v1 air-combat-banks-v1 terrain-banks-v2 turn-banks-v1 crouch-turn-banks-v1 high-mantle-banks-v1 double-jump-banks-v1 hard-landing-banks-v1 hard-landing-crouch-banks-v1 keyhole-banks-v2 darkvision-banks-v1 darkvision-context-banks-v1 windblast-banks-v1 windblast-context-banks-v1 crouch-balance-banks-v1 balanced-crouch-power-banks-v3 ranged-banks-v1 crouched-ranged-banks-v3 crouched-ranged-banks-v4 carry-banks-v1 carry-release-banks-v1 carry-contact-banks-v2 carry-pickup-banks-v1 CPC carry-pickup-contact-banks-v5 crouched-pickup-banks-v1 crouched-pickup-banks-v3 high-throw-banks-v31 fetal-banks-v35 bendtime-banks-v40 bendtime-balanced-banks-v42 outofmana-banks-v45 outofmana-context-banks-v46 grenade-banks-v56 stun-banks-v61 crouched-grenade-banks-v62 mesmerize-banks-v77 mesmerize-context-banks-v132 domino-banks-v192 domino-context-banks-v243 swarm-banks-v267 swarm-context-banks-v272 possession-banks-v323 possession-banks-v334 possession-crouch-banks-v363 possession-crouch-release-banks-v371 doppelganger-banks-v407 doppelganger-crouch-banks-v490 doppelganger-holstered-banks-v495 shadowwalk-banks-v523 shadowwalk-sneak-banks-v548 heart-banks-v564 trap-banks-v569 trap-banks-v575 choke-banks-v581 choke-breakout-banks-v592 lethal-banks-v604 kick-banks-v625 heavy-banks-v633 parry-banks-v1022 swim-banks-v1037 climb-banks-v1042 use-banks-v1212""".split()
EXTRA_MAN = {'low-mantle-banks-v1': ('low-mantle-bank-manifest-v1.json', 'build_low_mantle_bank.py'),
             'carry-contact-banks-v2': ('carry-contact-bank-manifest-v2.json', 'build_carry_contact_bank_v2.py'),
             'carry-pickup-banks-v1': ('carry-pickup-bank-manifest-v1.json', 'build_carry_pickup_bank_v1.py'),
             'carry-pickup-contact-banks-v3': ('carry-pickup-contact-bank-manifest-v3.json', 'author_pickup_contact_v3.py'),
             'carry-pickup-contact-banks-v4': ('carry-pickup-contact-bank-manifest-v4.json', 'author_pickup_contact_v4.py'),
             'crouched-pickup-banks-v1': ('crouched-pickup-bank-manifest-v1.json', 'author_crouched_pickup_v1.py')}
PARENT_KEYS = ('source_bank_sha256', 'parent_sha256', 'parent', 'parent_bank_sha256', 'walk_run_source_sha256')
SHA2BANK = {v['sha256'].lower(): k for k, v in H.items()}


def step_dir(s, role):
    if s == 'CPC':
        return 'carry-pickup-contact-banks-v3' if role == 'corvo' else 'carry-pickup-contact-banks-v4'
    return s


def bank(d, role):
    k = [k for k in H if k.split(BS)[0] == d and os.path.basename(k).startswith(role)]
    return k[0], H[k[0]]


rows = []
final = {role: {c: ORIG[role][c]['origin'] for c in range(374)} for role in ('corvo', 'emily')}
prev = {}
for s in BASE:
    r = dict(step=s)
    for role in ('corvo', 'emily'):
        d = step_dir(s, role)
        man, builder = (ORIGIN[d][0], ORIGIN[d][1]) if d in ORIGIN else EXTRA_MAN.get(d, ('?', '?'))
        r[role + '_dir'] = d; r[role + '_manifest'] = man; r[role + '_builder'] = builder
        k, b = bank(d, role)
        r[role + '_file'] = k
        r[role + '_sha256'] = b['sha256']
        r['version'] = b['v']; r['clips'] = b['count']
        declared = None
        try:
            mj = json.load(open(os.path.join(NM, man), encoding='utf-8-sig'))
            ents = [e for e in (mj if isinstance(mj, list) else [mj]) if e.get('role') == role and e.get('bank', '').split(BS)[0] == d]
            for pk in PARENT_KEYS:
                if ents and isinstance(ents[0].get(pk), str) and len(ents[0][pk]) == 64:
                    declared = ents[0][pk].lower(); break
        except Exception:
            pass
        r[role + '_declared_parent'] = declared
        r[role + '_declared_parent_bank'] = SHA2BANK.get(declared) if declared else None
        p = prev.get(role)
        if p is None:
            r[role + '_added'] = list(range(b['count'])); r[role + '_replaced'] = []; r[role + '_payload_parent'] = None
        else:
            pk, pb = p
            r[role + '_payload_parent'] = pk
            r[role + '_added'] = list(range(pb['count'], b['count']))
            r[role + '_replaced'] = [i for i in range(pb['count']) if pb['clips'][i]['ph'] != b['clips'][i]['ph']]
        r[role + '_surviving'] = [c for c in range(374) if final[role][c] == d]
        prev[role] = (k, b)
    rows.append(r)
json.dump(rows, open(os.path.join(T, 'chain_table.json'), 'w'), indent=1)


def rng(lst):
    if not lst:
        return '-'
    out = []; s = p = lst[0]
    for x in lst[1:] + [None]:
        if x is not None and x == p + 1:
            p = x; continue
        out.append(f'{s}' if s == p else f'{s}-{p}')
        if x is not None:
            s = p = x
    return ','.join(out)


lines = ['| D2MOVE | bank dir | manifest | builder | clips | added | replaced in place | survive in v1212 | corvo sha256 / declared parent | emily sha256 / declared parent |',
         '|---|---|---|---|---|---|---|---|---|---|']
for r in rows:
    assert r['corvo_added'] == r['emily_added'] and r['corvo_replaced'] == r['emily_replaced'] and r['corvo_surviving'] == r['emily_surviving']
    dirs = r['corvo_dir'] if r['corvo_dir'] == r['emily_dir'] else f"{r['corvo_dir']} (corvo) / {r['emily_dir']} (emily)"
    man = r['corvo_manifest'] if r['corvo_manifest'] == r['emily_manifest'] else f"{r['corvo_manifest']} / {r['emily_manifest']}"
    bld = r['corvo_builder'] if r['corvo_builder'] == r['emily_builder'] else f"{r['corvo_builder']} / {r['emily_builder']}"
    dc = (r['corvo_declared_parent'] or 'n/a')[:12]; de = (r['emily_declared_parent'] or 'n/a')[:12]
    lines.append(f"| {r['version']} | {dirs} | {man} | {bld} | {r['clips']} | {rng(r['corvo_added'])} | {rng(r['corvo_replaced'])} | {rng(r['corvo_surviving'])} | {r['corvo_sha256'][:12]} / {dc} | {r['emily_sha256'][:12]} / {de} |")
open(os.path.join(T, 'chain_table.md'), 'w').write('\n'.join(lines) + '\n')
bad = []
for r in rows:
    for role in ('corvo', 'emily'):
        dp = r[role + '_declared_parent_bank']; pp = r[role + '_payload_parent']
        if r[role + '_declared_parent'] and dp != pp:
            bad.append((r['step'], role, dp, pp))
print('declared parent != chain parent:', bad)
print('steps with declared parent:', sum(1 for r in rows if r['corvo_declared_parent']), '/', len(rows))
