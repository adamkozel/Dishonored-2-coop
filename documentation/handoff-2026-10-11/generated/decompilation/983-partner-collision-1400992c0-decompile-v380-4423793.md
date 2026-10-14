# partner-collision-1400992c0-decompile-v380.txt

Original: [work/movement-integration/partner-collision-1400992c0-decompile-v380.txt](<../../../../work/movement-integration/partner-collision-1400992c0-decompile-v380.txt>)

SHA-256: `25C82B45602E6C894019C0CE311356D3EAFF90CCEC5805EDF357A08ADE014F26`; 1078 bytes.

## Original saved record

````text

ulonglong FUN_1400992c0(undefined8 param_1,longlong param_2,uint *param_3,uint param_4)

{
  uint uVar1;
  uint uVar2;
  char cVar3;
  uint *puVar4;
  uint *puVar5;
  ulonglong uVar6;
  uint *puVar7;
  longlong lVar8;
  
  puVar5 = param_3;
  if (0 < (int)param_4) {
    uVar6 = (ulonglong)param_4;
    puVar4 = param_3;
    puVar7 = param_3;
    do {
      lVar8 = (ulonglong)(*puVar7 & 0xffffff) * 0xb0 + *(longlong *)(param_2 + 0x28);
      uVar1 = *(uint *)(lVar8 + 0x6c);
      uVar2 = *(uint *)((ulonglong)(puVar7[1] & 0xffffff) * 0xb0 + *(longlong *)(param_2 + 0x28) +
                       0x6c);
      puVar5 = puVar4;
      if (((((uVar2 >> 5) >> (uVar1 & 0x1f) & 1) != 0) &&
          (((uVar1 >> 5) >> (uVar2 & 0x1f) & 1) != 0)) &&
         (cVar3 = FUN_140093a40(param_1,lVar8), cVar3 == '\0')) {
        puVar5 = puVar4 + 2;
        *(undefined8 *)puVar4 = *(undefined8 *)puVar7;
      }
      puVar7 = puVar7 + 2;
      uVar6 = uVar6 - 1;
      puVar4 = puVar5;
    } while (uVar6 != 0);
  }
  return (longlong)puVar5 - (longlong)param_3 >> 3 & 0xffffffff;
}



````
