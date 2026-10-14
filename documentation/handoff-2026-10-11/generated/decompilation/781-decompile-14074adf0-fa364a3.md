# decompile-14074adf0.txt

Original: [work/movement-integration/door-native-api-v712/decompile-14074adf0.txt](<../../../../work/movement-integration/door-native-api-v712/decompile-14074adf0.txt>)

SHA-256: `40CF700ECA09C0A17320F7C55C81A98D6F7027BB926ACC47AFE8567F635AAAC1`; 1014 bytes.

## Original saved record

````text

void FUN_14074adf0(longlong param_1)

{
  undefined1 uVar1;
  longlong lVar2;
  longlong *plVar3;
  undefined8 uVar4;
  char cVar5;
  longlong lVar6;
  
  if (*(int *)(param_1 + 0x30) == 0) {
    lVar2 = *(longlong *)(param_1 + 8);
    if (*(int *)(lVar2 + 8) != 0) {
      plVar3 = (longlong *)FUN_1406df6d0(lVar2);
      uVar4 = FUN_1406df6d0(lVar2,0);
      if ((plVar3 == (longlong *)0x0) ||
         (cVar5 = (**(code **)(*plVar3 + 0x20))(plVar3,uVar4), cVar5 != '\0')) {
        *(undefined4 *)(lVar2 + 0xc) = 0;
        return;
      }
    }
  }
  else {
    lVar2 = *(longlong *)(param_1 + 8);
    uVar1 = *(undefined1 *)(param_1 + 0x12);
    if (*(int *)(lVar2 + 8) != 2) {
      plVar3 = (longlong *)FUN_1406df6d0(lVar2);
      lVar6 = FUN_1406df6d0(lVar2,2);
      if ((plVar3 == (longlong *)0x0) ||
         (cVar5 = (**(code **)(*plVar3 + 0x20))(plVar3,lVar6), cVar5 != '\0')) {
        *(undefined1 *)(lVar6 + 0x10) = uVar1;
        *(undefined4 *)(lVar2 + 0xc) = 2;
      }
    }
  }
  return;
}



````
