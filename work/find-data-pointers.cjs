const fs=require('node:fs');
const r=JSON.parse(fs.readFileSync('outputs/d2-coop-lab/research/build-inspection.json'));
const b=fs.readFileSync(r.executable);
for(const target of process.argv.slice(2).map(BigInt)) {
  const pointers=[];
  for(const s of r.sections.filter(s=>s.name!=='.text')) for(let o=s.rawOffset;o+8<=s.rawOffset+s.rawSize;o+=8) {
    if(b.readBigUInt64LE(o)===BigInt(r.imageBase)+target) pointers.push('0x'+(s.rva+o-s.rawOffset).toString(16));
  }
  console.log(target.toString(16),JSON.stringify(pointers));
}
