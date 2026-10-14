#pragma once
#include "native_mesmerize_eye.h"
// Read-only target/beam research from 14072c8b0 and 14072e5c0. No NPC
// state changes, effect creation or native power calls are performed here.
constexpr const char* kMesmerizeTargetFxNames[]={
 "models/effects/gameplay/powers/mesmerize_v2/link/mesmerize_v2_link_01.fx",
 "models/effects/gameplay/powers/mesmerize_v2/link/mesmerize_v2_link_trail_01.fx",
 "models/effects/gameplay/powers/mesmerize_v2/link/mesmerize_v2_link_head_01.fx"};
struct NativeMesmerizeTargetKey {
 unsigned handle=0,beam=~0u,trail=~0u,head=~0u;int anchor=-1;unsigned char removing=0,active=0;
 bool operator==(const NativeMesmerizeTargetKey& b)const{return handle==b.handle&&beam==b.beam&&trail==b.trail&&head==b.head&&anchor==b.anchor&&removing==b.removing&&active==b.active;}
};
struct NativeMesmerizeTargetTable {std::uintptr_t list=0;unsigned flags=0;std::vector<NativeMesmerizeTargetKey> entries;};
template<class Read> bool readMesmerizeTargetTable(Read read,std::uintptr_t component,NativeMesmerizeTargetTable& out){
 NativeMesmerizeTargetTable s{};if(!component||!read(component+0x1a8,&s.list,8)||!read(component+0x1b0,&s.flags,4)||(s.flags&0xffffff)>32||((s.flags&0xffffff)&&!s.list))return false;
 std::unordered_set<unsigned> targets,effects;
 for(unsigned i=0;i<(s.flags&0xffffff);++i){std::array<unsigned char,0x70> bytes{};const auto p=s.list+std::uintptr_t(i)*0x70;if(!read(p,bytes.data(),bytes.size()))return false;
  NativeMesmerizeTargetKey e{};std::memcpy(&e.handle,bytes.data(),4);std::memcpy(&e.beam,bytes.data()+0x30,4);std::memcpy(&e.trail,bytes.data()+0x34,4);std::memcpy(&e.head,bytes.data()+0x38,4);std::memcpy(&e.anchor,bytes.data()+0x60,4);e.removing=bytes[0x6c];e.active=bytes[0x6d];
  if(e.handle==0x3fff0010||e.handle==~0u||!targets.insert(e.handle).second||e.anchor< -1||e.anchor>=5||e.removing>1||e.active>1)return false;
  for(unsigned id:{e.beam,e.trail,e.head})if(id!=~0u&&(!id||!effects.insert(id).second))return false;
  s.entries.push_back(e);
 }
 out=std::move(s);return true;
}
struct NativeMesmerizeTargetEffect {unsigned kind=0;NativeWindblastFxRecord fx{};std::array<float,12> other{};};
struct NativeMesmerizeTarget {NativeMesmerizeTargetKey key{};std::uintptr_t actor=0,definition=0,vtable=0;std::string name;std::vector<NativeMesmerizeTargetEffect> effects;};
struct NativeMesmerizeTargets {NativeMesmerizeWorld world{};NativeMesmerizeTargetTable table{};std::vector<NativeMesmerizeTarget> targets;};
inline bool mesmerizeTargetTransform(const float* pose,const std::array<float,12>& center,float radius){
 float distance=0;for(unsigned i=0;i<12;++i)if(!std::isfinite(pose[i]))return false;
 for(unsigned i=0;i<3;++i){float d=pose[i]-center[i];distance+=d*d;}if(distance>radius*radius)return false;
 for(unsigned i=0;i<3;++i)for(unsigned j=i;j<3;++j){float d=0;for(unsigned k=0;k<3;++k)d+=pose[3+i*3+k]*pose[3+j*3+k];if(std::fabs(d-(i==j?1.f:0.f))>.02f)return false;}
 const auto* m=pose+3;const float det=m[0]*(m[4]*m[8]-m[7]*m[5])-m[3]*(m[1]*m[8]-m[7]*m[2])+m[6]*(m[1]*m[5]-m[4]*m[2]);return det>=.98f&&det<=1.02f;
}
template<class Read> bool readNativeMesmerizeTargets(Read read,std::uintptr_t base,NativeMesmerizeTargets& out,unsigned* stage=nullptr,const NativeMesmerizeWorld* known=nullptr){
 const auto at=[&](unsigned n){if(stage)*stage=n;};NativeMesmerizeTargets s{};at(1);
 if(!(known?readCurrentNativeMesmerizeWorld(read,base,*known,s.world):readNativeMesmerizeWorld(read,base,s.world)))return false;if(!s.world.present){out=std::move(s);at(0);return true;}
 at(2);if(!readMesmerizeTargetTable(read,s.world.component,s.table))return false;
 const auto game=s.world.equipment.player.sword.game;std::array<std::uintptr_t,3> decls{};constexpr unsigned offsets[]={0x138,0x140,0x150};
 at(3);for(unsigned k=0;k<3;++k){std::uintptr_t vt=0,p=0;std::string name;
  if(!read(s.world.declaration+offsets[k],&decls[k],8)||!decls[k]||!read(decls[k],&vt,8)||vt!=base+0x1ab3138||!read(decls[k]+8,&p,8)||!worldIdentityString(read,p,name)||name!=kMesmerizeTargetFxNames[k])return false;
 }
 for(const auto& key:s.table.entries){NativeMesmerizeTarget t{};t.key=key;t.actor=worldIdentityEntity(read,game,key.handle);std::uintptr_t p=0,vt=0;at(4);
  if(!t.actor||!read(t.actor,&t.vtable,8)||t.vtable<base+0x1a00000||t.vtable>=base+0x1e00000||!read(t.actor+0x2e8,&t.definition,8)||!t.definition||!read(t.definition,&vt,8)||vt!=base+0x1ab58f8||!read(t.definition+8,&p,8)||!worldIdentityString(read,p,t.name))return false;
  const unsigned ids[]={key.beam,key.trail,key.head};
  for(unsigned k=0;k<3;++k)if(ids[k]!=~0u){NativeMesmerizeTargetEffect e{};e.kind=k+1;at(5);
   if(!readNativeFxRecordById(read,base,game,ids[k],e.fx)||e.fx.declaration!=decls[k]||e.fx.name!=kMesmerizeTargetFxNames[k])return false;
   const auto owner=k==2?key.handle:s.world.handle;const auto actor=k==2?t.actor:s.world.actor;
   at(6);if(e.fx.owner!=owner||e.fx.ownerActor!=actor)return false;
   std::array<float,12> pose{};std::memcpy(pose.data(),e.fx.origin,12);std::memcpy(pose.data()+3,e.fx.axis,36);at(7);
   if(!mesmerizeTargetTransform(pose.data(),s.world.transform,100))return false;
   if(k<2&&(!read(e.fx.record+0x80,e.other.data(),48)||!mesmerizeTargetTransform(e.other.data(),s.world.transform,10)))return false;
   t.effects.push_back(std::move(e));
  }
  s.targets.push_back(std::move(t));
 }
 NativeMesmerizeWorld w{};NativeMesmerizeTargetTable table{};at(8);
 if(!(known?readCurrentNativeMesmerizeWorld(read,base,s.world,w):readNativeMesmerizeWorld(read,base,w))||!w.present||w.handle!=s.world.handle||w.actor!=s.world.actor||w.component!=s.world.component||w.declaration!=s.world.declaration||w.state!=s.world.state||w.transform!=s.world.transform||!readMesmerizeTargetTable(read,w.component,table)||table.list!=s.table.list||table.flags!=s.table.flags||table.entries!=s.table.entries)return false;
 at(9);for(const auto& t:s.targets){std::uintptr_t p=0;if(worldIdentityEntity(read,game,t.key.handle)!=t.actor||!read(t.actor+0x2e8,&p,8)||p!=t.definition)return false;
  for(const auto& e:t.effects){unsigned owner=0,id=0;std::array<float,12> pose{},other{};
   if(!read(e.fx.record,&owner,4)||owner!=e.fx.owner||!read(e.fx.record+4,&id,4)||id!=e.fx.id||!read(e.fx.record+8,&p,8)||p!=e.fx.declaration||!read(e.fx.record+0x38,pose.data(),48)||std::memcmp(pose.data(),e.fx.origin,12)||std::memcmp(pose.data()+3,e.fx.axis,36))return false;
   if(e.kind<3&&(!read(e.fx.record+0x80,other.data(),48)||other!=e.other))return false;
  }
 }
 out=std::move(s);at(0);return true;
}
