
void FUN_1402548e0(longlong *param_1,undefined8 param_2)

{
  longlong lVar1;
  longlong lVar2;
  longlong lVar3;
  char cVar4;
  uint uVar5;
  int iVar6;
  uint uVar7;
  longlong *plVar8;
  longlong lVar9;
  longlong lVar10;
  ulonglong uVar11;
  longlong lVar12;
  longlong lVar13;
  longlong lVar14;
  longlong lStack_30;
  uint uStack_28;
  uint uStack_24;
  
  lStack_30 = 0;
  FUN_1401548e0();
  uStack_28 = 0x5b000000;
  uStack_24 = 0x3f;
  if (lStack_30 != 0) {
    FUN_140167b60(lStack_30,0);
  }
  lStack_30 = 0;
  uStack_24 = uStack_24 & 0xff;
  uStack_28 = uStack_28 & 0xff000000;
  FUN_140253b70(&lStack_30,param_2);
  uVar7 = uStack_28 & 0xffffff;
  if ((*(uint *)((longlong)param_1 + 0xc) & 0x40) == 0) {
    if (*param_1 != 0) {
      FUN_1409c6b70(*param_1,*(uint *)((longlong)param_1 + 0xc) >> 8);
    }
    *param_1 = 0;
    *(uint *)((longlong)param_1 + 0xc) = *(uint *)((longlong)param_1 + 0xc) & 0xff;
  }
  *(uint *)(param_1 + 1) = *(uint *)(param_1 + 1) & 0xff000000;
  if (*(uint *)((longlong)param_1 + 0xc) >> 8 < uVar7) {
    FUN_140255020(param_1,uVar7);
  }
  lVar14 = (longlong)(int)uVar7;
  if (uVar7 != 0) {
    lVar9 = 0;
    do {
      lVar3 = lStack_30;
      if (*(char *)(lVar9 + lStack_30) == '\0') {
        uVar7 = *(uint *)((longlong)param_1 + 0xc);
        uVar5 = uVar7 >> 8;
        if ((*(uint *)(param_1 + 1) & 0xffffff) == uVar5) {
          if (((byte)uVar7 & 0xc0) == 0xc0) {
            if (DAT_1431aee98 == '\0') {
              if ((uVar7 & 0x3f) == 0x3f) {
                if (uVar7 >> 8 == 0) {
                  iVar6 = 1;
                }
                else if (uVar5 < 0x40000000) {
                  iVar6 = uVar5 * 2;
                }
                else {
                  iVar6 = 0x7fffffff;
                }
              }
              else {
                iVar6 = 1 << ((byte)(uVar7 & 0x3f) & 0x1f);
                uVar5 = uVar5 + iVar6;
                iVar6 = uVar5 - (iVar6 - 1U & uVar5);
              }
            }
            else {
              iVar6 = uVar5 + 1;
            }
            FUN_140254e30(param_1,iVar6);
          }
          else {
            if (DAT_1431aee98 == '\0') {
              if ((uVar7 & 0x3f) == 0x3f) {
                if (uVar7 >> 8 == 0) {
                  iVar6 = 1;
                }
                else if (uVar5 < 0x40000000) {
                  iVar6 = uVar5 * 2;
                }
                else {
                  iVar6 = 0x7fffffff;
                }
              }
              else {
                iVar6 = 1 << ((byte)(uVar7 & 0x3f) & 0x1f);
                uVar5 = uVar5 + iVar6;
                iVar6 = uVar5 - (iVar6 - 1U & uVar5);
              }
            }
            else {
              iVar6 = uVar5 + 1;
            }
            FUN_140255020(param_1,iVar6);
          }
        }
        uVar5 = *(uint *)((longlong)param_1 + 0xc) >> 8;
        uVar7 = *(uint *)(param_1 + 1);
        if ((uVar7 & 0xffffff) < uVar5) {
          plVar8 = (longlong *)((ulonglong)(uVar7 & 0xffffff) * 0x20 + *param_1);
          *(uint *)(param_1 + 1) = (uVar7 + 1 ^ uVar7) & 0xffffff ^ uVar7;
        }
        else {
          plVar8 = (longlong *)((ulonglong)(uVar5 - 1) * 0x20 + *param_1);
        }
        plVar8[2] = *(longlong *)(lVar9 + 0x18 + lVar3);
        *(undefined4 *)(plVar8 + 3) = *(undefined4 *)(lVar9 + 0x20 + lVar3);
        uVar7 = *(uint *)(lVar9 + 0x10 + lVar3) & 0xffffff;
        if ((uVar7 <= *(uint *)((longlong)plVar8 + 0xc) >> 8) ||
           (cVar4 = FUN_140254ef0(plVar8,uVar7), cVar4 != '\0')) {
          uVar5 = *(uint *)((longlong)plVar8 + 0xc) >> 8;
          *(uint *)(plVar8 + 1) = *(uint *)(plVar8 + 1) & 0xff000000;
          if (uVar7 < uVar5) {
            uVar5 = uVar7;
          }
          *(uint *)(plVar8 + 1) = *(uint *)(plVar8 + 1) | uVar5;
        }
        lVar12 = 0;
        uVar11 = (ulonglong)(int)uVar7;
        if (3 < uVar11) {
          lVar10 = 0;
          lVar13 = (uVar11 - 4 >> 2) + 1;
          lVar12 = lVar13 * 4;
          do {
            lVar1 = *(longlong *)(lVar9 + 8 + lVar3);
            lVar2 = *plVar8;
            *(undefined4 *)(lVar10 + lVar2) = *(undefined4 *)(lVar10 + lVar1);
            *(undefined4 *)(lVar10 + 4 + lVar2) = *(undefined4 *)(lVar10 + 4 + lVar1);
            *(undefined4 *)(lVar10 + 8 + lVar2) = *(undefined4 *)(lVar10 + 8 + lVar1);
            *(bool *)(lVar10 + 0xc + lVar2) = (*(byte *)(lVar10 + 0xf + lVar1) & 0x60) == 0x20;
            *(undefined2 *)(lVar10 + 0xe + lVar2) = *(undefined2 *)(lVar10 + 0xc + lVar1);
            lVar1 = *(longlong *)(lVar9 + 8 + lVar3);
            lVar2 = *plVar8;
            *(undefined4 *)(lVar10 + 0x10 + lVar2) = *(undefined4 *)(lVar10 + 0x10 + lVar1);
            *(undefined4 *)(lVar10 + 0x14 + lVar2) = *(undefined4 *)(lVar10 + 0x14 + lVar1);
            *(undefined4 *)(lVar10 + 0x18 + lVar2) = *(undefined4 *)(lVar10 + 0x18 + lVar1);
            *(bool *)(lVar10 + 0x1c + lVar2) = (*(byte *)(lVar10 + 0x1f + lVar1) & 0x60) == 0x20;
            *(undefined2 *)(lVar10 + 0x1e + lVar2) = *(undefined2 *)(lVar10 + 0x1c + lVar1);
            lVar1 = *(longlong *)(lVar9 + 8 + lVar3);
            lVar2 = *plVar8;
            *(undefined4 *)(lVar10 + 0x20 + lVar2) = *(undefined4 *)(lVar10 + 0x20 + lVar1);
            *(undefined4 *)(lVar10 + 0x24 + lVar2) = *(undefined4 *)(lVar10 + 0x24 + lVar1);
            *(undefined4 *)(lVar10 + 0x28 + lVar2) = *(undefined4 *)(lVar10 + 0x28 + lVar1);
            *(bool *)(lVar10 + 0x2c + lVar2) = (*(byte *)(lVar10 + 0x2f + lVar1) & 0x60) == 0x20;
            *(undefined2 *)(lVar10 + 0x2e + lVar2) = *(undefined2 *)(lVar10 + 0x2c + lVar1);
            lVar1 = *(longlong *)(lVar9 + 8 + lVar3);
            lVar2 = *plVar8;
            *(undefined4 *)(lVar10 + 0x30 + lVar2) = *(undefined4 *)(lVar10 + 0x30 + lVar1);
            *(undefined4 *)(lVar10 + 0x34 + lVar2) = *(undefined4 *)(lVar10 + 0x34 + lVar1);
            *(undefined4 *)(lVar10 + 0x38 + lVar2) = *(undefined4 *)(lVar10 + 0x38 + lVar1);
            *(bool *)(lVar10 + 0x3c + lVar2) = (*(byte *)(lVar10 + 0x3f + lVar1) & 0x60) == 0x20;
            *(undefined2 *)(lVar10 + 0x3e + lVar2) = *(undefined2 *)(lVar10 + 0x3c + lVar1);
            lVar10 = lVar10 + 0x40;
            lVar13 = lVar13 + -1;
          } while (lVar13 != 0);
        }
        if (lVar12 < (longlong)uVar11) {
          lVar10 = lVar12 << 4;
          lVar12 = uVar11 - lVar12;
          do {
            lVar13 = *(longlong *)(lVar9 + 8 + lVar3);
            lVar1 = *plVar8;
            *(undefined4 *)(lVar10 + lVar1) = *(undefined4 *)(lVar10 + lVar13);
            *(undefined4 *)(lVar10 + 4 + lVar1) = *(undefined4 *)(lVar10 + 4 + lVar13);
            *(undefined4 *)(lVar10 + 8 + lVar1) = *(undefined4 *)(lVar10 + 8 + lVar13);
            *(bool *)(lVar10 + 0xc + lVar1) = (*(byte *)(lVar10 + 0xf + lVar13) & 0x60) == 0x20;
            *(undefined2 *)(lVar10 + 0xe + lVar1) = *(undefined2 *)(lVar10 + 0xc + lVar13);
            lVar10 = lVar10 + 0x10;
            lVar12 = lVar12 + -1;
          } while (lVar12 != 0);
        }
      }
      lVar9 = lVar9 + 0x28;
      lVar14 = lVar14 + -1;
    } while (lVar14 != 0);
  }
  if (((uStack_24 & 0x40) == 0) && (lStack_30 != 0)) {
    FUN_140167b60(lStack_30,uStack_24 >> 8);
  }
  return;
}


