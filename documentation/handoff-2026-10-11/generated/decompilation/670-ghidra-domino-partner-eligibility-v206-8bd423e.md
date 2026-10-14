# ghidra-domino-partner-eligibility-v206.json

Original: [work/ghidra-domino-partner-eligibility-v206.json](<../../../../work/ghidra-domino-partner-eligibility-v206.json>)

SHA-256: `505C40A9C2B3B837A8615BFCB893A6CBA82C43972329A38B1FDD4D05E2C6631D`; 9173 bytes.

## Original saved record

````json
[
  {
    "program": "Dishonored2.exe",
    "addresses": [
      "140447170",
      "140447520",
      "14068ed50"
    ],
    "results": [
      {
        "status": "fulfilled",
        "value": {
          "content": [
            {
              "type": "text",
              "text": "\r\nvoid FUN_140447170(longlong param_1)\r\n\r\n{\r\n  undefined4 uVar1;\r\n  longlong lVar2;\r\n  undefined8 uVar3;\r\n  \r\n  lVar2 = FUN_1406dc560(*(undefined4 *)(param_1 + 200));\r\n  if (lVar2 == 0) {\r\n    FUN_14061bcb0(param_1);\r\n    return;\r\n  }\r\n  uVar3 = FUN_1406dc560(*(undefined4 *)(param_1 + 200));\r\n  lVar2 = FUN_1404bd680(*(longlong *)(param_1 + 0xb8),uVar3,*(longlong *)(param_1 + 0xb8) + 0x28);\r\n  *(uint *)(param_1 + 0xf8) = (uint)(lVar2 != 0);\r\n  FUN_14061bea0(param_1);\r\n  if (*(int *)(param_1 + 0xf8) == 0) {\r\n    uVar3 = FUN_1406dc560(*(undefined4 *)(param_1 + 200));\r\n    FUN_1404bc950(*(undefined8 *)(param_1 + 0xb8),uVar3);\r\n    FUN_1404e3050(param_1 + 0xcc);\r\n  }\r\n  else {\r\n    uVar3 = FUN_1406dc560(*(undefined4 *)(param_1 + 200));\r\n    FUN_1404be8b0(*(undefined8 *)(param_1 + 0xb8),uVar3);\r\n  }\r\n  uVar1 = *(undefined4 *)(*(longlong *)(param_1 + 8) + 0x1b0);\r\n  *(undefined4 *)(param_1 + 0xf8) = 1;\r\n  *(undefined4 *)(param_1 + 0xf4) = uVar1;\r\n  return;\r\n}\r\n\r\n"
            }
          ],
          "structuredContent": {
            "result": "\r\nvoid FUN_140447170(longlong param_1)\r\n\r\n{\r\n  undefined4 uVar1;\r\n  longlong lVar2;\r\n  undefined8 uVar3;\r\n  \r\n  lVar2 = FUN_1406dc560(*(undefined4 *)(param_1 + 200));\r\n  if (lVar2 == 0) {\r\n    FUN_14061bcb0(param_1);\r\n    return;\r\n  }\r\n  uVar3 = FUN_1406dc560(*(undefined4 *)(param_1 + 200));\r\n  lVar2 = FUN_1404bd680(*(longlong *)(param_1 + 0xb8),uVar3,*(longlong *)(param_1 + 0xb8) + 0x28);\r\n  *(uint *)(param_1 + 0xf8) = (uint)(lVar2 != 0);\r\n  FUN_14061bea0(param_1);\r\n  if (*(int *)(param_1 + 0xf8) == 0) {\r\n    uVar3 = FUN_1406dc560(*(undefined4 *)(param_1 + 200));\r\n    FUN_1404bc950(*(undefined8 *)(param_1 + 0xb8),uVar3);\r\n    FUN_1404e3050(param_1 + 0xcc);\r\n  }\r\n  else {\r\n    uVar3 = FUN_1406dc560(*(undefined4 *)(param_1 + 200));\r\n    FUN_1404be8b0(*(undefined8 *)(param_1 + 0xb8),uVar3);\r\n  }\r\n  uVar1 = *(undefined4 *)(*(longlong *)(param_1 + 8) + 0x1b0);\r\n  *(undefined4 *)(param_1 + 0xf8) = 1;\r\n  *(undefined4 *)(param_1 + 0xf4) = uVar1;\r\n  return;\r\n}\r\n\r\n"
          },
          "isError": false
        }
      },
      {
        "status": "fulfilled",
        "value": {
          "content": [
            {
              "type": "text",
              "text": "\r\nint FUN_140447520(longlong param_1)\r\n\r\n{\r\n  char cVar1;\r\n  longlong lVar2;\r\n  undefined8 uVar3;\r\n  \r\n  lVar2 = FUN_1406dc560(*(undefined4 *)(param_1 + 200));\r\n  if (lVar2 != 0) {\r\n    uVar3 = FUN_1406dc560(*(undefined4 *)(param_1 + 200));\r\n    lVar2 = FUN_1404bd680(*(longlong *)(param_1 + 0xb8),uVar3,*(longlong *)(param_1 + 0xb8) + 0x28);\r\n    cVar1 = FUN_140447460(param_1);\r\n    if (lVar2 == 0) {\r\n      return 2 - (uint)(cVar1 != '\\0');\r\n    }\r\n  }\r\n  return 0;\r\n}\r\n\r\n"
            }
          ],
          "structuredContent": {
            "result": "\r\nint FUN_140447520(longlong param_1)\r\n\r\n{\r\n  char cVar1;\r\n  longlong lVar2;\r\n  undefined8 uVar3;\r\n  \r\n  lVar2 = FUN_1406dc560(*(undefined4 *)(param_1 + 200));\r\n  if (lVar2 != 0) {\r\n    uVar3 = FUN_1406dc560(*(undefined4 *)(param_1 + 200));\r\n    lVar2 = FUN_1404bd680(*(longlong *)(param_1 + 0xb8),uVar3,*(longlong *)(param_1 + 0xb8) + 0x28);\r\n    cVar1 = FUN_140447460(param_1);\r\n    if (lVar2 == 0) {\r\n      return 2 - (uint)(cVar1 != '\\0');\r\n    }\r\n  }\r\n  return 0;\r\n}\r\n\r\n"
          },
          "isError": false
        }
      },
      {
        "status": "fulfilled",
        "value": {
          "content": [
            {
              "type": "text",
              "text": "\r\nvoid FUN_14068ed50(longlong param_1)\r\n\r\n{\r\n  undefined4 uVar1;\r\n  char cVar2;\r\n  int iVar3;\r\n  longlong *plVar4;\r\n  undefined8 uVar5;\r\n  longlong lVar6;\r\n  longlong lVar7;\r\n  uint7 uVar8;\r\n  bool bVar9;\r\n  \r\n  FUN_140856c50();\r\n  plVar4 = (longlong *)FUN_140856800(param_1);\r\n  if (*(char *)(param_1 + 0x70) == '\\0') {\r\n    lVar6 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x2f8);\r\n    uVar1 = *(undefined4 *)(param_1 + 0x24);\r\n    lVar7 = D2_GameSingletonPtr + 0x1f5338;\r\n    bVar9 = (*(ulonglong *)(lVar6 + 0x80) &\r\n            1L << ((byte)*(undefined4 *)(param_1 + 0x34) & 0x3f) & *(ulonglong *)(lVar6 + 0x90)) !=\r\n            0;\r\n    if (bVar9) {\r\n      cVar2 = FUN_140447410(plVar4);\r\n      if ((*(char *)(param_1 + 0x71) == '\\0') && (cVar2 != '\\0')) {\r\n        uVar5 = 1;\r\n        (**(code **)(*plVar4 + 0x120))(plVar4);\r\n      }\r\n      else {\r\n        uVar5 = 0;\r\n      }\r\n      uVar8 = (uint7)((ulonglong)lVar6 >> 8);\r\n      if ((*(char *)(param_1 + 0x71) == '\\0') && (cVar2 == '\\0')) {\r\n        FUN_14068ec30(param_1,uVar5,(ulonglong)uVar8 << 8);\r\n        *(bool *)(param_1 + 0x70) = bVar9;\r\n      }\r\n      else {\r\n        FUN_14068ec30(param_1,uVar5,CONCAT71(uVar8,1));\r\n        *(bool *)(param_1 + 0x70) = bVar9;\r\n      }\r\n    }\r\n    else if (*(char *)(param_1 + 0x72) != '\\0') {\r\n      iVar3 = FUN_140447520(plVar4);\r\n      if (iVar3 == 1) {\r\n        (**(code **)(*plVar4 + 0x120))(plVar4);\r\n        FUN_1407d9050(lVar7,uVar1,DAT_14398bb10,1);\r\n        FUN_1407d4cb0(lVar7,uVar1,DAT_14398baf0,0);\r\n        *(undefined2 *)(param_1 + 0x71) = 1;\r\n      }\r\n      else if (iVar3 == 2) {\r\n        if (DAT_143994e28 != 0) {\r\n          uVar5 = FUN_140854880(param_1);\r\n          lVar6 = FUN_140877720(uVar5,&DAT_1439d13b0,2);\r\n          if (lVar6 != 0) {\r\n            FUN_140745b20(lVar6);\r\n          }\r\n        }\r\n        FUN_140854190(param_1);\r\n      }\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
            }
          ],
          "structuredContent": {
            "result": "\r\nvoid FUN_14068ed50(longlong param_1)\r\n\r\n{\r\n  undefined4 uVar1;\r\n  char cVar2;\r\n  int iVar3;\r\n  longlong *plVar4;\r\n  undefined8 uVar5;\r\n  longlong lVar6;\r\n  longlong lVar7;\r\n  uint7 uVar8;\r\n  bool bVar9;\r\n  \r\n  FUN_140856c50();\r\n  plVar4 = (longlong *)FUN_140856800(param_1);\r\n  if (*(char *)(param_1 + 0x70) == '\\0') {\r\n    lVar6 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x2f8);\r\n    uVar1 = *(undefined4 *)(param_1 + 0x24);\r\n    lVar7 = D2_GameSingletonPtr + 0x1f5338;\r\n    bVar9 = (*(ulonglong *)(lVar6 + 0x80) &\r\n            1L << ((byte)*(undefined4 *)(param_1 + 0x34) & 0x3f) & *(ulonglong *)(lVar6 + 0x90)) !=\r\n            0;\r\n    if (bVar9) {\r\n      cVar2 = FUN_140447410(plVar4);\r\n      if ((*(char *)(param_1 + 0x71) == '\\0') && (cVar2 != '\\0')) {\r\n        uVar5 = 1;\r\n        (**(code **)(*plVar4 + 0x120))(plVar4);\r\n      }\r\n      else {\r\n        uVar5 = 0;\r\n      }\r\n      uVar8 = (uint7)((ulonglong)lVar6 >> 8);\r\n      if ((*(char *)(param_1 + 0x71) == '\\0') && (cVar2 == '\\0')) {\r\n        FUN_14068ec30(param_1,uVar5,(ulonglong)uVar8 << 8);\r\n        *(bool *)(param_1 + 0x70) = bVar9;\r\n      }\r\n      else {\r\n        FUN_14068ec30(param_1,uVar5,CONCAT71(uVar8,1));\r\n        *(bool *)(param_1 + 0x70) = bVar9;\r\n      }\r\n    }\r\n    else if (*(char *)(param_1 + 0x72) != '\\0') {\r\n      iVar3 = FUN_140447520(plVar4);\r\n      if (iVar3 == 1) {\r\n        (**(code **)(*plVar4 + 0x120))(plVar4);\r\n        FUN_1407d9050(lVar7,uVar1,DAT_14398bb10,1);\r\n        FUN_1407d4cb0(lVar7,uVar1,DAT_14398baf0,0);\r\n        *(undefined2 *)(param_1 + 0x71) = 1;\r\n      }\r\n      else if (iVar3 == 2) {\r\n        if (DAT_143994e28 != 0) {\r\n          uVar5 = FUN_140854880(param_1);\r\n          lVar6 = FUN_140877720(uVar5,&DAT_1439d13b0,2);\r\n          if (lVar6 != 0) {\r\n            FUN_140745b20(lVar6);\r\n          }\r\n        }\r\n        FUN_140854190(param_1);\r\n      }\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
          },
          "isError": false
        }
      }
    ]
  },
  {
    "program": "Dishonored2.exe",
    "addresses": [
      "140447410"
    ],
    "results": [
      {
        "status": "fulfilled",
        "value": {
          "content": [
            {
              "type": "text",
              "text": "\r\nundefined1 FUN_140447410(longlong param_1)\r\n\r\n{\r\n  longlong lVar1;\r\n  char cVar2;\r\n  \r\n  cVar2 = FUN_140447460();\r\n  lVar1 = FUN_1406dc560(*(undefined4 *)(param_1 + 200));\r\n  if ((lVar1 != 0) && (cVar2 != '\\0')) {\r\n    return 1;\r\n  }\r\n  return 0;\r\n}\r\n\r\n"
            }
          ],
          "structuredContent": {
            "result": "\r\nundefined1 FUN_140447410(longlong param_1)\r\n\r\n{\r\n  longlong lVar1;\r\n  char cVar2;\r\n  \r\n  cVar2 = FUN_140447460();\r\n  lVar1 = FUN_1406dc560(*(undefined4 *)(param_1 + 200));\r\n  if ((lVar1 != 0) && (cVar2 != '\\0')) {\r\n    return 1;\r\n  }\r\n  return 0;\r\n}\r\n\r\n"
          },
          "isError": false
        }
      }
    ]
  }
]

