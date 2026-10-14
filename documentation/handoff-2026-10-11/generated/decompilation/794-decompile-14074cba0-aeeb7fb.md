# decompile-14074cba0.txt

Original: [work/movement-integration/door-research-v639/decompile-14074cba0.txt](<../../../../work/movement-integration/door-research-v639/decompile-14074cba0.txt>)

SHA-256: `A9B3C35B5C2DD4B6BFCC785678481540EA4A8F3DD5F9EA4982BA7AB9515EE1A5`; 1628 bytes.

## Original saved record

````text

void FUN_14074cba0(longlong param_1,longlong *param_2)

{
  int iVar1;
  undefined8 uVar2;
  longlong *plVar3;
  longlong lVar4;
  undefined1 uVar5;
  undefined1 auStack_78 [8];
  undefined4 uStack_70;
  undefined1 auStack_60 [8];
  uint uStack_58;
  undefined8 uStack_48;
  undefined1 auStack_40 [24];
  undefined1 auStack_28 [32];
  
  uStack_48 = 0xfffffffffffffffe;
  if (param_2 != (longlong *)0x0) {
    iVar1 = (**(code **)(*param_2 + 0x28))(param_2);
    if (iVar1 == 1) {
      auStack_60[0] = 0x69;
      uStack_58 = (uint)*(byte *)((longlong)param_2 + 0x12);
      auStack_78[0] = 0x69;
      uStack_70 = 1;
      uVar2 = FUN_1406dc560(*(undefined4 *)(*(longlong *)(param_1 + 0x18) + 0x114));
      uVar2 = FUN_140882fa0(auStack_28,uVar2);
      FUN_140884820(*(undefined8 *)(*(longlong *)(param_1 + 0x18) + 0x10),auStack_40,&DAT_1439aca20,
                    uVar2,auStack_78,auStack_60);
      FUN_140882fc0(auStack_40);
      lVar4 = *(longlong *)(param_1 + 0x18);
      if ((*(byte *)(*(longlong *)(lVar4 + 0x10) + 0xd5) & 4) != 0) {
        if (((*(int *)(lVar4 + 0x48) == 0) && (*(char *)(lVar4 + 0x1ea) == '\0')) &&
           (*(char *)(lVar4 + 0x1ed) == '\0')) {
          uVar5 = 0;
        }
        else {
          uVar5 = 1;
        }
        plVar3 = (longlong *)(**(code **)(*D2_GameSingletonPtr + 0x50))();
        (**(code **)(*plVar3 + 0x110))(plVar3,*(undefined4 *)(lVar4 + 0x110),uVar5);
      }
      FUN_14074eaf0(*(undefined8 *)(param_1 + 0x18));
    }
  }
  lVar4 = FUN_1403da910(*(longlong *)(param_1 + 0x18) + 0x100,1);
  if (lVar4 != 0) {
    FUN_140738890(lVar4,0,1);
  }
  return;
}



````
