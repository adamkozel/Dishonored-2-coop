# ghidra-native-inventory.json

Original: [work/natural-moveset/native-gadgets-research-v561/ghidra-native-inventory.json](<../../../../work/natural-moveset/native-gadgets-research-v561/ghidra-native-inventory.json>)

SHA-256: `88B1D7FAF3602D459228BA2623E35C05E4AA2782CDC12E08A922873E7CE079E9`; 8819 bytes.

## Original saved record

````json
{
  "exactExecutable": "B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180",
  "evidence": [
    {
      "k": "quantity",
      "result": {
        "content": [
          {
            "type": "text",
            "text": "\r\nundefined4 FUN_140618de0(longlong param_1,undefined8 param_2,undefined4 param_3)\r\n\r\n{\r\n  undefined4 uVar1;\r\n  int iVar2;\r\n  undefined4 uVar3;\r\n  longlong lVar4;\r\n  undefined **local_res8;\r\n  undefined8 local_40;\r\n  undefined4 local_38;\r\n  undefined8 local_30;\r\n  undefined8 local_28;\r\n  undefined8 local_20;\r\n  undefined1 local_18;\r\n  undefined4 local_14;\r\n  undefined1 local_b;\r\n  \r\n  local_res8 = &PTR_FUN_141b9a840;\r\n  local_38 = 0x3fff0010;\r\n  local_30 = 0;\r\n  local_28 = 0;\r\n  local_20 = 0;\r\n  local_18 = 0;\r\n  local_14 = 0;\r\n  local_b = 1;\r\n  local_40 = param_2;\r\n  iVar2 = FUN_140618af0(&local_res8,*(undefined8 *)(param_1 + 0x38),\r\n                        *(uint *)(param_1 + 0x40) & 0xffffff,&local_40,0xfffffffffffffffe);\r\n  local_res8 = &PTR_FUN_141b9a808;\r\n  if (iVar2 < 0) {\r\n    uVar3 = 0;\r\n  }\r\n  else {\r\n    lVar4 = (longlong)iVar2 * 0x38 + *(longlong *)(param_1 + 0x38);\r\n    uVar1 = *(undefined4 *)(lVar4 + 0x20);\r\n    uVar3 = FUN_14063cc00(lVar4,param_3);\r\n    FUN_1406170b0(param_1,lVar4,uVar1,uVar3);\r\n  }\r\n  return uVar3;\r\n}\r\n\r\n"
          }
        ],
        "structuredContent": {
          "result": "\r\nundefined4 FUN_140618de0(longlong param_1,undefined8 param_2,undefined4 param_3)\r\n\r\n{\r\n  undefined4 uVar1;\r\n  int iVar2;\r\n  undefined4 uVar3;\r\n  longlong lVar4;\r\n  undefined **local_res8;\r\n  undefined8 local_40;\r\n  undefined4 local_38;\r\n  undefined8 local_30;\r\n  undefined8 local_28;\r\n  undefined8 local_20;\r\n  undefined1 local_18;\r\n  undefined4 local_14;\r\n  undefined1 local_b;\r\n  \r\n  local_res8 = &PTR_FUN_141b9a840;\r\n  local_38 = 0x3fff0010;\r\n  local_30 = 0;\r\n  local_28 = 0;\r\n  local_20 = 0;\r\n  local_18 = 0;\r\n  local_14 = 0;\r\n  local_b = 1;\r\n  local_40 = param_2;\r\n  iVar2 = FUN_140618af0(&local_res8,*(undefined8 *)(param_1 + 0x38),\r\n                        *(uint *)(param_1 + 0x40) & 0xffffff,&local_40,0xfffffffffffffffe);\r\n  local_res8 = &PTR_FUN_141b9a808;\r\n  if (iVar2 < 0) {\r\n    uVar3 = 0;\r\n  }\r\n  else {\r\n    lVar4 = (longlong)iVar2 * 0x38 + *(longlong *)(param_1 + 0x38);\r\n    uVar1 = *(undefined4 *)(lVar4 + 0x20);\r\n    uVar3 = FUN_14063cc00(lVar4,param_3);\r\n    FUN_1406170b0(param_1,lVar4,uVar1,uVar3);\r\n  }\r\n  return uVar3;\r\n}\r\n\r\n"
        },
        "isError": false
      }
    },
    {
      "k": "equip",
      "result": {
        "content": [
          {
            "type": "text",
            "text": "\r\n/* [confirmed-static 2026-10-09, exact B671F824 executable] MCP decompilation of RVA614b20\r\n   corroborates: requireQuantity gates entry+20 > 0; declaration+68 selects slot; prior slot\r\n   component receives 7e7c00; slot is replaced; new component receives 7e7b10. Event allocation\r\n   8112c0 uses owner handle and event 0x9f. Candidate names of 7e7c00/7e7b10 are Stop/Start but\r\n   remain hypotheses until direct decompilation. This equips an existing native entry and does not\r\n   create Emily equipment. Evidence: inventory-equip-decompilation.json, src/avatar_sword.h. */\r\n\r\nvoid D2_Inventory_Equip(D2_InventoryPartial *inventory,D2_InventoryEntryPartial *entry,\r\n                       bool requireQuantity)\r\n\r\n{\r\n  int iVar1;\r\n  undefined4 uVar2;\r\n  undefined *puVar3;\r\n  D2_InventoryEntryPartial *pDVar4;\r\n  longlong lVar5;\r\n  longlong lVar6;\r\n  undefined8 *puVar7;\r\n  undefined8 uVar8;\r\n  \r\n  if ((0 < entry->quantity) || (!requireQuantity)) {\r\n    lVar5 = FUN_14063c8c0(entry);\r\n    puVar3 = entry->itemDeclaration;\r\n    iVar1 = *(int *)(lVar5 + 0x68);\r\n    uVar2 = *(undefined4 *)(inventory->owner + 0xa8);\r\n    pDVar4 = (D2_InventoryEntryPartial *)inventory->slots[iVar1];\r\n    lVar6 = FUN_140810240();\r\n    puVar7 = (undefined8 *)FUN_1408112c0(lVar6 + 0xc8e8,0x9f,0x10,uVar2);\r\n    if (puVar7 != (undefined8 *)0x0) {\r\n      *puVar7 = puVar3;\r\n      puVar7[1] = lVar5;\r\n    }\r\n    if (pDVar4 != entry) {\r\n      if (pDVar4 != (D2_InventoryEntryPartial *)0x0) {\r\n        uVar8 = FUN_14063c890(pDVar4);\r\n        FUN_1407e7c00(uVar8);\r\n      }\r\n      inventory->slots[iVar1] = (undefined *)entry;\r\n      uVar8 = FUN_14063c890(entry);\r\n      FUN_1407e7b10(uVar8);\r\n      if (**(char **)(entry->itemDeclaration + 0xb0) != '\\0') {\r\n        FUN_1401911b0(inventory[1].slots + 1);\r\n      }\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
          }
        ],
        "structuredContent": {
          "result": "\r\n/* [confirmed-static 2026-10-09, exact B671F824 executable] MCP decompilation of RVA614b20\r\n   corroborates: requireQuantity gates entry+20 > 0; declaration+68 selects slot; prior slot\r\n   component receives 7e7c00; slot is replaced; new component receives 7e7b10. Event allocation\r\n   8112c0 uses owner handle and event 0x9f. Candidate names of 7e7c00/7e7b10 are Stop/Start but\r\n   remain hypotheses until direct decompilation. This equips an existing native entry and does not\r\n   create Emily equipment. Evidence: inventory-equip-decompilation.json, src/avatar_sword.h. */\r\n\r\nvoid D2_Inventory_Equip(D2_InventoryPartial *inventory,D2_InventoryEntryPartial *entry,\r\n                       bool requireQuantity)\r\n\r\n{\r\n  int iVar1;\r\n  undefined4 uVar2;\r\n  undefined *puVar3;\r\n  D2_InventoryEntryPartial *pDVar4;\r\n  longlong lVar5;\r\n  longlong lVar6;\r\n  undefined8 *puVar7;\r\n  undefined8 uVar8;\r\n  \r\n  if ((0 < entry->quantity) || (!requireQuantity)) {\r\n    lVar5 = FUN_14063c8c0(entry);\r\n    puVar3 = entry->itemDeclaration;\r\n    iVar1 = *(int *)(lVar5 + 0x68);\r\n    uVar2 = *(undefined4 *)(inventory->owner + 0xa8);\r\n    pDVar4 = (D2_InventoryEntryPartial *)inventory->slots[iVar1];\r\n    lVar6 = FUN_140810240();\r\n    puVar7 = (undefined8 *)FUN_1408112c0(lVar6 + 0xc8e8,0x9f,0x10,uVar2);\r\n    if (puVar7 != (undefined8 *)0x0) {\r\n      *puVar7 = puVar3;\r\n      puVar7[1] = lVar5;\r\n    }\r\n    if (pDVar4 != entry) {\r\n      if (pDVar4 != (D2_InventoryEntryPartial *)0x0) {\r\n        uVar8 = FUN_14063c890(pDVar4);\r\n        FUN_1407e7c00(uVar8);\r\n      }\r\n      inventory->slots[iVar1] = (undefined *)entry;\r\n      uVar8 = FUN_14063c890(entry);\r\n      FUN_1407e7b10(uVar8);\r\n      if (**(char **)(entry->itemDeclaration + 0xb0) != '\\0') {\r\n        FUN_1401911b0(inventory[1].slots + 1);\r\n      }\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
        },
        "isError": false
      }
    },
    {
      "k": "heartvt",
      "result": {
        "content": [
          {
            "type": "text",
            "text": "From 140611751 in FUN_140611730 [DATA]\nFrom 140611758 in FUN_140611730 [DATA]"
          }
        ],
        "structuredContent": {
          "result": "From 140611751 in FUN_140611730 [DATA]\nFrom 140611758 in FUN_140611730 [DATA]"
        },
        "isError": false
      }
    },
    {
      "k": "561_read_1",
      "result": {
        "status": "fulfilled",
        "value": {
          "content": [
            {
              "type": "text",
              "text": "\r\nundefined8 FUN_14063c890(longlong param_1)\r\n\r\n{\r\n  longlong lVar1;\r\n  \r\n  lVar1 = FUN_1406dc560(*(undefined4 *)(param_1 + 8));\r\n  if (lVar1 != 0) {\r\n    return *(undefined8 *)(param_1 + 0x10);\r\n  }\r\n  return 0;\r\n}\r\n\r\n"
            }
          ],
          "structuredContent": {
            "result": "\r\nundefined8 FUN_14063c890(longlong param_1)\r\n\r\n{\r\n  longlong lVar1;\r\n  \r\n  lVar1 = FUN_1406dc560(*(undefined4 *)(param_1 + 8));\r\n  if (lVar1 != 0) {\r\n    return *(undefined8 *)(param_1 + 0x10);\r\n  }\r\n  return 0;\r\n}\r\n\r\n"
          },
          "isError": false
        }
      }
    },
    {
      "k": "561_read_2",
      "result": {
        "status": "fulfilled",
        "value": {
          "content": [
            {
              "type": "text",
              "text": "\r\nundefined8 FUN_14063c8c0(longlong param_1)\r\n\r\n{\r\n  longlong lVar1;\r\n  \r\n  lVar1 = FUN_1406dc560(*(undefined4 *)(param_1 + 8));\r\n  if (lVar1 != 0) {\r\n    return *(undefined8 *)(param_1 + 0x18);\r\n  }\r\n  return 0;\r\n}\r\n\r\n"
            }
          ],
          "structuredContent": {
            "result": "\r\nundefined8 FUN_14063c8c0(longlong param_1)\r\n\r\n{\r\n  longlong lVar1;\r\n  \r\n  lVar1 = FUN_1406dc560(*(undefined4 *)(param_1 + 8));\r\n  if (lVar1 != 0) {\r\n    return *(undefined8 *)(param_1 + 0x18);\r\n  }\r\n  return 0;\r\n}\r\n\r\n"
          },
          "isError": false
        }
      }
    }
  ]
}

