const fs=require('node:fs');
const report=JSON.parse(fs.readFileSync('outputs/d2-coop-lab/research/build-inspection.json'));
const b=fs.readFileSync(report.executable),base=Number(BigInt(report.imageBase));
function string(address){const rva=address-base,s=report.sections.find(s=>rva>=s.rva&&rva<s.rva+s.rawSize);if(!s)return '';const p=s.rawOffset+rva-s.rva,z=b.indexOf(0,p);return z>=p&&z-p<192?b.toString('ascii',p,z):'';}
const text=fs.readFileSync(process.argv[2],'utf8'),rows=[];let pending='';
for(const line of text.split('\n')){let m=line.match(/lea\s+rdx,\[([0-9A-F]+)h\]/i);if(m)pending=string(parseInt(m[1],16));m=line.match(/lea\s+rcx,\[([0-9A-F]+)h\]/i);if(m&&pending.startsWith('m_')){rows.push({name:pending,metadata:parseInt(m[1],16)});pending='';}}
for(const row of rows){for(const delta of [8,12]){const dest=(row.metadata+delta).toString(16).padStart(16,'0'),re=new RegExp('mov\\s+dword ptr \\['+dest+'h\\],([0-9a-f]+h|[0-9]+|esi|ebx|edi)','i'),m=text.match(re);if(m)row[delta===8?'offset':'bytes']=m[1].endsWith('h')?parseInt(m[1],16):/^\d+$/.test(m[1])?Number(m[1]):null;}row.metadata='0x'+(row.metadata-base).toString(16);}
console.log(JSON.stringify(rows,null,2));
