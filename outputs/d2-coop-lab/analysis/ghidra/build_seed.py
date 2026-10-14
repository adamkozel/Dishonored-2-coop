"""Reproducible, evidence-labelled import of the co-op project's native findings."""
import hashlib
import json
from pathlib import Path

HERE = Path(__file__).resolve().parent
LAB = HERE.parents[1]
functions = [
 ('ff8f0','D2_CommandBuffer_Execute','confirmed-runtime','engine_bridge.cpp','Engine-thread command pump; exact original vtable slot checked before hook.'),
 ('48ad0','D2_Common_ScreenUpdate','confirmed-runtime','engine_bridge.cpp','Six integer-class arguments retained; loading and native UI transport pump.'),
 ('753c10','D2_ReferenceFrame_Update','confirmed-runtime','native_child_frames.h','Recursive direct-child frame update. Suppressing the owned root must still update native child weapon frames; milestone 95B4.'),
 ('540f20','D2_Animation_Evaluate','confirmed-runtime','engine_bridge.cpp','Hook runs native evaluation before bounded body/arm retargeting. Preserve unmapped native joints.'),
 ('9cfa0','D2_Animation_SetJointLocal','confirmed-runtime','arm_probe.h','Used for validated native joint rotations; preserve current local translations.'),
 ('9dc40','D2_Animation_RebuildPose','confirmed-runtime','arm_probe.h','Native rebuild after controlled local joint updates.'),
 ('23cb00','D2_Animation_GetJointName','confirmed-static','runtime_probe.cpp','Reconstructed read-only: intern manager RVA22995c0, entries+8, count+10, stride72, string pointer+16. IDs are process-local.'),
 ('1d9090','D2_InternedName_Resolve','confirmed-static','runtime_probe.cpp','Tail target used by joint name accessor; distinguish intern IDs from stable semantic bone names.'),
 ('717950','D2_Health_ApplyDamage','confirmed-runtime','damage_observer.h','Native damage entry, called once by observer hook; bounded event capture. Do not infer arbitrary damage synchronization from one-hit tests.'),
 ('7185b0','D2_Health_SetCurrent','confirmed-runtime','engine_bridge.cpp','Native setter used to restore player HP and decrease owned guard HP. No arbitrary increase in guard stream.'),
 ('717d50','D2_Health_GetRatio','confirmed-static','native_health.h','Reads component+20 and divides by virtual maximum; zero HP alone does not establish game-over.'),
 ('7197f0','D2_Health_GetMaximum','confirmed-static','native_health.h','Maximum from declaration, suppression flag and bounded attribute entries.'),
 ('71a9e0','D2_Health_ApplyLethal','confirmed-runtime','engine_bridge.cpp','Lethal native health path; natural fall/sword deaths tested separately.'),
 ('780260','D2_DamageParams_Construct','confirmed-runtime','enemy_melee.h','Observed parameter storage144bytes. Partial knowledge; not a recovered full class.'),
 ('780470','D2_DamageResult_Construct','confirmed-runtime','enemy_melee.h','Observed result storage32bytes. Do not assume all semantic fields known.'),
 ('614b20','D2_Inventory_Equip','confirmed-runtime','avatar_sword.h','Native inventory, existing equipment entry, bool true. Guard and authored Corvo tutorial sword; no Emily counterpart equipment yet.'),
 ('619640','D2_Inventory_Unequip','confirmed-runtime','avatar_sword.h','Native existing-entry unequip. Cleanup defers during pause/death; retires on world replacement.'),
 ('1599460','D2_Inventory_Reflection','confirmed-static','native_sword_equipment.h','Items+38 stride56, equipment pointers+58, sixteen slots+68.'),
 ('7e7800','D2_Component_Pause','confirmed-runtime','engine_bridge.cpp','Acquire one owned pause reference only; bounded identity checks.'),
 ('7e79c0','D2_Component_Resume','confirmed-runtime','engine_bridge.cpp','Release only our reference; never resume a retired stopped component after native death.'),
 ('87d200','D2_Entity_SetTransform','confirmed-runtime','engine_bridge.cpp','Native actor transform setter; separate root and child-frame update responsibilities.'),
 ('8a78f0','D2_Game_SpawnActor','confirmed-runtime','engine_bridge.cpp','Counterpart spawn uses existing declarations and native spawner registration.'),
 ('8a7650','D2_Game_SwitchCharacter','confirmed-runtime','native_character.h','Corvo/Emily restoration restricted to native loading15, followed by entity maintenance.'),
 ('8ab380','D2_Game_MaintainEntities','confirmed-runtime','native_character.h','Used at controlled save/load character boundary.'),
 ('4f2980','D2_GameOver_Retry','confirmed-runtime','native_gameover.h','Native retry; shared death13 screens precede one coordinated reload.'),
 ('78a5b0','D2_Cinematic_SkipConfirmed','confirmed-runtime','engine_bridge.cpp','Native shared skip callback; opening test evidence does not cover full campaign.'),
 ('7d71f0','D2_Sequence_Pause','confirmed-runtime','engine_bridge.cpp','Native sequence instance pause used by bounded shared scene barrier.'),
 ('7d8950','D2_Sequence_Resume','confirmed-runtime','engine_bridge.cpp','Resume matching captured instance only.'),
 ('605990','D2_Faction_SetRelationship','confirmed-runtime','sword_link.h','Per-entity enemy relation2 for bounded partner-vs-guard experiments.'),
 ('6060a0','D2_Faction_RemoveRelationship','confirmed-runtime','sword_link.h','Release per-entity experimental relationship.'),
 ('16fec0','D2_Entity_GetChildren','confirmed-static','native_child_frames.h','Returns entity+B0 direct children container.'),
]
labels = [
 ('39a5290','D2_GameSingletonPtr'),('22995c0','D2_InternedNameManagerPtr'),('3990b60','D2_DeclarationManager'),
 ('1c940a8','D2_VT_Game'),('1c5e428','D2_VT_Player'),('1c5e108','D2_VT_NPC'),('1c87d58','D2_VT_Entity'),
 ('1c573e8','D2_VT_ReferenceFrame'),('1b80408','D2_VT_AnimationComponent'),('1b9a878','D2_VT_Inventory'),
 ('1c51588','D2_VT_Health'),('1c51698','D2_VT_HealthDeclaration'),('1b9cf48','D2_VT_NPCMeleeWeapon'),
 ('1b9d508','D2_VT_PlayerMeleeWeapon'),('1ce1e98','D2_VT_PlayerWeaponEntity'),('1cc4108','D2_VT_AIDecision'),
 ('1b8a588','D2_VT_AIReflex'),('1b93a88','D2_VT_AIAttack'),('1b6aab8','D2_VT_GameOverState'),
]
# Offsets are corroborated by guarded runtime readers. Length is minimum view,
# never a claim about full native sizeof. Unknown bytes stay undefined.
structures = [
 ('D2_ComponentPartial',0x20,[(0,'ptr','vtable'),(8,'ptr','declaration'),(0x10,'ptr','owner'),(0x1c,'u32','flags')],'native_ai_state.h'),
 ('D2_HealthPartial',0xb8,[(0,'ptr','vtable'),(8,'ptr','declaration'),(0x10,'ptr','owner'),(0x20,'float','currentHealth'),(0x68,'ptr','attributes'),(0xb0,'i32','maximumAttributeIndex'),(0xb4,'i32','suppressedAttributeIndex')],'native_health.h'),
 ('D2_InventoryEntryPartial',56,[(0,'ptr','itemDeclaration'),(8,'u32','weaponHandle'),(0x10,'ptr','equipmentComponent'),(0x18,'ptr','equipmentDeclaration'),(0x20,'i32','quantity'),(0x24,'i32','maximumQuantity')],'native_sword_equipment.h'),
 ('D2_InventoryPartial',0xe8,[(0,'ptr','vtable'),(8,'ptr','declaration'),(0x10,'ptr','owner'),(0x38,'ptr','items'),(0x40,'u32','itemCountFlags'),(0x58,'ptr','equipmentEntries'),(0x60,'u32','equipmentCountFlags'),(0x68,'ptr16','slots')],'native_sword_equipment.h'),
 ('D2_EntityPartial',0x360,[(0,'ptr','vtable'),(0x20,'ptr','name'),(0x78,'matrix12','cachedTransform'),(0xa8,'u32','handle'),(0xac,'u32','parentHandle'),(0xb0,'ptr','children'),(0xb8,'u32','childCountFlags'),(0x268,'ptr','components'),(0x270,'u32','componentCountFlags'),(0x2e8,'ptr','definition'),(0x300,'matrix12','rootTransform'),(0x358,'ptr','referenceFrame')],'native_player_arm.h; native_skeleton.h; native_child_frames.h'),
 ('D2_ReferenceFramePartial',0x3d2,[(0,'ptr','vtable'),(0x10,'ptr','owner'),(0x3c0,'ptr','attachmentTag'),(0x3c8,'ptr','attachmentParent'),(0x3d0,'u16','attachmentJoint')],'native_sword_equipment.h'),
 ('D2_AnimationComponentPartial',0x2f4,[(0,'ptr','vtable'),(0x10,'ptr','owner'),(0x38,'ptr','renderEntity'),(0x80,'ptr','skeleton'),(0x188,'ptr','localPose'),(0x190,'u32','localPoseCountFlags'),(0x198,'ptr','globalPose'),(0x1a0,'u32','globalPoseCountFlags'),(0x2d8,'ptr','matrixCache'),(0x2e0,'u32','matrixCountFlags'),(0x2e8,'ptr','phaseHeaders'),(0x2f0,'u32','phaseCountFlags')],'native_skeleton.h'),
]
manifest = {'executableSha256':'B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180','imageBase':'0x140000000','functions':[], 'labels':labels,'structures':structures,
 'hypotheses':[
  'Emily NPC may require an authored inventory component/item path to receive a native sword. No equipment creation validated.',
  'General remote-player attacks may be representable through native player-weapon components. Existing tests establish only one guard and bounded one-hit forwarding.',
  'All campaign transitions may be coordinated through known sequence/loading states. Evidence currently covers a small Dunwall area and opening scenes only.',
  'Native host/join and role selection UI integration is unresolved. Native SteamNetworkingSockets012 loopback is verified; separate Steam accounts/invites are not.'
 ]}
