# swarm-decompile-1406baf20-v276.txt

Original: [work/movement-integration/swarm-decompile-1406baf20-v276.txt](<../../../../work/movement-integration/swarm-decompile-1406baf20-v276.txt>)

SHA-256: `7C1DAA839C128F7999F6841E7A162346048EEA379DE42B0AC8EC77DA0D68EA8F`; 3889 bytes.

## Original saved record

````text

void FUN_1406baf20(longlong *param_1,longlong param_2,longlong param_3)

{
  longlong lVar1;
  longlong *plVar2;
  undefined8 uVar3;
  longlong lVar4;
  undefined8 *puVar5;
  undefined4 auStackX_8 [2];
  undefined4 uStack_58;
  undefined4 uStack_54;
  undefined4 uStack_50;
  undefined1 auStack_48 [48];
  
  lVar1 = *(longlong *)(param_2 + 8);
  FUN_1406b9070();
  if (*(longlong *)(param_2 + 0x10) == 0) {
    *(undefined4 *)(param_1[2] + 0xac) = 0x3fff0010;
  }
  else {
    *(undefined4 *)(param_1[2] + 0xac) = *(undefined4 *)(*(longlong *)(param_2 + 0x10) + 0xa8);
  }
  FUN_14087caa0(param_1[2]);
  if ((*(char *)(param_3 + 0x2e) == '\0') && (*(int *)(param_1[2] + 0x288) != 0)) {
    FUN_14087ce50(param_1[2],0);
  }
  uStack_58 = *(undefined4 *)(param_3 + 0x28);
  lVar4 = *(longlong *)(param_1[2] + 0xf0);
  *(undefined4 *)(lVar4 + 0x1890) = uStack_58;
  *(undefined4 *)(lVar4 + 0x1894) = uStack_58;
  *(undefined4 *)(lVar4 + 0x1898) = uStack_58;
  uStack_54 = uStack_58;
  uStack_50 = uStack_58;
  uVar3 = FUN_1408783d0(param_1[2]);
  FUN_140751050(uVar3,param_1);
  func_0x000140753630(param_1,&uStack_58);
  *(longlong **)(param_1[2] + 0x360) = param_1;
  *(byte *)(param_1[2] + 0xd4) = *(byte *)(param_1[2] + 0xd4) | 4;
  *(undefined4 *)(param_1[2] + 0x2e0) = *(undefined4 *)(*(longlong *)(param_2 + 0x10) + 0x2e0);
  *(byte *)(param_1[2] + 0xd7) = *(byte *)(param_1[2] + 0xd7) | 2;
  FUN_14080e910(D2_GameSingletonPtr + 0x3ea5d,param_1[2],*(undefined8 *)(param_2 + 0x10));
  param_1[3] = param_2;
  *(undefined4 *)((longlong)param_1 + 0x4c) = 1;
  param_1[4] = 0;
  param_1[5] = 0;
  *(undefined2 *)(param_1 + 6) = 0xffff;
  *(undefined4 *)((longlong)param_1 + 0x84) = 0;
  *(undefined4 *)(param_1 + 9) = 0;
  *(undefined4 *)(param_1 + 0xb) = 0;
  auStackX_8[0] = 0;
  *(undefined4 *)(param_1 + 10) = 0;
  *(undefined4 *)((longlong)param_1 + 0x54) = 0;
  *(undefined4 *)(param_1 + 0xd) = DAT_1431af620;
  *(undefined4 *)((longlong)param_1 + 0x6c) = DAT_1431af624;
  *(undefined4 *)(param_1 + 0xe) = DAT_1431af628;
  *(undefined4 *)((longlong)param_1 + 0x5c) = DAT_1431af668;
  *(undefined4 *)(param_1 + 0xc) = DAT_1431af66c;
  *(undefined4 *)((longlong)param_1 + 100) = DAT_1431af670;
  auStackX_8[0] = FUN_1408ac000(*(undefined4 *)(lVar1 + 0x130));
  *(undefined8 *)((longlong)param_1 + 0x8c) = 0;
  *(undefined4 *)(param_1 + 0x11) = auStackX_8[0];
  FUN_1403dab80(param_1 + 0x13,0);
  *(undefined4 *)(param_1 + 0x17) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0xbc) = DAT_1431af668;
  *(undefined4 *)(param_1 + 0x18) = DAT_1431af66c;
  *(undefined4 *)((longlong)param_1 + 0xc4) = DAT_1431af670;
  plVar2 = (longlong *)param_1[0xf];
  *(undefined4 *)(param_1 + 0x19) = 0;
  if (plVar2 != (longlong *)0x0) {
    (**(code **)(*plVar2 + 0xe0))(plVar2,param_1);
  }
  if (*(char *)(param_3 + 0x2d) == '\0') {
    uVar3 = 6;
    if (*(char *)(param_3 + 0x2e) == '\0') {
      uVar3 = 1;
    }
  }
  else {
    uVar3 = 4;
  }
  (**(code **)(*param_1 + 0x80))(param_1,uVar3,0);
  if ((*(longlong *)(lVar1 + 0xa8) != 0) && (*(char *)(param_3 + 0x2c) != '\0')) {
    if (**(char **)(lVar1 + 0xd0) == '\0') {
      FUN_1406bb2a0(param_1);
    }
    else {
      lVar4 = (**(code **)(*D2_GameSingletonPtr + 0x1c8))
                        (D2_GameSingletonPtr,&DAT_1439bdeb0,0,param_1[2] + 0x300,&DAT_14228a780,0);
      *(byte *)(lVar4 + 0xd4) = *(byte *)(lVar4 + 0xd4) | 0x10;
      auStackX_8[0] = FUN_1408ac000(DAT_14228a74c);
      FUN_14010e6a0(auStack_48,2,auStackX_8);
      FUN_140875e80(param_1[2],lVar4,lVar1 + 0xd0,&DAT_1431af668,auStack_48,1);
      uVar3 = FUN_1404903b0(lVar4,1);
      puVar5 = (undefined8 *)FUN_14090faf0(uVar3);
      *puVar5 = *(undefined8 *)(lVar1 + 0xa8);
      *(undefined4 *)(puVar5 + 1) = *(undefined4 *)(lVar4 + 0xa8);
      *(undefined1 *)(puVar5 + 3) = 1;
      *(undefined1 *)((longlong)puVar5 + 0x1c) = 1;
    }
  }
  return;
}



````