````

## Unescaped text fields for reading

### text

````text

void FUN_140447170(longlong param_1)

{
  undefined4 uVar1;
  longlong lVar2;
  undefined8 uVar3;
  
  lVar2 = FUN_1406dc560(*(undefined4 *)(param_1 + 200));
  if (lVar2 == 0) {
    FUN_14061bcb0(param_1);
    return;
  }
  uVar3 = FUN_1406dc560(*(undefined4 *)(param_1 + 200));
  lVar2 = FUN_1404bd680(*(longlong *)(param_1 + 0xb8),uVar3,*(longlong *)(param_1 + 0xb8) + 0x28);
  *(uint *)(param_1 + 0xf8) = (uint)(lVar2 != 0);
  FUN_14061bea0(param_1);
  if (*(int *)(param_1 + 0xf8) == 0) {
    uVar3 = FUN_1406dc560(*(undefined4 *)(param_1 + 200));
    FUN_1404bc950(*(undefined8 *)(param_1 + 0xb8),uVar3);
    FUN_1404e3050(param_1 + 0xcc);
  }
  else {
    uVar3 = FUN_1406dc560(*(undefined4 *)(param_1 + 200));
    FUN_1404be8b0(*(undefined8 *)(param_1 + 0xb8),uVar3);
  }
  uVar1 = *(undefined4 *)(*(longlong *)(param_1 + 8) + 0x1b0);
  *(undefined4 *)(param_1 + 0xf8) = 1;
  *(undefined4 *)(param_1 + 0xf4) = uVar1;
  return;
}


