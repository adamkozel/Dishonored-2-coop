from pathlib import Path
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab/src';W=R/'work/movement-integration'
p=S/'npc_attack_protocol.h';s=p.read_text().replace('Hit,Ack,Stop','Hit,Ack,Stop,Terminal').replace('version=1','version=2').replace('p.version!=1','p.version!=2').replace('case NpcAttackMessage::Ack:return','case NpcAttackMessage::Terminal:case NpcAttackMessage::Ack:return');p.write_text(s)
p=S/'npc_attack_protocol_tests.h';s=p.read_text().replace('case 14:q.version=2','case 14:q.version=3');s=s.replace('    ack.offset[0]=.1f;', '''    auto terminal=ack;terminal.kind=NpcAttackMessage::Terminal;
    require(validNpcAttack(terminal,h,c),"client terminal outcome names the consumed native contact");
    terminal.event=0;require(!validNpcAttack(terminal,h,c),"unbound terminal outcome refused");
    terminal=ack;terminal.kind=NpcAttackMessage::Terminal;terminal.direction[0]=1;
    require(!validNpcAttack(terminal,h,c),"terminal outcome cannot carry extra damage geometry");
    terminal=p;terminal.kind=NpcAttackMessage::Terminal;for(unsigned i=0;i<3;++i)terminal.offset[i]=terminal.direction[i]=0;
    require(!validNpcAttack(terminal,c,h),"host cannot report remote-player terminal outcome");
    ack.offset[0]=.1f;''');p.write_text(s)
p=S/'npc_attack.h';s=p.read_text().replace('bool host=false;','bool host=false,terminal=false;')
s=s.replace('NpcAttackRuntime npcAttack{};', '''struct NpcAttackSession {bool enabled=false,waitingCheckpoint=false,terminal=false;DoorSessionScope scope{};ULONGLONG deadline=0;std::uint64_t renewals=0;};
NpcAttackSession npcAttackSession{};
NpcAttackRuntime npcAttack{};''')
s=s.replace('npcAttackReplicaBlocked{0};','npcAttackReplicaBlocked{0},npcAttackTerminalBlocked{0};')
a=s.index('void stopNpcAttack(const char* reason){');b=s.index('\nbool routeNpcAttackDamage',a)
s=s[:a]+'''void notifyNpcAttackStop();
void stopNpcAttack(const char* reason){
    clearNpcAttackCapture();
    const bool terminal=!strcmp(reason,"native_player_death")||!strcmp(reason,"peer_player_death");
    const bool retain=npcAttackSession.enabled&&(terminal||!strcmp(reason,"checkpoint_barrier")||!strcmp(reason,"shared_checkpoint")||!strcmp(reason,"native_checkpoint_command"));
    if(!retain&&npcAttack.enabled&&strcmp(reason,"peer_stopped"))notifyNpcAttackStop();
    npcAttack.enabled=false;npcAttack.result=reason;
    for(auto& b:npcAttack.bindings)b.ready=false;
    npcAttack.pending.clear();
    if(retain){
        if(!npcAttackSession.waitingCheckpoint)npcAttackSession.deadline=GetTickCount64()+60000;
        npcAttackSession.waitingCheckpoint=true;npcAttackSession.terminal|=terminal;
    }else npcAttackSession.enabled=npcAttackSession.waitingCheckpoint=npcAttackSession.terminal=false;
}
''' +s[b:]
s=s.replace('if(s.host){','if(s.host&&!s.terminal){').replace('}else ++npcAttackReplicaBlocked;','}else if(s.terminal)++npcAttackTerminalBlocked;else ++npcAttackReplicaBlocked;')
a=s.index('void beginNpcAttack(')
s=s[:a]+'''void notifyNpcAttackStop(){
    if(GetCurrentThreadId()!=engineThread.load())return;
    for(auto& b:npcAttack.bindings)if(b.ready&&b.peerNonce){auto* c=npcCombatActor(b.arm);if(c&&c->enabled){sendNpcAttack(npcAttackPacket(*c,b,NpcAttackMessage::Stop),*c);break;}}
}
''' +s[a:]
s=s.replace('npcAttackReplicaBlocked.store(0);','npcAttackReplicaBlocked.store(0);npcAttackTerminalBlocked.store(0);')
a='    if(!b->ready){++npcAttack.rejected;return;}';assert a in s
s=s.replace(a,a+'''
    if(p.kind==NpcAttackMessage::Terminal){
        if(localRole||!p.event||p.event!=npcAttack.acked){++npcAttack.rejected;return;}
        stopNpcAttack("peer_player_death");return;
    }''')
