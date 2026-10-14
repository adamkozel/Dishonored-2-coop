const fs=require('fs'),path=require('path'),crypto=require('crypto');
const lab=path.resolve(__dirname,'..'),workspace=path.resolve(lab,'../..');
const input=process.argv[2]||path.join(workspace,'work/guard-body-observations.json');
const read=p=>JSON.parse(fs.readFileSync(p,'utf8').replace(/^\uFEFF/,''));
const hash=p=>crypto.createHash('sha256').update(fs.readFileSync(p)).digest('hex').toUpperCase();
const r=read(input),checks=[];
let failure=null;
// Independent affine reconstruction of the received core joints. Parent
// indices come from the external native skeleton read, not from the mod log.
const multiply=(a,b)=>{const v=Array(12).fill(0);for(let c=0;c<4;c++)for(let row=0;row<3;row++){
 v[c*3+row]=c===3?a[9+row]:0;for(let k=0;k<3;k++)v[c*3+row]+=a[k*3+row]*b[c*3+k];}return v;};
const pack=m=>Array.from({length:12},(_,i)=>m[Math.floor(i/3)*4+i%3]);
const maxError=(a,b)=>Math.max(...a.map((v,i)=>Math.abs(v-b[i])));
try{
 if(r.DllSha256!==hash(path.join(lab,'bin/dinput8.dll')))throw Error('Wrong loaded build');
 for(const s of r.Samples){
  if(!s.Client.Stable||!s.Host.Stable)continue;
  if(s.Client.JointCount!==223||s.Host.JointCount!==223||s.Client.Skeleton!==s.Host.Skeleton)throw Error('Native rig identity differs');
  let best={error:Infinity,sequence:0,joints:0,perJoint:[]};
  const phase=s.Client.Phases.find(p=>p.Phase===0);
  if(!phase)throw Error('Native pre-physics pose cache unavailable');
  for(const history of s.ClientBody.history||[]){
   if(history.local.length!==27)continue;
   let global=[],errors=[];
   for(let j=0;j<27;j++){
    const parent=s.Client.Joints[j].parent;
    if(parent>=j||parent < -1)throw Error('Unexpected core hierarchy');
    global[j]=parent<0?history.local[j]:multiply(global[parent],history.local[j]);
    const native=phase.Matrices.find(x=>x.joint===j);
    if(native)errors.push({joint:j,error:maxError(global[j],pack(native.matrix))});
   }
   const error=Math.max(...errors.map(x=>x.error));
   if(error<best.error)best={error,sequence:history.sequence,joints:errors.length,perJoint:errors};
  }
  checks.push({stage:s.Stage,at:s.At,hostAttack:s.HostAttack.schedulerActive,target:s.Target.combat.handle,partner:s.HostBridge.probeActorHandle,...best});
 }
 const standing=checks.filter(x=>x.stage==='Standing'),combat=checks.filter(x=>x.stage==='Combat'&&x.hostAttack&&x.target===x.partner);
 if(standing.length<3||combat.length<4)throw Error('Insufficient independently observed standing and native attack poses');
 const coherent=checks.filter(x=>x.error<=.003&&x.joints>=20);
 if(standing.some(x=>x.error>.003)||combat.filter(x=>x.error<=.003&&x.joints>=20).length<4)throw Error('Native core pose did not match bounded received-pose history');
 const moving=r.Samples.filter(s=>s.Stage==='Combat'&&s.Client.Stable);
 const span=(role,joint)=>{
  const positions=moving.map(s=>s[role].Joints[joint].globalPose.slice(12,15));
  return Math.max(...positions.flatMap(a=>positions.map(b=>Math.hypot(...a.map((v,i)=>v-b[i])))));
 };
 const articulation={hostHands:[span('Host',9),span('Host',16)],clientHands:[span('Client',9),span('Client',16)]};
 if(Math.max(...articulation.hostHands)<.2||Math.max(...articulation.clientHands)<.2)throw Error('Native sword-arm movement was not observed on both rigs');
 r.Validation={coherentSamples:coherent.length,coherentJointObservations:coherent.reduce((n,x)=>n+x.joints,0),standingSamples:standing.length,hostAttackSamples:combat.length,articulation,checks};
 if(r.GuardWeapon){
  const initial=r.Samples[0].ClientSword;
  const drawn=r.Samples.filter(s=>s.Stage==='Combat'&&s.HostSword.equipped&&s.HostSword.started&&s.HostSword.attachedTag==='right_hand_wpn'&&
   s.ClientSword.equipped&&s.ClientSword.started&&s.ClientSword.attachedTag==='right_hand_wpn'&&s.ClientWeapon.active&&s.ClientWeapon.drawn&&s.ClientEquipment.active&&!s.ClientEquipment.localExperiment);
  if(drawn.length<4)throw Error('Insufficient observed native sword draws during animated combat');
  for(const s of drawn){
   for(const key of ['weaponHandle','entity','component','parent'])if(s.ClientSword[key]!==initial[key])throw Error('Animated sword changed local identity: '+key);
   if(s.ClientSword.joint!==17||s.ClientSword.slot!==0||s.ClientAttack.schedulerActive)throw Error('Animated sword attachment/AI isolation differs');
  }
  const observations=drawn.map(s=>{
   const row={at:s.At};
   for(const role of ['Host','Client']){
    const root=s[role].Root,j=s[role].Joints[17].globalPose;
    const hand=[0,1,2].map(i=>root[i]+root[3+i]*j[12]+root[6+i]*j[13]+root[9+i]*j[14]);
    const sword=s[role+'Sword'].root.slice(0,3);
    row[role]={hand,sword,distance:Math.hypot(...hand.map((v,i)=>v-sword[i]))};
   }
   return row;
  });
  r.Validation.weapon={drawnSamples:drawn.length,identityAndNativeAttachmentConfirmed:true,
   geometryScope:'Diagnostic sequential world positions; not a simultaneous attachment or rendered blade alignment assertion.',observations};
 }
}catch(e){failure=e.message;r.Validation={checks};}
if(!failure&&!r.Passed)failure='Collector failed: '+r.Failure;
const report={Passed:!failure,Failure:failure,ObservedAt:r.ObservedAt,AnalyzedAt:new Date().toISOString(),DllSha256:r.DllSha256,ProbeSha256:r.ProbeSha256,
 Scope:'Explicit one-guard Steam animation stream. Native host attack scheduling, external native pre-physics core joint matrices compared against bounded received-pose history, arm articulation, client AI remains paused, source expiry, re-enable and native guard release.',
 Limitations:'One exact 223-joint guard rig; quantitative verification covers the first 27 core joints. Other authored NPCs remain independently active and may damage or kill the local player; recorded damage must be exclusively from those identified NPCs. No full face/finger validation, rendered blade alignment, block/parry or general NPC animation support. Sequential observations do not establish a latency bound.',
 GuardWeapon:!!r.GuardWeapon,ObserverFixtureIsolation:!!r.IsolateObserver,UnreplicatedOtherGuardHits:r.OtherGuardHits,NativePlayerHealth:r.Health,RawEvidenceSha256:hash(input),Validation:r.Validation};
if(r.IsolateObserver)report.Limitations='Ramsey and escape guard 2 are temporarily held by a client-only 30-second test fixture; this does not establish shared AI authority. '+report.Limitations;
fs.writeFileSync(path.join(lab,`research/guard-body${r.GuardWeapon?'-weapon':''}${r.IsolateObserver?'-isolated':''}-verification.json`),JSON.stringify(report,null,2));
console.log(JSON.stringify({Passed:report.Passed,Failure:failure,coherentSamples:r.Validation.coherentSamples,
 coherentJointObservations:r.Validation.coherentJointObservations,hostAttackSamples:r.Validation.hostAttackSamples,
 maxPoseElementError:Math.max(...checks.map(x=>x.error)),articulation:r.Validation.articulation,weaponDrawnSamples:r.Validation.weapon?.drawnSamples},null,2));
if(failure)process.exitCode=1;
