const fs=require('fs');
const affineIndices=[0,1,2,4,5,6,8,9,10,12,13,14];
const identity=()=>[1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0];
function multiply(a,b){let out=identity();for(let c=0;c<4;c++)for(let r=0;r<3;r++){out[c*4+r]=(c===3?a[12+r]:0);for(let k=0;k<3;k++)out[c*4+r]+=a[k*4+r]*b[c*4+k];}return out;}
function inverse(a){let out=identity();for(let c=0;c<3;c++)for(let r=0;r<3;r++)out[c*4+r]=a[r*4+c];for(let r=0;r<3;r++)out[12+r]=-[0,1,2].reduce((s,k)=>s+out[k*4+r]*a[12+k],0);return out;}
function qts(q){let[x,y,z,w]=q;const n=Math.hypot(x,y,z,w);[x,y,z,w]=[x/n,y/n,z/n,w/n];let a=[1-2*(y*y+z*z),2*(x*y+z*w),2*(x*z-y*w),0,2*(x*y-z*w),1-2*(x*x+z*z),2*(y*z+x*w),0,2*(x*z+y*w),2*(y*z-x*w),1-2*(x*x+y*y),0,q[4],q[5],q[6],0];return a;}
function fk(joints,local){let result=[];let active=new Set();function at(j){if(result[j])return result[j];if(active.has(j))throw Error('cycle');active.add(j);result[j]=joints[j].parent<0?local[j].slice():multiply(at(joints[j].parent),local[j]);active.delete(j);return result[j];}joints.forEach((_,j)=>at(j));return result;}
function error(a,b){return Math.max(...affineIndices.map(i=>Math.abs(a[i]-b[i])));}
function retarget(source,target,pose){
 const sb=fk(source.joints,source.joints.map(j=>qts(j.bindLocalQTS))),tb=fk(target.joints,target.joints.map(j=>qts(j.bindLocalQTS)));
 const mapping=target.joints.map(j=>source.joints.findIndex(k=>k.nativeNameId===j.nativeNameId));
 const local=target.joints.map(j=>qts(j.bindLocalQTS)),global=[];
 function at(j){if(global[j])return global[j];const parent=target.joints[j].parent;if(parent>=0)at(parent);const src=mapping[j];
  if(src>=0){const wanted=multiply(multiply(pose[src],inverse(sb[src])),tb[j]);const rel=parent>=0?multiply(inverse(global[parent]),wanted):wanted;
   // Apply rotations; keep each target bone's native local translation.
   for(let c=0;c<3;c++)for(let r=0;r<3;r++)local[j][c*4+r]=rel[c*4+r];
   // Only the shared center-root joint carries body translation.
   if(src===1)for(let r=0;r<3;r++)local[j][12+r]=target.joints[j].bindLocalQTS[4+r]+pose[src][12+r]-sb[src][12+r];
  }
  global[j]=parent>=0?multiply(global[parent],local[j]):local[j].slice();return global[j];
 }target.joints.forEach((_,j)=>at(j));return {local,global,mapping,sb,tb};
}
if(require.main===module){
 let reports=[];
 for(const role of ['host','client'])for(const who of ['player','counterpart']){
  const s=JSON.parse(fs.readFileSync(`work/${role}-${who}-pose-skeleton.json`));const g=fk(s.joints,s.joints.map(j=>j.localPose));
  reports.push({role,who,joints:s.joints.length,stable:s.stable,maxFkError:Math.max(...s.joints.map((j,i)=>error(j.globalPose,g[i]))),phaseErrors:s.phases.map(p=>({phase:p.phase,maxError:Math.max(...p.matrices.map(m=>error(m.matrix,s.joints[m.joint].globalPose)))}))});
 }
 const samples=JSON.parse(fs.readFileSync('work/player-skeleton-observations.json')).Samples;let retargetReports=[];
 for(const role of ['host','client']){
  const source=JSON.parse(fs.readFileSync(`work/${role}-player-pose-skeleton.json`)),target=JSON.parse(fs.readFileSync(`work/${role}-counterpart-pose-skeleton.json`));
  const bind=fk(source.joints,source.joints.map(j=>qts(j.bindLocalQTS))),roundtrip=retarget(source,target,bind);
  retargetReports.push({role,test:'bind_roundtrip',maxError:Math.max(...roundtrip.global.map((m,i)=>error(m,roundtrip.tb[i]))),mapped:roundtrip.mapping.filter(j=>j>=0).length});
  for(const stage of ['stand','crouch','crouch_jump','stand_jump']){
   const s=samples.find(s=>s.Role===role&&s.Stage===stage&&s.Skeleton?.stable);if(!s)throw Error('missing stage');
   const pose=s.Skeleton.phases[0].matrices.map(m=>m.matrix),rt=retarget(source,target,pose);const ix=src=>rt.mapping.indexOf(src);
   retargetReports.push({role,stage,sourceHip:pose[1][14],targetHip:rt.global[ix(1)][14],leftAnkle:rt.global[ix(11)].slice(12,15),rightAnkle:rt.global[ix(16)].slice(12,15),head:rt.global[ix(6)].slice(12,15),maxNonRootBoneLengthChange:Math.max(...target.joints.filter((_,j)=>rt.mapping[j]!==1).map(j=>Math.abs(Math.hypot(...j.bindLocalQTS.slice(4,7))-Math.hypot(...rt.local[j.index].slice(12,15))))) });
  }
 }
 fs.writeFileSync('work/skeleton-math-research.json',JSON.stringify({reports,retargetReports},null,2));console.log(JSON.stringify({reports,retargetReports},null,2));
}
module.exports={affineIndices,identity,multiply,inverse,qts,fk,error,retarget};
