from pathlib import Path
p=Path('outputs/d2-coop-lab/src/engine_bridge.cpp');s=p.read_text().replace('#include "power_fx_stream.h"','#include "power_fx_stream.h"\n#include "mesmerize_world_stream.h"');needle='        if(magic==0x46503244){';s=s.replace(needle,'''        if(magic==0x4d573244){
            if(messages[i]->m_cbSize==sizeof(CoopMesmerizeWorldPacket)){CoopMesmerizeWorldPacket p{};std::memcpy(&p,messages[i]->m_pData,sizeof(p));receiveMesmerizeWorld(p,tick);}
            else ++mesmerizeWorldRejected;
            messages[i]->Release();continue;
        }
'''+needle)
s=s.replace('serviceMesmerizeVisual(tick);','sampleMesmerizeWorld(tick);presentMesmerizeWorld(tick);serviceMesmerizeVisual(tick);').replace('writeGrenadeFx();writeMesmerizeVisual();','writeGrenadeFx();writeMesmerizeVisual();writeMesmerizeWorldStream();')
s=s.replace('    if(!std::strcmp(text,"coop_mesmerize_visual in")', '    if(!std::strcmp(text,"coop_mesmerize_visual duplicate"))return true;\n    if(!std::strcmp(text,"coop_mesmerize_visual in")').replace('    if(!std::strcmp(request.command,"coop_mesmerize_visual in"))', '    if(!std::strcmp(request.command,"coop_mesmerize_visual duplicate")){duplicateMesmerizeWorld();return;}\n    if(!std::strcmp(request.command,"coop_mesmerize_visual in"))');p.write_text(s)
p=Path('outputs/d2-coop-lab/src/mesmerize_visual_fixture.h');s=p.read_text().replace('bool pendingHide=false;', 'bool pendingHide=false,network=false;std::uint64_t event=0,streamToken=0,peerToken=0;');s=s.replace('const unsigned request=mesmerizeVisualRequest;', 'unsigned request=mesmerizeVisualRequest;');needle='    if(request==4||(p.until&&tick>=p.until)';idx=s.index(needle)
s=s[:idx]+'''    const auto& stream=mesmerizeWorldStream;
    const bool automatic=localRole==0&&mesmerizeWorldStreamCurrent(tick)&&stream.lease>tick&&movementBankActive()&&coopGameplayReady(remote);
    const unsigned remoteVariant=stream.presented.state==1?1:stream.presented.state==3?2:3;
    if(p.network&&p.until&&(!automatic||p.event!=stream.presented.event||p.streamToken!=stream.token||p.peerToken!=stream.peerToken)){p.pendingHide=true;p.wanted=0;}
    if(automatic&&!request){
        if(p.network&&p.until&&!p.pendingHide&&p.event==stream.presented.event){p.until=tick+300;if(p.wanted!=remoteVariant)request=remoteVariant;}
        else if(!p.until&&!p.pendingHide&&stream.presented.state==1&&
                (p.event!=stream.presented.event||p.streamToken!=stream.token||p.peerToken!=stream.peerToken))request=1;
    }
'''+s[idx:]
s=s.replace('            float pos[3]{-33.6873f,-155.303f,66.3984f},axis[9]{1,0,0,0,1,0,0,0,1};','            float pos[3]{-33.6873f,-155.303f,66.3984f},axis[9]{1,0,0,0,1,0,0,0,1};\n            if(automatic){std::memcpy(pos,stream.presented.transform.data(),12);std::memcpy(axis,stream.presented.transform.data()+3,36);}')
s=s.replace('        if(request==1&&!p.until)p.until=tick+12000;', '''        if(request==1&&!p.until){
            p.network=automatic;p.until=tick+(automatic?300:12000);
            if(automatic){
                p.event=stream.presented.event;p.streamToken=stream.token;p.peerToken=stream.peerToken;
                reinterpret_cast<void(*)(void*,const float*)>(gameImage+0x87c320)(reinterpret_cast<void*>(p.target.actor),stream.presented.transform.data());
                reinterpret_cast<void(*)(void*,const float*)>(gameImage+0x87c2c0)(reinterpret_cast<void*>(p.target.actor),stream.presented.transform.data()+3);
            }
        }''')
s=s.replace('\\\"fxActive\\\":%s,', '\\\"network\\\":%s,\\\"event\\\":%llu,\\\"fxActive\\\":%s,').replace('mesmerizeVisualRetired,p.effect.expires?', 'mesmerizeVisualRetired,p.network?"true":"false",p.event,p.effect.expires?');p.write_text(s)
