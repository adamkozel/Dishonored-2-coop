const fs=require('node:fs');
const report=JSON.parse(fs.readFileSync('outputs/d2-coop-lab/research/build-inspection.json'));
const bytes=fs.readFileSync(report.executable);
const rows=[];
for(const section of report.sections.filter(s=>s.name!=='.text'&&s.name!=='.pdata')) {
 const data=bytes.subarray(section.rawOffset,section.rawOffset+section.rawSize).toString('latin1');
 for(const match of data.matchAll(/[\x20-\x7e]{6,}/g))if(/Kiscule|Midnight/i.test(match[0]))
  rows.push({rva:'0x'+(section.rva+match.index).toString(16),text:match[0]});
}
fs.writeFileSync('work/native-kiscule-strings.json',JSON.stringify(rows,null,2));
console.log(JSON.stringify(rows,null,2));

