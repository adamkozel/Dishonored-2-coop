# ghidra-dominoConnectionFxOwners-v220.json

Original: [work/ghidra-dominoConnectionFxOwners-v220.json](<../../../../work/ghidra-dominoConnectionFxOwners-v220.json>)

SHA-256: `F48F73D68786A9ACC9E22E9F08BFE48E445E602BF667839128924C5C55A5F0EF`; 9573 bytes.

## Original saved record

````json
{
  "addresses": [
    "1404db720",
    "1404db7d0",
    "1404db8f0",
    "1404db9e0"
  ],
  "results": [
    {
      "status": "fulfilled",
      "value": {
        "content": [
          {
            "type": "text",
            "text": "\r\nundefined8 FUN_1404db720(uint *param_1)\r\n\r\n{\r\n  int iVar1;\r\n  longlong lVar2;\r\n  longlong lVar3;\r\n  \r\n  if (1 < *param_1) {\r\n    return 0;\r\n  }\r\n  lVar2 = *(longlong *)(param_1 + 2);\r\n  lVar3 = FUN_1403da910(lVar2 + 0x68,1);\r\n  if (((lVar3 == 0) || (iVar1 = *(int *)(lVar2 + 0x78), iVar1 < 0)) ||\r\n     (*(float *)(*(longlong *)(lVar3 + 0x48) + 0x24 + (longlong)iVar1 * 0x28) ==\r\n      *(float *)(*(longlong *)(lVar3 + 0x48) + 8 + (longlong)iVar1 * 0x28))) {\r\n    lVar2 = *(longlong *)(param_1 + 4);\r\n    lVar3 = FUN_1403da910(lVar2 + 0x68,1);\r\n    if (((lVar3 == 0) || (iVar1 = *(int *)(lVar2 + 0x78), iVar1 < 0)) ||\r\n       (*(float *)(*(longlong *)(lVar3 + 0x48) + 0x24 + (longlong)iVar1 * 0x28) ==\r\n        *(float *)(*(longlong *)(lVar3 + 0x48) + 8 + (longlong)iVar1 * 0x28))) {\r\n      return 1;\r\n    }\r\n  }\r\n  return 0;\r\n}\r\n\r\n"
          }
        ],
        "structuredContent": {
          "result": "\r\nundefined8 FUN_1404db720(uint *param_1)\r\n\r\n{\r\n  int iVar1;\r\n  longlong lVar2;\r\n  longlong lVar3;\r\n  \r\n  if (1 < *param_1) {\r\n    return 0;\r\n  }\r\n  lVar2 = *(longlong *)(param_1 + 2);\r\n  lVar3 = FUN_1403da910(lVar2 + 0x68,1);\r\n  if (((lVar3 == 0) || (iVar1 = *(int *)(lVar2 + 0x78), iVar1 < 0)) ||\r\n     (*(float *)(*(longlong *)(lVar3 + 0x48) + 0x24 + (longlong)iVar1 * 0x28) ==\r\n      *(float *)(*(longlong *)(lVar3 + 0x48) + 8 + (longlong)iVar1 * 0x28))) {\r\n    lVar2 = *(longlong *)(param_1 + 4);\r\n    lVar3 = FUN_1403da910(lVar2 + 0x68,1);\r\n    if (((lVar3 == 0) || (iVar1 = *(int *)(lVar2 + 0x78), iVar1 < 0)) ||\r\n       (*(float *)(*(longlong *)(lVar3 + 0x48) + 0x24 + (longlong)iVar1 * 0x28) ==\r\n        *(float *)(*(longlong *)(lVar3 + 0x48) + 8 + (longlong)iVar1 * 0x28))) {\r\n      return 1;\r\n    }\r\n  }\r\n  return 0;\r\n}\r\n\r\n"
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
            "text": "\r\nvoid FUN_1404db7d0(uint *param_1)\r\n\r\n{\r\n  char cVar1;\r\n  int iVar2;\r\n  \r\n  if (*param_1 < 2) {\r\n    *param_1 = 2;\r\n    cVar1 = FUN_1404e2710(param_1 + 0xb);\r\n    if (cVar1 != '\\0') {\r\n      FUN_1404e3050(param_1 + 0xb);\r\n    }\r\n    cVar1 = FUN_1404e2710(param_1 + 0x15);\r\n    if (cVar1 != '\\0') {\r\n      FUN_1404e3050(param_1 + 0x15);\r\n    }\r\n    iVar2 = 0;\r\n    if ((param_1[8] & 0xffffff) != 0) {\r\n      do {\r\n        FUN_1404e3050((longlong)iVar2 * 0x130 + *(longlong *)(param_1 + 6) + 0x108);\r\n        iVar2 = iVar2 + 1;\r\n      } while (iVar2 < (int)(param_1[8] & 0xffffff));\r\n    }\r\n    if ((param_1[9] & 0x40) == 0) {\r\n      if (*(longlong *)(param_1 + 6) != 0) {\r\n        FUN_1404bcc70(*(longlong *)(param_1 + 6),param_1[9] >> 8);\r\n      }\r\n      param_1[9] = param_1[9] & 0xff;\r\n      param_1[6] = 0;\r\n      param_1[7] = 0;\r\n    }\r\n    param_1[8] = param_1[8] & 0xff000000;\r\n  }\r\n  return;\r\n}\r\n\r\n"
          }
        ],
        "structuredContent": {
          "result": "\r\nvoid FUN_1404db7d0(uint *param_1)\r\n\r\n{\r\n  char cVar1;\r\n  int iVar2;\r\n  \r\n  if (*param_1 < 2) {\r\n    *param_1 = 2;\r\n    cVar1 = FUN_1404e2710(param_1 + 0xb);\r\n    if (cVar1 != '\\0') {\r\n      FUN_1404e3050(param_1 + 0xb);\r\n    }\r\n    cVar1 = FUN_1404e2710(param_1 + 0x15);\r\n    if (cVar1 != '\\0') {\r\n      FUN_1404e3050(param_1 + 0x15);\r\n    }\r\n    iVar2 = 0;\r\n    if ((param_1[8] & 0xffffff) != 0) {\r\n      do {\r\n        FUN_1404e3050((longlong)iVar2 * 0x130 + *(longlong *)(param_1 + 6) + 0x108);\r\n        iVar2 = iVar2 + 1;\r\n      } while (iVar2 < (int)(param_1[8] & 0xffffff));\r\n    }\r\n    if ((param_1[9] & 0x40) == 0) {\r\n      if (*(longlong *)(param_1 + 6) != 0) {\r\n        FUN_1404bcc70(*(longlong *)(param_1 + 6),param_1[9] >> 8);\r\n      }\r\n      param_1[9] = param_1[9] & 0xff;\r\n      param_1[6] = 0;\r\n      param_1[7] = 0;\r\n    }\r\n    param_1[8] = param_1[8] & 0xff000000;\r\n  }\r\n  return;\r\n}\r\n\r\n"
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
            "text": "\r\nvoid FUN_1404db8f0(longlong param_1,undefined4 *param_2)\r\n\r\n{\r\n  longlong lVar1;\r\n  longlong lVar2;\r\n  undefined4 *puVar3;\r\n  float fVar4;\r\n  float local_68;\r\n  float local_64;\r\n  float local_60;\r\n  undefined1 local_58 [80];\r\n  \r\n  lVar1 = *(longlong *)(param_1 + 8);\r\n  lVar2 = *(longlong *)(param_1 + 0x10);\r\n  local_68 = *(float *)(lVar2 + 0x38) - *(float *)(lVar1 + 0x38);\r\n  local_64 = *(float *)(lVar2 + 0x3c) - *(float *)(lVar1 + 0x3c);\r\n  local_60 = *(float *)(lVar2 + 0x40) - *(float *)(lVar1 + 0x40);\r\n  fVar4 = local_64 * local_64 + local_68 * local_68 + local_60 * local_60;\r\n  if (fVar4 <= 1.1754944e-38) {\r\n    fVar4 = 1.1754944e-38;\r\n  }\r\n  fVar4 = 1.0 / SQRT(fVar4);\r\n  local_68 = local_68 * fVar4;\r\n  local_64 = local_64 * fVar4;\r\n  local_60 = local_60 * fVar4;\r\n  puVar3 = (undefined4 *)FUN_140112000(&local_68,local_58);\r\n  *param_2 = *puVar3;\r\n  param_2[1] = puVar3[1];\r\n  param_2[2] = puVar3[2];\r\n  param_2[3] = puVar3[3];\r\n  param_2[4] = puVar3[4];\r\n  param_2[5] = puVar3[5];\r\n  param_2[6] = puVar3[6];\r\n  param_2[7] = puVar3[7];\r\n  param_2[8] = puVar3[8];\r\n  return;\r\n}\r\n\r\n"
          }
        ],
        "structuredContent": {
          "result": "\r\nvoid FUN_1404db8f0(longlong param_1,undefined4 *param_2)\r\n\r\n{\r\n  longlong lVar1;\r\n  longlong lVar2;\r\n  undefined4 *puVar3;\r\n  float fVar4;\r\n  float local_68;\r\n  float local_64;\r\n  float local_60;\r\n  undefined1 local_58 [80];\r\n  \r\n  lVar1 = *(longlong *)(param_1 + 8);\r\n  lVar2 = *(longlong *)(param_1 + 0x10);\r\n  local_68 = *(float *)(lVar2 + 0x38) - *(float *)(lVar1 + 0x38);\r\n  local_64 = *(float *)(lVar2 + 0x3c) - *(float *)(lVar1 + 0x3c);\r\n  local_60 = *(float *)(lVar2 + 0x40) - *(float *)(lVar1 + 0x40);\r\n  fVar4 = local_64 * local_64 + local_68 * local_68 + local_60 * local_60;\r\n  if (fVar4 <= 1.1754944e-38) {\r\n    fVar4 = 1.1754944e-38;\r\n  }\r\n  fVar4 = 1.0 / SQRT(fVar4);\r\n  local_68 = local_68 * fVar4;\r\n  local_64 = local_64 * fVar4;\r\n  local_60 = local_60 * fVar4;\r\n  puVar3 = (undefined4 *)FUN_140112000(&local_68,local_58);\r\n  *param_2 = *puVar3;\r\n  param_2[1] = puVar3[1];\r\n  param_2[2] = puVar3[2];\r\n  param_2[3] = puVar3[3];\r\n  param_2[4] = puVar3[4];\r\n  param_2[5] = puVar3[5];\r\n  param_2[6] = puVar3[6];\r\n  param_2[7] = puVar3[7];\r\n  param_2[8] = puVar3[8];\r\n  return;\r\n}\r\n\r\n"
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
            "text": "\r\nfloat * FUN_1404db9e0(longlong param_1,float *param_2)\r\n\r\n{\r\n  float fVar1;\r\n  longlong lVar2;\r\n  float fVar3;\r\n  float fVar4;\r\n  float fVar5;\r\n  float fVar6;\r\n  float fVar7;\r\n  float fVar8;\r\n  float fVar9;\r\n  \r\n  lVar2 = *(longlong *)(param_1 + 0x10);\r\n  fVar6 = *(float *)(lVar2 + 0x38);\r\n  fVar4 = *(float *)(lVar2 + 0x3c);\r\n  fVar5 = *(float *)(lVar2 + 0x40);\r\n  lVar2 = *(longlong *)(param_1 + 8);\r\n  fVar8 = *(float *)(lVar2 + 0x38);\r\n  fVar9 = *(float *)(lVar2 + 0x3c);\r\n  fVar7 = *(float *)(lVar2 + 0x40);\r\n  fVar1 = *(float *)(param_1 + 0x28);\r\n  if (ABS(fVar6) <= 1e-18) {\r\n    fVar6 = 0.0;\r\n  }\r\n  if (ABS(fVar4) <= 1e-18) {\r\n    fVar4 = 0.0;\r\n  }\r\n  if (ABS(fVar5) <= 1e-18) {\r\n    fVar5 = 0.0;\r\n  }\r\n  if (ABS(fVar8) <= 1e-18) {\r\n    fVar8 = 0.0;\r\n  }\r\n  if (ABS(fVar9) <= 1e-18) {\r\n    fVar9 = 0.0;\r\n  }\r\n  if (ABS(fVar7) <= 1e-18) {\r\n    fVar7 = 0.0;\r\n  }\r\n  fVar3 = 1.0 - fVar1;\r\n  param_2[2] = fVar3 * fVar7 + fVar5 * fVar1;\r\n  *param_2 = fVar3 * fVar8 + fVar6 * fVar1;\r\n  param_2[1] = fVar3 * fVar9 + fVar4 * fVar1;\r\n  return param_2;\r\n}\r\n\r\n"
          }
        ],
        "structuredContent": {
          "result": "\r\nfloat * FUN_1404db9e0(longlong param_1,float *param_2)\r\n\r\n{\r\n  float fVar1;\r\n  longlong lVar2;\r\n  float fVar3;\r\n  float fVar4;\r\n  float fVar5;\r\n  float fVar6;\r\n  float fVar7;\r\n  float fVar8;\r\n  float fVar9;\r\n  \r\n  lVar2 = *(longlong *)(param_1 + 0x10);\r\n  fVar6 = *(float *)(lVar2 + 0x38);\r\n  fVar4 = *(float *)(lVar2 + 0x3c);\r\n  fVar5 = *(float *)(lVar2 + 0x40);\r\n  lVar2 = *(longlong *)(param_1 + 8);\r\n  fVar8 = *(float *)(lVar2 + 0x38);\r\n  fVar9 = *(float *)(lVar2 + 0x3c);\r\n  fVar7 = *(float *)(lVar2 + 0x40);\r\n  fVar1 = *(float *)(param_1 + 0x28);\r\n  if (ABS(fVar6) <= 1e-18) {\r\n    fVar6 = 0.0;\r\n  }\r\n  if (ABS(fVar4) <= 1e-18) {\r\n    fVar4 = 0.0;\r\n  }\r\n  if (ABS(fVar5) <= 1e-18) {\r\n    fVar5 = 0.0;\r\n  }\r\n  if (ABS(fVar8) <= 1e-18) {\r\n    fVar8 = 0.0;\r\n  }\r\n  if (ABS(fVar9) <= 1e-18) {\r\n    fVar9 = 0.0;\r\n  }\r\n  if (ABS(fVar7) <= 1e-18) {\r\n    fVar7 = 0.0;\r\n  }\r\n  fVar3 = 1.0 - fVar1;\r\n  param_2[2] = fVar3 * fVar7 + fVar5 * fVar1;\r\n  *param_2 = fVar3 * fVar8 + fVar6 * fVar1;\r\n  param_2[1] = fVar3 * fVar9 + fVar4 * fVar1;\r\n  return param_2;\r\n}\r\n\r\n"
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

undefined8 FUN_1404db720(uint *param_1)

{
  int iVar1;
  longlong lVar2;
  longlong lVar3;
  
  if (1 < *param_1) {
    return 0;
  }
  lVar2 = *(longlong *)(param_1 + 2);
  lVar3 = FUN_1403da910(lVar2 + 0x68,1);
  if (((lVar3 == 0) || (iVar1 = *(int *)(lVar2 + 0x78), iVar1 < 0)) ||
     (*(float *)(*(longlong *)(lVar3 + 0x48) + 0x24 + (longlong)iVar1 * 0x28) ==
      *(float *)(*(longlong *)(lVar3 + 0x48) + 8 + (longlong)iVar1 * 0x28))) {
    lVar2 = *(longlong *)(param_1 + 4);
    lVar3 = FUN_1403da910(lVar2 + 0x68,1);
    if (((lVar3 == 0) || (iVar1 = *(int *)(lVar2 + 0x78), iVar1 < 0)) ||
       (*(float *)(*(longlong *)(lVar3 + 0x48) + 0x24 + (longlong)iVar1 * 0x28) ==
        *(float *)(*(longlong *)(lVar3 + 0x48) + 8 + (longlong)iVar1 * 0x28))) {
      return 1;
    }
  }
  return 0;
}


````

### text

````text

void FUN_1404db7d0(uint *param_1)

{
  char cVar1;
  int iVar2;
  
  if (*param_1 < 2) {
    *param_1 = 2;
    cVar1 = FUN_1404e2710(param_1 + 0xb);
    if (cVar1 != '\0') {
      FUN_1404e3050(param_1 + 0xb);
    }
    cVar1 = FUN_1404e2710(param_1 + 0x15);
    if (cVar1 != '\0') {
      FUN_1404e3050(param_1 + 0x15);
    }
    iVar2 = 0;
    if ((param_1[8] & 0xffffff) != 0) {
      do {
        FUN_1404e3050((longlong)iVar2 * 0x130 + *(longlong *)(param_1 + 6) + 0x108);
        iVar2 = iVar2 + 1;
      } while (iVar2 < (int)(param_1[8] & 0xffffff));
    }
    if ((param_1[9] & 0x40) == 0) {
      if (*(longlong *)(param_1 + 6) != 0) {
        FUN_1404bcc70(*(longlong *)(param_1 + 6),param_1[9] >> 8);
      }
      param_1[9] = param_1[9] & 0xff;
      param_1[6] = 0;
      param_1[7] = 0;
    }
    param_1[8] = param_1[8] & 0xff000000;
  }
  return;
}


````

### text

````text

void FUN_1404db8f0(longlong param_1,undefined4 *param_2)

{
  longlong lVar1;
  longlong lVar2;
  undefined4 *puVar3;
  float fVar4;
  float local_68;
  float local_64;
  float local_60;
  undefined1 local_58 [80];
  
  lVar1 = *(longlong *)(param_1 + 8);
  lVar2 = *(longlong *)(param_1 + 0x10);
  local_68 = *(float *)(lVar2 + 0x38) - *(float *)(lVar1 + 0x38);
  local_64 = *(float *)(lVar2 + 0x3c) - *(float *)(lVar1 + 0x3c);
  local_60 = *(float *)(lVar2 + 0x40) - *(float *)(lVar1 + 0x40);
  fVar4 = local_64 * local_64 + local_68 * local_68 + local_60 * local_60;
  if (fVar4 <= 1.1754944e-38) {
    fVar4 = 1.1754944e-38;
  }
  fVar4 = 1.0 / SQRT(fVar4);
  local_68 = local_68 * fVar4;
  local_64 = local_64 * fVar4;
  local_60 = local_60 * fVar4;
  puVar3 = (undefined4 *)FUN_140112000(&local_68,local_58);
  *param_2 = *puVar3;
  param_2[1] = puVar3[1];
  param_2[2] = puVar3[2];
  param_2[3] = puVar3[3];
  param_2[4] = puVar3[4];
  param_2[5] = puVar3[5];
  param_2[6] = puVar3[6];
  param_2[7] = puVar3[7];
  param_2[8] = puVar3[8];
  return;
}


````

### text

````text

float * FUN_1404db9e0(longlong param_1,float *param_2)

{
  float fVar1;
  longlong lVar2;
  float fVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  float fVar7;
  float fVar8;
  float fVar9;
  
  lVar2 = *(longlong *)(param_1 + 0x10);
  fVar6 = *(float *)(lVar2 + 0x38);
  fVar4 = *(float *)(lVar2 + 0x3c);
  fVar5 = *(float *)(lVar2 + 0x40);
  lVar2 = *(longlong *)(param_1 + 8);
  fVar8 = *(float *)(lVar2 + 0x38);
  fVar9 = *(float *)(lVar2 + 0x3c);
  fVar7 = *(float *)(lVar2 + 0x40);
  fVar1 = *(float *)(param_1 + 0x28);
  if (ABS(fVar6) <= 1e-18) {
    fVar6 = 0.0;
  }
  if (ABS(fVar4) <= 1e-18) {
    fVar4 = 0.0;
  }
  if (ABS(fVar5) <= 1e-18) {
    fVar5 = 0.0;
  }
  if (ABS(fVar8) <= 1e-18) {
    fVar8 = 0.0;
  }
  if (ABS(fVar9) <= 1e-18) {
    fVar9 = 0.0;
  }
  if (ABS(fVar7) <= 1e-18) {
    fVar7 = 0.0;
  }
  fVar3 = 1.0 - fVar1;
  param_2[2] = fVar3 * fVar7 + fVar5 * fVar1;
  *param_2 = fVar3 * fVar8 + fVar6 * fVar1;
  param_2[1] = fVar3 * fVar9 + fVar4 * fVar1;
  return param_2;
}


````
