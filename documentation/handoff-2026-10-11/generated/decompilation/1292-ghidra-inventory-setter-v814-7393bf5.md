# ghidra-inventory-setter-v814.json

Original: [work/natural-moveset/native-pickup-research-v785/ghidra-inventory-setter-v814.json](<../../../../work/natural-moveset/native-pickup-research-v785/ghidra-inventory-setter-v814.json>)

SHA-256: `7A28506EFDBC36DF7948173EAA203953BA9CCB9A65533180BD5191D7A01413D3`; 2362 bytes.

## Original saved record

````json
{"content":[{"type":"text","text":"\r\nundefined4 FUN_140618de0(longlong param_1,undefined8 param_2,undefined4 param_3)\r\n\r\n{\r\n  undefined4 uVar1;\r\n  int iVar2;\r\n  undefined4 uVar3;\r\n  longlong lVar4;\r\n  undefined **local_res8;\r\n  undefined8 local_40;\r\n  undefined4 local_38;\r\n  undefined8 local_30;\r\n  undefined8 local_28;\r\n  undefined8 local_20;\r\n  undefined1 local_18;\r\n  undefined4 local_14;\r\n  undefined1 local_b;\r\n  \r\n  local_res8 = &PTR_FUN_141b9a840;\r\n  local_38 = 0x3fff0010;\r\n  local_30 = 0;\r\n  local_28 = 0;\r\n  local_20 = 0;\r\n  local_18 = 0;\r\n  local_14 = 0;\r\n  local_b = 1;\r\n  local_40 = param_2;\r\n  iVar2 = FUN_140618af0(&local_res8,*(undefined8 *)(param_1 + 0x38),\r\n                        *(uint *)(param_1 + 0x40) & 0xffffff,&local_40,0xfffffffffffffffe);\r\n  local_res8 = &PTR_FUN_141b9a808;\r\n  if (iVar2 < 0) {\r\n    uVar3 = 0;\r\n  }\r\n  else {\r\n    lVar4 = (longlong)iVar2 * 0x38 + *(longlong *)(param_1 + 0x38);\r\n    uVar1 = *(undefined4 *)(lVar4 + 0x20);\r\n    uVar3 = FUN_14063cc00(lVar4,param_3);\r\n    FUN_1406170b0(param_1,lVar4,uVar1,uVar3);\r\n  }\r\n  return uVar3;\r\n}\r\n\r\n"}],"structuredContent":{"result":"\r\nundefined4 FUN_140618de0(longlong param_1,undefined8 param_2,undefined4 param_3)\r\n\r\n{\r\n  undefined4 uVar1;\r\n  int iVar2;\r\n  undefined4 uVar3;\r\n  longlong lVar4;\r\n  undefined **local_res8;\r\n  undefined8 local_40;\r\n  undefined4 local_38;\r\n  undefined8 local_30;\r\n  undefined8 local_28;\r\n  undefined8 local_20;\r\n  undefined1 local_18;\r\n  undefined4 local_14;\r\n  undefined1 local_b;\r\n  \r\n  local_res8 = &PTR_FUN_141b9a840;\r\n  local_38 = 0x3fff0010;\r\n  local_30 = 0;\r\n  local_28 = 0;\r\n  local_20 = 0;\r\n  local_18 = 0;\r\n  local_14 = 0;\r\n  local_b = 1;\r\n  local_40 = param_2;\r\n  iVar2 = FUN_140618af0(&local_res8,*(undefined8 *)(param_1 + 0x38),\r\n                        *(uint *)(param_1 + 0x40) & 0xffffff,&local_40,0xfffffffffffffffe);\r\n  local_res8 = &PTR_FUN_141b9a808;\r\n  if (iVar2 < 0) {\r\n    uVar3 = 0;\r\n  }\r\n  else {\r\n    lVar4 = (longlong)iVar2 * 0x38 + *(longlong *)(param_1 + 0x38);\r\n    uVar1 = *(undefined4 *)(lVar4 + 0x20);\r\n    uVar3 = FUN_14063cc00(lVar4,param_3);\r\n    FUN_1406170b0(param_1,lVar4,uVar1,uVar3);\r\n  }\r\n  return uVar3;\r\n}\r\n\r\n"},"isError":false}

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
