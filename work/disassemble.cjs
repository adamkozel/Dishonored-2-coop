const fs=require('node:fs'),cp=require('node:child_process');
const report=JSON.parse(fs.readFileSync('outputs/d2-coop-lab/research/build-inspection.json'));
const b=fs.readFileSync(report.executable);
const pdata=report.sections.find(s=>s.name==='.pdata');
const dump='C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/VC/Tools/MSVC/14.44.35207/bin/Hostx64/x64/dumpbin.exe';
for(const arg of process.argv.slice(2)) {
 const parts=arg.split(':');const rva=Number(parts[0]);let start=rva,end=parts[1]?Number(parts[1]):rva+512;
 for(let o=pdata.rawOffset;parts.length===1 && o<pdata.rawOffset+pdata.virtualSize;o+=12) {
  const a=b.readUInt32LE(o),z=b.readUInt32LE(o+4);
  if(rva>=a&&rva<z){start=a;end=z;break;}
 }
 const range=`/RANGE:0x${(BigInt(report.imageBase)+BigInt(start)).toString(16)},0x${(BigInt(report.imageBase)+BigInt(end)).toString(16)}`;
 const text=cp.execFileSync(dump,['/DISASM:NOBYTES',range,report.executable],{encoding:'utf8',maxBuffer:4000000});
 fs.writeFileSync(`work/disasm-${rva.toString(16)}.txt`,text);
 console.log(text.replace(/File Type: EXECUTABLE IMAGE\r?\n/,'').replace(/\s+Summary[\s\S]*$/,''));
}
