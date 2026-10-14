# swarm-decompile-14053edb0-v297.txt

Original: [work/movement-integration/swarm-decompile-14053edb0-v297.txt](<../../../../work/movement-integration/swarm-decompile-14053edb0-v297.txt>)

SHA-256: `DCD46F6DA9FDE1BC9FD1568D1FFDD041B41D3FDF8F146205F5652974CFE6BEE0`; 866 bytes.

## Original saved record

````text

void FUN_14053edb0(longlong param_1)

{
  undefined8 uVar1;
  
  FUN_1407e7670();
  if (*(undefined8 **)(param_1 + 0x400) != (undefined8 *)0x0) {
    *(undefined8 *)(*(longlong *)(param_1 + 0x38) + 0x1ff0) = **(undefined8 **)(param_1 + 0x400);
    FUN_140541480(param_1,*(undefined8 *)
                           (*(longlong *)(*(longlong *)(param_1 + 0x38) + 0x1fe8) + 0x50));
    FUN_1400f1520(*(undefined8 *)(param_1 + 0x400));
    *(undefined8 *)(param_1 + 0x400) = 0;
  }
  if (**(char **)(*(longlong *)(param_1 + 8) + 0x90) != '\0') {
    FUN_14053efa0(param_1,0,*(undefined8 *)(*(longlong *)(param_1 + 8) + 0x90),0x3f800000,3,
                  0x3f800000,0,0);
  }
  uVar1 = FUN_140877720(*(undefined8 *)(param_1 + 0x10),&DAT_1439ee0a0,1);
  FUN_1403dab80(param_1 + 0x3f0,uVar1);
  *(byte *)(param_1 + 0x3ee) = *(byte *)(param_1 + 0x3ee) | 2;
  return;
}



````
