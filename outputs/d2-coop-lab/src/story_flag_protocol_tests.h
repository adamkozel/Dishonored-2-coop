#pragma once
#include "story_flag_protocol.h"
void storyFlagProtocolTests(){
    StoryFlagPacket p{};p.sequence=1;p.count=2;p.entries[0]={storyFlagHash("storyflags/dunwall.kiscule"),storyFlagHash("ramsey_is_alive"),0};
    p.entries[1]={storyFlagHash("storyflags/dunwall.kiscule"),storyFlagHash("ramsey_in_vault"),1};
    require(validStoryFlags(p),"host story flags accepted");
    require(storyFlagHash("ramsey_is_alive")!=storyFlagHash("ramsey_in_vault"),"flag names hash apart");
    for(unsigned i=0;i<9;++i){auto q=p;switch(i){
        case 0:q.role=1;break;case 1:q.version=2;break;case 2:--q.size;break;case 3:q.magic=0;break;case 4:q.sequence=0;break;
        case 5:q.count=kStoryFlagMax+1;break;case 6:q.entries[1].value=2;break;case 7:q.entries[2].value=1;break;
        case 8:q.entries[0].decl=q.entries[0].name=0;break;}
        require(!validStoryFlags(q),"partner, malformed, non-bool or untidy story flags refused");}
    {auto q=p;q.count=0;for(auto& e:q.entries)e={};require(validStoryFlags(q),"a map with no story flags is a valid empty set");}
}
