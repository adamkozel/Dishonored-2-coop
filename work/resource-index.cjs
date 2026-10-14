const fs=require('node:fs'),zlib=require('node:zlib'),path=require('node:path');
const base='C:/Program Files (x86)/Steam/steamapps/common/Dishonored2/base';
const indexName=process.argv[2]||'game1_patch.index';
const b=fs.readFileSync(path.join(base,indexName));let o=32;
const count=b.readUInt32BE(o);o+=4;
function text(){const n=b.readUInt32LE(o);o+=4;if(n>100000||o+n>b.length)throw Error('Invalid string length');const t=b.subarray(o,o+n).toString('utf8');o+=n;return t;}
const entries=[];
for(let i=0;i<count;i++){
 const id=b.readUInt32BE(o);o+=4;const type=text(),source=text(),destination=text();
 const offset=Number(b.readBigUInt64BE(o));o+=8;const size=b.readUInt32BE(o),packed=b.readUInt32BE(o+4),flags1=b.readUInt16BE(o+14),flags2=b.readUInt16BE(o+16);o+=18;
 entries.push({id,type,source,destination,offset,size,packed,flags1,flags2});
}
if(o!==b.length)throw Error(`Index trailing bytes ${b.length-o}`);
const regex=new RegExp(process.argv[3]||'player|corvo|emily','i');
const matches=entries.filter(e=>regex.test(e.destination));
if(process.argv[4]==='export'){
 fs.mkdirSync('work/extracted',{recursive:true});
 for(const e of matches){const level=(e.flags2&0x7fff)>>2;const resource=(e.flags2&0x8000)?'shared_2_3.sharedrsc':`${indexName.split('_')[0].split('.')[0]}${level===0?'':level===6?'_patch':'_'+String(level).padStart(3,'0')}.resources`;
  const fd=fs.openSync(path.join(base,resource),'r');const p=Buffer.alloc(e.packed);fs.readSync(fd,p,0,p.length,e.offset);fs.closeSync(fd);
  const data=e.size===e.packed?p:zlib.inflateSync(p);if(data.length!==e.size)throw Error('Invalid extracted length');
  const name=e.destination.replace(/[^\w.-]/g,'_');fs.writeFileSync(path.join('work/extracted',name),data);
  console.log(JSON.stringify({...e,resource,extracted:path.join('work/extracted',name)}));
 }
}else console.log(JSON.stringify(matches,null,2));
fs.writeFileSync(`work/${indexName}.json`,JSON.stringify(entries));
