# partner-fatality-14086bec0-decompile-v395.txt

Original: [work/movement-integration/partner-fatality-14086bec0-decompile-v395.txt](<../../../../work/movement-integration/partner-fatality-14086bec0-decompile-v395.txt>)

SHA-256: `BAF5302D94AC89521381BDF267B8C976770B978B0A84F95A4D2137C9FA3806EF`; 650 bytes.

## Original saved record

````text

void FUN_14086bec0(longlong param_1)

{
  float fVar1;
  char cVar2;
  
  fVar1 = *(float *)(*(longlong *)(param_1 + 0x18) + 0xa0);
  if (fVar1 <= *(float *)(param_1 + 0x38)) {
    *(undefined1 *)(param_1 + 0xd2) = *(undefined1 *)(param_1 + 0xd1);
  }
  else {
    *(undefined1 *)(param_1 + 0xd2) = 0;
  }
  if (*(char *)(param_1 + 0xd1) != '\0') {
    *(undefined1 *)(param_1 + 0xd1) = 0;
    cVar2 = FUN_14086b050(fVar1,0);
    if (cVar2 == '\0') {
      FUN_14086d320(param_1);
      FUN_14086cbf0(param_1);
    }
  }
  if (*(char *)(param_1 + 0xd2) != '\0') {
    *(undefined4 *)(param_1 + 0xb0) = 2;
  }
  FUN_1408551c0(param_1);
  return;
}



````
