# native-doppelganger-fx-14084c7a0-decompile-v436.txt

Original: [work/movement-integration/native-doppelganger-fx-14084c7a0-decompile-v436.txt](<../../../../work/movement-integration/native-doppelganger-fx-14084c7a0-decompile-v436.txt>)

SHA-256: `1FF1B1918E9024F523EA5F634FA56D384D2B439DD60609F99A0402DBA4745CC2`; 465 bytes.

## Original saved record

````text

void FUN_14084c7a0(undefined8 param_1,longlong *param_2)

{
  char cVar1;
  undefined4 uVar2;
  undefined8 *puVar3;
  undefined8 *puVar4;
  
  puVar3 = (undefined8 *)*param_2;
  puVar4 = puVar3 + (ulonglong)(*(uint *)(param_2 + 1) & 0xffffff) * 2;
  for (; puVar3 != puVar4; puVar3 = puVar3 + 2) {
    cVar1 = FUN_14084c810(param_1,*puVar3);
    uVar2 = 4;
    if (cVar1 != '\0') {
      uVar2 = 1;
    }
    *(undefined4 *)(puVar3 + 1) = uVar2;
  }
  return;
}



````