for rva,name,confidence,source,note in functions:
    p = LAB/'src'/source
    manifest['functions'].append(dict(rva='0x'+rva,name=name,confidence=confidence,source='src/'+source,sourceSha256=hashlib.sha256(p.read_bytes()).hexdigest() if p.exists() else None,note=note))
(HERE/'native-findings.json').write_text(json.dumps(manifest,indent=2)+'\n')
q=json.dumps
lines=['import ghidra.program.model.data.*;','import ghidra.program.model.listing.*;','import ghidra.program.model.symbol.*;','import ghidra.program.model.address.*;',
 'if (!currentProgram.getExecutableSHA256().equalsIgnoreCase('+q(manifest['executableSha256'])+')) throw new Exception("Wrong executable hash");',
 'if (currentProgram.getImageBase().getOffset()!=0x140000000L) throw new Exception("Wrong image base");',
 'int created=0;']
for f in manifest['functions']:
    lines += ['{ Address a=toAddr(0x140000000L+'+f['rva']+'L); disassemble(a); Function f=getFunctionAt(a); if(f==null)f=createFunction(a,'+q(f['name'])+'); if(f==null)throw new Exception("Function creation failed at "+a); f.setName('+q(f['name'])+',SourceType.USER_DEFINED); f.setComment('+q('['+f['confidence']+'] RVA '+f['rva']+'\n'+f['note']+'\nEvidence: '+f['source']+'\nNames are analyst-assigned semantic labels, not recovered official symbols.')+'); created++; }']
