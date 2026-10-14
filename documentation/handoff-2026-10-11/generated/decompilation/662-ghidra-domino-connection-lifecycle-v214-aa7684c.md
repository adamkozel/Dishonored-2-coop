# ghidra-domino-connection-lifecycle-v214.json

Original: [work/ghidra-domino-connection-lifecycle-v214.json](<../../../../work/ghidra-domino-connection-lifecycle-v214.json>)

SHA-256: `AA4A7597DB168273FD2232AA3496444B99A1DC4612A7ADF07E5CC031D0AB846A`; 20228 bytes.

## Original saved record

````json
{
  "retire": {
    "content": [
      {
        "type": "text",
        "text": "\r\nvoid FUN_1404db8a0(undefined4 *param_1,undefined8 param_2)\r\n\r\n{\r\n  *param_1 = 3;\r\n  *(int *)(*(longlong *)(param_1 + 2) + 0x84) = *(int *)(*(longlong *)(param_1 + 2) + 0x84) + -1;\r\n  *(int *)(*(longlong *)(param_1 + 4) + 0x84) = *(int *)(*(longlong *)(param_1 + 4) + 0x84) + -1;\r\n  FUN_1404bde00(param_2,param_1);\r\n  *(undefined8 *)(param_1 + 2) = 0;\r\n  *(undefined8 *)(param_1 + 4) = 0;\r\n  return;\r\n}\r\n\r\n"
      }
    ],
    "structuredContent": {
      "result": "\r\nvoid FUN_1404db8a0(undefined4 *param_1,undefined8 param_2)\r\n\r\n{\r\n  *param_1 = 3;\r\n  *(int *)(*(longlong *)(param_1 + 2) + 0x84) = *(int *)(*(longlong *)(param_1 + 2) + 0x84) + -1;\r\n  *(int *)(*(longlong *)(param_1 + 4) + 0x84) = *(int *)(*(longlong *)(param_1 + 4) + 0x84) + -1;\r\n  FUN_1404bde00(param_2,param_1);\r\n  *(undefined8 *)(param_1 + 2) = 0;\r\n  *(undefined8 *)(param_1 + 4) = 0;\r\n  return;\r\n}\r\n\r\n"
    },
    "isError": false
  },
  "functions": {
    "addresses": [
      "1404dcc60",
      "1404dcd10",
      "1404dcd80"
    ],
    "results": [
      {
        "status": "fulfilled",
        "value": {
          "content": [
            {
              "type": "text",
              "text": "\r\nvoid FUN_1404dcc60(longlong param_1)\r\n\r\n{\r\n  undefined4 uVar1;\r\n  undefined8 uVar2;\r\n  byte bVar3;\r\n  longlong lVar4;\r\n  \r\n  if (*(int *)(param_1 + 0x88) != 0) {\r\n    bVar3 = FUN_1407d63f0(D2_GameSingletonPtr + 0x1f5338,*(undefined4 *)(param_1 + 0x88));\r\n    if (bVar3 == 1) {\r\n      lVar4 = FUN_1403da910(param_1 + 8,1);\r\n      if ((lVar4 != 0) && (*(longlong *)(lVar4 + 0x10) != 0)) {\r\n        uVar1 = *(undefined4 *)(*(longlong *)(lVar4 + 0x10) + 0xa8);\r\n        uVar2 = FUN_140810240();\r\n        FUN_1408103f0(uVar2,0xb6,uVar1);\r\n      }\r\n      *(undefined4 *)(param_1 + 0x88) = 0;\r\n    }\r\n    *(byte *)(param_1 + 0x8c) = *(byte *)(param_1 + 0x8c) & 0xfb;\r\n    *(byte *)(param_1 + 0x8c) = *(byte *)(param_1 + 0x8c) | (bVar3 & 1) << 2;\r\n  }\r\n  return;\r\n}\r\n\r\n"
            }
          ],
          "structuredContent": {
            "result": "\r\nvoid FUN_1404dcc60(longlong param_1)\r\n\r\n{\r\n  undefined4 uVar1;\r\n  undefined8 uVar2;\r\n  byte bVar3;\r\n  longlong lVar4;\r\n  \r\n  if (*(int *)(param_1 + 0x88) != 0) {\r\n    bVar3 = FUN_1407d63f0(D2_GameSingletonPtr + 0x1f5338,*(undefined4 *)(param_1 + 0x88));\r\n    if (bVar3 == 1) {\r\n      lVar4 = FUN_1403da910(param_1 + 8,1);\r\n      if ((lVar4 != 0) && (*(longlong *)(lVar4 + 0x10) != 0)) {\r\n        uVar1 = *(undefined4 *)(*(longlong *)(lVar4 + 0x10) + 0xa8);\r\n        uVar2 = FUN_140810240();\r\n        FUN_1408103f0(uVar2,0xb6,uVar1);\r\n      }\r\n      *(undefined4 *)(param_1 + 0x88) = 0;\r\n    }\r\n    *(byte *)(param_1 + 0x8c) = *(byte *)(param_1 + 0x8c) & 0xfb;\r\n    *(byte *)(param_1 + 0x8c) = *(byte *)(param_1 + 0x8c) | (bVar3 & 1) << 2;\r\n  }\r\n  return;\r\n}\r\n\r\n"
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
              "text": "\r\nundefined8 FUN_1404dcd10(longlong *param_1,uint param_2)\r\n\r\n{\r\n  uint uVar1;\r\n  \r\n  if (-1 < (int)param_2) {\r\n    uVar1 = *(uint *)(param_1 + 1);\r\n    if ((int)param_2 < (int)(uVar1 & 0xffffff)) {\r\n      uVar1 = (uVar1 - 1 ^ uVar1) & 0xffffff ^ uVar1;\r\n      *(uint *)(param_1 + 1) = uVar1;\r\n      if (param_2 != (uVar1 & 0xffffff)) {\r\n        FUN_1404bc690((longlong)(int)param_2 * 0x130 + *param_1,\r\n                      (ulonglong)(uVar1 & 0xffffff) * 0x130 + *param_1);\r\n      }\r\n      return 1;\r\n    }\r\n  }\r\n  return 0;\r\n}\r\n\r\n"
            }
          ],
          "structuredContent": {
            "result": "\r\nundefined8 FUN_1404dcd10(longlong *param_1,uint param_2)\r\n\r\n{\r\n  uint uVar1;\r\n  \r\n  if (-1 < (int)param_2) {\r\n    uVar1 = *(uint *)(param_1 + 1);\r\n    if ((int)param_2 < (int)(uVar1 & 0xffffff)) {\r\n      uVar1 = (uVar1 - 1 ^ uVar1) & 0xffffff ^ uVar1;\r\n      *(uint *)(param_1 + 1) = uVar1;\r\n      if (param_2 != (uVar1 & 0xffffff)) {\r\n        FUN_1404bc690((longlong)(int)param_2 * 0x130 + *param_1,\r\n                      (ulonglong)(uVar1 & 0xffffff) * 0x130 + *param_1);\r\n      }\r\n      return 1;\r\n    }\r\n  }\r\n  return 0;\r\n}\r\n\r\n"
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
              "text": "\r\nundefined8 FUN_1404dcd80(longlong *param_1,uint param_2)\r\n\r\n{\r\n  uint uVar1;\r\n  longlong lVar2;\r\n  uint uVar3;\r\n  uint uVar4;\r\n  \r\n  uVar4 = *(uint *)((longlong)param_1 + 0xc);\r\n  if ((int)param_2 < 1) {\r\n    if ((uVar4 & 0x40) == 0) {\r\n      if (*param_1 != 0) {\r\n        FUN_1404bcc70(*param_1,uVar4 >> 8);\r\n      }\r\n      *(uint *)((longlong)param_1 + 0xc) = *(uint *)((longlong)param_1 + 0xc) & 0xff;\r\n      *param_1 = 0;\r\n    }\r\n    *(uint *)(param_1 + 1) = *(uint *)(param_1 + 1) & 0xff000000;\r\n    return 1;\r\n  }\r\n  uVar1 = uVar4 >> 8;\r\n  if (param_2 == uVar1) {\r\n    return 1;\r\n  }\r\n  uVar3 = uVar4 >> 6 & 3;\r\n  if ((uVar3 == 3) && ((int)uVar1 < (int)param_2)) {\r\n    FUN_1404bda80(param_1,param_2);\r\n    *(uint *)((longlong)param_1 + 0xc) = (uint)*(byte *)((longlong)param_1 + 0xc) | param_2 << 8;\r\n    return 1;\r\n  }\r\n  if (uVar3 == 0) {\r\n    if (uVar1 == 0) {\r\n      if (param_2 == 1 << ((byte)uVar4 & 0x1f)) {\r\n        FUN_1400998b0();\r\n      }\r\n    }\r\n    else {\r\n      FUN_1400998b0();\r\n    }\r\n    lVar2 = FUN_1404db460(param_1,param_2,0);\r\n    *param_1 = lVar2;\r\n    if (lVar2 != 0) {\r\n      *(uint *)((longlong)param_1 + 0xc) = *(uint *)((longlong)param_1 + 0xc) & 0xff;\r\n      *(uint *)((longlong)param_1 + 0xc) = *(uint *)((longlong)param_1 + 0xc) | param_2 << 8;\r\n      uVar4 = *(uint *)((longlong)param_1 + 0xc) >> 8;\r\n      if ((*(uint *)(param_1 + 1) & 0xffffff) <= uVar4) {\r\n        return 1;\r\n      }\r\n      *(uint *)(param_1 + 1) = *(uint *)(param_1 + 1) & 0xff000000 | uVar4;\r\n      return 1;\r\n    }\r\n  }\r\n  return 0;\r\n}\r\n\r\n"
            }
          ],
          "structuredContent": {
            "result": "\r\nundefined8 FUN_1404dcd80(longlong *param_1,uint param_2)\r\n\r\n{\r\n  uint uVar1;\r\n  longlong lVar2;\r\n  uint uVar3;\r\n  uint uVar4;\r\n  \r\n  uVar4 = *(uint *)((longlong)param_1 + 0xc);\r\n  if ((int)param_2 < 1) {\r\n    if ((uVar4 & 0x40) == 0) {\r\n      if (*param_1 != 0) {\r\n        FUN_1404bcc70(*param_1,uVar4 >> 8);\r\n      }\r\n      *(uint *)((longlong)param_1 + 0xc) = *(uint *)((longlong)param_1 + 0xc) & 0xff;\r\n      *param_1 = 0;\r\n    }\r\n    *(uint *)(param_1 + 1) = *(uint *)(param_1 + 1) & 0xff000000;\r\n    return 1;\r\n  }\r\n  uVar1 = uVar4 >> 8;\r\n  if (param_2 == uVar1) {\r\n    return 1;\r\n  }\r\n  uVar3 = uVar4 >> 6 & 3;\r\n  if ((uVar3 == 3) && ((int)uVar1 < (int)param_2)) {\r\n    FUN_1404bda80(param_1,param_2);\r\n    *(uint *)((longlong)param_1 + 0xc) = (uint)*(byte *)((longlong)param_1 + 0xc) | param_2 << 8;\r\n    return 1;\r\n  }\r\n  if (uVar3 == 0) {\r\n    if (uVar1 == 0) {\r\n      if (param_2 == 1 << ((byte)uVar4 & 0x1f)) {\r\n        FUN_1400998b0();\r\n      }\r\n    }\r\n    else {\r\n      FUN_1400998b0();\r\n    }\r\n    lVar2 = FUN_1404db460(param_1,param_2,0);\r\n    *param_1 = lVar2;\r\n    if (lVar2 != 0) {\r\n      *(uint *)((longlong)param_1 + 0xc) = *(uint *)((longlong)param_1 + 0xc) & 0xff;\r\n      *(uint *)((longlong)param_1 + 0xc) = *(uint *)((longlong)param_1 + 0xc) | param_2 << 8;\r\n      uVar4 = *(uint *)((longlong)param_1 + 0xc) >> 8;\r\n      if ((*(uint *)(param_1 + 1) & 0xffffff) <= uVar4) {\r\n        return 1;\r\n      }\r\n      *(uint *)(param_1 + 1) = *(uint *)(param_1 + 1) & 0xff000000 | uVar4;\r\n      return 1;\r\n    }\r\n  }\r\n  return 0;\r\n}\r\n\r\n"
          },
          "isError": false
        }
      }
    ]
  },
  "otherFunctions": {
    "addresses": [
      "1404dc1b0",
      "1404dc280",
      "1404dc380"
    ],
    "results": [
      {
        "status": "fulfilled",
        "value": {
          "content": [
            {
              "type": "text",
              "text": "\r\nvoid FUN_1404dc1b0(longlong param_1,longlong param_2)\r\n\r\n{\r\n  undefined4 uVar1;\r\n  undefined4 uVar2;\r\n  undefined4 uVar3;\r\n  undefined4 uVar4;\r\n  undefined4 uVar5;\r\n  longlong lVar6;\r\n  undefined4 *puVar7;\r\n  uint uStack_14;\r\n  \r\n  FUN_1404dc940();\r\n  lVar6 = FUN_1403da910(param_1 + 8,1);\r\n  if ((lVar6 != 0) && (*(longlong *)(lVar6 + 0x10) != 0)) {\r\n    uStack_14 = uStack_14 | 1;\r\n    uVar1 = *(undefined4 *)(*(longlong *)(lVar6 + 0x10) + 0xa8);\r\n    uVar2 = *(undefined4 *)(param_2 + 0xb4);\r\n    uVar3 = *(undefined4 *)(param_2 + 0xb0);\r\n    uVar4 = *(undefined4 *)(param_2 + 0xb8);\r\n    uVar5 = *(undefined4 *)(param_2 + 0xbc);\r\n    lVar6 = FUN_140810240();\r\n    puVar7 = (undefined4 *)FUN_1408112c0(lVar6 + 0xc8e8,0xc1,0x18,uVar1);\r\n    if (puVar7 != (undefined4 *)0x0) {\r\n      *puVar7 = 0;\r\n      puVar7[1] = uVar3;\r\n      puVar7[2] = uVar2;\r\n      puVar7[3] = uVar4;\r\n      puVar7[4] = uVar5;\r\n      puVar7[5] = uStack_14;\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
            }
          ],
          "structuredContent": {
            "result": "\r\nvoid FUN_1404dc1b0(longlong param_1,longlong param_2)\r\n\r\n{\r\n  undefined4 uVar1;\r\n  undefined4 uVar2;\r\n  undefined4 uVar3;\r\n  undefined4 uVar4;\r\n  undefined4 uVar5;\r\n  longlong lVar6;\r\n  undefined4 *puVar7;\r\n  uint uStack_14;\r\n  \r\n  FUN_1404dc940();\r\n  lVar6 = FUN_1403da910(param_1 + 8,1);\r\n  if ((lVar6 != 0) && (*(longlong *)(lVar6 + 0x10) != 0)) {\r\n    uStack_14 = uStack_14 | 1;\r\n    uVar1 = *(undefined4 *)(*(longlong *)(lVar6 + 0x10) + 0xa8);\r\n    uVar2 = *(undefined4 *)(param_2 + 0xb4);\r\n    uVar3 = *(undefined4 *)(param_2 + 0xb0);\r\n    uVar4 = *(undefined4 *)(param_2 + 0xb8);\r\n    uVar5 = *(undefined4 *)(param_2 + 0xbc);\r\n    lVar6 = FUN_140810240();\r\n    puVar7 = (undefined4 *)FUN_1408112c0(lVar6 + 0xc8e8,0xc1,0x18,uVar1);\r\n    if (puVar7 != (undefined4 *)0x0) {\r\n      *puVar7 = 0;\r\n      puVar7[1] = uVar3;\r\n      puVar7[2] = uVar2;\r\n      puVar7[3] = uVar4;\r\n      puVar7[4] = uVar5;\r\n      puVar7[5] = uStack_14;\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
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
              "text": "\r\nvoid FUN_1404dc280(longlong param_1,longlong param_2)\r\n\r\n{\r\n  longlong lVar1;\r\n  char cVar2;\r\n  longlong lVar3;\r\n  undefined8 uStackX_8;\r\n  undefined1 auStack_48 [16];\r\n  undefined1 auStack_38 [48];\r\n  \r\n  FUN_1404dc380(param_1,param_2 + 8);\r\n  lVar3 = FUN_1403da910(param_1 + 8,1);\r\n  if ((lVar3 != 0) && (lVar3 = *(longlong *)(lVar3 + 0x10), lVar3 != 0)) {\r\n    uStackX_8 = *(undefined8 *)(param_2 + 0xa8);\r\n    FUN_140879890(lVar3,&uStackX_8,*(undefined8 *)(lVar3 + 0xf0),param_1 + 0x130);\r\n    lVar1 = *(longlong *)(param_2 + 0xa0);\r\n    if ((lVar1 != 0) && (cVar2 = FUN_1404dbda0(param_1,auStack_48,auStack_38), cVar2 != '\\0')) {\r\n      FUN_1404e2de0(param_1 + 0x108,lVar1,lVar3,auStack_48,auStack_38,0,0,0,0,0,0,0,0);\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
            }
          ],
          "structuredContent": {
            "result": "\r\nvoid FUN_1404dc280(longlong param_1,longlong param_2)\r\n\r\n{\r\n  longlong lVar1;\r\n  char cVar2;\r\n  longlong lVar3;\r\n  undefined8 uStackX_8;\r\n  undefined1 auStack_48 [16];\r\n  undefined1 auStack_38 [48];\r\n  \r\n  FUN_1404dc380(param_1,param_2 + 8);\r\n  lVar3 = FUN_1403da910(param_1 + 8,1);\r\n  if ((lVar3 != 0) && (lVar3 = *(longlong *)(lVar3 + 0x10), lVar3 != 0)) {\r\n    uStackX_8 = *(undefined8 *)(param_2 + 0xa8);\r\n    FUN_140879890(lVar3,&uStackX_8,*(undefined8 *)(lVar3 + 0xf0),param_1 + 0x130);\r\n    lVar1 = *(longlong *)(param_2 + 0xa0);\r\n    if ((lVar1 != 0) && (cVar2 = FUN_1404dbda0(param_1,auStack_48,auStack_38), cVar2 != '\\0')) {\r\n      FUN_1404e2de0(param_1 + 0x108,lVar1,lVar3,auStack_48,auStack_38,0,0,0,0,0,0,0,0);\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
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
              "text": "\r\nvoid FUN_1404dc380(longlong param_1,undefined8 *param_2)\r\n\r\n{\r\n  float fVar1;\r\n  float fVar2;\r\n  float fVar3;\r\n  float fVar4;\r\n  float fVar5;\r\n  float fVar6;\r\n  float fVar7;\r\n  float fVar8;\r\n  float fVar9;\r\n  float fVar10;\r\n  float fVar11;\r\n  float fVar12;\r\n  float fVar13;\r\n  float fVar14;\r\n  longlong lVar15;\r\n  undefined8 uVar16;\r\n  undefined8 uStack_168;\r\n  undefined8 uStack_160;\r\n  undefined8 uStack_158;\r\n  undefined8 uStack_150;\r\n  undefined4 uStack_148;\r\n  float fStack_144;\r\n  float fStack_140;\r\n  float fStack_13c;\r\n  float fStack_138;\r\n  float fStack_134;\r\n  float fStack_130;\r\n  float fStack_12c;\r\n  float fStack_128;\r\n  float fStack_124;\r\n  undefined8 uStack_120;\r\n  undefined8 uStack_118;\r\n  undefined8 uStack_110;\r\n  undefined8 uStack_108;\r\n  undefined8 uStack_100;\r\n  undefined8 uStack_f8;\r\n  undefined8 uStack_f0;\r\n  undefined8 uStack_e8;\r\n  undefined8 uStack_e0;\r\n  undefined1 auStack_d8 [24];\r\n  undefined1 auStack_c0 [184];\r\n  \r\n  FUN_1404dc940();\r\n  lVar15 = FUN_1403da910(param_1 + 8,1);\r\n  if ((lVar15 != 0) && (lVar15 = *(longlong *)(lVar15 + 0x10), lVar15 != 0)) {\r\n    if ((*(uint *)(param_2 + 2) >> 0xb & 1) == 0) {\r\n      uStack_168 = *param_2;\r\n      uStack_160 = param_2[1];\r\n      uStack_158 = param_2[2];\r\n      uStack_150 = param_2[3];\r\n      uStack_148 = *(undefined4 *)(param_2 + 4);\r\n      fVar9 = *(float *)((longlong)param_2 + 0x24);\r\n      fVar10 = *(float *)(param_2 + 5);\r\n      uStack_120 = param_2[9];\r\n      uStack_108 = param_2[0xc];\r\n      uStack_100 = param_2[0xd];\r\n      fVar11 = *(float *)(param_2 + 6);\r\n      fVar12 = *(float *)((longlong)param_2 + 0x34);\r\n      fVar13 = *(float *)(param_2 + 7);\r\n      fVar14 = *(float *)((longlong)param_2 + 0x3c);\r\n      uStack_118 = param_2[10];\r\n      uStack_110 = param_2[0xb];\r\n      uStack_e8 = param_2[0x10];\r\n      uStack_e0 = param_2[0x11];\r\n      fVar1 = *(float *)((longlong)param_2 + 0x2c);\r\n      uStack_f8 = param_2[0xe];\r\n      uStack_f0 = param_2[0xf];\r\n      fVar2 = *(float *)(lVar15 + 0x318);\r\n      fVar3 = *(float *)(lVar15 + 0x324);\r\n      fVar4 = *(float *)(lVar15 + 0x31c);\r\n      fVar5 = *(float *)(lVar15 + 0x328);\r\n      fVar6 = *(float *)(lVar15 + 800);\r\n      fVar7 = *(float *)(lVar15 + 0x314);\r\n      fVar8 = *(float *)(lVar15 + 0x32c);\r\n      fStack_144 = *(float *)(lVar15 + 0x30c) * fVar9 + fVar2 * fVar10 + fVar3 * fVar1 +\r\n                   *(float *)(lVar15 + 0x300);\r\n      fStack_140 = *(float *)(lVar15 + 0x310) * fVar9 + fVar4 * fVar10 + fVar5 * fVar1 +\r\n                   *(float *)(lVar15 + 0x304);\r\n      fStack_13c = fVar7 * fVar9 + fVar6 * fVar10 + fVar8 * fVar1 + *(float *)(lVar15 + 0x308);\r\n      fStack_138 = *(float *)(lVar15 + 0x30c) * fVar11 + fVar2 * fVar12 + fVar3 * fVar13;\r\n      fStack_134 = *(float *)(lVar15 + 0x310) * fVar11 + fVar4 * fVar12 + fVar5 * fVar13;\r\n      fVar1 = *(float *)(param_2 + 8);\r\n      fVar9 = *(float *)((longlong)param_2 + 0x44);\r\n      fStack_130 = fVar7 * fVar11 + fVar6 * fVar12 + fVar8 * fVar13;\r\n      fStack_12c = *(float *)(lVar15 + 0x30c) * fVar14 + fVar2 * fVar1 + fVar3 * fVar9;\r\n      _fStack_128 = CONCAT44(fVar7 * fVar14 + fVar6 * fVar1 + fVar8 * fVar9,\r\n                             *(float *)(lVar15 + 0x310) * fVar14 + fVar4 * fVar1 + fVar5 * fVar9);\r\n      uVar16 = FUN_140882e20(auStack_c0,&uStack_168);\r\n      FUN_140884920(lVar15,auStack_d8,&DAT_1439a9d20,uVar16);\r\n      FUN_140882fc0(auStack_d8);\r\n    }\r\n    else {\r\n      FUN_1404dcad0(param_1,param_2);\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
            }
          ],
          "structuredContent": {
            "result": "\r\nvoid FUN_1404dc380(longlong param_1,undefined8 *param_2)\r\n\r\n{\r\n  float fVar1;\r\n  float fVar2;\r\n  float fVar3;\r\n  float fVar4;\r\n  float fVar5;\r\n  float fVar6;\r\n  float fVar7;\r\n  float fVar8;\r\n  float fVar9;\r\n  float fVar10;\r\n  float fVar11;\r\n  float fVar12;\r\n  float fVar13;\r\n  float fVar14;\r\n  longlong lVar15;\r\n  undefined8 uVar16;\r\n  undefined8 uStack_168;\r\n  undefined8 uStack_160;\r\n  undefined8 uStack_158;\r\n  undefined8 uStack_150;\r\n  undefined4 uStack_148;\r\n  float fStack_144;\r\n  float fStack_140;\r\n  float fStack_13c;\r\n  float fStack_138;\r\n  float fStack_134;\r\n  float fStack_130;\r\n  float fStack_12c;\r\n  float fStack_128;\r\n  float fStack_124;\r\n  undefined8 uStack_120;\r\n  undefined8 uStack_118;\r\n  undefined8 uStack_110;\r\n  undefined8 uStack_108;\r\n  undefined8 uStack_100;\r\n  undefined8 uStack_f8;\r\n  undefined8 uStack_f0;\r\n  undefined8 uStack_e8;\r\n  undefined8 uStack_e0;\r\n  undefined1 auStack_d8 [24];\r\n  undefined1 auStack_c0 [184];\r\n  \r\n  FUN_1404dc940();\r\n  lVar15 = FUN_1403da910(param_1 + 8,1);\r\n  if ((lVar15 != 0) && (lVar15 = *(longlong *)(lVar15 + 0x10), lVar15 != 0)) {\r\n    if ((*(uint *)(param_2 + 2) >> 0xb & 1) == 0) {\r\n      uStack_168 = *param_2;\r\n      uStack_160 = param_2[1];\r\n      uStack_158 = param_2[2];\r\n      uStack_150 = param_2[3];\r\n      uStack_148 = *(undefined4 *)(param_2 + 4);\r\n      fVar9 = *(float *)((longlong)param_2 + 0x24);\r\n      fVar10 = *(float *)(param_2 + 5);\r\n      uStack_120 = param_2[9];\r\n      uStack_108 = param_2[0xc];\r\n      uStack_100 = param_2[0xd];\r\n      fVar11 = *(float *)(param_2 + 6);\r\n      fVar12 = *(float *)((longlong)param_2 + 0x34);\r\n      fVar13 = *(float *)(param_2 + 7);\r\n      fVar14 = *(float *)((longlong)param_2 + 0x3c);\r\n      uStack_118 = param_2[10];\r\n      uStack_110 = param_2[0xb];\r\n      uStack_e8 = param_2[0x10];\r\n      uStack_e0 = param_2[0x11];\r\n      fVar1 = *(float *)((longlong)param_2 + 0x2c);\r\n      uStack_f8 = param_2[0xe];\r\n      uStack_f0 = param_2[0xf];\r\n      fVar2 = *(float *)(lVar15 + 0x318);\r\n      fVar3 = *(float *)(lVar15 + 0x324);\r\n      fVar4 = *(float *)(lVar15 + 0x31c);\r\n      fVar5 = *(float *)(lVar15 + 0x328);\r\n      fVar6 = *(float *)(lVar15 + 800);\r\n      fVar7 = *(float *)(lVar15 + 0x314);\r\n      fVar8 = *(float *)(lVar15 + 0x32c);\r\n      fStack_144 = *(float *)(lVar15 + 0x30c) * fVar9 + fVar2 * fVar10 + fVar3 * fVar1 +\r\n                   *(float *)(lVar15 + 0x300);\r\n      fStack_140 = *(float *)(lVar15 + 0x310) * fVar9 + fVar4 * fVar10 + fVar5 * fVar1 +\r\n                   *(float *)(lVar15 + 0x304);\r\n      fStack_13c = fVar7 * fVar9 + fVar6 * fVar10 + fVar8 * fVar1 + *(float *)(lVar15 + 0x308);\r\n      fStack_138 = *(float *)(lVar15 + 0x30c) * fVar11 + fVar2 * fVar12 + fVar3 * fVar13;\r\n      fStack_134 = *(float *)(lVar15 + 0x310) * fVar11 + fVar4 * fVar12 + fVar5 * fVar13;\r\n      fVar1 = *(float *)(param_2 + 8);\r\n      fVar9 = *(float *)((longlong)param_2 + 0x44);\r\n      fStack_130 = fVar7 * fVar11 + fVar6 * fVar12 + fVar8 * fVar13;\r\n      fStack_12c = *(float *)(lVar15 + 0x30c) * fVar14 + fVar2 * fVar1 + fVar3 * fVar9;\r\n      _fStack_128 = CONCAT44(fVar7 * fVar14 + fVar6 * fVar1 + fVar8 * fVar9,\r\n                             *(float *)(lVar15 + 0x310) * fVar14 + fVar4 * fVar1 + fVar5 * fVar9);\r\n      uVar16 = FUN_140882e20(auStack_c0,&uStack_168);\r\n      FUN_140884920(lVar15,auStack_d8,&DAT_1439a9d20,uVar16);\r\n      FUN_140882fc0(auStack_d8);\r\n    }\r\n    else {\r\n      FUN_1404dcad0(param_1,param_2);\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
          },
          "isError": false
        }
      }
    ]
  }
}

