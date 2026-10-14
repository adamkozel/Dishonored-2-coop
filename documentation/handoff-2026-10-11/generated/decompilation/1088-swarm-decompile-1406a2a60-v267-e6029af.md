# swarm-decompile-1406a2a60-v267.txt

Original: [work/movement-integration/swarm-decompile-1406a2a60-v267.txt](<../../../../work/movement-integration/swarm-decompile-1406a2a60-v267.txt>)

SHA-256: `45C0C37FFEEA1715C575B1BE99CDD8A8E349E360FDCFAB42A1C64EF91B414FF0`; 1289 bytes.

## Original saved record

````text

void FUN_1406a2a60(longlong param_1,longlong param_2,undefined4 param_3,uint param_4)

{
  longlong lVar1;
  char cVar2;
  longlong lVar3;
  int iVar4;
  bool bVar5;
  float fVar6;
  
  cVar2 = FUN_1406a9840();
  FUN_1406a7200(param_1,param_2,param_3,param_4);
  lVar1 = *(longlong *)(param_1 + 8);
  lVar3 = FUN_1408a8ba0(D2_GameSingletonPtr);
  iVar4 = *(int *)(*(longlong *)(lVar3 + 0x408) + 0x10c);
  if (iVar4 < 0) {
    iVar4 = 0;
  }
  else {
    fVar6 = (float)FUN_1411ad95c(*(float *)(*(longlong *)(*(longlong *)(lVar3 + 0x408) + 0x48) +
                                            0x24 + (longlong)iVar4 * 0x28) + 0.5);
    iVar4 = (int)fVar6;
  }
  iVar4 = *(int *)(lVar1 + 0x440) + iVar4;
  if (iVar4 < 0) {
    iVar4 = 0;
  }
  else if (100 < iVar4) {
    iVar4 = 100;
  }
  bVar5 = (int)(iVar4 * param_4) / 100 != (int)((param_4 + 1) * iVar4) / 100;
  if (cVar2 == '\0') {
    *(undefined4 *)(param_2 + 4) = DAT_1431af620;
    *(undefined4 *)(param_2 + 8) = DAT_1431af624;
    *(undefined4 *)(param_2 + 0xc) = DAT_1431af628;
  }
  else {
    *(undefined1 *)(param_2 + 0x2d) = 1;
  }
  *(bool *)(param_2 + 0x30) = bVar5;
  if ((bVar5) || ((param_4 & 1) == 0)) {
    *(undefined1 *)(param_2 + 0x31) = 0;
  }
  else {
    *(undefined1 *)(param_2 + 0x31) = 1;
  }
  return;
}



````
