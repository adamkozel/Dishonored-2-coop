const fs=require('fs'),path=require('path'),crypto=require('crypto');
const lab=path.resolve(__dirname,'..');
const read=p=>JSON.parse(fs.readFileSync(p,'utf8').replace(/^\uFEFF/,''));
const hash=p=>crypto.createHash('sha256').update(fs.readFileSync(p)).digest('hex').toUpperCase();
const pack=m=>Array.from({length:12},(_,i)=>m[Math.floor(i/3)*4+i%3]);
const multiply=(a,b)=>Array.from({length:12},(_,i)=>{const c=Math.floor(i/3),r=i%3;let v=c===3?a[9+r]:0;for(let k=0;k<3;k++)v+=a[k*3+r]*b[c*3+k];return v;});
const inverse=a=>{const v=Array(12).fill(0);for(let c=0;c<3;c++)for(let r=0;r<3;r++)v[c*3+r]=a[r*3+c];for(let r=0;r<3;r++)for(let k=0;k<3;k++)v[9+r]-=v[k*3+r]*a[9+k];return v;};
const qts=q=>{let[x,y,z,w]=q;const n=Math.hypot(x,y,z,w);if(Math.abs(n-1)>.001||q.slice(8,11).some(s=>Math.abs(s-1)>.001))throw Error('Unsupported bind transform');[x,y,z,w]=[x/n,y/n,z/n,w/n];
 return[1-2*(y*y+z*z),2*(x*y+z*w),2*(x*z-y*w),2*(x*y-z*w),1-2*(x*x+z*z),2*(y*z+x*w),2*(x*z+y*w),2*(y*z-x*w),1-2*(x*x+y*y),...q.slice(4,7)];};
const forward=joints=>{const global=[],visiting=new Set();function visit(j){if(global[j])return global[j];if(j<0||j>=joints.length||visiting.has(j))throw Error('Invalid hierarchy');visiting.add(j);
 const local=qts(joints[j].bindLocalQTS),parent=joints[j].parent;global[j]=parent<0?local:multiply(visit(parent),local);visiting.delete(j);return global[j];}joints.forEach((_,j)=>visit(j));return global;};
