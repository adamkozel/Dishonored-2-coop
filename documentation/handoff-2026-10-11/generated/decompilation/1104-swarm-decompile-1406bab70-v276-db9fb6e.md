# swarm-decompile-1406bab70-v276.txt

Original: [work/movement-integration/swarm-decompile-1406bab70-v276.txt](<../../../../work/movement-integration/swarm-decompile-1406bab70-v276.txt>)

SHA-256: `30532505909B3E461BD8D82C21BC8778B6DA4F6566A3CBF92E3811D6F274426E`; 1170 bytes.

## Original saved record

````text

void FUN_1406bab70(longlong param_1,undefined4 *param_2)

{
  undefined4 uVar1;
  longlong lVar2;
  longlong lVar3;
  undefined4 uVar4;
  longlong lVar5;
  undefined4 *puVar6;
  
  lVar2 = *(longlong *)(*(longlong *)(param_1 + 0x18) + 0x138);
  lVar3 = *(longlong *)(lVar2 + 0x170);
  lVar5 = (longlong)
          (int)((param_1 - *(longlong *)(lVar2 + 0x1a8)) / (longlong)*(int *)(lVar2 + 0x1b0));
  puVar6 = (undefined4 *)(lVar5 * 0x20 + *(longlong *)(lVar2 + 0x138));
  *param_2 = *(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0xa8);
  uVar1 = puVar6[1];
  uVar4 = puVar6[2];
  param_2[1] = *puVar6;
  param_2[2] = uVar1;
  param_2[3] = uVar4;
  uVar1 = puVar6[5];
  uVar4 = puVar6[6];
  param_2[4] = puVar6[4];
  param_2[5] = uVar1;
  param_2[6] = uVar4;
  param_2[7] = *(undefined4 *)(lVar3 + lVar5 * 0x18);
  param_2[8] = *(undefined4 *)(lVar3 + 4 + lVar5 * 0x18);
  param_2[9] = *(undefined4 *)(lVar3 + 8 + lVar5 * 0x18);
  uVar1 = puVar6[7];
  *(undefined1 *)(param_2 + 0xb) = 0;
  param_2[10] = uVar1;
  *(bool *)((longlong)param_2 + 0x2d) = *(int *)(param_1 + 0x48) == 4;
  *(bool *)((longlong)param_2 + 0x2e) = *(int *)(param_1 + 0x48) == 6;
  return;
}



````
