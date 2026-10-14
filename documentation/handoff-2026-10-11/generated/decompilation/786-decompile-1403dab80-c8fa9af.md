# decompile-1403dab80.txt

Original: [work/movement-integration/door-research-v639/decompile-1403dab80.txt](<../../../../work/movement-integration/door-research-v639/decompile-1403dab80.txt>)

SHA-256: `73A95B616E644B5318AD01952CA8C3C635C64659E2EF525891CAF3D97E34A351`; 304 bytes.

## Original saved record

````text

void FUN_1403dab80(undefined4 *param_1,longlong param_2)

{
  undefined4 uVar1;
  
  if (param_2 == 0) {
    *param_1 = 0xffffffff;
    *(undefined8 *)(param_1 + 2) = 0;
    return;
  }
  uVar1 = *(undefined4 *)(param_2 + 0x18);
  *(longlong *)(param_1 + 2) = param_2;
  *param_1 = uVar1;
  return;
}



````
