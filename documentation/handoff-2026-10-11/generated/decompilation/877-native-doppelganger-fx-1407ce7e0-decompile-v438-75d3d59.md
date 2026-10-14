# native-doppelganger-fx-1407ce7e0-decompile-v438.txt

Original: [work/movement-integration/native-doppelganger-fx-1407ce7e0-decompile-v438.txt](<../../../../work/movement-integration/native-doppelganger-fx-1407ce7e0-decompile-v438.txt>)

SHA-256: `2F61D8EF2D2B37FB39D6545F29686452C2A65882051F485246B89E9ED3316AB1`; 9305 bytes.

## Original saved record

````text

void FUN_1407ce7e0(undefined **param_1,longlong *param_2)

{
  undefined4 uVar1;
  uint uVar2;
  undefined *puVar3;
  bool bVar4;
  undefined1 *puVar5;
  char cVar6;
  uint uVar7;
  undefined *puVar8;
  longlong lVar9;
  longlong *plVar10;
  longlong *plVar11;
  uint uVar12;
  int iVar13;
  ulonglong uVar14;
  longlong lVar15;
  longlong lVar16;
  ulonglong uVar17;
  ulonglong uVar18;
  ulonglong uVar19;
  char cVar20;
  longlong lVar21;
  undefined **local_res8;
  longlong *local_res10;
  char local_res18;
  undefined *local_res20;
  ulonglong in_stack_fffffffffffffc08;
  longlong local_3c0;
  longlong local_3b8;
  undefined **local_398;
  undefined1 **local_390;
  undefined4 local_388;
  undefined8 local_378;
  undefined1 *local_368;
  uint local_360;
  uint local_35c;
  undefined1 local_358 [256];
  undefined **local_258;
  undefined8 local_250;
  undefined4 local_248;
  undefined1 *local_240;
  undefined4 local_238;
  undefined4 local_234;
  undefined1 local_230 [384];
  undefined1 *local_b0;
  undefined4 local_a8;
  undefined4 local_a4;
  undefined1 local_a0 [64];
  undefined8 local_60;
  undefined4 local_58;
  undefined4 local_54;
  
  local_378 = 0xfffffffffffffffe;
  cVar20 = '\0';
  local_res18 = '\0';
  local_360 = 0x5000000;
  local_368 = local_358;
  local_35c = 0x20ff;
  puVar3 = *param_1;
  uVar7 = *(uint *)(puVar3 + 0xb8) & 0xffffff;
  local_3b8 = (longlong)(int)uVar7;
  local_res8 = param_1;
  local_res10 = param_2;
  if (uVar7 != 0) {
    local_3c0 = 0;
    do {
      plVar11 = local_res10;
      uVar17 = 0;
      lVar16 = *(longlong *)(puVar3 + 0xb0) + local_3c0;
      cVar6 = FUN_140190c20(lVar16 + 0x10);
      uVar18 = 0xffffffffffffffff;
      if ((*(uint *)(plVar11 + 1) & 0xffffff) != 0) {
        plVar10 = (longlong *)*local_res10;
        uVar14 = uVar17;
        uVar19 = uVar17;
        do {
          uVar18 = uVar19;
          if (*plVar10 == *(longlong *)(lVar16 + 0x10)) break;
          uVar7 = (int)uVar14 + 1;
          uVar14 = (ulonglong)uVar7;
          uVar19 = uVar19 + 1;
          plVar10 = plVar10 + 6;
          uVar18 = 0xffffffffffffffff;
        } while ((int)uVar7 < (int)(*(uint *)(plVar11 + 1) & 0xffffff));
      }
      if (cVar6 == '\0') {
        if ((longlong)uVar18 < 0) {
          if ((*(longlong *)(lVar16 + 0x328) != 0) && (*(char *)((longlong)param_1 + 0x2b4) == '\0')
             ) {
            in_stack_fffffffffffffc08 = 0;
            lVar15 = FUN_1408ae9b0(D2_GameSingletonPtr,*(longlong *)(lVar16 + 0x328),&DAT_1431af668,
                                   &DAT_14228a780,0,1,0,0,0);
            *(byte *)(lVar15 + 0xd4) = *(byte *)(lVar15 + 0xd4) | 4;
            local_res18 = '\x01';
            puVar8 = (undefined *)FUN_140652d60();
            local_res20 = puVar8;
            if (puVar8 != (undefined *)0x0) {
              FUN_1407db090(puVar8,param_1,lVar16,0);
            }
            FUN_1407dc130(puVar8,lVar15,1);
            FUN_1408918f0(&local_368,&local_res20);
            cVar20 = '\x01';
          }
        }
        else {
          lVar21 = uVar18 * 0x30;
          lVar15 = *local_res10;
          plVar11 = local_res10;
          uVar18 = uVar17;
          cVar20 = local_res18;
          if ((*(uint *)(lVar15 + 0x18 + lVar21) & 0xffffff) != 0) {
            do {
              local_res20 = (undefined *)
                            CONCAT44(local_res20._4_4_,
                                     *(undefined4 *)(uVar17 + *(longlong *)(lVar15 + 0x10 + lVar21))
                                    );
              lVar9 = FUN_1407b5e00(D2_GameSingletonPtr + 0x3130b0,&local_res20);
              if ((lVar9 != 0) && (lVar9 = FUN_1407d5c40(lVar9,0), lVar9 != 0)) {
                uVar1 = *(undefined4 *)(lVar21 + 8 + *plVar11);
                puVar8 = (undefined *)FUN_140652d60();
                param_1 = local_res8;
                local_res20 = puVar8;
                if (puVar8 != (undefined *)0x0) {
                  FUN_1407db090(puVar8,local_res8,lVar16,uVar1);
                }
                FUN_1407dc130(puVar8,lVar9,0);
                FUN_1408918f0(&local_368,&local_res20);
                plVar11 = local_res10;
              }
              uVar7 = (int)uVar18 + 1;
              uVar17 = uVar17 + 4;
              uVar18 = (ulonglong)uVar7;
              cVar20 = local_res18;
            } while ((int)uVar7 < (int)(*(uint *)(lVar15 + 0x18 + lVar21) & 0xffffff));
          }
        }
      }
      else {
        puVar8 = (undefined *)FUN_140652d60();
        local_res20 = puVar8;
        if (puVar8 != (undefined *)0x0) {
          FUN_1407db090(puVar8,param_1,lVar16,0);
        }
        FUN_1407dc120(puVar8);
        param_1[0x2a] = puVar8;
        FUN_1408918f0(&local_368,&local_res20);
      }
      local_3c0 = local_3c0 + 0x388;
      local_3b8 = local_3b8 + -1;
    } while (local_3b8 != 0);
  }
  lVar16 = 0;
  uVar7 = local_360 & 0xffffff;
  if (((uVar7 < 5) || (DAT_14398f828 == 0)) ||
     (bVar4 = true, *(char *)(D2_GameSingletonPtr + 0x337122) == '\0')) {
    bVar4 = false;
  }
  if (bVar4) {
    local_250 = 0;
    local_248 = 0x1ffff;
    local_258 = &PTR_LAB_141ad4a48;
    local_240 = local_230;
    local_238 = 0;
    local_234 = 0x80000010;
    local_b0 = local_a0;
    local_a8 = 0;
    local_a4 = 0x80000010;
    local_60 = 0;
    local_58 = 0;
    local_54 = 0x80000000;
    local_398 = &PTR_LAB_141c6b370;
    local_390 = &local_368;
    local_388 = 0xffffffff;
    FUN_140aa96f0(&local_258,&local_res8,&local_398,*(undefined4 *)(D2_GameSingletonPtr + 0x1f6524),
                  in_stack_fffffffffffffc08 & 0xffffffff00000000);
    FUN_14007a9e0(&local_258);
    local_398 = &PTR_LAB_141abd918;
    FUN_140090650(&local_258);
  }
  else {
    lVar15 = lVar16;
    if (uVar7 != 0) {
      do {
        if (((*(byte *)(*(longlong *)(local_368 + lVar15 * 8) + 0x1ac) & 8) == 0) &&
           (*(longlong *)(*(longlong *)(local_368 + lVar15 * 8) + 0xd0) != 0)) {
          FUN_1407dc1d0();
        }
        lVar15 = lVar15 + 1;
      } while (lVar15 < (int)uVar7);
    }
  }
  if (cVar20 != '\0') {
    FUN_1407cf680(param_1,&local_368);
  }
  if (*(uint *)((longlong)param_1 + 0x1c) >> 8 < uVar7) {
    FUN_1407d01e0(param_1 + 2,uVar7);
  }
  if (uVar7 != 0) {
    do {
      lVar15 = *(longlong *)(local_368 + lVar16 * 8);
      uVar2 = *(uint *)((longlong)param_1 + 0x1c);
      uVar12 = uVar2 >> 8;
      if ((*(uint *)(param_1 + 3) & 0xffffff) == uVar12) {
        if (((byte)uVar2 & 0xc0) == 0xc0) {
          if (DAT_1431aee98 == '\0') {
            if ((uVar2 & 0x3f) == 0x3f) {
              if (uVar2 >> 8 == 0) {
                iVar13 = 1;
              }
              else if (uVar12 < 0x40000000) {
                iVar13 = uVar12 * 2;
              }
              else {
                iVar13 = 0x7fffffff;
              }
            }
            else {
              iVar13 = 1 << ((byte)(uVar2 & 0x3f) & 0x1f);
              uVar12 = uVar12 + iVar13;
              iVar13 = uVar12 - (iVar13 - 1U & uVar12);
            }
          }
          else {
            iVar13 = uVar12 + 1;
          }
          FUN_1407cf300(param_1 + 2,iVar13);
        }
        else {
          if (DAT_1431aee98 == '\0') {
            if ((uVar2 & 0x3f) == 0x3f) {
              if (uVar2 >> 8 == 0) {
                iVar13 = 1;
              }
              else if (uVar12 < 0x40000000) {
                iVar13 = uVar12 * 2;
              }
              else {
                iVar13 = 0x7fffffff;
              }
            }
            else {
              iVar13 = 1 << ((byte)(uVar2 & 0x3f) & 0x1f);
              uVar12 = uVar12 + iVar13;
              iVar13 = uVar12 - (iVar13 - 1U & uVar12);
            }
          }
          else {
            iVar13 = uVar12 + 1;
          }
          FUN_1407d01e0(param_1 + 2,iVar13);
        }
      }
      uVar12 = *(uint *)((longlong)param_1 + 0x1c) >> 8;
      uVar2 = *(uint *)(param_1 + 3);
      if ((uVar2 & 0xffffff) < uVar12) {
        plVar11 = (longlong *)(param_1[2] + (ulonglong)(uVar2 & 0xffffff) * 0x20);
        *(uint *)(param_1 + 3) = (uVar2 + 1 ^ uVar2) & 0xffffff ^ uVar2;
      }
      else {
        plVar11 = (longlong *)(param_1[2] + (ulonglong)(uVar12 - 1) * 0x20);
      }
      *plVar11 = lVar15;
      plVar11[2] = *(longlong *)(lVar15 + 0xb0);
      *(undefined4 *)(plVar11 + 3) = *(undefined4 *)(lVar15 + 0xa8);
      if ((((*(byte *)(lVar15 + 0x1ac) & 1) == 0) && ((*(byte *)(lVar15 + 0x1ac) & 8) == 0)) &&
         (*(longlong *)(lVar15 + 0xd0) != 0)) {
        plVar11[1] = *(longlong *)(lVar15 + 0xd0);
      }
      if (*(longlong *)(lVar15 + 0xb0) == DAT_1432ce5b8) {
        *(undefined1 *)((longlong)param_1 + 0x2b5) = 1;
      }
      lVar16 = lVar16 + 1;
    } while (lVar16 < (int)uVar7);
  }
  local_res8 = &PTR_FUN_141c6b3b0;
  if (param_1[0x1c] != (undefined *)0x0) {
    FUN_1407d1e50(&local_res8,param_1[0x1c],*(uint *)(param_1 + 0x1d) & 0xffffff);
  }
  puVar5 = local_368;
  if (((local_35c & 0x40) == 0) && (local_368 != (undefined1 *)0x0)) {
    plVar11 = (longlong *)FUN_1401548e0();
    (**(code **)(*plVar11 + 0x10))
              (plVar11,
               "c:\\users\\zmiarchiving\\desktop\\dh2\\dishonored2\\voidengine\\core\\idlib\\containers/List.h"
               ,0x1ec,puVar5,0x10);
  }
  return;
}



````
