# partner-fatality-1403b1980-decompile-v395.txt

Original: [work/movement-integration/partner-fatality-1403b1980-decompile-v395.txt](<../../../../work/movement-integration/partner-fatality-1403b1980-decompile-v395.txt>)

SHA-256: `8FE31097187DE33F276989FED65A8689D75B6D97ED5103D237A503FF9B15B736`; 324 bytes.

## Original saved record

````text

bool FUN_1403b1980(undefined8 param_1,undefined8 param_2)

{
  float fVar1;
  longlong lVar2;
  float fVar3;
  
  lVar2 = FUN_140877720(param_2,&DAT_1439ae8e0,2);
  if (lVar2 != 0) {
    fVar1 = *(float *)(lVar2 + 0x20);
    fVar3 = (float)FUN_1403af6f0(param_1,param_2);
    return fVar1 <= fVar3;
  }
  return false;
}



````
