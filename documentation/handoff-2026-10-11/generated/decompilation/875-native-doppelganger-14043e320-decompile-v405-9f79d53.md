# native-doppelganger-14043e320-decompile-v405.txt

Original: [work/movement-integration/native-doppelganger-14043e320-decompile-v405.txt](<../../../../work/movement-integration/native-doppelganger-14043e320-decompile-v405.txt>)

SHA-256: `70D9459E99426642BE232F32DCA1A7FC3A8DAB941A421FE292847B43CE6A05EB`; 670 bytes.

## Original saved record

````text

void FUN_14043e320(undefined8 param_1,undefined8 param_2)

{
  longlong lVar1;
  undefined8 uVar2;
  longlong lVar3;
  undefined4 auStackX_18 [2];
  undefined4 auStackX_20 [2];
  
  lVar1 = FUN_140877720(param_2,&DAT_1439e27c0,0);
  if (lVar1 != 0) {
    FUN_14087cdb0(param_2);
    uVar2 = FUN_14043dcb0(param_2,1);
    lVar3 = FUN_140877720(param_2,&DAT_1439e82e0,0);
    if (lVar3 != 0) {
      FUN_1407e7b10(lVar3);
    }
    lVar1 = *(longlong *)(lVar1 + 8);
    auStackX_18[0] = *(undefined4 *)(lVar1 + 0x19c);
    auStackX_20[0] = *(undefined4 *)(lVar1 + 0x198);
    FUN_14039db60(uVar2,*(undefined4 *)(lVar1 + 0x160),auStackX_20,auStackX_18);
  }
  return;
}



````
