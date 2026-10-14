const fs=require('node:fs');
const r=JSON.parse(fs.readFileSync('outputs/d2-coop-lab/research/build-inspection.json'));
const b=fs.readFileSync(r.executable),s=r.sections.find(s=>s.name==='.text'),p=r.sections.find(s=>s.name==='.pdata');
const target=Number(process.argv[2]);if(!Number.isInteger(target)||target<0)throw Error('Need a target RVA');
const a=[];
for(let o=s.rawOffset;o<s.rawOffset+s.rawSize-5;o++)if(b[o]===0xe8||b[o]===0xe9){
 const q=s.rva+o-s.rawOffset;if(q+5+b.readInt32LE(o+1)!==target)continue;
 let f=q,end=q+5;for(let z=p.rawOffset;z<p.rawOffset+p.virtualSize;z+=12)if(q>=b.readUInt32LE(z)&&q<b.readUInt32LE(z+4)){f=b.readUInt32LE(z);end=b.readUInt32LE(z+4);break;}
 a.push({call:'0x'+q.toString(16),fn:'0x'+f.toString(16),end:'0x'+end.toString(16)});
}console.log(JSON.stringify(a,null,2));

