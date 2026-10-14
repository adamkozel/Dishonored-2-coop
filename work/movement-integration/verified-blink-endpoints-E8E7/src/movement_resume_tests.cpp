#include "movement_resume_policy.h"
#include <iostream>
#include <cstdlib>
void check(bool ok,const char* name){if(!ok){std::cerr<<name<<'\n';std::exit(1);}}
int main(){
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
