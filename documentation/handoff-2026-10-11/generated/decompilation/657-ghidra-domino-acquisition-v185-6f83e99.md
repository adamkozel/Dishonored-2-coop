# ghidra-domino-acquisition-v185.json

Original: [work/ghidra-domino-acquisition-v185.json](<../../../../work/ghidra-domino-acquisition-v185.json>)

SHA-256: `FC896665142EA77C91BF3C39603193B2A94DDE1D0324DDA0F9EF5B032673003F`; 11994 bytes.

## Original saved record

````json
{
  "program": "Dishonored2.exe",
  "addresses": [
    "140619380",
    "140618de0",
    "140446f20"
  ],
  "results": [
    {
      "status": "fulfilled",
      "value": {
        "content": [
          {
            "type": "text",
            "text": "\r\n/* [confirmed-static behavior; semantic name hypothesis] RVA619380 only enters if itemDeclaration+80\r\n   exists and entry weaponHandle does not resolve. With null third argument it calls the game spawn\r\n   virtual, initializes transform and parentHandle, registers the spawned entity, finds its\r\n   equipment component by RTTI, checks componentDeclaration+11c and slot<16, appends the existing\r\n   entry to inventory equipment list+58, then links it through63cb90. It requires an existing item\r\n   entry and is not a general inventory-add operation. Runtime read-only observation of the current\r\n   owned Emily NPC found exactly eight hat/accessory items and zero equipment entries, no sword. No\r\n   runtime calls to619380 have been made. Next: identify native item insertion and its inverse,\r\n   declaration lifetime and removal invariants before a bounded Emily equip experiment. */\r\n\r\nvoid D2_Inventory_EnsureEquipmentEntity_Hypothesis\r\n               (longlong param_1,longlong *param_2,longlong param_3)\r\n\r\n{\r\n  longlong lVar1;\r\n  undefined8 *puVar2;\r\n  bool bVar3;\r\n  uint uVar4;\r\n  longlong lVar5;\r\n  undefined8 uVar6;\r\n  undefined *puVar7;\r\n  longlong lVar8;\r\n  longlong lVar9;\r\n  longlong *local_res10;\r\n  longlong local_48;\r\n  undefined8 local_40;\r\n  \r\n  lVar1 = *param_2;\r\n  if ((*(longlong *)(lVar1 + 0x80) == 0) || (lVar5 = FUN_1406dc560((int)param_2[1]), lVar5 != 0)) {\r\n    return;\r\n  }\r\n  puVar2 = *(undefined8 **)(param_1 + 0x10);\r\n  lVar5 = (**(code **)*puVar2)(puVar2);\r\n  if ((*(int *)(lVar5 + 0x60) < DAT_1439ce350) || (DAT_1439ce354 < *(int *)(lVar5 + 0x60))) {\r\n    bVar3 = false;\r\n  }\r\n  else {\r\n    bVar3 = true;\r\n  }\r\n  if (param_3 == 0) {\r\n    lVar5 = *D2_GameSingletonPtr;\r\n    uVar6 = FUN_140146520(&local_48);\r\n    param_3 = (**(code **)(lVar5 + 0x1c0))(D2_GameSingletonPtr,*(undefined8 *)(lVar1 + 0x80),uVar6);\r\n    if (param_3 == 0) {\r\n      return;\r\n    }\r\n    FUN_14087c320(param_3,puVar2 + 0x60);\r\n    FUN_14087c2c0(param_3,(longlong)puVar2 + 0x30c);\r\n    if (*(longlong *)(param_1 + 0x10) == 0) {\r\n      *(undefined4 *)(param_3 + 0xac) = 0x3fff0010;\r\n    }\r\n    else {\r\n      *(undefined4 *)(param_3 + 0xac) = *(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0xa8);\r\n    }\r\n    FUN_14087c360(param_3,*(longlong *)(param_1 + 0x10) + 0xc0);\r\n    FUN_14087c350(param_3);\r\n    if (bVar3) {\r\n      *(byte *)(param_3 + 0xd4) = *(byte *)(param_3 + 0xd4) | 8;\r\n    }\r\n    else {\r\n      *(byte *)(param_3 + 0xd4) = *(byte *)(param_3 + 0xd4) | 4;\r\n    }\r\n    FUN_1408aecc0(D2_GameSingletonPtr,param_3,0xffffffff);\r\n  }\r\n  lVar5 = FUN_140877720(param_3,&DAT_1439cb440,0);\r\n  if (lVar5 == 0) {\r\n    puVar7 = &DAT_141b9aa60;\r\n  }\r\n  else {\r\n    if ((*(int *)(*(longlong *)(lVar5 + 8) + 0x11c) != 0) &&\r\n       (*(int *)(*(longlong *)(lVar5 + 8) + 0x68) < 0x10)) {\r\n      local_res10 = param_2;\r\n      FUN_1408918f0(param_1 + 0x58,&local_res10);\r\n      FUN_14063cb90(param_2,param_3);\r\n      if (!bVar3) {\r\n        return;\r\n      }\r\n      lVar5 = FUN_1408e7bb0(lVar5);\r\n      if (lVar5 == 0) {\r\n        return;\r\n      }\r\n      lVar5 = *(longlong *)(lVar5 + 8);\r\n      uVar4 = *(uint *)(lVar5 + 400) & 0xffffff;\r\n      lVar9 = (longlong)(int)uVar4;\r\n      if (uVar4 == 0) {\r\n        return;\r\n      }\r\n      lVar8 = 0;\r\n      local_48 = lVar1;\r\n      do {\r\n        local_40 = *(undefined8 *)(lVar8 + *(longlong *)(lVar5 + 0x188));\r\n        FUN_14030c670(param_1 + 0x28,&local_48);\r\n        lVar8 = lVar8 + 0x70;\r\n        lVar9 = lVar9 + -1;\r\n      } while (lVar9 != 0);\r\n      return;\r\n    }\r\n    puVar7 = &DAT_141b9aa10;\r\n  }\r\n                    /* WARNING: Subroutine does not return */\r\n  FUN_140166830(puVar7,*(undefined8 *)(*(longlong *)(lVar1 + 0x80) + 8),*(undefined8 *)(lVar1 + 8));\r\n}\r\n\r\n"
          }
        ],
        "structuredContent": {
          "result": "\r\n/* [confirmed-static behavior; semantic name hypothesis] RVA619380 only enters if itemDeclaration+80\r\n   exists and entry weaponHandle does not resolve. With null third argument it calls the game spawn\r\n   virtual, initializes transform and parentHandle, registers the spawned entity, finds its\r\n   equipment component by RTTI, checks componentDeclaration+11c and slot<16, appends the existing\r\n   entry to inventory equipment list+58, then links it through63cb90. It requires an existing item\r\n   entry and is not a general inventory-add operation. Runtime read-only observation of the current\r\n   owned Emily NPC found exactly eight hat/accessory items and zero equipment entries, no sword. No\r\n   runtime calls to619380 have been made. Next: identify native item insertion and its inverse,\r\n   declaration lifetime and removal invariants before a bounded Emily equip experiment. */\r\n\r\nvoid D2_Inventory_EnsureEquipmentEntity_Hypothesis\r\n               (longlong param_1,longlong *param_2,longlong param_3)\r\n\r\n{\r\n  longlong lVar1;\r\n  undefined8 *puVar2;\r\n  bool bVar3;\r\n  uint uVar4;\r\n  longlong lVar5;\r\n  undefined8 uVar6;\r\n  undefined *puVar7;\r\n  longlong lVar8;\r\n  longlong lVar9;\r\n  longlong *local_res10;\r\n  longlong local_48;\r\n  undefined8 local_40;\r\n  \r\n  lVar1 = *param_2;\r\n  if ((*(longlong *)(lVar1 + 0x80) == 0) || (lVar5 = FUN_1406dc560((int)param_2[1]), lVar5 != 0)) {\r\n    return;\r\n  }\r\n  puVar2 = *(undefined8 **)(param_1 + 0x10);\r\n  lVar5 = (**(code **)*puVar2)(puVar2);\r\n  if ((*(int *)(lVar5 + 0x60) < DAT_1439ce350) || (DAT_1439ce354 < *(int *)(lVar5 + 0x60))) {\r\n    bVar3 = false;\r\n  }\r\n  else {\r\n    bVar3 = true;\r\n  }\r\n  if (param_3 == 0) {\r\n    lVar5 = *D2_GameSingletonPtr;\r\n    uVar6 = FUN_140146520(&local_48);\r\n    param_3 = (**(code **)(lVar5 + 0x1c0))(D2_GameSingletonPtr,*(undefined8 *)(lVar1 + 0x80),uVar6);\r\n    if (param_3 == 0) {\r\n      return;\r\n    }\r\n    FUN_14087c320(param_3,puVar2 + 0x60);\r\n    FUN_14087c2c0(param_3,(longlong)puVar2 + 0x30c);\r\n    if (*(longlong *)(param_1 + 0x10) == 0) {\r\n      *(undefined4 *)(param_3 + 0xac) = 0x3fff0010;\r\n    }\r\n    else {\r\n      *(undefined4 *)(param_3 + 0xac) = *(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0xa8);\r\n    }\r\n    FUN_14087c360(param_3,*(longlong *)(param_1 + 0x10) + 0xc0);\r\n    FUN_14087c350(param_3);\r\n    if (bVar3) {\r\n      *(byte *)(param_3 + 0xd4) = *(byte *)(param_3 + 0xd4) | 8;\r\n    }\r\n    else {\r\n      *(byte *)(param_3 + 0xd4) = *(byte *)(param_3 + 0xd4) | 4;\r\n    }\r\n    FUN_1408aecc0(D2_GameSingletonPtr,param_3,0xffffffff);\r\n  }\r\n  lVar5 = FUN_140877720(param_3,&DAT_1439cb440,0);\r\n  if (lVar5 == 0) {\r\n    puVar7 = &DAT_141b9aa60;\r\n  }\r\n  else {\r\n    if ((*(int *)(*(longlong *)(lVar5 + 8) + 0x11c) != 0) &&\r\n       (*(int *)(*(longlong *)(lVar5 + 8) + 0x68) < 0x10)) {\r\n      local_res10 = param_2;\r\n      FUN_1408918f0(param_1 + 0x58,&local_res10);\r\n      FUN_14063cb90(param_2,param_3);\r\n      if (!bVar3) {\r\n        return;\r\n      }\r\n      lVar5 = FUN_1408e7bb0(lVar5);\r\n      if (lVar5 == 0) {\r\n        return;\r\n      }\r\n      lVar5 = *(longlong *)(lVar5 + 8);\r\n      uVar4 = *(uint *)(lVar5 + 400) & 0xffffff;\r\n      lVar9 = (longlong)(int)uVar4;\r\n      if (uVar4 == 0) {\r\n        return;\r\n      }\r\n      lVar8 = 0;\r\n      local_48 = lVar1;\r\n      do {\r\n        local_40 = *(undefined8 *)(lVar8 + *(longlong *)(lVar5 + 0x188));\r\n        FUN_14030c670(param_1 + 0x28,&local_48);\r\n        lVar8 = lVar8 + 0x70;\r\n        lVar9 = lVar9 + -1;\r\n      } while (lVar9 != 0);\r\n      return;\r\n    }\r\n    puVar7 = &DAT_141b9aa10;\r\n  }\r\n                    /* WARNING: Subroutine does not return */\r\n  FUN_140166830(puVar7,*(undefined8 *)(*(longlong *)(lVar1 + 0x80) + 8),*(undefined8 *)(lVar1 + 8));\r\n}\r\n\r\n"
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
      "status": "fulfilled",
      "value": {
        "content": [
          {
            "type": "text",
            "text": "\r\nundefined8 *\r\nFUN_140446f20(undefined8 *param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)\r\n\r\n{\r\n  undefined8 uVar1;\r\n  \r\n  uVar1 = 0xfffffffffffffffe;\r\n  FUN_14061ba50();\r\n  *param_1 = &PTR_FUN_141b52d88;\r\n  param_1[0x33] = 0;\r\n  FUN_140131f90(param_1 + 0x34,&DAT_141ab2848,0,param_4,uVar1);\r\n  param_1[0x35] = 0;\r\n  *(undefined4 *)(param_1 + 0x36) = 0;\r\n  *(undefined4 *)(param_1 + 0x32) = 0xc;\r\n  return param_1;\r\n}\r\n\r\n"
          }
        ],
        "structuredContent": {
          "result": "\r\nundefined8 *\r\nFUN_140446f20(undefined8 *param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)\r\n\r\n{\r\n  undefined8 uVar1;\r\n  \r\n  uVar1 = 0xfffffffffffffffe;\r\n  FUN_14061ba50();\r\n  *param_1 = &PTR_FUN_141b52d88;\r\n  param_1[0x33] = 0;\r\n  FUN_140131f90(param_1 + 0x34,&DAT_141ab2848,0,param_4,uVar1);\r\n  param_1[0x35] = 0;\r\n  *(undefined4 *)(param_1 + 0x36) = 0;\r\n  *(undefined4 *)(param_1 + 0x32) = 0xc;\r\n  return param_1;\r\n}\r\n\r\n"
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

/* [confirmed-static behavior; semantic name hypothesis] RVA619380 only enters if itemDeclaration+80
   exists and entry weaponHandle does not resolve. With null third argument it calls the game spawn
   virtual, initializes transform and parentHandle, registers the spawned entity, finds its
   equipment component by RTTI, checks componentDeclaration+11c and slot<16, appends the existing
   entry to inventory equipment list+58, then links it through63cb90. It requires an existing item
   entry and is not a general inventory-add operation. Runtime read-only observation of the current
   owned Emily NPC found exactly eight hat/accessory items and zero equipment entries, no sword. No
   runtime calls to619380 have been made. Next: identify native item insertion and its inverse,
   declaration lifetime and removal invariants before a bounded Emily equip experiment. */

void D2_Inventory_EnsureEquipmentEntity_Hypothesis
               (longlong param_1,longlong *param_2,longlong param_3)

{
  longlong lVar1;
  undefined8 *puVar2;
  bool bVar3;
  uint uVar4;
  longlong lVar5;
  undefined8 uVar6;
  undefined *puVar7;
  longlong lVar8;
  longlong lVar9;
  longlong *local_res10;
  longlong local_48;
  undefined8 local_40;
  
  lVar1 = *param_2;
  if ((*(longlong *)(lVar1 + 0x80) == 0) || (lVar5 = FUN_1406dc560((int)param_2[1]), lVar5 != 0)) {
    return;
  }
  puVar2 = *(undefined8 **)(param_1 + 0x10);
  lVar5 = (**(code **)*puVar2)(puVar2);
  if ((*(int *)(lVar5 + 0x60) < DAT_1439ce350) || (DAT_1439ce354 < *(int *)(lVar5 + 0x60))) {
    bVar3 = false;
  }
  else {
    bVar3 = true;
  }
  if (param_3 == 0) {
    lVar5 = *D2_GameSingletonPtr;
    uVar6 = FUN_140146520(&local_48);
    param_3 = (**(code **)(lVar5 + 0x1c0))(D2_GameSingletonPtr,*(undefined8 *)(lVar1 + 0x80),uVar6);
    if (param_3 == 0) {
      return;
    }
    FUN_14087c320(param_3,puVar2 + 0x60);
    FUN_14087c2c0(param_3,(longlong)puVar2 + 0x30c);
    if (*(longlong *)(param_1 + 0x10) == 0) {
      *(undefined4 *)(param_3 + 0xac) = 0x3fff0010;
    }
    else {
      *(undefined4 *)(param_3 + 0xac) = *(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0xa8);
    }
    FUN_14087c360(param_3,*(longlong *)(param_1 + 0x10) + 0xc0);
    FUN_14087c350(param_3);
    if (bVar3) {
      *(byte *)(param_3 + 0xd4) = *(byte *)(param_3 + 0xd4) | 8;
    }
    else {
      *(byte *)(param_3 + 0xd4) = *(byte *)(param_3 + 0xd4) | 4;
    }
    FUN_1408aecc0(D2_GameSingletonPtr,param_3,0xffffffff);
  }
  lVar5 = FUN_140877720(param_3,&DAT_1439cb440,0);
  if (lVar5 == 0) {
    puVar7 = &DAT_141b9aa60;
  }
  else {
    if ((*(int *)(*(longlong *)(lVar5 + 8) + 0x11c) != 0) &&
       (*(int *)(*(longlong *)(lVar5 + 8) + 0x68) < 0x10)) {
      local_res10 = param_2;
      FUN_1408918f0(param_1 + 0x58,&local_res10);
      FUN_14063cb90(param_2,param_3);
      if (!bVar3) {
        return;
      }
      lVar5 = FUN_1408e7bb0(lVar5);
      if (lVar5 == 0) {
        return;
      }
      lVar5 = *(longlong *)(lVar5 + 8);
      uVar4 = *(uint *)(lVar5 + 400) & 0xffffff;
      lVar9 = (longlong)(int)uVar4;
      if (uVar4 == 0) {
        return;
      }
      lVar8 = 0;
      local_48 = lVar1;
      do {
        local_40 = *(undefined8 *)(lVar8 + *(longlong *)(lVar5 + 0x188));
        FUN_14030c670(param_1 + 0x28,&local_48);
        lVar8 = lVar8 + 0x70;
        lVar9 = lVar9 + -1;
      } while (lVar9 != 0);
      return;
    }
    puVar7 = &DAT_141b9aa10;
  }
                    /* WARNING: Subroutine does not return */
  FUN_140166830(puVar7,*(undefined8 *)(*(longlong *)(lVar1 + 0x80) + 8),*(undefined8 *)(lVar1 + 8));
}


````

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

undefined8 *
FUN_140446f20(undefined8 *param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  undefined8 uVar1;
  
  uVar1 = 0xfffffffffffffffe;
  FUN_14061ba50();
  *param_1 = &PTR_FUN_141b52d88;
  param_1[0x33] = 0;
  FUN_140131f90(param_1 + 0x34,&DAT_141ab2848,0,param_4,uVar1);
  param_1[0x35] = 0;
  *(undefined4 *)(param_1 + 0x36) = 0;
  *(undefined4 *)(param_1 + 0x32) = 0xc;
  return param_1;
}


````
