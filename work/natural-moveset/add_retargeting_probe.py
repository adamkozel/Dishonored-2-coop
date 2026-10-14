from pathlib import Path
P=Path(__file__).resolve().parents[2]/'outputs/d2-coop-lab/src'
p=P/'world_identity_tests.cpp';s=p.read_text();s=s.replace('#include "native_crossbow_presentation_tests.h"','#include "native_crossbow_presentation_tests.h"\n#include "native_retargeting_tests.h"');s=s.replace('int main() {','int main() {\n    nativeRetargetingTests();');p.write_text(s)
p=P/'runtime_probe.cpp';s=p.read_text();s=s.replace('#include "native_player_carry_scene.h"','#include "native_player_carry_scene.h"\n#include "native_retargeting.h"')
s=s.replace('(argc==4&&std::strcmp(argv[2],"--loading-world")','(argc==4&&std::strcmp(argv[2],"--retargeting")&&std::strcmp(argv[2],"--loading-world")')
anchor='    if(argc==3 && std::strcmp(argv[2],"--player-states")==0) {'
addition=r'''    if(argc==4 && std::strcmp(argv[2],"--retargeting")==0) {
        NativeRetargeting s{};const auto handle=static_cast<unsigned>(std::strtoul(argv[3],nullptr,0));
        const bool valid=readNativeRetargeting([&](auto p,auto b,auto n){return read(process.value,p,b,n);},base,handle,s);
        std::cout<<"{\"valid\":"<<(valid?"true":"false")<<",\"handle\":"<<handle;
        if(valid){
            std::cout<<",\"actor\":\"0x"<<std::hex<<s.actor<<"\",\"component\":\"0x"<<s.component<<"\",\"declaration\":\"0x"<<s.declaration
                <<"\",\"records\":\"0x"<<s.records<<"\",\"target\":\"0x"<<s.target<<"\",\"targetFrame\":\"0x"<<s.targetFrame
                <<"\",\"targetPhysics\":\"0x"<<s.targetPhysics<<"\""<<std::dec<<",\"count\":"<<s.count<<",\"mode\":"<<s.mode
                <<",\"flags\":"<<s.flags<<",\"targetHandle\":"<<s.targetHandle<<",\"ownerJoint\":"<<s.ownerJoint<<",\"targetJoint\":"<<s.targetJoint;
            std::cout<<",\"ownerOffset\":[";for(unsigned i=0;i<7;++i)std::cout<<(i?",":"")<<s.ownerOffset[i];
            std::cout<<"],\"targetOffset\":[";for(unsigned i=0;i<7;++i)std::cout<<(i?",":"")<<s.targetOffset[i];std::cout<<']';
        }
        std::cout<<"}\n";return 0;
    }
'''
assert s.count(anchor)==1;s=s.replace(anchor,addition+anchor);p.write_text(s)
