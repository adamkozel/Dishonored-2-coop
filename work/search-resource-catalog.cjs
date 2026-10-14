const fs=require('node:fs'),path=require('node:path');
const base='C:/Program Files (x86)/Steam/steamapps/common/Dishonored2/base';
const pattern=new RegExp(process.argv[2]||'crouch|sneak|jump|doppelganger','i');
const entries=[];
for(const index of fs.readdirSync(base).filter(n=>/^game\d+(?:_\d+|_patch)?\.index$/.test(n))){
 const b=fs.readFileSync(path.join(base,index));let o=32;const count=b.readUInt32BE(o);o+=4;
 function text(){const n=b.readUInt32LE(o);o+=4;if(n>100000||o+n>b.length)throw Error('Bad string');const s=b.toString('utf8',o,o+n);o+=n;return s;}
 for(let i=0;i<count;i++){
  const id=b.readUInt32BE(o);o+=4;const type=text(),source=text(),destination=text();const offset=Number(b.readBigUInt64BE(o));o+=8;
  const size=b.readUInt32BE(o),packed=b.readUInt32BE(o+4),flags2=b.readUInt16BE(o+16);o+=18;
  if(pattern.test(destination))entries.push({index,id,type,source,destination,offset,size,packed,flags2});
 }
 if(o!==b.length)throw Error('Index length mismatch');
}
fs.writeFileSync('work/resource-search-results.json',JSON.stringify(entries,null,2)+'\n');
const names=[...new Set(entries.map(e=>e.destination))];
console.log(JSON.stringify({entries:entries.length,unique:names.length,names},null,2));
