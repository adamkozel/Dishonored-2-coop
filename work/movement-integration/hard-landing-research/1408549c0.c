
int * FUN_1408549c0(longlong *param_1,int *param_2,undefined8 param_3,undefined8 param_4,
                   float param_5)

{
  int *piVar1;
  longlong lVar2;
  char cVar3;
  int *piVar4;
  int local_res8 [2];
  undefined4 local_res10 [2];
  undefined4 uVar5;
  
  lVar2 = D2_GameSingletonPtr;
  uVar5 = 0;
  piVar4 = (int *)FUN_1407d6040(D2_GameSingletonPtr + 0x1f5338,local_res8,param_3,param_4,
                                &DAT_14398f888,0,0);
  piVar1 = (int *)((longlong)param_1 + 0x24);
  if (piVar1 != piVar4) {
    *piVar1 = *piVar4;
  }
  if (*piVar1 != 0) {
    (**(code **)(*param_1 + 0x88))(param_1);
    local_res8[0] = (int)(param_5 * 1000.0);
    local_res10[0] = 0;
    cVar3 = FUN_1407d9270(lVar2 + 0x1f5338,*piVar1,local_res10,local_res8,0,CONCAT44(uVar5,0x100002)
                         );
    if ((cVar3 == '\0') && (piVar1 != local_res8)) {
      *piVar1 = 0;
    }
  }
  (**(code **)(*param_1 + 0x80))(param_1,param_3);
  *param_2 = *piVar1;
  return param_2;
}


