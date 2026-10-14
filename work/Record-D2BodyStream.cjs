const fs=require('fs'),crypto=require('crypto'),m=require('./analyze-skeleton.cjs');
const input='work/body-stream-observations.json',r=JSON.parse(fs.readFileSync(input));
const hash=p=>crypto.createHash('sha256').update(fs.readFileSync(p)).digest('hex').toUpperCase();
if(!r.Passed||r.DllSha256!==hash('outputs/d2-coop-lab/bin/dinput8.dll'))throw Error('unverified or wrong-build observations');
let validation=[];
for(const role of ['host','client']){
 const rows=r.Samples.filter(s=>s.Role===role&&s.Stage==='Crouched'&&s.Source?.stable&&s.Target?.stable);let maxRotationError=0,joints=0;
 for(const x of rows){const a=x.Source,b=x.Target,sb=m.fk(a.joints,a.joints.map(j=>m.qts(j.bindLocalQTS))),tb=m.fk(b.joints,b.joints.map(j=>m.qts(j.bindLocalQTS)));
  // Semantic indices recovered independently from each local rig asset.
  // Never compare process-local joint IDs across the two game processes.
  const corvo=b.joints.length===215;
  const mapping={1:2,2:3,3:4,4:5,5:11,6:12,7:corvo?62:59,9:19,10:20,11:21,12:22,13:corvo?73:70,14:23,15:24,16:25,17:26,18:corvo?74:71};
  for(const [source,target] of Object.entries(mapping)){const want=m.multiply(m.multiply(a.joints[source].globalPose,m.inverse(sb[source])),tb[target]);const got=b.phases[0].matrices.find(t=>t.joint===target).matrix;
   for(let c=0;c<3;c++)for(let v=0;v<3;v++)maxRotationError=Math.max(maxRotationError,Math.abs(want[c*4+v]-got[c*4+v]));joints++;
  }
 }
 const walking=r.Samples.filter(s=>s.Role===role&&s.Stage==='CrouchWalk'&&s.Target?.stable);
 const spans=[21,25].map(j=>{const positions=walking.map(s=>s.Target.joints[j].globalPose.slice(12,15));return Math.max(...positions.flatMap(a=>positions.map(b=>Math.hypot(...a.map((x,i)=>x-b[i])))));});
 const jumps=['CrouchJump','StandingJump'].map(stage=>{const x=r.Samples.filter(s=>s.Role===role&&s.Stage===stage&&s.Target?.stable);const heights=x.map(s=>s.Target.root[2]);return{stage,samples:x.length,heightSpan:Math.max(...heights)-Math.min(...heights)};});
 if(rows.length<4||maxRotationError>.003||walking.length<4||spans.some(s=>s<.05)||jumps.some(j=>j.samples<5||j.heightSpan<.45))throw Error('body analysis criteria failed: '+JSON.stringify({role,coherentCrouchSamples:rows.length,maxRotationError,crouchWalkSamples:walking.length,spans,jumps}));
 validation.push({role,coherentCrouchSamples:rows.length,mappedJointObservations:joints,maxPrePhysicsRotationElementError:maxRotationError,crouchWalkSamples:walking.length,localAnkleSpans:spans,jumps});
}
const report={Passed:true,ObservedAt:r.ObservedAt,AnalyzedAt:new Date().toISOString(),DllSha256:r.DllSha256,ProbeSha256:r.ProbeSha256,PoseProtocol:10,PacketBytes:1156,Cases:r.Cases,Validation:validation,
 Scope:'Native Steam body pose stream for both rig counterparts. Pre-physics mapped rotations and hip translation match independently read source poses, crouch walking articulates legs, both jump roots rise, native gait resumes, opposite counterpart stays independent.',
 Limitations:'No visual appearance review. No first-person arm-to-NPC arm retargeting, remote combat or powers, shared NPC world simulation, or general hitbox agreement. Native NPC IK runs after the applied pose. One-PC native Steam loopback only.',RawEvidenceSha256:hash(input)};
fs.writeFileSync('outputs/d2-coop-lab/research/body-stream-verification.json',JSON.stringify(report,null,2));console.log(JSON.stringify({Passed:true,DllSha256:r.DllSha256,Validation:validation},null,2));
