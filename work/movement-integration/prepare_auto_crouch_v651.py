from pathlib import Path
import shutil
R=Path(__file__).resolve().parents[2]
S=R/'outputs/d2-coop-lab/src'
B=R/'work/movement-integration/auto-crouch-pre-v651'
shutil.copytree(S,B)
def edit(name,old,new):
 p=S/name;t=p.read_text();assert t.count(old)==1,(name,t.count(old));p.write_text(t.replace(old,new))
edit('native_player_states.h','inline bool nativePlayerMotion(','''//649/650 native window-sill observations: physical stance2, AutoCrouch10,
// optionally the independent right-arm EquipChange53. Map presentation only;
// retain actual state IDs and capsule for all ownership checks and gameplay.
inline bool nativePlayerAutoCrouchStates(const NativePlayerStates& s,std::uintptr_t base){
    if(s.player.logic!=1||!s.defaultMode||s.player.current!=2||s.active.empty()||s.active.size()>2)return false;
    unsigned crouch=0,equip=0;
    for(const auto& a:s.active){
        if(a.id==10&&a.vtable==base+0x1c812b8)++crouch;
        else if(a.id==53&&a.vtable==base+0x1c79cf8)++equip;
        else return false;
    }
    return crouch==1&&equip<=1;
}
inline bool nativePlayerMotion(''')
edit('native_player_states.h','    if(nativePlayerGroundPairedStates(s,base)){motion=0;return true;}','    if(nativePlayerAutoCrouchStates(s,base)){motion=1;return true;}\n    if(nativePlayerGroundPairedStates(s,base)){motion=0;return true;}')
edit('native_player_action.h','    if(nativePlayerGroundDrawRequestStates(s,base)){action=7;return true;}','    if(nativePlayerAutoCrouchStates(s,base)){action=s.active.size()==2?7u:0u;return true;}\n    if(nativePlayerGroundDrawRequestStates(s,base)){action=7;return true;}')
edit('native_player_equipment_scene.h','    bool ground=false;for(const auto& a:states.active){','    const bool autoCrouch=nativePlayerAutoCrouchStates(states,base);\n    bool ground=false;for(const auto& a:states.active){')
edit('native_player_equipment_scene.h','        else if(a.id==53&&a.vtable==base+0x1c79cf8)s.statePointer=a.pointer;','        else if(autoCrouch&&a.id==10&&a.vtable==base+0x1c812b8)ground=true;\n        else if(a.id==53&&a.vtable==base+0x1c79cf8)s.statePointer=a.pointer;')
edit('native_player_equipment_scene.h','    //525 observed Emily preparation:', '''    //649/650: automatic crouch recovery owns only this333ms fast draw.
    if(autoCrouch&&(s.variant!=6||s.durationMs!=333||s.timeMs>333||powerState||drawRequest||possessionCast))return false;
    //525 observed Emily preparation:''')
edit('native_player_traversal_scene.h','(next!=0&&next!=6)','(next!=0&&next!=6&&!(kind==1&&next==10&&states.player.current==2))')
edit('native_player_traversal_scene.h','    s.variant=(kind==2?4u:1u)+(next==6?1u:0u);s.endStance=next==6?1u:0u;', '''    //649/650: low mantle into confined AutoCrouch10 reuses the authored
    //low-crouch presentation; native scene, destination and clock stay authoritative.
    const bool lowExit=next==6||next==10;
    s.variant=(kind==2?4u:1u)+(lowExit?1u:0u);s.endStance=lowExit?1u:0u;''')
edit('native_player_traversal_scene.h','    const char* asset=kind==2?', '    const char* asset=next==10?"midnight/gameplay/player/base/mobility/mantle_low_crouch.midnightscene":kind==2?')
edit('native_player_traversal_scene.h','       !final.defaultMode||final.active.size()!=1','       final.player.current!=states.player.current||!final.defaultMode||final.active.size()!=1')
p=S/'native_player_traversal_scene.h'
with p.open('a') as f:f.write('''
// Presentation stance is distinct from the native capsule enumeration. Never
// rewrite NativePlayerStates to masquerade AutoCrouch10 as Sneaking6.
template<class Read> bool readNativePlayerPresentationStance(Read read,std::uintptr_t base,const NativePlayerStates& states,unsigned& stance){
    if(states.player.current<=1){stance=states.player.current;return true;}
    if(nativePlayerAutoCrouchStates(states,base)){stance=1;return true;}
    NativePlayerTraversalScene traversal{};
    if(states.player.current==2&&readNativePlayerTraversalScene(read,base,states,traversal)&&
       traversal.variant==2&&traversal.asset=="midnight/gameplay/player/base/mobility/mantle_low_crouch.midnightscene"){
        stance=1;return true;
    }
    return false;
}
''')
edit('engine_bridge.cpp','#include "native_player_states.h"','#include "native_player_states.h"\n#include "native_player_traversal_scene.h"')
edit('engine_bridge.cpp','''       readNativePlayerStance(memoryRead,base,stance)&&stance.game==game&&stance.player==player&&stance.current<=1) {
        outgoing.playerStance=stance.current;outgoing.playerStanceValid=1;
        NativePlayerStates states{};std::uint32_t motion=0;
        const bool stateValid=readNativePlayerStates(memoryRead,base,states);''','''       readNativePlayerStance(memoryRead,base,stance)&&stance.game==game&&stance.player==player) {
        NativePlayerStates states{};std::uint32_t motion=0,presentation=0;
        const bool stateValid=readNativePlayerStates(memoryRead,base,states);
        // Existing0/1 wire stays unchanged. Exact native automatic crouch and
        //its owned low-mantle scene additionally map to presentation crouch1.
        if(stance.current<=1){outgoing.playerStance=stance.current;outgoing.playerStanceValid=1;}
        else if(stateValid&&states.player.game==game&&states.player.player==player&&states.player.current==stance.current&&
                readNativePlayerPresentationStance(memoryRead,base,states,presentation)){
            outgoing.playerStance=presentation;outgoing.playerStanceValid=1;
        }''')
print('Preserved635 sources; added exact automatic-crouch presentation and owned low-mantle/recovery bindings')
