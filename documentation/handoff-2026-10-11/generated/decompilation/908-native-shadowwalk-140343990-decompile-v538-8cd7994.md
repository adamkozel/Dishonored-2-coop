# native-shadowwalk-140343990-decompile-v538.txt

Original: [work/movement-integration/native-shadowwalk-140343990-decompile-v538.txt](<../../../../work/movement-integration/native-shadowwalk-140343990-decompile-v538.txt>)

SHA-256: `69E1B8BB6765E5F9C7F8FE0F934C21D48779F9995FC6175668996DDB3B7EA9BC`; 4580 bytes.

## Original saved record

````text

longlong * FUN_140343990(longlong *param_1,longlong param_2,undefined8 *param_3)

{
  uint uVar1;
  int iVar2;
  ushort *puVar3;
  undefined8 *puVar4;
  uint uVar5;
  longlong lVar6;
  int iVar7;
  undefined2 *puVar8;
  
  iVar7 = (*(uint *)(param_1 + 3) & 0xffffff) - 1;
  lVar6 = (longlong)iVar7;
  if (-1 < iVar7) {
    puVar3 = (ushort *)(param_1[2] + lVar6 * 2);
    do {
      if ((uint)*puVar3 == *(uint *)(param_2 + 0x48)) {
        puVar8 = (undefined2 *)(param_1[2] + (longlong)iVar7 * 2);
        puVar4 = (undefined8 *)((longlong)iVar7 * 0x10 + *param_1);
        if (puVar8 != (undefined2 *)0x0) goto LAB_140343c52;
        break;
      }
      iVar7 = iVar7 + -1;
      puVar3 = puVar3 + -1;
      lVar6 = lVar6 + -1;
    } while (-1 < lVar6);
  }
  if (((*(uint *)((longlong)param_1 + 0x1c) & 0xc0) != 0) &&
     ((*(uint *)(param_1 + 3) & 0xffffff) == *(uint *)((longlong)param_1 + 0x1c) >> 8)) {
                    /* WARNING: Subroutine does not return */
    FUN_140166830(&DAT_141b2bee8);
  }
  uVar1 = *(uint *)((longlong)param_1 + 0x1c);
  uVar5 = uVar1 >> 8;
  iVar7 = 1;
  if ((*(uint *)(param_1 + 3) & 0xffffff) == uVar5) {
    if (((byte)uVar1 & 0xc0) == 0xc0) {
      if (DAT_1431aee98 == '\0') {
        if ((uVar1 & 0x3f) == 0x3f) {
          if (uVar1 >> 8 == 0) {
            FUN_140067d20(param_1 + 2,1);
          }
          else if (uVar5 < 0x40000000) {
            FUN_140067d20(param_1 + 2,uVar5 * 2);
          }
          else {
            FUN_140067d20(param_1 + 2,0x7fffffff);
          }
        }
        else {
          iVar2 = 1 << ((byte)(uVar1 & 0x3f) & 0x1f);
          uVar5 = uVar5 + iVar2;
          FUN_140067d20(param_1 + 2,uVar5 - (iVar2 - 1U & uVar5));
        }
      }
      else {
        FUN_140067d20(param_1 + 2,uVar5 + 1);
      }
    }
    else {
      if (DAT_1431aee98 == '\0') {
        if ((uVar1 & 0x3f) == 0x3f) {
          if (uVar1 >> 8 == 0) {
            iVar2 = 1;
          }
          else if (uVar5 < 0x40000000) {
            iVar2 = uVar5 * 2;
          }
          else {
            iVar2 = 0x7fffffff;
          }
        }
        else {
          iVar2 = 1 << ((byte)(uVar1 & 0x3f) & 0x1f);
          uVar5 = uVar5 + iVar2;
          iVar2 = uVar5 - (iVar2 - 1U & uVar5);
        }
      }
      else {
        iVar2 = uVar5 + 1;
      }
      FUN_140244f70(param_1 + 2,iVar2);
    }
  }
  uVar1 = *(uint *)(param_1 + 3);
  uVar5 = *(uint *)((longlong)param_1 + 0x1c) >> 8;
  if ((uVar1 & 0xffffff) < uVar5) {
    uVar5 = uVar1 & 0xffffff;
    *(uint *)(param_1 + 3) = (uVar1 + 1 ^ uVar1) & 0xffffff ^ uVar1;
  }
  else {
    uVar5 = uVar5 - 1;
  }
  puVar8 = (undefined2 *)(param_1[2] + (ulonglong)uVar5 * 2);
  uVar1 = *(uint *)((longlong)param_1 + 0xc);
  uVar5 = uVar1 >> 8;
  if ((*(uint *)(param_1 + 1) & 0xffffff) == uVar5) {
    if (((byte)uVar1 & 0xc0) == 0xc0) {
      if (DAT_1431aee98 == '\0') {
        if ((uVar1 & 0x3f) == 0x3f) {
          if (uVar1 >> 8 != 0) {
            if (uVar5 < 0x40000000) {
              FUN_1403435d0(param_1,uVar5 * 2);
            }
            else {
              FUN_1403435d0(param_1,0x7fffffff);
            }
            goto LAB_140343c03;
          }
        }
        else {
          iVar7 = 1 << ((byte)(uVar1 & 0x3f) & 0x1f);
          uVar5 = iVar7 + uVar5;
          iVar7 = uVar5 - (iVar7 - 1U & uVar5);
        }
        FUN_1403435d0(param_1,iVar7);
      }
      else {
        FUN_1403435d0(param_1,uVar5 + 1);
      }
    }
    else {
      if (DAT_1431aee98 == '\0') {
        if ((uVar1 & 0x3f) == 0x3f) {
          if (uVar1 >> 8 != 0) {
            if (uVar5 < 0x40000000) {
              iVar7 = uVar5 * 2;
            }
            else {
              iVar7 = 0x7fffffff;
            }
          }
        }
        else {
          iVar7 = 1 << ((byte)(uVar1 & 0x3f) & 0x1f);
          uVar5 = iVar7 + uVar5;
          iVar7 = uVar5 - (iVar7 - 1U & uVar5);
        }
      }
      else {
        iVar7 = uVar5 + 1;
      }
      FUN_140343810(param_1,iVar7);
    }
  }
LAB_140343c03:
  uVar1 = *(uint *)(param_1 + 1);
  uVar5 = *(uint *)((longlong)param_1 + 0xc) >> 8;
  if ((uVar1 & 0xffffff) < uVar5) {
    puVar4 = (undefined8 *)((ulonglong)(uVar1 & 0xffffff) * 0x10 + *param_1);
    *(uint *)(param_1 + 1) = (uVar1 + 1 ^ uVar1) & 0xffffff ^ uVar1;
  }
  else {
    puVar4 = (undefined8 *)((ulonglong)(uVar5 - 1) * 0x10 + *param_1);
  }
  *puVar8 = 0;
  *puVar4 = 0;
  puVar4[1] = 0;
LAB_140343c52:
  *puVar8 = *(undefined2 *)(param_2 + 0x48);
  *puVar4 = *param_3;
  puVar4[1] = param_3[1];
  return param_1;
}



````
