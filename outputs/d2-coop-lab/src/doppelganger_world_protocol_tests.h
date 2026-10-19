#pragma once
void doppelgangerPacketTests(const CoopPosePacket& h,const CoopPosePacket& c){
    NativeSkeleton rig{};std::strcpy(rig.modelName,kDoppelgangerModel);std::strcpy(rig.skeletonName,kDoppelgangerSkeleton);
    rig.parents.assign(219,-1);rig.bindLocal.assign(219,{0,0,0,1,0,0,0,0,1,1,1,0});
    const auto reference=doppelgangerRig(rig);rig.bindLocal[71][0]=-0.f;rig.bindLocal[150][4]=-0.f;
    check(reference&&reference==doppelgangerRig(rig)&&reference!=doppelgangerRig(rig,false),"equivalent signed zero in native/exported bind data has one fingerprint");
    rig.bindLocal[150][4]=.001f;check(doppelgangerRig(rig)!=reference,"nonzero bind translation change still changes clone rig fingerprint");
    rig.bindLocal[150][4]=0;rig.parents[1]=0;check(doppelgangerRig(rig)!=reference,"clone rig fingerprint still distinguishes hierarchy changes");
    CoopDoppelgangerWorldPacket p{};p.sequence=1;p.sourceSession=c.session;p.targetSession=h.session;p.sourceWorldEpoch=c.worldEpoch;p.targetWorldEpoch=h.worldEpoch;p.mapId=c.mapId;
    p.sourceToken=4;p.targetToken=9;p.event=1;p.sampleTimeUs=1000000;p.rootSequence=c.sequence;p.nativeHandle=0xc871210;p.equipmentHandle=0xa1234;p.present=1;p.generation=8826;p.rig=kDoppelgangerRig;for(auto& j:p.local){j={1,0,0,0,1,0,0,0,1,0,0,0};}
    for(unsigned i=0;i<3;++i)p.sourceOrigin[i]=c.position[i];p.transform={c.position[0]+3,c.position[1],c.position[2],1,0,0,0,1,0,0,0,1};
    check(validDoppelgangerWorldPacket(p,h,c),"exact Emily clone identity packet accepted");
    {auto second=p;second.slot=1;check(validDoppelgangerWorldPacket(second,h,c)&&!sameDoppelgangerIncarnation(p,second),"Twin Shade's second clone slot accepted as its own clone");
     second.slot=2;check(!validDoppelgangerWorldPacket(second,h,c),"clone slot beyond Twin Shade's two refused");}
    // Native480: exact matrices captured at the sender's validation failure,
    // rather than an independent later read after the marker has recovered.
    const std::array<float,12> leftMarker={.00661819614f,-.724930584f,.688790083f,.730769217f,-.466677248f,-.498185158f,.682592392f,.506643653f,.526668549f,-2.01066375f,-4.17935562f,2.20471048f};
    const std::array<float,12> rightMarker={.268502593f,.856058598f,.441667378f,-.683938384f,-.153454363f,.713218093f,.678332269f,-.493574172f,.544288456f,.0502535626f,-3.24469805f,-1.50890505f};
    auto markers=p;markers.local[10]=leftMarker;markers.local[17]=rightMarker;markers.local[1][9]=3.68536544f;
    check(validDoppelgangerWorldPacket(markers,h,c),"native attachment markers beyond3m do not drop a valid whole clone pose");
    for(unsigned j=0;j<kDoppelgangerJoints;++j){auto ordinary=p;ordinary.local[j]=leftMarker;
        check(validDoppelgangerWorldPacket(ordinary,h,c)==(j==1||j==10||j==17),"larger marker translation is restricted to the three exact rig leaves");}
    for(unsigned j:{1u,10u,17u})for(unsigned failure=0;failure<5;++failure){auto bad=p;
        switch(failure){case 0:bad.local[j][9]=10.001f;break;case 1:bad.local[j][10]=-10.001f;break;
            case 2:bad.local[j][11]=NAN;break;case 3:bad.local[j][0]=-1;break;case 4:bad.local[j][0]=1.1f;break;}
        check(!validDoppelgangerWorldPacket(bad,h,c),"marker allowance retains finite, bounded, rigid, proper-rotation checks");}
    auto oldWire=markers;oldWire.version=3;check(!validDoppelgangerWorldPacket(oldWire,h,c),"old wire contract cannot silently claim support for extended native markers");
    std::array<float,12> markerBlend{};presentationRotation(leftMarker.data(),rightMarker.data(),.5f,markerBlend.data());
    for(unsigned i=9;i<12;++i)markerBlend[i]=(leftMarker[i]+rightMarker[i])*.5f;
    check(doppelgangerJointValid(10,doppelgangerLocalMatrix(markerBlend)),"interpolated marker uses the same bounded contract as sender and receiver");
    auto effect=p;effect.fxKnown=1;effect.fxScene=77;effect.fxVariant=1;effect.fxTimeMs=350;
    check(validDoppelgangerWorldPacket(effect,h,c),"native clone summon scene clock travels with its body pose");
    effect.fxVariant=2;effect.fxTimeMs=466;check(validDoppelgangerWorldPacket(effect,h,c),"exact disappearance scene duration accepted");
    for(unsigned i=0;i<5;++i){auto b=effect;switch(i){case 0:b.fxKnown=0;break;case 1:b.fxScene=0;break;case 2:b.fxVariant=3;break;case 3:b.fxTimeMs=467;break;case 4:b.fxKnown=2;break;}
        check(!validDoppelgangerWorldPacket(b,h,c),"untrusted or out-of-bounds clone effect clock refused");}
    effect.fxScene=effect.fxVariant=effect.fxTimeMs=0;check(validDoppelgangerWorldPacket(effect,h,c),"confirmed absence of native effect scene accepted");
    for(unsigned i=0;i<14;++i){auto b=p;switch(i){case 0:b.role=0;break;case 1:b.sourceSession++;break;case 2:b.targetWorldEpoch++;break;case 3:b.mapId++;break;
        case 4:b.nativeHandle=1;break;case 5:b.equipmentHandle=b.nativeHandle;break;case 6:b.transform[3]=-1;break;case 7:b.transform[0]=NAN;break;
        case 8:b.transform[0]+=101;break;case 9:b.sourceOrigin[0]+=20;break;case 10:b.version++;break;case 11:b.size--;break;case 12:b.sourceToken=0;break;case 13:b.present=2;break;}
        check(!validDoppelgangerWorldPacket(b,h,c),"clone wire rejects foreign world, role, identity, transform and malformed payload");}
    auto empty=p;empty.present=0;empty.nativeHandle=0;empty.equipmentHandle=0;empty.generation=0;empty.transform={};empty.rig=0;empty.local={};check(validDoppelgangerWorldPacket(empty,h,c),"clean authoritative clone absence accepted");
    auto dirty=empty;dirty.transform[3]=1;check(!validDoppelgangerWorldPacket(dirty,h,c),"absence cannot retain a transform");
    dirty=empty;dirty.generation=1;check(!validDoppelgangerWorldPacket(dirty,h,c),"absence cannot retain a generation");
    dirty=p;dirty.local[218][0]=NAN;check(!validDoppelgangerWorldPacket(dirty,h,c),"nonfinite helper bone rejected");
    dirty=p;dirty.local[218][0]=-1;check(!validDoppelgangerWorldPacket(dirty,h,c),"reflected helper bone rejected");
    dirty=p;dirty.local[0][9]=4;check(!validDoppelgangerWorldPacket(dirty,h,c),"unbounded root local translation rejected");
    dirty=p;dirty.rig++;check(!validDoppelgangerWorldPacket(dirty,h,c),"foreign humanoid rig rejected");
    dirty=empty;dirty.local[1][0]=1;check(!validDoppelgangerWorldPacket(dirty,h,c),"absent clone cannot retain a body pose");
    DoppelgangerWorldOrder order{};check(order.accept(p)&&!order.accept(p),"duplicate clone packet rejected");
    auto next=p;next.sequence++;next.sampleTimeUs+=33000;next.transform[0]+=.8f;check(order.accept(next),"same clone moving sample accepted");
    auto reused=next;reused.sequence++;reused.sampleTimeUs++;reused.generation++;check(!order.accept(reused),"component reuse requires fresh event");
    reused.event++;check(order.accept(reused),"new component incarnation accepted only as fresh event");
    empty.sequence=reused.sequence+1;empty.sampleTimeUs=reused.sampleTimeUs+1;empty.event=reused.event+1;check(order.accept(empty)&&!order.accept(next),"clear prevents stale clone resurrection");
    std::array<float,12> shown{};float alpha=0;check(doppelgangerWorldPresentation(p,1000,&next,1080,1040,shown,alpha)&&std::fabs(alpha-.5f)<.00001f&&std::fabs(shown[0]-p.transform[0]-.4f)<.0001f,"clone root interpolates rather than stepping at packet Hz");
    check(doppelgangerWorldPresentation(p,1000,&next,1080,1200,shown,alpha)&&alpha==1,"clone does not extrapolate past known sample");
    for(unsigned i=0;i<7;++i){auto b=next;switch(i){case 0:b.generation++;break;case 1:b.equipmentHandle++;break;case 2:b.event++;break;case 3:b.sourceWorldEpoch++;break;case 4:b.sourceToken++;break;case 5:b.transform[0]+=10;break;case 6:b.present=0;break;}
        check(doppelgangerWorldPresentation(p,1000,&b,1080,1040,shown,alpha)&&alpha==0&&shown==p.transform,"clone does not blend across identity, clear, teleport or epoch boundaries");}
    check(doppelgangerWorldDigest(p)!=doppelgangerWorldDigest(next),"wire digest records exact sampled clone transform");
    std::cout<<"PASS: Doppelganger world wire, source ownership, incarnation retirement and root interpolation\n";
}
