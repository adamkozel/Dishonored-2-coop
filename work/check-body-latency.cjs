const fs=require('fs'),m=require('./analyze-skeleton.cjs');
const r=JSON.parse(fs.readFileSync(process.argv[2]||'work/body-stream-observations.json'));
for(const role of ['host','client']){
 const rows=r.Samples.filter(s=>s.Role===role&&s.Stage==='Crouched'&&s.Source?.stable&&s.Target?.stable);
 const matches=[];
 for(let n=0;n<rows.length;n++){
  const target=rows[n],b=target.Target,tb=m.fk(b.joints,b.joints.map(j=>m.qts(j.bindLocalQTS))),corvo=b.joints.length===215;
  const mapping={1:2,2:3,3:4,4:5,5:11,6:12,7:corvo?62:59,9:19,10:20,11:21,12:22,13:corvo?73:70,14:23,15:24,16:25,17:26,18:corvo?74:71};
  const errors=[];
  for(let i=0;i<=n;i++){
   const age=Date.parse(target.At)-Date.parse(rows[i].At);if(age>=250)continue;
   const a=rows[i].Source,sb=m.fk(a.joints,a.joints.map(j=>m.qts(j.bindLocalQTS)));let error=0,worst=0;
   for(const [source,joint] of Object.entries(mapping)){
    const want=m.multiply(m.multiply(a.joints[source].globalPose,m.inverse(sb[source])),tb[joint]);const got=b.phases[0].matrices.find(t=>t.joint===joint).matrix;
    for(let c=0;c<3;c++)for(let v=0;v<3;v++){const d=Math.abs(want[c*4+v]-got[c*4+v]);if(d>error){error=d;worst=source;}}
   }errors.push({sourceIndex:i,ageMs:age,error,worst});
  }
  matches.push({targetIndex:n,same:errors.find(e=>e.sourceIndex===n),best:errors.sort((a,b)=>a.error-b.error)[0]});
 }
 console.log(JSON.stringify({role,matches},null,2));
}
