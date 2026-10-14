# native-doppelganger-14043e3d0-decompile-v407.txt

Original: [work/movement-integration/native-doppelganger-14043e3d0-decompile-v407.txt](<../../../../work/movement-integration/native-doppelganger-14043e3d0-decompile-v407.txt>)

SHA-256: `14AEAB7DE68EBE4E01DF40A0568CAB3EADC459C2FFBD17E94BCEAC865B61B6D4`; 1187 bytes.

## Original saved record

````text

void FUN_14043e3d0(longlong param_1)

{
  uint uVar1;
  longlong lVar2;
  longlong lVar3;
  int iVar4;
  undefined1 auStack_28 [32];
  
  if (((*(uint *)(param_1 + 0xc0) & 0xffffff) != 0) &&
     (lVar2 = FUN_1406dc560(**(undefined4 **)(param_1 + 0xb8)), lVar2 != 0)) {
    lVar3 = FUN_140877720(lVar2,&DAT_1439ba040,2);
    if (lVar3 == 0) {
      lVar3 = FUN_140877720(lVar2,&DAT_1439bf9f0,2);
      if (lVar3 == 0) {
        FUN_1408847f0(lVar2,auStack_28,&DAT_1439a6f30);
        FUN_140882fc0(auStack_28);
      }
      else {
        FUN_14039e140(lVar3);
      }
    }
    else {
      thunk_FUN_1404a4eb0(lVar3);
    }
    uVar1 = *(uint *)(param_1 + 0xc0);
    if ((uVar1 & 0xffffff) != 0) {
      iVar4 = 0;
      uVar1 = (uVar1 - 1 ^ uVar1) & 0xffffff ^ uVar1;
      *(uint *)(param_1 + 0xc0) = uVar1;
      if ((uVar1 & 0xffffff) != 0) {
        lVar2 = 0;
        do {
          iVar4 = iVar4 + 1;
          lVar2 = lVar2 + 4;
          *(undefined4 *)(*(longlong *)(param_1 + 0xb8) + -4 + lVar2) =
               *(undefined4 *)(*(longlong *)(param_1 + 0xb8) + lVar2);
        } while (iVar4 < (int)(*(uint *)(param_1 + 0xc0) & 0xffffff));
      }
    }
  }
  return;
}



````
