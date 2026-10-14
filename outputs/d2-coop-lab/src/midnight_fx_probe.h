#pragma once
#include <sstream>
#include "material_parm_probe.h"
// External read-only research438. These observations do not authorize playback.
// Layouts:7ce7e0 bindings;7db090/7db430 group;7e0690 keys;84c810/84c9e0 FX.
inline std::string midnightFxResearch(HANDLE process,std::uintptr_t base,std::uintptr_t game,
    std::uintptr_t instance,std::uintptr_t binding,std::uintptr_t actor){
    std::ostringstream out;unsigned failure=0;
    struct Frozen {std::uintptr_t address;std::vector<unsigned char> bytes;};
    std::vector<Frozen> frozen;
    const auto get=[&](std::uintptr_t p,void* b,std::size_t n){return read(process,p,b,n);};
    const auto keep=[&](std::uintptr_t p,void* b,std::size_t n){
        if(!get(p,b,n))return false;
        frozen.push_back({p,std::vector<unsigned char>(static_cast<unsigned char*>(b),static_cast<unsigned char*>(b)+n)});return true;
    };
    const auto ptr=[&](std::uintptr_t p,std::uintptr_t wanted){std::uintptr_t x=0;return keep(p,&x,8)&&x==wanted;};
    std::uintptr_t group=0,tracks=0;unsigned flags=0,sceneId=0;
    bool valid=keep(binding,&group,8)&&group&&ptr(binding+8,actor)&&ptr(group,base+0x1c6ca98)&&
        ptr(group+0xc8,instance)&&ptr(group+0xd0,actor)&&keep(instance+0x158,&sceneId,4)&&sceneId&&
        keep(group+0x20,&tracks,8)&&keep(group+0x28,&flags,4)&&(flags&0xffffff)<=32&&(!(flags&0xffffff)||tracks);
    out<<"{\"group\":\"0x"<<std::hex<<group<<std::dec<<"\",\"trackCount\":"<<(flags&0xffffff)<<",\"tracks\":[";
    if(!valid)failure=__LINE__;
    bool first=true;
    for(unsigned i=0;valid&&i<(flags&0xffffff);++i){
        std::uintptr_t track=0,vt=0;
        if(!keep(tracks+i*8,&track,8)||!track||!keep(track,&vt,8)||!ptr(track+0x10,group)){valid=false;failure=__LINE__;break;}
        if(!first)out<<',';first=false;
        out<<"{\"track\":\"0x"<<std::hex<<track<<std::dec<<"\",\"type\":"<<jsonString(objectType(process,base,track));
        if(vt==base+0x1c7f5a8){
            std::uintptr_t keys=0,records=0;unsigned keyFlags=0,recordFlags=0;
            struct Key {std::uintptr_t pointer;unsigned id,a,b,c;};static_assert(sizeof(Key)==24);
            struct Record {unsigned key,handle,stop;};static_assert(sizeof(Record)==12);
            std::vector<Key> keyList;std::vector<Record> recordList;
            valid=keep(track+0x18,&keys,8)&&keep(track+0x20,&keyFlags,4)&&(keyFlags&0xffffff)<=32&&
                keep(track+0xc0,&records,8)&&keep(track+0xc8,&recordFlags,4)&&(recordFlags&0xffffff)<=32;
            if(valid){keyList.resize(keyFlags&0xffffff);recordList.resize(recordFlags&0xffffff);
                // Key timing/state words can advance while observing. Only identity is frozen.
                valid=(keyList.empty()||(keys&&get(keys,keyList.data(),keyList.size()*sizeof(Key))))&&
                    (recordList.empty()||(records&&keep(records,recordList.data(),recordList.size()*sizeof(Record))));
            }
            if(!valid)failure=__LINE__;
            out<<",\"keyCount\":"<<(keyFlags&0xffffff)<<",\"records\":[";
            bool firstRecord=true;
            for(const auto& r:recordList){
                if(!firstRecord)out<<',';firstRecord=false;
                auto helper=worldIdentityEntity(get,game,r.handle);
                out<<"{\"keyId\":"<<r.key<<",\"helperHandle\":"<<r.handle<<",\"stopMode\":"<<r.stop
                    <<",\"helper\":\"0x"<<std::hex<<helper<<std::dec<<"\",\"helperType\":"<<jsonString(objectType(process,base,helper));
                std::uintptr_t key=0,prop=0,decl=0,name=0,socket=0;bool resolved=false;
                for(unsigned k=0;k<keyList.size();++k)if(keyList[k].id==r.key){
                    key=keyList[k].pointer;unsigned id=0;
                    resolved=key&&ptr(keys+k*sizeof(Key),key)&&keep(key+8,&id,4)&&id==r.key&&
                        keep(key+0x28,&prop,8)&&prop&&ptr(prop,base+0x1afdfd0)&&keep(prop+8,&decl,8)&&decl&&
                        ptr(decl,base+0x1ab3138)&&keep(decl+8,&name,8)&&name&&keep(prop+0x10,&socket,8)&&socket;
                    break;
                }
                out<<",\"keyResolved\":"<<(resolved?"true":"false")<<",\"key\":\"0x"<<std::hex<<key<<"\",\"property\":\"0x"<<prop<<"\",\"declaration\":\"0x"<<decl<<std::dec
                    <<"\",\"effect\":"<<jsonString(resolved?readString(process,name):"")<<",\"socket\":"<<jsonString(resolved?readString(process,socket):"")<<'}';
                if(!helper||!resolved){valid=false;failure=__LINE__;}
                else {unsigned handle=0;if(!keep(helper+0xa8,&handle,4)||handle!=r.handle){valid=false;failure=__LINE__;}}
            }
            out<<']';
        }
        if(vt==base+0x1c35b08){
            std::uintptr_t keys=0;unsigned keyFlags=0;
            struct MaterialKey{std::uintptr_t pointer;unsigned id,a,b,c;};
            std::vector<MaterialKey> list;
            bool materialValid=keep(track+0x18,&keys,8)&&keep(track+0x20,&keyFlags,4)&&(keyFlags&0xffffff)<=32;
            if(materialValid){list.resize(keyFlags&0xffffff);materialValid=list.empty()||(keys&&get(keys,list.data(),list.size()*sizeof(MaterialKey)));}
            out<<",\"materialKeys\":[";bool firstKey=true;
            for(unsigned k=0;materialValid&&k<list.size();++k){const auto key=list[k].pointer;
                std::uintptr_t property=0,declaration=0,name=0;unsigned id=0,type=0;unsigned char clear=0,attachments=0;
                materialValid=key&&ptr(keys+k*sizeof(MaterialKey),key)&&keep(key+0x28,&property,8)&&property&&ptr(property,base+0x1afe008)&&
                    keep(property+8,&declaration,8)&&declaration&&keep(declaration+8,&name,8)&&name&&
                    keep(declaration+0x48,&id,4)&&id<65536&&keep(declaration+0x4c,&type,4)&&
                    keep(property+0x10,&clear,1)&&clear<=1&&keep(property+0x11,&attachments,1)&&attachments<=1;
                if(!materialValid)break;if(!firstKey)out<<',';firstKey=false;
                out<<"{\"keyId\":"<<list[k].id<<",\"declaration\":\"0x"<<std::hex<<declaration<<std::dec<<"\",\"name\":"<<jsonString(readString(process,name))
                    <<",\"id\":"<<id<<",\"valueType\":"<<((type>>4)&3)<<",\"clearAtEnd\":"<<(clear?"true":"false")<<",\"attachments\":"<<(attachments?"true":"false")<<'}';
            }
            out<<"],\"materialValid\":"<<(materialValid?"true":"false");
            unsigned handle=0;if(get(actor+0xa8,&handle,4))out<<",\"materialSnapshot\":"<<materialParmResearch(process,base,game,handle);
            if(!materialValid){valid=false;failure=__LINE__;}
        }
        out<<'}';
    }
    // Any membership/identity drift is unknown; it is not absence or a new event.
    for(const auto& f:frozen){std::vector<unsigned char> again(f.bytes.size());if(!get(f.address,again.data(),again.size())||again!=f.bytes){valid=false;failure=__LINE__;break;}}
    out<<"],\"valid\":"<<(valid?"true":"false")<<",\"failureLine\":"<<failure<<'}';return out.str();
}
