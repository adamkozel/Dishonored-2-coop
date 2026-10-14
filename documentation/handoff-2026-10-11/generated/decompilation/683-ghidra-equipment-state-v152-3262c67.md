# ghidra-equipment-state-v152.json

Original: [work/ghidra-equipment-state-v152.json](<../../../../work/ghidra-equipment-state-v152.json>)

SHA-256: `51ECBE1DC36AE79E31438C3FDD58458071DBFC138FDB676A9E6F29C57F7B9074`; 5661 bytes.

## Original saved record

````json
[
  {
    "status": "fulfilled",
    "value": {
      "address": "140853d40",
      "content": [
        {
          "type": "text",
          "text": "\r\nvoid FUN_140853d40(longlong param_1)\r\n\r\n{\r\n  longlong lVar1;\r\n  undefined8 uVar2;\r\n  longlong lVar3;\r\n  undefined1 local_58 [8];\r\n  undefined4 local_50;\r\n  undefined1 local_40 [8];\r\n  undefined4 local_38;\r\n  undefined1 local_28 [32];\r\n  \r\n  lVar1 = *(longlong *)(param_1 + 0x18);\r\n  if (*(char *)(param_1 + 0x20) == '\\0') {\r\n    *(undefined1 *)(param_1 + 0x20) = 1;\r\n    uVar2 = *(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x10);\r\n    local_38 = *(undefined4 *)(param_1 + 8);\r\n    local_58[0] = 0x69;\r\n    local_50 = 1;\r\n    local_40[0] = 0x69;\r\n    FUN_1408848a0(uVar2,local_28,&DAT_1439ab5d0,local_40,local_58);\r\n    FUN_140882fc0(local_28);\r\n    if ((*(byte *)(lVar1 + 0x65) & 0x10) != 0) {\r\n      FUN_140774900(*(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x300),1);\r\n    }\r\n    lVar3 = *(longlong *)(param_1 + 0x10);\r\n    if ((*(byte *)(lVar1 + 0x69) & 0x40) != 0) {\r\n      FUN_1407f0310(*(undefined8 *)(lVar3 + 0x310),1);\r\n    }\r\n    if ((*(byte *)(lVar1 + 0x6a) & 1) != 0) {\r\n      FUN_1407fb080(*(undefined8 *)(lVar3 + 0x2f8));\r\n    }\r\n    if ((*(byte *)(lVar1 + 0x6a) & 2) != 0) {\r\n      FUN_140784bf0(uVar2,1);\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
        }
      ],
      "structuredContent": {
        "result": "\r\nvoid FUN_140853d40(longlong param_1)\r\n\r\n{\r\n  longlong lVar1;\r\n  undefined8 uVar2;\r\n  longlong lVar3;\r\n  undefined1 local_58 [8];\r\n  undefined4 local_50;\r\n  undefined1 local_40 [8];\r\n  undefined4 local_38;\r\n  undefined1 local_28 [32];\r\n  \r\n  lVar1 = *(longlong *)(param_1 + 0x18);\r\n  if (*(char *)(param_1 + 0x20) == '\\0') {\r\n    *(undefined1 *)(param_1 + 0x20) = 1;\r\n    uVar2 = *(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x10);\r\n    local_38 = *(undefined4 *)(param_1 + 8);\r\n    local_58[0] = 0x69;\r\n    local_50 = 1;\r\n    local_40[0] = 0x69;\r\n    FUN_1408848a0(uVar2,local_28,&DAT_1439ab5d0,local_40,local_58);\r\n    FUN_140882fc0(local_28);\r\n    if ((*(byte *)(lVar1 + 0x65) & 0x10) != 0) {\r\n      FUN_140774900(*(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x300),1);\r\n    }\r\n    lVar3 = *(longlong *)(param_1 + 0x10);\r\n    if ((*(byte *)(lVar1 + 0x69) & 0x40) != 0) {\r\n      FUN_1407f0310(*(undefined8 *)(lVar3 + 0x310),1);\r\n    }\r\n    if ((*(byte *)(lVar1 + 0x6a) & 1) != 0) {\r\n      FUN_1407fb080(*(undefined8 *)(lVar3 + 0x2f8));\r\n    }\r\n    if ((*(byte *)(lVar1 + 0x6a) & 2) != 0) {\r\n      FUN_140784bf0(uVar2,1);\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
      },
      "isError": false
    }
  },
  {
    "status": "fulfilled",
    "value": {
      "address": "1408541a0",
      "content": [
        {
          "type": "text",
          "text": "\r\nvoid FUN_1408541a0(longlong param_1)\r\n\r\n{\r\n  undefined8 uVar1;\r\n  longlong lVar2;\r\n  longlong lVar3;\r\n  undefined1 local_58 [8];\r\n  undefined4 local_50;\r\n  undefined1 local_40 [8];\r\n  undefined4 local_38;\r\n  undefined1 local_28 [32];\r\n  \r\n  *(undefined1 *)(param_1 + 0x20) = 0;\r\n  uVar1 = *(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x10);\r\n  local_38 = *(undefined4 *)(param_1 + 8);\r\n  local_58[0] = 0x69;\r\n  local_50 = 0;\r\n  local_40[0] = 0x69;\r\n  FUN_1408848a0(uVar1,local_28,&DAT_1439ab5d0,local_40,local_58);\r\n  FUN_140882fc0(local_28);\r\n  lVar2 = *(longlong *)(param_1 + 0x18);\r\n  if ((*(byte *)(lVar2 + 0x65) & 0x10) != 0) {\r\n    FUN_140774900(*(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x300),0);\r\n  }\r\n  lVar3 = *(longlong *)(param_1 + 0x10);\r\n  if ((*(byte *)(lVar2 + 0x69) & 0x40) != 0) {\r\n    FUN_1407f0310(*(undefined8 *)(lVar3 + 0x310),0);\r\n  }\r\n  if ((*(byte *)(lVar2 + 0x6a) & 1) != 0) {\r\n    FUN_1407fb2b0(*(undefined8 *)(lVar3 + 0x2f8));\r\n  }\r\n  if ((*(byte *)(lVar2 + 0x6a) & 2) != 0) {\r\n    FUN_140784bf0(uVar1,0);\r\n  }\r\n  return;\r\n}\r\n\r\n"
        }
      ],
      "structuredContent": {
        "result": "\r\nvoid FUN_1408541a0(longlong param_1)\r\n\r\n{\r\n  undefined8 uVar1;\r\n  longlong lVar2;\r\n  longlong lVar3;\r\n  undefined1 local_58 [8];\r\n  undefined4 local_50;\r\n  undefined1 local_40 [8];\r\n  undefined4 local_38;\r\n  undefined1 local_28 [32];\r\n  \r\n  *(undefined1 *)(param_1 + 0x20) = 0;\r\n  uVar1 = *(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x10);\r\n  local_38 = *(undefined4 *)(param_1 + 8);\r\n  local_58[0] = 0x69;\r\n  local_50 = 0;\r\n  local_40[0] = 0x69;\r\n  FUN_1408848a0(uVar1,local_28,&DAT_1439ab5d0,local_40,local_58);\r\n  FUN_140882fc0(local_28);\r\n  lVar2 = *(longlong *)(param_1 + 0x18);\r\n  if ((*(byte *)(lVar2 + 0x65) & 0x10) != 0) {\r\n    FUN_140774900(*(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x300),0);\r\n  }\r\n  lVar3 = *(longlong *)(param_1 + 0x10);\r\n  if ((*(byte *)(lVar2 + 0x69) & 0x40) != 0) {\r\n    FUN_1407f0310(*(undefined8 *)(lVar3 + 0x310),0);\r\n  }\r\n  if ((*(byte *)(lVar2 + 0x6a) & 1) != 0) {\r\n    FUN_1407fb2b0(*(undefined8 *)(lVar3 + 0x2f8));\r\n  }\r\n  if ((*(byte *)(lVar2 + 0x6a) & 2) != 0) {\r\n    FUN_140784bf0(uVar1,0);\r\n  }\r\n  return;\r\n}\r\n\r\n"
      },
      "isError": false
    }
  },
  {
    "status": "fulfilled",
    "value": {
      "address": "140854940",
      "content": [
        {
          "type": "text",
          "text": "{\"error\":\"No function found for 0x140854940\"}"
        }
      ],
      "structuredContent": {
        "result": "{\"error\":\"No function found for 0x140854940\"}"
      },
      "isError": false
    }
  }
]

