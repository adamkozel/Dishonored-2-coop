const fs=require('node:fs');
const r=JSON.parse(fs.readFileSync('outputs/d2-coop-lab/research/build-inspection.json')),b=fs.readFileSync(r.executable);
function off(v){const s=r.sections.find(s=>v>=s.rva&&v<s.rva+s.rawSize);return s?s.rawOffset+v-s.rva:null;}
function str(v){const o=off(v);if(o===null)return '';const z=b.indexOf(0,o);return b.subarray(o,Math.min(z,o+160)).toString('utf8');}
for(const arg of process.argv.slice(2)){
 const [v,n]=arg.split(':');const a=Number(v),o=off(a);if(o===null)continue;
 if(n==='str'){console.log(v,str(a));continue;}
 const rows=[];for(let i=0;i<Number(n||32);i++){const p=b.readBigUInt64LE(o+i*8);const rv=Number(p-BigInt(r.imageBase));rows.push({slot:i,rva:'0x'+rv.toString(16),string:str(rv)});}
 console.log(v,JSON.stringify(rows));
}