````

## Unescaped text fields for reading

### text

````text

void FUN_1404db8a0(undefined4 *param_1,undefined8 param_2)

{
  *param_1 = 3;
  *(int *)(*(longlong *)(param_1 + 2) + 0x84) = *(int *)(*(longlong *)(param_1 + 2) + 0x84) + -1;
  *(int *)(*(longlong *)(param_1 + 4) + 0x84) = *(int *)(*(longlong *)(param_1 + 4) + 0x84) + -1;
  FUN_1404bde00(param_2,param_1);
  *(undefined8 *)(param_1 + 2) = 0;
  *(undefined8 *)(param_1 + 4) = 0;
  return;
}


````

### text

````text

void FUN_1404dcc60(longlong param_1)

{
  undefined4 uVar1;
  undefined8 uVar2;
  byte bVar3;
  longlong lVar4;
  
  if (*(int *)(param_1 + 0x88) != 0) {
    bVar3 = FUN_1407d63f0(D2_GameSingletonPtr + 0x1f5338,*(undefined4 *)(param_1 + 0x88));
    if (bVar3 == 1) {
      lVar4 = FUN_1403da910(param_1 + 8,1);
      if ((lVar4 != 0) && (*(longlong *)(lVar4 + 0x10) != 0)) {
        uVar1 = *(undefined4 *)(*(longlong *)(lVar4 + 0x10) + 0xa8);
        uVar2 = FUN_140810240();
        FUN_1408103f0(uVar2,0xb6,uVar1);
      }
      *(undefined4 *)(param_1 + 0x88) = 0;
    }
    *(byte *)(param_1 + 0x8c) = *(byte *)(param_1 + 0x8c) & 0xfb;
    *(byte *)(param_1 + 0x8c) = *(byte *)(param_1 + 0x8c) | (bVar3 & 1) << 2;
  }
  return;
}


