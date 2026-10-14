# ghidra-domino-cancel-state-v229.json

Original: [work/ghidra-domino-cancel-state-v229.json](<../../../../work/ghidra-domino-cancel-state-v229.json>)

SHA-256: `1217685787A5A196CEFDDD4EF2471C31436B967BFB729C8D113127BF4A6EAF5A`; 3967 bytes.

## Original saved record

````json
[
  {
    "status": "fulfilled",
    "value": {
      "address": "14068ec30",
      "result": {
        "content": [
          {
            "type": "text",
            "text": "\r\nvoid FUN_14068ec30(longlong param_1,undefined1 param_2,undefined1 param_3)\r\n\r\n{\r\n  undefined4 uVar1;\r\n  longlong lVar2;\r\n  \r\n  uVar1 = *(undefined4 *)(param_1 + 0x24);\r\n  lVar2 = D2_GameSingletonPtr + 0x1f5338;\r\n  FUN_1407d9050(lVar2,uVar1,DAT_14398bb00,param_2);\r\n  FUN_1407d9050(lVar2,uVar1,DAT_14398bb08,param_3);\r\n  FUN_1407d9050(lVar2,uVar1,DAT_14398baf8,0);\r\n  FUN_1407d9050(lVar2,uVar1,DAT_14398bb10,0);\r\n  FUN_1407d4cb0(lVar2,uVar1,DAT_14398baf0,0);\r\n  return;\r\n}\r\n\r\n"
          }
        ],
        "structuredContent": {
          "result": "\r\nvoid FUN_14068ec30(longlong param_1,undefined1 param_2,undefined1 param_3)\r\n\r\n{\r\n  undefined4 uVar1;\r\n  longlong lVar2;\r\n  \r\n  uVar1 = *(undefined4 *)(param_1 + 0x24);\r\n  lVar2 = D2_GameSingletonPtr + 0x1f5338;\r\n  FUN_1407d9050(lVar2,uVar1,DAT_14398bb00,param_2);\r\n  FUN_1407d9050(lVar2,uVar1,DAT_14398bb08,param_3);\r\n  FUN_1407d9050(lVar2,uVar1,DAT_14398baf8,0);\r\n  FUN_1407d9050(lVar2,uVar1,DAT_14398bb10,0);\r\n  FUN_1407d4cb0(lVar2,uVar1,DAT_14398baf0,0);\r\n  return;\r\n}\r\n\r\n"
        },
        "isError": false
      }
    }
  },
  {
    "status": "fulfilled",
    "value": {
      "address": "14068eee0",
      "result": {
        "content": [
          {
            "type": "text",
            "text": "{\"error\":\"No function found for 0x14068eee0\"}"
          }
        ],
        "structuredContent": {
          "result": "{\"error\":\"No function found for 0x14068eee0\"}"
        },
        "isError": false
      }
    }
  },
  {
    "status": "fulfilled",
    "value": {
      "address": "140447410",
      "result": {
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
  },
  {
    "status": "fulfilled",
    "value": {
      "address": "140447520",
      "result": {
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
    }
  }
]

````

## Unescaped text fields for reading

### text

````text

void FUN_14068ec30(longlong param_1,undefined1 param_2,undefined1 param_3)

{
  undefined4 uVar1;
  longlong lVar2;
  
  uVar1 = *(undefined4 *)(param_1 + 0x24);
  lVar2 = D2_GameSingletonPtr + 0x1f5338;
  FUN_1407d9050(lVar2,uVar1,DAT_14398bb00,param_2);
  FUN_1407d9050(lVar2,uVar1,DAT_14398bb08,param_3);
  FUN_1407d9050(lVar2,uVar1,DAT_14398baf8,0);
  FUN_1407d9050(lVar2,uVar1,DAT_14398bb10,0);
  FUN_1407d4cb0(lVar2,uVar1,DAT_14398baf0,0);
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
