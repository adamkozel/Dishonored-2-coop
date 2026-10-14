# native-shadowwalk-140820680-decompile-v514.txt

Original: [work/movement-integration/candidate-shadowwalk-inventory-v515/native-shadowwalk-140820680-decompile-v514.txt](<../../../../work/movement-integration/candidate-shadowwalk-inventory-v515/native-shadowwalk-140820680-decompile-v514.txt>)

SHA-256: `66BCB6BCAAC65F97BFB56DEC5EB6F1189F41F9FED7F03E4D1A60B986348F3117`; 747 bytes.

## Original saved record

````text
void FUN_140820680(longlong param_1)

{
  longlong lVar1;
  longlong lVar2;
  undefined8 uVar3;
  undefined4 *puVar4;
  undefined1 auStackX_8 [8];
  
  lVar1 = *(longlong *)(param_1 + 0x10);
  lVar2 = *(longlong *)(param_1 + 0x18);
  uVar3 = *(undefined8 *)(*(longlong *)(lVar1 + 0x28) + 0x300);
  FUN_14081e420(param_1 + 0x40,uVar3,lVar2 + 0x108);
  FUN_14081e420(param_1 + 0x50,uVar3,lVar2 + 0x120);
  *(undefined4 *)(param_1 + 0x28) = 0;
  if (*(longlong *)(*(longlong *)(param_1 + 0x18) + 0xc0) != 0) {
    puVar4 = (undefined4 *)FUN_1408120b0(lVar1,auStackX_8);
    *(undefined4 *)(param_1 + 0x2c) = *puVar4;
  }
  FUN_14081e7d0(param_1 + 0x40,lVar1);
  *(undefined2 *)(param_1 + 0x38) = 0;
  *(undefined1 *)(param_1 + 0x3a) = 0;
  return;
}

````
