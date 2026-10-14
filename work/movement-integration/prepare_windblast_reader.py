"""Derive an exact-type read-only Windblast inventory reader; no generic grants."""
from pathlib import Path
P=Path(__file__).resolve().parents[2]/'outputs/d2-coop-lab/src'
s=(P/'native_player_darkvision_item.h').read_text()
s=s.replace('DarkVision','Windblast').replace('Dark Vision','Windblast').replace('darkvision','windblast')
s=s.replace('std::uintptr_t manager=0;unsigned managerFlags=0;bool instantiated=false,managerStarted=false;','bool instantiated=false;')
s=s.replace('if(!readNativePlayerSword(read,base,p.player)||p.player.sword.logic!=1)return false;','if(!readNativePlayerSword(read,base,p.player)||p.player.sword.logic!=1||p.player.character!=1)return false;')
s=s.replace('0x1b51c18','0x1b53488').replace('0x1b51ad8','0x1b53348')
start=s.index('    // The exact player has88')
end=s.index('\n}',start)
s=s[:start]+'''    // Exact RTTI and asset identity precede any fixture acquisition. The
    // Windblast equipment has its own native entity; no player vision manager.
    if(!member(p.entity,p.component,64))return false;
    p.instantiated=true;return true;'''+s[end:]
s=s.replace('||a.manager!=b.manager||a.managerFlags!=b.managerFlags','')
(P/'native_player_windblast_item.h').write_text(s)
print('Prepared exact Corvo-only Windblast item reader')
