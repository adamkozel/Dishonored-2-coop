#pragma once
#include "use_mirror_protocol.h"
inline void useMirrorProtocolTests(){
    UseMirrorPacket p{};p.role=1;p.session=22;p.epoch=5;p.mapId=9;p.sequence=1;std::strcpy(p.name,"lift_01_button_3");
    require(validUseMirror(p,0),"a partner's button use reaches the host");
    require(!validUseMirror(p,1),"a world never applies its own use");
    for(unsigned field=0;field<8;++field){auto bad=p;switch(field){case 0:bad.magic=1;break;case 1:bad.version=2;break;case 2:bad.size=1;break;case 3:bad.role=2;break;
        case 4:bad.session=0;break;case 5:bad.epoch=0;break;case 6:bad.sequence=0;break;case 7:bad.name[0]=0;break;}require(!validUseMirror(bad,0),"malformed use mirror rejected");}
    auto bad=p;std::memset(bad.name,'a',sizeof(bad.name));require(!validUseMirror(bad,0),"unterminated name rejected");
    for(const char* n:{"ld_safe_main_01_door","pr_a_ld_safe_main_01_digit_a","script_combinaison_numbers_2","Lift_Button"}){bad=p;std::strcpy(bad.name,n);require(!validUseMirror(bad,0),"safes, dials and noncanonical names stay per world");}
    NativeUsable u{};u.name=u.definitionName="lift_01_button_3";u.declarationName="map_cpntusablewithstatelist_decl_9";u.enabled=1;u.stateCount=1;u.initial=0;
    require(useMirrorEligible(u),"one-state button is mirrored");
    u.stateCount=2;u.entries[0]={255,0,0,0,0};u.entries[1]={255,1,0,0,0};require(!useMirrorEligible(u),"a shared toggle keeps its state sync");
    u.entries[0].transitionSeconds=1.5f;u.entries[1].transitionSeconds=1.5f;require(useMirrorEligible(u),"a two-state lever outside the toggle family is mirrored");
    u.stateCount=10;require(!useMirrorEligible(u),"multi-state devices are not mirrored");
    u.stateCount=1;u.enabled=0;require(!useMirrorEligible(u),"a disabled usable is not mirrored");
    std::cout<<"PASS: use mirror packets, eligible usables and per-world safes\n";
}
