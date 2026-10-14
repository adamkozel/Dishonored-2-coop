from pathlib import Path
root=Path(__file__).resolve().parents[2]
src=root/'outputs/d2-coop-lab/src'
s=(src/'door_sync.h').read_text()
for a,b in [('DOOR','USABLE'),('Door','Usable'),('door','usable'),('Openable','Usable')]:
    s=s.replace(a,b)
s=s.replace('#include "usable_native_queue.h"','#include "usable_native_api.h"\n#include "usable_catalog.h"')
s=s.replace('#include "usable_session_scope.h"','#include "door_session_scope.h"')
# Reuse the already tested world/menu/checkpoint scope policy.
for a,b in [('UsableSessionScope','DoorSessionScope'),('usableSessionScope(','doorSessionScope('),('usableSessionRenewalReady(','doorSessionRenewalReady('),('usableSessionWorldMatches(','doorSessionWorldMatches('),('usableSessionCanRetainMenu(','doorSessionCanRetainMenu('),('usableResumeDeadline(','doorResumeDeadline('),('usableCheckpointBarrierActive(','doorCheckpointBarrierActive(')]:s=s.replace(a,b)
s=s.replace('// Opt-in authored ordinary-usable catalogue for the current paired world.\n// Locked, linked, blocked, broken and ambiguous usables need separate authority.','// Opt-in native state-list family. First runtime coverage: Dunwall windows.\n// Quest devices, vaults and custom-prompt state lists are not automatically eligible.')
s=s.replace('observedTarget=0,observedSide=0,requestTarget=0,requestSide=0','observedTarget=0,requestTarget=0')
s=s.replace(',sideCorrections=0','')
a=s.index('bool usableNativeDesired(');b=s.index('void enableUsableSync(',a)
s=s[:a]+'''bool usableNativeDesired(const NativeUsable& n,unsigned& target){
    if(!nativeUsableWindowFamily(n))return false;
    target=static_cast<unsigned>(n.pending>=0?n.pending:n.current);
    return nativeUsableTargetValid(n,target);
}
'''+s[b:]
s=s.replace('nativeUsableQueueAvailable()','nativeUsableIndexAvailable()')
s=s.replace('if(!nativeUsableIndexAvailable()||!observedGame','if(std::strcmp(mapName,kGuardMap)||!nativeUsableIndexAvailable()||!observedGame')
s=s.replace(';fresh.observedSide=fresh.binding.openSide','')
s=s.replace('ordinary_usable_catalog_armed','native_window_family_armed')
s=s.replace('p.sourceArm=s.arm;','p.sourceArm=s.arm;p.stateCount=s.binding.stateCount;p.rulesHash=nativeUsableRulesHash(s.binding);')
s=s.replace(';p.side=s.order.side','').replace(';p.side=s.requestSide','')
s=s.replace('s.binding.definitionName!=p.definition){','s.binding.definitionName!=p.definition||p.stateCount!=s.binding.stateCount||p.rulesHash!=nativeUsableRulesHash(s.binding)){')
s=s.replace('s.order.set(p.target,p.side)','s.order.set(p.target)').replace('(p.target!=s.order.target||p.side!=s.order.side)','p.target!=s.order.target')
s=s.replace('s.order.accept(p.revision,p.target,p.side)','s.order.accept(p.revision,p.target)').replace('&&p.side==s.order.side','')
s=s.replace('unsigned target=0,side=0','unsigned target=0').replace('usableNativeDesired(n,target,side)','usableNativeDesired(n,target)')
s=s.replace('s.order.set(target,side)','s.order.set(target)')
s=s.replace('if(n.current==n.pending&&n.current!=1)','if(!n.transitioning()&&n.pending== -1)')
s=s.replace(';s.observedSide=side','').replace('||(target==2&&side!=s.observedSide)','').replace(';s.requestSide=side','')
s=s.replace('nativeUsableMatchesTarget(n,s.order.target,s.order.side)','nativeUsableAtTarget(n,s.order.target)')
a=s.index('    if(s.inFlight||n.current==1');b=s.index('\n}\nvoid receiveUsablePacket',a)
s=s[:a]+'''    if(s.inFlight||n.transitioning()||n.pending!= -1)return;
    if(!nativeUsableTargetValid(n,s.order.target)){failUsableSync(s,"native_transition_not_eligible");return;}
    NativeUsable fresh{},after{};
    if(!readBoundUsable(s,fresh)||fresh.current!=n.current||fresh.pending!= -1||fresh.transitioning()||
       !nativeUsableWindowFamily(fresh)||!nativeUsableTargetValid(fresh,s.order.target)){failUsableSync(s,"native_lifetime_changed_before_apply");return;}
    ++s.calls;
    if(!requestNativeUsableIndex(fresh,s.order.target,after)){failUsableSync(s,"native_index_refused");return;}
    s.inFlight=s.order.revision;s.inFlightTarget=s.order.target;s.inFlightUntil=tick+4000;
    s.observedTarget=s.order.target;s.result="native_transition_requested";
'''+s[b:]
s=s.replace('\\"side\\":%u,','').replace('\\"sideCorrections\\":%llu,','').replace(',s.order.side','').replace(',s.sideCorrections','')
(src/'usable_sync.h').write_text(s)

p=src/'engine_bridge.cpp';s=p.read_text()
assert '#include "usable_sync.h"' not in s
s=s.replace('#include "door_sync.h"','#include "door_sync.h"\n#include "usable_sync.h"')
s=s.replace('        if(magic==0x4f443244) {','''        if(magic==0x53553244) {
            if(messages[i]->m_cbSize==sizeof(CoopUsablePacket)){CoopUsablePacket p{};std::memcpy(&p,messages[i]->m_pData,sizeof(p));receiveUsablePacket(p,tick);}
            else ++usableCatalogRejected;
            messages[i]->Release();continue;
        }
        if(magic==0x4f443244) {''')
line=next(x for x in s.splitlines() if 'coop_door_sync on' in x and 'return true;' in x)
s=s.replace(line,line+'\n'+line.replace('coop_door_sync','coop_usable_sync'))
for cmd in ['on','session','off','duplicate']:
    line=next(x for x in s.splitlines() if f'request.command,"coop_door_sync {cmd}"' in x)
    s=s.replace(line,line+'\n'+line.replace('coop_door_sync','coop_usable_sync').replace('doorSync','usableSync'))
s=s.replace('writeDoorSync();writeCoinSync();','writeDoorSync();writeCoinSync();writeUsableSync();')
s=s.replace('serviceDoorSync(tick);});','serviceDoorSync(tick);});timedNativeService("serviceUsableSync",[&]{serviceUsableSync(tick);});')
p.write_text(s)
p=src/'checkpoint_sync.h';s=p.read_text().replace('finishCoinCheckpointRecovery();','finishCoinCheckpointRecovery();finishUsableCheckpointRecovery();').replace('beginCoinCheckpointRecovery();','beginCoinCheckpointRecovery();beginUsableCheckpointRecovery();');p.write_text(s)
p=root/'outputs/d2-coop-lab/Send-D2Command.ps1';s=p.read_text().replace('|coop_usable_fixture|','|coop_usable_fixture|coop_usable_sync|');p.write_text(s)
p=root/'work/movement-integration/Build-UsableState-v829.ps1';s=p.read_text().replace('v829','v831').replace('Build829','Build831');p.with_name('Build-UsableSync-v831.ps1').write_text(s)
print('Created usable sync831 using proven native world/menu/checkpoint scope policy; no deployment.')
