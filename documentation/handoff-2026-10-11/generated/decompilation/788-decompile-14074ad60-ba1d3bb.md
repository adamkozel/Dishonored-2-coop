# decompile-14074ad60.txt

Original: [work/movement-integration/door-research-v639/decompile-14074ad60.txt](<../../../../work/movement-integration/door-research-v639/decompile-14074ad60.txt>)

SHA-256: `1CCDC2701D9006EBB3AAE998FD68FB339FACD65352B6884A72DFC107598E05C7`; 405 bytes.

## Original saved record

````text

void FUN_14074ad60(undefined8 param_1)

{
  undefined8 uVar1;
  int iVar2;
  longlong lVar3;
  
  lVar3 = FUN_1407e7380();
  iVar2 = FUN_1401581c0(lVar3 + 0xc0c,0,1);
  while (iVar2 == 1) {
    iVar2 = FUN_1401581c0(lVar3 + 0xc0c,0,1);
  }
  FUN_14048ff20(*(undefined8 *)(lVar3 + 0x5a8),param_1);
  FUN_1401581e0(lVar3 + 0xc0c,0);
  uVar1 = FUN_1407e7380();
  FUN_1407ec5a0(uVar1,param_1);
  return;
}



````
