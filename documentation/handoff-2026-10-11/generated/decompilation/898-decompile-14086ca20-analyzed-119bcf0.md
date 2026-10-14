# decompile-14086ca20-analyzed.txt

Original: [work/movement-integration/native-kick-research-v624/decompile-14086ca20-analyzed.txt](<../../../../work/movement-integration/native-kick-research-v624/decompile-14086ca20-analyzed.txt>)

SHA-256: `1FB15F2DD71B734D1A2B291880F1C6C8182C1FB51D4826CF67C3140A47EA773C`; 1214 bytes.

## Original saved record

````text

undefined8 FUN_14086ca20(longlong param_1,int param_2)

{
  uint uVar1;
  undefined4 *puVar2;
  int iVar3;
  int *piVar4;
  longlong lVar5;
  
  iVar3 = 0;
  lVar5 = 0;
  uVar1 = *(uint *)(param_1 + 0x58) & 0xffffff;
  if (uVar1 != 0) {
    piVar4 = *(int **)(param_1 + 0x50);
    do {
      if (*piVar4 == param_2) {
        if (-1 < iVar3) {
          uVar1 = *(uint *)(param_1 + 0x58);
          if (iVar3 < (int)(uVar1 & 0xffffff)) {
            uVar1 = (uVar1 - 1 ^ uVar1) & 0xffffff ^ uVar1;
            *(uint *)(param_1 + 0x58) = uVar1;
            if (iVar3 < (int)(uVar1 & 0xffffff)) {
              lVar5 = (longlong)iVar3 * 0xc;
              do {
                iVar3 = iVar3 + 1;
                puVar2 = (undefined4 *)(*(longlong *)(param_1 + 0x50) + lVar5);
                lVar5 = lVar5 + 0xc;
                *puVar2 = puVar2[3];
                puVar2[1] = puVar2[4];
                puVar2[2] = puVar2[5];
              } while (iVar3 < (int)(*(uint *)(param_1 + 0x58) & 0xffffff));
            }
          }
        }
        return 1;
      }
      lVar5 = lVar5 + 1;
      iVar3 = iVar3 + 1;
      piVar4 = piVar4 + 3;
    } while (lVar5 < (longlong)(ulonglong)uVar1);
  }
  return 0;
}



````
