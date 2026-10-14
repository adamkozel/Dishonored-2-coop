# native-shadowwalk-140820730-decompile-v518.txt

Original: [work/movement-integration/native-shadowwalk-140820730-decompile-v518.txt](<../../../../work/movement-integration/native-shadowwalk-140820730-decompile-v518.txt>)

SHA-256: `B910FC5501655F741056230A1C93E85FCE14C045D68F8BD842DD854A2C5DB7B4`; 809 bytes.

## Original saved record

````text
void FUN_140820730(longlong param_1)

{
  undefined8 uVar1;
  
  uVar1 = *(undefined8 *)(param_1 + 0x10);
  if (*(int *)(param_1 + 0x2c) != -1) {
    FUN_140812290(uVar1,*(undefined4 *)(param_1 + 0x2c));
    *(undefined4 *)(param_1 + 0x2c) = 0xffffffff;
  }
  FUN_14081e850(param_1 + 0x40,uVar1);
  if (*(char *)(param_1 + 0x39) != '\0') {
    FUN_14081e850(param_1 + 0x50,uVar1);
  }
  if (*(int *)(param_1 + 0x30) != -1) {
    FUN_140812290(uVar1,*(undefined4 *)(param_1 + 0x30));
    *(undefined4 *)(param_1 + 0x30) = 0xffffffff;
  }
  if (*(char *)(param_1 + 0x38) != '\0') {
    FUN_140812290(uVar1,*(undefined4 *)(param_1 + 0x34));
    *(undefined1 *)(param_1 + 0x38) = 0;
  }
  *(undefined4 *)(param_1 + 0x34) = 0xffffffff;
  FUN_14081e4d0(param_1 + 0x40);
  FUN_14081e4d0(param_1 + 0x50);
  return;
}

````
