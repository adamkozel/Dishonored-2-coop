# partner-collision-1401b7780-decompile-v380.txt

Original: [work/movement-integration/partner-collision-1401b7780-decompile-v380.txt](<../../../../work/movement-integration/partner-collision-1401b7780-decompile-v380.txt>)

SHA-256: `9FA1070F3BB6D1EE7E8FCD5282CBEDD4B98E11FCB39103A3B8E04AB6E7848D89`; 514 bytes.

## Original saved record

````text

undefined8 * FUN_1401b7780(undefined8 *param_1,ulonglong param_2)

{
  longlong lVar1;
  short sVar2;
  
  *param_1 = &PTR_LAB_141b02838;
  FUN_140a9e0d0();
  if ((param_2 & 1) != 0) {
    sVar2 = 0x20;
    if (*(short *)(param_1 + 2) != -1) {
      sVar2 = *(short *)(param_1 + 2);
    }
    lVar1 = TlsGetValue(DAT_143e8dd58);
    if (lVar1 == 0) {
      lVar1 = DAT_143e8dd50;
    }
    (**(code **)(**(longlong **)(lVar1 + 0x58) + 0x10))(*(longlong **)(lVar1 + 0x58),param_1,sVar2);
  }
  return param_1;
}



````
