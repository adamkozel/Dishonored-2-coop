from pathlib import Path
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab/src'
def edit(p,a,b):
    s=p.read_text(encoding='utf-8-sig');assert s.count(a)==1,(p,a[:90]);p.write_text(s.replace(a,b),encoding='utf-8')
p=S/'doppelganger_visual.h'
edit(p,'SRWLOCK doppelgangerCloneLock=', '''// Probe brackets native phase3 publication and this exact expected pose.
struct DoppelgangerClonePublication {
    std::atomic<std::uint64_t> sequence{0};std::atomic<std::uintptr_t> animation{0};
    std::uint64_t sourceSequence=0;std::array<std::array<float,12>,kDoppelgangerJoints> expected{};
} doppelgangerClonePublication;
static_assert(offsetof(DoppelgangerClonePublication,expected)==24);
void doppelgangerClonePosePhase(void* component,int phase,bool finished){
    auto& p=doppelgangerClonePublication;if(phase!=3||p.animation.load()!=reinterpret_cast<std::uintptr_t>(component))return;
    if(finished){const auto seq=p.sequence.load();p.sequence.fetch_add((seq&1)?1:2);}else if(!(p.sequence.load()&1))p.sequence.fetch_add(1);
}
SRWLOCK doppelgangerCloneLock=''')
edit(p,'        ++doppelgangerCloneApplied;doppelgangerCloneAppliedSequence.store(s.sequence);','''        if(doppelgangerClonePublication.sequence.load()&1){
            doppelgangerClonePublication.sourceSequence=s.sequence;
            for(unsigned j=0;j<kDoppelgangerJoints;++j)for(unsigned c=0;c<4;++c)for(unsigned r=0;r<3;++r)doppelgangerClonePublication.expected[j][c*3+r]=s.local[j].v[c*4+r];
        }
        ++doppelgangerCloneApplied;doppelgangerCloneAppliedSequence.store(s.sequence);''')
edit(p,'p.skeleton=rig.skeleton;p.target=target;','p.skeleton=rig.skeleton;p.target=target;doppelgangerClonePublication.animation.store(target.animation);')
edit(p,'for(unsigned i=0;i<12;++i)fprintf(f,"%s%.7g",i?",":"",p.transform[i]);fputs("]}\\n",f);','for(unsigned i=0;i<12;++i)fprintf(f,"%s%.7g",i?",":"",p.transform[i]);fprintf(f,"],\\\"publication\\\":\\\"0x%llx\\\",\\\"publicationSequence\\\":%llu}\\n",reinterpret_cast<unsigned long long>(&doppelgangerClonePublication),doppelgangerClonePublication.sequence.load());')
p=S/'body_probe.h'
edit(p,'void applyDoppelgangerCloneBody(void* component,void* actor);','void applyDoppelgangerCloneBody(void* component,void* actor);\nvoid doppelgangerClonePosePhase(void* component,int phase,bool finished);')
edit(p,'void capturePosePhase(void* component,int phase){','void capturePosePhase(void* component,int phase){\n    doppelgangerClonePosePhase(component,phase,false);')
edit(p,'    realCapturePosePhase(component,phase);','    realCapturePosePhase(component,phase);\n    doppelgangerClonePosePhase(component,phase,true);')
p=S/'runtime_probe.cpp';s=p.read_text(encoding='utf-8-sig')
s=s.replace('&&std::strcmp(argv[2],"--skeleton-final")','&&std::strcmp(argv[2],"--doppelganger-skeleton-final")&&std::strcmp(argv[2],"--skeleton-final")',1)
s=s.replace('(argc==5&&std::strcmp(argv[2],"--watch-potion-states")&&std::strcmp(argv[2],"--skeleton-final")==0)','(argc==5&&(!std::strcmp(argv[2],"--doppelganger-skeleton-final")||!std::strcmp(argv[2],"--skeleton-final")))')
s=s.replace('std::strcmp(argv[2],"--doppelganger-skeleton")==0?NativeSkeletonScope::CosmeticDoppelganger','(std::strcmp(argv[2],"--doppelganger-skeleton")==0||std::strcmp(argv[2],"--doppelganger-skeleton-final")==0)?NativeSkeletonScope::CosmeticDoppelganger')
a='        bool finalComplete=false;'
assert s.count(a)==1
s=s.replace(a,'''        const bool cloneFinal=argc==5&&!std::strcmp(argv[2],"--doppelganger-skeleton-final");
        std::array<std::array<float,12>,219> expectedClone{};std::uint64_t expectedSequence=0;double poseError=0;
        if(cloneFinal){
            if(s.localPose.size()!=219||!read(process.value,publication+16,&expectedSequence,8)||!read(process.value,publication+24,expectedClone.data(),sizeof(expectedClone)))return 64;
            for(unsigned j=0;j<219;++j)for(unsigned c=0;c<4;++c)for(unsigned r=0;r<3;++r){const double expected=expectedClone[j][c*3+r];if(!std::isfinite(expected))return 64;poseError=(std::max)(poseError,std::fabs(double(s.localPose[j][c*4+r])-expected));}
        }
'''+a)
a='<<",\\\"nativeNamesValid\\\":"<<(namesValid?"true":"false")'
assert s.count(a)==1
s=s.replace(a,'<<",\\\"cloneExpectedSequence\\\":"<<expectedSequence<<",\\\"cloneExpectedPoseError\\\":"<<poseError'+a)
p.write_text(s,encoding='utf-8')
print('Added independent219-joint final-pose observer bracketed around native phase3 publication')
