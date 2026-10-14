const fs=require('node:fs');
const report=JSON.parse(fs.readFileSync('outputs/d2-coop-lab/research/build-inspection.json'));
const bytes=fs.readFileSync(report.executable);
const text=report.sections.find(s=>s.name==='.text');
const pdata=report.sections.find(s=>s.name==='.pdata');
const target=Number(process.argv[2]);const needle=Buffer.alloc(4);needle.writeUInt32LE(target);
const results=[];
for(let off=bytes.indexOf(needle,text.rawOffset);off>=text.rawOffset&&off<text.rawOffset+text.rawSize;off=bytes.indexOf(needle,off+1)) {
 const rva=text.rva+off-text.rawOffset;let fn=0,end=0;
 for(let p=pdata.rawOffset;p<pdata.rawOffset+pdata.virtualSize;p+=12)if(rva>=bytes.readUInt32LE(p)&&rva<bytes.readUInt32LE(p+4)){fn=bytes.readUInt32LE(p);end=bytes.readUInt32LE(p+4);break;}
 results.push({rva:'0x'+rva.toString(16),fn:'0x'+fn.toString(16),end:'0x'+end.toString(16),nearby:bytes.subarray(off-4,off+8).toString('hex')});
}
console.log(JSON.stringify(results,null,2));
