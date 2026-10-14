# partner-context-1403b0000-decompile-v384.txt

Original: [work/movement-integration/partner-context-1403b0000-decompile-v384.txt](<../../../../work/movement-integration/partner-context-1403b0000-decompile-v384.txt>)

SHA-256: `D7B838434CEE8E2B817C2EB78B7B3DA770586B4BA2AE6DB914E4AC47EE6A4B89`; 983 bytes.

## Original saved record

````text

ulonglong FUN_1403b0000(longlong param_1,uint param_2)

{
  undefined4 uVar1;
  char cVar2;
  ulonglong uVar3;
  longlong lVar4;
  
  lVar4 = *(longlong *)(param_1 + 0x20);
  cVar2 = FUN_140803180(lVar4,0x2d);
  if (cVar2 == '\0') {
    uVar3 = FUN_140803180(lVar4,0x2e);
    if ((((char)uVar3 == '\0') || (uVar3 = *(ulonglong *)(lVar4 + 0x90), uVar3 == 0)) ||
       (*(longlong *)(uVar3 + 0x180) == 0)) goto code_r0x0001403b008b;
    uVar1 = *(undefined4 *)(*(longlong *)(uVar3 + 0x180) + 0x38);
  }
  else {
    uVar3 = *(ulonglong *)(lVar4 + 0x90);
    if ((uVar3 == 0) || (*(longlong *)(uVar3 + 0x178) == 0)) goto code_r0x0001403b008b;
    uVar1 = *(undefined4 *)(*(longlong *)(uVar3 + 0x178) + 0xf8);
  }
  lVar4 = FUN_1406dc560(uVar1);
  uVar3 = 0;
  if (lVar4 != 0) {
    uVar3 = (ulonglong)param_2;
    if (*(uint *)(lVar4 + 0xa8) == param_2) {
      return CONCAT71((uint7)(uint3)(param_2 >> 8),1);
    }
  }
code_r0x0001403b008b:
  return uVar3 & 0xffffffffffffff00;
}



````
