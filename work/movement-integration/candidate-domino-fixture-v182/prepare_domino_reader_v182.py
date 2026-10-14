"""Strict, read-only Domino equipment identification from exact Void/RTTI data."""
from pathlib import Path
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab/src'
s=(S/'native_player_mesmerize_item.h').read_text(encoding='utf-8-sig')
s=s.replace('models/powers/mesmerize/mesmerize.inventoryitem','models/powers/phantasmallink/phantasmal_link.inventoryitem')
s=s.replace('components/characters/player/base/powers/mesmerize/mesmerize.cpntpowermesmerize','components/characters/player/base/powers/phantasmallink/phantasmal_link.cpntpowerphantasmallink')
s=s.replace('Mesmerize','Domino').replace('Mesmer','Domino').replace('0x1b9bea8','0x1b52ec8').replace('0x1b9bd68','0x1b52d88')
s=s.replace('// Exact installed Domino item. Its initial zero-quantity entry has no\n// entity; native 618DE0 creates the equipment through 619380 when acquired.', '// Exact installed Domino (PhantasmalLink) declaration and static RTTI.\n// Saved native inventory v177 confirms its zero-quantity, uninstantiated entry.\n// Instantiated ownership is a candidate until the bounded native grant test.')
(S/'native_player_domino_item.h').write_text(s,encoding='utf-8')
f=S/'runtime_probe.cpp';s=f.read_text(encoding='utf-8-sig')
anchor='#include "native_player_mesmerize_item.h"';assert s.count(anchor)==1
s=s.replace(anchor,anchor+'\n#include "native_player_domino_item.h"')
a=s.index('    if(argc==3&&!std::strcmp(argv[2],"--player-mesmerize"))');b=s.index('    if(argc==3&&!std::strcmp(argv[2],"--player-bendtime"))',a)
block=s[a:b].replace('--player-mesmerize','--player-domino').replace('NativeMesmerize','NativeDomino')
s=s[:a]+block+s[a:];f.write_text(s,encoding='utf-8')
f=S/'world_identity_tests.cpp';s=f.read_text(encoding='utf-8-sig')
anchor='#include "native_player_mesmerize_item.h"';assert s.count(anchor)==1;s=s.replace(anchor,anchor+'\n#include "native_player_domino_item.h"')
anchor='                        NativeMesmerizeItem observed{};';assert s.count(anchor)==1
insert='''                        {
                            auto domino=mesmer;
                            domino.str(strings,"models/powers/phantasmallink/phantasmal_link.inventoryitem");
                            domino.str(strings+0x100,"components/characters/player/base/powers/phantasmallink/phantasmal_link.cpntpowerphantasmallink");
                            domino.q(pc,base+0x1b52ec8);domino.q(ped,base+0x1b52d88);
                            NativeDominoItem item{};const auto inspectDomino=[&](Memory& mem){return readNativeDominoItem([&](auto a,auto d,auto n){return mem.read(a,d,n);},base,item);};
                            require(inspectDomino(domino)&&item.player.character==2&&item.instantiated&&item.started&&item.equipped,"Domino static RTTI and exact native asset identities are coherent");
                            auto unowned=domino;unowned.d(inv+0x60,1);unowned.q(inv+0x70,0);unowned.d(pe+8,0x3fff0010);unowned.q(pe+0x10,0);unowned.q(pe+0x18,0);unowned.d(pe+0x20,0);
                            require(inspectDomino(unowned)&&!item.instantiated&&!item.quantity,"recorded unowned Domino inventory does not imply a target network");
                            auto invalid=domino;invalid.str(names+0x400,"models/characters/player/corvo/body.def");invalid.str(names+0x600,"models/characters/player/corvo/right_arm.def");require(!inspectDomino(invalid),"Domino initial native fixture refuses Corvo");
                            invalid=domino;invalid.q(pc,base+0x1b9bea8);require(!inspectDomino(invalid),"Mesmerize cannot impersonate Domino equipment");
                            invalid=domino;invalid.q(pc+0x10,actor);require(!inspectDomino(invalid),"Domino rejects a foreign component owner");
                            invalid=domino;invalid.d(entity+0xa8,handle+1);require(!inspectDomino(invalid),"Domino rejects a recycled equipment entity");
                            invalid=domino;invalid.d(pe+0x20,2);require(!inspectDomino(invalid),"Domino rejects impossible quantity");
                            invalid=domino;unsigned changed=0;require(!readNativeDominoItem([&](auto a,auto d,auto n){const bool ok=invalid.read(a,d,n);if(a==pc+0x1c&&++changed==1)invalid.d(a,0);return ok;},base,item),"Domino detects concurrent equipment retirement");
                            std::cout<<"PASS: Domino exact asset/RTTI, native unowned inventory, character, ownership and race guards\\n";
                        }
'''
s=s.replace(anchor,insert+anchor);f.write_text(s,encoding='utf-8')
