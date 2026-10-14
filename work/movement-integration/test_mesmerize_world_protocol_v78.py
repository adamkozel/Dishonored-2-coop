from pathlib import Path
p=Path('outputs/d2-coop-lab/src/shadow_action_protocol_tests.cpp');s=p.read_text().replace('#include "power_fx_protocol.h"','#include "power_fx_protocol.h"\n#include "mesmerize_world_protocol.h"')
pos=s.index('    CoopShadowActionPacket p{};')
test='''    {
      CoopMesmerizeWorldPacket m{};m.sequence=1;m.sourceSession=22;m.targetSession=11;m.sourceWorldEpoch=7;m.targetWorldEpoch=3;m.mapId=99;m.sourceToken=9;m.targetToken=4;m.event=1;m.sampleTimeUs=1000000;m.rootSequence=99;m.nativeHandle=0xc881200;m.state=1;m.transform={-33,-155,66,1,0,0,0,1,0,0,0,1};
      check(validMesmerizeWorldPacket(m,h,c),"Mesmerize exact Emily world cosmetic scope");
      auto bad=m;bad.role=0;check(!validMesmerizeWorldPacket(bad,h,c),"Corvo cannot author Mesmerize world");
      bad=m;bad.targetWorldEpoch++;check(!validMesmerizeWorldPacket(bad,h,c),"Mesmerize old world refused");
      bad=m;bad.transform[0]=NAN;check(!validMesmerizeWorldPacket(bad,h,c),"Mesmerize nonfinite origin refused");
      bad=m;bad.transform[3]=-1;check(!validMesmerizeWorldPacket(bad,h,c),"Mesmerize reflected basis refused");
      bad=m;bad.transform[7]=0;check(!validMesmerizeWorldPacket(bad,h,c),"Mesmerize degenerate basis refused");
      bad=m;bad.state=0;check(!validMesmerizeWorldPacket(bad,h,c),"Preview or unknown Mesmerize state refused");
      bad=m;bad.nativeHandle=0x3fff0010;check(!validMesmerizeWorldPacket(bad,h,c),"Absent handle cannot create Mesmerize");
      bad=m;bad.reserved=1;check(!validMesmerizeWorldPacket(bad,h,c),"Unknown Mesmerize semantics refused");
      MesmerizeWorldOrder order;check(order.accept(m)&&!order.accept(m),"Mesmerize incarnation starts once");
      auto loop=m;loop.sequence++;loop.sampleTimeUs+=100000;loop.state=3;
      MesmerizeWorldOrder orphan;check(!orphan.accept(loop),"Orphan active object cannot spawn");check(order.accept(loop),"Native active state follows spawn");
      bad=loop;bad.sequence++;bad.sampleTimeUs+=100000;bad.nativeHandle++;check(!order.accept(bad),"Handle replacement inside same event refused");
      bad=loop;bad.sequence++;bad.sampleTimeUs+=100000;bad.state=1;check(!order.accept(bad),"Active world cannot replay spawn");
      bad=loop;bad.sequence++;bad.sampleTimeUs+=100000;bad.transform[0]+=.03f;check(!order.accept(bad),"Static world anchor cannot drift");
      auto end=loop;end.sequence++;end.sampleTimeUs+=100000;end.state=2;check(order.accept(end),"Source native removal retires same event");
      bad=end;bad.sequence++;bad.sampleTimeUs+=100000;bad.state=3;check(!order.accept(bad),"Retired object cannot revive");
      auto next=m;next.sequence=4;next.sampleTimeUs=1500000;next.event++;next.nativeHandle++;check(order.accept(next),"New committed incarnation after retirement accepted");
      auto cancel=next;cancel.sequence++;cancel.sampleTimeUs+=100000;cancel.state=2;check(order.accept(cancel),"Spawn can retire without an active phase");
    }
'''
s=s[:pos]+test+s[pos:];p.write_text(s)
