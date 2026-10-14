
void FUN_140866160(longlong param_1)

{
  float fVar1;
  longlong lVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  undefined1 local_d0 [24];
  undefined1 local_b8 [32];
  undefined1 local_98 [144];
  
  uVar5 = 0xfffffffffffffffe;
  lVar2 = FUN_140854880();
  fVar1 = *(float *)(*(longlong *)(D2_GameSingletonPtr + 0x1e8948) + 0x3e0);
  if ((*(float *)(lVar2 + 0x308) <= fVar1 && fVar1 != *(float *)(lVar2 + 0x308)) &&
     (*(longlong *)(*(longlong *)(param_1 + 0x18) + 0x180) != 0)) {
    D2_DamageParams_Construct(local_98);
    uVar3 = FUN_140882e20(local_d0,local_98);
    uVar4 = FUN_140854880(param_1);
    FUN_140884920(uVar4,local_b8,&DAT_1439a9d20,uVar3,uVar5);
    FUN_140882fc0(local_b8);
  }
  return;
}


