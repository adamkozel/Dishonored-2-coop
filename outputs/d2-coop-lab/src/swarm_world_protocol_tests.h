#pragma once
void swarmPacketTests(const CoopPosePacket& h,const CoopPosePacket& c){
    CoopSwarmWorldPacket p{};p.sequence=1;p.sourceSession=h.session;p.targetSession=c.session;p.sourceWorldEpoch=h.worldEpoch;p.targetWorldEpoch=c.worldEpoch;p.mapId=h.mapId;
    p.sourceToken=4;p.targetToken=9;p.event=1;p.sampleTimeUs=1000000;p.rootSequence=h.sequence;p.nativeHandle=0xc871210;p.present=1;p.count=1;p.size=swarmWorldWireSize(1);
    for(unsigned i=0;i<3;++i)p.sourceOrigin[i]=h.position[i];
    p.agents[0]={7,0x9870321,0,1,{h.position[0]+3,h.position[1],h.position[2],1,0,0,0,1,0,0,0,1},1.1f,1};
    check(validSwarmWorldPacket(p,c,h),"indexed native Corvo rat packet accepted; zero incarnation is valid");
    {
        auto trail=p;auto& t=trail.agents[0];t.trailId=80;t.trailAgeMs=20;t.trailTransform=t.transform;t.trailTransform[2]+=.1f;
        check(validSwarmWorldPacket(trail,c,h),"strict rat trail payload fits version2 world wire");
        CoopSwarmWorldPacket copied{};check(decodeSwarmWorldPacket(&trail,trail.size,copied)&&copied.agents[0].trailId==80&&
            copied.agents[0].trailTransform==t.trailTransform&&swarmWorldDigest(copied)==swarmWorldDigest(trail),"trail identity and exact transform survive variable decode and digest");
        for(unsigned i=0;i<6;++i){auto bad=trail;switch(i){case 0:bad.agents[0].trailAgeMs=2501;break;case 1:bad.agents[0].trailId=~0u;break;
            case 2:bad.agents[0].trailTransform[0]+=3;break;case 3:bad.agents[0].trailTransform[3]=NAN;break;case 4:bad.agents[0].trailTransform[3]=-1;break;case 5:bad.agents[0].trailId=0;break;}
            check(!validSwarmWorldPacket(bad,c,h),"trail rejects expired age, invalid ID, distant/reflected/nonfinite transform and dirty absence");}
        auto duplicate=trail;duplicate.count=2;duplicate.size=swarmWorldWireSize(2);duplicate.agents[1]=t;duplicate.agents[1].index++;duplicate.agents[1].handle++;
        check(!validSwarmWorldPacket(duplicate,c,h),"one trail ID cannot attach to two rats");
        auto tooMany=trail;tooMany.count=8;tooMany.size=swarmWorldWireSize(8);for(unsigned i=0;i<8;++i){tooMany.agents[i]=t;tooMany.agents[i].index=i;tooMany.agents[i].handle+=i;tooMany.agents[i].trailId+=i;}
        check(!validSwarmWorldPacket(tooMany,c,h),"native initial trail limit7 enforced");
        auto next=trail;next.sequence++;next.sampleTimeUs+=80000;next.agents[0].transform[0]+=.8f;next.agents[0].trailTransform[0]+=.8f;next.agents[0].trailAgeMs+=80;
        CoopSwarmAgent shown{};float alpha=0;check(swarmRatPresentation(trail,1000,&next,1080,1040,t.index,shown,alpha)&&
            shown.trailId==80&&shown.trailAgeMs==60&&std::fabs(shown.trailTransform[0]-t.trailTransform[0]-.4f)<.00001f,"trail uses rat source-time interpolation");
        next.agents[0].trailId++;check(swarmRatPresentation(trail,1000,&next,1080,1040,t.index,shown,alpha)&&shown.trailTransform==t.trailTransform&&shown.trailId==80,"different source trail IDs are never blended");
        next.agents[0].trailId=0;check(swarmRatPresentation(trail,1000,&next,1080,1040,t.index,shown,alpha)&&shown.trailId==80&&shown.trailTransform==t.trailTransform,"trail retirement waits for presented source snapshot");
    }
    {
        auto next=p;next.sequence++;next.sampleTimeUs+=80000;next.agents[0].transform[0]+=.8f;next.agents[0].transform[1]+=.4f;next.agents[0].transform[2]+=.2f;
        const float turn[]={0,1,0,-1,0,0,0,0,1};std::memcpy(next.agents[0].transform.data()+3,turn,sizeof(turn));
        CoopSwarmAgent shown{};float alpha=0;
        check(swarmRatPresentation(p,10000,&next,10080,10040,7,shown,alpha)&&std::fabs(alpha-.5f)<.00001f&&
            std::fabs(shown.transform[0]-p.agents[0].transform[0]-.4f)<.00001f&&std::fabs(shown.transform[2]-p.agents[0].transform[2]-.1f)<.00001f&&
            std::fabs(shown.transform[3]-.70710678f)<.0001f,"rat XYZ and shortest-arc orientation interpolate on source time");
        check(swarmRatPresentation(p,10000,&next,10080,11000,7,shown,alpha)&&alpha==1&&shown.transform[0]==next.agents[0].transform[0],"rat presentation never extrapolates after last sample");
        for(unsigned change=0;change<11;++change){auto altered=next;
            switch(change){case 0:altered.event++;break;case 1:altered.nativeHandle++;break;case 2:altered.sourceToken++;break;case 3:altered.targetWorldEpoch++;break;
                case 4:altered.agents[0].handle++;break;case 5:altered.agents[0].incarnation++;break;case 6:altered.count=0;break;case 7:altered.present=0;break;
                case 8:altered.agents[0].transform[0]+=10;break;case 9:altered.mapId++;break;case 10:altered.sourceSession++;break;}
            check(swarmRatPresentation(p,10000,&altered,10080,10040,7,shown,alpha)&&alpha==0&&shown.transform==p.agents[0].transform,"rat interpolation holds across identity, clear, map and teleport boundaries");
        }
        check(swarmRatPresentation(p,10000,&next,10300,10100,7,shown,alpha)&&alpha==0,"rat interpolation refuses long sample gaps");
        check(!swarmRatPresentation(p,10000,&next,10080,10040,8,shown,alpha),"no invented rat for absent source index");
        check(!swarmRatPresentation(p,10000,&next,10080,NAN,7,shown,alpha),"invalid presentation clock refused");
    }
    CoopSwarmWorldPacket decoded{};
    check(decodeSwarmWorldPacket(&p,p.size,decoded)&&decoded.agents[0].handle==p.agents[0].handle&&decoded.agents[1].handle==0,"variable wire preserves used agent and zeroes absent tail");
    check(swarmWorldDigest(p)==swarmWorldDigest(decoded),"sent and decoded wire digests match");
    decoded.agents[0].transform[0]+=.001f;check(swarmWorldDigest(p)!=swarmWorldDigest(decoded),"wire digest detects changed rat transform");
    check(!decodeSwarmWorldPacket(&p,p.size-1,decoded)&&!decodeSwarmWorldPacket(&p,p.size+1,decoded)&&!decodeSwarmWorldPacket(&p,123,decoded)&&!decodeSwarmWorldPacket(&p,sizeof(p)+1,decoded),"truncated, trailing, undersized and oversized rat frames refused");
    auto b=p;b.count=101;check(!decodeSwarmWorldPacket(&b,sizeof(b),decoded)&&!validSwarmWorldPacket(b,c,h),"unbounded rat count refused before access");
    b=p;b.role=1;check(!validSwarmWorldPacket(b,c,h),"Emily cannot author Corvo Swarm channel");
    b=p;b.version++;check(!validSwarmWorldPacket(b,c,h),"unknown Swarm wire refused");
    b=p;b.targetWorldEpoch++;check(!validSwarmWorldPacket(b,c,h),"old checkpoint rat snapshot refused");
    b=p;b.sourceSession++;check(!validSwarmWorldPacket(b,c,h),"foreign source session refused");
    b=p;b.targetSession++;check(!validSwarmWorldPacket(b,c,h),"foreign target session refused");
    b=p;b.sourceToken=0;check(!validSwarmWorldPacket(b,c,h),"unarmed source token refused");
    b=p;b.sourceOrigin[0]+=20;check(!validSwarmWorldPacket(b,c,h),"rat source stays in current root neighbourhood");
    b=p;b.agents[0].transform[0]+=101;check(!validSwarmWorldPacket(b,c,h),"rat remains bounded around source");
    b=p;b.agents[0].transform[3]=-1;check(!validSwarmWorldPacket(b,c,h),"reflected rat matrix refused");
    b=p;b.agents[0].scale=NAN;check(!validSwarmWorldPacket(b,c,h),"nonfinite rat scale refused");
    b=p;b.agents[0].index=100;check(!validSwarmWorldPacket(b,c,h),"index outside native pool refused");
    b=p;b.agents[0].handle=p.nativeHandle;check(!validSwarmWorldPacket(b,c,h),"world cannot impersonate own agent");
    b=p;b.agents[0].handle=1;check(!validSwarmWorldPacket(b,c,h),"player handle cannot impersonate rat");
    b=p;b.agents[0].directDriver=0;check(!validSwarmWorldPacket(b,c,h),"detached driver only valid in confirmed contextual state");
    b.agents[0].state=3;check(validSwarmWorldPacket(b,c,h),"native contextual attachment is transmittable");
    b=p;b.agents[0].state=7;check(!validSwarmWorldPacket(b,c,h),"unknown rat state refused");
    b=p;b.count=2;b.size=swarmWorldWireSize(2);b.agents[1]=b.agents[0];b.agents[1].index++;
    check(!validSwarmWorldPacket(b,c,h),"duplicate entity across indices refused");
    b.agents[1].handle++;b.agents[1].index=b.agents[0].index;check(!validSwarmWorldPacket(b,c,h),"duplicate pool index refused");
    b.agents[1].index++;check(validSwarmWorldPacket(b,c,h),"distinct live pool agents accepted");
    b=p;b.count=100;b.size=swarmWorldWireSize(100);for(unsigned i=0;i<100;++i){b.agents[i]=p.agents[0];b.agents[i].index=i;b.agents[i].handle+=i;}
    check(validSwarmWorldPacket(b,c,h)&&decodeSwarmWorldPacket(&b,b.size,decoded),"full native pool capacity fits bounded wire");
    b=p;b.count=0;b.size=124;check(validSwarmWorldPacket(b,c,h),"live native world may have zero surviving indexed agents");
    {auto big=b;big.present=2;check(validSwarmWorldPacket(big,c,h),"1214ja: a Bigger Swarm world (present 2) accepted");
     big.present=3;check(!validSwarmWorldPacket(big,c,h),"1214ja: present beyond 2 refused");}
    b.present=0;check(!validSwarmWorldPacket(b,c,h),"absence cannot retain a world handle");
    b.nativeHandle=0;check(validSwarmWorldPacket(b,c,h)&&decodeSwarmWorldPacket(&b,124,decoded),"confirmed world absence uses header-only clear");
    SwarmWorldOrder order{};check(order.accept(p)&&!order.accept(p),"duplicate rat snapshot refused");
    auto n=p;n.sequence++;n.sampleTimeUs++;check(order.accept(n),"new sample for same world accepted");
    auto stale=n;stale.sequence++;check(!order.accept(stale),"same-time rat snapshot refused");
    stale=n;stale.sequence++;stale.sampleTimeUs++;stale.nativeHandle++;check(!order.accept(stale),"world incarnation change needs event");
    stale=n;stale.sequence++;stale.sampleTimeUs++;stale.event++;check(!order.accept(stale),"same incarnation cannot claim different event");
    b.sequence=n.sequence+1;b.sampleTimeUs=n.sampleTimeUs+1;b.event=n.event+1;check(order.accept(b)&&!order.accept(p),"clear retires old swarm and prevents resurrection");
    std::cout<<"PASS: Swarm variable wire, world ownership, bounded agents, contextual state and replay retirement\n";
}
