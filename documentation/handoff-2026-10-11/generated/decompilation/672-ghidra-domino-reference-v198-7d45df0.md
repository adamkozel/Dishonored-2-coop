# ghidra-domino-reference-v198.json

Original: [work/ghidra-domino-reference-v198.json](<../../../../work/ghidra-domino-reference-v198.json>)

SHA-256: `38D23D339DE158E5DF2D64B348F0DC84619333C11B10235CFCA17A19D6B042F4`; 5177 bytes.

## Original saved record

````json
{
  "program": "Dishonored2.exe",
  "addresses": [
    "1403da910",
    "1404db600",
    "1404dc100",
    "1404dcc30"
  ],
  "results": [
    {
      "status": "fulfilled",
      "value": {
        "content": [
          {
            "type": "text",
            "text": "\r\nlonglong FUN_1403da910(int *param_1)\r\n\r\n{\r\n  longlong lVar1;\r\n  char cVar2;\r\n  \r\n  if ((*param_1 != -1) && (lVar1 = *(longlong *)(param_1 + 2), *param_1 == *(int *)(lVar1 + 0x18)))\r\n  {\r\n    cVar2 = FUN_1407e7390(lVar1);\r\n    if (cVar2 != '\\0') {\r\n      return lVar1;\r\n    }\r\n  }\r\n  return 0;\r\n}\r\n\r\n"
          }
        ],
        "structuredContent": {
          "result": "\r\nlonglong FUN_1403da910(int *param_1)\r\n\r\n{\r\n  longlong lVar1;\r\n  char cVar2;\r\n  \r\n  if ((*param_1 != -1) && (lVar1 = *(longlong *)(param_1 + 2), *param_1 == *(int *)(lVar1 + 0x18)))\r\n  {\r\n    cVar2 = FUN_1407e7390(lVar1);\r\n    if (cVar2 != '\\0') {\r\n      return lVar1;\r\n    }\r\n  }\r\n  return 0;\r\n}\r\n\r\n"
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
            "text": "\r\nvoid FUN_1404db600(longlong param_1,undefined8 param_2,undefined8 *param_3)\r\n\r\n{\r\n  longlong lVar1;\r\n  undefined8 *puVar2;\r\n  undefined4 *puVar3;\r\n  undefined1 local_res8 [8];\r\n  undefined8 local_28;\r\n  undefined8 local_20;\r\n  undefined8 local_18;\r\n  undefined4 local_10;\r\n  undefined2 local_c;\r\n  \r\n  lVar1 = FUN_140877720(param_2,&DAT_1439dc9d0,1);\r\n  FUN_1403dab80(param_1 + 8,lVar1);\r\n  if ((lVar1 == 0) || (*(char *)*param_3 == '\\0')) {\r\n    local_28 = 0;\r\n    local_20 = 0;\r\n    local_18 = 0;\r\n    puVar2 = &local_28;\r\n    local_10 = 0x3f800000;\r\n    local_c = 0xffff;\r\n  }\r\n  else {\r\n    lVar1 = FUN_140878470(param_2);\r\n    puVar2 = (undefined8 *)\r\n             FUN_14025c0c0(*(longlong *)(lVar1 + 0x1fe8) + 0x108,&DAT_1432d07a0,param_3);\r\n  }\r\n  FUN_14025b6a0(param_1 + 0x18,puVar2);\r\n  lVar1 = FUN_140877720(param_2,&DAT_1439edb90,1);\r\n  FUN_1403dab80(param_1 + 0x68,lVar1);\r\n  if (lVar1 != 0) {\r\n    puVar3 = (undefined4 *)FUN_14060ce50(lVar1,local_res8,&DAT_143981070);\r\n    if ((undefined4 *)(param_1 + 0x78) != puVar3) {\r\n      *(undefined4 *)(param_1 + 0x78) = *puVar3;\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
          }
        ],
        "structuredContent": {
          "result": "\r\nvoid FUN_1404db600(longlong param_1,undefined8 param_2,undefined8 *param_3)\r\n\r\n{\r\n  longlong lVar1;\r\n  undefined8 *puVar2;\r\n  undefined4 *puVar3;\r\n  undefined1 local_res8 [8];\r\n  undefined8 local_28;\r\n  undefined8 local_20;\r\n  undefined8 local_18;\r\n  undefined4 local_10;\r\n  undefined2 local_c;\r\n  \r\n  lVar1 = FUN_140877720(param_2,&DAT_1439dc9d0,1);\r\n  FUN_1403dab80(param_1 + 8,lVar1);\r\n  if ((lVar1 == 0) || (*(char *)*param_3 == '\\0')) {\r\n    local_28 = 0;\r\n    local_20 = 0;\r\n    local_18 = 0;\r\n    puVar2 = &local_28;\r\n    local_10 = 0x3f800000;\r\n    local_c = 0xffff;\r\n  }\r\n  else {\r\n    lVar1 = FUN_140878470(param_2);\r\n    puVar2 = (undefined8 *)\r\n             FUN_14025c0c0(*(longlong *)(lVar1 + 0x1fe8) + 0x108,&DAT_1432d07a0,param_3);\r\n  }\r\n  FUN_14025b6a0(param_1 + 0x18,puVar2);\r\n  lVar1 = FUN_140877720(param_2,&DAT_1439edb90,1);\r\n  FUN_1403dab80(param_1 + 0x68,lVar1);\r\n  if (lVar1 != 0) {\r\n    puVar3 = (undefined4 *)FUN_14060ce50(lVar1,local_res8,&DAT_143981070);\r\n    if ((undefined4 *)(param_1 + 0x78) != puVar3) {\r\n      *(undefined4 *)(param_1 + 0x78) = *puVar3;\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
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
            "text": "{\"error\":\"No function found for 0x1404dc100\"}"
          }
        ],
        "structuredContent": {
          "result": "{\"error\":\"No function found for 0x1404dc100\"}"
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
            "text": "\r\nvoid FUN_1404dcc30(undefined4 *param_1,longlong param_2,undefined8 param_3)\r\n\r\n{\r\n  *(undefined8 *)(param_1 + 4) = param_3;\r\n  *(longlong *)(param_1 + 2) = param_2;\r\n  *param_1 = 0;\r\n  *(int *)(param_2 + 0x84) = *(int *)(param_2 + 0x84) + 1;\r\n  *(int *)(*(longlong *)(param_1 + 4) + 0x84) = *(int *)(*(longlong *)(param_1 + 4) + 0x84) + 1;\r\n  param_1[10] = 0;\r\n  return;\r\n}\r\n\r\n"
          }
        ],
        "structuredContent": {
          "result": "\r\nvoid FUN_1404dcc30(undefined4 *param_1,longlong param_2,undefined8 param_3)\r\n\r\n{\r\n  *(undefined8 *)(param_1 + 4) = param_3;\r\n  *(longlong *)(param_1 + 2) = param_2;\r\n  *param_1 = 0;\r\n  *(int *)(param_2 + 0x84) = *(int *)(param_2 + 0x84) + 1;\r\n  *(int *)(*(longlong *)(param_1 + 4) + 0x84) = *(int *)(*(longlong *)(param_1 + 4) + 0x84) + 1;\r\n  param_1[10] = 0;\r\n  return;\r\n}\r\n\r\n"
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

### text

````text

void FUN_1404db600(longlong param_1,undefined8 param_2,undefined8 *param_3)

{
  longlong lVar1;
  undefined8 *puVar2;
  undefined4 *puVar3;
  undefined1 local_res8 [8];
  undefined8 local_28;
  undefined8 local_20;
  undefined8 local_18;
  undefined4 local_10;
  undefined2 local_c;
  
  lVar1 = FUN_140877720(param_2,&DAT_1439dc9d0,1);
  FUN_1403dab80(param_1 + 8,lVar1);
  if ((lVar1 == 0) || (*(char *)*param_3 == '\0')) {
    local_28 = 0;
    local_20 = 0;
    local_18 = 0;
    puVar2 = &local_28;
    local_10 = 0x3f800000;
    local_c = 0xffff;
  }
  else {
    lVar1 = FUN_140878470(param_2);
    puVar2 = (undefined8 *)
             FUN_14025c0c0(*(longlong *)(lVar1 + 0x1fe8) + 0x108,&DAT_1432d07a0,param_3);
  }
  FUN_14025b6a0(param_1 + 0x18,puVar2);
  lVar1 = FUN_140877720(param_2,&DAT_1439edb90,1);
  FUN_1403dab80(param_1 + 0x68,lVar1);
  if (lVar1 != 0) {
    puVar3 = (undefined4 *)FUN_14060ce50(lVar1,local_res8,&DAT_143981070);
    if ((undefined4 *)(param_1 + 0x78) != puVar3) {
      *(undefined4 *)(param_1 + 0x78) = *puVar3;
    }
  }
  return;
}


````

### text

````text

void FUN_1404dcc30(undefined4 *param_1,longlong param_2,undefined8 param_3)

{
  *(undefined8 *)(param_1 + 4) = param_3;
  *(longlong *)(param_1 + 2) = param_2;
  *param_1 = 0;
  *(int *)(param_2 + 0x84) = *(int *)(param_2 + 0x84) + 1;
  *(int *)(*(longlong *)(param_1 + 4) + 0x84) = *(int *)(*(longlong *)(param_1 + 4) + 0x84) + 1;
  param_1[10] = 0;
  return;
}


````
