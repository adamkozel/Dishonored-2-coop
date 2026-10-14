# partner-fatality-1403b83a0-decompile-v395.txt

Original: [work/movement-integration/partner-fatality-1403b83a0-decompile-v395.txt](<../../../../work/movement-integration/partner-fatality-1403b83a0-decompile-v395.txt>)

SHA-256: `88B666DB2AFB22E556755FF59F4D00F305FD7C66AF2BC0AD25E6C8686AEA836E`; 455 bytes.

## Original saved record

````text

void FUN_1403b83a0(longlong param_1)

{
  longlong *plVar1;
  longlong *plVar2;
  longlong lVar3;
  
  plVar2 = (longlong *)(param_1 + 0x48);
  lVar3 = 5;
  do {
    plVar1 = (longlong *)*plVar2;
    if (plVar1 != (longlong *)0x0) {
      (**(code **)(*plVar1 + 8))(plVar1,1);
      *plVar2 = 0;
    }
    plVar2 = plVar2 + 1;
    lVar3 = lVar3 + -1;
  } while (lVar3 != 0);
  *(undefined8 *)(param_1 + 0x40) = 0;
  FUN_1403a3730(param_1);
  return;
}



````
