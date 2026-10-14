const fs=require('fs');const report=JSON.parse(fs.readFileSync('outputs/d2-coop-lab/research/build-inspection.json'));
const b=fs.readFileSync(report.executable),base=Number(BigInt(report.imageBase));
function string(address){const rva=address-base,s=report.sections.find(s=>rva>=s.rva&&rva<s.rva+s.rawSize);if(!s)return '';const p=s.rawOffset+rva-s.rva,z=b.indexOf(0,p);return z>=p&&z-p<192?b.toString('ascii',p,z):'';}
const result={};
for(const type of ['params','result']){const text=fs.readFileSync(`work/damage-${type}-fields.txt`,'utf8'),rows=[];let pending='',at=0;
 for(const line of text.split('\n')){let m=line.match(/lea\s+rdx,\[([0-9A-F]+)h\]/i);if(m)pending=string(parseInt(m[1],16));
  m=line.match(/lea\s+rcx,\[([0-9A-F]+)h\]/i);if(m&&pending.startsWith('m_')){at=parseInt(m[1],16);rows.push({name:pending,metadata:at});pending='';}
 }
 for(const row of rows){for(const delta of [8,12]){const dest=(row.metadata+delta).toString(16).padStart(16,'0');const re=new RegExp('mov\\s+dword ptr \\['+dest+'h\\],([0-9a-f]+h|[0-9]+|esi|ebx|edi)','i'),m=text.match(re);if(m)row[delta===8?'offset':'bytes']=m[1].endsWith('h')?parseInt(m[1],16):/^\d+$/.test(m[1])?Number(m[1]):0;}
  if(row.bytes===0xffffffff){delete row.offset;delete row.bytes;row.accessorOnly=true;let rax=0;for(const line of text.split('\n')){let m=line.match(/lea\s+rax,\[([0-9A-F]+)h\]/i);if(m)rax=parseInt(m[1],16);m=line.match(/mov\s+qword ptr \[([0-9A-F]+)h\],rax/i);if(m){const address=parseInt(m[1],16);if(address===row.metadata+32)row.getter='0x'+(rax-base).toString(16);if(address===row.metadata+40)row.setter='0x'+(rax-base).toString(16);}}}
  row.metadata='0x'+(row.metadata-base).toString(16);}
 result[type]=rows.filter(row=>type!=='result'||parseInt(row.metadata)>=0x3b32700&&parseInt(row.metadata)<0x3b32990);
}
fs.writeFileSync('work/damage-reflected-layout.json',JSON.stringify(result,null,2));console.log(JSON.stringify(result,null,2));
