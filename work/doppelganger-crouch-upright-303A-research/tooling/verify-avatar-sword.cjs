const fs=require('fs'),path=require('path'),crypto=require('crypto');
const lab=path.resolve(__dirname,'..'),input=process.argv[2]||path.join(lab,'research/avatar-sword-arms-observations.json');
const read=p=>JSON.parse(fs.readFileSync(p,'utf8').replace(/^\uFEFF/,''));
const hash=p=>crypto.createHash('sha256').update(fs.readFileSync(p)).digest('hex').toUpperCase();
const r=read(input),rows=[];let failure=null,summary={};
const network=path.basename(input)==='avatar-weapon-observations.json',streamArm=network||r.StreamArm;
const mul=(a,b)=>Array.from({length:9},(_,i)=>[0,1,2].reduce((s,k)=>s+a[k*3+i%3]*b[Math.floor(i/3)*3+k],0));
const error=(a,b)=>Math.max(...a.map((v,i)=>Math.abs(v-b[i])));
try{
 if(!r.Passed||r.DllSha256!==hash(path.join(lab,'bin/dinput8.dll')))throw Error('Native sword observations must pass on the current loader');
 for(const sample of r.Samples){
  const a=sample.Attachment;if(sample.Stage!=='Drawn'||!a||a.tag!=='right_hand_wpn'||a.joint!==17)continue;
  const [x,y,z,w]=a.quaternion;if(Math.abs(Math.hypot(x,y,z,w)-1)>.001)throw Error('Invalid binding quaternion');
  const q=[1-2*(y*y+z*z),2*(x*y-z*w),2*(x*z+y*w),2*(x*y+z*w),1-2*(x*x+z*z),2*(y*z-x*w),2*(x*z-y*w),2*(y*z+x*w),1-2*(x*x+y*y)];
  const j=a.jointMatrix,p=a.parentRenderRoot;
  const local=[0,1,2].map(i=>j[12+i]+j[i]*a.offset[0]+j[4+i]*a.offset[1]+j[8+i]*a.offset[2]);
  const position=[0,1,2].map(i=>p[i]+p[3+i]*local[0]+p[6+i]*local[1]+p[9+i]*local[2]);
  const basis=mul(p.slice(3),mul(Array.from({length:9},(_,i)=>j[Math.floor(i/3)*4+i%3]),q));
  rows.push({Tick:a.tick,Stable:a.stable,PositionError:Math.hypot(...position.map((v,i)=>v-a.weaponRenderRoot[i])),BasisError:error(basis,a.weaponRenderRoot.slice(3)),NativeRenderError:error(a.weaponNativeRoot,a.weaponRenderRoot),Height:a.weaponNativeRoot[2]-a.parentNativeRoot[2],Offset:a.offset});
 }
 const stable=rows.filter(r=>r.Stable);if(stable.length<4)throw Error('Too few stable drawn native attachments');
 summary={StableSamples:stable.length,MaxPositionError:Math.max(...stable.map(r=>r.PositionError)),MaxBasisError:Math.max(...stable.map(r=>r.BasisError)),MaxNativeRenderError:Math.max(...stable.map(r=>r.NativeRenderError)),HeightSpan:Math.max(...stable.map(r=>r.Height))-Math.min(...stable.map(r=>r.Height))};
 if(stable.some(r=>r.PositionError>.003||r.BasisError>.003||r.NativeRenderError>.003))throw Error('Rendered Corvo sword does not follow its native hand joint');
 if(streamArm&&summary.HeightSpan<.15)throw Error('Too little articulated sword movement during the streamed slash');
}catch(e){failure=e.message;}
const report={Passed:!failure,Failure:failure,ObservedAt:new Date().toISOString(),DllSha256:r.DllSha256,ProbeSha256:r.ProbeSha256,RawEvidenceSha256:hash(input),StreamArm:streamArm,NetworkEquipment:network,
 Scope:`Independent read-only reconstruction of the ${network?'Steam-equipped':'locally equipped'} Corvo tutorial sword render transform from phase-3 native hand joint, binding offset/quaternion and parent render transform. Does not establish collision or finger grip parity.`,Summary:summary,Observations:rows};
fs.writeFileSync(path.join(lab,`research/${network?'avatar-weapon':`avatar-sword${r.StreamArm?'-arms':''}`}-verification.json`),JSON.stringify(report,null,2)+'\n');console.log(JSON.stringify({...report,Observations:undefined},null,2));if(failure)process.exitCode=1;
