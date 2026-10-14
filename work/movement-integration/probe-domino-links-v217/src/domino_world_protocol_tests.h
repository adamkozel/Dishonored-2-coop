#pragma once
void dominoPacketTests(const CoopPosePacket& h,const CoopPosePacket& c){
    CoopDominoWorldPacket p{};p.sequence=1;p.sourceSession=c.session;p.targetSession=h.session;p.sourceWorldEpoch=c.worldEpoch;p.targetWorldEpoch=h.worldEpoch;p.mapId=h.mapId;
    p.sourceToken=9;p.targetToken=4;p.event=1;p.sampleTimeUs=1000000;p.rootSequence=c.sequence;p.nativeHandle=0xc871210;p.present=1;p.count=1;
    for(unsigned i=0;i<3;++i)p.sourceOrigin[i]=c.position[i];p.marks[0]={415,0xb480764,{c.position[0]+3,c.position[1],c.position[2]+1,1,0,0,0,1,0,0,0,1}};
    check(validDominoWorldPacket(p,h,c),"Domino bounded native target mark accepted in exact Emily-to-Corvo world pair");
    auto b=p;b.role=0;check(!validDominoWorldPacket(b,h,c),"Corvo cannot author Domino marks");
    b=p;b.version=0;check(!validDominoWorldPacket(b,h,c),"unknown Domino wire refused");
    b=p;b.targetWorldEpoch++;check(!validDominoWorldPacket(b,h,c),"old Domino checkpoint refused");
    b=p;b.sourceSession++;check(!validDominoWorldPacket(b,h,c),"foreign source session refused");
    b=p;b.sourceOrigin[0]+=20;check(!validDominoWorldPacket(b,h,c),"world mark source must match current root neighbourhood");
    b=p;b.count=5;check(!validDominoWorldPacket(b,h,c),"more than four Domino marks refused");
    b=p;b.count=2;b.marks[1]=b.marks[0];check(!validDominoWorldPacket(b,h,c),"duplicate target/effect rejected");
    b=p;b.marks[0].target=1;check(!validDominoWorldPacket(b,h,c),"local player handle is not a target identity");
    b=p;b.marks[0].target=p.nativeHandle;check(!validDominoWorldPacket(b,h,c),"network cannot target itself");
    b=p;b.marks[0].id=~0u;check(!validDominoWorldPacket(b,h,c),"retired native mark ID refused");
    b=p;b.marks[0].transform[0]=NAN;check(!validDominoWorldPacket(b,h,c),"nonfinite mark refused");
    b=p;b.marks[0].transform[3]=-1;check(!validDominoWorldPacket(b,h,c),"reflected native mark basis refused");
    b=p;b.marks[0].transform[0]+=101;check(!validDominoWorldPacket(b,h,c),"remote mark cannot leave bounded native neighbourhood");
    b=p;b.marks[3].target=3;check(!validDominoWorldPacket(b,h,c),"unused marks must be canonical zero");
    b=p;b.present=0;check(!validDominoWorldPacket(b,h,c),"absent native network cannot contain active marks");
    b=p;b.count=0;b.marks={};check(validDominoWorldPacket(b,h,c),"selected network can have confirmed empty marks");
    b.present=0;b.nativeHandle=0;check(validDominoWorldPacket(b,h,c),"confirmed network absence can retire cosmetic effects");
    DominoWorldOrder order{};check(order.accept(p)&&!order.accept(p),"duplicate snapshot rejected");
    b=p;b.sequence++;b.sampleTimeUs++;check(order.accept(b),"same network fresh snapshot accepted");
    auto stale=b;stale.sequence++;check(!order.accept(stale),"same-time replay rejected");
    stale=b;stale.sequence++;stale.sampleTimeUs++;stale.nativeHandle++;check(!order.accept(stale),"network incarnation change needs new semantic event");
    stale=b;stale.sequence++;stale.sampleTimeUs++;stale.event++;check(!order.accept(stale),"same incarnation cannot claim a new event");
    b.sequence++;b.sampleTimeUs++;b.event++;b.nativeHandle=0;b.present=0;b.count=0;b.marks={};check(order.accept(b),"ordered clear retires old native network");
    check(!order.accept(p),"old active mark cannot resurrect after clear");
}
