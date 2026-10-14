#pragma once
void possessionFxTests(const CoopPosePacket& h,const CoopPosePacket& c){
    CoopPossessionFxPacket p{};p.sequence=1;p.event=1;p.target=0x80005;p.id=30;p.present=1;
    p.sourceSession=h.session;p.targetSession=c.session;p.sourceWorldEpoch=h.worldEpoch;p.targetWorldEpoch=c.worldEpoch;p.mapId=h.mapId;
    p.sourceToken=4;p.targetToken=9;p.sampleTimeUs=1000000;p.rootSequence=h.sequence;
    for(unsigned i=0;i<3;++i)p.transform[i]=p.sourceOrigin[i]=h.position[i];p.transform[2]+=1.7f;p.transform[3]=p.transform[7]=p.transform[11]=1;
    check(validPossessionFxPacket(p,c,h),"Possession target particles have scoped Corvo source and rigid transform");
    for(unsigned i=0;i<16;++i){auto b=p;switch(i){case 0:b.sourceSession++;break;case 1:b.targetSession++;break;case 2:b.sourceWorldEpoch++;break;case 3:b.targetWorldEpoch++;break;case 4:b.mapId++;break;case 5:b.role=1;break;case 6:b.target=1;break;case 7:b.id=~0u;break;case 8:b.event=0;break;case 9:b.transform[0]+=21;break;case 10:b.transform[3]=-1;break;case 11:b.transform[0]=NAN;break;case 12:b.present=2;break;case 13:b.size--;break;case 14:b.sourceToken=0;break;case 15:b.sourceOrigin[0]+=11;break;}check(!validPossessionFxPacket(b,c,h),"Possession rejects malformed target/world/time/geometry");}
    auto clear=p;clear.present=0;clear.target=clear.id=0;clear.transform={};check(validPossessionFxPacket(clear,c,h),"explicit empty snapshot is canonical");clear.transform[3]=1;check(!validPossessionFxPacket(clear,c,h),"empty snapshot cannot retain geometry");
    PossessionFxOrder order{};check(order.accept(p)&&!order.accept(p),"duplicate Possession FX snapshot refused");
    auto b=p;b.sequence++;b.sampleTimeUs++;check(order.accept(b),"same effect advances normally");
    b.sequence++;b.sampleTimeUs++;b.id++;check(!order.accept(b),"changed native effect requires a new incarnation event");
    b.event++;check(order.accept(b),"new effect incarnation accepted");
    b.sequence++;b.sampleTimeUs++;b.event++;check(!order.accept(b),"new event cannot replay same native effect key");
    b.target=b.id=b.present=0;b.transform={};check(order.accept(b),"explicit retirement advances event");
    std::cout<<"PASS: Possession cosmetic FX protocol, world identity, canonical absence and replay guards\n";
}
