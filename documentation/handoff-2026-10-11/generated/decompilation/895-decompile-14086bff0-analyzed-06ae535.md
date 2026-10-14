# decompile-14086bff0-analyzed.txt

Original: [work/movement-integration/native-kick-research-v624/decompile-14086bff0-analyzed.txt](<../../../../work/movement-integration/native-kick-research-v624/decompile-14086bff0-analyzed.txt>)

SHA-256: `659AC97529AB18EBA72C644DE9E521CFBAC9DF1C7524282FB8917CCC102BEBC5`; 428 bytes.

## Original saved record

````text

int * FUN_14086bff0(longlong param_1,int param_2)

{
  int *piVar1;
  longlong lVar2;
  uint uVar3;
  
  lVar2 = 0;
  uVar3 = *(uint *)(param_1 + 0x58) & 0xffffff;
  if (uVar3 != 0) {
    piVar1 = *(int **)(param_1 + 0x50);
    do {
      if (*piVar1 == param_2) {
        return piVar1;
      }
      lVar2 = lVar2 + 1;
      piVar1 = piVar1 + 3;
    } while (lVar2 < (longlong)(ulonglong)uVar3);
  }
  return (int *)0x0;
}



````
