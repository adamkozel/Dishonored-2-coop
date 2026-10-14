
undefined8 FUN_140253350(char *param_1,undefined8 param_2,longlong *param_3)

{
  longlong lVar1;
  uint uVar2;
  char cVar3;
  uint uVar4;
  undefined8 uVar5;
  undefined2 *puVar6;
  byte bVar7;
  byte bVar8;
  longlong lVar9;
  longlong lVar10;
  longlong lVar11;
  int iVar12;
  char cStackX_8;
  undefined7 uStackX_9;
  undefined8 uStackX_10;
  char acStackX_18 [8];
  undefined1 auStackX_20 [8];
  int iStack_198;
  uint uStack_194;
  char acStack_190 [4];
  char acStack_18c [4];
  byte abStack_188 [4];
  int iStack_184;
  undefined1 auStack_180 [2];
  undefined1 auStack_17e [6];
  undefined1 auStack_178 [40];
  undefined8 uStack_150;
  undefined1 auStack_148 [264];
  
  uStack_150 = 0xfffffffffffffffe;
  uStackX_10 = param_2;
  (**(code **)(*param_3 + 0x28))(param_3,&iStack_184,4);
  (**(code **)(*param_3 + 0x28))(param_3,auStack_148,(longlong)iStack_184);
  FUN_140131f90(&cStackX_8,auStack_148,0);
  *(ulonglong *)(param_1 + 0x18) = CONCAT71(uStackX_9,cStackX_8);
  (**(code **)(*param_3 + 0x28))(param_3,param_1 + 0x20,4);
  (**(code **)(*param_3 + 0x28))(param_3,param_1,1);
  if (*param_1 == '\0') {
    (**(code **)(*param_3 + 0x28))(param_3,param_1 + 0x24,2);
    (**(code **)(*param_3 + 0x28))(param_3,&uStack_194,4);
    uVar2 = uStack_194;
    if (((int)uStack_194 <= (int)(*(uint *)(param_1 + 0x14) >> 8)) ||
       (cVar3 = FUN_140253ec0(param_1 + 8,uStack_194), cVar3 != '\0')) {
      *(uint *)(param_1 + 0x10) = *(uint *)(param_1 + 0x10) & 0xff000000;
      uVar4 = *(uint *)(param_1 + 0x14) >> 8;
      if ((int)uVar2 < (int)(*(uint *)(param_1 + 0x14) >> 8)) {
        uVar4 = uVar2;
      }
      *(uint *)(param_1 + 0x10) = *(uint *)(param_1 + 0x10) | uVar4 & 0xffffff;
    }
    iVar12 = 0;
    if (0 < (int)uStack_194) {
      lVar10 = 0;
      do {
        lVar11 = *(longlong *)(param_1 + 8) + lVar10;
        (**(code **)(*param_3 + 0x28))(param_3,acStack_18c,4);
        (**(code **)(*param_3 + 0x28))(param_3,acStack_190,4);
        (**(code **)(*param_3 + 0x28))(param_3,auStackX_20,2);
        (**(code **)(*param_3 + 0x28))(param_3,abStack_188,4);
        (**(code **)(*param_3 + 0x28))(param_3,lVar11,0xc);
        *(byte *)(lVar11 + 0xf) =
             *(byte *)(lVar11 + 0xf) ^ (acStack_190[0] << 5 ^ *(byte *)(lVar11 + 0xf)) & 0x60;
        bVar8 = *(byte *)(lVar11 + 0xf);
        bVar7 = (acStack_18c[0] << 3 ^ *(byte *)(lVar11 + 0xf)) & 0x18 ^ bVar8;
        *(byte *)(lVar11 + 0xf) = bVar7;
        *(byte *)(lVar11 + 0xf) = (bVar7 ^ abStack_188[0]) & 7 ^ bVar7;
        *(undefined1 *)(lVar11 + 0xe) = auStackX_20[0];
        bVar7 = bVar7 >> 3 & 3;
        if (bVar7 == 1) {
          bVar8 = bVar8 >> 5 & 3;
          if (bVar8 == 1) {
            (**(code **)(*param_3 + 0x28))(param_3,&cStackX_8,1);
            if (cStackX_8 != '\0') {
              FUN_14013fb30(auStack_178);
              iStack_198 = 0;
              (**(code **)(*param_3 + 0x28))(param_3,&iStack_198,4);
              if (iStack_198 < 1) {
                FUN_140141e70(auStack_178);
              }
              else {
                FUN_140142560(auStack_178,0x20);
                lVar1 = *param_3;
                lVar9 = (longlong)iStack_198;
                uVar5 = FUN_140140230(auStack_178,0);
                (**(code **)(lVar1 + 0x28))(param_3,uVar5,lVar9);
              }
              uVar5 = D2_InternedName_Resolve(auStack_178);
              puVar6 = (undefined2 *)FUN_140256080(uStackX_10,auStack_180,uVar5);
              if ((undefined2 *)(lVar11 + 0xc) != puVar6) {
                *(undefined2 *)(lVar11 + 0xc) = *puVar6;
              }
              goto code_r0x0001402536d5;
            }
          }
          else if (bVar8 == 2) {
            (**(code **)(*param_3 + 0x28))(param_3,lVar11 + 0xc,2);
          }
        }
        else if ((bVar7 == 0) &&
                ((**(code **)(*param_3 + 0x28))(param_3,acStackX_18,1), acStackX_18[0] != '\0')) {
          FUN_14013fb30(auStack_178);
          iStack_198 = 0;
          (**(code **)(*param_3 + 0x28))(param_3,&iStack_198,4);
          if (iStack_198 < 1) {
            FUN_140141e70(auStack_178);
          }
          else {
            FUN_140142560(auStack_178,0x20);
            lVar1 = *param_3;
            lVar9 = (longlong)iStack_198;
            uVar5 = FUN_140140230(auStack_178,0);
            (**(code **)(lVar1 + 0x28))(param_3,uVar5,lVar9);
          }
          uVar5 = D2_InternedName_Resolve(auStack_178);
          puVar6 = (undefined2 *)FUN_140256080(uStackX_10,auStack_17e,uVar5);
          if ((undefined2 *)(lVar11 + 0xc) != puVar6) {
            *(undefined2 *)(lVar11 + 0xc) = *puVar6;
          }
code_r0x0001402536d5:
          FUN_14013fd10(auStack_178);
        }
        iVar12 = iVar12 + 1;
        lVar10 = lVar10 + 0x10;
      } while (iVar12 < (int)uStack_194);
    }
  }
  return 1;
}