````

## Unescaped text fields for reading

### text

````text

undefined4 FUN_140618de0(longlong param_1,undefined8 param_2,undefined4 param_3)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 uVar3;
  longlong lVar4;
  undefined **local_res8;
  undefined8 local_40;
  undefined4 local_38;
  undefined8 local_30;
  undefined8 local_28;
  undefined8 local_20;
  undefined1 local_18;
  undefined4 local_14;
  undefined1 local_b;
  
  local_res8 = &PTR_FUN_141b9a840;
  local_38 = 0x3fff0010;
  local_30 = 0;
  local_28 = 0;
  local_20 = 0;
  local_18 = 0;
  local_14 = 0;
  local_b = 1;
  local_40 = param_2;
  iVar2 = FUN_140618af0(&local_res8,*(undefined8 *)(param_1 + 0x38),
                        *(uint *)(param_1 + 0x40) & 0xffffff,&local_40,0xfffffffffffffffe);
  local_res8 = &PTR_FUN_141b9a808;
  if (iVar2 < 0) {
    uVar3 = 0;
  }
  else {
    lVar4 = (longlong)iVar2 * 0x38 + *(longlong *)(param_1 + 0x38);
    uVar1 = *(undefined4 *)(lVar4 + 0x20);
    uVar3 = FUN_14063cc00(lVar4,param_3);
    FUN_1406170b0(param_1,lVar4,uVar1,uVar3);
  }
  return uVar3;
}


