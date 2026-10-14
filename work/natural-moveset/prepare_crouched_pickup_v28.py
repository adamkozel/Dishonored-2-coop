"""Append a reversible, version-gated crouched pickup experiment."""
from pathlib import Path
P=Path(__file__).resolve().parents[2];S=P/'outputs/d2-coop-lab/src'
def edit(name, changes):
 p=S/name;t=p.read_text()
 for old,new in changes:
  assert t.count(old)==1,(name,old,t.count(old));t=t.replace(old,new)
 p.write_text(t)
edit('movement_bank.h',[
 ('32+232*','32+233*'),
 ('bool hasCarryPickupSettle()const{return version>=34;}','bool hasCarryPickupSettle()const{return version>=34;}\n    bool hasCrouchedCarryPickup()const{return version>=35;}'),
 ('231,232,232};return v<=34','231,232,232,233};return v<=35'),
 ('for(unsigned v=1;v<=34;++v)','for(unsigned v=1;v<=35;++v)')])
edit('movement_bank_tests.cpp',[
 ('(argc==1?1:34);format<=34','(argc==1?1:35);format<=35'),
 ('check(bank.hasCarryPickupSettle()', 'check(bank.hasCrouchedCarryPickup()==(bank.version>=35),"crouched hoist requires the new clip");\n        check(bank.hasCarryPickupSettle()')])
edit('movement_playback.h',[
 ('unsigned carryVariant=0;','unsigned carryVariant=0,carryStance=0;'),
 ('// v33 adds standing carrier gather/hoist only. Corpse choreography\n        // and crouched pickup remain separate, unaccepted work.', '// v35 adds a separate whole-body crouched hoist. Older banks retain\n        // their standing-only pickup behavior.'),
 ('const bool carryPickup=p.bank.hasCarryPickup()&&(carry.variant==7||carry.variant==1)&&ground&&carry.stance==0;', 'const bool carryPickupStance=(ground&&carry.stance==0)||(p.bank.hasCrouchedCarryPickup()&&crouched&&carry.stance==1);\n        const bool carryPickup=p.bank.hasCarryPickup()&&(carry.variant==7||carry.variant==1)&&carryPickupStance;'),
 ('if(!carryActive||!ground){p.carryFrom.clear();p.carrySettle=1;}', 'if(!carryActive||!carryPickupStance||p.carryStance!=carry.stance){p.carryFrom.clear();p.carrySettle=1;}'),
 ('p.carryVariant==1&&carry.variant==2&&ground&&carry.stance==0&&', 'p.carryVariant==1&&carry.variant==2&&carryPickupStance&&p.carryStance==carry.stance&&'),
 ('p.carryVariant=carry.variant;p.carryAmount=carryAmount;', 'p.carryVariant=carry.variant;p.carryStance=carry.stance;p.carryAmount=carryAmount;'),
 ('p.bank.sampleParameter(231,j,carryPickupAmount', 'p.bank.sampleParameter(p.bank.hasCrouchedCarryPickup()&&p.carryStance==1?232:231,j,carryPickupAmount'),
 ('fprintf(f,"\\\"carrySampleAmount', 'fprintf(f,"\\\"carryPlaybackStance\\\":%u,",p.carryStance);\n    fprintf(f,"\\\"carrySampleAmount')])
edit('carry_presentation.h',[
 ('float phase,float& amount){','float phase,float& amount,bool allowCrouched=false){'),
 ('if(stance)return false;', 'if(stance>1||(stance==1&&!allowCrouched))return false;')])
edit('carry_attachment_fixture.h',[
 ('unsigned dropStage=0,dropVariant=0;', 'unsigned dropStage=0,dropVariant=0;bool crouchedPickup=false;'),
 ('bool carryAttachmentPeerPickup(ULONGLONG tick){', 'bool carryAttachmentPeerPickup(ULONGLONG tick,unsigned* stance=nullptr){'),
 ('const bool carrier=movementPlayback.end>tick&&movementPlayback.lease>tick&&movementPlayback.bank.hasCarryPickup();', 'const bool carrier=movementPlayback.end>tick&&movementPlayback.lease>tick&&movementPlayback.bank.hasCarryPickup();\n    const bool crouched=movementPlayback.bank.hasCrouchedCarryPickup();'),
 ('return carrier&&shadowStream.carry.sample(at,sample,amount)&&sample.stance==0&&', 'if(!carrier||!shadowStream.carry.sample(at,sample,amount))return false;\n    if(stance)*stance=sample.stance;\n    return (sample.stance==0||(crouched&&sample.stance==1))&&'),
 ('unsigned char carryFlags=0;', 'unsigned char carryFlags=0;unsigned pickupStance=0;'),
 ('carryAttachmentPeerPickup(tick):', 'carryAttachmentPeerPickup(tick,&pickupStance):'),
 ('f.sourcePickup=carryAttachmentPickupRequested;\n                if(carryAttachmentSnapshotRequested&&!startCarrySnapshot(f.snapshot,victim,carrier))', 'f.sourcePickup=carryAttachmentPickupRequested;f.crouchedPickup=f.sourcePickup&&pickupStance==1;\n                // Exact-build native Carry45 crouched victim root is 0.674m\n                // lower in both archived role references. Bounded Mayhew\n                // hypothesis: use the same root for capture and placement,\n                // preserving the original floor pose at blend weight zero.\n                auto victimFrame=carrier;if(f.crouchedPickup)victimFrame.root[2]-=.674f;\n                if(carryAttachmentSnapshotRequested&&!startCarrySnapshot(f.snapshot,victim,victimFrame))'),
 ('float anchor[3]={carrier.root[0],carrier.root[1],carrier.root[2]};', 'float anchor[3]={victimFrame.root[0],victimFrame.root[1],victimFrame.root[2]};'),
 ('BodyMatrix desired=anchor;','const auto delta=bodyMultiply(bodyInverse(carryRootMatrix(carrier.root)),carryRootMatrix(victim.root));\n    if(f.crouchedPickup)anchor=bodyMultiply(delta,anchor);\n    BodyMatrix desired=anchor;'),
 ('const auto& b=victim.globalPose[abdomen-victim.jointIds.begin()];', 'BodyMatrix abdomenPose{};std::memcpy(abdomenPose.v,victim.globalPose[abdomen-victim.jointIds.begin()].data(),64);\n        if(f.crouchedPickup)abdomenPose=bodyMultiply(delta,abdomenPose);\n        const auto* b=abdomenPose.v;'),
 ('carryVictimPickupAmount(source.variant,source.stance,amount,pickupAmount)', 'carryVictimPickupAmount(source.variant,source.stance,amount,pickupAmount,f.crouchedPickup)')])
edit('carry_presentation_tests.h',[
 ('"loaded crouch may retain contact, but crouched pickup and release cannot masquerade as hoist");', '"legacy banks retain stance gating; release cannot masquerade as hoist");\n    check(carryVictimPickupAmount(7,1,.4f,victim,true)&&victim==0&&carryVictimPickupAmount(1,1,.44f,victim,true)&&std::fabs(victim-.5f)<1e-6f,"new crouched hoist uses the confirmed source timing");\n    check(!carryVictimPickupAmount(1,2,.4f,victim,true)&&!carryVictimPickupAmount(6,1,.4f,victim,true),"crouch permission does not allow invalid stance or drop as hoist");')])
print('CROUCHED_PICKUP_V28_SOURCE_PREPARED')
