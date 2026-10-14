from pathlib import Path
p=Path('outputs/d2-coop-lab/src/mesmerize_world_stream.h');s=p.read_text().replace('s.presented=pending.packet;s.lease=tick+300;', 's.presented=pending.packet;s.lease=pending.packet.state==2?s.retireAt:tick+300;');p.write_text(s)
p=Path('outputs/d2-coop-lab/src/mesmerize_world_protocol.h');s=p.read_text().replace('        if(p.sequence<=sequence', '        if(p.state!=1&&p.state!=2&&p.state!=3)return false;\n        if(p.sequence<=sequence');p.write_text(s)
