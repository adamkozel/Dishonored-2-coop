# set-joint-local-14009cfa0.txt

Original: [work/movement-integration/npc-combat-research-v894/set-joint-local-14009cfa0.txt](<../../../../work/movement-integration/npc-combat-research-v894/set-joint-local-14009cfa0.txt>)

SHA-256: `53811FD19320FEDB5F284098E0F70966A188A4DB942B8829D9DF8799B90D0B83`; 2242 bytes.

## Original saved record

````text

/* [confirmed-runtime] RVA 0x9cfa0
   Used for validated native joint rotations; preserve current local translations.
   Evidence: src/arm_probe.h
   Names are analyst-assigned semantic labels, not recovered official symbols. */

void D2_Animation_SetJointLocal(longlong param_1,short param_2,undefined8 *param_3)

{
  uint *puVar1;
  byte bVar2;
  undefined8 uVar3;
  undefined8 *puVar4;
  uint *puVar5;
  int iVar6;
  longlong lVar7;
  longlong lVar8;
  
  uVar3 = param_3[1];
  puVar4 = (undefined8 *)((longlong)param_2 * 0x40 + *(longlong *)(param_1 + 0x148));
  *puVar4 = *param_3;
  puVar4[1] = uVar3;
  uVar3 = param_3[3];
  puVar4[2] = param_3[2];
  puVar4[3] = uVar3;
  uVar3 = param_3[5];
  puVar4[4] = param_3[4];
  puVar4[5] = uVar3;
  uVar3 = param_3[7];
  puVar4[6] = param_3[6];
  puVar4[7] = uVar3;
  bVar2 = (byte)param_2;
  puVar5 = (uint *)(param_1 + ((longlong)param_2 >> 5) * 4 + 0xd0);
  *puVar5 = *puVar5 & ~(1 << (bVar2 & 0x1f));
  puVar5 = (uint *)(param_1 + ((longlong)param_2 >> 5) * 4 + 0xf8);
  *puVar5 = *puVar5 | 1 << (bVar2 & 0x1f);
  puVar5 = (uint *)(param_1 + 0x120 + ((longlong)param_2 >> 5) * 4);
  *puVar5 = *puVar5 | 1 << (bVar2 & 0x1f);
  lVar8 = *(longlong *)(param_1 + 0x48) + (longlong)param_2 * 0x28;
  iVar6 = *(int *)(param_1 + 0x118);
  if (*(int *)(lVar8 + 0x20) < *(int *)(param_1 + 0x118)) {
    iVar6 = *(int *)(lVar8 + 0x20);
  }
  if ((-1 < iVar6) && (lVar7 = (longlong)iVar6, iVar6 != 0)) {
    puVar5 = (uint *)(param_1 + 0xf8 + lVar7 * 4);
    do {
      puVar1 = (uint *)((lVar8 - (param_1 + 0xf8)) + -4 + (longlong)puVar5);
      puVar5 = puVar5 + -1;
      *puVar5 = *puVar5 | *puVar1;
      lVar7 = lVar7 + -1;
    } while (lVar7 != 0);
  }
  lVar8 = *(longlong *)(param_1 + 0x48) + (longlong)param_2 * 0x28;
  iVar6 = *(int *)(param_1 + 0x140);
  if (*(int *)(lVar8 + 0x20) < *(int *)(param_1 + 0x140)) {
    iVar6 = *(int *)(lVar8 + 0x20);
  }
  if ((-1 < iVar6) && (lVar7 = (longlong)iVar6, iVar6 != 0)) {
    puVar5 = (uint *)(param_1 + 0x120 + lVar7 * 4);
    do {
      puVar1 = (uint *)((lVar8 - (param_1 + 0x120)) + -4 + (longlong)puVar5);
      puVar5 = puVar5 + -1;
      *puVar5 = *puVar5 | *puVar1;
      lVar7 = lVar7 + -1;
    } while (lVar7 != 0);
  }
  return;
}



````
