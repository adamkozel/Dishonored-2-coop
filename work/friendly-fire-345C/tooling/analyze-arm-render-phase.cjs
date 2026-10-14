const fs=require('fs'),path=require('path'),vm=require('vm');
const lab=path.resolve(__dirname,'..');
let source=fs.readFileSync(path.join(__dirname,'verify-arm-stream.cjs'),'utf8');
source=source.replace("||r.DllSha256!==hash(path.join(lab,'bin/dinput8.dll'))","");
source=source.replace("p.phase===0","p.phase===3");
source=source.replaceAll('pre-physics','render-stage');
source=source.replace('arm-stream-verification.json','arm-stream-phase3-analysis.json');
vm.runInNewContext(source,{require,__dirname,process,console});
