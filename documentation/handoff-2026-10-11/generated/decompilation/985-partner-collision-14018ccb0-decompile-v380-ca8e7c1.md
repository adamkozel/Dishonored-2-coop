# partner-collision-14018ccb0-decompile-v380.txt

Original: [work/movement-integration/partner-collision-14018ccb0-decompile-v380.txt](<../../../../work/movement-integration/partner-collision-14018ccb0-decompile-v380.txt>)

SHA-256: `64FAE7D9EDD23E05C92754F8F1AF3FBBD73A6B7975287D2AE579CF41CD4ADC00`; 407 bytes.

## Original saved record

````text

void FUN_14018ccb0(longlong param_1)

{
  longlong *plVar1;
  
  plVar1 = (longlong *)FUN_140a9de90();
  *(undefined4 *)(param_1 + 0x68) = 0;
  if (-1 < *(int *)(param_1 + 0x6c)) {
    (**(code **)(*plVar1 + 0x20))
              (plVar1,*(undefined8 *)(param_1 + 0x60),*(int *)(param_1 + 0x6c) << 3);
  }
  *(undefined4 *)(param_1 + 0x6c) = 0x80000000;
  *(undefined8 *)(param_1 + 0x60) = 0;
  return;
}



````
