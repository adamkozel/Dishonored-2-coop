const fs=require('fs'),path=require('path');
const lab=path.resolve(__dirname,'..'),r=JSON.parse(fs.readFileSync(path.join(lab,'research/arm-stream-observations.json'),'utf8').replace(/^\uFEFF/,''));
const dot=(a,b)=>a.reduce((v,x,i)=>v+x*b[i],0);
const vec=(a,b)=>a.map((x,i)=>x-b[i]);
const norm=a=>{const n=Math.hypot(...a);return a.map(x=>x/n);};
const angle=(a,b)=>Math.acos(Math.max(-1,Math.min(1,dot(norm(a),norm(b)))))*180/Math.PI;
const point=(m)=>m.slice(12,15);
const rows=[];
for(const c of r.Cases){
 const fingers=[['thumb',[5,6,7,8],[38,39,c.Role==='host'?63:60]],['index',[16,17,18,19],[40,41,c.Role==='host'?65:62]],['middle',[27,28,29,30],[42,43,c.Role==='host'?67:64]],['ring',[38,39,40,41],[44,45,c.Role==='host'?69:66]],['pinky',[48,49,50,51,52],[46,47,48,c.Role==='host'?71:68]]];
 const row=r.Samples.find(s=>s.Role===c.Role&&s.Stage==='Idle'&&s.Target.stable);
 const mats=row.Target.phases.find(p=>p.phase===3).matrices;
 for(const[name,from,to]of fingers){
  const src=from.map(i=>point(c.SourceRig.joints[i].globalPose));
  const dst=to.map(i=>point(mats.find(m=>m.joint===i).matrix));
  const srcSegments=src.slice(1).map((p,i)=>vec(p,src[i])),dstSegments=dst.slice(1).map((p,i)=>vec(p,dst[i]));
  rows.push({Role:c.Role,Finger:name,SourceBends:srcSegments.slice(1).map((p,i)=>angle(srcSegments[i],p)),TargetBends:dstSegments.slice(1).map((p,i)=>angle(dstSegments[i],p)),Source:src,Target:dst});
 }
}
fs.writeFileSync(path.join(lab,'research/hand-grip-geometry-analysis.json'),JSON.stringify({Loader:r.DllSha256,Scope:'Historical DBA7 idle source and target geometry, not synchronized snapshots; diagnostic only.',Rows:rows},null,2));
console.log(JSON.stringify(rows.map(({Source,Target,...r})=>r),null,2));
