const fs=require('fs');
const r=JSON.parse(fs.readFileSync('outputs/d2-coop-lab/research/build-inspection.json'));
const b=fs.readFileSync(r.executable);
for(const arg of process.argv.slice(2)){
 const [address,count='16']=arg.split(':');const v=Number(address),n=Number(count);
 const s=r.sections.find(s=>v>=s.rva&&v+n<=s.rva+s.rawSize);if(!s)throw Error('Out of section');
 console.log(address+': '+[...b.subarray(s.rawOffset+v-s.rva,s.rawOffset+v-s.rva+n)].map(x=>'0x'+x.toString(16).padStart(2,'0')).join(','));
}
