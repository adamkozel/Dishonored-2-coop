
void FUN_14023fca0(longlong param_1,longlong param_2,longlong param_3)

{
  uint *puVar1;
  char cVar2;
  int iVar3;
  uint *puVar4;
  longlong lVar5;
  uint uVar6;
  ulonglong uVar7;
  longlong lVar8;
  longlong lVar9;
  short asStackX_8 [4];
  undefined4 uStack_138;
  undefined4 uStack_134;
  undefined4 uStack_130;
  undefined4 uStack_128;
  undefined4 uStack_124;
  undefined4 uStack_120;
  float fStack_118;
  float fStack_114;
  float fStack_110;
  undefined4 uStack_108;
  undefined4 uStack_104;
  undefined4 uStack_100;
  undefined4 uStack_fc;
  undefined4 uStack_f8;
  undefined4 uStack_f4;
  undefined4 uStack_f0;
  undefined4 uStack_ec;
  undefined4 uStack_e8;
  undefined8 uStack_d8;
  undefined8 uStack_d0;
  undefined8 uStack_c8;
  undefined8 uStack_c0;
  undefined8 uStack_b8;
  undefined8 uStack_b0;
  undefined1 auStack_a8 [16];
  undefined1 auStack_98 [16];
  undefined1 auStack_88 [16];
  undefined8 uStack_78;
  undefined8 uStack_70;
  undefined8 uStack_68;
  undefined8 uStack_60;
  undefined8 uStack_58;
  undefined8 uStack_50;
  undefined1 auStack_48 [16];
  
  if (((*(uint *)(param_1 + 0xa8) & 0xffffff) != 0) && (iRam00000001432d0218 == 0)) {
    FUN_14009e410(param_2);
    uVar6 = *(uint *)(param_1 + 0xa8) & 0xffffff;
    uVar7 = (ulonglong)uVar6;
    if (uVar6 != 0) {
      lVar8 = 0;
      do {
        lVar9 = *(longlong *)(param_1 + 0xa0) + lVar8;
        asStackX_8[0] = -1;
        cVar2 = (**(code **)((longlong)*(int *)(lVar9 + 0x18) * 8 + 0x1422996f0))
                          (param_2,lVar9 + 0x10,lVar9,asStackX_8,&uStack_128,&fStack_118);
        if (cVar2 != '\0') {
          uStack_138 = uStack_124;
          uStack_134 = uStack_120;
          uStack_130 = uStack_128;
          FUN_14010d490(&uStack_138,&uStack_108);
          auStack_a8._4_4_ = uStack_f0;
          auStack_a8._0_4_ = uStack_e8;
          auStack_a8._8_4_ = uStack_ec;
          auStack_a8._12_4_ = 0;
          auStack_98._4_4_ = uStack_108;
          auStack_98._0_4_ = uStack_100;
          auStack_98._8_4_ = uStack_104;
          auStack_98._12_4_ = 0;
          auStack_88._4_4_ = uStack_fc;
          auStack_88._0_4_ = uStack_f4;
          auStack_88._8_4_ = uStack_f8;
          auStack_88._12_4_ = 0;
          FUN_140ae17d0(&uStack_d8,
                        (longlong)asStackX_8[0] * 0x40 +
                        *(longlong *)(*(longlong *)(param_1 + 0x50) + 0x30),auStack_a8);
          uStack_78 = uStack_d8;
          uStack_70 = uStack_d0;
          uStack_58 = uStack_b8;
          uStack_50 = uStack_b0;
          uStack_68 = uStack_c8;
          uStack_60 = uStack_c0;
          auStack_48._4_4_ = fStack_118 * 0.01;
          auStack_48._0_4_ = fStack_110 * 0.01;
          auStack_48._8_4_ = fStack_114 * 0.01;
          auStack_48._12_4_ = 0;
          D2_Animation_SetJointLocal(param_2,asStackX_8[0],&uStack_78);
          if ((*(uint *)(param_3 + ((longlong)asStackX_8[0] >> 5) * 4) >>
               ((byte)asStackX_8[0] & 0x1f) & 1) == 0) {
            puVar4 = (uint *)(param_3 + ((longlong)asStackX_8[0] >> 5) * 4);
            *puVar4 = *puVar4 | 1 << ((byte)asStackX_8[0] & 0x1f);
            lVar9 = *(longlong *)(param_2 + 0x48) + (longlong)asStackX_8[0] * 0x28;
            iVar3 = *(int *)(param_3 + 0x20);
            if (*(int *)(lVar9 + 0x20) < *(int *)(param_3 + 0x20)) {
              iVar3 = *(int *)(lVar9 + 0x20);
            }
            if ((-1 < iVar3) && (lVar5 = (longlong)iVar3, iVar3 != 0)) {
              puVar4 = (uint *)(param_3 + lVar5 * 4);
              do {
                puVar1 = (uint *)((longlong)puVar4 + (lVar9 - param_3) + -4);
                puVar4 = puVar4 + -1;
                *puVar4 = *puVar4 | *puVar1;
                lVar5 = lVar5 + -1;
              } while (lVar5 != 0);
            }
          }
        }
        lVar8 = lVar8 + 0x20;
        uVar7 = uVar7 - 1;
      } while (uVar7 != 0);
    }
    FUN_14009dcc0(param_2);
  }
  return;
}


