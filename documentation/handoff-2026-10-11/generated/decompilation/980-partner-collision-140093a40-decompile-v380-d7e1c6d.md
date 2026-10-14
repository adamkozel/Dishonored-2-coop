# partner-collision-140093a40-decompile-v380.txt

Original: [work/movement-integration/partner-collision-140093a40-decompile-v380.txt](<../../../../work/movement-integration/partner-collision-140093a40-decompile-v380.txt>)

SHA-256: `81E2066947EAA22414DD732CDBFDBBCD17618F8EB66AAD718A82BCAD96E1EC61`; 849 bytes.

## Original saved record

````text

undefined8 FUN_140093a40(undefined8 param_1,longlong param_2,longlong param_3)

{
  char cVar1;
  int iVar2;
  uint uVar3;
  longlong *plVar4;
  longlong *plVar5;
  longlong lVar6;
  
  cVar1 = func_0x000140093b00(param_1,*(undefined4 *)(param_2 + 0x70),
                              *(undefined4 *)(param_3 + 0x70));
  if (cVar1 != '\0') {
    return 1;
  }
  plVar4 = (longlong *)FUN_140017d10(param_2);
  plVar5 = (longlong *)FUN_140017d10(param_3);
  if (plVar4[1] == plVar5[1]) {
    lVar6 = (**(code **)(*plVar4 + 0x20))(plVar4);
    if (lVar6 != 0) {
      iVar2 = (**(code **)(*plVar4 + 0x28))(plVar4);
      uVar3 = (**(code **)(*plVar5 + 0x28))(plVar5);
      if ((*(uint *)(lVar6 + 0x30 + (((longlong)(int)uVar3 >> 5) + (longlong)iVar2) * 4) >>
           (uVar3 & 0x1f) & 1) == 0) {
        return 1;
      }
    }
  }
  return 0;
}



````
