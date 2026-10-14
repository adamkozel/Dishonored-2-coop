#pragma once
#include <algorithm>
#include "native_windblast_fx_records.h"
// Correlate a newly observed native first-person world flash with an owned
// gun firing scene. Baseline existing effects on every new stream, and never
// infer a shot from a trigger press, an empty click or a body animation alone.
struct RangedShotObservation {
    std::uint64_t token=0,peerToken=0,shotScene=0;unsigned highWater=0,nativeId=0;bool baseline=false;
    unsigned observe(std::uint64_t own,std::uint64_t peer,std::uint64_t scene,unsigned role,bool fire,const float* root,const std::vector<NativeWindblastFxRecord>& records){
        if(!own||!peer||role>1){*this={};return 0;}
        if(token!=own||peerToken!=peer){*this={};token=own;peerToken=peer;}
        const char* expected=role?"models/effects/weapons/gun/muzzle_1p_emily.fx":"models/effects/weapons/gun/muzzle_1p_corvo.fx";
        unsigned highest=highWater,matches=0,id=0;
        for(const auto& r:records){highest=(std::max)(highest,r.id);
            float distance=0;for(unsigned k=0;k<3;++k){const float d=r.origin[k]-root[k];distance+=d*d;}
            if(baseline&&fire&&scene&&scene!=shotScene&&r.id>highWater&&r.ownerActor&&r.name==expected&&std::isfinite(distance)&&distance<=16){++matches;id=r.id;}
        }
        highWater=highest;baseline=true;
        if(matches==1){shotScene=scene;nativeId=id;}
        return fire&&scene&&scene==shotScene?1u:0u;
    }
};
