# native-shadowwalk-1407e0a90-decompile-v538.txt

Original: [work/movement-integration/native-shadowwalk-1407e0a90-decompile-v538.txt](<../../../../work/movement-integration/native-shadowwalk-1407e0a90-decompile-v538.txt>)

SHA-256: `EBE79A67F002A00D27B074134D7D0B8C09ADC877752F6C9BC4DFCC254D0AA988`; 2232 bytes.

## Original saved record

````text

void FUN_1407e0a90(longlong *param_1)

{
  longlong lVar1;
  undefined8 *puVar2;
  longlong *plVar3;
  int iVar4;
  ulonglong uVar5;
  uint uVar6;
  ulonglong uVar7;
  undefined8 local_178;
  undefined4 local_170;
  undefined8 local_168;
  undefined8 *local_158;
  uint local_150;
  uint local_14c;
  undefined8 local_148;
  undefined4 local_140 [78];
  
  local_168 = 0xfffffffffffffffe;
  uVar5 = 0;
  if ((((short)param_1[0x17] != 1) && (1 < (ushort)((short)param_1[0x17] - 8U))) &&
     (((uint)(int)*(short *)((longlong)param_1 + 0xba) >> 4 & 1) == 0)) {
    local_150 = 0x5000000;
    local_158 = &local_148;
    local_14c = 0x14ff;
    puVar2 = &local_148;
    iVar4 = 0x13;
    do {
      *puVar2 = 0;
      *(undefined4 *)(puVar2 + 1) = 0;
      puVar2 = puVar2 + 2;
      iVar4 = iVar4 + -1;
    } while (-1 < iVar4);
    uVar7 = uVar5;
    if ((*(uint *)(param_1 + 4) & 0xffffff) != 0) {
      do {
        local_178 = *(undefined8 *)(uVar7 + param_1[3]);
        local_170 = 0;
        FUN_14049e280(&local_158,&local_178);
        uVar6 = (int)uVar5 + 1;
        uVar5 = (ulonglong)uVar6;
        uVar7 = uVar7 + 0x18;
      } while ((int)uVar6 < (int)(*(uint *)(param_1 + 4) & 0xffffff));
    }
    if ((local_150 & 0xffffff) != 0) {
      (**(code **)(*param_1 + 0x88))(param_1,&local_158);
    }
    puVar2 = local_158;
    if (((local_14c & 0x40) == 0) && (local_158 != (undefined8 *)0x0)) {
      plVar3 = (longlong *)FUN_1401548e0();
      (**(code **)(*plVar3 + 0x10))
                (plVar3,
                 "c:\\users\\zmiarchiving\\desktop\\dh2\\dishonored2\\voidengine\\core\\idlib\\containers/List.h"
                 ,0x1ec,puVar2,0x10);
    }
  }
  if ((*(byte *)((longlong)param_1 + 0x24) & 0x40) == 0) {
    lVar1 = param_1[3];
    if (lVar1 != 0) {
      plVar3 = (longlong *)FUN_1401548e0();
      (**(code **)(*plVar3 + 0x10))
                (plVar3,
                 "c:\\users\\zmiarchiving\\desktop\\dh2\\dishonored2\\voidengine\\core\\idlib\\containers/List.h"
                 ,0x1ec,lVar1,0x10);
    }
    param_1[3] = 0;
    *(uint *)((longlong)param_1 + 0x24) = *(uint *)((longlong)param_1 + 0x24) & 0xff;
  }
  *(uint *)(param_1 + 4) = *(uint *)(param_1 + 4) & 0xff000000;
  return;
}



````
