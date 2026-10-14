# damage-preliminary-140717800.txt

Original: [work/movement-integration/npc-combat-research-v869/damage-preliminary-140717800.txt](<../../../../work/movement-integration/npc-combat-research-v869/damage-preliminary-140717800.txt>)

SHA-256: `9873113CFB39289A43AC8E2D50263CC20DEF4EAC79186EB082A1F36B5414FB35`; 1683 bytes.

## Original saved record

````text
void FUN_140717800(longlong param_1,longlong param_2,float *param_3)

{
  undefined8 *puVar1;
  ulonglong uVar2;
  ulonglong uVar3;
  char cVar4;
  longlong lVar5;
  bool bVar6;
  float fVar7;
  float fVar8;
  
  puVar1 = *(undefined8 **)(param_1 + 0x10);
  uVar2 = *(ulonglong *)(param_2 + 0x10);
  uVar3 = *(ulonglong *)(*(longlong *)(param_1 + 8) + 0x78);
  if (uVar3 == 0) {
    bVar6 = (~*(ulonglong *)(param_1 + 0x48) & uVar2) == uVar2;
  }
  else {
    bVar6 = (uVar2 & uVar3) != 0;
  }
  if (bVar6) {
    cVar4 = FUN_140717dc0(param_1,param_2);
    if (cVar4 == '\0') {
      *param_3 = 0.0;
    }
    else {
      fVar8 = *(float *)(param_2 + 8);
      lVar5 = (**(code **)*puVar1)(puVar1);
      if ((DAT_1439ce350 <= *(int *)(lVar5 + 0x60)) && (*(int *)(lVar5 + 0x60) <= DAT_1439ce354)) {
        if (*(float *)(puVar1 + 0x62) * *(float *)(param_2 + 0x40) +
            *(float *)((longlong)puVar1 + 0x30c) * *(float *)(param_2 + 0x3c) +
            *(float *)((longlong)puVar1 + 0x314) * *(float *)(param_2 + 0x44) <= 0.0) {
          fVar7 = (float)FUN_140647ff0(D2_GameSingletonPtr + 0x1f7b90);
        }
        else {
          fVar7 = (float)FUN_140647fc0();
        }
        fVar8 = fVar8 * fVar7;
      }
      if ((*(byte *)(param_2 + 0x10) >> 2 & 1) == 0) {
        if (((uint)param_3[6] & 8) == 0) {
          fVar7 = *(float *)(param_1 + 0x20);
          if (fVar8 <= *(float *)(param_1 + 0x20)) {
            fVar7 = fVar8;
          }
          *param_3 = fVar7;
        }
        else {
          *param_3 = fVar8;
        }
      }
      else {
        *param_3 = *(float *)(param_1 + 0x20);
      }
    }
  }
  else {
    *param_3 = 0.0;
  }
  return;
}

````
