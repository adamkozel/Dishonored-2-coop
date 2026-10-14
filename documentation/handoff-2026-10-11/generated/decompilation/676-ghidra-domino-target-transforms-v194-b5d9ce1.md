# ghidra-domino-target-transforms-v194.json

Original: [work/ghidra-domino-target-transforms-v194.json](<../../../../work/ghidra-domino-target-transforms-v194.json>)

SHA-256: `3B49E9F9597E579F121869D988327BD6369109ECBC70963B3835EA5C744D4329`; 11491 bytes.

## Original saved record

````json
{
  "program": "Dishonored2.exe",
  "addresses": [
    "1404db600",
    "1404ddf10",
    "1404bd900",
    "1404be520"
  ],
  "results": [
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
            "text": "\r\nvoid FUN_1404ddf10(longlong param_1)\r\n\r\n{\r\n  char cVar1;\r\n  longlong lVar2;\r\n  \r\n  lVar2 = FUN_1403da910(param_1 + 8,1);\r\n  if (lVar2 != 0) {\r\n    cVar1 = FUN_14053dac0(lVar2,3,param_1 + 0x18,param_1 + 0x38,(undefined4 *)(param_1 + 0x44));\r\n    if (cVar1 == '\\0') {\r\n      lVar2 = *(longlong *)(lVar2 + 0x10);\r\n      *(undefined4 *)(param_1 + 0x38) = *(undefined4 *)(lVar2 + 0x300);\r\n      *(undefined4 *)(param_1 + 0x3c) = *(undefined4 *)(lVar2 + 0x304);\r\n      *(undefined4 *)(param_1 + 0x40) = *(undefined4 *)(lVar2 + 0x308);\r\n      *(undefined4 *)(param_1 + 0x44) = *(undefined4 *)(lVar2 + 0x30c);\r\n      *(undefined4 *)(param_1 + 0x48) = *(undefined4 *)(lVar2 + 0x310);\r\n      *(undefined4 *)(param_1 + 0x4c) = *(undefined4 *)(lVar2 + 0x314);\r\n      *(undefined4 *)(param_1 + 0x50) = *(undefined4 *)(lVar2 + 0x318);\r\n      *(undefined4 *)(param_1 + 0x54) = *(undefined4 *)(lVar2 + 0x31c);\r\n      *(undefined4 *)(param_1 + 0x58) = *(undefined4 *)(lVar2 + 800);\r\n      *(undefined4 *)(param_1 + 0x5c) = *(undefined4 *)(lVar2 + 0x324);\r\n      *(undefined4 *)(param_1 + 0x60) = *(undefined4 *)(lVar2 + 0x328);\r\n      *(undefined4 *)(param_1 + 100) = *(undefined4 *)(lVar2 + 0x32c);\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
          }
        ],
        "structuredContent": {
          "result": "\r\nvoid FUN_1404ddf10(longlong param_1)\r\n\r\n{\r\n  char cVar1;\r\n  longlong lVar2;\r\n  \r\n  lVar2 = FUN_1403da910(param_1 + 8,1);\r\n  if (lVar2 != 0) {\r\n    cVar1 = FUN_14053dac0(lVar2,3,param_1 + 0x18,param_1 + 0x38,(undefined4 *)(param_1 + 0x44));\r\n    if (cVar1 == '\\0') {\r\n      lVar2 = *(longlong *)(lVar2 + 0x10);\r\n      *(undefined4 *)(param_1 + 0x38) = *(undefined4 *)(lVar2 + 0x300);\r\n      *(undefined4 *)(param_1 + 0x3c) = *(undefined4 *)(lVar2 + 0x304);\r\n      *(undefined4 *)(param_1 + 0x40) = *(undefined4 *)(lVar2 + 0x308);\r\n      *(undefined4 *)(param_1 + 0x44) = *(undefined4 *)(lVar2 + 0x30c);\r\n      *(undefined4 *)(param_1 + 0x48) = *(undefined4 *)(lVar2 + 0x310);\r\n      *(undefined4 *)(param_1 + 0x4c) = *(undefined4 *)(lVar2 + 0x314);\r\n      *(undefined4 *)(param_1 + 0x50) = *(undefined4 *)(lVar2 + 0x318);\r\n      *(undefined4 *)(param_1 + 0x54) = *(undefined4 *)(lVar2 + 0x31c);\r\n      *(undefined4 *)(param_1 + 0x58) = *(undefined4 *)(lVar2 + 800);\r\n      *(undefined4 *)(param_1 + 0x5c) = *(undefined4 *)(lVar2 + 0x324);\r\n      *(undefined4 *)(param_1 + 0x60) = *(undefined4 *)(lVar2 + 0x328);\r\n      *(undefined4 *)(param_1 + 100) = *(undefined4 *)(lVar2 + 0x32c);\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
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
            "text": "\r\nvoid FUN_1404bd900(longlong param_1,longlong param_2)\r\n\r\n{\r\n  longlong *plVar1;\r\n  char cVar2;\r\n  longlong *plVar3;\r\n  undefined4 local_108 [2];\r\n  undefined1 local_100 [144];\r\n  undefined8 local_70;\r\n  undefined8 local_68;\r\n  undefined1 local_60 [24];\r\n  undefined1 local_48 [64];\r\n  \r\n  cVar2 = FUN_1404dbee0(param_2);\r\n  if (cVar2 == '\\0') {\r\n    plVar3 = *(longlong **)(param_1 + 0x28);\r\n    plVar1 = plVar3 + (*(uint *)(param_1 + 0x30) & 0xffffff);\r\n    for (; plVar3 != plVar1; plVar3 = plVar3 + 1) {\r\n      param_2 = *plVar3;\r\n      cVar2 = FUN_1404dbee0(param_2);\r\n      if (cVar2 != '\\0') goto LAB_1404bd95a;\r\n    }\r\n  }\r\n  else {\r\nLAB_1404bd95a:\r\n    if (param_2 != 0) {\r\n      FUN_140780390(local_100);\r\n      local_70 = 0;\r\n      local_68 = 0;\r\n      FUN_140131ae0(local_60);\r\n      FUN_1404db250(local_48);\r\n      local_108[0] = 3;\r\n      FUN_1404bfaa0(param_1,param_2,local_108);\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
          }
        ],
        "structuredContent": {
          "result": "\r\nvoid FUN_1404bd900(longlong param_1,longlong param_2)\r\n\r\n{\r\n  longlong *plVar1;\r\n  char cVar2;\r\n  longlong *plVar3;\r\n  undefined4 local_108 [2];\r\n  undefined1 local_100 [144];\r\n  undefined8 local_70;\r\n  undefined8 local_68;\r\n  undefined1 local_60 [24];\r\n  undefined1 local_48 [64];\r\n  \r\n  cVar2 = FUN_1404dbee0(param_2);\r\n  if (cVar2 == '\\0') {\r\n    plVar3 = *(longlong **)(param_1 + 0x28);\r\n    plVar1 = plVar3 + (*(uint *)(param_1 + 0x30) & 0xffffff);\r\n    for (; plVar3 != plVar1; plVar3 = plVar3 + 1) {\r\n      param_2 = *plVar3;\r\n      cVar2 = FUN_1404dbee0(param_2);\r\n      if (cVar2 != '\\0') goto LAB_1404bd95a;\r\n    }\r\n  }\r\n  else {\r\nLAB_1404bd95a:\r\n    if (param_2 != 0) {\r\n      FUN_140780390(local_100);\r\n      local_70 = 0;\r\n      local_68 = 0;\r\n      FUN_140131ae0(local_60);\r\n      FUN_1404db250(local_48);\r\n      local_108[0] = 3;\r\n      FUN_1404bfaa0(param_1,param_2,local_108);\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
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
            "text": "\r\nvoid FUN_1404be520(longlong param_1,undefined8 param_2,undefined8 param_3)\r\n\r\n{\r\n  undefined8 *puVar1;\r\n  ulonglong uVar2;\r\n  ushort uVar3;\r\n  int iVar4;\r\n  undefined8 *puVar5;\r\n  ulonglong *puVar6;\r\n  ulonglong uVar7;\r\n  ulonglong uVar8;\r\n  ulonglong local_res8;\r\n  \r\n  uVar3 = FUN_1404bcb10(param_1 + 0x15c0,0xffff);\r\n  uVar7 = 0;\r\n  local_res8 = uVar7;\r\n  if (uVar3 != 0xffff) {\r\n    local_res8 = (ulonglong)uVar3 * 0x80 + *(longlong *)(param_1 + 0x15c0);\r\n  }\r\n  if (local_res8 == 0) {\r\n    puVar5 = *(undefined8 **)(param_1 + 0x15b0);\r\n    puVar1 = puVar5 + (*(uint *)(param_1 + 0x15b8) & 0xffffff);\r\n    for (; puVar5 != puVar1; puVar5 = puVar5 + 1) {\r\n      if (*(int *)*puVar5 == 2) {\r\n        FUN_1404db8a0((int *)*puVar5,param_1);\r\n        uVar3 = FUN_1404bcb10(param_1 + 0x15c0,0xffff);\r\n        local_res8 = uVar7;\r\n        if (uVar3 != 0xffff) {\r\n          local_res8 = (ulonglong)uVar3 * 0x80 + *(longlong *)(param_1 + 0x15c0);\r\n        }\r\n        break;\r\n      }\r\n    }\r\n  }\r\n  uVar2 = local_res8;\r\n  FUN_1404dcc30(local_res8,param_2,param_3);\r\n  iVar4 = (*(uint *)(param_1 + 0x15b8) & 0xffffff) - 1;\r\n  if (-1 < iVar4) {\r\n    puVar6 = *(ulonglong **)(param_1 + 0x15b0);\r\n    uVar8 = uVar7;\r\n    do {\r\n      if (*puVar6 == uVar2) {\r\n        if (-1 < (int)uVar8) {\r\n          return;\r\n        }\r\n        break;\r\n      }\r\n      uVar7 = uVar7 + 1;\r\n      uVar8 = (ulonglong)((int)uVar8 + 1);\r\n      puVar6 = puVar6 + 1;\r\n    } while ((longlong)uVar7 <= (longlong)iVar4);\r\n  }\r\n  FUN_1408918f0(param_1 + 0x15b0,&local_res8);\r\n  return;\r\n}\r\n\r\n"
          }
        ],
        "structuredContent": {
          "result": "\r\nvoid FUN_1404be520(longlong param_1,undefined8 param_2,undefined8 param_3)\r\n\r\n{\r\n  undefined8 *puVar1;\r\n  ulonglong uVar2;\r\n  ushort uVar3;\r\n  int iVar4;\r\n  undefined8 *puVar5;\r\n  ulonglong *puVar6;\r\n  ulonglong uVar7;\r\n  ulonglong uVar8;\r\n  ulonglong local_res8;\r\n  \r\n  uVar3 = FUN_1404bcb10(param_1 + 0x15c0,0xffff);\r\n  uVar7 = 0;\r\n  local_res8 = uVar7;\r\n  if (uVar3 != 0xffff) {\r\n    local_res8 = (ulonglong)uVar3 * 0x80 + *(longlong *)(param_1 + 0x15c0);\r\n  }\r\n  if (local_res8 == 0) {\r\n    puVar5 = *(undefined8 **)(param_1 + 0x15b0);\r\n    puVar1 = puVar5 + (*(uint *)(param_1 + 0x15b8) & 0xffffff);\r\n    for (; puVar5 != puVar1; puVar5 = puVar5 + 1) {\r\n      if (*(int *)*puVar5 == 2) {\r\n        FUN_1404db8a0((int *)*puVar5,param_1);\r\n        uVar3 = FUN_1404bcb10(param_1 + 0x15c0,0xffff);\r\n        local_res8 = uVar7;\r\n        if (uVar3 != 0xffff) {\r\n          local_res8 = (ulonglong)uVar3 * 0x80 + *(longlong *)(param_1 + 0x15c0);\r\n        }\r\n        break;\r\n      }\r\n    }\r\n  }\r\n  uVar2 = local_res8;\r\n  FUN_1404dcc30(local_res8,param_2,param_3);\r\n  iVar4 = (*(uint *)(param_1 + 0x15b8) & 0xffffff) - 1;\r\n  if (-1 < iVar4) {\r\n    puVar6 = *(ulonglong **)(param_1 + 0x15b0);\r\n    uVar8 = uVar7;\r\n    do {\r\n      if (*puVar6 == uVar2) {\r\n        if (-1 < (int)uVar8) {\r\n          return;\r\n        }\r\n        break;\r\n      }\r\n      uVar7 = uVar7 + 1;\r\n      uVar8 = (ulonglong)((int)uVar8 + 1);\r\n      puVar6 = puVar6 + 1;\r\n    } while ((longlong)uVar7 <= (longlong)iVar4);\r\n  }\r\n  FUN_1408918f0(param_1 + 0x15b0,&local_res8);\r\n  return;\r\n}\r\n\r\n"
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

void FUN_1404ddf10(longlong param_1)

{
  char cVar1;
  longlong lVar2;
  
  lVar2 = FUN_1403da910(param_1 + 8,1);
  if (lVar2 != 0) {
    cVar1 = FUN_14053dac0(lVar2,3,param_1 + 0x18,param_1 + 0x38,(undefined4 *)(param_1 + 0x44));
    if (cVar1 == '\0') {
      lVar2 = *(longlong *)(lVar2 + 0x10);
      *(undefined4 *)(param_1 + 0x38) = *(undefined4 *)(lVar2 + 0x300);
      *(undefined4 *)(param_1 + 0x3c) = *(undefined4 *)(lVar2 + 0x304);
      *(undefined4 *)(param_1 + 0x40) = *(undefined4 *)(lVar2 + 0x308);
      *(undefined4 *)(param_1 + 0x44) = *(undefined4 *)(lVar2 + 0x30c);
      *(undefined4 *)(param_1 + 0x48) = *(undefined4 *)(lVar2 + 0x310);
      *(undefined4 *)(param_1 + 0x4c) = *(undefined4 *)(lVar2 + 0x314);
      *(undefined4 *)(param_1 + 0x50) = *(undefined4 *)(lVar2 + 0x318);
      *(undefined4 *)(param_1 + 0x54) = *(undefined4 *)(lVar2 + 0x31c);
      *(undefined4 *)(param_1 + 0x58) = *(undefined4 *)(lVar2 + 800);
      *(undefined4 *)(param_1 + 0x5c) = *(undefined4 *)(lVar2 + 0x324);
      *(undefined4 *)(param_1 + 0x60) = *(undefined4 *)(lVar2 + 0x328);
      *(undefined4 *)(param_1 + 100) = *(undefined4 *)(lVar2 + 0x32c);
    }
  }
  return;
}


````

### text

````text

void FUN_1404bd900(longlong param_1,longlong param_2)

{
  longlong *plVar1;
  char cVar2;
  longlong *plVar3;
  undefined4 local_108 [2];
  undefined1 local_100 [144];
  undefined8 local_70;
  undefined8 local_68;
  undefined1 local_60 [24];
  undefined1 local_48 [64];
  
  cVar2 = FUN_1404dbee0(param_2);
  if (cVar2 == '\0') {
    plVar3 = *(longlong **)(param_1 + 0x28);
    plVar1 = plVar3 + (*(uint *)(param_1 + 0x30) & 0xffffff);
    for (; plVar3 != plVar1; plVar3 = plVar3 + 1) {
      param_2 = *plVar3;
      cVar2 = FUN_1404dbee0(param_2);
      if (cVar2 != '\0') goto LAB_1404bd95a;
    }
  }
  else {
LAB_1404bd95a:
    if (param_2 != 0) {
      FUN_140780390(local_100);
      local_70 = 0;
      local_68 = 0;
      FUN_140131ae0(local_60);
      FUN_1404db250(local_48);
      local_108[0] = 3;
      FUN_1404bfaa0(param_1,param_2,local_108);
    }
  }
  return;
}


````

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
