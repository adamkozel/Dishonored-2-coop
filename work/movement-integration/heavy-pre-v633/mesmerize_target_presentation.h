#pragma once
// Cosmetic effects only. Do not invoke 72e5c0 or target-add 72a280: those native
// routines also issue NPC events and damage. Source target handles are opaque.
std::uint64_t mesmerizeLinkStarted[3]{},mesmerizeLinkStopped[3]{},mesmerizeLinkUpdated[3]{},mesmerizeLinkRefused=0;
bool stopMesmerizeLink(MesmerizeVisualLink& link){
 if(!link.sourceId)return true;if(!nativePowerFxApi())return false;
 if(link.effect.expires){if(nativePowerFxRecord(link.effect))reinterpret_cast<void(*)(std::uintptr_t,unsigned)>(gameImage+0x1a31f0)(nativePowerFxManager(),link.effect.id);++mesmerizeLinkStopped[link.kind-1];}
 link={};return true;
}
bool serviceMesmerizeLinks(ULONGLONG tick,bool automatic){
 auto& p=mesmerizeVisual;const auto& packet=mesmerizeWorldStream.presented;
 if(!automatic){for(auto& link:p.links)if(!stopMesmerizeLink(link))return false;return true;}
 const bool snapshot=packet.linksValid&&packet.sequence!=p.linkSequence;
 for(auto& link:p.links)if(link.sourceId){
  bool retained=!snapshot;
  for(unsigned i=0;snapshot&&i<packet.linkCount;++i){const auto& incoming=packet.links[i];if(incoming.id==link.sourceId&&incoming.target==link.target&&incoming.kind==link.kind)retained=true;}
  if(!retained||tick>=link.until)if(!stopMesmerizeLink(link))return false;
 }
 if(!snapshot)return true;p.linkSequence=packet.sequence;
 for(unsigned i=0;i<packet.linkCount;++i){const auto& source=packet.links[i];MesmerizeVisualLink* slot=nullptr;
  for(auto& link:p.links)if(link.sourceId==source.id&&link.target==source.target&&link.kind==source.kind){slot=&link;break;}
  if(!slot){for(auto& link:p.links)if(!link.sourceId){slot=&link;break;}if(!slot)return false;
   slot->sourceId=source.id;slot->target=source.target;slot->kind=source.kind;
   auto& effect=slot->effect;effect.game=p.game;effect.epoch=p.epoch;effect.actor=p.target.actor;effect.owner=p.target.handle;effect.kind=14+source.kind;
   effect.declaration=nativePowerFxDefinition(effect.kind);if(!effect.declaration)return false;
   using Start=unsigned*(*)(std::uintptr_t,unsigned*,std::uintptr_t,std::uintptr_t,std::uintptr_t,std::uintptr_t,const float*,const float*,std::uintptr_t,unsigned,std::uintptr_t,std::uintptr_t);
   reinterpret_cast<Start>(gameImage+0x87cb90)(effect.actor,&effect.id,0,0,0,effect.declaration,source.transform.data(),source.transform.data()+3,0,0,0,0);
   if(effect.id==~0u)return false;effect.expires=tick+300;effect.record=nativePowerFxRecord(effect);++mesmerizeLinkStarted[source.kind-1];
   if(!effect.record)return false;
  }
  const auto record=nativePowerFxRecord(slot->effect);if(!record)return false;
  // These fields match native beam/head updates, but this record belongs only
  // to our animation-only presentation object. No NPC component is written.
  std::memcpy(reinterpret_cast<void*>(record+0x38),source.transform.data(),48);
  if(source.kind<3)std::memcpy(reinterpret_cast<void*>(record+0x80),source.other.data(),48);
  slot->until=tick+300;slot->effect.expires=slot->until;++mesmerizeLinkUpdated[source.kind-1];
 }
 return true;
}
void writeMesmerizeLinks(){
 char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.mesmerize-links.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
 FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;const auto& p=mesmerizeVisual;
 fprintf(f,"{\"started\":[%llu,%llu,%llu],\"stopped\":[%llu,%llu,%llu],\"updated\":[%llu,%llu,%llu],\"refused\":%llu,\"sequence\":%llu,\"links\":[",mesmerizeLinkStarted[0],mesmerizeLinkStarted[1],mesmerizeLinkStarted[2],mesmerizeLinkStopped[0],mesmerizeLinkStopped[1],mesmerizeLinkStopped[2],mesmerizeLinkUpdated[0],mesmerizeLinkUpdated[1],mesmerizeLinkUpdated[2],mesmerizeLinkRefused,p.linkSequence);
 unsigned n=0;for(const auto& link:p.links)if(link.sourceId)fprintf(f,"%s{\"sourceId\":%u,\"target\":%u,\"kind\":%u,\"id\":%u,\"owner\":%u,\"active\":%s}",n++?",":"",link.sourceId,link.target,link.kind,link.effect.id,link.effect.owner,link.until>GetTickCount64()?"true":"false");
 fputs("]}\n",f);const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
