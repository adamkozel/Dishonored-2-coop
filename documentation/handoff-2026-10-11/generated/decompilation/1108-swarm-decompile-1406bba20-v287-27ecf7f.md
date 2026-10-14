# swarm-decompile-1406bba20-v287.txt

Original: [work/movement-integration/swarm-decompile-1406bba20-v287.txt](<../../../../work/movement-integration/swarm-decompile-1406bba20-v287.txt>)

SHA-256: `3886D493627BA19C1CC786898A09C945B272344107307D39A0B8797170200A6A`; 1204 bytes.

## Original saved record

````text

void FUN_1406bba20(longlong *param_1,float *param_2,longlong param_3)

{
  float fVar1;
  float fVar2;
  float fVar3;
  longlong lVar4;
  float fVar5;
  undefined1 auStack_b8 [36];
  undefined4 uStack_94;
  undefined4 uStack_90;
  undefined4 uStack_8c;
  float fStack_88;
  float fStack_84;
  float fStack_80;
  undefined4 uStack_68;
  
  if ((int)param_1[9] - 5U < 2) {
    D2_DamageParams_Construct(auStack_b8,DAT_14231f330);
    lVar4 = param_1[2];
    fVar1 = param_2[1];
    fVar2 = *param_2;
    fVar3 = param_2[2];
    uStack_94 = *(undefined4 *)(lVar4 + 0x300);
    uStack_8c = *(undefined4 *)(lVar4 + 0x308);
    uStack_90 = *(undefined4 *)(lVar4 + 0x304);
    fVar5 = fVar2 * fVar2 + fVar1 * fVar1 + fVar3 * fVar3;
    if (fVar5 <= 1.1754944e-38) {
      fVar5 = 1.1754944e-38;
    }
    fVar5 = 1.0 / SQRT(fVar5);
    fStack_88 = -(fVar2 * fVar5);
    fStack_84 = -(fVar1 * fVar5);
    fStack_80 = -(fVar3 * fVar5);
    if (param_3 != 0) {
      uStack_68 = *(undefined4 *)(param_3 + 0xa8);
    }
    FUN_1406b8b70(param_1,auStack_b8,2);
  }
  else {
    if ((int)param_1[9] != 4) {
      (**(code **)(*param_1 + 0x80))(param_1,4);
    }
    FUN_1406b8400(param_1,param_2);
  }
  return;
}



````
