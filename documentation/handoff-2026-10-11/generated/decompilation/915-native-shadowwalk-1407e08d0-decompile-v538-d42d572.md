# native-shadowwalk-1407e08d0-decompile-v538.txt

Original: [work/movement-integration/native-shadowwalk-1407e08d0-decompile-v538.txt](<../../../../work/movement-integration/native-shadowwalk-1407e08d0-decompile-v538.txt>)

SHA-256: `9981BC7A441E7B0D50C91446D87E9DDCBF20102DBC43D6E77000E3A6E17BD0D8`; 1607 bytes.

## Original saved record

````text

void FUN_1407e08d0(longlong param_1,undefined8 param_2,longlong param_3,uint param_4)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  int iVar5;
  int *piVar6;
  int *piVar7;
  
  *(undefined8 *)(param_1 + 8) = param_2;
  if ((((param_4 & 0xf) == 3) && (iVar2 = *(int *)(param_3 + 0x24), 0 < iVar2)) &&
     (((uint)(int)*(short *)(param_1 + 0xba) >> 3 & 1) == 0)) {
    iVar3 = *(int *)(param_3 + 8);
    uVar4 = *(uint *)(param_1 + 0x20);
    iVar5 = *(int *)(param_3 + 4);
    piVar6 = *(int **)(param_1 + 0x18);
    if (piVar6 != piVar6 + (ulonglong)(uVar4 & 0xffffff) * 6) {
      piVar7 = piVar6 + 4;
      do {
        if ((piVar7[-1] <= iVar5 + 1 + (iVar3 - iVar2)) &&
           (*(int *)(param_3 + 8) + *(int *)(param_3 + 4) <= *piVar7)) {
          *piVar7 = *piVar7 + iVar2;
        }
        piVar1 = piVar7 + 2;
        piVar7 = piVar7 + 6;
      } while (piVar1 != piVar6 + (ulonglong)(uVar4 & 0xffffff) * 6);
      return;
    }
  }
  else if (((param_4 & 0xf) == 4) && ((param_4 & 0xf00) != 0xb00)) {
    iVar2 = *(int *)(param_3 + 0x14);
    iVar3 = *(int *)(param_3 + 0x24);
    piVar6 = *(int **)(param_1 + 0x18);
    iVar5 = *(int *)(param_3 + 0x18);
    uVar4 = *(uint *)(param_1 + 0x20);
    if (piVar6 != piVar6 + (ulonglong)(uVar4 & 0xffffff) * 6) {
      piVar7 = piVar6 + 4;
      do {
        if ((piVar7[-1] <= iVar5) && (iVar5 + -1 + (iVar2 - iVar3) <= *piVar7)) {
          *piVar7 = *piVar7 + iVar3;
        }
        piVar1 = piVar7 + 2;
        piVar7 = piVar7 + 6;
      } while (piVar1 != piVar6 + (ulonglong)(uVar4 & 0xffffff) * 6);
    }
  }
  return;
}



````