a='    if(!sendNpcAttack(ack,*c))stopNpcAttack("ack_failed");';assert a in s
s=s.replace(a,'''    if(!sendNpcAttack(ack,*c)){stopNpcAttack("ack_failed");return;}
    if(npcAttack.after==0&&npcAttack.applied){
        auto terminal=npcAttackPacket(*c,*b,NpcAttackMessage::Terminal);terminal.event=p.event;
        if(!sendNpcAttack(terminal,*c)){stopNpcAttack("terminal_send_failed");return;}
        stopNpcAttack("native_player_death");
    }''')
a=s.index('    NpcAttackCaptureScope next{};');b=s.index('\n}\nvoid repeatNpcAttack()',a)
capture=s[a:b]
capture=capture.replace('    if(ready){','    {').replace('next.host=!localRole;next.until=tick+250;','next.host=!localRole;next.terminal=terminal;next.until=tick+250;')
s=s[:a]+'    if(ready)publishNpcAttackCapture(tick,false);'+s[b:]
a=s.index('void serviceNpcAttack(')
s=s[:a]+'''void publishNpcAttackCapture(ULONGLONG tick,bool terminal){
'''+capture+'''
}
''' +s[a:]
a='    if(npcAttackRequest>=0){const auto q=npcAttackRequest;npcAttackRequest=-1;if(q)beginNpcAttack(tick);else stopNpcAttack("disabled");}\n    if(!npcAttack.enabled)return;'
assert a in s
s=s.replace(a,'''    if(npcAttackRequest>=0){
        const auto q=npcAttackRequest;npcAttackRequest=-1;
        if(!q){stopNpcAttack("disabled");return;}
        if(!npcAttackSession.enabled){
            beginNpcAttack(tick);
            if(npcAttack.enabled){npcAttackSession={};npcAttackSession.enabled=true;npcAttackSession.scope=doorSessionScope(outgoing,remote);}
        }
    }
    if(!npcAttack.enabled){
        auto& intent=npcAttackSession;
        if(!intent.enabled||!intent.waitingCheckpoint)return;
        if(!sockets||!connection||connectionState!=k_ESteamNetworkingConnectionState_Connected||
           outgoing.session!=intent.scope.session||remote.session!=intent.scope.peerSession||outgoing.mapId!=intent.scope.map||remote.mapId!=intent.scope.map||
           !npcStreamSessionState.enabled||tick>intent.deadline){stopNpcAttack("recovery_scope_expired");return;}
        if(intent.terminal&&!localRole&&npcStreamContext(tick)&&doorSessionWorldMatches(intent.scope,outgoing,remote))publishNpcAttackCapture(tick,true);
        if(!doorSessionRenewalReady(intent.scope,outgoing,remote)||npcStreamSessionState.reloadPending||!npcStreamContext(tick)||
           !probeFollow||!avatarAiHeld||npcChannels.empty()||
           !std::all_of(npcChannels.begin(),npcChannels.end(),[](const auto& c){return c.enabled&&c.ready&&c.peerArm;}))return;
        beginNpcAttack(tick);
        if(npcAttack.enabled){intent.scope=doorSessionScope(outgoing,remote);intent.waitingCheckpoint=intent.terminal=false;++intent.renewals;}
        else stopNpcAttack("renewal_refused");
        return;
    }''')
# Append session information without disturbing the existing diagnostic fields.
a='    fputs("]}\\n",f);const bool ok=!ferror(f);';assert a in s
s=s.replace(a,'''    fprintf(f,"],\\"session\\":{\\"enabled\\":%s,\\"waitingCheckpoint\\":%s,\\"terminal\\":%s,\\"renewals\\":%llu,\\"terminalBlocked\\":%llu}}\\n",npcAttackSession.enabled?"true":"false",npcAttackSession.waitingCheckpoint?"true":"false",npcAttackSession.terminal?"true":"false",npcAttackSession.renewals,npcAttackTerminalBlocked.load());const bool ok=!ferror(f);''')
p.write_text(s)
for name in ('Build-NpcAttack','Deploy-NpcAttack'):
    t=(W/f'{name}-v988.ps1').read_text().replace('v988','v992').replace('Build988','Build992').replace('Deploy988','Deploy992');(W/f'{name}-v992.ps1').write_text(t)
t=(W/'stage_npc_attack_v988.py').read_text().replace('v988','v992').replace("P=W/'candidate-npc-combat-v983'","P=W/'candidate-npc-combat-v988'").replace("scope='Retains983", "scope='Experimental992: native terminal outcome, peer stop and automatic attack intent renewal after coordinated checkpoint. Runtime pending. Retains983")
(W/'stage_npc_attack_v992.py').write_text(t)
print('Prepared992;988 preserved; build and runtime validation pending')
