#pragma once
#include <algorithm>
#include <cmath>
#include <cstdio>
// Scalar native food outcome only. Identity/ownership is checked separately.
struct PickupFoodValue {
    float health=0,healthTarget=0,healthMaximum=0,mana=0,manaTarget=0,manaMaximum=0,healthGrant=0,manaGrant=0;
    int stored=0,storageMaximum=0;
    bool healthSuppressed=false,manaEnabled=false;
};
inline float foodHealth(const PickupFoodValue& v){return (std::max)(v.health,v.healthTarget);}
inline float foodMana(const PickupFoodValue& v){return (std::max)(v.mana,v.manaTarget);}
inline bool validFoodValue(const PickupFoodValue& v){
    for(float n:{v.health,v.healthTarget,v.healthMaximum,v.mana,v.manaTarget,v.manaMaximum,v.healthGrant,v.manaGrant})
        if(!std::isfinite(n)||n<0||n>1000000.f)return false;
    return v.healthMaximum>0&&v.manaMaximum>0&&v.health>0&&v.health<=v.healthMaximum+.01f&&v.healthTarget<=v.healthMaximum+.01f&&
        v.mana<=v.manaMaximum+.01f&&v.manaTarget<=v.manaMaximum+.01f&&(v.healthGrant>0||v.manaGrant>0)&&
        v.storageMaximum>=1&&v.storageMaximum<=1000&&v.stored>=0&&v.stored<=v.storageMaximum;
}
inline bool sameFoodRules(const PickupFoodValue& a,const PickupFoodValue& b){
    return a.healthMaximum==b.healthMaximum&&a.manaMaximum==b.manaMaximum&&a.healthGrant==b.healthGrant&&a.manaGrant==b.manaGrant&&
        a.healthSuppressed==b.healthSuppressed&&a.manaEnabled==b.manaEnabled&&a.storageMaximum==b.storageMaximum;
}
inline bool validFoodOutcome(const PickupFoodValue& before,const PickupFoodValue& after,int quantity){
    if(!validFoodValue(before)||!validFoodValue(after)||!sameFoodRules(before,after)||quantity<1||quantity>1000)return false;
    const bool heal=before.healthGrant>0;
    const bool mana=before.manaGrant>0&&before.manaEnabled&&before.mana<before.manaMaximum;
    const float expectedHealth=heal&&!before.healthSuppressed?(std::min)(foodHealth(before)+before.healthGrant,before.healthMaximum):foodHealth(before);
    const float expectedMana=mana?(std::min)(before.manaTarget+before.manaGrant,before.manaMaximum):foodMana(before);
    const int stored=heal||mana?before.stored:(std::min)(before.stored+quantity,before.storageMaximum);
    return std::fabs(foodHealth(after)-expectedHealth)<=.01f&&std::fabs(foodMana(after)-expectedMana)<=.01f&&after.stored==stored&&
        after.health+.01f>=before.health&&after.mana+.01f>=before.mana;
}
inline void writePickupFoodValue(FILE* f,const PickupFoodValue& v){
    std::fprintf(f,"{\"health\":%.4f,\"healthTarget\":%.4f,\"healthMaximum\":%.4f,\"mana\":%.4f,\"manaTarget\":%.4f,\"manaMaximum\":%.4f,\"healthGrant\":%.4f,\"manaGrant\":%.4f,\"stored\":%d,\"storageMaximum\":%d,\"healthSuppressed\":%s,\"manaEnabled\":%s}",v.health,v.healthTarget,v.healthMaximum,v.mana,v.manaTarget,v.manaMaximum,v.healthGrant,v.manaGrant,v.stored,v.storageMaximum,v.healthSuppressed?"true":"false",v.manaEnabled?"true":"false");
}
