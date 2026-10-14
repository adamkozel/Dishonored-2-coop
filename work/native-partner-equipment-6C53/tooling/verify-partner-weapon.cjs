const fs=require('fs'),path=require('path'),crypto=require('crypto');
const lab=path.resolve(__dirname,'..');
const file=path.join(lab,'research/partner-weapon-observations.json');
const r=JSON.parse(fs.readFileSync(file,'utf8').replace(/^\uFEFF/,''));
const hash=p=>crypto.createHash('sha256').update(fs.readFileSync(p)).digest('hex').toUpperCase();
const mul=(a,b)=>Array.from({length:9},(_,i)=>[0,1,2].reduce((s,k)=>s+a[k*3+i%3]*b[Math.floor(i/3)*3+k],0));
const error=(a,b)=>Math.max(...a.map((v,i)=>Math.abs(v-b[i])));
const groups={};let failure=null;
try {
 if(!r.Passed||r.DllSha256!==hash(path.join(lab,'bin/dinput8.dll')))throw Error('Raw test must pass on current loader');
 for(const role of ['host','client']) {
  const samples=r.Samples.filter(x=>x.SourceRole===role&&x.Stage==='Drawn'&&x.Attachment?.stable&&x.Attachment.tag==='right_hand_wpn');
  if(samples.length<4)throw Error('Insufficient stable hand samples: '+role);
  const values=samples.map(x=>{
   const a=x.Attachment,[qx,qy,qz,w]=a.quaternion;
   if(Math.abs(Math.hypot(qx,qy,qz,w)-1)>.001||a.joint!==17)throw Error('Unexpected native hand attachment');
   const q=[1-2*(qy*qy+qz*qz),2*(qx*qy-qz*w),2*(qx*qz+qy*w),2*(qx*qy+qz*w),1-2*(qx*qx+qz*qz),2*(qy*qz-qx*w),2*(qx*qz-qy*w),2*(qy*qz+qx*w),1-2*(qx*qx+qy*qy)];
   const j=a.jointMatrix,p=a.parentRenderRoot;
   const local=[0,1,2].map(i=>j[12+i]+j[i]*a.offset[0]+j[4+i]*a.offset[1]+j[8+i]*a.offset[2]);
   const pos=[0,1,2].map(i=>p[i]+p[3+i]*local[0]+p[6+i]*local[1]+p[9+i]*local[2]);
   const basis=mul(p.slice(3),mul(Array.from({length:9},(_,i)=>j[Math.floor(i/3)*4+i%3]),q));
   return {position:Math.hypot(...pos.map((v,i)=>v-a.weaponRenderRoot[i])),basis:error(basis,a.weaponRenderRoot.slice(3)),native:error(a.weaponNativeRoot,a.weaponRenderRoot)};
  });
  groups[role]={StableSamples:values.length,MaxPositionError:Math.max(...values.map(x=>x.position)),MaxBasisError:Math.max(...values.map(x=>x.basis)),MaxNativeRenderError:Math.max(...values.map(x=>x.native))};
  if(values.some(x=>x.position>.003||x.basis>.003||x.native>.003))throw Error('Render attachment differs from native hand: '+role);
 }
}catch(e){failure=e.message;}
const report={Passed:!failure,Failure:failure,Time:new Date().toISOString(),DllSha256:r.DllSha256,RawEvidenceSha256:hash(file),Groups:groups,VisualGripPassed:false,Scope:'Independent reconstruction of both partner sword render matrices from the native phase-3 hand joint, binding offset/quaternion and parent render transform. Does not prove hand grip or collision.'};
fs.writeFileSync(path.join(lab,'research/partner-weapon-render-verification.json'),JSON.stringify(report,null,2)+'\n');
console.log(JSON.stringify(report,null,2));if(failure)process.exitCode=1;
