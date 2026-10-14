#pragma once
#include "movement_semantics.h"
// Presentation only. A replicated native remaining-count transition and apex
// increment identify the impulse; height or button state alone cannot do so.
struct MovementSecondJump {
    bool primed=false,active=false;unsigned serial=0;
    double lastAt=0,lastSample=0;bool timed=false;
    float start=0,firstTarget=0,segmentStart=0,target=0,progress=0,transition=0;
    std::uint64_t event=0;
    void clear(){primed=active=timed=false;lastAt=lastSample=0;progress=transition=0;event=0;}
    bool advance(double at,const MovementSemantics::Sample& s,float z,unsigned motion,bool eligible){
        if(!eligible||!std::isfinite(at)||!std::isfinite(z)||!std::isfinite(s.time)||
            !std::isfinite(s.startZ)||!std::isfinite(s.targetZ)||s.time>at||at-s.time>200||
            (motion!=2&&motion!=3)){clear();return false;}
        if(timed&&(at<lastAt||at-lastAt>250||s.time<lastSample||s.event<event)){clear();return false;}
        const float dt=timed?float((at-lastAt)*.001):0;lastAt=at;timed=true;
        bool fired=false;
        if(s.time!=lastSample){
            if(s.state==3&&s.airStage==1&&s.remaining==1&&s.targetZ-s.startZ>=.05f&&s.targetZ-s.startZ<=10.f){
                if(active&&std::fabs(s.startZ-start)>.01f){clear();lastAt=at;timed=true;}
                if(!active){primed=true;start=s.startZ;firstTarget=s.targetZ;}
            }else if(!active&&primed&&s.state==3&&s.airStage==1&&s.remaining==0&&
                std::fabs(s.startZ-start)<=.01f&&std::fabs(s.targetZ-firstTarget-2.5f)<=.02f&&
                s.targetZ-z>.05f&&s.targetZ-z<=10.f){
                active=true;primed=false;segmentStart=z;target=s.targetZ;progress=transition=0;++serial;fired=true;
            }else if(!active)primed=false;
            lastSample=s.time;event=s.event;
        }
        if(active){
            const float amount=motion==3?1.f:(z-segmentStart)/(target-segmentStart);
            progress=(std::max)(progress,(std::max)(0.f,(std::min)(1.f,amount)));
            if(!fired)transition=(std::min)(1.f,transition+dt/.09f);
        }
        return fired;
    }
};
