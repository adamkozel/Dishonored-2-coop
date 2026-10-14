# native-shadowwalk-140820a20-decompile-v518.txt

Original: [work/movement-integration/native-shadowwalk-140820a20-decompile-v518.txt](<../../../../work/movement-integration/native-shadowwalk-140820a20-decompile-v518.txt>)

SHA-256: `4DB3F0FEE27AB5897BAC957DCD704C23EECD3B4A6579FF6F2B9061766F2CAA4A`; 512 bytes.

## Original saved record

````text
void FUN_140820a20(longlong param_1)

{
  undefined8 uVar1;
  undefined1 auStack_38 [48];
  
  uVar1 = *(undefined8 *)(param_1 + 0x10);
  if (*(int *)(param_1 + 0x34) != -1) {
    FUN_140112000(*(longlong *)(param_1 + 0x20) + 0x68,auStack_38);
    FUN_1408122c0(uVar1,*(undefined4 *)(param_1 + 0x34),*(longlong *)(param_1 + 0x20) + 0x5c,
                  auStack_38);
  }
  FUN_14081e8d0(param_1 + 0x40,uVar1);
  if (*(char *)(param_1 + 0x39) != '\0') {
    FUN_14081e8d0(param_1 + 0x50,uVar1);
  }
  return;
}

````
