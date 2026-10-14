# possession-component-reference-decompile-v323.txt

Original: [work/movement-integration/possession-component-reference-decompile-v323.txt](<../../../../work/movement-integration/possession-component-reference-decompile-v323.txt>)

SHA-256: `C3A5C3029775508F30B9D67ED4670CE582EE620DB7FAF15858EB394AD9728A46`; 726 bytes.

## Original saved record

````text
{"0x1403da910":"\r\nlonglong FUN_1403da910(int *param_1)\r\n\r\n{\r\n  longlong lVar1;\r\n  char cVar2;\r\n  \r\n  if ((*param_1 != -1) && (lVar1 = *(longlong *)(param_1 + 2), *param_1 == *(int *)(lVar1 + 0x18)))\r\n  {\r\n    cVar2 = FUN_1407e7390(lVar1);\r\n    if (cVar2 != '\\0') {\r\n      return lVar1;\r\n    }\r\n  }\r\n  return 0;\r\n}\r\n\r\n","0x1403dab80":"\r\nvoid FUN_1403dab80(undefined4 *param_1,longlong param_2)\r\n\r\n{\r\n  undefined4 uVar1;\r\n  \r\n  if (param_2 == 0) {\r\n    *param_1 = 0xffffffff;\r\n    *(undefined8 *)(param_1 + 2) = 0;\r\n    return;\r\n  }\r\n  uVar1 = *(undefined4 *)(param_2 + 0x18);\r\n  *(longlong *)(param_1 + 2) = param_2;\r\n  *param_1 = uVar1;\r\n  return;\r\n}\r\n\r\n"}

````