for rva,name in labels:
    lines += ['createLabel(toAddr(0x140000000L+0x'+rva+'L),'+q(name)+',true);']
lines+=['DataTypeManager dtm=currentProgram.getDataTypeManager(); CategoryPath category=new CategoryPath("/D2Coop/ConfirmedPartial");']
types={'ptr':'new PointerDataType(null,8,dtm)','ptr16':'new ArrayDataType(new PointerDataType(null,8,dtm),16,8)','matrix12':'new ArrayDataType(FloatDataType.dataType,12,4)','u32':'UnsignedIntegerDataType.dataType','i32':'IntegerDataType.dataType','u16':'UnsignedShortDataType.dataType','float':'FloatDataType.dataType'}
for name,size,fields,source in structures:
    lines += ['{ StructureDataType s=new StructureDataType(category,'+q(name)+','+str(size)+',dtm); s.setDescription('+q('Confirmed observed fields only; minimum view length, NOT full native sizeof. Evidence: src/'+source)+');']
    for off,t,n in fields: lines += ['s.replaceAtOffset('+str(off)+','+types[t]+',-1,'+q(n)+',"Observed and guarded in native reader; other fields unknown");']
    lines += ['dtm.addDataType(s,DataTypeConflictHandler.KEEP_HANDLER); }']
lines += ['setPlateComment(toAddr(0x140717d50L),'+q('Project scope: exact retail executable only. Confirmed findings have bounded static/runtime evidence; imported hypotheses are unverified. See analysis/ghidra/native-findings.json. No complete campaign, general combat, Emily equipment, or online session claim.')+');',
 'currentProgram.getOptions("D2Coop Research").setString("Unverified hypotheses",'+q('\n'.join(manifest['hypotheses']))+');',
 'currentProgram.getOptions("D2Coop Research").setString("Evidence policy","Analyst semantic labels. Confirmed runtime is limited to existing Dunwall tests; confirmed static is disassembly/reader reconstruction. Partial structures leave unknown bytes undefined and do not assert full sizeof.");',
 'println("D2_SEED_OK functions="+created+" structures=7 labels=19 hash="+currentProgram.getExecutableSHA256());']
(HERE/'seed-inline.java').write_text('\n'.join(lines)+'\n')
(HERE/'seed-request.json').write_text(json.dumps({'code':'\n'.join(lines),'program':'Dishonored2.exe'},indent=2)+'\n')
print(json.dumps({'functions':len(functions),'labels':len(labels),'structures':len(structures)}))