````

### text

````text

/* [confirmed-static 2026-10-09, exact B671F824 executable] MCP decompilation of RVA614b20
   corroborates: requireQuantity gates entry+20 > 0; declaration+68 selects slot; prior slot
   component receives 7e7c00; slot is replaced; new component receives 7e7b10. Event allocation
   8112c0 uses owner handle and event 0x9f. Candidate names of 7e7c00/7e7b10 are Stop/Start but
   remain hypotheses until direct decompilation. This equips an existing native entry and does not
   create Emily equipment. Evidence: inventory-equip-decompilation.json, src/avatar_sword.h. */

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
    lVar6 = FUN_140810240();
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

### text

````text

undefined8 FUN_14063c890(longlong param_1)

{
  longlong lVar1;
  
  lVar1 = FUN_1406dc560(*(undefined4 *)(param_1 + 8));
  if (lVar1 != 0) {
    return *(undefined8 *)(param_1 + 0x10);
  }
  return 0;
}


````

### text

````text

undefined8 FUN_14063c8c0(longlong param_1)

{
  longlong lVar1;
  
  lVar1 = FUN_1406dc560(*(undefined4 *)(param_1 + 8));
  if (lVar1 != 0) {
    return *(undefined8 *)(param_1 + 0x18);
  }
  return 0;
}


````
