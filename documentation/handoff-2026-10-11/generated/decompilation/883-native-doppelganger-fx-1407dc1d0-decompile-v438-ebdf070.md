# native-doppelganger-fx-1407dc1d0-decompile-v438.txt

Original: [work/movement-integration/native-doppelganger-fx-1407dc1d0-decompile-v438.txt](<../../../../work/movement-integration/native-doppelganger-fx-1407dc1d0-decompile-v438.txt>)

SHA-256: `10E9D71148467279E72F7C06C1510DC6FDDCAAA4118A4B9DCB03911F4BF389CA`; 4841 bytes.

## Original saved record

````text

void FUN_1407dc1d0(longlong param_1)

{
  int iVar1;
  longlong lVar2;
  longlong lVar3;
  byte bVar4;
  int *piVar5;
  undefined8 uVar6;
  undefined1 *puVar7;
  char *pcVar8;
  undefined4 *puVar9;
  int *piVar10;
  char cVar11;
  byte bVar12;
  ushort *puVar13;
  uint uVar14;
  undefined8 uVar15;
  ushort local_res8 [4];
  ushort local_res10 [4];
  ushort local_res18 [4];
  undefined1 local_res20 [8];
  undefined4 local_78;
  undefined8 local_74;
  undefined4 local_6c;
  byte local_68;
  ushort local_64;
  undefined4 local_60;
  undefined8 local_5c;
  undefined4 local_54;
  byte local_50;
  ushort local_4c;
  undefined1 local_48 [8];
  undefined1 local_40 [8];
  undefined1 local_38 [8];
  undefined1 local_30 [8];
  undefined1 local_28 [16];
  
  FUN_140883a10(*(undefined8 *)(param_1 + 0xd0),param_1);
  FUN_1407db670(param_1);
  piVar5 = (int *)FUN_140191b60(*(undefined8 *)(param_1 + 0xb8));
  iVar1 = *piVar5;
  if (iVar1 == 1) {
    FUN_140131f90(local_48,"Sword",0);
    uVar15 = *(undefined8 *)(param_1 + 0xf8);
    uVar6 = FUN_140131ae0(local_28);
    puVar7 = local_48;
    puVar13 = local_res18;
  }
  else if (iVar1 == 2) {
    FUN_140131f90(local_40,"Empty",0);
    uVar15 = *(undefined8 *)(param_1 + 0xf8);
    uVar6 = FUN_140131ae0(local_38);
    puVar7 = local_40;
    puVar13 = local_res10;
  }
  else {
    if (iVar1 != 3) goto LAB_1407dc2b7;
    FUN_140131f90(local_res20,"SwordGunAim",0);
    uVar15 = *(undefined8 *)(param_1 + 0xf8);
    uVar6 = FUN_140131ae0(local_30);
    puVar7 = local_res20;
    puVar13 = local_res8;
  }
  puVar7 = (undefined1 *)FUN_1405484d0(uVar15,puVar13,puVar7,uVar6);
  if ((undefined1 *)(param_1 + 0x189) != puVar7) {
    *(undefined1 *)(param_1 + 0x189) = *puVar7;
  }
LAB_1407dc2b7:
  if (-1 < *(char *)(param_1 + 0x189)) {
    puVar7 = (undefined1 *)
             FUN_140548230(*(undefined8 *)(param_1 + 0xf8),local_res8,
                           *(undefined1 *)(param_1 + 0x189));
    if ((undefined1 *)(param_1 + 0x188) != puVar7) {
      *(undefined1 *)(param_1 + 0x188) = *puVar7;
    }
  }
  uVar14 = 0;
  if ((-1 < *(char *)(param_1 + 0x188)) && (-1 < *(char *)(param_1 + 0x189))) {
    pcVar8 = (char *)FUN_140548950(*(undefined8 *)(param_1 + 0xf8),local_res8);
    if ((*pcVar8 == *(char *)(param_1 + 0x188)) &&
       (pcVar8 = (char *)FUN_140548480(*(undefined8 *)(param_1 + 0xf8),local_res8),
       *pcVar8 == *(char *)(param_1 + 0x189))) {
      *(undefined2 *)(param_1 + 0x188) = 0xffff;
    }
    else {
      local_68 = local_68 & 0xfe;
      local_50 = local_50 & 0xfe;
      local_res8[0] = (ushort)*(byte *)(param_1 + 0x188);
      local_64 = local_res8[0];
      local_res8[0] = (ushort)*(byte *)(param_1 + 0x189);
      local_78 = 0;
      local_74 = 1;
      local_6c = 0;
      local_60 = 0;
      local_4c = local_res8[0];
      local_5c = 1;
      local_54 = 0;
      puVar9 = (undefined4 *)FUN_140545a00(*(undefined8 *)(param_1 + 0xf8),local_res8,&local_78);
      if ((undefined4 *)(param_1 + 0x17c) != puVar9) {
        *(undefined4 *)(param_1 + 0x17c) = *puVar9;
      }
      puVar9 = (undefined4 *)FUN_140544ed0(*(undefined8 *)(param_1 + 0xf8),local_res8,&local_60);
      if ((undefined4 *)(param_1 + 0x180) != puVar9) {
        *(undefined4 *)(param_1 + 0x180) = *puVar9;
      }
    }
  }
  lVar2 = *(longlong *)(param_1 + 0xd0);
  bVar4 = *(byte *)(lVar2 + 0xd1) >> 5 & 1;
  bVar12 = *(byte *)(lVar2 + 0xd1) >> 3 & 1;
  if ((bVar12 == 0) && (bVar4 == 0)) {
    cVar11 = '\0';
  }
  else {
    cVar11 = '\x01';
  }
  *(byte *)(param_1 + 0x1ac) = *(byte *)(param_1 + 0x1ac) & 0xfb;
  *(byte *)(param_1 + 0x1ac) = *(byte *)(param_1 + 0x1ac) | cVar11 << 2;
  lVar3 = *(longlong *)(param_1 + 0xb8);
  if ((((bVar12 != 0) && ((*(byte *)(lVar3 + 0x264) & 4) == 0)) &&
      ((*(byte *)(lVar3 + 0x268) & 4) != 0)) ||
     (((bVar4 != 0 && ((*(byte *)(lVar3 + 0x264) & 2) == 0)) &&
      ((*(byte *)(lVar3 + 0x268) & 2) != 0)))) {
    *(byte *)(param_1 + 0x1ac) = *(byte *)(param_1 + 0x1ac) | 2;
    *(undefined4 *)(param_1 + 400) = *(undefined4 *)(lVar2 + 0xa8);
    *(undefined4 *)(param_1 + 0x194) = *(undefined4 *)(lVar2 + 0x300);
    *(undefined4 *)(param_1 + 0x198) = *(undefined4 *)(lVar2 + 0x304);
    *(undefined4 *)(param_1 + 0x19c) = *(undefined4 *)(lVar2 + 0x308);
  }
  piVar10 = *(int **)(lVar3 + 0x208);
  piVar5 = piVar10 + (*(uint *)(lVar3 + 0x210) & 0xffffff);
  for (; piVar10 != piVar5; piVar10 = piVar10 + 1) {
    uVar14 = uVar14 | *(uint *)(&DAT_141c6c7a0 + (longlong)*piVar10 * 4);
  }
  *(uint *)(param_1 + 0x1a0) = uVar14;
  if (*(longlong *)(param_1 + 0xb0) == DAT_1432ce5b8) {
    *(undefined4 *)(param_1 + 0x1a8) = 2;
    return;
  }
  if (*(longlong *)(param_1 + 0xe8) != 0) {
    FUN_140767420(*(longlong *)(param_1 + 0xe8),
                  *(undefined4 *)(*(longlong *)(param_1 + 200) + 0x158),0);
  }
  return;
}



````
