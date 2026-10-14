# ghidra-inventory-capacity-v803.json

Original: [work/natural-moveset/native-pickup-research-v785/ghidra-inventory-capacity-v803.json](<../../../../work/natural-moveset/native-pickup-research-v785/ghidra-inventory-capacity-v803.json>)

SHA-256: `F2B5C7775279B1CC3AF2E33F9D0229C559137192E8CC70D1544A00702D9B6E0F`; 6945 bytes.

## Original saved record

````json
{
  "140615d40": {
    "status": "fulfilled",
    "value": {
      "content": [
        {
          "type": "text",
          "text": "\r\nundefined8 FUN_140615d40(undefined8 param_1,longlong param_2)\r\n\r\n{\r\n  int iVar1;\r\n  undefined8 uVar2;\r\n  \r\n  if ((*(uint *)(param_2 + 0x60) < 0x12) &&\r\n     ((0x38022U >> (*(uint *)(param_2 + 0x60) & 0x1f) & 1) != 0)) {\r\n    return 0;\r\n  }\r\n  iVar1 = FUN_140614f30(param_1);\r\n  if (-1 < iVar1) {\r\n    uVar2 = FUN_140615d90(param_1,iVar1);\r\n    return uVar2;\r\n  }\r\n  return 1;\r\n}\r\n\r\n"
        }
      ],
      "structuredContent": {
        "result": "\r\nundefined8 FUN_140615d40(undefined8 param_1,longlong param_2)\r\n\r\n{\r\n  int iVar1;\r\n  undefined8 uVar2;\r\n  \r\n  if ((*(uint *)(param_2 + 0x60) < 0x12) &&\r\n     ((0x38022U >> (*(uint *)(param_2 + 0x60) & 0x1f) & 1) != 0)) {\r\n    return 0;\r\n  }\r\n  iVar1 = FUN_140614f30(param_1);\r\n  if (-1 < iVar1) {\r\n    uVar2 = FUN_140615d90(param_1,iVar1);\r\n    return uVar2;\r\n  }\r\n  return 1;\r\n}\r\n\r\n"
      },
      "isError": false
    }
  },
  "140614f30": {
    "status": "fulfilled",
    "value": {
      "content": [
        {
          "type": "text",
          "text": "\r\nvoid FUN_140614f30(longlong param_1,undefined8 param_2)\r\n\r\n{\r\n  undefined **local_res8 [4];\r\n  undefined8 local_40;\r\n  undefined4 local_38;\r\n  undefined8 local_30;\r\n  undefined8 local_28;\r\n  undefined8 local_20;\r\n  undefined1 local_18;\r\n  undefined4 local_14;\r\n  undefined1 local_b;\r\n  \r\n  local_res8[0] = &PTR_FUN_141b9a840;\r\n  local_38 = 0x3fff0010;\r\n  local_30 = 0;\r\n  local_28 = 0;\r\n  local_20 = 0;\r\n  local_18 = 0;\r\n  local_14 = 0;\r\n  local_b = 1;\r\n  local_40 = param_2;\r\n  FUN_140618af0(local_res8,*(undefined8 *)(param_1 + 0x38),*(uint *)(param_1 + 0x40) & 0xffffff,\r\n                &local_40,0xfffffffffffffffe);\r\n  return;\r\n}\r\n\r\n"
        }
      ],
      "structuredContent": {
        "result": "\r\nvoid FUN_140614f30(longlong param_1,undefined8 param_2)\r\n\r\n{\r\n  undefined **local_res8 [4];\r\n  undefined8 local_40;\r\n  undefined4 local_38;\r\n  undefined8 local_30;\r\n  undefined8 local_28;\r\n  undefined8 local_20;\r\n  undefined1 local_18;\r\n  undefined4 local_14;\r\n  undefined1 local_b;\r\n  \r\n  local_res8[0] = &PTR_FUN_141b9a840;\r\n  local_38 = 0x3fff0010;\r\n  local_30 = 0;\r\n  local_28 = 0;\r\n  local_20 = 0;\r\n  local_18 = 0;\r\n  local_14 = 0;\r\n  local_b = 1;\r\n  local_40 = param_2;\r\n  FUN_140618af0(local_res8,*(undefined8 *)(param_1 + 0x38),*(uint *)(param_1 + 0x40) & 0xffffff,\r\n                &local_40,0xfffffffffffffffe);\r\n  return;\r\n}\r\n\r\n"
      },
      "isError": false
    }
  },
  "14063c9f0": {
    "status": "fulfilled",
    "value": {
      "content": [
        {
          "type": "text",
          "text": "{\"error\":\"No function found for 0x14063c9f0\"}"
        }
      ],
      "structuredContent": {
        "result": "{\"error\":\"No function found for 0x14063c9f0\"}"
      },
      "isError": false
    }
  },
  "1406170b0": {
    "status": "fulfilled",
    "value": {
      "content": [
        {
          "type": "text",
          "text": "\r\nvoid FUN_1406170b0(longlong param_1,longlong *param_2,int param_3,int param_4)\r\n\r\n{\r\n  undefined4 uVar1;\r\n  longlong lVar2;\r\n  longlong *plVar3;\r\n  longlong lVar4;\r\n  undefined *puVar5;\r\n  \r\n  if (param_3 != param_4) {\r\n    lVar4 = *param_2;\r\n    uVar1 = *(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0xa8);\r\n    lVar2 = FUN_140810240();\r\n    plVar3 = (longlong *)FUN_1408112c0(lVar2 + 0xc8e8,0x1ad,0x10,uVar1);\r\n    if (plVar3 != (longlong *)0x0) {\r\n      *plVar3 = lVar4;\r\n      plVar3[1] = CONCAT44(param_4,param_3);\r\n    }\r\n  }\r\n  if ((param_3 == 0) && (0 < param_4)) {\r\n    D2_Inventory_EnsureEquipmentEntity_Hypothesis(param_1,param_2,0);\r\n    FUN_1406139f0(param_1,param_2);\r\n  }\r\n  else if (param_4 == 0) {\r\n    D2_Inventory_NotifyItemDepleted_Hypothesis(param_1,param_2);\r\n    goto LAB_14061716b;\r\n  }\r\n  if ((0 < param_4) &&\r\n     (lVar4 = *(longlong *)(param_1 + 0x10), lVar2 = FUN_1408a8ba0(D2_GameSingletonPtr),\r\n     lVar2 == lVar4)) {\r\n    *(undefined1 *)((longlong)param_2 + 0x34) = 1;\r\n  }\r\nLAB_14061716b:\r\n  lVar4 = FUN_1406dc560((int)param_2[1]);\r\n  if (lVar4 != 0) {\r\n    FUN_1406162b0(param_1,param_2,param_3,param_4);\r\n  }\r\n  for (puVar5 = (undefined *)(**(code **)(*(longlong *)*param_2 + 0xb8))();\r\n      puVar5 != (undefined *)0x0; puVar5 = *(undefined **)(puVar5 + 0xa0)) {\r\n    if (puVar5 == &DAT_14397edd0) {\r\n      FUN_1406164b0(param_1,param_2,*param_2,param_3,param_4);\r\n      return;\r\n    }\r\n  }\r\n  lVar4 = *param_2;\r\n  if (*(int *)(lVar4 + 0x60) == 0x12) {\r\n    FUN_140616210(param_1,param_2,param_3,param_4);\r\n    return;\r\n  }\r\n  if ((*(uint *)(lVar4 + 0x98) & 0xffffff) != 0) {\r\n    FUN_140616940(param_1,lVar4 + 0x90,param_3,param_4);\r\n  }\r\n  return;\r\n}\r\n\r\n"
        }
      ],
      "structuredContent": {
        "result": "\r\nvoid FUN_1406170b0(longlong param_1,longlong *param_2,int param_3,int param_4)\r\n\r\n{\r\n  undefined4 uVar1;\r\n  longlong lVar2;\r\n  longlong *plVar3;\r\n  longlong lVar4;\r\n  undefined *puVar5;\r\n  \r\n  if (param_3 != param_4) {\r\n    lVar4 = *param_2;\r\n    uVar1 = *(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0xa8);\r\n    lVar2 = FUN_140810240();\r\n    plVar3 = (longlong *)FUN_1408112c0(lVar2 + 0xc8e8,0x1ad,0x10,uVar1);\r\n    if (plVar3 != (longlong *)0x0) {\r\n      *plVar3 = lVar4;\r\n      plVar3[1] = CONCAT44(param_4,param_3);\r\n    }\r\n  }\r\n  if ((param_3 == 0) && (0 < param_4)) {\r\n    D2_Inventory_EnsureEquipmentEntity_Hypothesis(param_1,param_2,0);\r\n    FUN_1406139f0(param_1,param_2);\r\n  }\r\n  else if (param_4 == 0) {\r\n    D2_Inventory_NotifyItemDepleted_Hypothesis(param_1,param_2);\r\n    goto LAB_14061716b;\r\n  }\r\n  if ((0 < param_4) &&\r\n     (lVar4 = *(longlong *)(param_1 + 0x10), lVar2 = FUN_1408a8ba0(D2_GameSingletonPtr),\r\n     lVar2 == lVar4)) {\r\n    *(undefined1 *)((longlong)param_2 + 0x34) = 1;\r\n  }\r\nLAB_14061716b:\r\n  lVar4 = FUN_1406dc560((int)param_2[1]);\r\n  if (lVar4 != 0) {\r\n    FUN_1406162b0(param_1,param_2,param_3,param_4);\r\n  }\r\n  for (puVar5 = (undefined *)(**(code **)(*(longlong *)*param_2 + 0xb8))();\r\n      puVar5 != (undefined *)0x0; puVar5 = *(undefined **)(puVar5 + 0xa0)) {\r\n    if (puVar5 == &DAT_14397edd0) {\r\n      FUN_1406164b0(param_1,param_2,*param_2,param_3,param_4);\r\n      return;\r\n    }\r\n  }\r\n  lVar4 = *param_2;\r\n  if (*(int *)(lVar4 + 0x60) == 0x12) {\r\n    FUN_140616210(param_1,param_2,param_3,param_4);\r\n    return;\r\n  }\r\n  if ((*(uint *)(lVar4 + 0x98) & 0xffffff) != 0) {\r\n    FUN_140616940(param_1,lVar4 + 0x90,param_3,param_4);\r\n  }\r\n  return;\r\n}\r\n\r\n"
      },
      "isError": false
    }
  }
}

