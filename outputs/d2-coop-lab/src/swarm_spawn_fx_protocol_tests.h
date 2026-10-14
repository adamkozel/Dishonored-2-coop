#pragma once
void swarmSpawnFxTests(const CoopPosePacket& h,const CoopPosePacket& c){
    CoopSwarmSpawnFxPacket p{};p.sequence=1;p.event=1;p.nativeFxId=50;p.powerEvent=10;p.worldEvent=3;p.worldHandle=0x871003;
    p.sourceSession=h.session;p.targetSession=c.session;p.sourceWorldEpoch=h.worldEpoch;p.targetWorldEpoch=c.worldEpoch;p.mapId=h.mapId;
    p.sourceToken=4;p.targetToken=9;p.sampleTimeUs=1000000;p.rootSequence=h.sequence;p.castTimeMs=300;
    for(unsigned k=0;k<3;++k)p.position[k]=h.position[k];p.axis={1,0,0,0,1,0,0,0,1};
    check(validSwarmSpawnFxPacket(p,c,h),"scoped Swarm cosmetic summon packet accepted");
    for(unsigned i=0;i<17;++i){auto b=p;switch(i){
        case 0:b.sourceSession++;break;case 1:b.targetSession++;break;case 2:b.sourceWorldEpoch++;break;case 3:b.targetWorldEpoch++;break;
        case 4:b.mapId++;break;case 5:b.role=1;break;case 6:b.worldHandle=1;break;case 7:b.worldEvent=0;break;case 8:b.powerEvent=0;break;
        case 9:b.castTimeMs=1001;break;case 10:b.position[0]+=31;break;case 11:b.axis[0]=-1;break;case 12:b.axis[0]=NAN;break;
        case 13:b.nativeFxId=~0u;break;case 14:b.size--;break;case 15:b.sourceToken=0;break;case 16:b.version++;break;}
        check(!validSwarmSpawnFxPacket(b,c,h),"Swarm FX rejects wrong world, role, invalid geometry, stale cast and malformed wire");
    }
    SwarmSpawnFxOrder order{};check(order.accept(p)&&!order.accept(p),"Swarm burst cannot replay duplicate packet");
    auto next=p;next.sequence++;next.event++;next.sampleTimeUs++;next.nativeFxId++;next.powerEvent++;next.worldEvent++;
    for(unsigned i=0;i<3;++i){auto reused=next;if(i==0)reused.nativeFxId=p.nativeFxId;if(i==1)reused.powerEvent=p.powerEvent;if(i==2)reused.worldEvent=p.worldEvent;
        check(!order.accept(reused),"new packet cannot replay old effect, cast or source world");}
    check(order.accept(next),"independent later cast accepted after duplicate refusals");
    SwarmSpawnDiscovery d{};d.observe(100,1000);check(!d.mayRead(1000,true),"late join baselines persistent native record");
    d.observe(0,1100);check(!d.mayRead(1100,true),"no world cannot authorize burst");
    d.observe(200,1200);check(!d.mayRead(1200,false)&&d.mayRead(1200,true),"new native world requires corroborated cast");
    check(!d.mayRead(1199,true)&&!d.mayRead(1951,true),"discovery refuses backwards time and expired window");
    d.attempts=8;check(!d.mayRead(1300,true),"bounded full-map discovery attempts");
    d.attempts=0;d.consumed=true;d.observe(200,1300);check(!d.mayRead(1300,true),"failed native start or send cannot retry same record");
    d.observe(300,1400);check(d.mayRead(1400,true),"new world incarnation reopens bounded discovery");
    std::cout<<"PASS: Swarm cosmetic FX ownership protocol, replay guards and bounded birth discovery\n";
}
