# native-shadowwalk-1406612f0-decompile-v538.txt

Original: [work/movement-integration/native-shadowwalk-1406612f0-decompile-v538.txt](<../../../../work/movement-integration/native-shadowwalk-1406612f0-decompile-v538.txt>)

SHA-256: `0797155C763237A7518788DA555F647ECA74059D3EB21C61C6FC2080478EE1C0`; 6711 bytes.

## Original saved record

````text

void FUN_1406612f0(longlong param_1,longlong param_2,undefined4 param_3,char param_4,
                  longlong param_5)

{
  byte *pbVar1;
  longlong lVar2;
  longlong lVar3;
  uint uVar4;
  int iVar5;
  uint uVar6;
  longlong *plVar7;
  longlong lVar8;
  longlong lVar9;
  int iVar10;
  longlong lVar11;
  float fVar12;
  undefined8 local_res10;
  int local_68;
  undefined4 local_64;
  undefined4 local_60;
  undefined4 local_5c;
  int local_58 [4];
  longlong local_48;
  longlong local_40;
  
  lVar2 = *(longlong *)(param_2 + 0x28);
  lVar3 = *(longlong *)(lVar2 + 8);
  if (lVar3 == 0) {
    return;
  }
  local_res10 = lVar3;
  if ((param_4 != '\0') && (*(char *)(lVar2 + 0x10) != '\0')) {
    FUN_1408918f0(param_5,&local_res10);
    if (*(char *)(lVar2 + 0x11) == '\0') {
      return;
    }
    FUN_1408918f0(param_5 + 0x90,&local_res10);
    return;
  }
  plVar7 = *(longlong **)(param_2 + 0x30);
  lVar8 = 0;
  local_58[0] = DAT_1431af678;
  local_58[1] = DAT_1431af67c;
  local_40 = 0;
  local_58[2] = DAT_1431af680;
  local_48 = 0;
  iVar10 = 1;
  local_58[3] = DAT_1431af684;
  lVar11 = lVar8;
  if (plVar7 != (longlong *)0x0) {
    iVar5 = *(int *)((longlong)plVar7 + 0xc);
    if (iVar5 == 1) {
      local_58[0] = (**(code **)(*plVar7 + 0x68))(plVar7,param_3);
      lVar11 = 1;
    }
    else {
      if (iVar5 == 2) {
        (**(code **)(*plVar7 + 0x70))(plVar7,&local_res10,param_3);
        lVar11 = 2;
        local_58[0] = (int)local_res10;
        local_64 = local_res10._4_4_;
      }
      else {
        if (1 < iVar5 - 3U) {
          lVar11 = 0;
          if (iVar5 - 6U < 2) {
            (**(code **)(*plVar7 + 0x80))(plVar7,&local_68,param_3);
            lVar11 = 4;
            local_58[0] = local_68;
            local_58[1] = local_64;
            local_58[2] = local_60;
            local_58[3] = local_5c;
          }
          goto LAB_140661494;
        }
        (**(code **)(*plVar7 + 0x78))(plVar7,&local_68,param_3);
        lVar11 = 3;
        local_58[0] = local_68;
        local_58[2] = local_60;
      }
      local_58[1] = local_64;
    }
  }
LAB_140661494:
  iVar5 = (int)lVar11;
  lVar11 = (longlong)iVar5;
  uVar4 = *(uint *)(lVar3 + 0x4c) >> 4 & 3;
  if (uVar4 == 1) {
    if (iVar5 != 0) {
      do {
        fVar12 = (float)FUN_1411ad95c(local_58[lVar8]);
        lVar9 = lVar8 + 1;
        local_58[lVar8 + 4] = (int)fVar12;
        lVar8 = lVar9;
      } while (lVar9 < lVar11);
    }
  }
  else if (uVar4 == 2) {
    if (iVar5 != 0) {
      do {
        fVar12 = (float)FUN_1411ad95c(local_58[lVar8]);
        lVar9 = lVar8 + 1;
        local_58[lVar8 + 4] = (int)(longlong)fVar12;
        lVar8 = lVar9;
      } while (lVar9 < lVar11);
    }
  }
  else if ((uVar4 == 3) && (iVar5 != 0)) {
    FUN_1411ac5f0(&local_48,local_58,lVar11 << 2);
  }
  plVar7 = (longlong *)(param_5 + 0x120);
  uVar4 = *(uint *)(param_5 + 300);
  uVar6 = uVar4 >> 8;
  if ((*(uint *)(param_5 + 0x128) & 0xffffff) == uVar6) {
    if (((byte)uVar4 & 0xc0) == 0xc0) {
      if (DAT_1431aee98 == '\0') {
        if ((uVar4 & 0x3f) == 0x3f) {
          if (uVar4 >> 8 == 0) {
            FUN_14075ad30(plVar7,1);
          }
          else if (uVar6 < 0x40000000) {
            FUN_14075ad30(plVar7,uVar6 * 2);
          }
          else {
            FUN_14075ad30(plVar7,0x7fffffff);
          }
        }
        else {
          iVar5 = 1 << ((byte)(uVar4 & 0x3f) & 0x1f);
          uVar6 = uVar6 + iVar5;
          FUN_14075ad30(plVar7,uVar6 - (iVar5 - 1U & uVar6));
        }
      }
      else {
        FUN_14075ad30(plVar7,uVar6 + 1);
      }
    }
    else {
      if (DAT_1431aee98 == '\0') {
        if ((uVar4 & 0x3f) == 0x3f) {
          if (uVar4 >> 8 == 0) {
            iVar5 = 1;
          }
          else if (uVar6 < 0x40000000) {
            iVar5 = uVar6 * 2;
          }
          else {
            iVar5 = 0x7fffffff;
          }
        }
        else {
          iVar5 = 1 << ((byte)(uVar4 & 0x3f) & 0x1f);
          uVar6 = uVar6 + iVar5;
          iVar5 = uVar6 - (iVar5 - 1U & uVar6);
        }
      }
      else {
        iVar5 = uVar6 + 1;
      }
      FUN_140661830(plVar7,iVar5);
    }
  }
  lVar11 = local_40;
  lVar8 = local_48;
  uVar4 = *(uint *)(param_5 + 0x128);
  uVar6 = *(uint *)(param_5 + 300) >> 8;
  if ((uVar4 & 0xffffff) < uVar6) {
    plVar7 = (longlong *)(*plVar7 + (ulonglong)(uVar4 & 0xffffff) * 0x18);
    *(uint *)(param_5 + 0x128) = (uVar4 + 1 ^ uVar4) & 0xffffff ^ uVar4;
  }
  else {
    plVar7 = (longlong *)(*plVar7 + (ulonglong)(uVar6 - 1) * 0x18);
  }
  *plVar7 = lVar3;
  plVar7[1] = local_48;
  plVar7[2] = local_40;
  if (*(char *)(lVar2 + 0x11) == '\0') goto LAB_140661789;
  plVar7 = (longlong *)(param_5 + 0x2b0);
  uVar4 = *(uint *)(param_5 + 700);
  uVar6 = uVar4 >> 8;
  if ((*(uint *)(param_5 + 0x2b8) & 0xffffff) == uVar6) {
    if (((byte)uVar4 & 0xc0) == 0xc0) {
      if (DAT_1431aee98 == '\0') {
        if ((uVar4 & 0x3f) == 0x3f) {
          if (uVar4 >> 8 != 0) {
            if (uVar6 < 0x40000000) {
              FUN_14075ad30(plVar7,uVar6 * 2);
            }
            else {
              FUN_14075ad30(plVar7,0x7fffffff);
            }
            goto LAB_140661733;
          }
        }
        else {
          iVar10 = 1 << ((byte)(uVar4 & 0x3f) & 0x1f);
          uVar6 = iVar10 + uVar6;
          iVar10 = uVar6 - (iVar10 - 1U & uVar6);
        }
        FUN_14075ad30(plVar7,iVar10);
      }
      else {
        FUN_14075ad30(plVar7,uVar6 + 1);
      }
    }
    else {
      if (DAT_1431aee98 == '\0') {
        if ((uVar4 & 0x3f) == 0x3f) {
          if (uVar4 >> 8 != 0) {
            if (uVar6 < 0x40000000) {
              iVar10 = uVar6 * 2;
            }
            else {
              iVar10 = 0x7fffffff;
            }
          }
        }
        else {
          iVar10 = 1 << ((byte)(uVar4 & 0x3f) & 0x1f);
          uVar6 = iVar10 + uVar6;
          iVar10 = uVar6 - (iVar10 - 1U & uVar6);
        }
      }
      else {
        iVar10 = uVar6 + 1;
      }
      FUN_140661830(plVar7,iVar10);
    }
  }
LAB_140661733:
  uVar4 = *(uint *)(param_5 + 0x2b8);
  uVar6 = *(uint *)(param_5 + 700) >> 8;
  if ((uVar4 & 0xffffff) < uVar6) {
    plVar7 = (longlong *)(*plVar7 + (ulonglong)(uVar4 & 0xffffff) * 0x18);
    *(uint *)(param_5 + 0x2b8) = (uVar4 + 1 ^ uVar4) & 0xffffff ^ uVar4;
  }
  else {
    plVar7 = (longlong *)(*plVar7 + (ulonglong)(uVar6 - 1) * 0x18);
  }
  *plVar7 = lVar3;
  plVar7[1] = lVar8;
  plVar7[2] = lVar11;
LAB_140661789:
  if (*(char *)(lVar2 + 0x10) == '\0') {
    lVar2 = *(longlong *)(param_1 + 0x10);
    FUN_1400998b0(lVar2);
    pbVar1 = (byte *)(*(longlong *)(lVar2 + 0xd0) + 0xd4);
    *pbVar1 = *pbVar1 | 0x20;
  }
  return;
}



````
