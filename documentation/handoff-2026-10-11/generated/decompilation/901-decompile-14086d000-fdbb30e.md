# decompile-14086d000.txt

Original: [work/movement-integration/native-kick-research-v624/decompile-14086d000.txt](<../../../../work/movement-integration/native-kick-research-v624/decompile-14086d000.txt>)

SHA-256: `E66958978EF83073EC261C66825540BB6C63F6719EE33C8E9710D7D74BBB77EB`; 385 bytes.

## Original saved record

````text

undefined8 FUN_14086d000(longlong param_1,undefined8 param_2)

{
  uint uVar1;
  int iVar2;
  
  uVar1 = *(uint *)(param_1 + 0x5c) >> 6 & 3;
  if (((uVar1 != 0) && (uVar1 != 3)) &&
     (*(uint *)(param_1 + 0x5c) >> 8 <= (*(uint *)(param_1 + 0x58) & 0xffffff))) {
    return 0;
  }
  iVar2 = FUN_14086ad50(param_1 + 0x50,param_2);
  if (iVar2 < 0) {
    return 0;
  }
  return 1;
}



````
