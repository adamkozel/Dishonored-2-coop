# partner-fatality-1403b82c0-decompile-v395.txt

Original: [work/movement-integration/partner-fatality-1403b82c0-decompile-v395.txt](<../../../../work/movement-integration/partner-fatality-1403b82c0-decompile-v395.txt>)

SHA-256: `0E1661A1D879DA6FD4D006156304136AD95D89EAAA485EDFD2C1B28392F6BD14`; 1099 bytes.

## Original saved record

````text

void FUN_1403b82c0(longlong param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  undefined4 uVar1;
  longlong lVar2;
  longlong lVar3;
  longlong lVar4;
  longlong *plVar5;
  undefined8 uVar6;
  undefined8 uStack_48;
  undefined8 uStack_40;
  
  uVar6 = 0xfffffffffffffffe;
  FUN_1403a36a0();
  uVar6 = FUN_140877720(*(undefined8 *)(*(longlong *)(param_1 + 0x30) + 0x10),&DAT_1439d7480,0,
                        param_4,uVar6);
  *(undefined8 *)(param_1 + 0x40) = uVar6;
  lVar4 = 0x74;
  plVar5 = (longlong *)(param_1 + 0x48);
  do {
    uStack_48 = 0;
    uStack_40 = 0;
    uVar1 = *(undefined4 *)(lVar4 + *(longlong *)(param_1 + 8));
    lVar2 = TlsGetValue(DAT_143e8dd58);
    if (lVar2 == 0) {
      lVar2 = DAT_143e8dd50;
    }
    lVar3 = (**(code **)(**(longlong **)(lVar2 + 0x58) + 8))(*(longlong **)(lVar2 + 0x58),0x60);
    lVar2 = 0;
    if (lVar3 != 0) {
      lVar2 = FUN_140ca6550(lVar3,&uStack_48,uVar1);
    }
    *(undefined2 *)(lVar2 + 0x10) = 0x60;
    *plVar5 = lVar2;
    lVar4 = lVar4 + 4;
    plVar5 = plVar5 + 1;
  } while (lVar4 < 0x88);
  return;
}



````