````

### text

````text

undefined8 FUN_1404dcd10(longlong *param_1,uint param_2)

{
  uint uVar1;
  
  if (-1 < (int)param_2) {
    uVar1 = *(uint *)(param_1 + 1);
    if ((int)param_2 < (int)(uVar1 & 0xffffff)) {
      uVar1 = (uVar1 - 1 ^ uVar1) & 0xffffff ^ uVar1;
      *(uint *)(param_1 + 1) = uVar1;
      if (param_2 != (uVar1 & 0xffffff)) {
        FUN_1404bc690((longlong)(int)param_2 * 0x130 + *param_1,
                      (ulonglong)(uVar1 & 0xffffff) * 0x130 + *param_1);
      }
      return 1;
    }
  }
  return 0;
}


````

### text

````text

undefined8 FUN_1404dcd80(longlong *param_1,uint param_2)

{
  uint uVar1;
  longlong lVar2;
  uint uVar3;
  uint uVar4;
  
  uVar4 = *(uint *)((longlong)param_1 + 0xc);
  if ((int)param_2 < 1) {
    if ((uVar4 & 0x40) == 0) {
      if (*param_1 != 0) {
        FUN_1404bcc70(*param_1,uVar4 >> 8);
      }
      *(uint *)((longlong)param_1 + 0xc) = *(uint *)((longlong)param_1 + 0xc) & 0xff;
      *param_1 = 0;
    }
    *(uint *)(param_1 + 1) = *(uint *)(param_1 + 1) & 0xff000000;
    return 1;
  }
  uVar1 = uVar4 >> 8;
  if (param_2 == uVar1) {
    return 1;
  }
  uVar3 = uVar4 >> 6 & 3;
  if ((uVar3 == 3) && ((int)uVar1 < (int)param_2)) {
    FUN_1404bda80(param_1,param_2);
    *(uint *)((longlong)param_1 + 0xc) = (uint)*(byte *)((longlong)param_1 + 0xc) | param_2 << 8;
    return 1;
  }
  if (uVar3 == 0) {
    if (uVar1 == 0) {
      if (param_2 == 1 << ((byte)uVar4 & 0x1f)) {
        FUN_1400998b0();
      }
    }
    else {
      FUN_1400998b0();
    }
    lVar2 = FUN_1404db460(param_1,param_2,0);
    *param_1 = lVar2;
    if (lVar2 != 0) {
      *(uint *)((longlong)param_1 + 0xc) = *(uint *)((longlong)param_1 + 0xc) & 0xff;
      *(uint *)((longlong)param_1 + 0xc) = *(uint *)((longlong)param_1 + 0xc) | param_2 << 8;
      uVar4 = *(uint *)((longlong)param_1 + 0xc) >> 8;
      if ((*(uint *)(param_1 + 1) & 0xffffff) <= uVar4) {
        return 1;
      }
      *(uint *)(param_1 + 1) = *(uint *)(param_1 + 1) & 0xff000000 | uVar4;
      return 1;
    }
  }
  return 0;
}


