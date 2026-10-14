# ghidra-domino-nodes-v193.json

Original: [work/ghidra-domino-nodes-v193.json](<../../../../work/ghidra-domino-nodes-v193.json>)

SHA-256: `35745DD0C734BF1B2DAFC3FE545D08FEB8F7A9A930623BA2B1AF1C0D1B4317B0`; 2974 bytes.

## Original saved record

````json
{
  "program": "Dishonored2.exe",
  "addresses": [
    "1404db590",
    "1404db5c0",
    "1404bd2e0"
  ],
  "results": [
    {
      "status": "fulfilled",
      "value": {
        "content": [
          {
            "type": "text",
            "text": "\r\nundefined8 FUN_1404db590(longlong param_1)\r\n\r\n{\r\n  longlong lVar1;\r\n  \r\n  lVar1 = FUN_1403da910(param_1 + 8,1);\r\n  if (lVar1 == 0) {\r\n    return 0;\r\n  }\r\n  return *(undefined8 *)(lVar1 + 0x10);\r\n}\r\n\r\n"
          }
        ],
        "structuredContent": {
          "result": "\r\nundefined8 FUN_1404db590(longlong param_1)\r\n\r\n{\r\n  longlong lVar1;\r\n  \r\n  lVar1 = FUN_1403da910(param_1 + 8,1);\r\n  if (lVar1 == 0) {\r\n    return 0;\r\n  }\r\n  return *(undefined8 *)(lVar1 + 0x10);\r\n}\r\n\r\n"
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
            "text": "\r\nvoid FUN_1404db5c0(undefined4 *param_1,longlong param_2)\r\n\r\n{\r\n  *param_1 = *(undefined4 *)(param_2 + 0xa8);\r\n  FUN_1404db600();\r\n  *(byte *)(param_1 + 0x23) = *(byte *)(param_1 + 0x23) & 0xfa;\r\n  param_1[0x21] = 0;\r\n  FUN_1404ddf10(param_1);\r\n  return;\r\n}\r\n\r\n"
          }
        ],
        "structuredContent": {
          "result": "\r\nvoid FUN_1404db5c0(undefined4 *param_1,longlong param_2)\r\n\r\n{\r\n  *param_1 = *(undefined4 *)(param_2 + 0xa8);\r\n  FUN_1404db600();\r\n  *(byte *)(param_1 + 0x23) = *(byte *)(param_1 + 0x23) & 0xfa;\r\n  param_1[0x21] = 0;\r\n  FUN_1404ddf10(param_1);\r\n  return;\r\n}\r\n\r\n"
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
            "text": "\r\nvoid FUN_1404bd2e0(longlong param_1,longlong param_2)\r\n\r\n{\r\n  longlong *plVar1;\r\n  longlong lVar2;\r\n  longlong *plVar3;\r\n  \r\n  FUN_1404c0050();\r\n  plVar3 = *(longlong **)(param_1 + 0x28);\r\n  plVar1 = plVar3 + (*(uint *)(param_1 + 0x30) & 0xffffff);\r\n  for (; plVar3 != plVar1; plVar3 = plVar3 + 1) {\r\n    lVar2 = *plVar3;\r\n    if (lVar2 != param_2) {\r\n      FUN_1404be520(param_1,param_2,lVar2);\r\n      FUN_1404be520(param_1,lVar2,param_2);\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
          }
        ],
        "structuredContent": {
          "result": "\r\nvoid FUN_1404bd2e0(longlong param_1,longlong param_2)\r\n\r\n{\r\n  longlong *plVar1;\r\n  longlong lVar2;\r\n  longlong *plVar3;\r\n  \r\n  FUN_1404c0050();\r\n  plVar3 = *(longlong **)(param_1 + 0x28);\r\n  plVar1 = plVar3 + (*(uint *)(param_1 + 0x30) & 0xffffff);\r\n  for (; plVar3 != plVar1; plVar3 = plVar3 + 1) {\r\n    lVar2 = *plVar3;\r\n    if (lVar2 != param_2) {\r\n      FUN_1404be520(param_1,param_2,lVar2);\r\n      FUN_1404be520(param_1,lVar2,param_2);\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
        },
        "isError": false
      }
    }
  ]
}

````

## Unescaped text fields for reading

### text

````text

undefined8 FUN_1404db590(longlong param_1)

{
  longlong lVar1;
  
  lVar1 = FUN_1403da910(param_1 + 8,1);
  if (lVar1 == 0) {
    return 0;
  }
  return *(undefined8 *)(lVar1 + 0x10);
}


````

### text

````text

void FUN_1404db5c0(undefined4 *param_1,longlong param_2)

{
  *param_1 = *(undefined4 *)(param_2 + 0xa8);
  FUN_1404db600();
  *(byte *)(param_1 + 0x23) = *(byte *)(param_1 + 0x23) & 0xfa;
  param_1[0x21] = 0;
  FUN_1404ddf10(param_1);
  return;
}


````

### text

````text

void FUN_1404bd2e0(longlong param_1,longlong param_2)

{
  longlong *plVar1;
  longlong lVar2;
  longlong *plVar3;
  
  FUN_1404c0050();
  plVar3 = *(longlong **)(param_1 + 0x28);
  plVar1 = plVar3 + (*(uint *)(param_1 + 0x30) & 0xffffff);
  for (; plVar3 != plVar1; plVar3 = plVar3 + 1) {
    lVar2 = *plVar3;
    if (lVar2 != param_2) {
      FUN_1404be520(param_1,param_2,lVar2);
      FUN_1404be520(param_1,lVar2,param_2);
    }
  }
  return;
}


````
