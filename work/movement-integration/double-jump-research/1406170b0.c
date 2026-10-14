void FUN_1406170b0(longlong param_1,longlong *param_2,int param_3,int param_4)

{
  undefined4 uVar1;
  longlong lVar2;
  longlong *plVar3;
  longlong lVar4;
  undefined *puVar5;
  
  if (param_3 != param_4) {
    lVar4 = *param_2;
    uVar1 = *(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0xa8);
    lVar2 = FUN_140810240();
    plVar3 = (longlong *)FUN_1408112c0(lVar2 + 0xc8e8,0x1ad,0x10,uVar1);
    if (plVar3 != (longlong *)0x0) {
      *plVar3 = lVar4;
      plVar3[1] = CONCAT44(param_4,param_3);
    }
  }
  if ((param_3 == 0) && (0 < param_4)) {
    D2_Inventory_EnsureEquipmentEntity_Hypothesis(param_1,param_2,0);
    FUN_1406139f0(param_1,param_2);
  }
  else if (param_4 == 0) {
    D2_Inventory_NotifyItemDepleted_Hypothesis(param_1,param_2);
    goto LAB_14061716b;
  }
  if ((0 < param_4) &&
     (lVar4 = *(longlong *)(param_1 + 0x10), lVar2 = FUN_1408a8ba0(D2_GameSingletonPtr),
     lVar2 == lVar4)) {
    *(undefined1 *)((longlong)param_2 + 0x34) = 1;
  }
LAB_14061716b:
  lVar4 = FUN_1406dc560((int)param_2[1]);
  if (lVar4 != 0) {
    FUN_1406162b0(param_1,param_2,param_3,param_4);
  }
  for (puVar5 = (undefined *)(**(code **)(*(longlong *)*param_2 + 0xb8))();
      puVar5 != (undefined *)0x0; puVar5 = *(undefined **)(puVar5 + 0xa0)) {
    if (puVar5 == &DAT_14397edd0) {
      FUN_1406164b0(param_1,param_2,*param_2,param_3,param_4);
      return;
    }
  }
  lVar4 = *param_2;
  if (*(int *)(lVar4 + 0x60) == 0x12) {
    FUN_140616210(param_1,param_2,param_3,param_4);
    return;
  }
  if ((*(uint *)(lVar4 + 0x98) & 0xffffff) != 0) {
    FUN_140616940(param_1,lVar4 + 0x90,param_3,param_4);
  }
  return;
}
