const fs=require('node:fs'); const r=JSON.parse(fs.readFileSync('outputs/d2-coop-lab/research/build-inspection.json'));const b=fs.readFileSync(r.executable);const s=r.sections.find(s=>s.name==='.text');const p=r.sections.find(s=>s.name==='.pdata');const a=[];
for(let o=s.rawOffset;o<s.rawOffset+s.rawSize-5;o++)if(b[o]===0xe8){let q=s.rva+o-s.rawOffset;if(q+5+b.readInt32LE(o+1)===0x48ad0){let f=q;for(let z=p.rawOffset;z<p.rawOffset+p.virtualSize;z+=12)if(q>=b.readUInt32LE(z)&&q<b.readUInt32LE(z+4)){f=b.readUInt32LE(z);break;}a.push({call:q.toString(16),fn:f.toString(16)});}}
console.log(a);
