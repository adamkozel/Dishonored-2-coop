const fs=require('fs'),path=require('path'),crypto=require('crypto');
const lab=path.resolve(__dirname,'..');
const input=process.argv[2]||path.join(lab,'research/local-arm-probe-observations.json');
const read=p=>JSON.parse(fs.readFileSync(p,'utf8').replace(/^\uFEFF/,''));
const hash=p=>crypto.createHash('sha256').update(fs.readFileSync(p)).digest('hex').toUpperCase();
const pack=m=>Array.from({length:12},(_,i)=>m[Math.floor(i/3)*4+i%3]);
const multiply=(a,b)=>Array.from({length:12},(_,i)=>{
 const c=Math.floor(i/3),r=i%3;let v=c===3?a[9+r]:0;for(let k=0;k<3;k++)v+=a[k*3+r]*b[c*3+k];return v;
});
const inverse=a=>{const v=Array(12).fill(0);for(let c=0;c<3;c++)for(let r=0;r<3;r++)v[c*3+r]=a[r*3+c];for(let r=0;r<3;r++)for(let k=0;k<3;k++)v[9+r]-=v[k*3+r]*a[9+k];return v;};
const qts=q=>{
 let[x,y,z,w]=q;const n=Math.hypot(x,y,z,w);if(Math.abs(n-1)>.001||q.slice(8,11).some(s=>Math.abs(s-1)>.001))throw Error('Unsupported native bind scale/quaternion');[x,y,z,w]=[x/n,y/n,z/n,w/n];
 return[1-2*(y*y+z*z),2*(x*y+z*w),2*(x*z-y*w),2*(x*y-z*w),1-2*(x*x+z*z),2*(y*z+x*w),2*(x*z+y*w),2*(y*z-x*w),1-2*(x*x+y*y),...q.slice(4,7)];
};
const forward=joints=>{
 const global=[],visiting=new Set();
 function visit(j){if(global[j])return global[j];if(j<0||j>=joints.length||visiting.has(j))throw Error('Invalid native hierarchy');visiting.add(j);
  const local=qts(joints[j].bindLocalQTS),parent=joints[j].parent;global[j]=parent<0?local:multiply(visit(parent),local);visiting.delete(j);return global[j];}
 joints.forEach((_,j)=>visit(j));return global;
};
const rotationError=(a,b)=>Math.max(...a.slice(0,9).map((v,i)=>Math.abs(v-b[i])));
const r=read(input),checks=[],measurements=[];let failure=null;
try{
 if(!r.Passed||r.DllSha256!==hash(path.join(lab,'bin/dinput8.dll')))throw Error('Observation did not pass on the current loader');
 if(r.Cases.length!==2)throw Error('Both native target characters are required');
 for(const c of r.Cases){
  const core=[1,2,3,4,59],sourceBind=forward(c.Source.joints),targetBind=forward(c.Target.joints);
  const targets=core.map(j=>{const candidates=c.Target.joints.filter(t=>t.nativeNameId===c.Source.joints[j].nativeNameId);if(candidates.length!==1)throw Error('Ambiguous same-process arm joint');return candidates[0].index;});
  if(JSON.stringify(targets)!==JSON.stringify(c.Armed.targets))throw Error('Logged arm mapping differs from independent native identity');
  const expected=core.map((j,k)=>multiply(multiply(c.Armed.source[k],inverse(sourceBind[j])),targetBind[targets[k]]));
  const coherent=r.Samples.filter(s=>s.Role===c.Role&&s.Target.stable&&s.State.active&&s.State.started===c.Armed.started&&s.State.applied>c.Before.applied);
  if(coherent.length<4)throw Error('Too few coherent active arm observations: '+c.Role);
  let maxRotationError=0,maxBindLengthDifference=0,maxNativeLengthDifference=0;
  for(const s of coherent){
   const phase=s.Target.phases.find(p=>p.phase===0);if(!phase)throw Error('Native pre-physics phase missing');
   for(let k=0;k<5;k++){
    const joint=phase.matrices.find(m=>m.joint===targets[k]);if(!joint)throw Error('Mapped native joint missing');
    const error=rotationError(expected[k],pack(joint.matrix));maxRotationError=Math.max(maxRotationError,error);
    const actual=s.Target.joints[targets[k]].localPose.slice(12,15),bind=s.Target.joints[targets[k]].bindLocalQTS.slice(4,7);
    maxBindLengthDifference=Math.max(maxBindLengthDifference,Math.abs(Math.hypot(...actual)-Math.hypot(...bind)));
    const before=c.Target.joints[targets[k]].localPose.slice(12,15);
    maxNativeLengthDifference=Math.max(maxNativeLengthDifference,Math.abs(Math.hypot(...actual)-Math.hypot(...before)));
   }
  }
  const restored=c.Restored.phases.find(p=>p.phase===0);
  const restorationDifference=Math.max(...targets.map((j,k)=>rotationError(expected[k],pack(restored.matrices.find(m=>m.joint===j).matrix))));
  measurements.push({Role:c.Role,CoherentSamples:coherent.length,JointsCompared:coherent.length*5,MaxRotationError:maxRotationError,MaxBindLengthDifference:maxBindLengthDifference,MaxNativeLengthDifference:maxNativeLengthDifference,RestorationDifference:restorationDifference});
  if(maxRotationError>.003)throw Error('Native arm rotations differ from independent retarget: '+c.Role);
  if(maxNativeLengthDifference>.003)throw Error('Mapped native arm length changed: '+c.Role);
  if(!c.Restored.stable||restorationDifference<.03)throw Error('Native arm did not visibly change its measured pose after expiry: '+c.Role);
  checks.push(c.Role+': five native pre-physics joint rotations match the independently reconstructed frozen player arm, then return to a different native pose after expiry');
 }
}catch(e){failure=e.message;}
const report={Passed:!failure,Failure:failure,ObservedAt:new Date().toISOString(),DllSha256:r.DllSha256,ProbeSha256:r.ProbeSha256,
 Scope:'Independent numeric verification of the local right-arm pose experiment. Rotation comparison only; no remote stream, render-image, equipment, finger or combat-parity claim.',Checks:checks,Measurements:measurements};
fs.writeFileSync(path.join(lab,'research/local-arm-probe-verification.json'),JSON.stringify(report,null,2)+'\n');console.log(JSON.stringify(report,null,2));if(failure)process.exitCode=1;