````

### text

````text

void FUN_1404dc1b0(longlong param_1,longlong param_2)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  longlong lVar6;
  undefined4 *puVar7;
  uint uStack_14;
  
  FUN_1404dc940();
  lVar6 = FUN_1403da910(param_1 + 8,1);
  if ((lVar6 != 0) && (*(longlong *)(lVar6 + 0x10) != 0)) {
    uStack_14 = uStack_14 | 1;
    uVar1 = *(undefined4 *)(*(longlong *)(lVar6 + 0x10) + 0xa8);
    uVar2 = *(undefined4 *)(param_2 + 0xb4);
    uVar3 = *(undefined4 *)(param_2 + 0xb0);
    uVar4 = *(undefined4 *)(param_2 + 0xb8);
    uVar5 = *(undefined4 *)(param_2 + 0xbc);
    lVar6 = FUN_140810240();
    puVar7 = (undefined4 *)FUN_1408112c0(lVar6 + 0xc8e8,0xc1,0x18,uVar1);
    if (puVar7 != (undefined4 *)0x0) {
      *puVar7 = 0;
      puVar7[1] = uVar3;
      puVar7[2] = uVar2;
      puVar7[3] = uVar4;
      puVar7[4] = uVar5;
      puVar7[5] = uStack_14;
    }
  }
  return;
}


