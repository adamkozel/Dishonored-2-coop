from pathlib import Path
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab/src';W=R/'work/movement-integration'
def edit(p,a,b):
    s=p.read_text(encoding='utf-8-sig');assert s.count(a)==1,(p,a[:70]);p.write_text(s.replace(a,b),encoding='utf-8')
p=S/'engine_bridge.cpp'
edit(p,'#include "swarm_world_stream.h"','#include "swarm_world_stream.h"\n#include "doppelganger_world_stream.h"')
edit(p,'        if(magic==0x53573244){','''        if(magic==0x43443244){
            if(messages[i]->m_cbSize==sizeof(CoopDoppelgangerWorldPacket)){CoopDoppelgangerWorldPacket p{};std::memcpy(&p,messages[i]->m_pData,sizeof(p));receiveDoppelgangerWorld(p,tick);}
            else ++doppelgangerWorldRejected;
            messages[i]->Release();continue;
        }
        if(magic==0x53573244){''')
edit(p,'writeSwarmWorld();','writeSwarmWorld();writeDoppelgangerWorld();')
edit(p,'sampleSwarmWorld(tick);presentSwarmWorld(tick);','sampleSwarmWorld(tick);presentSwarmWorld(tick);sampleDoppelgangerWorld(tick);presentDoppelgangerWorld(tick);')
p=S/'shadow_action_protocol_tests.cpp'
edit(p,'#include "swarm_world_protocol.h"','#include "swarm_world_protocol.h"\n#include "doppelganger_world_protocol.h"')
edit(p,'#include "swarm_world_protocol_tests.h"','#include "swarm_world_protocol_tests.h"\n#include "doppelganger_world_protocol_tests.h"')
edit(p,'swarmPacketTests(h,c);','swarmPacketTests(h,c);doppelgangerPacketTests(h,c);')
t=(W/'Build-DoppelgangerBinding-v407.ps1').read_text(encoding='utf-8-sig').replace('doppelganger-binding-','doppelganger-stream-').replace('-v407.log','-v418.log').replace('Build407','Build418')
(W/'Build-DoppelgangerStream-v418.ps1').write_text(t,encoding='utf-8')
print('Connected176-byte clone presentation stream to existing native Steam transport; no receiver entity creation')
