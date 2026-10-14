const fs = require('node:fs');
const report = JSON.parse(fs.readFileSync('outputs/d2-coop-lab/research/build-inspection.json'));
const b = fs.readFileSync(report.executable);
const text = Buffer.from('Dishonored 2 is already running\0');
const offset = b.indexOf(text);
const section = report.sections.find(s => offset >= s.rawOffset && offset < s.rawOffset + s.rawSize);
const target = section.rva + offset - section.rawOffset;
const code = report.sections.find(s => s.name === '.text');
const refs = [];
for (let i = code.rawOffset; i < code.rawOffset + code.rawSize - 7; ++i) {
  if ((b[i] === 0x48 || b[i] === 0x4c) && (b[i+1] === 0x8d || b[i+1] === 0x8b) && (b[i+2] & 0xc7) === 5) {
    const rva = code.rva + i - code.rawOffset;
    if (rva + 7 + b.readInt32LE(i+3) === target) refs.push({rva:'0x'+rva.toString(16),fileOffset:'0x'+i.toString(16)});
  }
}
console.log(JSON.stringify({target:'0x'+target.toString(16),imageBase:report.imageBase,refs},null,2));
