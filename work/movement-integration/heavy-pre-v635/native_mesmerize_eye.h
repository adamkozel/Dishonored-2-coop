#pragma once
#include "native_mesmerize_world_owner.h"
#include "native_mesmerize_world.h"
#include "native_windblast_fx_records.h"
#include "native_fx_record_by_id.h"
// Read-only research. Native target FX update also performs gameplay work;
// do not invoke it to reproduce the eye. See exact 14072c8b0/14072e5c0 trace.
constexpr const char* kNativeMesmerizeEye="models/effects/gameplay/powers/mesmerize_v2/eye/mesmerize_v2_eye_01.fx";
struct NativeMesmerizeEyeSlots {
    std::uintptr_t list=0;unsigned flags=0;
    std::array<std::array<unsigned,2>,5> entries{}; // effect ID, reference count
};
struct NativeMesmerizeEyes {
    NativeMesmerizeWorld world{};NativeMesmerizeEyeSlots slots{};
    std::vector<NativeWindblastFxRecord> effects;
};
template<class Read> bool readMesmerizeEyeSlots(Read read,std::uintptr_t component,NativeMesmerizeEyeSlots& out){
    NativeMesmerizeEyeSlots s{};
    if(!component||!read(component+0x140,&s.list,8)||!s.list||!read(component+0x148,&s.flags,4)||(s.flags&0xffffff)!=5||
       !read(s.list,s.entries.data(),sizeof(s.entries)))return false;
    std::unordered_set<unsigned> ids;
    for(const auto& e:s.entries){
        if(e[1]>32||(e[1]&&e[0]==~0u)||(e[0]!=~0u&&!ids.insert(e[0]).second))return false;
    }
    out=s;return true;
}
inline bool selectNativeMesmerizeEyes(const NativeMesmerizeWorld& w,const NativeMesmerizeEyeSlots& slots,
    std::uintptr_t declaration,const std::vector<NativeWindblastFxRecord>& records,std::vector<NativeWindblastFxRecord>& out){
    out.clear();if(!w.present||!w.actor||!w.component||!declaration||(slots.flags&0xffffff)!=5)return false;
    for(const auto& entry:slots.entries){
        if(!entry[1])continue;
        const NativeWindblastFxRecord* found=nullptr;
        for(const auto& r:records)if(r.id==entry[0]){if(found)return false;found=&r;}
        if(!found||found->name!=kNativeMesmerizeEye||found->owner!=w.handle||found->ownerActor!=w.actor||
           found->parent!=0x3fff0010||found->ownerDefinition!="models/powers/mesmerize/mesmerize_object.def"||found->declaration!=declaration)return false;
        float distance=0;for(unsigned i=0;i<3;++i){const float delta=found->origin[i]-w.transform[i];distance+=delta*delta;}
        if(!std::isfinite(distance)||distance>100)return false;
        for(float v:found->axis)if(!std::isfinite(v))return false;
        for(unsigned i=0;i<3;++i)for(unsigned j=i;j<3;++j){float d=0;for(unsigned k=0;k<3;++k)d+=found->axis[i*3+k]*found->axis[j*3+k];if(std::fabs(d-(i==j?1.f:0.f))>.02f)return false;}
        const auto& m=found->axis;const float det=m[0]*(m[4]*m[8]-m[7]*m[5])-m[3]*(m[1]*m[8]-m[7]*m[2])+m[6]*(m[1]*m[5]-m[4]*m[2]);
        if(det<.98f||det>1.02f)return false;
        out.push_back(*found);
    }
    return true;
}
template<class Read> bool readNativeMesmerizeEyes(Read read,std::uintptr_t base,NativeMesmerizeEyes& out,unsigned* stage=nullptr,const NativeMesmerizeWorld* known=nullptr){
    const auto at=[&](unsigned n){if(stage)*stage=n;};NativeMesmerizeEyes s{};at(1);
    if(!(known?readCurrentNativeMesmerizeWorld(read,base,*known,s.world):readNativeMesmerizeWorld(read,base,s.world)))return false;
    if(!s.world.present){out=std::move(s);at(0);return true;}
    at(2);if(!readMesmerizeEyeSlots(read,s.world.component,s.slots))return false;
    std::uintptr_t declaration=0,vt=0,name=0;std::string text;
    at(3);if(!read(s.world.declaration+0x128,&declaration,8)||!declaration||!read(declaration,&vt,8)||vt!=base+0x1ab3138||
       !read(declaration+8,&name,8)||!worldIdentityString(read,name,text)||text!=kNativeMesmerizeEye)return false;
    std::vector<NativeWindblastFxRecord> records;at(4);
    for(const auto& entry:s.slots.entries)if(entry[1]){
        NativeWindblastFxRecord r{};if(!readNativeFxRecordById(read,base,s.world.equipment.player.sword.game,entry[0],r))return false;
        records.push_back(std::move(r));
    }
    at(5);if(!selectNativeMesmerizeEyes(s.world,s.slots,declaration,records,s.effects))return false;
    NativeMesmerizeWorld end{};NativeMesmerizeEyeSlots slots{};std::uintptr_t again=0;at(6);
    if(!(known?readCurrentNativeMesmerizeWorld(read,base,s.world,end):readNativeMesmerizeWorld(read,base,end))||!end.present||end.actor!=s.world.actor||end.handle!=s.world.handle||end.state!=s.world.state||
       end.component!=s.world.component||end.declaration!=s.world.declaration||end.transform!=s.world.transform||
       !read(end.declaration+0x128,&again,8)||again!=declaration||!readMesmerizeEyeSlots(read,end.component,slots)||
       slots.list!=s.slots.list||slots.flags!=s.slots.flags||slots.entries!=s.slots.entries)return false;
    for(const auto& e:s.effects){unsigned owner=0,id=0;std::array<float,12> transform{};
        if(!read(e.record,&owner,4)||owner!=e.owner||!read(e.record+4,&id,4)||id!=e.id||!read(e.record+8,&again,8)||again!=e.declaration||
           !read(e.record+0x38,transform.data(),48)||std::memcmp(transform.data(),e.origin,12)||std::memcmp(transform.data()+3,e.axis,36))return false;
    }
    out=std::move(s);at(0);return true;
}
