from pathlib import Path
import re,json
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab/src';L=R/'outputs/d2-coop-lab';W=R/'work/movement-integration'
def edit(p,a,b):
    s=p.read_text(encoding='utf-8-sig');assert s.count(a)==1,(p,a[:80]);p.write_text(s.replace(a,b),encoding='utf-8')
# Extend existing exact cosmetic skeleton inspection, without widening NPC scope.
p=S/'native_skeleton.h'
edit(p,'PlayerRanged,CosmeticRat };','PlayerRanged,CosmeticRat,CosmeticDoppelganger };')
edit(p,'const bool rat=scope==NativeSkeletonScope::CosmeticRat;','const bool rat=scope==NativeSkeletonScope::CosmeticRat;const bool clone=scope==NativeSkeletonScope::CosmeticDoppelganger;')
edit(p,'((cosmetic||rat)?0x1c87d58','((cosmetic||rat||clone)?0x1c87d58')
edit(p,'if(rat){std::uintptr_t definition','if(rat||clone){std::uintptr_t definition')
edit(p,'text!="models/powers/d2coop/swarm_rat_visual.def"','text!=(clone?"models/powers/d2coop/doppelganger_visual.def":"models/powers/d2coop/swarm_rat_visual.def")')
anchor='    if(cosmetic&&(s.animation!=prop.animation'
t=p.read_text(encoding='utf-8-sig');assert t.count(anchor)==1
t=t.replace(anchor,'    if(clone&&(std::strcmp(s.modelName,"models/characters/female/emily/head01/emily_head01.md6")||std::strcmp(s.skeletonName,"models/characters/female/emily/head01/emily_head01.md6skl")||(jointCount&0xffffff)!=219))return fail(__LINE__);\n'+anchor);p.write_text(t,encoding='utf-8')
p=S/'doppelganger_world_protocol.h'
edit(p,'#include "pose_interpolation.h"','#include "pose_interpolation.h"\n#include "doppelganger_rig.h"')
edit(p,'version=1,size=176','version=2,size=10696')
edit(p,'std::uint32_t equipmentHandle=0;','std::uint32_t equipmentHandle=0;\n    std::uint64_t rig=0;\n    std::array<std::array<float,12>,kDoppelgangerJoints> local{};')
edit(p,'==176);','==10696);')
edit(p,'p.version!=1','p.version!=2')
edit(p,'        float d2=0;for(unsigned k=0;k<3;++k)','        if(p.rig!=kDoppelgangerRig)return false;for(const auto& joint:p.local)if(!bodyRigid(doppelgangerLocalMatrix(joint),3.f))return false;\n        float d2=0;for(unsigned k=0;k<3;++k)')
edit(p,'if(p.nativeHandle||p.equipmentHandle||p.generation)return false;for(float x:p.transform)if(x!=0)return false;','if(p.nativeHandle||p.equipmentHandle||p.generation||p.rig)return false;for(float x:p.transform)if(x!=0)return false;for(const auto& j:p.local)for(float x:j)if(x!=0)return false;')
p=S/'doppelganger_world_stream.h'
edit(p,'// Stage418 is transport/identity only: it creates no receiver entity or effect.','// Source-owned clone body snapshots; no native animation events travel with poses.')
edit(p,'double doppelgangerReadMaxMs=0;','double doppelgangerReadMaxMs=0,doppelgangerPoseReadMaxMs=0;std::uint64_t doppelgangerPoseReadFailed=0;')
edit(p,'if(w.present){p.nativeHandle=w.handle;p.equipmentHandle=w.equipment.handle;p.generation=w.generation;p.transform=w.transform;}','''if(w.present){
        const double poseStart=presentationClock();NativeSkeleton pose{};
        const bool poseValid=readNativeSkeleton(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),w.handle,pose)&&pose.stable&&pose.logic==1&&
            pose.game==w.equipment.player.sword.game&&pose.actor==w.actor&&pose.localPose.size()==kDoppelgangerJoints&&doppelgangerRig(pose)==kDoppelgangerRig;
        doppelgangerPoseReadMaxMs=(std::max)(doppelgangerPoseReadMaxMs,presentationClock()-poseStart);
        if(!poseValid){++doppelgangerPoseReadFailed;return;}
        p.nativeHandle=w.handle;p.equipmentHandle=w.equipment.handle;p.generation=w.generation;p.rig=kDoppelgangerRig;
        std::memcpy(p.transform.data(),pose.root,48);
        for(unsigned j=0;j<kDoppelgangerJoints;++j)for(unsigned c=0;c<4;++c)for(unsigned r=0;r<3;++r)p.local[j][c*3+r]=pose.localPose[j][c*4+r];
    }''')
