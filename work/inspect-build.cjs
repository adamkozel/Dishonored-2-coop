const fs = require('node:fs');
const crypto = require('node:crypto');
const path = require('node:path');
const exe = process.argv[2];
const output = process.argv[3];
if (!exe || !output) throw new Error('Usage: node inspect-build.cjs executable output');
const data = fs.readFileSync(exe);
const pe = data.readUInt32LE(0x3c);
if (data.readUInt32LE(pe) !== 0x4550) throw new Error('Invalid PE');
const count = data.readUInt16LE(pe + 6);
const optional = pe + 24;
const sectionsOffset = optional + data.readUInt16LE(pe + 20);
const sections = Array.from({length: count}, (_, i) => {
  const offset = sectionsOffset + 40 * i;
  return {name: data.subarray(offset, offset + 8).toString('ascii').replace(/\0.*$/, ''),
    virtualSize: data.readUInt32LE(offset + 8), rva: data.readUInt32LE(offset + 12),
    rawSize: data.readUInt32LE(offset + 16), rawOffset: data.readUInt32LE(offset + 20)};
});
function rvaToOffset(rva) {
  const s = sections.find(s => rva >= s.rva && rva < s.rva + Math.max(s.virtualSize, s.rawSize));
  if (!s) throw new Error('RVA outside sections: ' + rva.toString(16));
  return s.rawOffset + rva - s.rva;
}
function cstring(offset) { return data.subarray(offset, data.indexOf(0, offset)).toString('ascii'); }
const imports = [];
const importRva = data.readUInt32LE(optional + 112 + 8);
let desc = rvaToOffset(importRva);
while (data.readUInt32LE(desc + 12)) {
  const dll = cstring(rvaToOffset(data.readUInt32LE(desc + 12)));
  let thunk = rvaToOffset(data.readUInt32LE(desc) || data.readUInt32LE(desc + 16));
  const names = [];
  while (data.readBigUInt64LE(thunk)) {
    const value = data.readBigUInt64LE(thunk);
    names.push(value & (1n << 63n) ? '#' + Number(value & 0xffffn) : cstring(rvaToOffset(Number(value)) + 2));
    thunk += 8;
  }
  imports.push({dll, names});
  desc += 20;
}
const allStrings = [...data.toString('latin1').matchAll(/[\x20-\x7e]{5,}/g)];
const targets = ['win_allowMultipleInstances','fs_savepath','r_fullscreen','r_windowWidth','r_windowHeight','win_pauseOnFocusLoss','in_controlInactiveWindow'];
const settings = targets.map(name => {
  const matches = allStrings.filter(m => m[0] === name).map(m => ({fileOffset:'0x' + m.index.toString(16), rva: (() => {
    const s = sections.find(s => m.index >= s.rawOffset && m.index < s.rawOffset + s.rawSize);
    return s ? '0x' + (s.rva + m.index - s.rawOffset).toString(16) : null;
  })()}));
  return {name, matches};
});
const interfaceStrings = allStrings.filter(m => /^(?:.*(?:Iggy|Cutscene|CutScene|Network|idPlayer|arkPlayer|idCmdSystem|idCVarSystem|SpawnEntity).*)$/.test(m[0]) && m[0].length < 180)
  .map(m => ({text:m[0], fileOffset:'0x' + m.index.toString(16)}));
const report = {executable:exe,sha256:crypto.createHash('sha256').update(data).digest('hex'),
  peMachine:'0x' + data.readUInt16LE(pe + 4).toString(16), imageBase:'0x' + data.readBigUInt64LE(optional + 24).toString(16),
  imageSize:data.readUInt32LE(optional + 56),sections,imports,settings,interfaceStrings};
fs.mkdirSync(path.dirname(output), {recursive:true});
fs.writeFileSync(output, JSON.stringify(report,null,2) + '\n');
console.log(JSON.stringify({sha256:report.sha256,settings,imports:imports.map(i=>i.dll),interfaceStringCount:interfaceStrings.length},null,2));
