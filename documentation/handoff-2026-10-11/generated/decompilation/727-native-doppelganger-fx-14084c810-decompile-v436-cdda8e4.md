# native-doppelganger-fx-14084c810-decompile-v436.txt

Original: [work/movement-integration/candidate-doppelganger-fx-probe-v434/native-doppelganger-fx-14084c810-decompile-v436.txt](<../../../../work/movement-integration/candidate-doppelganger-fx-probe-v434/native-doppelganger-fx-14084c810-decompile-v436.txt>)

SHA-256: `EF6C4D0A99306567F8000391E34C012154C1AB3181E40EA406F40D2A337E1055`; 2232 bytes.

## Original saved record

````text

undefined8 FUN_14084c810(longlong param_1,longlong param_2)

{
  undefined1 uVar1;
  char cVar2;
  longlong lVar3;
  longlong lVar4;
  undefined8 uVar5;
  undefined8 *puVar6;
  longlong lVar7;
  undefined4 local_38;
  undefined4 local_34;
  undefined4 local_30;
  
  lVar7 = *(longlong *)(param_1 + 0x10);
  FUN_1400998b0(lVar7);
  lVar7 = *(longlong *)(lVar7 + 0xd0);
  if ((*(byte *)(lVar7 + 0xd3) & 1) != 0) {
    lVar3 = FUN_140877720(lVar7,&DAT_1439e2340,2);
    if (lVar3 == 0) {
      return 0;
    }
    if ((*(byte *)(lVar3 + 0x151) & 0x40) == 0) {
      return 0;
    }
  }
  lVar3 = *(longlong *)(param_2 + 0x28);
  if (*(longlong *)(lVar3 + 8) == 0) {
    return 0;
  }
  if ((*(char *)(lVar3 + 0x34) == '\0') &&
     (cVar2 = FUN_140468020(lVar7,lVar3 + 0x10), cVar2 != '\0')) {
    return 0;
  }
  lVar4 = (**(code **)(*D2_GameSingletonPtr + 0x1c8))
                    (D2_GameSingletonPtr,&DAT_1439bdeb0,0,&DAT_1431af668,&DAT_14228a780,0);
  *(byte *)(lVar4 + 0xd4) = *(byte *)(lVar4 + 0xd4) | 0x10;
  uVar5 = FUN_14010e9e0(*(undefined4 *)(lVar3 + 0x18));
  FUN_140875e80(lVar7,lVar4,lVar3 + 0x10,&DAT_1431af658,uVar5,1);
  uVar5 = FUN_1404903b0(lVar4,0);
  puVar6 = (undefined8 *)FUN_14090faf0(uVar5);
  *puVar6 = *(undefined8 *)(lVar3 + 8);
  FUN_1404b1d60(uVar5,*(undefined4 *)(lVar7 + 0xa8));
  if (**(char **)(lVar3 + 0x20) != '\0') {
    lVar7 = FUN_1407cdd60(*(undefined8 *)(*(longlong *)(param_1 + 0x10) + 200),lVar3 + 0x20);
    if (lVar7 == 0) {
      *(undefined4 *)(puVar6 + 1) = 0x3fff0010;
    }
    else {
      *(undefined4 *)(puVar6 + 1) = *(undefined4 *)(lVar7 + 0xa8);
    }
    puVar6[2] = *(undefined8 *)(lVar3 + 0x28);
  }
  *(undefined1 *)(puVar6 + 3) = *(undefined1 *)(lVar3 + 0x30);
  *(undefined1 *)((longlong)puVar6 + 0x19) = *(undefined1 *)(lVar3 + 0x31);
  *(undefined1 *)((longlong)puVar6 + 0x1a) = *(undefined1 *)(lVar3 + 0x32);
  uVar1 = *(undefined1 *)(lVar3 + 0x33);
  *(undefined1 *)((longlong)puVar6 + 0x1c) = 1;
  *(undefined1 *)((longlong)puVar6 + 0x1b) = uVar1;
  FUN_1407e7b10(uVar5);
  local_30 = *(undefined4 *)(lVar3 + 0x38);
  local_38 = *(undefined4 *)(param_2 + 8);
  local_34 = *(undefined4 *)(lVar4 + 0xa8);
  FUN_14084c140(param_1 + 0xc0,&local_38);
  return 1;
}



````