````

## Unescaped text fields for reading

### text

````text

undefined8 FUN_140615d40(undefined8 param_1,longlong param_2)

{
  int iVar1;
  undefined8 uVar2;
  
  if ((*(uint *)(param_2 + 0x60) < 0x12) &&
     ((0x38022U >> (*(uint *)(param_2 + 0x60) & 0x1f) & 1) != 0)) {
    return 0;
  }
  iVar1 = FUN_140614f30(param_1);
  if (-1 < iVar1) {
    uVar2 = FUN_140615d90(param_1,iVar1);
    return uVar2;
  }
  return 1;
}


````

### text

````text

void FUN_140614f30(longlong param_1,undefined8 param_2)

{
  undefined **local_res8 [4];
  undefined8 local_40;
  undefined4 local_38;
  undefined8 local_30;
  undefined8 local_28;
  undefined8 local_20;
  undefined1 local_18;
  undefined4 local_14;
  undefined1 local_b;
  
  local_res8[0] = &PTR_FUN_141b9a840;
  local_38 = 0x3fff0010;
  local_30 = 0;
  local_28 = 0;
  local_20 = 0;
  local_18 = 0;
  local_14 = 0;
  local_b = 1;
  local_40 = param_2;
  FUN_140618af0(local_res8,*(undefined8 *)(param_1 + 0x38),*(uint *)(param_1 + 0x40) & 0xffffff,
                &local_40,0xfffffffffffffffe);
  return;
}


