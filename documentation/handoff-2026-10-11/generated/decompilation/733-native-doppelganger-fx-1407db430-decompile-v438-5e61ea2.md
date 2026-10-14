# native-doppelganger-fx-1407db430-decompile-v438.txt

Original: [work/movement-integration/candidate-midnight-fx-probe-v445/native-doppelganger-fx-1407db430-decompile-v438.txt](<../../../../work/movement-integration/candidate-midnight-fx-probe-v445/native-doppelganger-fx-1407db430-decompile-v438.txt>)

SHA-256: `FDED9E8ACC0FA3E5BF4CE00BE6D79241F8D4B349E7A4726D8BB9BCC2E410C0D4`; 919 bytes.

## Original saved record

````text

void FUN_1407db430(undefined8 *param_1)

{
  undefined8 *puVar1;
  longlong lVar2;
  longlong *plVar3;
  undefined8 *puVar4;
  
  *param_1 = &PTR_LAB_141c6ca98;
  puVar4 = (undefined8 *)param_1[4];
  puVar1 = puVar4 + (*(uint *)(param_1 + 5) & 0xffffff);
  for (; puVar4 != puVar1; puVar4 = puVar4 + 1) {
    FUN_140652ea0(*puVar4);
  }
  if ((*(byte *)((longlong)param_1 + 0x2c) & 0x40) == 0) {
    lVar2 = param_1[4];
    if (lVar2 != 0) {
      plVar3 = (longlong *)FUN_1401548e0();
      (**(code **)(*plVar3 + 0x10))
                (plVar3,
                 "c:\\users\\zmiarchiving\\desktop\\dh2\\dishonored2\\voidengine\\core\\idlib\\containers/List.h"
                 ,0x1ec,lVar2,0x10);
    }
    param_1[4] = 0;
    *(uint *)((longlong)param_1 + 0x2c) = *(uint *)((longlong)param_1 + 0x2c) & 0xff;
  }
  *(uint *)(param_1 + 5) = *(uint *)(param_1 + 5) & 0xff000000;
  FUN_140883760(param_1);
  return;
}



````
