#include "movement_resume_policy.h"
#include "movement_fixture_policy.h"
#include "native_readable_pause.h"
#include "diagnostic_schedule.h"
#include <iostream>
#include <cstdlib>
void check(bool ok,const char* name){if(!ok){std::cerr<<name<<'\n';std::exit(1);}}
int main(){
    {
        DiagnosticSchedule<7> d;unsigned i=99;
        for(unsigned k=0;k<7;++k)check(d.next(100+k,i)&&i==k,"diagnostics visit every writer in order");
        check(!d.next(349,i),"diagnostics do not repeat inside250ms");
        check(d.next(350,i)&&i==0,"first diagnostic becomes due at its own deadline");
        check(!d.next(350,i),"later slots do not burst early with the first slot");
        for(unsigned k=1;k<7;++k)check(d.next(350+k,i)&&i==k,"writers retain fair independent deadlines");
        for(unsigned k=0;k<7;++k)check(d.next(5000,i)&&i==k,"late frame advances one fair cycle");
        check(!d.next(5000,i),"late frame never accumulates missed cycles");
        check(!d.next(4999,i),"clock rollback does not repeat writes");
    }

    {
        CoopPosePacket a{},b{};a.role=0;b.role=1;a.flags=b.flags=1;
        a.session=1;b.session=2;a.worldEpoch=3;b.worldEpoch=4;a.mapId=b.mapId=5;
        a.gameLogicState=b.gameLogicState=1;a.cinematicMetadataValid=b.cinematicMetadataValid=1;
        check(movementFixtureMenuPair(a,b),"finite test fixture may retain in gameplay");
        for(unsigned state:{2u,4u}){auto menu=b;menu.gameLogicState=state;menu.cinematicMetadataValid=0;
            check(nativeMovementMenuState(state),"known native screen suspends movement");
            check(movementFixtureMenuPair(a,menu)&&movementFixtureMenuPair(menu,a),"either native menu retains existing fixture");
            check(!coopGameplayReady(menu),"fixture retention never changes gameplay readiness");
            menu=b;menu.pendingGameLogicState=state;
            check(movementFixtureMenuPair(a,menu)&&!coopGameplayReady(menu),"pending native screen retains fixture but blocks gameplay");}
        for(unsigned state:{0u,3u,5u,6u,7u,8u,9u,10u,11u,12u,13u,14u,15u,16u,17u,999u}){
            check(!nativeMovementMenuState(state),"other logic states never authorize screen resume");
            auto bad=b;bad.gameLogicState=state;check(!movementFixtureMenuPair(a,bad),"other native states retire fixture");
            if(state){bad=b;bad.pendingGameLogicState=state;check(!movementFixtureMenuPair(a,bad),"pending load/death/unknown retires fixture");}}
        auto bad=b;bad.mapId++;check(!movementFixtureMenuPair(a,bad),"other map rejected");
        bad=b;bad.role=0;check(!movementFixtureMenuPair(a,bad),"same role rejected");
        bad=b;bad.session=0;check(!movementFixtureMenuPair(a,bad),"missing session rejected");
        bad=b;bad.flags=0;check(!movementFixtureMenuPair(a,bad),"missing local world pose rejected");
        bad=b;bad.cinematicNodeCount=1;check(!movementFixtureMenuPair(a,bad),"cinematic fixture retirement retained");
        bad=b;bad.cinematicMetadataValid=0;check(!movementFixtureMenuPair(a,bad),"unknown gameplay cinematic state rejected");
        bool held=false;auto note=b;note.gameLogicState=4;note.cinematicMetadataValid=0;
        check(!retainNativeReadableBody(a,b,true,true,true,false,held),"ordinary running never starts readable hold");
        check(retainNativeReadableBody(a,note,true,true,true,false,held)&&!coopGameplayReady(note),"native note retains existing visible body without gameplay permission");
        check(retainNativeReadableBody(a,b,true,true,true,true,held),"same-world fresh recovery keeps held body");
        check(!retainNativeReadableBody(a,b,true,true,true,false,held),"fresh native root rearm ends hold before semantic handshake and bank recovery");
        for(unsigned failure=0;failure<3;++failure){held=true;
            check(!retainNativeReadableBody(a,note,failure!=0,failure!=1,failure!=2,true,held)&&!held,"identity freshness and native ownership are independently required");}
        for(unsigned state:{0u,2u,3u,5u,6u,7u,8u,9u,10u,11u,12u,13u,14u,15u,16u}){auto other=note;other.gameLogicState=state;held=true;
            check(!retainNativeReadableBody(a,other,true,true,true,true,held),"foreign logic retires readable body");}
        auto loading=note;loading.pendingGameLogicState=3;held=true;
        check(!retainNativeReadableBody(a,loading,true,true,true,true,held),"pending load retires readable body");
        auto pending=b;pending.pendingGameLogicState=4;held=false;
        check(retainNativeReadableBody(pending,a,true,true,true,false,held),"pending ReadNote is recognized before commit");
        auto other=note;other.mapId++;held=true;check(!retainNativeReadableBody(a,other,true,true,true,true,held),"different map retires visible body");
        other=note;other.flags=0;held=true;check(!retainNativeReadableBody(a,other,true,true,true,true,held),"missing pose retires visible body");
        other=note;other.cinematicNodeCount=1;held=true;check(!retainNativeReadableBody(a,other,true,true,true,true,held),"cinematic activity retires visible body");
    }
    using P=MovementResumePolicy;P p;P::Identity id{1,2,3,4,5,6,7,8};
    check(!p.arm(id,false)&&p.phase==P::Off,"requires a live owned bank");
    auto invalid=id;invalid[2]=0;check(!p.arm(invalid,true),"zero identity refused");
    check(p.arm(id,true),"arm current presentation");
    check(p.observe(id,false,true,true,100)==P::Continue,"normal gameplay does not restart");
    check(p.running(false,110)&&p.running(false,190),"native ownership may retire before sampled menu state");
    check(p.observe(id,true,false,true,200)==P::Wait&&p.phase==P::Suspended,"native pause suspends");
    check(!p.restored(),"pause cannot commit a restore");
    check(p.observe(id,false,true,false,400)==P::Wait&&p.phase==P::Suspended,"stale peer cannot rearm");
    check(p.observe(id,false,true,true,500)==P::Wait,"settling begins after readiness");
    check(p.observe(id,false,true,true,799)==P::Wait,"300 ms stable readiness required");
    check(p.observe(id,false,true,true,800)==P::Restore,"same-world native resume may restore");
    check(p.restored()&&p.phase==P::Running,"only successful rearm resumes normal ownership");
    for(unsigned k=0;k<id.size();++k){p.arm(id,true);p.observe(id,true,false,true,100);auto changed=id;++changed[k];
        check(p.observe(changed,false,true,true,500)==P::Reject&&!p.enabled(),"every actor/world/session identity is pinned");}
    p.arm(id,true);p.observe(id,true,false,true,100);p.observe(id,false,true,true,200);p.observe(id,false,true,true,500);
    check(p.observe(id,false,true,true,10500)==P::Reject,"failed native recovery has a bounded deadline");
    p.arm(id,true);p.observe(id,true,false,true,100);p.observe(id,false,true,true,200);p.observe(id,false,true,true,500);
    check(p.observe(id,true,false,true,600)==P::Wait&&p.phase==P::Suspended,"second pause cancels pending rearm");
    p.off();check(p.observe(id,false,true,true,1000)==P::Wait&&!p.enabled(),"explicit off prevents resurrection");
    p.arm(id,true);check(p.running(false,100)&&!p.running(false,450)&&!p.enabled(),"unknown retirement eventually faults without rearming");
    std::cout<<"Movement pause resume policy passed\n";
}