````

## Unescaped text fields for reading

### text

````text

void FUN_140853d40(longlong param_1)

{
  longlong lVar1;
  undefined8 uVar2;
  longlong lVar3;
  undefined1 local_58 [8];
  undefined4 local_50;
  undefined1 local_40 [8];
  undefined4 local_38;
  undefined1 local_28 [32];
  
  lVar1 = *(longlong *)(param_1 + 0x18);
  if (*(char *)(param_1 + 0x20) == '\0') {
    *(undefined1 *)(param_1 + 0x20) = 1;
    uVar2 = *(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x10);
    local_38 = *(undefined4 *)(param_1 + 8);
    local_58[0] = 0x69;
    local_50 = 1;
    local_40[0] = 0x69;
    FUN_1408848a0(uVar2,local_28,&DAT_1439ab5d0,local_40,local_58);
    FUN_140882fc0(local_28);
    if ((*(byte *)(lVar1 + 0x65) & 0x10) != 0) {
      FUN_140774900(*(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x300),1);
    }
    lVar3 = *(longlong *)(param_1 + 0x10);
    if ((*(byte *)(lVar1 + 0x69) & 0x40) != 0) {
      FUN_1407f0310(*(undefined8 *)(lVar3 + 0x310),1);
    }
    if ((*(byte *)(lVar1 + 0x6a) & 1) != 0) {
      FUN_1407fb080(*(undefined8 *)(lVar3 + 0x2f8));
    }
    if ((*(byte *)(lVar1 + 0x6a) & 2) != 0) {
      FUN_140784bf0(uVar2,1);
    }
  }
  return;
}


