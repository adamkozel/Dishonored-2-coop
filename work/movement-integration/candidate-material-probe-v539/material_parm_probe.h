#pragma once
#include <array>
#include <cmath>
#include <cstring>
#include <vector>
// Read-only research539. Exact-image native material parameter update path:
// 661960 -> 662540 -> 876030 -> 2369c0 -> 343990. No renderer mutation.
inline std::string materialParmResearch(HANDLE process,std::uintptr_t base,std::uintptr_t game,unsigned handle){
    std::ostringstream out;
    auto get=[&](std::uintptr_t p,void* b,std::size_t n){return read(process,p,b,n);};
    const auto actor=worldIdentityEntity(get,game,handle);
    std::uintptr_t render=0,vt=0,values=0,ids=0;
    unsigned valueFlags=0,idFlags=0;bool valid=actor&&get(actor+0xf0,&render,8)&&render&&get(render,&vt,8)&&
        (vt==base+0x1b0d628||vt==base+0x1b0dd88)&&get(render+0x1950,&values,8)&&get(render+0x1958,&valueFlags,4)&&
        get(render+0x1960,&ids,8)&&get(render+0x1968,&idFlags,4)&&(valueFlags&0xffffff)==(idFlags&0xffffff)&&
        (idFlags&0xffffff)<=256&&(!(idFlags&0xffffff)||(values&&ids));
    std::vector<unsigned short> indices;std::vector<std::array<unsigned,4>> words;
    if(valid){indices.resize(idFlags&0xffffff);words.resize(indices.size());
        valid=indices.empty()||(get(ids,indices.data(),indices.size()*2)&&get(values,words.data(),words.size()*16));}
    out<<"{\"handle\":"<<handle<<",\"actor\":\"0x"<<std::hex<<actor<<"\",\"render\":\"0x"<<render
       <<std::dec<<"\",\"renderType\":"<<jsonString(render?objectType(process,base,render):"")<<",\"parameters\":[";
    if(valid)for(unsigned i=0;i<indices.size();++i){out<<(i?",":"")<<"{\"id\":"<<indices[i]<<",\"words\":[";
        for(unsigned k=0;k<4;++k)out<<(k?",":"")<<words[i][k];out<<"],\"floats\":[";
        for(unsigned k=0;k<4;++k){float f=0;std::memcpy(&f,&words[i][k],4);if(k)out<<',';if(std::isfinite(f))out<<f;else out<<"null";}out<<"]}";
    }
    if(valid){std::uintptr_t p=0;unsigned n=0;auto again=words;auto againIds=indices;
        valid=worldIdentityEntity(get,game,handle)==actor&&get(actor+0xf0,&p,8)&&p==render&&get(render,&p,8)&&p==vt&&
            get(render+0x1950,&p,8)&&p==values&&get(render+0x1958,&n,4)&&n==valueFlags&&
            get(render+0x1960,&p,8)&&p==ids&&get(render+0x1968,&n,4)&&n==idFlags&&
            (indices.empty()||(get(ids,againIds.data(),indices.size()*2)&&againIds==indices&&get(values,again.data(),words.size()*16)&&again==words));}
    out<<"],\"stable\":"<<(valid?"true":"false")<<'}';return out.str();
}
