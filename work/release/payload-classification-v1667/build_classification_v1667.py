"""#137: the release classification for bank 75/382 (shop-banks-v1409). The v1312 classification covers clips 0-373
(use-banks-v1212, unchanged through v1266); clips 374-381 were added by the four banks after it, each built from
retargeted native samples named in its manifest:
  374-377 death-banks-v1362 (sleep-dart collapses), 378-379 recoil-banks-v1365 (sword parry-lose),
  380 knockdown-banks-v1400 (sleep-dart fall and recovery), 381 shop-banks-v1409 (think and calm gestures).
Each manifest also states that the prior clips' payloads are unchanged. Read only on lab data.
Usage: python -I build_classification_v1667.py
"""
import gzip, json
from pathlib import Path
R = Path(__file__).resolve().parents[3]
NM = R / 'work/natural-moveset'
OLD = R / 'work/release/payload-classification-v1312/bank-clips.json.gz'
OUT = Path(__file__).resolve().parent
clips = json.load(gzip.open(OLD))
assert {(c['role'], c['clip']) for c in clips} == {(r, i) for r in ('corvo', 'emily') for i in range(374)}
banks = [('death-bank-manifest-v1362.json', 'build_death_bank_v1362.py', 'death-banks-v1362', 72, 'prior374PayloadsUnchanged'),
         ('recoil-bank-manifest-v1365.json', 'build_recoil_bank_v1365.py', 'recoil-banks-v1365', 73, 'prior378PayloadsUnchanged'),
         ('knockdown-bank-manifest-v1400.json', 'build_knockdown_bank_v1400.py', 'knockdown-banks-v1400', 74, 'prior380PayloadsUnchanged'),
         ('shop-bank-manifest-v1409.json', 'build_shop_bank_v1409.py', 'shop-banks-v1409', 75, 'prior381PayloadsUnchanged')]
purpose = {'death-banks-v1362': 'partner death collapse ({})', 'recoil-banks-v1365': 'parry-lose stagger ({})',
           'knockdown-banks-v1400': 'knockdown fall, lie and get-up', 'shop-banks-v1409': 'shop gesture loop (think, calm)'}
added = []
for manifest, builder, bank, version, unchanged in banks:
    for m in json.load(open(NM / manifest, encoding='utf-8-sig')):
        assert m[unchanged] is True and m['version'] == version
        for a in m['added']:
            if 'sources' in a:
                src = list(a['sources'].values())
                assets = [s['asset'] for s in src]; donors = [s['retargeted'] for s in src]; shas = [s.get('sha256') or s.get('retargetedSha256') for s in src]
            else:
                assets = [a['asset']]; donors = [a['retargeted']]; shas = [a['retargetedSha256']]
            label = a.get('direction') or a.get('side') or ''
            added.append(dict(
                role=m['role'], clip=a['clip'], category='native-sampled', purpose=purpose[bank].format(label),
                donor=donors, donorSha256=shas, donorExists=all((NM / d).exists() for d in donors),
                donorExistsDetail=[dict(donor=d, exists=(NM / d).exists()) for d in donors],
                baseClips=[], timingReferenceClips=[], manifest=f'work/natural-moveset/{manifest}',
                builder=f'work/natural-moveset/{builder}', originBank=f'work/natural-moveset/{bank}', version=version,
                firstAddedIn=f'work/natural-moveset/{bank}', replacedInPlaceBy=[], components=['native-sampled'],
                nativeAssets=sorted(set(assets)), mixamoSources=[], containsNativeAnimationData=True, containsMixamoData=False,
                embedsNativeBindTranslations=True, note='Retargeted native clip(s), resampled; added after v1312.',
                evidence=[f'bank manifest {manifest}: clip {a["clip"]} added with these sources; {unchanged} true [confirmed static]']))
assert sorted((c['role'], c['clip']) for c in added) == sorted((r, i) for r in ('corvo', 'emily') for i in range(374, 382))
allc = sorted(clips + added, key=lambda c: (c['role'], c['clip']))
(OUT / 'bank-clips.json').write_text(json.dumps(allc, indent=1), encoding='utf-8')
with gzip.open(OUT / 'bank-clips.json.gz', 'wt', encoding='utf-8') as g:
    json.dump(allc, g)
print(json.dumps(dict(clips=len(allc), added=len(added), missingDonors=[c for c in added if not c['donorExists']]), indent=1)[:2000])
