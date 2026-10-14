const fs=require('fs'),path=require('path'),crypto=require('crypto');
const lab=path.resolve(__dirname,'..'),workspace=path.resolve(lab,'../..');
const input=process.argv[2]||path.join(workspace,'work/guard-body-weapon-isolated-observations.json');
const read=p=>JSON.parse(fs.readFileSync(p,'utf8').replace(/^\uFEFF/,''));
const hash=p=>crypto.createHash('sha256').update(fs.readFileSync(p)).digest('hex').toUpperCase();
const r=read(input),observations=[];
const mul=(a,b)=>Array.from({length:9},(_,i)=>[0,1,2].reduce((s,k)=>s+a[k*3+i%3]*b[Math.floor(i/3)*3+k],0));
const error=(a,b)=>Math.max(...a.map((v,i)=>Math.abs(v-b[i])));
let failure=null,summary={};
try {
 if(!r.Passed||!r.GuardWeapon||!r.IsolateObserver)throw Error('Requires a completed isolated body/weapon collection');
 if(r.DllSha256!==hash(path.join(lab,'bin/dinput8.dll')))throw Error('Different runtime build');
 if(r.Health.client.current!==56||r.OtherGuardHits!==0)throw Error('Observer was damaged');
 for(const sample of r.Samples.filter(s=>s.Stage==='Combat'))for(const role of ['host','client']){
  const a=sample[role+'Attachment'];
  if(a.joint!==17||a.tag!=='right_hand_wpn'||a.phase!==3)continue;
  // The native draw binding uses zero offset. This makes the position
  // reconstruction independent of the parent's unobserved render scale.
  if(a.offset.some(v=>v!==0))throw Error('Unexpected native draw offset');
  const [x,y,z,w]=a.quaternion;
  if(Math.abs(Math.hypot(x,y,z,w)-1)>.001)throw Error('Invalid native binding quaternion');
  // Retail idQuat basis is transposed relative to the usual column-vector
  // Hamilton convention. Native 53d3f0 combines it with phase-3 joint data;
  // 53d800 then applies the parent render basis. Reconstruction is read-only.
  const q=[1-2*(y*y+z*z),2*(x*y-z*w),2*(x*z+y*w),
   2*(x*y+z*w),1-2*(x*x+z*z),2*(y*z-x*w),
   2*(x*z-y*w),2*(y*z+x*w),1-2*(x*x+y*y)];
  const j=a.jointMatrix,p=a.parentRenderRoot;
  const position=[0,1,2].map(i=>p[i]+p[3+i]*j[12]+p[6+i]*j[13]+p[9+i]*j[14]);
  const basis=mul(p.slice(3),mul(Array.from({length:9},(_,i)=>j[Math.floor(i/3)*4+i%3]),q));
  observations.push({role,at:sample.At,tick:a.tick,stable:a.stable,
   positionError:Math.hypot(...position.map((v,i)=>v-a.weaponRenderRoot[i])),
   basisError:error(basis,a.weaponRenderRoot.slice(3)),nativeRenderError:error(a.weaponNativeRoot,a.weaponRenderRoot),
   localHeight:a.weaponNativeRoot[2]-a.parentNativeRoot[2],childrenUpdated:sample.ClientGuard.childrenUpdated,
   childrenRefused:sample.ClientGuard.childrenRefused});
 }
 for(const role of ['host','client']){
  const rows=observations.filter(o=>o.role===role&&o.stable);
  if(rows.length<4)throw Error('Insufficient stable drawn attachment observations: '+role);
  if(rows.some(o=>o.positionError>.003||o.basisError>.003||o.nativeRenderError>.003))throw Error('Native/rendered sword did not follow its bone: '+role);
  const heightSpan=Math.max(...rows.map(o=>o.localHeight))-Math.min(...rows.map(o=>o.localHeight));
  if(heightSpan<.3)throw Error('Insufficient articulated sword movement: '+role);
  summary[role]={stableDrawnSamples:rows.length,heightSpan,
   maxPositionError:Math.max(...rows.map(o=>o.positionError)),maxBasisError:Math.max(...rows.map(o=>o.basisError)),
   maxNativeRenderError:Math.max(...rows.map(o=>o.nativeRenderError))};
 }
 const child=observations.filter(o=>o.role==='client');
 if(child.at(-1).childrenUpdated<=child[0].childrenUpdated||child.some(o=>o.childrenRefused!==child[0].childrenRefused))throw Error('Native child updates stopped or rejected an active attachment');
}catch(e){failure=e.message;}
const report={Passed:!failure,Failure:failure,ObservedAt:r.ObservedAt,AnalyzedAt:new Date().toISOString(),
 DllSha256:r.DllSha256,ProbeSha256:r.ProbeSha256,RawEvidenceSha256:hash(input),
 Scope:'Read-only reconstruction of the drawn guard sword position and orientation from native phase-3 hand joint, local binding quaternion and parent render transform; compare with native entity and actual static render model. Native child traversal continues under a network-owned parent root.',
 Limitations:'One guard and sword. Sequential observations, not a frame capture or latency proof. Only stable samples are asserted; all samples retained. Other NPCs temporarily held by a local test fixture. Does not establish shared weapon collision, parry, all attachments or world authority.',
 Tolerance:{position:.003,basisElement:.003,nativeRenderElement:.003},Summary:summary,Observations:observations};
fs.writeFileSync(path.join(lab,'research/guard-attachment-verification.json'),JSON.stringify(report,null,2));
console.log(JSON.stringify({Passed:report.Passed,Failure:failure,Summary:summary},null,2));
if(failure)process.exitCode=1;
