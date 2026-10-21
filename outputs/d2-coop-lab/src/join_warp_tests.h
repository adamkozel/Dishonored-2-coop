#pragma once
#include "join_warp.h"
void joinWarpTests(){
    const float host[3]{-48.f,-125.8f,61.1f},facingSouth[9]{0,-1,0,1,0,0,0,0,1};
    const float farSelf[3]{-33.f,-148.f,66.97f},nearSelf[3]{-46.f,-124.f,61.1f};
    const auto t=joinWarpTarget(farSelf,host,facingSouth);
    require(t.warp&&t.distance>27&&t.distance<28,"1214fr: a partner 27 m away is placed beside the host");
    require(std::fabs(t.x+48.f)<1e-4f&&std::fabs(t.y+124.6f)<1e-4f&&std::fabs(t.z-62.8f)<1e-4f&&std::fabs(t.yaw+90.f)<1e-3f,
            "1214fr: 1.2 m behind the host, at eye height, facing the host's way");
    require(!joinWarpTarget(nearSelf,host,facingSouth).warp,"1214fr: a partner already near the host stays");
    const float bad[3]{NAN,0,0};require(!joinWarpTarget(bad,host,facingSouth).warp&&!joinWarpTarget(farSelf,bad,facingSouth).warp,"1214fr: unread positions never warp");
    const float flat[9]{0,0,1,0,0,0,0,0,0};const auto u=joinWarpTarget(farSelf,host,flat);
    require(u.warp&&std::fabs(u.x+49.2f)<1e-4f&&std::fabs(u.yaw)<1e-3f,"1214fr: a degenerate host facing falls back to +x");
    require(joinWarpDue(7,7,0,0,5)&&!joinWarpDue(7,7,7,5,5),"1214fr: due once after a followed load on a connection");
    require(joinWarpDue(7,7,7,5,6),"1214ga (#135): a rejoin (a new connection) makes the placement due again");
    require(!joinWarpDue(6,7,0,0,5)&&!joinWarpDue(0,0,0,0,5)&&!joinWarpDue(7,7,0,0,0),"1214ga: never on a map not followed, without a map or without a connection");
}