````

### text

````text

void FUN_1404dc280(longlong param_1,longlong param_2)

{
  longlong lVar1;
  char cVar2;
  longlong lVar3;
  undefined8 uStackX_8;
  undefined1 auStack_48 [16];
  undefined1 auStack_38 [48];
  
  FUN_1404dc380(param_1,param_2 + 8);
  lVar3 = FUN_1403da910(param_1 + 8,1);
  if ((lVar3 != 0) && (lVar3 = *(longlong *)(lVar3 + 0x10), lVar3 != 0)) {
    uStackX_8 = *(undefined8 *)(param_2 + 0xa8);
    FUN_140879890(lVar3,&uStackX_8,*(undefined8 *)(lVar3 + 0xf0),param_1 + 0x130);
    lVar1 = *(longlong *)(param_2 + 0xa0);
    if ((lVar1 != 0) && (cVar2 = FUN_1404dbda0(param_1,auStack_48,auStack_38), cVar2 != '\0')) {
      FUN_1404e2de0(param_1 + 0x108,lVar1,lVar3,auStack_48,auStack_38,0,0,0,0,0,0,0,0);
    }
  }
  return;
}


````

### text

````text

void FUN_1404dc380(longlong param_1,undefined8 *param_2)

{
  float fVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  float fVar7;
  float fVar8;
  float fVar9;
  float fVar10;
  float fVar11;
  float fVar12;
  float fVar13;
  float fVar14;
  longlong lVar15;
  undefined8 uVar16;
  undefined8 uStack_168;
  undefined8 uStack_160;
  undefined8 uStack_158;
  undefined8 uStack_150;
  undefined4 uStack_148;
  float fStack_144;
  float fStack_140;
  float fStack_13c;
  float fStack_138;
  float fStack_134;
  float fStack_130;
  float fStack_12c;
  float fStack_128;
  float fStack_124;
  undefined8 uStack_120;
  undefined8 uStack_118;
  undefined8 uStack_110;
  undefined8 uStack_108;
  undefined8 uStack_100;
  undefined8 uStack_f8;
  undefined8 uStack_f0;
  undefined8 uStack_e8;
  undefined8 uStack_e0;
  undefined1 auStack_d8 [24];
  undefined1 auStack_c0 [184];
  
  FUN_1404dc940();
  lVar15 = FUN_1403da910(param_1 + 8,1);
  if ((lVar15 != 0) && (lVar15 = *(longlong *)(lVar15 + 0x10), lVar15 != 0)) {
    if ((*(uint *)(param_2 + 2) >> 0xb & 1) == 0) {
      uStack_168 = *param_2;
      uStack_160 = param_2[1];
      uStack_158 = param_2[2];
      uStack_150 = param_2[3];
      uStack_148 = *(undefined4 *)(param_2 + 4);
      fVar9 = *(float *)((longlong)param_2 + 0x24);
      fVar10 = *(float *)(param_2 + 5);
      uStack_120 = param_2[9];
      uStack_108 = param_2[0xc];
      uStack_100 = param_2[0xd];
      fVar11 = *(float *)(param_2 + 6);
      fVar12 = *(float *)((longlong)param_2 + 0x34);
      fVar13 = *(float *)(param_2 + 7);
      fVar14 = *(float *)((longlong)param_2 + 0x3c);
      uStack_118 = param_2[10];
      uStack_110 = param_2[0xb];
      uStack_e8 = param_2[0x10];
      uStack_e0 = param_2[0x11];
      fVar1 = *(float *)((longlong)param_2 + 0x2c);
      uStack_f8 = param_2[0xe];
      uStack_f0 = param_2[0xf];
      fVar2 = *(float *)(lVar15 + 0x318);
      fVar3 = *(float *)(lVar15 + 0x324);
      fVar4 = *(float *)(lVar15 + 0x31c);
      fVar5 = *(float *)(lVar15 + 0x328);
      fVar6 = *(float *)(lVar15 + 800);
      fVar7 = *(float *)(lVar15 + 0x314);
      fVar8 = *(float *)(lVar15 + 0x32c);
      fStack_144 = *(float *)(lVar15 + 0x30c) * fVar9 + fVar2 * fVar10 + fVar3 * fVar1 +
                   *(float *)(lVar15 + 0x300);
      fStack_140 = *(float *)(lVar15 + 0x310) * fVar9 + fVar4 * fVar10 + fVar5 * fVar1 +
                   *(float *)(lVar15 + 0x304);
      fStack_13c = fVar7 * fVar9 + fVar6 * fVar10 + fVar8 * fVar1 + *(float *)(lVar15 + 0x308);
      fStack_138 = *(float *)(lVar15 + 0x30c) * fVar11 + fVar2 * fVar12 + fVar3 * fVar13;
      fStack_134 = *(float *)(lVar15 + 0x310) * fVar11 + fVar4 * fVar12 + fVar5 * fVar13;
      fVar1 = *(float *)(param_2 + 8);
      fVar9 = *(float *)((longlong)param_2 + 0x44);
      fStack_130 = fVar7 * fVar11 + fVar6 * fVar12 + fVar8 * fVar13;
      fStack_12c = *(float *)(lVar15 + 0x30c) * fVar14 + fVar2 * fVar1 + fVar3 * fVar9;
      _fStack_128 = CONCAT44(fVar7 * fVar14 + fVar6 * fVar1 + fVar8 * fVar9,
                             *(float *)(lVar15 + 0x310) * fVar14 + fVar4 * fVar1 + fVar5 * fVar9);
      uVar16 = FUN_140882e20(auStack_c0,&uStack_168);
      FUN_140884920(lVar15,auStack_d8,&DAT_1439a9d20,uVar16);
      FUN_140882fc0(auStack_d8);
    }
    else {
      FUN_1404dcad0(param_1,param_2);
    }
  }
  return;
}


````
