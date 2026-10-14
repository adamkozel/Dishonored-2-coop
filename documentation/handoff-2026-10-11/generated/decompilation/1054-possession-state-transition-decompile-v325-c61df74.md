# possession-state-transition-decompile-v325.txt

Original: [work/movement-integration/possession-state-transition-decompile-v325.txt](<../../../../work/movement-integration/possession-state-transition-decompile-v325.txt>)

SHA-256: `CE85660C8C88BD42BB020C9AE3D656E6DC1144F174233B8A8826B88CA53FCB0D`; 1322 bytes.

## Original saved record

````text
{"0x140824e40":"\r\nvoid FUN_140824e40(longlong param_1,int param_2)\r\n\r\n{\r\n  int iVar1;\r\n  undefined4 uVar2;\r\n  code *pcVar3;\r\n  undefined4 uVar4;\r\n  undefined4 uVar5;\r\n  longlong lVar6;\r\n  \r\n  iVar1 = *(int *)(param_1 + 0x1c0);\r\n  if (param_2 != iVar1) {\r\n    uVar4 = 0;\r\n    uVar5 = 0;\r\n    if (iVar1 != 0xe) {\r\n      lVar6 = (longlong)iVar1 * 0x20;\r\n      pcVar3 = *(code **)(lVar6 + 8 + param_1);\r\n      if (pcVar3 != (code *)0x0) {\r\n        (*pcVar3)(param_1,param_2);\r\n      }\r\n      uVar4 = *(undefined4 *)(lVar6 + 0x18 + param_1);\r\n      uVar5 = *(undefined4 *)(lVar6 + 0x1c + param_1);\r\n    }\r\n    if (param_2 == 0xe) {\r\n      FUN_140821e60(param_1,uVar4,0);\r\n      FUN_140822290(param_1,uVar5,0);\r\n      *(undefined4 *)(param_1 + 0x1c0) = 0xe;\r\n      return;\r\n    }\r\n    uVar2 = *(undefined4 *)(param_1 + 0x1c0);\r\n    *(int *)(param_1 + 0x1c0) = param_2;\r\n    lVar6 = (longlong)param_2 * 0x20;\r\n    FUN_140821e60(param_1,uVar4,*(undefined4 *)(lVar6 + 0x18 + param_1));\r\n    FUN_140822290(param_1,uVar5,*(undefined4 *)(lVar6 + 0x1c + param_1));\r\n    *(undefined4 *)(param_1 + 0x1c4) = 0;\r\n    if (*(code **)(lVar6 + param_1) != (code *)0x0) {\r\n      (**(code **)(lVar6 + param_1))(param_1,uVar2);\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"}

````
