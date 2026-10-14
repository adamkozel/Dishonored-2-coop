const fs = require('node:fs');
const path = require('node:path');
const crypto = require('node:crypto');
const base = 'C:/Program Files (x86)/Steam/steamapps/common/Dishonored2/base';
const indexFiles = fs.readdirSync(base).filter(name => /^game\d+(?:_\d+|_patch)?\.index$/.test(name)).sort();
const maps = new Map(), indexes = [];
for (const name of indexFiles) {
  const bytes = fs.readFileSync(path.join(base, name));
  let cursor = 32;
  const count = bytes.readUInt32BE(cursor); cursor += 4;
  function string() {
    const length = bytes.readUInt32LE(cursor); cursor += 4;
    if (length > 100000 || cursor + length > bytes.length) throw Error(`Invalid string in ${name}`);
    const value = bytes.toString('utf8', cursor, cursor + length); cursor += length;
    return value;
  }
  const roots = [];
  for (let index = 0; index < count; index++) {
    cursor += 4;
    const type = string(), source = string(), destination = string();
    const offset = Number(bytes.readBigUInt64BE(cursor)); cursor += 8;
    const size = bytes.readUInt32BE(cursor), packed = bytes.readUInt32BE(cursor + 4);
    const flags2 = bytes.readUInt16BE(cursor + 16); cursor += 18;
    if (!/^maps\/.*\.entities$/.test(destination)) continue;
    const entry = { index: name, type, source, destination, offset, size, packed, flags2 };
    if (!maps.has(destination)) maps.set(destination, []);
    maps.get(destination).push(entry);
    roots.push(destination);
  }
  if (cursor !== bytes.length) throw Error(`Trailing bytes in ${name}: ${bytes.length - cursor}`);
  indexes.push({ name, sha256: crypto.createHash('sha256').update(bytes).digest('hex'), entries: count, maps: roots });
}
const result = {
  observedAt: new Date().toISOString(),
  method: 'Read-only parse of every installed game resource index; entity roots are packaged candidates, not proof of successful loading or modifiable geometry.',
  scope: 'No game processes, resources, saves, or configuration modified.',
  indexes,
  mapRoots: [...maps].map(([destination, entries]) => ({ destination, entries })),
  separateTestRoots: [...maps.keys()].filter(name => /(^|\/)(test[^/]*|sandbox|dev[^/]*)(\/|\.)/i.test(name)),
  tutorialRoots: [...maps.keys()].filter(name => /(^|\/)(tutorial|training)(\/|\.)/i.test(name))
};
const tutorialFile = 'work/extracted/maps_campaign_tutorial_tutorial_p.entities';
if (fs.existsSync(tutorialFile)) {
  const bytes = fs.readFileSync(tutorialFile), source = bytes.toString('utf8');
  const worldStart = source.indexOf('\tentityDef world {');
  const worldEnd = source.indexOf('\nentity {', worldStart);
  if (worldStart < 0 || worldEnd < worldStart) throw Error('Tutorial world block not found');
  const world = source.slice(worldStart, worldEnd);
  result.tutorialCandidate = {
    evidence: 'World entity from tutorial_p.entities, extracted through the game1_patch.index entry into scratch storage; static inspection only.',
    extractedSha256: crypto.createHash('sha256').update(bytes).digest('hex'),
    defaultPlayer: /m_defaultPlayer\s*=\s*"([^"]+)"/.exec(world)?.[1],
    emilyDefinition: /enumItem\[PLAYER_EMILY\]\s*=\s*([^;]+);/.exec(world)?.[1],
    corvoDefinition: /enumItem\[PLAYER_CORVO\]\s*=\s*([^;]+);/.exec(world)?.[1],
    navmeshSettings: /m_navmeshSettings\s*=\s*"([^"]+)"/.exec(world)?.[1],
    suitability: 'Not yet a Corvo/Emily test fixture: shipped world defaults to Emily and has no Corvo player definition. Loading, resource changes and both-role support remain unverified.'
  };
}
const output = 'outputs/d2-coop-lab/research/test-map-inventory.json';
fs.writeFileSync(output, JSON.stringify(result, null, 2) + '\n');
console.log(JSON.stringify({ indexes: indexes.length, mapRootCount: maps.size, separateTestRoots: result.separateTestRoots, tutorialRoots: result.tutorialRoots, tutorialCandidate: result.tutorialCandidate, output }, null, 2));