````

### text

````text

int FUN_140447520(longlong param_1)

{
  char cVar1;
  longlong lVar2;
  undefined8 uVar3;
  
  lVar2 = FUN_1406dc560(*(undefined4 *)(param_1 + 200));
  if (lVar2 != 0) {
    uVar3 = FUN_1406dc560(*(undefined4 *)(param_1 + 200));
    lVar2 = FUN_1404bd680(*(longlong *)(param_1 + 0xb8),uVar3,*(longlong *)(param_1 + 0xb8) + 0x28);
    cVar1 = FUN_140447460(param_1);
    if (lVar2 == 0) {
      return 2 - (uint)(cVar1 != '\0');
    }
  }
  return 0;
}


````

### text

````text

void FUN_14068ed50(longlong param_1)

{
  undefined4 uVar1;
  char cVar2;
  int iVar3;
  longlong *plVar4;
  undefined8 uVar5;
  longlong lVar6;
  longlong lVar7;
  uint7 uVar8;
  bool bVar9;
  
  FUN_140856c50();
  plVar4 = (longlong *)FUN_140856800(param_1);
  if (*(char *)(param_1 + 0x70) == '\0') {
    lVar6 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x2f8);
    uVar1 = *(undefined4 *)(param_1 + 0x24);
    lVar7 = D2_GameSingletonPtr + 0x1f5338;
    bVar9 = (*(ulonglong *)(lVar6 + 0x80) &
            1L << ((byte)*(undefined4 *)(param_1 + 0x34) & 0x3f) & *(ulonglong *)(lVar6 + 0x90)) !=
            0;
    if (bVar9) {
      cVar2 = FUN_140447410(plVar4);
      if ((*(char *)(param_1 + 0x71) == '\0') && (cVar2 != '\0')) {
        uVar5 = 1;
        (**(code **)(*plVar4 + 0x120))(plVar4);
      }
      else {
        uVar5 = 0;
      }
      uVar8 = (uint7)((ulonglong)lVar6 >> 8);
      if ((*(char *)(param_1 + 0x71) == '\0') && (cVar2 == '\0')) {
        FUN_14068ec30(param_1,uVar5,(ulonglong)uVar8 << 8);
        *(bool *)(param_1 + 0x70) = bVar9;
      }
      else {
        FUN_14068ec30(param_1,uVar5,CONCAT71(uVar8,1));
        *(bool *)(param_1 + 0x70) = bVar9;
      }
    }
    else if (*(char *)(param_1 + 0x72) != '\0') {
      iVar3 = FUN_140447520(plVar4);
      if (iVar3 == 1) {
        (**(code **)(*plVar4 + 0x120))(plVar4);
        FUN_1407d9050(lVar7,uVar1,DAT_14398bb10,1);
        FUN_1407d4cb0(lVar7,uVar1,DAT_14398baf0,0);
        *(undefined2 *)(param_1 + 0x71) = 1;
      }
      else if (iVar3 == 2) {
        if (DAT_143994e28 != 0) {
          uVar5 = FUN_140854880(param_1);
          lVar6 = FUN_140877720(uVar5,&DAT_1439d13b0,2);
          if (lVar6 != 0) {
            FUN_140745b20(lVar6);
          }
        }
        FUN_140854190(param_1);
      }
    }
  }
  return;
}


````

### text

````text

undefined1 FUN_140447410(longlong param_1)

{
  longlong lVar1;
  char cVar2;
  
  cVar2 = FUN_140447460();
  lVar1 = FUN_1406dc560(*(undefined4 *)(param_1 + 200));
  if ((lVar1 != 0) && (cVar2 != '\0')) {
    return 1;
  }
  return 0;
}


````
