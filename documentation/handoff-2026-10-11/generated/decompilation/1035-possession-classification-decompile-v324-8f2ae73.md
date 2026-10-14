# possession-classification-decompile-v324.txt

Original: [work/movement-integration/possession-classification-decompile-v324.txt](<../../../../work/movement-integration/possession-classification-decompile-v324.txt>)

SHA-256: `30661933B654796535106DCE6D570D42ECBF47C3C0713FAE2E569985940115A2`; 1322 bytes.

## Original saved record

````text
{"0x140448050":"Error: Function not found","0x1404480b0":"Error: Function not found","0x140823300":"\r\nundefined8 FUN_140823300(undefined8 param_1,undefined8 *param_2)\r\n\r\n{\r\n  undefined8 *puVar1;\r\n  longlong lVar2;\r\n  undefined8 uVar3;\r\n  \r\n  lVar2 = (**(code **)*param_2)(param_2);\r\n  if ((DAT_1439aef70 <= *(int *)(lVar2 + 0x60)) && (*(int *)(lVar2 + 0x60) <= DAT_1439aef74)) {\r\n    lVar2 = FUN_1408c3e30(param_2[2]);\r\n    if (lVar2 != 0) {\r\n      puVar1 = *(undefined8 **)(*(longlong *)(lVar2 + 0x360) + 0x18);\r\n      lVar2 = (**(code **)*puVar1)(puVar1);\r\n      if ((DAT_1439b9f80 <= *(int *)(lVar2 + 0x60)) && (*(int *)(lVar2 + 0x60) <= DAT_1439b9f84)) {\r\n        return 8;\r\n      }\r\n      lVar2 = (**(code **)*puVar1)(puVar1);\r\n      if ((DAT_1439ede30 <= *(int *)(lVar2 + 0x60)) && (*(int *)(lVar2 + 0x60) <= DAT_1439ede34)) {\r\n        return 0x10;\r\n      }\r\n      lVar2 = (**(code **)*puVar1)(puVar1);\r\n      if ((DAT_1439e8a00 <= *(int *)(lVar2 + 0x60)) && (*(int *)(lVar2 + 0x60) <= DAT_1439e8a04)) {\r\n        return 4;\r\n      }\r\n    }\r\n  }\r\n  lVar2 = (**(code **)*param_2)(param_2);\r\n  if ((*(int *)(lVar2 + 0x60) < DAT_1439edd10) ||\r\n     (uVar3 = 2, DAT_1439edd14 < *(int *)(lVar2 + 0x60))) {\r\n    uVar3 = 1;\r\n  }\r\n  return uVar3;\r\n}\r\n\r\n"}

````
