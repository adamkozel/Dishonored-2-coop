# native-doppelganger-fx-1407dc130-decompile-v438.txt

Original: [work/movement-integration/candidate-midnight-fx-probe-v445/native-doppelganger-fx-1407dc130-decompile-v438.txt](<../../../../work/movement-integration/candidate-midnight-fx-probe-v445/native-doppelganger-fx-1407dc130-decompile-v438.txt>)

SHA-256: `53290E68D24D4D0E7F93C4EB6FBDD2EFC9C19A5EEF231462BBE7D23DEF296C3D`; 831 bytes.

## Original saved record

````text

void FUN_1407dc130(longlong param_1,undefined8 param_2,char param_3)

{
  byte bVar1;
  
  *(undefined8 *)(param_1 + 0xd0) = param_2;
  bVar1 = FUN_1401934f0(*(undefined8 *)(param_1 + 0xb8));
  *(byte *)(param_1 + 0x1ac) = *(byte *)(param_1 + 0x1ac) & 0xdf;
  *(byte *)(param_1 + 0x1ac) = *(byte *)(param_1 + 0x1ac) | (bVar1 & 1) << 5;
  bVar1 = FUN_1401934e0(*(undefined8 *)(param_1 + 0xb8));
  *(byte *)(param_1 + 0x1ac) = *(byte *)(param_1 + 0x1ac) & 0xbf;
  *(byte *)(param_1 + 0x1ac) = *(byte *)(param_1 + 0x1ac) | (bVar1 & 1) << 6;
  *(byte *)(param_1 + 0x1ad) =
       *(byte *)(param_1 + 0x1ad) ^
       (*(byte *)(*(longlong *)(param_1 + 0xb8) + 0x280) ^ *(byte *)(param_1 + 0x1ad)) & 1;
  *(byte *)(param_1 + 0x1ac) =
       (param_3 << 4 ^ *(byte *)(param_1 + 0x1ac)) & 0x10 ^ *(byte *)(param_1 + 0x1ac);
  return;
}



````
