# inventory-equip-decompilation.json

Original: [outputs/d2-coop-lab/analysis/ghidra/inventory-equip-decompilation.json](<../../../../outputs/d2-coop-lab/analysis/ghidra/inventory-equip-decompilation.json>)

SHA-256: `3DCE2C97EAEF6488200E35A99D6CD8DF19F9218F2076D90E610FE9D4B0BF1551`; 3503 bytes.

## Original saved record

````json
{
  "meta": null,
  "content": [
    {
      "type": "text",
      "text": "\r\n/* [confirmed-runtime] RVA 0x614b20\r\n   Native inventory, existing equipment entry, bool true. Guard and authored Corvo tutorial sword;\r\n   no Emily counterpart equipment yet.\r\n   Evidence: src/avatar_sword.h\r\n   Names are analyst-assigned semantic labels, not recovered official symbols. */\r\n\r\nvoid D2_Inventory_Equip(D2_InventoryPartial *inventory,D2_InventoryEntryPartial *entry,\r\n                       bool requireQuantity)\r\n\r\n{\r\n  int iVar1;\r\n  undefined4 uVar2;\r\n  undefined *puVar3;\r\n  D2_InventoryEntryPartial *pDVar4;\r\n  longlong lVar5;\r\n  longlong lVar6;\r\n  undefined8 *puVar7;\r\n  undefined8 uVar8;\r\n  \r\n  if ((0 < entry->quantity) || (!requireQuantity)) {\r\n    lVar5 = FUN_14063c8c0(entry);\r\n    puVar3 = entry->itemDeclaration;\r\n    iVar1 = *(int *)(lVar5 + 0x68);\r\n    uVar2 = *(undefined4 *)(inventory->owner + 0xa8);\r\n    pDVar4 = (D2_InventoryEntryPartial *)inventory->slots[iVar1];\r\n    lVar6 = func_0x000140810240();\r\n    puVar7 = (undefined8 *)FUN_1408112c0(lVar6 + 0xc8e8,0x9f,0x10,uVar2);\r\n    if (puVar7 != (undefined8 *)0x0) {\r\n      *puVar7 = puVar3;\r\n      puVar7[1] = lVar5;\r\n    }\r\n    if (pDVar4 != entry) {\r\n      if (pDVar4 != (D2_InventoryEntryPartial *)0x0) {\r\n        uVar8 = FUN_14063c890(pDVar4);\r\n        FUN_1407e7c00(uVar8);\r\n      }\r\n      inventory->slots[iVar1] = (undefined *)entry;\r\n      uVar8 = FUN_14063c890(entry);\r\n      FUN_1407e7b10(uVar8);\r\n      if (**(char **)(entry->itemDeclaration + 0xb0) != '\\0') {\r\n        FUN_1401911b0(inventory[1].slots + 1);\r\n      }\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n",
      "annotations": null,
      "meta": null
    }
  ],
  "structuredContent": {
    "result": "\r\n/* [confirmed-runtime] RVA 0x614b20\r\n   Native inventory, existing equipment entry, bool true. Guard and authored Corvo tutorial sword;\r\n   no Emily counterpart equipment yet.\r\n   Evidence: src/avatar_sword.h\r\n   Names are analyst-assigned semantic labels, not recovered official symbols. */\r\n\r\nvoid D2_Inventory_Equip(D2_InventoryPartial *inventory,D2_InventoryEntryPartial *entry,\r\n                       bool requireQuantity)\r\n\r\n{\r\n  int iVar1;\r\n  undefined4 uVar2;\r\n  undefined *puVar3;\r\n  D2_InventoryEntryPartial *pDVar4;\r\n  longlong lVar5;\r\n  longlong lVar6;\r\n  undefined8 *puVar7;\r\n  undefined8 uVar8;\r\n  \r\n  if ((0 < entry->quantity) || (!requireQuantity)) {\r\n    lVar5 = FUN_14063c8c0(entry);\r\n    puVar3 = entry->itemDeclaration;\r\n    iVar1 = *(int *)(lVar5 + 0x68);\r\n    uVar2 = *(undefined4 *)(inventory->owner + 0xa8);\r\n    pDVar4 = (D2_InventoryEntryPartial *)inventory->slots[iVar1];\r\n    lVar6 = func_0x000140810240();\r\n    puVar7 = (undefined8 *)FUN_1408112c0(lVar6 + 0xc8e8,0x9f,0x10,uVar2);\r\n    if (puVar7 != (undefined8 *)0x0) {\r\n      *puVar7 = puVar3;\r\n      puVar7[1] = lVar5;\r\n    }\r\n    if (pDVar4 != entry) {\r\n      if (pDVar4 != (D2_InventoryEntryPartial *)0x0) {\r\n        uVar8 = FUN_14063c890(pDVar4);\r\n        FUN_1407e7c00(uVar8);\r\n      }\r\n      inventory->slots[iVar1] = (undefined *)entry;\r\n      uVar8 = FUN_14063c890(entry);\r\n      FUN_1407e7b10(uVar8);\r\n      if (**(char **)(entry->itemDeclaration + 0xb0) != '\\0') {\r\n        FUN_1401911b0(inventory[1].slots + 1);\r\n      }\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
  },
  "isError": false
}

````

## Unescaped text fields for reading

### text

````text

/* [confirmed-runtime] RVA 0x614b20
   Native inventory, existing equipment entry, bool true. Guard and authored Corvo tutorial sword;
   no Emily counterpart equipment yet.
   Evidence: src/avatar_sword.h
   Names are analyst-assigned semantic labels, not recovered official symbols. */

void D2_Inventory_Equip(D2_InventoryPartial *inventory,D2_InventoryEntryPartial *entry,
                       bool requireQuantity)

{
  int iVar1;
  undefined4 uVar2;
  undefined *puVar3;
  D2_InventoryEntryPartial *pDVar4;
  longlong lVar5;
  longlong lVar6;
  undefined8 *puVar7;
  undefined8 uVar8;
  
  if ((0 < entry->quantity) || (!requireQuantity)) {
    lVar5 = FUN_14063c8c0(entry);
    puVar3 = entry->itemDeclaration;
    iVar1 = *(int *)(lVar5 + 0x68);
    uVar2 = *(undefined4 *)(inventory->owner + 0xa8);
    pDVar4 = (D2_InventoryEntryPartial *)inventory->slots[iVar1];
    lVar6 = func_0x000140810240();
    puVar7 = (undefined8 *)FUN_1408112c0(lVar6 + 0xc8e8,0x9f,0x10,uVar2);
    if (puVar7 != (undefined8 *)0x0) {
      *puVar7 = puVar3;
      puVar7[1] = lVar5;
    }
    if (pDVar4 != entry) {
      if (pDVar4 != (D2_InventoryEntryPartial *)0x0) {
        uVar8 = FUN_14063c890(pDVar4);
        FUN_1407e7c00(uVar8);
      }
      inventory->slots[iVar1] = (undefined *)entry;
      uVar8 = FUN_14063c890(entry);
      FUN_1407e7b10(uVar8);
      if (**(char **)(entry->itemDeclaration + 0xb0) != '\0') {
        FUN_1401911b0(inventory[1].slots + 1);
      }
    }
  }
  return;
}


````
