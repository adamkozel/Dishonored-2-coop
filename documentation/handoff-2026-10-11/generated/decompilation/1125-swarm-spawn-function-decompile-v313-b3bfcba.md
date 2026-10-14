# swarm-spawn-function-decompile-v313.txt

Original: [work/movement-integration/swarm-spawn-function-decompile-v313.txt](<../../../../work/movement-integration/swarm-spawn-function-decompile-v313.txt>)

SHA-256: `30BD74262C96766495BDED94724FB1D9CE13C3B98947487E1795D02F4E7A6BFD`; 2633 bytes.

## Original saved record

````text
1406a7a30

void FUN_1406a7a30(longlong *param_1)

{
  longlong lVar1;
  longlong lVar2;
  longlong lVar3;
  longlong lVar4;
  undefined4 *puVar5;
  longlong *plVar6;
  undefined8 uVar7;
  undefined4 uVar8;
  undefined4 local_res8 [2];
  longlong *local_68;
  code *local_60;
  undefined1 local_58 [64];
  
  FUN_1407e7670();
  lVar4 = param_1[1];
  lVar1 = param_1[2];
  if (*(int *)(lVar1 + 0x294) <= *(int *)(lVar4 + 0x240)) {
    (**(code **)(*param_1 + 0x168))(param_1,1);
    if ((*(uint *)((longlong)param_1 + 0x1c) >> 0x1e & 1) == 0) {
      if (*(longlong *)(lVar4 + 0xa0) != 0) {
        lVar2 = param_1[2];
        uVar7 = (**(code **)(*D2_GameSingletonPtr + 0x388))();
        lVar4 = FUN_14019d100(uVar7,*(undefined4 *)(lVar2 + 0xa8),*(undefined8 *)(lVar4 + 0xa0));
        if (lVar4 != 0) {
          lVar3 = param_1[2];
          *(undefined4 *)(lVar4 + 0x38) = *(undefined4 *)(lVar3 + 0x300);
          *(undefined4 *)(lVar4 + 0x3c) = *(undefined4 *)(lVar3 + 0x304);
          *(undefined4 *)(lVar4 + 0x40) = *(undefined4 *)(lVar3 + 0x308);
          local_res8[0] = FUN_1408ac000(DAT_14228a74c);
          puVar5 = (undefined4 *)FUN_14010e6a0(local_58,2,local_res8);
          *(undefined4 *)(lVar4 + 0x44) = *puVar5;
          *(undefined4 *)(lVar4 + 0x48) = puVar5[1];
          *(undefined4 *)(lVar4 + 0x4c) = puVar5[2];
          *(undefined4 *)(lVar4 + 0x50) = puVar5[3];
          *(undefined4 *)(lVar4 + 0x54) = puVar5[4];
          *(undefined4 *)(lVar4 + 0x58) = puVar5[5];
          *(undefined4 *)(lVar4 + 0x5c) = puVar5[6];
          *(undefined4 *)(lVar4 + 0x60) = puVar5[7];
          *(undefined4 *)(lVar4 + 100) = puVar5[8];
          uVar8 = FUN_1408780a0(lVar2);
          *(undefined4 *)(lVar4 + 0xf8) = uVar8;
          *(byte *)(lVar4 + 0x138) = *(byte *)(lVar4 + 0x138) & 0xfb;
          FUN_1401a2ff0(uVar7,lVar4,0,0x1ffff,0);
        }
      }
    }
    else {
      FUN_1406aae50(param_1,1);
    }
  }
  if (((*(uint *)((longlong)param_1 + 0x1c) >> 0x1e & 1) != 0) &&
     ((*(byte *)((longlong)param_1 + 0xcc) & 0x80) != 0)) {
    plVar6 = (longlong *)FUN_140720780(param_1[9],local_res8,"arkCpntSwarmBase::LockLOD",0);
    if (param_1 + 0x1a != plVar6) {
      *(int *)(param_1 + 0x1a) = (int)*plVar6;
    }
  }
  if (0.0 < *(float *)(param_1 + 0x26)) {
    FUN_140720780(param_1[9],local_res8,"arkCpntSwarmBase::LockLOD",0);
  }
  local_60 = FUN_1406ab860;
  local_68 = param_1;
  uVar7 = FUN_140810240();
  FUN_140810810(uVar7,lVar1,0x110,&local_68);
  local_60 = FUN_1406ab8d0;
  local_68 = param_1;
  uVar7 = FUN_140810240();
  FUN_140810810(uVar7,lVar1,0x111,&local_68);
  return;
}



````
