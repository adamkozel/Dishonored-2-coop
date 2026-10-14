# decompile-1403da910.txt

Original: [work/movement-integration/door-research-v639/decompile-1403da910.txt](<../../../../work/movement-integration/door-research-v639/decompile-1403da910.txt>)

SHA-256: `6E7171DE45D55596D23DBED1E17DCEDC211EB9F539B969868D7E795A0EABB800`; 285 bytes.

## Original saved record

````text

longlong FUN_1403da910(int *param_1)

{
  longlong lVar1;
  char cVar2;
  
  if ((*param_1 != -1) && (lVar1 = *(longlong *)(param_1 + 2), *param_1 == *(int *)(lVar1 + 0x18)))
  {
    cVar2 = FUN_1407e7390(lVar1);
    if (cVar2 != '\0') {
      return lVar1;
    }
  }
  return 0;
}



````
