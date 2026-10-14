from pathlib import Path
p=Path(__file__).resolve().parents[2]/'outputs/d2-coop-lab/src/shadow_stream.h';s=p.read_text()
def sub(a,b):
 global s
 assert s.count(a)==1,(a[:90],s.count(a));s=s.replace(a,b)
sub('CoopShadowActionPacket shadowStreamLastPacket{};', 'CoopShadowActionPacket shadowStreamLastPacket{};\nunsigned shadowStreamRejectReason=0,shadowStreamRetireReason=0;ULONGLONG shadowStreamRetireRootAge=0,shadowStreamRetireHelloAge=0;')
sub('''    if(!nativeMutationAllowed||!shadowStreamCurrent(tick)||!validShadowAction(p,outgoing,remote)||p.sequence<=shadowStream.lastSequence||
       (shadowStream.peerToken&&p.sourceToken!=shadowStream.peerToken)||(p.targetToken&&p.targetToken!=shadowStream.token)){
        ++shadowStreamRejected;return;
    }''','''    const unsigned reject=!nativeMutationAllowed?1:!shadowStreamCurrent(tick)?2:!validShadowAction(p,outgoing,remote)?3:p.sequence<=shadowStream.lastSequence?4:
        (shadowStream.peerToken&&p.sourceToken!=shadowStream.peerToken)?5:(p.targetToken&&p.targetToken!=shadowStream.token)?6:0;
    if(reject){shadowStreamRejectReason=reject;++shadowStreamRejected;return;}''')
sub('''       !shadowActionEventFresh(p,shadowStream.lastSequence,shadowStream.lastEvent,shadowStream.remoteState)){
        ++shadowStreamRejected;return;''','''       !shadowActionEventFresh(p,shadowStream.lastSequence,shadowStream.lastEvent,shadowStream.remoteState)){
        shadowStreamRejectReason=7;++shadowStreamRejected;return;''')
sub('''        stopShadowStream(tick>=shadowStream.until?"expired":"scope_or_peer_changed");return;''','''        shadowStreamRetireRootAge=tick-lastReceive;shadowStreamRetireHelloAge=tick-shadowStream.lastPeer;
        shadowStreamRetireReason=tick>=shadowStream.until?1:!shadowStreamPairReady(tick)?2:!shadowStreamCurrent(tick)?3:4;
        stopShadowStream(tick>=shadowStream.until?"expired":"scope_or_peer_changed");return;''')
mark='    fprintf(f,"{\\"armed\\":';at=s.index(mark)
s=s[:at]+'''    fprintf(f,"{\\"rejectReason\\":%u,\\"retireReason\\":%u,\\"retireRootAge\\":%llu,\\"retireHelloAge\\":%llu,\\"sourceGrenadeHeld\\":%u,\\"sourceGrenade\\":%u,\\"remoteGrenadeHeld\\":%u,\\"remoteGrenade\\":%u,",shadowStreamRejectReason,shadowStreamRetireReason,shadowStreamRetireRootAge,shadowStreamRetireHelloAge,shadowStream.sourceGrenadeHeld,shadowStream.sourceGrenade,shadowStream.remoteGrenadeHeld,shadowStream.remoteGrenade);
'''+s[at:];s=s.replace('fprintf(f,"{\\"armed\\":','fprintf(f,"\\"armed\\":',1);p.write_text(s)
print('Narrow stream rejection and retirement diagnostics added')