````

### text

````text

void FUN_1406170b0(longlong param_1,longlong *param_2,int param_3,int param_4)

{
  undefined4 uVar1;
  longlong lVar2;
  longlong *plVar3;
  longlong lVar4;
  undefined *puVar5;
  
  if (param_3 != param_4) {
    lVar4 = *param_2;
    uVar1 = *(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0xa8);
    lVar2 = FUN_140810240();
    plVar3 = (longlong *)FUN_1408112c0(lVar2 + 0xc8e8,0x1ad,0x10,uVar1);
    if (plVar3 != (longlong *)0x0) {
      *plVar3 = lVar4;
      plVar3[1] = CONCAT44(param_4,param_3);
    }
  }
  if ((param_3 == 0) && (0 < param_4)) {
    D2_Inventory_EnsureEquipmentEntity_Hypothesis(param_1,param_2,0);
    FUN_1406139f0(param_1,param_2);
  }
  else if (param_4 == 0) {
    D2_Inventory_NotifyItemDepleted_Hypothesis(param_1,param_2);
    goto LAB_14061716b;
  }
  if ((0 < param_4) &&
     (lVar4 = *(longlong *)(param_1 + 0x10), lVar2 = FUN_1408a8ba0(D2_GameSingletonPtr),
     lVar2 == lVar4)) {
    *(undefined1 *)((longlong)param_2 + 0x34) = 1;
  }
LAB_14061716b:
  lVar4 = FUN_1406dc560((int)param_2[1]);
  if (lVar4 != 0) {
    FUN_1406162b0(param_1,param_2,param_3,param_4);
  }
  for (puVar5 = (undefined *)(**(code **)(*(longlong *)*param_2 + 0xb8))();
      puVar5 != (undefined *)0x0; puVar5 = *(undefined **)(puVar5 + 0xa0)) {
    if (puVar5 == &DAT_14397edd0) {
      FUN_1406164b0(param_1,param_2,*param_2,param_3,param_4);
      return;
    }
  }
  lVar4 = *param_2;
  if (*(int *)(lVar4 + 0x60) == 0x12) {
    FUN_140616210(param_1,param_2,param_3,param_4);
    return;
  }
  if ((*(uint *)(lVar4 + 0x98) & 0xffffff) != 0) {
    FUN_140616940(param_1,lVar4 + 0x90,param_3,param_4);
  }
  return;
}


````
