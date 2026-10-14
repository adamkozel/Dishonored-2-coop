# native-shadowwalk-1401bd420-decompile-v528.txt

Original: [work/movement-integration/native-shadowwalk-1401bd420-decompile-v528.txt](<../../../../work/movement-integration/native-shadowwalk-1401bd420-decompile-v528.txt>)

SHA-256: `25DA428F48F9D0DAE6308A23F5523FA15B8D2F3D9438A2AA6EB17D69282C27F2`; 7647 bytes.

## Original saved record

````text

void FUN_1401bd420(longlong param_1,int param_2,longlong *param_3,undefined8 param_4)

{
  int iVar1;
  char cVar2;
  int iVar3;
  longlong *plVar4;
  undefined8 uVar5;
  uint uVar6;
  uint uVar7;
  longlong lVar8;
  ulonglong uVar9;
  longlong lVar10;
  ulonglong uVar11;
  int *piVar12;
  longlong lVar13;
  uint uVar14;
  ulonglong uVar15;
  ulonglong uVar16;
  int *piVar17;
  undefined **local_60;
  undefined8 local_58;
  int local_50;
  int local_4c;
  undefined8 local_48;
  
  local_58 = 0xfffffffffffffffe;
  uVar14 = *(uint *)(param_3 + 1) & 0xffffff;
  if (uVar14 != 0) {
    lVar8 = *(longlong *)(param_1 + 0x1c40);
    uVar9 = 0;
    if (uVar14 == (*(uint *)(lVar8 + 0x78) & 0xffffff)) {
      if (uVar14 != 0) {
        plVar4 = (longlong *)*param_3;
        lVar13 = *(longlong *)(lVar8 + 0x70) - (longlong)plVar4;
        uVar15 = uVar9;
        do {
          if (*plVar4 != *(longlong *)(lVar13 + (longlong)plVar4)) goto LAB_1401bd4b7;
          uVar6 = (int)uVar15 + 1;
          uVar15 = (ulonglong)uVar6;
          plVar4 = plVar4 + 1;
        } while ((int)uVar6 < (int)uVar14);
      }
      lVar8 = (longlong)param_2 * 0x10 + *(longlong *)(lVar8 + 0x40);
    }
    else {
LAB_1401bd4b7:
      *(uint *)(lVar8 + 0x78) = *(uint *)(lVar8 + 0x78) & 0xff000000;
      FUN_1400af430(*(longlong *)(param_1 + 0x1c40) + 0x70,param_3);
      lVar8 = *(longlong *)(param_1 + 0x1c40);
      if ((*(byte *)(lVar8 + 0x6c) & 0x40) == 0) {
        lVar13 = *(longlong *)(lVar8 + 0x60);
        if (lVar13 != 0) {
          plVar4 = (longlong *)FUN_1401548e0();
          (**(code **)(*plVar4 + 0x10))
                    (plVar4,
                     "c:\\users\\zmiarchiving\\desktop\\dh2\\dishonored2\\voidengine\\core\\idlib\\containers/List.h"
                     ,0x1ec,lVar13,0x10);
        }
        *(undefined8 *)(lVar8 + 0x60) = 0;
        *(uint *)(lVar8 + 0x6c) = *(uint *)(lVar8 + 0x6c) & 0xff;
      }
      *(uint *)(lVar8 + 0x68) = *(uint *)(lVar8 + 0x68) & 0xff000000;
      lVar8 = *(longlong *)(param_1 + 0x1c40);
      lVar13 = *(longlong *)(lVar8 + 0x38);
      uVar14 = *(uint *)(lVar13 + 0x60) & 0xffffff;
      if ((uVar14 <= *(uint *)(lVar8 + 0x5c) >> 8) ||
         (cVar2 = FUN_140384fe0(lVar8 + 0x50,uVar14), cVar2 != '\0')) {
        uVar6 = *(uint *)(lVar8 + 0x5c) >> 8;
        *(uint *)(lVar8 + 0x58) = *(uint *)(lVar8 + 0x58) & 0xff000000;
        if (uVar14 < uVar6) {
          uVar6 = uVar14;
        }
        *(uint *)(lVar8 + 0x58) = *(uint *)(lVar8 + 0x58) | uVar6;
      }
      local_60 = (undefined **)(longlong)(int)(*(uint *)(lVar13 + 0x60) & 0xffffff);
      uVar15 = uVar9;
      uVar16 = uVar9;
      if (local_60 != (undefined **)0x0) {
        do {
          iVar3 = (int)uVar16;
          lVar10 = (longlong)iVar3 * 0x50 + *(longlong *)(lVar13 + 0x58);
          lVar8 = *(longlong *)PTR_PTR_142299f98;
          uVar5 = D2_InternedName_Resolve(lVar10);
          lVar8 = (**(code **)(lVar8 + 0x10))(PTR_PTR_142299f98,uVar5,1);
          if (lVar8 == 0) {
            *(undefined4 *)(*(longlong *)(*(longlong *)(param_1 + 0x1c40) + 0x50) + uVar15 * 4) = 0;
            uVar5 = D2_InternedName_Resolve(lVar10);
                    /* WARNING: Subroutine does not return */
            FUN_140166830("[%s] Model \"%s\" not found in blueprint \"%s\".\n",
                          "arkRenderModelAssembly::GetCustomMaterialsPerModel",uVar5,
                          *(undefined8 *)(param_1 + 0x1bb0));
          }
          *(uint *)(*(longlong *)(*(longlong *)(param_1 + 0x1c40) + 0x50) + uVar15 * 4) =
               *(uint *)(lVar8 + 0x88) & 0xffffff;
          uVar14 = *(uint *)(lVar8 + 0x88) & 0xffffff;
          lVar10 = (longlong)(int)uVar14;
          uVar16 = uVar9;
          uVar11 = uVar9;
          iVar1 = iVar3;
          if (uVar14 != 0) {
            do {
              local_50 = iVar1;
              local_48 = *(undefined8 *)(*(longlong *)(uVar16 + *(longlong *)(lVar8 + 0x80)) + 8);
              local_4c = (int)uVar11;
              FUN_1401bc8a0(*(longlong *)(param_1 + 0x1c40) + 0x60,&local_50);
              lVar10 = lVar10 + -1;
              uVar16 = uVar16 + 0x90;
              uVar11 = (ulonglong)((int)uVar11 + 1);
              iVar1 = local_50;
            } while (lVar10 != 0);
          }
          uVar16 = (ulonglong)(iVar3 + 1);
          uVar15 = uVar15 + 1;
        } while ((longlong)uVar15 < (longlong)local_60);
      }
      local_60 = &PTR_FUN_141b03cc0;
      lVar8 = *(longlong *)(*(longlong *)(param_1 + 0x1c40) + 0x60);
      if (lVar8 != 0) {
        FUN_1401bec70(&local_60,lVar8,*(uint *)(*(longlong *)(param_1 + 0x1c40) + 0x68) & 0xffffff);
      }
      uVar14 = *(uint *)(lVar13 + 0x60) & 0xffffff;
      lVar8 = *(longlong *)(param_1 + 0x1c40);
      if ((uVar14 <= *(uint *)(lVar8 + 0x4c) >> 8) ||
         (cVar2 = FUN_140358440(lVar8 + 0x40,uVar14), cVar2 != '\0')) {
        uVar6 = *(uint *)(lVar8 + 0x4c) >> 8;
        *(uint *)(lVar8 + 0x48) = *(uint *)(lVar8 + 0x48) & 0xff000000;
        if (uVar14 < uVar6) {
          uVar6 = uVar14;
        }
        *(uint *)(lVar8 + 0x48) = *(uint *)(lVar8 + 0x48) | uVar6;
      }
      if ((*(uint *)(*(longlong *)(param_1 + 0x1c40) + 0x68) & 0xffffff) == 0) {
        return;
      }
      uVar14 = *(uint *)(*(longlong *)(param_1 + 0x1c40) + 0x48) & 0xffffff;
      uVar15 = uVar9;
      uVar16 = uVar9;
      if (uVar14 != 0) {
        do {
          plVar4 = (longlong *)(*(longlong *)(*(longlong *)(param_1 + 0x1c40) + 0x40) + uVar16);
          uVar6 = *(uint *)(*(longlong *)(*(longlong *)(param_1 + 0x1c40) + 0x50) + uVar15 * 4);
          if (((int)uVar6 <= (int)(*(uint *)((longlong)plVar4 + 0xc) >> 8)) ||
             (cVar2 = FUN_14016a4a0(plVar4,uVar6), cVar2 != '\0')) {
            uVar7 = *(uint *)((longlong)plVar4 + 0xc) >> 8;
            if ((int)uVar6 < (int)uVar7) {
              uVar7 = uVar6;
            }
            for (lVar8 = (longlong)(int)(*(uint *)(plVar4 + 1) & 0xffffff); lVar8 < (int)uVar7;
                lVar8 = lVar8 + 1) {
              *(undefined8 *)(*plVar4 + lVar8 * 8) = 0;
            }
            *(uint *)(plVar4 + 1) = *(uint *)(plVar4 + 1) & 0xff000000;
            *(uint *)(plVar4 + 1) = *(uint *)(plVar4 + 1) | uVar7 & 0xffffff;
          }
          uVar15 = uVar15 + 1;
          uVar16 = uVar16 + 0x10;
        } while ((longlong)uVar15 < (longlong)(int)uVar14);
      }
      piVar17 = *(int **)(*(longlong *)(param_1 + 0x1c40) + 0x60);
      uVar6 = *(uint *)(*(longlong *)(param_1 + 0x1c40) + 0x68) & 0xffffff;
      uVar14 = *(uint *)(param_3 + 1);
      uVar15 = uVar9;
      uVar16 = uVar9;
      if (uVar6 != 0) {
        do {
          if ((longlong)(int)(uVar14 & 0xffffff) <= (longlong)uVar16) break;
          piVar12 = (int *)(*(longlong *)(*(longlong *)(param_1 + 0x1c40) + 0x60) + uVar9);
          iVar3 = FUN_140143650(*(undefined8 *)(piVar17 + 2),*(undefined8 *)(piVar12 + 2));
          if (iVar3 == 0) {
            *(undefined8 *)
             (*(longlong *)
               (*(longlong *)(*(longlong *)(param_1 + 0x1c40) + 0x40) + (longlong)*piVar12 * 0x10) +
             (longlong)piVar12[1] * 8) = *(undefined8 *)(*param_3 + uVar16 * 8);
          }
          else {
            uVar16 = uVar16 + 1;
            uVar15 = uVar15 - 1;
            uVar9 = uVar9 - 0x10;
            piVar17 = piVar12;
          }
          uVar15 = uVar15 + 1;
          uVar9 = uVar9 + 0x10;
        } while ((longlong)uVar15 < (longlong)(int)uVar6);
      }
      lVar8 = (longlong)param_2 * 0x10 + *(longlong *)(*(longlong *)(param_1 + 0x1c40) + 0x40);
    }
    FUN_1400af430(param_4,lVar8);
  }
  return;
}



````
