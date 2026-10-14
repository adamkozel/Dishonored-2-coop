const fs=require('node:fs');
const magnitude=v=>Math.hypot(...v);
const span=a=>{let max=0;for(const x of a)for(const y of a)max=Math.max(max,magnitude(x.map((v,i)=>v-y[i])));return max;};
const float=w=>{const b=Buffer.alloc(4);b.writeUInt32LE(w);return b.readFloatLE();};
for(const file of process.argv.slice(2)){
 const r=JSON.parse(fs.readFileSync(file));
 console.log(JSON.stringify({file,hash:r.DllSha256,failure:r.Failure,stages:['Idle','Moving','Stopped'].map(stage=>{
  const all=r.Samples.filter(s=>s.Stage===stage),s=all.filter(s=>s.Bones?.stable);
  return {stage,count:all.length,stable:s.length,maxInputSpeed:Math.max(...all.map(s=>s.Status.locomotionSpeed)),
   rootSpan:span(s.map(s=>s.Bones.root.slice(0,3))),footSpans:[21,25].map(joint=>({joint,span:span(s.map(s=>s.Bones.bodies.find(b=>b.joint===joint)?.localPosition).filter(Boolean))})),
   maxOutputSpeed:Math.max(...all.map(s=>s.Animation.modernOutputWords?magnitude(s.Animation.modernOutputWords.slice(0,3).map(float)):0)),
   resetFlags:[...new Set(all.flatMap(s=>s.Animation.locomotionNodes?.map(n=>n.requestFlags)||[]))]};
 })},null,2));
}
