from pathlib import Path
p=Path(__file__).resolve().parents[2]/'outputs/d2-coop-lab/src/shadow_stream.h';s=p.read_text()
s=s.replace('unsigned shadowStreamRejectReason=0,','unsigned shadowStreamStateRejectReason=0;double shadowStreamRejectedAge=0;\nunsigned shadowStreamRejectReason=0,',1)
s=s.replace('if(reject){shadowStreamRejectReason=reject;++shadowStreamRejected;return;}','if(reject){shadowStreamRejectReason=reject;if(p.kind==1&&reject!=2)shadowStreamStateRejectReason=reject;++shadowStreamRejected;return;}',1)
s=s.replace('shadowStreamRejectReason=7;++shadowStreamRejected;return;','shadowStreamStateRejectReason=shadowStreamRejectReason=7;++shadowStreamRejected;return;',1)
a=s.index('    if(sourcePoseTimeline.session!=p.sourceSession');b=s.index('    shadowStream.combat=std::move(combatTimeline);',a)
s=s[:a]+'''    const unsigned timelineReject=(sourcePoseTimeline.session!=p.sourceSession||sourcePoseTimeline.epoch!=p.sourceWorldEpoch||sourcePoseTimeline.map!=p.mapId)?10:
       (mapped>now+80.||now-mapped>250.)?11:
       !combatTimeline.push({mapped,p.event,p.combatVariant,p.combatPhase,p.combatTimeMs,p.combatDurationMs})?12:
       !equipmentTimeline.push({mapped,p.event,p.swordState,p.equipmentVariant,p.equipmentTimeMs,p.equipmentDurationMs})?13:
       !traversalTimeline.push({mapped,p.event,p.traversalVariant,p.traversalTimeMs,p.traversalDurationMs,p.traversalEndStance,p.traversalStart,p.traversalGoal,p.traversalPoint,p.traversalHeight})?14:
       !powerTimeline.push({mapped,p.event,p.powerVariant,p.powerTimeMs,p.powerDurationMs})?15:
       !rangedTimeline.push({mapped,p.event,p.rangedKind,p.rangedVariant,p.rangedTimeMs,p.rangedDurationMs,p.rangedMechanism,p.rangedShot})?16:
       !grenadeTimeline.push({mapped,p.event,p.grenadeHeld,p.grenadeVariant,p.grenadeTimeMs,p.grenadeDurationMs})?17:
       !carryTimeline.push({mapped,p.event,p.carryVariant,p.carryTimeMs,p.carryDurationMs,p.carryStance,p.carryIncarnation,p.carrySpawner,p.carryDefinition,p.carryThrowVelocity})?18:
       !movementTimeline.push({mapped,p.event,p.state,p.leanSide,p.leanUp,p.airStartZ,p.airTargetZ,p.airStage,p.airRemaining})?19:0;
    if(timelineReject){shadowStreamStateRejectReason=shadowStreamRejectReason=timelineReject;shadowStreamRejectedAge=now-mapped;++shadowStreamRejected;return;}
'''+s[b:]
a=s.index('    if(p.event==shadowStream.lastEvent&&');b=s.index('    NativeWorldNpc carryTarget',a);region=s[a:b].replace('++shadowStreamRejected;return;','shadowStreamStateRejectReason=8;++shadowStreamRejected;return;');s=s[:a]+region+s[b:]
mark='    fprintf(f,"{\\"rejectReason\\":';at=s.index(mark)
s=s[:at]+'''    fprintf(f,"{\\"stateRejectReason\\":%u,\\"rejectedAgeMs\\":%.5f,",shadowStreamStateRejectReason,shadowStreamRejectedAge);
'''+s[at:];s=s.replace('fprintf(f,"{\\"rejectReason\\":','fprintf(f,"\\"rejectReason\\":',1);p.write_text(s)
print('Per-timeline first-cause diagnostics retain rejected packet age')
