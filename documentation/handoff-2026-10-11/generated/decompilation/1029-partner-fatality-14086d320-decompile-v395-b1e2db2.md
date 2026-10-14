# partner-fatality-14086d320-decompile-v395.txt

Original: [work/movement-integration/partner-fatality-14086d320-decompile-v395.txt](<../../../../work/movement-integration/partner-fatality-14086d320-decompile-v395.txt>)

SHA-256: `BFE914BA6824D70AC5636A715479D6054FD0F77740B4D6AF90AB874782AD07B1`; 441 bytes.

## Original saved record

````text

bool FUN_14086d320(longlong param_1)

{
  undefined8 uVar1;
  longlong lVar2;
  
  uVar1 = FUN_140777b90(*(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x338),0x21);
  lVar2 = FUN_1403b8020(uVar1,0);
  if (lVar2 == 0) {
    *(undefined4 *)(param_1 + 0xe0) = 0x3fff0010;
  }
  else {
    *(undefined4 *)(param_1 + 0xe0) = *(undefined4 *)(lVar2 + 0xa8);
  }
  lVar2 = FUN_1406dc560(*(undefined4 *)(param_1 + 0xe0));
  return lVar2 != 0;
}



````
