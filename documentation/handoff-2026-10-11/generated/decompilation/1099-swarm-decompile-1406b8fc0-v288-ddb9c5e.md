# swarm-decompile-1406b8fc0-v288.txt

Original: [work/movement-integration/swarm-decompile-1406b8fc0-v288.txt](<../../../../work/movement-integration/swarm-decompile-1406b8fc0-v288.txt>)

SHA-256: `D846874BAB8DB9F4ECBD493DE5470A37BE0892AF2466BFFA592245C2355A2D33`; 1949 bytes.

## Original saved record

````text

void FUN_1406b8fc0(longlong param_1,int param_2)

{
  byte bVar1;
  undefined4 uVar2;
  char cVar3;
  longlong lVar4;
  undefined8 *puVar5;
  undefined8 uVar6;
  longlong lVar7;
  byte bVar8;
  longlong *plVar9;
  
  if (param_2 != 0) {
    uVar2 = *(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0xa8);
    lVar4 = FUN_140810240();
    FUN_1408112c0(lVar4 + 0xc8e8,0x129,0,uVar2);
    return;
  }
  lVar4 = FUN_1403da910(param_1 + 0x98,2);
  if (lVar4 != 0) {
    FUN_1408762b0(*(undefined8 *)(lVar4 + 0x10),*(undefined8 *)(param_1 + 0x10));
    FUN_1406aeb40(lVar4,*(undefined4 *)(param_1 + 0xb8));
  }
  lVar4 = FUN_1408783d0();
  if (lVar4 == 0) {
    lVar7 = 0;
  }
  else {
    lVar7 = lVar4 + 0x20;
  }
  if (*(longlong *)(lVar4 + 0xd8) != lVar7) {
    return;
  }
  lVar4 = FUN_1408783d0(*(undefined8 *)(param_1 + 0x10));
  *(longlong *)(lVar4 + 0xd8) = param_1;
  *(longlong *)(param_1 + 8) = lVar4;
  cVar3 = FUN_140751140();
  if (cVar3 != '\0') {
    FUN_140752260(lVar4);
  }
  bVar1 = *(byte *)(lVar4 + 0x408);
  if (*(longlong *)(lVar4 + 0xd8) == lVar4 + 0x20) {
    bVar8 = 0;
    puVar5 = (undefined8 *)D2_Entity_GetChildren(*(undefined8 *)(lVar4 + 0x10));
    lVar7 = 0;
    if ((*(uint *)(puVar5 + 1) & 0xffffff) != 0) {
      plVar9 = (longlong *)*puVar5;
      do {
        if ((*(byte *)(*(longlong *)(*plVar9 + 0x358) + 0x408) & 0x20) != 0) goto LAB_1407510e1;
        lVar7 = lVar7 + 1;
        plVar9 = plVar9 + 1;
      } while (lVar7 < (longlong)(ulonglong)(*(uint *)(puVar5 + 1) & 0xffffff));
    }
  }
  else {
LAB_1407510e1:
    bVar8 = 1;
  }
  if ((bVar1 >> 5 & 1) != bVar8) {
    *(byte *)(lVar4 + 0x408) = *(byte *)(lVar4 + 0x408) & 0xdf;
    *(byte *)(lVar4 + 0x408) = *(byte *)(lVar4 + 0x408) | bVar8 << 5;
    if ((*(longlong *)(lVar4 + 0x3c8) != 0) &&
       (*(longlong *)(*(longlong *)(lVar4 + 0x3c8) + 0x358) != 0)) {
      uVar6 = FUN_1408783d0();
      FUN_140751e80(uVar6,bVar8);
    }
  }
  return;
}



````
