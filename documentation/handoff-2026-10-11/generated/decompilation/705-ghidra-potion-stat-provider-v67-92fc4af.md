# ghidra-potion-stat-provider-v67.json

Original: [work/ghidra-potion-stat-provider-v67.json](<../../../../work/ghidra-potion-stat-provider-v67.json>)

SHA-256: `CA7933FC6CCC72069EEEE3584944E99394E8774D9657513D1D31A72CCE89D480`; 2089 bytes.

## Original saved record

````json
{
  "program": "Dishonored2.exe",
  "address": "14067bec0",
  "result": {
    "content": [
      {
        "type": "text",
        "text": "\r\nulonglong FUN_14067bec0(longlong param_1)\r\n\r\n{\r\n  int iVar1;\r\n  longlong lVar2;\r\n  int *piVar3;\r\n  ulonglong uVar4;\r\n  int local_res10 [6];\r\n  \r\n  iVar1 = (**(code **)(*D2_GameSingletonPtr + 0x90))();\r\n  if (iVar1 == 3) {\r\n    lVar2 = FUN_1403da910(param_1 + 0x10,2);\r\n    if (lVar2 != 0) {\r\n      piVar3 = (int *)FUN_14092e460(lVar2);\r\n      local_res10[0] = *piVar3;\r\n      if (local_res10[0] == 0) {\r\n        piVar3 = (int *)FUN_140724160(lVar2);\r\n        local_res10[0] = *piVar3;\r\n      }\r\n      uVar4 = FUN_140723df0(lVar2,local_res10);\r\n      return uVar4;\r\n    }\r\n  }\r\n  else {\r\n    if (0 < (int)*(uint *)(param_1 + 0x208)) {\r\n      return (ulonglong)*(uint *)(param_1 + 0x208);\r\n    }\r\n    if (D2_GameSingletonPtr[0x66e23] != 0) {\r\n      return (ulonglong)*(uint *)(D2_GameSingletonPtr[0x66e23] + 0x20c);\r\n    }\r\n  }\r\n  return 0xffffffff;\r\n}\r\n\r\n"
      }
    ],
    "structuredContent": {
      "result": "\r\nulonglong FUN_14067bec0(longlong param_1)\r\n\r\n{\r\n  int iVar1;\r\n  longlong lVar2;\r\n  int *piVar3;\r\n  ulonglong uVar4;\r\n  int local_res10 [6];\r\n  \r\n  iVar1 = (**(code **)(*D2_GameSingletonPtr + 0x90))();\r\n  if (iVar1 == 3) {\r\n    lVar2 = FUN_1403da910(param_1 + 0x10,2);\r\n    if (lVar2 != 0) {\r\n      piVar3 = (int *)FUN_14092e460(lVar2);\r\n      local_res10[0] = *piVar3;\r\n      if (local_res10[0] == 0) {\r\n        piVar3 = (int *)FUN_140724160(lVar2);\r\n        local_res10[0] = *piVar3;\r\n      }\r\n      uVar4 = FUN_140723df0(lVar2,local_res10);\r\n      return uVar4;\r\n    }\r\n  }\r\n  else {\r\n    if (0 < (int)*(uint *)(param_1 + 0x208)) {\r\n      return (ulonglong)*(uint *)(param_1 + 0x208);\r\n    }\r\n    if (D2_GameSingletonPtr[0x66e23] != 0) {\r\n      return (ulonglong)*(uint *)(D2_GameSingletonPtr[0x66e23] + 0x20c);\r\n    }\r\n  }\r\n  return 0xffffffff;\r\n}\r\n\r\n"
    },
    "isError": false
  }
}

````

## Unescaped text fields for reading

### text

````text

ulonglong FUN_14067bec0(longlong param_1)

{
  int iVar1;
  longlong lVar2;
  int *piVar3;
  ulonglong uVar4;
  int local_res10 [6];
  
  iVar1 = (**(code **)(*D2_GameSingletonPtr + 0x90))();
  if (iVar1 == 3) {
    lVar2 = FUN_1403da910(param_1 + 0x10,2);
    if (lVar2 != 0) {
      piVar3 = (int *)FUN_14092e460(lVar2);
      local_res10[0] = *piVar3;
      if (local_res10[0] == 0) {
        piVar3 = (int *)FUN_140724160(lVar2);
        local_res10[0] = *piVar3;
      }
      uVar4 = FUN_140723df0(lVar2,local_res10);
      return uVar4;
    }
  }
  else {
    if (0 < (int)*(uint *)(param_1 + 0x208)) {
      return (ulonglong)*(uint *)(param_1 + 0x208);
    }
    if (D2_GameSingletonPtr[0x66e23] != 0) {
      return (ulonglong)*(uint *)(D2_GameSingletonPtr[0x66e23] + 0x20c);
    }
  }
  return 0xffffffff;
}


````
