# native-shadowwalk-1401bd910-decompile-v536.txt

Original: [work/movement-integration/native-shadowwalk-1401bd910-decompile-v536.txt](<../../../../work/movement-integration/native-shadowwalk-1401bd910-decompile-v536.txt>)

SHA-256: `8B1C64481E32ABC771E92B478CB967CFD20AF4AD0EAA23C756CB9B10F619EA00`; 3543 bytes.

## Original saved record

````text

longlong FUN_1401bd910(longlong param_1,longlong param_2)

{
  longlong lVar1;
  longlong lVar2;
  longlong *plVar3;
  undefined8 uVar4;
  ulonglong uVar5;
  ulonglong *puVar6;
  int iVar7;
  ulonglong uVar8;
  ulonglong uVar9;
  uint uVar10;
  ulonglong uVar11;
  ulonglong uVar12;
  ulonglong uVar13;
  uint uVar14;
  ulonglong uStackX_8;
  longlong lStack_30;
  undefined4 uStack_28;
  uint uStack_24;
  
  lVar1 = *(longlong *)(param_1 + 0x1c40);
  uVar9 = 0;
  if ((*(byte *)(lVar1 + 0x9c) & 0x40) == 0) {
    lVar2 = *(longlong *)(lVar1 + 0x90);
    if (lVar2 != 0) {
      plVar3 = (longlong *)FUN_1401548e0();
      (**(code **)(*plVar3 + 0x10))
                (plVar3,
                 "c:\\users\\zmiarchiving\\desktop\\dh2\\dishonored2\\voidengine\\core\\idlib\\containers/List.h"
                 ,0x1ec,lVar2,0x10);
    }
    *(longlong *)(lVar1 + 0x90) = 0;
    *(uint *)(lVar1 + 0x9c) = *(uint *)(lVar1 + 0x9c) & 0xff;
  }
  *(uint *)(lVar1 + 0x98) = *(uint *)(lVar1 + 0x98) & 0xff000000;
  if ((*(uint *)(param_2 + 8) & 0xffffff) == 0) {
    uVar4 = FUN_1401bdd60(param_1);
    FUN_1400af430(*(longlong *)(param_1 + 0x1c40) + 0x90,uVar4);
  }
  else {
    if ((*(uint *)(*(longlong *)(param_1 + 0x1c40) + 0x68) & 0xffffff) == 0) {
      lStack_30 = 0;
      FUN_1401548e0();
      lStack_30 = 0;
      uStack_24 = 0x3f;
      uStack_28 = 0x5000000;
      FUN_1401bd420(param_1,0,param_2,&lStack_30);
      lVar1 = lStack_30;
      if (((uStack_24 & 0x40) == 0) && (lStack_30 != 0)) {
        plVar3 = (longlong *)FUN_1401548e0();
        (**(code **)(*plVar3 + 0x10))
                  (plVar3,
                   "c:\\users\\zmiarchiving\\desktop\\dh2\\dishonored2\\voidengine\\core\\idlib\\containers/List.h"
                   ,0x1ec,lVar1,0x10);
      }
    }
    uVar10 = *(uint *)(*(longlong *)(param_1 + 0x1c40) + 0x68) & 0xffffff;
    uVar11 = (ulonglong)uVar10;
    uVar12 = uVar9;
    if (uVar10 != 0) {
      do {
        lVar1 = *(longlong *)(*(longlong *)(*(longlong *)(param_1 + 0x1c40) + 0x60) + 8 + uVar12);
        uVar5 = uVar9;
        if (lVar1 != 0) {
          uVar5 = FUN_14001e030(&DAT_142434350,lVar1,0,0);
        }
        lVar1 = *(longlong *)(param_1 + 0x1c40);
        iVar7 = (*(uint *)(lVar1 + 0x98) & 0xffffff) - 1;
        uStackX_8 = uVar5;
        if (-1 < iVar7) {
          puVar6 = *(ulonglong **)(lVar1 + 0x90);
          uVar8 = uVar9;
          uVar13 = uVar9;
          do {
            if (*puVar6 == uVar5) {
              if (-1 < (int)uVar13) goto LAB_1401bdb0c;
              break;
            }
            uVar13 = (ulonglong)((int)uVar13 + 1);
            uVar8 = uVar8 + 1;
            puVar6 = puVar6 + 1;
          } while ((longlong)uVar8 <= (longlong)iVar7);
        }
        FUN_1408918f0(lVar1 + 0x90,&uStackX_8);
LAB_1401bdb0c:
        uVar11 = uVar11 - 1;
        uVar12 = uVar12 + 0x10;
      } while (uVar11 != 0);
    }
    uVar10 = *(uint *)(*(longlong *)(param_1 + 0x1c40) + 0x98);
    uVar14 = *(uint *)(*(longlong *)(param_1 + 0x1c40) + 0x78) & 0xffffff;
    if (uVar14 != 0) {
      do {
        if ((longlong)(ulonglong)(uVar10 & 0xffffff) <= (longlong)uVar9) break;
        lVar1 = *(longlong *)(*(longlong *)(*(longlong *)(param_1 + 0x1c40) + 0x70) + uVar9 * 8);
        if (lVar1 != 0) {
          *(longlong *)(*(longlong *)(*(longlong *)(param_1 + 0x1c40) + 0x90) + uVar9 * 8) = lVar1;
        }
        uVar9 = uVar9 + 1;
      } while ((longlong)uVar9 < (longlong)(ulonglong)uVar14);
    }
  }
  return *(longlong *)(param_1 + 0x1c40) + 0x90;
}



````
