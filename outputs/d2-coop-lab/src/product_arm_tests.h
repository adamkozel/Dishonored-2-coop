#pragma once
#include <cstring>
#include "product_arm.h"
void productArmTests(){
    ProductArm a{};a.step=22;a.dispatched=313;a.repairs=2;a.quickRepairs=1;a.repairBackoff=12000;a.hostKeys=1;a.leaveKeys=1;a.result="armed";a.epoch=4;a.peerEpoch=5;a.map=9;
    productNewWorld(a,6,7,9,1000);
    require(a.worlds==1&&a.step==22&&a.dispatched==313&&a.repairs==2&&a.quickRepairs==1&&a.hostKeys==1&&a.leaveKeys==1,
            "1214ga (#145): a new world keeps the once-per-process arming and the counters, and counts the world");
    require(a.repairBackoff==0&&a.epoch==6&&a.peerEpoch==7&&a.map==9&&a.nextAt==4000&&!std::strcmp(a.result,"world_settling"),
            "1214ga (#145): a new world restarts the repair backoff and settles for 3 s");
    productNewWorld(a,8,7,10,5000);require(a.worlds==2&&a.map==10,"1214ga (#145): every world counts");
    ProductArm f{};productNewWorld(f,1,1,3,0);require(f.worlds==1&&f.step==0,"1214ga (#145): after F8 or coop_product_auto the first world arms from step 0");
}
