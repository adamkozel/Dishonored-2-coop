# native-shadowwalk-140661960-decompile-v538.txt

Original: [work/movement-integration/native-shadowwalk-140661960-decompile-v538.txt](<../../../../work/movement-integration/native-shadowwalk-140661960-decompile-v538.txt>)

SHA-256: `E6D7CAD478256BD90C050FDB486069E99BCA5E52177D80E7AD3DAAF55444E6EB`; 2281 bytes.

## Original saved record

````text

void FUN_140661960(undefined8 param_1,longlong *param_2)

{
  longlong *plVar1;
  undefined8 *puVar2;
  undefined8 *puVar3;
  longlong lStack_448;
  byte bStack_43c;
  longlong lStack_3b8;
  uint uStack_3b0;
  uint uStack_3ac;
  longlong lStack_328;
  uint uStack_320;
  uint uStack_31c;
  longlong lStack_198;
  uint uStack_190;
  uint uStack_18c;
  
  FUN_140661100(&lStack_448);
  puVar3 = (undefined8 *)*param_2;
  puVar2 = puVar3 + (ulonglong)(*(uint *)(param_2 + 1) & 0xffffff) * 2;
  for (; puVar3 != puVar2; puVar3 = puVar3 + 2) {
    *(undefined4 *)(puVar3 + 1) = 1;
    FUN_1406612f0(param_1,*puVar3,0,0,&lStack_448);
  }
  FUN_140662540(param_1,&lStack_448);
  if ((uStack_18c & 0x40) == 0) {
    if (lStack_198 != 0) {
      plVar1 = (longlong *)FUN_1401548e0();
      (**(code **)(*plVar1 + 0x10))
                (plVar1,
                 "c:\\users\\zmiarchiving\\desktop\\dh2\\dishonored2\\voidengine\\core\\idlib\\containers/List.h"
                 ,0x1ec,lStack_198,0x10);
    }
    lStack_198 = 0;
    uStack_18c = uStack_18c & 0xff;
  }
  uStack_190 = uStack_190 & 0xff000000;
  if ((uStack_31c & 0x40) == 0) {
    if (lStack_328 != 0) {
      plVar1 = (longlong *)FUN_1401548e0();
      (**(code **)(*plVar1 + 0x10))
                (plVar1,
                 "c:\\users\\zmiarchiving\\desktop\\dh2\\dishonored2\\voidengine\\core\\idlib\\containers/List.h"
                 ,0x1ec,lStack_328,0x10);
    }
    lStack_328 = 0;
    uStack_31c = uStack_31c & 0xff;
  }
  uStack_320 = uStack_320 & 0xff000000;
  if ((uStack_3ac & 0x40) == 0) {
    if (lStack_3b8 != 0) {
      plVar1 = (longlong *)FUN_1401548e0();
      (**(code **)(*plVar1 + 0x10))
                (plVar1,
                 "c:\\users\\zmiarchiving\\desktop\\dh2\\dishonored2\\voidengine\\core\\idlib\\containers/List.h"
                 ,0x1ec,lStack_3b8,0x10);
    }
    lStack_3b8 = 0;
    uStack_3ac = uStack_3ac & 0xff;
  }
  uStack_3b0 = uStack_3b0 & 0xff000000;
  if (((bStack_43c & 0x40) == 0) && (lStack_448 != 0)) {
    plVar1 = (longlong *)FUN_1401548e0();
    (**(code **)(*plVar1 + 0x10))
              (plVar1,
               "c:\\users\\zmiarchiving\\desktop\\dh2\\dishonored2\\voidengine\\core\\idlib\\containers/List.h"
               ,0x1ec,lStack_448,0x10);
  }
  return;
}



````
