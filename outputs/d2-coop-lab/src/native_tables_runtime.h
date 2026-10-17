#pragma once
#include "native_tables.h"
#include <mutex>
// The installer's native-tables.bin (product mode) or the lab's <status path>.tables.bin, loaded on first use.
inline const NativeTables& nativeTables(){
    static NativeTables tables;static std::once_flag once;
    std::call_once(once,[]{char path[MAX_PATH]{};
        const int n=productMode?std::snprintf(path,sizeof(path),"%s\\native-tables.bin",productDataDir):std::snprintf(path,sizeof(path),"%s.tables.bin",statusPath);
        if(n>0&&n<int(sizeof(path)))tables.load(path);});
    return tables;
}
// coop_tables_probe: <status path>.native-tables.json lists every table the DLL looks up, with the shape it uses.
void probeNativeTables(){
    const auto& t=nativeTables();
    struct Use {const char* name;std::initializer_list<std::uint32_t> dims;};
    const Use uses[]={{"ranged_hand_tracks_corvo",{12,120,7}},{"ranged_hand_tracks_emily",{12,120,7}},
        {"crossbow_corvo_idle",{0,0,7}},{"crossbow_corvo_fire",{0,0,7}},{"crossbow_corvo_outofammo",{0,0,7}},{"crossbow_corvo_parents",{0}},{"crossbow_corvo_bind",{0,7}},
        {"crossbow_emily_idle",{0,0,7}},{"crossbow_emily_fire",{0,0,7}},{"crossbow_emily_outofammo",{0,0,7}},{"crossbow_emily_parents",{0}},{"crossbow_emily_bind",{0,7}},
        {"grenade_idle",{0,6,7}},{"grenade_cooking_in",{0,6,7}},{"grenade_cooking_idle",{0,6,7}},{"grenade_parents",{6}},{"grenade_bind",{6,7}}};
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.native-tables.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    unsigned found=0;FILE* f=nullptr;
    if(!fopen_s(&f,tmp,"wb")&&f){
        fprintf(f,"{\"readOnly\":true,\"productMode\":%s,\"loaded\":%s,\"tables\":[",productMode?"true":"false",t.loaded()?"true":"false");
        bool first=true;
        for(const auto& u:uses){const auto* x=t.find(u.name,u.dims);if(x)++found;
            fprintf(f,"%s{\"name\":\"%s\",\"found\":%s,\"dims\":[",first?"":",",u.name,x?"true":"false");first=false;
            if(x)for(std::size_t k=0;k<x->dims.size();++k)fprintf(f,"%s%u",k?",":"",x->dims[k]);fprintf(f,"]}");}
        fprintf(f,"],\"found\":%u,\"expected\":%u}\n",found,unsigned(sizeof(uses)/sizeof(uses[0])));
        const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
    }
    consoleOutput("COOP: native tables loaded=%u found=%u\n",t.loaded()?1:0,found);
}
