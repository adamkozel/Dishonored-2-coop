# partner-context-1403b0bf0-decompile-v384.txt

Original: [work/movement-integration/partner-context-1403b0bf0-decompile-v384.txt](<../../../../work/movement-integration/partner-context-1403b0bf0-decompile-v384.txt>)

SHA-256: `E9F720B8FE32B8428477278F98565F445421B61FF606DDF23BB6F7E29D3C58B5`; 3975 bytes.

## Original saved record

````text

undefined4 FUN_1403b0bf0(longlong param_1)

{
  longlong lVar1;
  char cVar2;
  undefined4 uVar3;
  longlong lVar4;
  longlong lVar5;
  longlong lVar6;
  longlong lVar7;
  undefined4 *puVar8;
  undefined2 *puVar9;
  float fVar10;
  undefined2 local_res8 [4];
  float local_108;
  float local_104;
  float local_100;
  float local_f8;
  float local_f4;
  float local_f0;
  undefined4 local_e8;
  undefined4 local_e4;
  undefined4 local_e0;
  undefined4 local_dc;
  undefined4 local_d8;
  undefined4 local_d4;
  undefined4 local_d0;
  undefined2 uStack_cc;
  undefined2 uStack_ca;
  float local_c8;
  float local_c4;
  float local_c0;
  float local_bc;
  float local_b8;
  float local_b4;
  float local_b0;
  float local_ac;
  float local_a8;
  float local_a0;
  float local_9c;
  float local_94;
  float local_90;
  float local_8c;
  undefined1 local_78 [80];
  
  lVar4 = FUN_1406dc560(*(undefined4 *)(param_1 + 0x20));
  if (lVar4 != 0) {
    cVar2 = FUN_1405bf440(lVar4);
    if (cVar2 == '\0') {
      cVar2 = FUN_1405bf480(lVar4);
      if (cVar2 == '\0') {
        cVar2 = FUN_1405bf4c0(lVar4);
        if (cVar2 == '\0') goto LAB_1403b0feb;
      }
    }
    lVar4 = FUN_1406dc560(*(undefined4 *)(param_1 + 0x20));
    lVar5 = FUN_1408a8ba0(D2_GameSingletonPtr);
    lVar6 = FUN_140877720(lVar4,&DAT_1439e8010,2);
    local_108 = -*(float *)(lVar5 + 0x30c);
    local_100 = (float)(*(uint *)(lVar5 + 0x314) ^ 0x80000000);
    local_104 = -*(float *)(lVar5 + 0x310);
    uVar3 = FUN_1406dd170(lVar4 + 0x30c,&local_108);
    if (lVar6 != 0) {
      lVar7 = FUN_140878470(lVar4);
      puVar8 = (undefined4 *)
               FUN_14025c0c0(*(longlong *)(lVar7 + 0x1fe8) + 0x108,&DAT_1432d07a0,&DAT_143995a60);
      local_e8 = *puVar8;
      local_e4 = puVar8[1];
      local_e0 = puVar8[2];
      local_dc = puVar8[3];
      local_d8 = puVar8[4];
      local_d4 = puVar8[5];
      local_d0 = puVar8[6];
      uStack_cc = (undefined2)puVar8[7];
      uStack_ca = (undefined2)((uint)puVar8[7] >> 0x10);
      if ((short)((uint6)*(undefined6 *)(puVar8 + 6) >> 0x20) == -1) {
        lVar1 = *(longlong *)(*(longlong *)(lVar7 + 0x1fe8) + 0x50);
        if (lVar1 == 0) {
          puVar9 = local_res8;
          local_res8[0] = 0xffff;
        }
        else {
          puVar9 = (undefined2 *)FUN_140256080(lVar1,local_res8,DAT_143995a60);
        }
        lVar7 = *(longlong *)(*(longlong *)(lVar7 + 0x1fe8) + 0x50);
        if (lVar7 == 0) {
          local_res8[0] = 0xffff;
          puVar9 = local_res8;
        }
        else {
          puVar9 = (undefined2 *)FUN_140256000(lVar7,local_res8,*puVar9);
        }
        uStack_cc = *puVar9;
      }
      cVar2 = FUN_14053dac0(lVar6,3,&local_e8,local_78,&local_a0);
      if (cVar2 != '\0') {
        local_f0 = local_8c;
        if (SQRT(local_90 * local_90 + local_94 * local_94 + local_8c * local_8c) < 0.1) {
          local_94 = local_a0;
          local_90 = local_9c;
        }
        local_ac = DAT_14228a944;
        local_a8 = DAT_14228a948;
        fVar10 = local_94 * local_94 + local_90 * local_90 + 0.0;
        if (fVar10 <= 1.1754944e-38) {
          fVar10 = 1.1754944e-38;
        }
        local_c0 = 1.0 / SQRT(fVar10);
        local_b0 = DAT_14228a940;
        local_c4 = local_90 * local_c0;
        local_c8 = local_94 * local_c0;
        local_c0 = local_c0 * 0.0;
        local_bc = DAT_14228a944 * local_c0 - local_c4 * DAT_14228a948;
        local_b8 = local_c8 * DAT_14228a948 - DAT_14228a940 * local_c0;
        local_b4 = local_c4 * DAT_14228a940 - local_c8 * DAT_14228a944;
        FUN_140878230(lVar4,&local_108);
        local_100 = *(float *)(lVar4 + 0x308);
        local_f8 = *(float *)(lVar5 + 0x300) - local_108;
        local_f4 = *(float *)(lVar5 + 0x304) - local_104;
        local_f0 = *(float *)(lVar5 + 0x308) - local_100;
        uVar3 = FUN_1406dd170(&local_c8,&local_f8);
      }
    }
    return uVar3;
  }
LAB_1403b0feb:
  return *(undefined4 *)(param_1 + 0x30);
}



````
