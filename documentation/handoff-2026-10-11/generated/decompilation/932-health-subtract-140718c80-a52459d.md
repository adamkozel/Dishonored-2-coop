# health-subtract-140718c80.txt

Original: [work/movement-integration/npc-combat-research-v869/health-subtract-140718c80.txt](<../../../../work/movement-integration/npc-combat-research-v869/health-subtract-140718c80.txt>)

SHA-256: `5FE491F5D4EC4261317A92F284890F0918D0551F1D560FC287F4C833BFFCB302`; 337 bytes.

## Original saved record

````text
void FUN_140718c80(longlong param_1,float param_2)

{
  longlong lVar1;
  
  FUN_1407177e0();
  lVar1 = (**(code **)**(undefined8 **)(param_1 + 0x10))();
  if ((DAT_1439ce350 <= *(int *)(lVar1 + 0x60)) && (*(int *)(lVar1 + 0x60) <= DAT_1439ce354)) {
    *(float *)(param_1 + 0xf4) = *(float *)(param_1 + 0xf4) - param_2;
  }
  return;
}

````
