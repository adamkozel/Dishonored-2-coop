# swarm-fx-helpers-decompile-v312.txt

Original: [work/movement-integration/swarm-fx-helpers-decompile-v312.txt](<../../../../work/movement-integration/swarm-fx-helpers-decompile-v312.txt>)

SHA-256: `AB9365DF14898B6B517101F43F6F4FC32D8280980E6059009F367223E7888ED0`; 4092 bytes.

## Original saved record

````text
1406bb500

void FUN_1406bb500(longlong param_1,longlong param_2)

{
  longlong lVar1;
  char cVar2;
  longlong lVar3;
  undefined8 uVar4;
  longlong lVar5;
  undefined4 uVar6;
  ulonglong in_stack_ffffffffffffffe8;
  
  if (param_2 != 0) {
    lVar3 = D2_GameSingletonPtr[0x3d12b];
    cVar2 = FUN_14061c8f0();
    if (cVar2 != '\0') {
      in_stack_ffffffffffffffe8 = *(longlong *)(param_1 + 0x10) + 0x30c;
      lVar3 = (**(code **)(*D2_GameSingletonPtr + 0x1c8))
                        (D2_GameSingletonPtr,&DAT_1439cf460,0,*(longlong *)(param_1 + 0x10) + 0x300,
                         in_stack_ffffffffffffffe8,0);
      FUN_14080e910(D2_GameSingletonPtr + 0x3ea5d,lVar3,*(undefined8 *)(param_1 + 0x10));
      FUN_14087e7e0(lVar3,1);
    }
    uVar4 = (**(code **)(*D2_GameSingletonPtr + 0x388))();
    lVar5 = FUN_14019d100(uVar4,*(undefined4 *)(lVar3 + 0xa8),param_2);
    if (lVar5 != 0) {
      lVar1 = *(longlong *)(param_1 + 0x10);
      *(undefined4 *)(lVar5 + 0x38) = *(undefined4 *)(lVar1 + 0x300);
      *(undefined4 *)(lVar5 + 0x3c) = *(undefined4 *)(lVar1 + 0x304);
      *(undefined4 *)(lVar5 + 0x40) = *(undefined4 *)(lVar1 + 0x308);
      *(undefined4 *)(lVar5 + 0x44) = *(undefined4 *)(lVar1 + 0x30c);
      *(undefined4 *)(lVar5 + 0x48) = *(undefined4 *)(lVar1 + 0x310);
      *(undefined4 *)(lVar5 + 0x4c) = *(undefined4 *)(lVar1 + 0x314);
      *(undefined4 *)(lVar5 + 0x50) = *(undefined4 *)(lVar1 + 0x318);
      *(undefined4 *)(lVar5 + 0x54) = *(undefined4 *)(lVar1 + 0x31c);
      *(undefined4 *)(lVar5 + 0x58) = *(undefined4 *)(lVar1 + 800);
      *(undefined4 *)(lVar5 + 0x5c) = *(undefined4 *)(lVar1 + 0x324);
      *(undefined4 *)(lVar5 + 0x60) = *(undefined4 *)(lVar1 + 0x328);
      *(undefined4 *)(lVar5 + 100) = *(undefined4 *)(lVar1 + 0x32c);
      *(undefined4 *)(lVar5 + 0x68) = *(undefined4 *)(lVar1 + 0x33c);
      *(undefined4 *)(lVar5 + 0x6c) = *(undefined4 *)(lVar1 + 0x340);
      *(undefined4 *)(lVar5 + 0x70) = *(undefined4 *)(lVar1 + 0x344);
      uVar6 = FUN_1408780a0(lVar3);
      *(undefined4 *)(lVar5 + 0xf8) = uVar6;
      *(byte *)(lVar5 + 0x138) = *(byte *)(lVar5 + 0x138) & 0xfb;
      FUN_1401a2ff0(uVar4,lVar5,0,0x1ffff,in_stack_ffffffffffffffe8 & 0xffffffff00000000);
    }
  }
  return;
}



1406bb2a0

void FUN_1406bb2a0(longlong param_1,longlong param_2)

{
  longlong lVar1;
  undefined8 uVar2;
  longlong lVar3;
  undefined4 *puVar4;
  undefined4 uVar5;
  undefined1 local_res10 [8];
  
  if (param_2 != 0) {
    if (*(int *)(param_1 + 0x80) != -1) {
      uVar2 = (**(code **)(*D2_GameSingletonPtr + 0x388))();
      FUN_1401a31f0(uVar2,*(undefined4 *)(param_1 + 0x80));
      *(undefined4 *)(param_1 + 0x80) = 0xffffffff;
    }
    uVar2 = (**(code **)(*D2_GameSingletonPtr + 0x388))();
    lVar3 = FUN_14019d100(uVar2,*(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0xa8),param_2);
    if (lVar3 != 0) {
      lVar1 = *(longlong *)(param_1 + 0x10);
      *(undefined4 *)(lVar3 + 0x38) = *(undefined4 *)(lVar1 + 0x300);
      *(undefined4 *)(lVar3 + 0x3c) = *(undefined4 *)(lVar1 + 0x304);
      *(undefined4 *)(lVar3 + 0x40) = *(undefined4 *)(lVar1 + 0x308);
      lVar1 = *(longlong *)(param_1 + 0x10);
      *(undefined4 *)(lVar3 + 0x44) = *(undefined4 *)(lVar1 + 0x30c);
      *(undefined4 *)(lVar3 + 0x48) = *(undefined4 *)(lVar1 + 0x310);
      *(undefined4 *)(lVar3 + 0x4c) = *(undefined4 *)(lVar1 + 0x314);
      *(undefined4 *)(lVar3 + 0x50) = *(undefined4 *)(lVar1 + 0x318);
      *(undefined4 *)(lVar3 + 0x54) = *(undefined4 *)(lVar1 + 0x31c);
      *(undefined4 *)(lVar3 + 0x58) = *(undefined4 *)(lVar1 + 800);
      *(undefined4 *)(lVar3 + 0x5c) = *(undefined4 *)(lVar1 + 0x324);
      *(undefined4 *)(lVar3 + 0x60) = *(undefined4 *)(lVar1 + 0x328);
      *(undefined4 *)(lVar3 + 100) = *(undefined4 *)(lVar1 + 0x32c);
      uVar5 = FUN_1408780a0(*(undefined8 *)(param_1 + 0x10));
      *(undefined4 *)(lVar3 + 0xf8) = uVar5;
      FUN_1401a2ff0(uVar2,lVar3,0,0x1ffff,0);
      puVar4 = (undefined4 *)FUN_140167a00(lVar3,local_res10);
      *(undefined4 *)(param_1 + 0x80) = *puVar4;
    }
  }
  return;
}



````
