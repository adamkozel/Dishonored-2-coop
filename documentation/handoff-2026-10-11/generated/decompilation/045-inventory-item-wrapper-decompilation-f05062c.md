# inventory-item-wrapper-decompilation.json

Original: [outputs/d2-coop-lab/analysis/ghidra/inventory-item-wrapper-decompilation.json](<../../../../outputs/d2-coop-lab/analysis/ghidra/inventory-item-wrapper-decompilation.json>)

SHA-256: `AF81C4FDC1AEBF8811333B6B41640138D1366670141BB0161C0DCA08D502119A`; 3399 bytes.

## Original saved record

````json
{
  "meta": null,
  "content": [
    {
      "type": "text",
      "text": "\r\nvoid FUN_140614c20(longlong param_1,undefined8 param_2,longlong param_3)\r\n\r\n{\r\n  int iVar1;\r\n  uint uVar2;\r\n  longlong lVar3;\r\n  longlong lVar4;\r\n  longlong *plVar5;\r\n  longlong lStackX_18;\r\n  \r\n  if (*(longlong *)(param_3 + 0x1e0) == 0) {\r\n    if ((*(longlong *)(param_3 + 0x1e8) != 0) &&\r\n       (uVar2 = *(uint *)(param_1 + 0x40) & 0xffffff, lVar3 = (longlong)(int)uVar2, uVar2 != 0)) {\r\n      lVar4 = 0;\r\n      do {\r\n        plVar5 = (longlong *)(*(longlong *)(param_1 + 0x38) + lVar4);\r\n        if (*(longlong *)(*plVar5 + 0x88) != 0) {\r\n          iVar1 = *(int *)(*(longlong *)(*plVar5 + 0x88) + 0x60);\r\n          if ((*(int *)(*(longlong *)(param_3 + 0x1e8) + 0x60) <= iVar1) &&\r\n             (iVar1 <= *(int *)(*(longlong *)(param_3 + 0x1e8) + 100))) {\r\n            D2_Inventory_EnsureEquipmentEntity_Hypothesis(param_1,plVar5,0);\r\n            lStackX_18 = FUN_14063c890(plVar5);\r\n            if (lStackX_18 != 0) {\r\n              FUN_1408918f0(param_2,&lStackX_18);\r\n            }\r\n          }\r\n        }\r\n        lVar4 = lVar4 + 0x38;\r\n        lVar3 = lVar3 + -1;\r\n      } while (lVar3 != 0);\r\n    }\r\n  }\r\n  else {\r\n    iVar1 = FUN_140614f30();\r\n    if (-1 < iVar1) {\r\n      lVar3 = (longlong)iVar1 * 0x38 + *(longlong *)(param_1 + 0x38);\r\n      D2_Inventory_EnsureEquipmentEntity_Hypothesis(param_1,lVar3,0);\r\n      lStackX_18 = FUN_14063c890(lVar3);\r\n      if (lStackX_18 != 0) {\r\n        FUN_1408918f0(param_2,&lStackX_18);\r\n        return;\r\n      }\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n",
      "annotations": null,
      "meta": null
    }
  ],
  "structuredContent": {
    "result": "\r\nvoid FUN_140614c20(longlong param_1,undefined8 param_2,longlong param_3)\r\n\r\n{\r\n  int iVar1;\r\n  uint uVar2;\r\n  longlong lVar3;\r\n  longlong lVar4;\r\n  longlong *plVar5;\r\n  longlong lStackX_18;\r\n  \r\n  if (*(longlong *)(param_3 + 0x1e0) == 0) {\r\n    if ((*(longlong *)(param_3 + 0x1e8) != 0) &&\r\n       (uVar2 = *(uint *)(param_1 + 0x40) & 0xffffff, lVar3 = (longlong)(int)uVar2, uVar2 != 0)) {\r\n      lVar4 = 0;\r\n      do {\r\n        plVar5 = (longlong *)(*(longlong *)(param_1 + 0x38) + lVar4);\r\n        if (*(longlong *)(*plVar5 + 0x88) != 0) {\r\n          iVar1 = *(int *)(*(longlong *)(*plVar5 + 0x88) + 0x60);\r\n          if ((*(int *)(*(longlong *)(param_3 + 0x1e8) + 0x60) <= iVar1) &&\r\n             (iVar1 <= *(int *)(*(longlong *)(param_3 + 0x1e8) + 100))) {\r\n            D2_Inventory_EnsureEquipmentEntity_Hypothesis(param_1,plVar5,0);\r\n            lStackX_18 = FUN_14063c890(plVar5);\r\n            if (lStackX_18 != 0) {\r\n              FUN_1408918f0(param_2,&lStackX_18);\r\n            }\r\n          }\r\n        }\r\n        lVar4 = lVar4 + 0x38;\r\n        lVar3 = lVar3 + -1;\r\n      } while (lVar3 != 0);\r\n    }\r\n  }\r\n  else {\r\n    iVar1 = FUN_140614f30();\r\n    if (-1 < iVar1) {\r\n      lVar3 = (longlong)iVar1 * 0x38 + *(longlong *)(param_1 + 0x38);\r\n      D2_Inventory_EnsureEquipmentEntity_Hypothesis(param_1,lVar3,0);\r\n      lStackX_18 = FUN_14063c890(lVar3);\r\n      if (lStackX_18 != 0) {\r\n        FUN_1408918f0(param_2,&lStackX_18);\r\n        return;\r\n      }\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
  },
  "isError": false
}

````

## Unescaped text fields for reading

### text

````text

void FUN_140614c20(longlong param_1,undefined8 param_2,longlong param_3)

{
  int iVar1;
  uint uVar2;
  longlong lVar3;
  longlong lVar4;
  longlong *plVar5;
  longlong lStackX_18;
  
  if (*(longlong *)(param_3 + 0x1e0) == 0) {
    if ((*(longlong *)(param_3 + 0x1e8) != 0) &&
       (uVar2 = *(uint *)(param_1 + 0x40) & 0xffffff, lVar3 = (longlong)(int)uVar2, uVar2 != 0)) {
      lVar4 = 0;
      do {
        plVar5 = (longlong *)(*(longlong *)(param_1 + 0x38) + lVar4);
        if (*(longlong *)(*plVar5 + 0x88) != 0) {
          iVar1 = *(int *)(*(longlong *)(*plVar5 + 0x88) + 0x60);
          if ((*(int *)(*(longlong *)(param_3 + 0x1e8) + 0x60) <= iVar1) &&
             (iVar1 <= *(int *)(*(longlong *)(param_3 + 0x1e8) + 100))) {
            D2_Inventory_EnsureEquipmentEntity_Hypothesis(param_1,plVar5,0);
            lStackX_18 = FUN_14063c890(plVar5);
            if (lStackX_18 != 0) {
              FUN_1408918f0(param_2,&lStackX_18);
            }
          }
        }
        lVar4 = lVar4 + 0x38;
        lVar3 = lVar3 + -1;
      } while (lVar3 != 0);
    }
  }
  else {
    iVar1 = FUN_140614f30();
    if (-1 < iVar1) {
      lVar3 = (longlong)iVar1 * 0x38 + *(longlong *)(param_1 + 0x38);
      D2_Inventory_EnsureEquipmentEntity_Hypothesis(param_1,lVar3,0);
      lStackX_18 = FUN_14063c890(lVar3);
      if (lStackX_18 != 0) {
        FUN_1408918f0(param_2,&lStackX_18);
        return;
      }
    }
  }
  return;
}


````
