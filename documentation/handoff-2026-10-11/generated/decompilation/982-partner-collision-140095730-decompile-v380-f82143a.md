# partner-collision-140095730-decompile-v380.txt

Original: [work/movement-integration/partner-collision-140095730-decompile-v380.txt](<../../../../work/movement-integration/partner-collision-140095730-decompile-v380.txt>)

SHA-256: `1EC916BF37BBD693BBE3504CB7C01E6466FCF0DAA6180741BCEB7E0D125A5A4E`; 474 bytes.

## Original saved record

````text

void FUN_140095730(longlong param_1,uint param_2,undefined4 param_3,undefined1 param_4)

{
  undefined8 uVar1;
  uint local_res10 [2];
  undefined4 local_res18 [4];
  
  local_res10[0] = param_2;
  local_res18[0] = param_3;
  uVar1 = FUN_140017d10((ulonglong)(param_2 & 0xffffff) * 0xb0 +
                        *(longlong *)(*(longlong *)(param_1 + 400) + 0x28));
  FUN_14018cf60(uVar1,local_res18,param_4);
  FUN_14001d6b0(param_1 + 0x148370,local_res10);
  return;
}



````
