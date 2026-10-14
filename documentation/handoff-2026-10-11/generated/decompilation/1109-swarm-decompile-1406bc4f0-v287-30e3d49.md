# swarm-decompile-1406bc4f0-v287.txt

Original: [work/movement-integration/swarm-decompile-1406bc4f0-v287.txt](<../../../../work/movement-integration/swarm-decompile-1406bc4f0-v287.txt>)

SHA-256: `3404F1217798A2FE5BC58B29145F10B00FF18F208076C79770F89AB2056D190D`; 1191 bytes.

## Original saved record

````text

void FUN_1406bc4f0(longlong param_1,longlong param_2)

{
  undefined4 *puVar1;
  undefined1 auStack_38 [48];
  
  *(undefined4 *)(param_1 + 0x300) = *(undefined4 *)(param_2 + 4);
  *(undefined4 *)(param_1 + 0x304) = *(undefined4 *)(param_2 + 8);
  *(undefined4 *)(param_1 + 0x308) = *(undefined4 *)(param_2 + 0xc);
  puVar1 = (undefined4 *)FUN_14010d490(param_2 + 0x1c,auStack_38);
  *(undefined4 *)(param_1 + 0x30c) = *puVar1;
  *(undefined4 *)(param_1 + 0x310) = puVar1[1];
  *(undefined4 *)(param_1 + 0x314) = puVar1[2];
  *(undefined4 *)(param_1 + 0x318) = puVar1[3];
  *(undefined4 *)(param_1 + 0x31c) = puVar1[4];
  *(undefined4 *)(param_1 + 800) = puVar1[5];
  *(undefined4 *)(param_1 + 0x324) = puVar1[6];
  *(undefined4 *)(param_1 + 0x328) = puVar1[7];
  *(undefined4 *)(param_1 + 0x32c) = puVar1[8];
  *(undefined4 *)(param_1 + 0x33c) = *(undefined4 *)(param_2 + 0x10);
  *(undefined4 *)(param_1 + 0x340) = *(undefined4 *)(param_2 + 0x14);
  *(undefined4 *)(param_1 + 0x344) = *(undefined4 *)(param_2 + 0x18);
  *(undefined4 *)(param_1 + 0x348) = DAT_1431af668;
  *(undefined4 *)(param_1 + 0x34c) = DAT_1431af66c;
  *(undefined4 *)(param_1 + 0x350) = DAT_1431af670;
  return;
}



````
