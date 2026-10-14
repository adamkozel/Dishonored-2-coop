# ghidra-domino-connections-v212.json

Original: [work/ghidra-domino-connections-v212.json](<../../../../work/ghidra-domino-connections-v212.json>)

SHA-256: `FF542392F7A819F055764B58CBD5AB550E4F9E61C1B9D559F8AEDDAB639B933E`; 6062 bytes.

## Original saved record

````json
{
  "addresses": [
    "1404be520",
    "1404dcc30",
    "1404bd2e0"
  ],
  "results": [
    {
      "status": "fulfilled",
      "value": {
        "content": [
          {
            "type": "text",
            "text": "\r\nvoid FUN_1404be520(longlong param_1,undefined8 param_2,undefined8 param_3)\r\n\r\n{\r\n  undefined8 *puVar1;\r\n  ulonglong uVar2;\r\n  ushort uVar3;\r\n  int iVar4;\r\n  undefined8 *puVar5;\r\n  ulonglong *puVar6;\r\n  ulonglong uVar7;\r\n  ulonglong uVar8;\r\n  ulonglong local_res8;\r\n  \r\n  uVar3 = FUN_1404bcb10(param_1 + 0x15c0,0xffff);\r\n  uVar7 = 0;\r\n  local_res8 = uVar7;\r\n  if (uVar3 != 0xffff) {\r\n    local_res8 = (ulonglong)uVar3 * 0x80 + *(longlong *)(param_1 + 0x15c0);\r\n  }\r\n  if (local_res8 == 0) {\r\n    puVar5 = *(undefined8 **)(param_1 + 0x15b0);\r\n    puVar1 = puVar5 + (*(uint *)(param_1 + 0x15b8) & 0xffffff);\r\n    for (; puVar5 != puVar1; puVar5 = puVar5 + 1) {\r\n      if (*(int *)*puVar5 == 2) {\r\n        FUN_1404db8a0((int *)*puVar5,param_1);\r\n        uVar3 = FUN_1404bcb10(param_1 + 0x15c0,0xffff);\r\n        local_res8 = uVar7;\r\n        if (uVar3 != 0xffff) {\r\n          local_res8 = (ulonglong)uVar3 * 0x80 + *(longlong *)(param_1 + 0x15c0);\r\n        }\r\n        break;\r\n      }\r\n    }\r\n  }\r\n  uVar2 = local_res8;\r\n  FUN_1404dcc30(local_res8,param_2,param_3);\r\n  iVar4 = (*(uint *)(param_1 + 0x15b8) & 0xffffff) - 1;\r\n  if (-1 < iVar4) {\r\n    puVar6 = *(ulonglong **)(param_1 + 0x15b0);\r\n    uVar8 = uVar7;\r\n    do {\r\n      if (*puVar6 == uVar2) {\r\n        if (-1 < (int)uVar8) {\r\n          return;\r\n        }\r\n        break;\r\n      }\r\n      uVar7 = uVar7 + 1;\r\n      uVar8 = (ulonglong)((int)uVar8 + 1);\r\n      puVar6 = puVar6 + 1;\r\n    } while ((longlong)uVar7 <= (longlong)iVar4);\r\n  }\r\n  FUN_1408918f0(param_1 + 0x15b0,&local_res8);\r\n  return;\r\n}\r\n\r\n"
          }
        ],
        "structuredContent": {
          "result": "\r\nvoid FUN_1404be520(longlong param_1,undefined8 param_2,undefined8 param_3)\r\n\r\n{\r\n  undefined8 *puVar1;\r\n  ulonglong uVar2;\r\n  ushort uVar3;\r\n  int iVar4;\r\n  undefined8 *puVar5;\r\n  ulonglong *puVar6;\r\n  ulonglong uVar7;\r\n  ulonglong uVar8;\r\n  ulonglong local_res8;\r\n  \r\n  uVar3 = FUN_1404bcb10(param_1 + 0x15c0,0xffff);\r\n  uVar7 = 0;\r\n  local_res8 = uVar7;\r\n  if (uVar3 != 0xffff) {\r\n    local_res8 = (ulonglong)uVar3 * 0x80 + *(longlong *)(param_1 + 0x15c0);\r\n  }\r\n  if (local_res8 == 0) {\r\n    puVar5 = *(undefined8 **)(param_1 + 0x15b0);\r\n    puVar1 = puVar5 + (*(uint *)(param_1 + 0x15b8) & 0xffffff);\r\n    for (; puVar5 != puVar1; puVar5 = puVar5 + 1) {\r\n      if (*(int *)*puVar5 == 2) {\r\n        FUN_1404db8a0((int *)*puVar5,param_1);\r\n        uVar3 = FUN_1404bcb10(param_1 + 0x15c0,0xffff);\r\n        local_res8 = uVar7;\r\n        if (uVar3 != 0xffff) {\r\n          local_res8 = (ulonglong)uVar3 * 0x80 + *(longlong *)(param_1 + 0x15c0);\r\n        }\r\n        break;\r\n      }\r\n    }\r\n  }\r\n  uVar2 = local_res8;\r\n  FUN_1404dcc30(local_res8,param_2,param_3);\r\n  iVar4 = (*(uint *)(param_1 + 0x15b8) & 0xffffff) - 1;\r\n  if (-1 < iVar4) {\r\n    puVar6 = *(ulonglong **)(param_1 + 0x15b0);\r\n    uVar8 = uVar7;\r\n    do {\r\n      if (*puVar6 == uVar2) {\r\n        if (-1 < (int)uVar8) {\r\n          return;\r\n        }\r\n        break;\r\n      }\r\n      uVar7 = uVar7 + 1;\r\n      uVar8 = (ulonglong)((int)uVar8 + 1);\r\n      puVar6 = puVar6 + 1;\r\n    } while ((longlong)uVar7 <= (longlong)iVar4);\r\n  }\r\n  FUN_1408918f0(param_1 + 0x15b0,&local_res8);\r\n  return;\r\n}\r\n\r\n"
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

void FUN_1404be520(longlong param_1,undefined8 param_2,undefined8 param_3)

{
  undefined8 *puVar1;
  ulonglong uVar2;
  ushort uVar3;
  int iVar4;
  undefined8 *puVar5;
  ulonglong *puVar6;
  ulonglong uVar7;
  ulonglong uVar8;
  ulonglong local_res8;
  
  uVar3 = FUN_1404bcb10(param_1 + 0x15c0,0xffff);
  uVar7 = 0;
  local_res8 = uVar7;
  if (uVar3 != 0xffff) {
    local_res8 = (ulonglong)uVar3 * 0x80 + *(longlong *)(param_1 + 0x15c0);
  }
  if (local_res8 == 0) {
    puVar5 = *(undefined8 **)(param_1 + 0x15b0);
    puVar1 = puVar5 + (*(uint *)(param_1 + 0x15b8) & 0xffffff);
    for (; puVar5 != puVar1; puVar5 = puVar5 + 1) {
      if (*(int *)*puVar5 == 2) {
        FUN_1404db8a0((int *)*puVar5,param_1);
        uVar3 = FUN_1404bcb10(param_1 + 0x15c0,0xffff);
        local_res8 = uVar7;
        if (uVar3 != 0xffff) {
          local_res8 = (ulonglong)uVar3 * 0x80 + *(longlong *)(param_1 + 0x15c0);
        }
        break;
      }
    }
  }
  uVar2 = local_res8;
  FUN_1404dcc30(local_res8,param_2,param_3);
  iVar4 = (*(uint *)(param_1 + 0x15b8) & 0xffffff) - 1;
  if (-1 < iVar4) {
    puVar6 = *(ulonglong **)(param_1 + 0x15b0);
    uVar8 = uVar7;
    do {
      if (*puVar6 == uVar2) {
        if (-1 < (int)uVar8) {
          return;
        }
        break;
      }
      uVar7 = uVar7 + 1;
      uVar8 = (ulonglong)((int)uVar8 + 1);
      puVar6 = puVar6 + 1;
    } while ((longlong)uVar7 <= (longlong)iVar4);
  }
  FUN_1408918f0(param_1 + 0x15b0,&local_res8);
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
