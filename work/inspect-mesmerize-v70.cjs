// Exact-PE read-only string locations for subsequent RTTI/RIP lookup.
const fs=require('node:fs'),crypto=require('node:crypto');
const layout=JSON.parse(fs.readFileSync('outputs/d2-coop-lab/research/build-inspection.json'));
const b=fs.readFileSync(layout.executable),sha=crypto.createHash('sha256').update(b).digest('hex').toUpperCase();
if(sha!=='B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180')throw Error('Exact executable required');
const rows=[];
for(const m of b.toString('latin1').matchAll(/[\x20-\x7e]{6,}/g)){
 if(!/mesmeriz/i.test(m[0]))continue;
 const s=layout.sections.find(s=>m.index>=s.rawOffset&&m.index<s.rawOffset+s.rawSize);
 if(s)rows.push({text:m[0],rva:'0x'+(s.rva+m.index-s.rawOffset).toString(16)});
}
const out={sha256:sha,strings:rows};fs.writeFileSync('work/movement-integration/mesmerize-strings-v70.json',JSON.stringify(out,null,2));
console.log(JSON.stringify(rows.filter(r=>/arkCpntPowerMesmerize|arkDeclCpntPowerMesmerize|arkCpntMesmer|arkDeclCpntMesmer/.test(r.text)),null,2));