edit(p,'\\"transportOnly\\":true','\\"transportOnly\\":false')
edit(p,'fputs("]}\\n",f);const bool ok=', 'fprintf(f,"],\\\"poseReadMaxMs\\\":%.4f,\\\"poseReadFailed\\\":%llu}\\n",doppelgangerPoseReadMaxMs,doppelgangerPoseReadFailed);const bool ok=')
p=S/'doppelganger_world_protocol_tests.h'
edit(p,'p.generation=8826;','p.generation=8826;p.rig=kDoppelgangerRig;for(auto& j:p.local){j={1,0,0,0,1,0,0,0,1,0,0,0};}')
edit(p,'empty.generation=0;empty.transform={};','empty.generation=0;empty.transform={};empty.rig=0;empty.local={};')
edit(p,'    DoppelgangerWorldOrder order{};', '''    dirty=p;dirty.local[218][0]=NAN;check(!validDoppelgangerWorldPacket(dirty,h,c),"nonfinite helper bone rejected");
    dirty=p;dirty.local[218][0]=-1;check(!validDoppelgangerWorldPacket(dirty,h,c),"reflected helper bone rejected");
    dirty=p;dirty.local[0][9]=4;check(!validDoppelgangerWorldPacket(dirty,h,c),"unbounded root local translation rejected");
    dirty=p;dirty.rig++;check(!validDoppelgangerWorldPacket(dirty,h,c),"foreign humanoid rig rejected");
    dirty=empty;dirty.local[1][0]=1;check(!validDoppelgangerWorldPacket(dirty,h,c),"absent clone cannot retain a body pose");
    DoppelgangerWorldOrder order{};''')
# Probe support for exact four-component cosmetic model.
p=S/'runtime_probe.cpp';text=p.read_text(encoding='utf-8-sig')
text=text.replace('&&std::strcmp(argv[2],"--rat-skeleton")','&&std::strcmp(argv[2],"--doppelganger-skeleton")&&std::strcmp(argv[2],"--rat-skeleton")')
text=text.replace('std::strcmp(argv[2],"--rat-skeleton")==0||','std::strcmp(argv[2],"--doppelganger-skeleton")==0||std::strcmp(argv[2],"--rat-skeleton")==0||')
text=text.replace('s,std::strcmp(argv[2],"--rat-skeleton")==0?', 's,std::strcmp(argv[2],"--doppelganger-skeleton")==0?NativeSkeletonScope::CosmeticDoppelganger:std::strcmp(argv[2],"--rat-skeleton")==0?')
p.write_text(text,encoding='utf-8')
# Four-component cosmetic body. Native idle graph schedules pose evaluation;
# copied full-body transforms replace its output before final publication.
text=(L/'assets/mesmerize_visual.def').read_text(encoding='utf-8-sig').replace('models/interactive/animated/mesmerize_v2/mesmerize_v2.md6','models/characters/female/emily/head01/emily_head01.md6').replace('components/interactive/animevolved_midnight.cpntanimevolved','components/d2coop/doppelganger_visual.cpntanimevolved')
text=text.replace('\tedit = {','\tedit = {\n\t\tflags = { hide = true; }')
(L/'assets/doppelganger_visual.def').write_text(text,encoding='utf-8')
(L/'assets/doppelganger_visual.cpntanimevolved').write_text('{\n edit = { m_animTree = "animtree/d2coop_doppelganger_visual.animtree"; }\n}\n',encoding='utf-8')
(L/'assets/doppelganger_visual.animtree').write_text('{\n animleaf "idle" {\n  rate 1\n  aliasname "empty_idle"\n  weightgroup "MD6_WEIGHTGROUP_ALL"\n  wrapmode "WRAP_REPEAT"\n }\n rootnode "idle"\n}\n',encoding='utf-8')
text=(L/'Build-D2SwarmRatVisualAssets.ps1').read_text(encoding='utf-8-sig')
start=text.index('$resources=@(');end=text.index('[void][IO.Directory]::CreateDirectory($package)',start)
resources=[('models/powers/d2coop/doppelganger_visual.def','doppelganger_visual.def','entitydef','entityDef'),('animtree/d2coop_doppelganger_visual.animtree','doppelganger_visual.animtree','animtree','animTree'),('components/d2coop/doppelganger_visual.cpntanimevolved','doppelganger_visual.cpntanimevolved','cpntanimevolved','cpntAnimEvolved')]
text=text[:start]+'$resources=@('+','.join("@{Name='%s';File='assets/%s';Kind='%s';Class='%s'}"%x for x in resources)+')\n'+text[end:]
text=text.replace('swarm-rat-gait-v301','doppelganger-visual-v423').replace('swarm-rat-visual-v296','doppelganger-visual-v423').replace('remote cosmetic rat model animation experiment','remote cosmetic Emily clone body experiment')
(L/'Build-D2DoppelgangerVisualAssets.ps1').write_text(text,encoding='utf-8')
(W/'Build-DoppelgangerVisual-v423.ps1').write_text((W/'Build-DoppelgangerStream-v418.ps1').read_text(encoding='utf-8-sig').replace('doppelganger-stream-','doppelganger-visual-').replace('-v418.log','-v423.log').replace('Build418','Build423'),encoding='utf-8')
print('Prepared219-joint native pose protocol2 and exact cosmetic visual assets423; runtime pending')
