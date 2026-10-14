# swarm-decompile-1406b3d20-v279.txt

Original: [work/movement-integration/swarm-decompile-1406b3d20-v279.txt](<../../../../work/movement-integration/swarm-decompile-1406b3d20-v279.txt>)

SHA-256: `1D2A4A8EB4A4FAD08203ACB72314DF87E01BD49267CE69876AB727DAB4EE8E1D`; 1658 bytes.

## Original saved record

````text

/* WARNING: Removing unreachable block (ram,0x000140ff2776) */

void FUN_1406b3d20(longlong param_1)

{
  int iVar1;
  longlong *plVar2;
  longlong lVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  uint uVar7;
  undefined8 uVar8;
  longlong *plVar9;
  longlong lVar10;
  undefined1 auStackX_18 [8];
  
  FUN_140ff2750(param_1,param_1 + 0x4b8);
  FUN_140ff2750(param_1,param_1 + 0x1b8);
  FUN_140ff2750(param_1,param_1 + 0x2e8);
  FUN_140ff2750(param_1,param_1 + 0x520);
  plVar2 = (longlong *)(param_1 + 0x3f0);
  iVar4 = *(int *)(param_1 + 0x28);
  if (plVar2 != (longlong *)0x0) {
    FUN_140a9e120(plVar2);
  }
  uVar8 = FUN_140a9de90();
  iVar5 = *(int *)(param_1 + 0x28);
  iVar1 = iVar5 + 1;
  uVar7 = *(uint *)(param_1 + 0x2c) & 0x3fffffff;
  if (((int)uVar7 < iVar1) && ((int)uVar7 < iVar1)) {
    iVar6 = iVar1;
    if (iVar1 < (int)(uVar7 * 2)) {
      iVar6 = uVar7 * 2;
    }
    FUN_140a9e6a0(auStackX_18,uVar8,param_1 + 0x20,iVar6,8);
  }
  lVar3 = (longlong)iVar4 * 8;
  lVar10 = *(longlong *)(param_1 + 0x20) + lVar3;
  FUN_140ae3e00(lVar10 + 8,lVar10,(longlong)(iVar5 - iVar4) << 3);
  plVar9 = (longlong *)(*(longlong *)(param_1 + 0x20) + lVar3);
  if ((plVar9 != (longlong *)0x0) && (*plVar9 = (longlong)plVar2, plVar2 != (longlong *)0x0)) {
    FUN_140a9e120(plVar2);
  }
  *(int *)(param_1 + 0x28) = iVar1;
  if (plVar2 != (longlong *)0x0) {
    FUN_140a9e180(plVar2);
  }
  *(longlong *)(param_1 + 0x410) = param_1;
                    /* WARNING: Could not recover jumptable at 0x000140ff283c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(*plVar2 + 0x30))(plVar2);
  return;
}



````
