# swarm-decompile-1406b9750-defined-v276.txt

Original: [work/movement-integration/swarm-decompile-1406b9750-defined-v276.txt](<../../../../work/movement-integration/swarm-decompile-1406b9750-defined-v276.txt>)

SHA-256: `FFCD345FF6C6FB9E6EB7DBC86A491156BF1420BA3E327E641220B3595812FE48`; 642 bytes.

## Original saved record

````text

void FUN_1406b9750(longlong param_1,undefined8 param_2)

{
  longlong lVar1;
  
  *(undefined8 *)(param_1 + 0x20) = param_2;
  if (*(int *)(param_1 + 0x84) != 7) {
    *(undefined4 *)(param_1 + 0x84) = 0;
  }
  lVar1 = *(longlong *)(param_1 + 0x28);
  if (lVar1 != 0) {
    *(uint *)(lVar1 + 0x8c) =
         *(uint *)(lVar1 + 0x8c) ^
         ((int)*(short *)(lVar1 + 0x8c) - 1U ^ *(uint *)(lVar1 + 0x8c)) & 0xffff;
  }
  *(undefined8 *)(param_1 + 0x28) = 0;
  lVar1 = *(longlong *)(*(longlong *)(param_1 + 0x18) + 8);
  *(undefined2 *)(param_1 + 0x30) = 0;
  *(undefined4 *)(param_1 + 0x40) = *(undefined4 *)(lVar1 + 0x200);
  return;
}



````
