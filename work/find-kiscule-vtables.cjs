const fs=require('node:fs');
const report=JSON.parse(fs.readFileSync('outputs/d2-coop-lab/research/build-inspection.json'));
const bytes=fs.readFileSync(report.executable),base=BigInt(report.imageBase);
const strings=JSON.parse(fs.readFileSync('work/native-kiscule-strings.json'));
const rdata=report.sections.find(s=>s.name==='.rdata');
const output=[];
for(const type of strings.filter(s=>/^\.\?AVarkKiscule(Manager|Instance@@|Graph@@|NodeCinematic)/.test(s.text))) {
 const typeRva=Number(type.rva)-16;
 const locators=[];
 for(let off=rdata.rawOffset;off<rdata.rawOffset+rdata.rawSize-24;off+=4)
  if(bytes.readUInt32LE(off)===1&&bytes.readUInt32LE(off+12)===typeRva&&bytes.readUInt32LE(off+20)===rdata.rva+off-rdata.rawOffset)
   locators.push(rdata.rva+off-rdata.rawOffset);
 for(const locator of locators) {
  const search=Buffer.alloc(8);search.writeBigUInt64LE(base+BigInt(locator));
  for(let off=bytes.indexOf(search,rdata.rawOffset);off>=rdata.rawOffset&&off<rdata.rawOffset+rdata.rawSize;off=bytes.indexOf(search,off+8)) {
   const vtable=rdata.rva+off+8-rdata.rawOffset;
   const methods=[];for(let slot=0;slot<36;++slot){const p=bytes.readBigUInt64LE(off+8+slot*8);const rva=Number(p-base);if(rva<4096||rva>=rdata.rva)break;methods.push('0x'+rva.toString(16));}
   output.push({type:type.text,vtable:'0x'+vtable.toString(16),methods});
  }
 }
}
fs.writeFileSync('work/native-kiscule-vtables.json',JSON.stringify(output,null,2));console.log(JSON.stringify(output,null,2));

