# native-doppelganger-fx-14084c9e0-decompile-v436.txt

Original: [work/movement-integration/native-doppelganger-fx-14084c9e0-decompile-v436.txt](<../../../../work/movement-integration/native-doppelganger-fx-14084c9e0-decompile-v436.txt>)

SHA-256: `4CA7C82EDE06099DB10C5EFCF6466559CDD325A0056502FED065D67B4F1E40F7`; 2010 bytes.

## Original saved record

````text

void FUN_14084c9e0(longlong param_1,longlong *param_2)

{
  longlong lVar1;
  int *piVar2;
  longlong lVar3;
  longlong lVar4;
  int *piVar5;
  longlong *plVar6;
  uint uVar7;
  longlong *plVar8;
  uint uVar9;
  
  plVar6 = (longlong *)*param_2;
  plVar8 = plVar6 + (ulonglong)(*(uint *)(param_2 + 1) & 0xffffff) * 2;
  do {
    if (plVar6 == plVar8) {
      return;
    }
    uVar7 = 0;
    lVar4 = 0;
    uVar9 = *(uint *)(param_1 + 200) & 0xffffff;
    if (uVar9 != 0) {
      piVar2 = *(int **)(param_1 + 0xc0);
      piVar5 = piVar2;
      do {
        if (*piVar5 == *(int *)(*plVar6 + 8)) {
          if (-1 < lVar4) {
            lVar1 = lVar4 * 0xc;
            lVar3 = FUN_1406dc560(piVar2[lVar4 * 3 + 1]);
            if ((lVar3 != 0) && (lVar3 = FUN_140877720(lVar3,&DAT_1439d1200,0), lVar3 != 0)) {
              if (piVar2[lVar4 * 3 + 2] == 1) {
                FUN_1404b2040(lVar3);
              }
              else if (piVar2[lVar4 * 3 + 2] == 2) {
                FUN_1404b1fd0(lVar3);
              }
            }
            uVar9 = *(uint *)(param_1 + 200);
            if ((int)uVar7 < (int)(uVar9 & 0xffffff)) {
              uVar9 = (uVar9 - 1 ^ uVar9) & 0xffffff ^ uVar9;
              *(uint *)(param_1 + 200) = uVar9;
              if (uVar7 != (uVar9 & 0xffffff)) {
                lVar3 = *(longlong *)(param_1 + 0xc0);
                lVar4 = lVar3 + (ulonglong)(uVar9 & 0xffffff) * 0xc;
                *(undefined4 *)(lVar3 + lVar1) =
                     *(undefined4 *)(lVar3 + (ulonglong)(uVar9 & 0xffffff) * 0xc);
                *(undefined4 *)(lVar3 + 4 + lVar1) = *(undefined4 *)(lVar4 + 4);
                *(undefined4 *)(lVar3 + 8 + lVar1) = *(undefined4 *)(lVar4 + 8);
              }
            }
          }
          break;
        }
        lVar4 = lVar4 + 1;
        uVar7 = uVar7 + 1;
        piVar5 = piVar5 + 3;
      } while (lVar4 < (longlong)(ulonglong)uVar9);
    }
    *(undefined4 *)(plVar6 + 1) = 2;
    plVar6 = plVar6 + 2;
  } while( true );
}



````
