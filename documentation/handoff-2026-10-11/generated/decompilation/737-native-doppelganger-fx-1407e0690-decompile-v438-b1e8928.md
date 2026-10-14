# native-doppelganger-fx-1407e0690-decompile-v438.txt

Original: [work/movement-integration/candidate-midnight-fx-probe-v445/native-doppelganger-fx-1407e0690-decompile-v438.txt](<../../../../work/movement-integration/candidate-midnight-fx-probe-v445/native-doppelganger-fx-1407e0690-decompile-v438.txt>)

SHA-256: `88056005D87417BA761DDE52B906C29D4E026EE1505DBDFCC4E26A045620239E`; 3565 bytes.

## Original saved record

````text

void FUN_1407e0690(longlong param_1,undefined8 *param_2)

{
  longlong lVar1;
  longlong *plVar2;
  uint uVar3;
  ulonglong uVar5;
  undefined8 uVar6;
  longlong *plVar7;
  longlong lVar8;
  ulonglong uVar9;
  uint uVar10;
  int iVar11;
  longlong lVar12;
  uint local_res10 [4];
  int local_res20 [2];
  undefined4 local_58;
  undefined4 local_54;
  undefined4 local_50 [2];
  longlong local_48;
  undefined4 local_40;
  undefined4 local_3c;
  undefined4 local_38;
  undefined4 local_34;
  ulonglong uVar4;
  
  if (*(char *)(param_2 + 1) == '\0') {
    uVar9 = 0;
    lVar12 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0xc0);
    uVar10 = *(uint *)(lVar12 + 0x60) & 0xffffff;
    uVar5 = uVar9;
    if (uVar10 != 0) {
      plVar2 = *(longlong **)(lVar12 + 0x58);
      uVar4 = uVar9;
      plVar7 = plVar2;
      do {
        if (*(int *)(*plVar7 + 0x1c) == (int)*(short *)(param_1 + 0xb8)) {
          uVar5 = plVar2[(int)uVar4];
          break;
        }
        uVar3 = (int)uVar4 + 1;
        uVar4 = (ulonglong)uVar3;
        plVar7 = plVar7 + 1;
      } while ((int)uVar3 < (int)uVar10);
    }
    *(ulonglong *)(param_1 + 8) = uVar5;
    (**(code **)(*(longlong *)*param_2 + 0x28))((longlong *)*param_2,local_res20,4);
    if (0 < local_res20[0]) {
      do {
        local_res10[0] = 0;
        lVar12 = *(longlong *)*param_2;
        if (*(char *)(param_2 + 1) == '\0') {
          (**(code **)(lVar12 + 0x28))((longlong *)*param_2,local_res10,4);
        }
        else {
          (**(code **)(lVar12 + 0x30))();
        }
        (**(code **)(*(longlong *)*param_2 + 0x28))((longlong *)*param_2,&local_58,4);
        (**(code **)(*(longlong *)*param_2 + 0x28))((longlong *)*param_2,&local_54,4);
        (**(code **)(*(longlong *)*param_2 + 0x28))((longlong *)*param_2,local_50,4);
        uVar6 = FUN_1407dc000(*(undefined8 *)(param_1 + 0x10));
        lVar12 = FUN_140196780(uVar6,local_res10[0]);
        if (lVar12 == 0) {
          *(undefined1 *)((longlong)param_2 + 10) = 1;
        }
        else {
          local_40 = *(undefined4 *)(lVar12 + 8);
          local_3c = local_58;
          local_38 = local_54;
          local_34 = local_50[0];
          local_48 = lVar12;
          FUN_1407df5d0(param_1 + 0x18,&local_48);
        }
        uVar10 = (int)uVar9 + 1;
        uVar9 = (ulonglong)uVar10;
      } while ((int)uVar10 < local_res20[0]);
    }
  }
  else {
    local_res10[0] = *(uint *)(param_1 + 0x20) & 0xffffff;
    (**(code **)(*(longlong *)*param_2 + 0x30))((longlong *)*param_2,local_res10,4);
    iVar11 = 0;
    if ((*(uint *)(param_1 + 0x20) & 0xffffff) != 0) {
      lVar12 = 0;
      do {
        lVar8 = *(longlong *)(param_1 + 0x18) + lVar12;
        local_res10[0] = *(uint *)(lVar8 + 8);
        lVar1 = *(longlong *)*param_2;
        if (*(char *)(param_2 + 1) == '\0') {
          (**(code **)(lVar1 + 0x28))((longlong *)*param_2,local_res10,4);
        }
        else {
          (**(code **)(lVar1 + 0x30))();
        }
        local_res10[0] = *(uint *)(lVar8 + 0xc);
        (**(code **)(*(longlong *)*param_2 + 0x30))((longlong *)*param_2,local_res10,4);
        local_res10[0] = *(uint *)(lVar8 + 0x10);
        (**(code **)(*(longlong *)*param_2 + 0x30))((longlong *)*param_2,local_res10,4);
        local_res10[0] = *(uint *)(lVar8 + 0x14);
        (**(code **)(*(longlong *)*param_2 + 0x30))((longlong *)*param_2,local_res10,4);
        iVar11 = iVar11 + 1;
        lVar12 = lVar12 + 0x18;
      } while (iVar11 < (int)(*(uint *)(param_1 + 0x20) & 0xffffff));
      return;
    }
  }
  return;
}



````
