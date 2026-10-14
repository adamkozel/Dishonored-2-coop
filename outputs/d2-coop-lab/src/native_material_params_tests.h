#pragma once
#include "native_material_params.h"
void nativeMaterialParamsTests(){
    constexpr std::uintptr_t base=0x140000000,actor=0x900000,render=0x910000,values=0x920000,ids=0x930000;
    Memory m;m.q(actor+0xf0,render);m.q(render,base+0x1b0d628);m.q(render+0x1950,values);m.d(render+0x1958,0x5000002);
    m.q(render+0x1960,ids);m.d(render+0x1968,0x5000002);
    std::array<unsigned short,2> index{7,6};std::array<std::array<unsigned,4>,2> words{{{4,0,0,0},{0x3f000000,0,0,0}}};
    m.put(ids,index.data(),4);m.put(values,words.data(),32);NativeMaterialParams out{};
    auto inspect=[&](Memory& n){return readNativeMaterialParams([&](auto p,auto b,auto z){return n.read(p,b,z);},base,actor,render,out);};
    require(inspect(m)&&out.find(7)==0&&out.find(6)==1&&out.words==std::vector<std::array<unsigned,4>>(words.begin(),words.end()),"raw integer effect and float fade remain distinct native words");
    auto n=m;n.q(actor+0xf0,render+8);require(!inspect(n),"foreign renderer refused");
    n=m;n.q(render,base+0x1b0dd88);require(!inspect(n),"unexpected render class refused for scoped skinned body");
    n=m;n.d(render+0x1968,3);require(!inspect(n),"unequal material id/value tables refused");
    n=m;index[1]=7;n.put(ids,index.data(),4);require(!inspect(n),"duplicate parameter id refused");
    unsigned reads=0;const bool drift=readNativeMaterialParams([&](auto p,auto b,auto z){const bool ok=m.read(p,b,z);if(ok&&p==values&&++reads==2){const unsigned changed=5;std::memcpy(b,&changed,4);}return ok;},base,actor,render,out);
    require(!drift,"concurrent material value drift is unknown");
    n=m;n.d(render+0x1958,0);n.d(render+0x1968,0);require(inspect(n)&&out.ids.empty(),"empty baseline is valid absence");
}
