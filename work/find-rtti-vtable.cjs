const fs=require('node:fs');
const r=JSON.parse(fs.readFileSync('outputs/d2-coop-lab/research/build-inspection.json'));
const b=fs.readFileSync(r.executable),base=BigInt(r.imageBase);
function off(rva){const s=r.sections.find(s=>rva>=s.rva&&rva<s.rva+s.rawSize);return s?s.rawOffset+rva-s.rva:null;}
for(const nameRva of process.argv.slice(2).map(Number)) {
  const typeRva=nameRva-16,locators=[],tables=[];
  for(const s of r.sections.filter(s=>s.name==='.rdata'))for(let o=s.rawOffset;o+24<s.rawOffset+s.rawSize;o+=4){
    if(b.readUInt32LE(o)===1&&b.readUInt32LE(o+12)===typeRva&&b.readUInt32LE(o+20)===s.rva+o-s.rawOffset)locators.push(s.rva+o-s.rawOffset);
  }
  for(const l of locators)for(const s of r.sections.filter(s=>s.name==='.rdata'))for(let o=s.rawOffset;o+8<s.rawOffset+s.rawSize;o+=8){
    if(b.readBigUInt64LE(o)===base+BigInt(l)) {
      const vt=s.rva+o-s.rawOffset+8,functions=[];
      for(let i=0;i<96;i++){const v=b.readBigUInt64LE(off(vt)+i*8)-base;if(v<0n||v>0x1a00000n)break;functions.push({slot:'0x'+(i*8).toString(16),rva:'0x'+v.toString(16)});}
      tables.push({vtable:'0x'+vt.toString(16),functions});
    }
  }
  console.log(JSON.stringify({typeRva:'0x'+typeRva.toString(16),locators,tables},null,2));
}

