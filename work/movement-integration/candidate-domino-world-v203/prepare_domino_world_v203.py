from pathlib import Path
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab/src'
p=S/'engine_bridge.cpp';s=p.read_text(encoding='utf-8-sig');assert '#include "domino_world_stream.h"' not in s
s=s.replace('#include "mesmerize_world_stream.h"','#include "mesmerize_world_stream.h"\n#include "domino_world_stream.h"\n#include "domino_mark_presentation.h"',1)
needle='        if(magic==0x4d573244){';assert needle in s
s=s.replace(needle,'''        if(magic==0x44573244){
            if(messages[i]->m_cbSize==sizeof(CoopDominoWorldPacket)){CoopDominoWorldPacket p{};std::memcpy(&p,messages[i]->m_pData,sizeof(p));receiveDominoWorld(p,tick);}
            else ++dominoWorldRejected;
            messages[i]->Release();continue;
        }
'''+needle,1)
s=s.replace('writeGrenadeFx();writeMesmerizeVisual();writeMesmerizeWorldStream();','writeGrenadeFx();writeMesmerizeVisual();writeMesmerizeWorldStream();writeDominoWorld();',1)
s=s.replace('sampleMesmerizeWorld(tick);presentMesmerizeWorld(tick);','sampleMesmerizeWorld(tick);presentMesmerizeWorld(tick);sampleDominoWorld(tick);presentDominoWorld(tick);serviceDominoMarks(tick);',1)
p.write_text(s,encoding='utf-8')
p=S/'shadow_action_protocol_tests.cpp';s=p.read_text(encoding='utf-8-sig');s=s.replace('#include "mesmerize_world_protocol.h"','#include "mesmerize_world_protocol.h"\n#include "domino_world_protocol.h"',1)
needle='void rangedShotTests(){';assert needle in s;s=s.replace(needle,'#include "domino_world_protocol_tests.h"\n'+needle,1)
needle='      CoopMesmerizeWorldPacket m{};';assert needle in s;s=s.replace(needle,'      dominoPacketTests(h,c);\n'+needle,1);p.write_text(s,encoding='utf-8')
print('Native Domino cosmetic mark stream connected; runtime verification pending')
