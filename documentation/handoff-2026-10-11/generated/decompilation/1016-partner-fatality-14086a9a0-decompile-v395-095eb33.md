# partner-fatality-14086a9a0-decompile-v395.txt

Original: [work/movement-integration/partner-fatality-14086a9a0-decompile-v395.txt](<../../../../work/movement-integration/partner-fatality-14086a9a0-decompile-v395.txt>)

SHA-256: `3FD104A1964C084A45B9C7697988AC1EDF98C6F8110139FA79E250BC777B4447`; 894 bytes.

## Original saved record

````text

void FUN_14086a9a0(longlong param_1)

{
  char cVar1;
  undefined8 uVar2;
  longlong lVar3;
  undefined4 local_res8 [2];
  
  cVar1 = FUN_140803180(*(undefined8 *)(param_1 + 0x10),7);
  if ((cVar1 == '\0') && (*(longlong *)(param_1 + 0xa0) != 0)) {
    *(undefined1 *)(param_1 + 0xd2) = 1;
    FUN_140895820();
    lVar3 = *(longlong *)(param_1 + 0xa0);
    uVar2 = FUN_14018cd20(lVar3 + 0x40);
    lVar3 = FUN_140894430(lVar3 + 0x40,uVar2,&DAT_1439e5a60);
    if ((lVar3 != 0) && (*(longlong *)(lVar3 + 0x38) != 0)) {
      local_res8[0] = 0;
      FUN_140854be0(param_1,local_res8,*(longlong *)(lVar3 + 0x38),5,0);
      goto LAB_14086aa3f;
    }
  }
  FUN_140854d80(param_1);
LAB_14086aa3f:
  *(undefined4 *)(param_1 + 0xcf) = 0;
  lVar3 = FUN_1403da910(param_1 + 0x78,2);
  if ((lVar3 != 0) && (*(char *)(lVar3 + 0x60) != '\0')) {
    FUN_14061aac0(lVar3,0);
    return;
  }
  return;
}



````
