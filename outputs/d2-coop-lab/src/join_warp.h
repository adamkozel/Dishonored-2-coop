#pragma once
#include <cmath>
// 1214fr (#135): a partner whose game followed the host into a mission (join-follow loads it from its start) is
// placed beside the host once both worlds have been in gameplay on that map for a few seconds: route take
// route-dunwall-join-v1666b had the partner 27.5 m from the host after a join, behind doors the host had already passed.
// The placement is the game's own setviewpos (eye height 1.7 m above the feet), 1.2 m behind the host, facing the host's
// way; it happens once per followed load, and not when the partner is already near.
constexpr float kJoinWarpMinDistance=25.f,kJoinWarpBehind=1.2f,kJoinWarpEye=1.7f;
constexpr unsigned long long kJoinWarpSettleMs=4000;
struct JoinWarpTarget {bool warp=false;float distance=0,x=0,y=0,z=0,yaw=0;};
inline JoinWarpTarget joinWarpTarget(const float (&self)[3],const float (&host)[3],const float (&hostOrientation)[9]){
    JoinWarpTarget t{};
    for(float v:self)if(!std::isfinite(v))return t;
    for(float v:host)if(!std::isfinite(v))return t;
    const float dx=host[0]-self[0],dy=host[1]-self[1],dz=host[2]-self[2];t.distance=std::sqrt(dx*dx+dy*dy+dz*dz);
    if(!(t.distance>=kJoinWarpMinDistance))return t;
    float fx=hostOrientation[0],fy=hostOrientation[1];const float n=std::sqrt(fx*fx+fy*fy);
    if(std::isfinite(n)&&n>0.01f){fx/=n;fy/=n;}else{fx=1;fy=0;}
    t.warp=true;t.x=host[0]-kJoinWarpBehind*fx;t.y=host[1]-kJoinWarpBehind*fy;t.z=host[2]+kJoinWarpEye;
    t.yaw=std::atan2(fy,fx)*57.29577951f;return t;
}
// 1214ga (#135): the placement is due once per followed map and connection. A partner who leaves (F8) and joins the
// same lobby again while still in the mission stayed where her world had left her (two-account take
// campaign-join-vm-v1703a: 69 m from the host after the rejoin), so a new connection makes it due again; it still
// warps only from 25 m away.
inline bool joinWarpDue(unsigned long long followedMap,unsigned long long map,unsigned long long warpedMap,unsigned warpedConnection,unsigned connection){
    return map&&followedMap==map&&connection&&(warpedMap!=map||warpedConnection!=connection);
}