const rotationError=(a,b)=>Math.max(...a.slice(0,9).map((v,i)=>Math.abs(v-b[i])));
const r=read(process.argv[2]||path.join(lab,'research/arm-stream-observations.json')),measurements=[],checks=[];let failure=null;
try{
 if(!r.Passed||r.DllSha256!==hash(path.join(lab,'bin/dinput8.dll'))||r.Cases.length!==2)throw Error('Both directions must pass on the current loader');
 for(const c of r.Cases){
  const core=[1,2,3,4,59],sb=forward(c.SourceRig.joints),tb=forward(c.TargetRig.joints);
  const targets=core.map(j=>{const matches=c.TargetRig.joints.filter(t=>t.nativeNameId===c.SourceRig.joints[j].nativeNameId);if(matches.length!==1)throw Error('Ambiguous native joint identity');return matches[0].index;});
  if(c.Armed.targets.length===21){
   if(!c.SourceRig.nativeNamesValid||!c.TargetRig.nativeNamesValid)throw Error('Independent native hand labels unavailable');
   const pairs=[];
   for(const [from,to,first,last] of [['thumb','thumb',1,3],['index','index',2,4],['medium','middle',2,4],['ring','ring',2,4],['pinky','pinky',1,4]]){
    for(let j=first;j<=last;j++)pairs.push([`right_${from}_0${j}_jnt`,`right_${to}0${j-first+1}_jnt`]);
   }
   for(const [sourceName,targetName] of pairs){
    const a=c.SourceRig.joints.filter(j=>j.nativeName===sourceName),b=c.TargetRig.joints.filter(j=>j.nativeName===targetName);
    if(a.length!==1||b.length!==1)throw Error('Ambiguous semantic finger name');core.push(a[0].index);targets.push(b[0].index);
   }
  }
  if(JSON.stringify(targets)!==JSON.stringify(c.Armed.targets))throw Error('Native semantic arm mapping differs');
  const rows=r.Samples.filter(s=>s.Role===c.Role),history=new Map();
  // Status is written every 250 ms. Include the recorded post-capture states
  // so the final native snapshot can match packets published just afterward.
  const states=[...rows.flatMap(row=>[row.Before,row.After]),c.After,c.Duplicate,c.Expired];
  for(const state of states)for(const p of state.history){
   if(p.sourceWorldEpoch!==c.Armed.peerEpoch||p.targetWorldEpoch!==c.Armed.epoch||p.sourceSession!==c.Armed.peerSession)continue;
   if(p.source.length!==core.length)throw Error('Wire joint count differs from independently named mapping');
   history.set(p.sequence,{...p,expected:core.map((j,k)=>multiply(multiply(p.source[k],inverse(sb[j])),tb[targets[k]]))});
  }
  const accepted=[];let maxError=0,maxLengthDifference=0;
  for(const row of rows){
   if(!row.Target.stable||!row.Before.active||!row.After.active)continue;
   const phase=row.Target.phases.find(p=>p.phase===0);if(!phase)throw Error('Missing pre-physics pose');
   const actual=targets.map(j=>pack(phase.matrices.find(m=>m.joint===j).matrix));
   let best={error:Infinity,sequence:0};
   for(const p of history.values()){
    if(p.sequence<row.Before.appliedSequence-16||p.sequence>row.After.sequence+16)continue;
    const error=Math.max(...actual.map((m,k)=>rotationError(m,p.expected[k])));if(error<best.error)best={error,sequence:p.sequence};
   }
   maxError=Math.max(maxError,best.error);accepted.push({Stage:row.Stage,Index:row.Index,Sequence:best.sequence,Error:best.error});
   for(const j of targets){const actual=row.Target.joints[j].localPose.slice(12,15),before=c.TargetRig.joints[j].localPose.slice(12,15);maxLengthDifference=Math.max(maxLengthDifference,Math.abs(Math.hypot(...actual)-Math.hypot(...before)));}
  }
  const byStage=['Idle','StandingSlash','CrouchSlash','ReturnedIdle'].map(stage=>({Stage:stage,Samples:accepted.filter(s=>s.Stage===stage).length,MaximumError:Math.max(0,...accepted.filter(s=>s.Stage===stage).map(s=>s.Error))}));
  const poses=[...history.values()];let motion=0;for(const p of poses)for(let k=0;k<core.length;k++)motion=Math.max(motion,rotationError(poses[0].source[k],p.source[k]));
  let restoredFingerDifference=0;
  for(const row of rows.filter(s=>s.Stage==='Idle'&&s.Target.stable&&s.Before.active&&s.After.active))for(const j of targets.slice(5))
   restoredFingerDifference=Math.max(restoredFingerDifference,rotationError(pack(row.Target.joints[j].localPose),pack(c.Restored.joints[j].localPose)));
  const crouched=rows.filter(s=>s.Stage==='CrouchSlash'&&s.SourceBridge.localPlayerStance===1&&s.TargetBridge.bodyStreamApplied>rows[0].TargetBridge.bodyStreamApplied);
  measurements.push({Role:c.Role,Observer:c.Observer,NativeSamples:accepted.length,JointCount:core.length,JointsCompared:accepted.length*core.length,MaximumRotationError:maxError,MaximumNativeLengthDifference:maxLengthDifference,SourceMotion:motion,RestoredFingerDifference:restoredFingerDifference,CrouchedBodySamples:crouched.length,Stages:byStage,Samples:accepted});
  if(byStage.some(s=>s.Samples<4)||accepted.length<40)throw Error('Too few stable native arm samples: '+c.Role);
  if(maxError>.003)throw Error('Streamed arm rotation differs from native result: '+c.Role);
  if(maxLengthDifference>.003)throw Error('Native arm length changed: '+c.Role);
  if(motion<.15||crouched.length<6)throw Error('Sword swing or coexisting crouch body motion missing: '+c.Role);
  if(core.length>5&&restoredFingerDifference<.1)throw Error('Finger grip did not differ from restored native idle: '+c.Role);
  checks.push(c.Role+`: independently reconstructed received arm/hand poses match ${core.length} native pre-physics joints while standing and crouching`);
 }
}catch(e){failure=e.message;}
const report={Passed:!failure,Failure:failure,ObservedAt:new Date().toISOString(),DllSha256:r.DllSha256,ProbeSha256:r.ProbeSha256,Scope:'Independent numeric comparison of actual native counterpart arm/hand poses to received Steam semantic joint history in both directions. Finger correspondence independently resolves native joint names when present. No aim, physics, damage or render parity claim.',Checks:checks,Measurements:measurements};
fs.writeFileSync(path.join(lab,'research/arm-stream-verification.json'),JSON.stringify(report,null,2)+'\n');console.log(JSON.stringify({...report,Measurements:measurements.map(({Samples,...m})=>m)},null,2));if(failure)process.exitCode=1;

