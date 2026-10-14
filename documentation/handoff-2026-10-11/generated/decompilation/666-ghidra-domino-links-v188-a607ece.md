# ghidra-domino-links-v188.json

Original: [work/ghidra-domino-links-v188.json](<../../../../work/ghidra-domino-links-v188.json>)

SHA-256: `85627B6E1B7636AD6B71984013FDD4C57BDED48567E0D040C2227239472F3379`; 12744 bytes.

## Original saved record

````json
{
  "program": "Dishonored2.exe",
  "addresses": [
    "140447460",
    "140447520",
    "1404bd680",
    "1404bc950",
    "1404be8b0"
  ],
  "results": [
    {
      "status": "fulfilled",
      "value": {
        "content": [
          {
            "type": "text",
            "text": "\r\nbool FUN_140447460(longlong param_1)\r\n\r\n{\r\n  int iVar1;\r\n  longlong lVar2;\r\n  undefined8 uVar3;\r\n  float fVar4;\r\n  float fVar5;\r\n  \r\n  lVar2 = FUN_1406dc560(*(undefined4 *)(param_1 + 200));\r\n  if (lVar2 == 0) {\r\n    return false;\r\n  }\r\n  uVar3 = FUN_1406dc560(*(undefined4 *)(param_1 + 200));\r\n  lVar2 = FUN_1404bd680(*(longlong *)(param_1 + 0xb8),uVar3,*(longlong *)(param_1 + 0xb8) + 0x28);\r\n  if (lVar2 == 0) {\r\n    iVar1 = FUN_1404bd810(*(undefined8 *)(param_1 + 0xb8));\r\n    if (iVar1 != 0) {\r\n      fVar4 = (float)FUN_14061c060(param_1);\r\n      uVar3 = FUN_1406dc560(*(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0xac));\r\n      lVar2 = FUN_140877720(uVar3,&DAT_1439d13b0,2);\r\n      if (lVar2 != 0) {\r\n        fVar5 = (float)FUN_140745f60(lVar2);\r\n        return fVar4 <= fVar5;\r\n      }\r\n      return false;\r\n    }\r\n  }\r\n  return true;\r\n}\r\n\r\n"
          }
        ],
        "structuredContent": {
          "result": "\r\nbool FUN_140447460(longlong param_1)\r\n\r\n{\r\n  int iVar1;\r\n  longlong lVar2;\r\n  undefined8 uVar3;\r\n  float fVar4;\r\n  float fVar5;\r\n  \r\n  lVar2 = FUN_1406dc560(*(undefined4 *)(param_1 + 200));\r\n  if (lVar2 == 0) {\r\n    return false;\r\n  }\r\n  uVar3 = FUN_1406dc560(*(undefined4 *)(param_1 + 200));\r\n  lVar2 = FUN_1404bd680(*(longlong *)(param_1 + 0xb8),uVar3,*(longlong *)(param_1 + 0xb8) + 0x28);\r\n  if (lVar2 == 0) {\r\n    iVar1 = FUN_1404bd810(*(undefined8 *)(param_1 + 0xb8));\r\n    if (iVar1 != 0) {\r\n      fVar4 = (float)FUN_14061c060(param_1);\r\n      uVar3 = FUN_1406dc560(*(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0xac));\r\n      lVar2 = FUN_140877720(uVar3,&DAT_1439d13b0,2);\r\n      if (lVar2 != 0) {\r\n        fVar5 = (float)FUN_140745f60(lVar2);\r\n        return fVar4 <= fVar5;\r\n      }\r\n      return false;\r\n    }\r\n  }\r\n  return true;\r\n}\r\n\r\n"
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
            "text": "\r\nundefined8 FUN_1404bd680(undefined8 param_1,longlong param_2,longlong *param_3)\r\n\r\n{\r\n  undefined8 *puVar1;\r\n  undefined8 uVar2;\r\n  longlong lVar3;\r\n  undefined8 *puVar4;\r\n  \r\n  puVar4 = (undefined8 *)*param_3;\r\n  puVar1 = puVar4 + (*(uint *)(param_3 + 1) & 0xffffff);\r\n  while( true ) {\r\n    if (puVar4 == puVar1) {\r\n      return 0;\r\n    }\r\n    uVar2 = *puVar4;\r\n    lVar3 = FUN_1404db590(uVar2);\r\n    if (lVar3 == param_2) break;\r\n    puVar4 = puVar4 + 1;\r\n  }\r\n  return uVar2;\r\n}\r\n\r\n"
          }
        ],
        "structuredContent": {
          "result": "\r\nundefined8 FUN_1404bd680(undefined8 param_1,longlong param_2,longlong *param_3)\r\n\r\n{\r\n  undefined8 *puVar1;\r\n  undefined8 uVar2;\r\n  longlong lVar3;\r\n  undefined8 *puVar4;\r\n  \r\n  puVar4 = (undefined8 *)*param_3;\r\n  puVar1 = puVar4 + (*(uint *)(param_3 + 1) & 0xffffff);\r\n  while( true ) {\r\n    if (puVar4 == puVar1) {\r\n      return 0;\r\n    }\r\n    uVar2 = *puVar4;\r\n    lVar3 = FUN_1404db590(uVar2);\r\n    if (lVar3 == param_2) break;\r\n    puVar4 = puVar4 + 1;\r\n  }\r\n  return uVar2;\r\n}\r\n\r\n"
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
            "text": "\r\nvoid FUN_1404bc950(longlong param_1,longlong param_2)\r\n\r\n{\r\n  longlong *plVar1;\r\n  undefined8 *puVar2;\r\n  ulonglong uVar3;\r\n  ushort uVar4;\r\n  longlong *plVar5;\r\n  longlong lVar6;\r\n  uint uVar7;\r\n  undefined8 *puVar8;\r\n  longlong lVar9;\r\n  float fVar10;\r\n  float fVar11;\r\n  undefined8 local_res8;\r\n  \r\n  fVar11 = 0.0;\r\n  if (*(int *)(param_1 + 0x2640) < 0) {\r\n    fVar10 = 0.0;\r\n  }\r\n  else {\r\n    fVar10 = *(float *)(*(longlong *)(*(longlong *)(param_1 + 0x2628) + 0x48) + 0x24 +\r\n                       (longlong)*(int *)(param_1 + 0x2640) * 0x28);\r\n  }\r\n  uVar7 = (int)(longlong)fVar10 + 2;\r\n  if (4 < uVar7) {\r\n    uVar7 = 4;\r\n  }\r\n  if (uVar7 == (*(uint *)(param_1 + 0x30) & 0xffffff)) {\r\n    plVar5 = *(longlong **)(param_1 + 0x28);\r\n    lVar9 = 0;\r\n    plVar1 = plVar5 + (*(uint *)(param_1 + 0x30) & 0xffffff);\r\n    for (; plVar5 != plVar1; plVar5 = plVar5 + 1) {\r\n      fVar10 = *(float *)(*plVar5 + 0x7c);\r\n      uVar3 = (ulonglong)local_res8 >> 0x20;\r\n      if ((lVar9 == 0) || (fVar11 < fVar10)) {\r\n        lVar9 = *plVar5;\r\n        fVar11 = fVar10;\r\n      }\r\n      local_res8 = CONCAT44((int)uVar3,fVar10);\r\n    }\r\n    FUN_1404bd510(param_1,lVar9,CONCAT71((int7)((ulonglong)plVar1 >> 8),1));\r\n  }\r\n  plVar5 = *(longlong **)(param_1 + 0x58);\r\n  plVar1 = plVar5 + (*(uint *)(param_1 + 0x60) & 0xffffff);\r\n  for (; plVar5 != plVar1; plVar5 = plVar5 + 1) {\r\n    lVar9 = *plVar5;\r\n    lVar6 = FUN_1404db590(lVar9);\r\n    if (lVar6 == param_2) goto LAB_1404bca3e;\r\n  }\r\n  lVar9 = 0;\r\nLAB_1404bca3e:\r\n  local_res8 = lVar9;\r\n  if (lVar9 == 0) {\r\n    uVar4 = FUN_1404bcbc0(param_1 + 0x68,0xffff);\r\n    if (uVar4 != 0xffff) {\r\n      lVar9 = (ulonglong)uVar4 * 0x150 + *(longlong *)(param_1 + 0x68);\r\n    }\r\n    FUN_1404db5c0(lVar9,param_2,*(longlong *)(param_1 + 8) + 0x80);\r\n  }\r\n  else {\r\n    FUN_1407b8270(param_1 + 0x58,&local_res8);\r\n  }\r\n  FUN_1404bc8d0(param_1,lVar9);\r\n  FUN_1404bd2e0(param_1,lVar9);\r\n  *(longlong *)(param_1 + 0x20) = lVar9;\r\n  puVar8 = *(undefined8 **)(param_1 + 0x15b0);\r\n  puVar2 = puVar8 + (*(uint *)(param_1 + 0x15b8) & 0xffffff);\r\n  for (; puVar8 != puVar2; puVar8 = puVar8 + 1) {\r\n    if (*(uint *)*puVar8 < 2) {\r\n      if (*(longlong *)((uint *)*puVar8 + 2) == *(longlong *)(param_1 + 0x20)) {\r\n        FUN_1404dd500();\r\n      }\r\n      else {\r\n        FUN_1404dd910();\r\n      }\r\n    }\r\n  }\r\n  FUN_1404bd900(param_1,lVar9);\r\n  return;\r\n}\r\n\r\n"
          }
        ],
        "structuredContent": {
          "result": "\r\nvoid FUN_1404bc950(longlong param_1,longlong param_2)\r\n\r\n{\r\n  longlong *plVar1;\r\n  undefined8 *puVar2;\r\n  ulonglong uVar3;\r\n  ushort uVar4;\r\n  longlong *plVar5;\r\n  longlong lVar6;\r\n  uint uVar7;\r\n  undefined8 *puVar8;\r\n  longlong lVar9;\r\n  float fVar10;\r\n  float fVar11;\r\n  undefined8 local_res8;\r\n  \r\n  fVar11 = 0.0;\r\n  if (*(int *)(param_1 + 0x2640) < 0) {\r\n    fVar10 = 0.0;\r\n  }\r\n  else {\r\n    fVar10 = *(float *)(*(longlong *)(*(longlong *)(param_1 + 0x2628) + 0x48) + 0x24 +\r\n                       (longlong)*(int *)(param_1 + 0x2640) * 0x28);\r\n  }\r\n  uVar7 = (int)(longlong)fVar10 + 2;\r\n  if (4 < uVar7) {\r\n    uVar7 = 4;\r\n  }\r\n  if (uVar7 == (*(uint *)(param_1 + 0x30) & 0xffffff)) {\r\n    plVar5 = *(longlong **)(param_1 + 0x28);\r\n    lVar9 = 0;\r\n    plVar1 = plVar5 + (*(uint *)(param_1 + 0x30) & 0xffffff);\r\n    for (; plVar5 != plVar1; plVar5 = plVar5 + 1) {\r\n      fVar10 = *(float *)(*plVar5 + 0x7c);\r\n      uVar3 = (ulonglong)local_res8 >> 0x20;\r\n      if ((lVar9 == 0) || (fVar11 < fVar10)) {\r\n        lVar9 = *plVar5;\r\n        fVar11 = fVar10;\r\n      }\r\n      local_res8 = CONCAT44((int)uVar3,fVar10);\r\n    }\r\n    FUN_1404bd510(param_1,lVar9,CONCAT71((int7)((ulonglong)plVar1 >> 8),1));\r\n  }\r\n  plVar5 = *(longlong **)(param_1 + 0x58);\r\n  plVar1 = plVar5 + (*(uint *)(param_1 + 0x60) & 0xffffff);\r\n  for (; plVar5 != plVar1; plVar5 = plVar5 + 1) {\r\n    lVar9 = *plVar5;\r\n    lVar6 = FUN_1404db590(lVar9);\r\n    if (lVar6 == param_2) goto LAB_1404bca3e;\r\n  }\r\n  lVar9 = 0;\r\nLAB_1404bca3e:\r\n  local_res8 = lVar9;\r\n  if (lVar9 == 0) {\r\n    uVar4 = FUN_1404bcbc0(param_1 + 0x68,0xffff);\r\n    if (uVar4 != 0xffff) {\r\n      lVar9 = (ulonglong)uVar4 * 0x150 + *(longlong *)(param_1 + 0x68);\r\n    }\r\n    FUN_1404db5c0(lVar9,param_2,*(longlong *)(param_1 + 8) + 0x80);\r\n  }\r\n  else {\r\n    FUN_1407b8270(param_1 + 0x58,&local_res8);\r\n  }\r\n  FUN_1404bc8d0(param_1,lVar9);\r\n  FUN_1404bd2e0(param_1,lVar9);\r\n  *(longlong *)(param_1 + 0x20) = lVar9;\r\n  puVar8 = *(undefined8 **)(param_1 + 0x15b0);\r\n  puVar2 = puVar8 + (*(uint *)(param_1 + 0x15b8) & 0xffffff);\r\n  for (; puVar8 != puVar2; puVar8 = puVar8 + 1) {\r\n    if (*(uint *)*puVar8 < 2) {\r\n      if (*(longlong *)((uint *)*puVar8 + 2) == *(longlong *)(param_1 + 0x20)) {\r\n        FUN_1404dd500();\r\n      }\r\n      else {\r\n        FUN_1404dd910();\r\n      }\r\n    }\r\n  }\r\n  FUN_1404bd900(param_1,lVar9);\r\n  return;\r\n}\r\n\r\n"
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
            "text": "\r\nvoid FUN_1404be8b0(longlong param_1,longlong param_2)\r\n\r\n{\r\n  longlong *plVar1;\r\n  longlong lVar2;\r\n  longlong *plVar3;\r\n  longlong lVar4;\r\n  longlong local_res8;\r\n  \r\n  plVar3 = *(longlong **)(param_1 + 0x28);\r\n  plVar1 = plVar3 + (*(uint *)(param_1 + 0x30) & 0xffffff);\r\n  for (; plVar3 != plVar1; plVar3 = plVar3 + 1) {\r\n    lVar4 = *plVar3;\r\n    lVar2 = FUN_1404db590(lVar4);\r\n    if (lVar2 == param_2) goto LAB_1404be8fb;\r\n  }\r\n  lVar4 = 0;\r\nLAB_1404be8fb:\r\n  FUN_1404bd510(param_1,lVar4,1);\r\n  if (*(int *)(lVar4 + 0x84) < 1) {\r\n    local_res8 = lVar4;\r\n    FUN_1404dbd70(lVar4);\r\n    FUN_1407b8270(param_1 + 0x58,&local_res8);\r\n    lVar4 = lVar4 - *(longlong *)(param_1 + 0x68);\r\n    lVar4 = SUB168(SEXT816(-0x3cf3cf3cf3cf3cf3) * SEXT816(lVar4),8) + lVar4;\r\n    FUN_1404bd780(param_1 + 0x68,(lVar4 >> 8) - (lVar4 >> 0x3f));\r\n  }\r\n  if ((((*(uint *)(param_1 + 0x30) & 0xffffff) != 0) && (lVar4 = FUN_1404bf530(param_1), lVar4 != 0)\r\n      ) && (*(longlong *)(param_1 + 0x20) != lVar4)) {\r\n    FUN_1404be740(param_1,lVar4);\r\n  }\r\n  return;\r\n}\r\n\r\n"
          }
        ],
        "structuredContent": {
          "result": "\r\nvoid FUN_1404be8b0(longlong param_1,longlong param_2)\r\n\r\n{\r\n  longlong *plVar1;\r\n  longlong lVar2;\r\n  longlong *plVar3;\r\n  longlong lVar4;\r\n  longlong local_res8;\r\n  \r\n  plVar3 = *(longlong **)(param_1 + 0x28);\r\n  plVar1 = plVar3 + (*(uint *)(param_1 + 0x30) & 0xffffff);\r\n  for (; plVar3 != plVar1; plVar3 = plVar3 + 1) {\r\n    lVar4 = *plVar3;\r\n    lVar2 = FUN_1404db590(lVar4);\r\n    if (lVar2 == param_2) goto LAB_1404be8fb;\r\n  }\r\n  lVar4 = 0;\r\nLAB_1404be8fb:\r\n  FUN_1404bd510(param_1,lVar4,1);\r\n  if (*(int *)(lVar4 + 0x84) < 1) {\r\n    local_res8 = lVar4;\r\n    FUN_1404dbd70(lVar4);\r\n    FUN_1407b8270(param_1 + 0x58,&local_res8);\r\n    lVar4 = lVar4 - *(longlong *)(param_1 + 0x68);\r\n    lVar4 = SUB168(SEXT816(-0x3cf3cf3cf3cf3cf3) * SEXT816(lVar4),8) + lVar4;\r\n    FUN_1404bd780(param_1 + 0x68,(lVar4 >> 8) - (lVar4 >> 0x3f));\r\n  }\r\n  if ((((*(uint *)(param_1 + 0x30) & 0xffffff) != 0) && (lVar4 = FUN_1404bf530(param_1), lVar4 != 0)\r\n      ) && (*(longlong *)(param_1 + 0x20) != lVar4)) {\r\n    FUN_1404be740(param_1,lVar4);\r\n  }\r\n  return;\r\n}\r\n\r\n"
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

bool FUN_140447460(longlong param_1)

{
  int iVar1;
  longlong lVar2;
  undefined8 uVar3;
  float fVar4;
  float fVar5;
  
  lVar2 = FUN_1406dc560(*(undefined4 *)(param_1 + 200));
  if (lVar2 == 0) {
    return false;
  }
  uVar3 = FUN_1406dc560(*(undefined4 *)(param_1 + 200));
  lVar2 = FUN_1404bd680(*(longlong *)(param_1 + 0xb8),uVar3,*(longlong *)(param_1 + 0xb8) + 0x28);
  if (lVar2 == 0) {
    iVar1 = FUN_1404bd810(*(undefined8 *)(param_1 + 0xb8));
    if (iVar1 != 0) {
      fVar4 = (float)FUN_14061c060(param_1);
      uVar3 = FUN_1406dc560(*(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0xac));
      lVar2 = FUN_140877720(uVar3,&DAT_1439d13b0,2);
      if (lVar2 != 0) {
        fVar5 = (float)FUN_140745f60(lVar2);
        return fVar4 <= fVar5;
      }
      return false;
    }
  }
  return true;
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

undefined8 FUN_1404bd680(undefined8 param_1,longlong param_2,longlong *param_3)

{
  undefined8 *puVar1;
  undefined8 uVar2;
  longlong lVar3;
  undefined8 *puVar4;
  
  puVar4 = (undefined8 *)*param_3;
  puVar1 = puVar4 + (*(uint *)(param_3 + 1) & 0xffffff);
  while( true ) {
    if (puVar4 == puVar1) {
      return 0;
    }
    uVar2 = *puVar4;
    lVar3 = FUN_1404db590(uVar2);
    if (lVar3 == param_2) break;
    puVar4 = puVar4 + 1;
  }
  return uVar2;
}


````

### text

````text

void FUN_1404bc950(longlong param_1,longlong param_2)

{
  longlong *plVar1;
  undefined8 *puVar2;
  ulonglong uVar3;
  ushort uVar4;
  longlong *plVar5;
  longlong lVar6;
  uint uVar7;
  undefined8 *puVar8;
  longlong lVar9;
  float fVar10;
  float fVar11;
  undefined8 local_res8;
  
  fVar11 = 0.0;
  if (*(int *)(param_1 + 0x2640) < 0) {
    fVar10 = 0.0;
  }
  else {
    fVar10 = *(float *)(*(longlong *)(*(longlong *)(param_1 + 0x2628) + 0x48) + 0x24 +
                       (longlong)*(int *)(param_1 + 0x2640) * 0x28);
  }
  uVar7 = (int)(longlong)fVar10 + 2;
  if (4 < uVar7) {
    uVar7 = 4;
  }
  if (uVar7 == (*(uint *)(param_1 + 0x30) & 0xffffff)) {
    plVar5 = *(longlong **)(param_1 + 0x28);
    lVar9 = 0;
    plVar1 = plVar5 + (*(uint *)(param_1 + 0x30) & 0xffffff);
    for (; plVar5 != plVar1; plVar5 = plVar5 + 1) {
      fVar10 = *(float *)(*plVar5 + 0x7c);
      uVar3 = (ulonglong)local_res8 >> 0x20;
      if ((lVar9 == 0) || (fVar11 < fVar10)) {
        lVar9 = *plVar5;
        fVar11 = fVar10;
      }
      local_res8 = CONCAT44((int)uVar3,fVar10);
    }
    FUN_1404bd510(param_1,lVar9,CONCAT71((int7)((ulonglong)plVar1 >> 8),1));
  }
  plVar5 = *(longlong **)(param_1 + 0x58);
  plVar1 = plVar5 + (*(uint *)(param_1 + 0x60) & 0xffffff);
  for (; plVar5 != plVar1; plVar5 = plVar5 + 1) {
    lVar9 = *plVar5;
    lVar6 = FUN_1404db590(lVar9);
    if (lVar6 == param_2) goto LAB_1404bca3e;
  }
  lVar9 = 0;
LAB_1404bca3e:
  local_res8 = lVar9;
  if (lVar9 == 0) {
    uVar4 = FUN_1404bcbc0(param_1 + 0x68,0xffff);
    if (uVar4 != 0xffff) {
      lVar9 = (ulonglong)uVar4 * 0x150 + *(longlong *)(param_1 + 0x68);
    }
    FUN_1404db5c0(lVar9,param_2,*(longlong *)(param_1 + 8) + 0x80);
  }
  else {
    FUN_1407b8270(param_1 + 0x58,&local_res8);
  }
  FUN_1404bc8d0(param_1,lVar9);
  FUN_1404bd2e0(param_1,lVar9);
  *(longlong *)(param_1 + 0x20) = lVar9;
  puVar8 = *(undefined8 **)(param_1 + 0x15b0);
  puVar2 = puVar8 + (*(uint *)(param_1 + 0x15b8) & 0xffffff);
  for (; puVar8 != puVar2; puVar8 = puVar8 + 1) {
    if (*(uint *)*puVar8 < 2) {
      if (*(longlong *)((uint *)*puVar8 + 2) == *(longlong *)(param_1 + 0x20)) {
        FUN_1404dd500();
      }
      else {
        FUN_1404dd910();
      }
    }
  }
  FUN_1404bd900(param_1,lVar9);
  return;
}


````

### text

````text

void FUN_1404be8b0(longlong param_1,longlong param_2)

{
  longlong *plVar1;
  longlong lVar2;
  longlong *plVar3;
  longlong lVar4;
  longlong local_res8;
  
  plVar3 = *(longlong **)(param_1 + 0x28);
  plVar1 = plVar3 + (*(uint *)(param_1 + 0x30) & 0xffffff);
  for (; plVar3 != plVar1; plVar3 = plVar3 + 1) {
    lVar4 = *plVar3;
    lVar2 = FUN_1404db590(lVar4);
    if (lVar2 == param_2) goto LAB_1404be8fb;
  }
  lVar4 = 0;
LAB_1404be8fb:
  FUN_1404bd510(param_1,lVar4,1);
  if (*(int *)(lVar4 + 0x84) < 1) {
    local_res8 = lVar4;
    FUN_1404dbd70(lVar4);
    FUN_1407b8270(param_1 + 0x58,&local_res8);
    lVar4 = lVar4 - *(longlong *)(param_1 + 0x68);
    lVar4 = SUB168(SEXT816(-0x3cf3cf3cf3cf3cf3) * SEXT816(lVar4),8) + lVar4;
    FUN_1404bd780(param_1 + 0x68,(lVar4 >> 8) - (lVar4 >> 0x3f));
  }
  if ((((*(uint *)(param_1 + 0x30) & 0xffffff) != 0) && (lVar4 = FUN_1404bf530(param_1), lVar4 != 0)
      ) && (*(longlong *)(param_1 + 0x20) != lVar4)) {
    FUN_1404be740(param_1,lVar4);
  }
  return;
}


````
