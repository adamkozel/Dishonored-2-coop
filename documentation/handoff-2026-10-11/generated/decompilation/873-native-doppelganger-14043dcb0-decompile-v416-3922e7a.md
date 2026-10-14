# native-doppelganger-14043dcb0-decompile-v416.txt

Original: [work/movement-integration/native-doppelganger-14043dcb0-decompile-v416.txt](<../../../../work/movement-integration/native-doppelganger-14043dcb0-decompile-v416.txt>)

SHA-256: `781CD0D983A1253F4A5427C7AF6C118B06F2009A1527CDCC0D78721893AC56C5`; 536 bytes.

## Original saved record

````text

void FUN_14043dcb0(undefined8 param_1,undefined1 param_2,undefined8 param_3,undefined8 param_4)

{
  longlong *plVar1;
  undefined8 uVar2;
  
  uVar2 = 0xfffffffffffffffe;
  if (DAT_143a139c8 == 0) {
    plVar1 = (longlong *)FUN_1401548e0();
    (**(code **)(*plVar1 + 0x48))(plVar1,0,*plVar1,param_4,uVar2);
    DAT_143a139c8 = (**(code **)(DAT_143a138d0 + 0x30))(&DAT_143a138d0);
    plVar1 = (longlong *)FUN_1401548e0();
    (**(code **)(*plVar1 + 0x50))(plVar1,0);
  }
  FUN_140875d50(param_1,DAT_143a139c8,param_2);
  return;
}



````