````

### text

````text

void FUN_1408541a0(longlong param_1)

{
  undefined8 uVar1;
  longlong lVar2;
  longlong lVar3;
  undefined1 local_58 [8];
  undefined4 local_50;
  undefined1 local_40 [8];
  undefined4 local_38;
  undefined1 local_28 [32];
  
  *(undefined1 *)(param_1 + 0x20) = 0;
  uVar1 = *(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x10);
  local_38 = *(undefined4 *)(param_1 + 8);
  local_58[0] = 0x69;
  local_50 = 0;
  local_40[0] = 0x69;
  FUN_1408848a0(uVar1,local_28,&DAT_1439ab5d0,local_40,local_58);
  FUN_140882fc0(local_28);
  lVar2 = *(longlong *)(param_1 + 0x18);
  if ((*(byte *)(lVar2 + 0x65) & 0x10) != 0) {
    FUN_140774900(*(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x300),0);
  }
  lVar3 = *(longlong *)(param_1 + 0x10);
  if ((*(byte *)(lVar2 + 0x69) & 0x40) != 0) {
    FUN_1407f0310(*(undefined8 *)(lVar3 + 0x310),0);
  }
  if ((*(byte *)(lVar2 + 0x6a) & 1) != 0) {
    FUN_1407fb2b0(*(undefined8 *)(lVar3 + 0x2f8));
  }
  if ((*(byte *)(lVar2 + 0x6a) & 2) != 0) {
    FUN_140784bf0(uVar1,0);
  }
  return;
}


````
