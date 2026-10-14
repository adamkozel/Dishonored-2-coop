# native-shadowwalk-140622c10-decompile-v514.txt

Original: [work/movement-integration/candidate-shadowwalk-inventory-v515/native-shadowwalk-140622c10-decompile-v514.txt](<../../../../work/movement-integration/candidate-shadowwalk-inventory-v515/native-shadowwalk-140622c10-decompile-v514.txt>)

SHA-256: `983DA21DC5354FF4C0D7C41354B2DB5DF909919209E47013510618760A01BD22`; 1872 bytes.

## Original saved record

````text
void FUN_140622c10(longlong param_1)

{
  longlong lVar1;
  char cVar2;
  int iVar3;
  float fVar4;
  byte local_res8 [8];
  undefined4 local_res10;
  
  lVar1 = *(longlong *)(param_1 + 200);
  *(undefined1 *)(param_1 + 0xfc) = 0;
  *(undefined4 *)(param_1 + 0xe4) = 1;
  FUN_1407fb080(lVar1);
  *(ulonglong *)(lVar1 + 0x98) = *(ulonglong *)(lVar1 + 0x98) & 0xffffffffffefffff;
  FUN_1407f86f0(lVar1,0xfffffffffffff901);
  *(undefined1 *)(lVar1 + 0x171) = 0;
  FUN_1407f8740(lVar1);
  FUN_140853d10(local_res8);
  lVar1 = *(longlong *)(param_1 + 0xc0);
  local_res8[0] = local_res8[0] | 0xc;
  FUN_140802a60(lVar1,local_res8);
  FUN_140802bc0(lVar1,0xe);
  FUN_140802bc0(lVar1,0x10);
  FUN_1408029e0(*(undefined8 *)(param_1 + 0xc0),1);
  FUN_1408029e0(*(undefined8 *)(param_1 + 0xc0),2);
  FUN_1408029e0(*(undefined8 *)(param_1 + 0xc0),3);
  cVar2 = FUN_140803180(lVar1,0x4c);
  if (cVar2 != '\0') {
    FUN_140802a20(lVar1,0x4c);
  }
  cVar2 = FUN_140803180(lVar1,0x24);
  if (cVar2 != '\0') {
    FUN_140802a20(lVar1,0x24);
  }
  cVar2 = FUN_140803180(lVar1,0x12);
  if (cVar2 != '\0') {
    FUN_140802a20(lVar1,0x12);
  }
  cVar2 = FUN_140803180(lVar1,0x54);
  if (cVar2 != '\0') {
    FUN_140802a20(lVar1,0x54);
  }
  lVar1 = *(longlong *)(lVar1 + 0x330);
  if (*(int *)(param_1 + 0x11c) < 0) {
    local_res10 = 0;
  }
  else {
    local_res10 = *(undefined4 *)
                   (*(longlong *)(lVar1 + 0x48) + 0x24 + (longlong)*(int *)(param_1 + 0x11c) * 0x28)
    ;
  }
  *(undefined4 *)(param_1 + 0xec) = local_res10;
  if (*(int *)(param_1 + 0x118) < 0) {
    iVar3 = 0;
  }
  else {
    fVar4 = (float)FUN_1411ad95c(*(float *)(*(longlong *)(lVar1 + 0x48) + 0x24 +
                                           (longlong)*(int *)(param_1 + 0x118) * 0x28) + 0.5);
    iVar3 = (int)fVar4;
  }
  *(int *)(param_1 + 0xf0) = iVar3;
  FUN_1407e79a0(param_1);
  return;
}

````
