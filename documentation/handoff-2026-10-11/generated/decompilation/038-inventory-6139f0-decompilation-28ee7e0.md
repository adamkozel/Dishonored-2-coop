# inventory-6139f0-decompilation.json

Original: [outputs/d2-coop-lab/analysis/ghidra/inventory-6139f0-decompilation.json](<../../../../outputs/d2-coop-lab/analysis/ghidra/inventory-6139f0-decompilation.json>)

SHA-256: `13511295C3F77EC4D1931FF83298D30D924C16B8AB40AF0286EB903E4B61A155`; 1863 bytes.

## Original saved record

````json
{
  "meta": null,
  "content": [
    {
      "type": "text",
      "text": "\r\n/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */\r\n\r\nvoid FUN_1406139f0(longlong param_1,undefined8 *param_2)\r\n\r\n{\r\n  undefined4 uVar1;\r\n  undefined8 uVar2;\r\n  longlong lVar3;\r\n  undefined8 *puVar4;\r\n  \r\n  if (*(longlong *)(param_1 + 0x20) == 0) {\r\n    puVar4 = *(undefined8 **)(param_1 + 0x10);\r\n    lVar3 = (**(code **)*puVar4)(puVar4);\r\n    if ((_DAT_1439ce350 <= *(int *)(lVar3 + 0x60)) && (*(int *)(lVar3 + 0x60) <= _DAT_1439ce354)) {\r\n      uVar2 = *param_2;\r\n      uVar1 = *(undefined4 *)(puVar4 + 0x15);\r\n      lVar3 = func_0x000140810240();\r\n      puVar4 = (undefined8 *)FUN_1408112c0(lVar3 + 0xc8e8,0x16a,8,uVar1);\r\n      if (puVar4 != (undefined8 *)0x0) {\r\n        *puVar4 = uVar2;\r\n      }\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n",
      "annotations": null,
      "meta": null
    }
  ],
  "structuredContent": {
    "result": "\r\n/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */\r\n\r\nvoid FUN_1406139f0(longlong param_1,undefined8 *param_2)\r\n\r\n{\r\n  undefined4 uVar1;\r\n  undefined8 uVar2;\r\n  longlong lVar3;\r\n  undefined8 *puVar4;\r\n  \r\n  if (*(longlong *)(param_1 + 0x20) == 0) {\r\n    puVar4 = *(undefined8 **)(param_1 + 0x10);\r\n    lVar3 = (**(code **)*puVar4)(puVar4);\r\n    if ((_DAT_1439ce350 <= *(int *)(lVar3 + 0x60)) && (*(int *)(lVar3 + 0x60) <= _DAT_1439ce354)) {\r\n      uVar2 = *param_2;\r\n      uVar1 = *(undefined4 *)(puVar4 + 0x15);\r\n      lVar3 = func_0x000140810240();\r\n      puVar4 = (undefined8 *)FUN_1408112c0(lVar3 + 0xc8e8,0x16a,8,uVar1);\r\n      if (puVar4 != (undefined8 *)0x0) {\r\n        *puVar4 = uVar2;\r\n      }\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
  },
  "isError": false
}

````

## Unescaped text fields for reading

### text

````text

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_1406139f0(longlong param_1,undefined8 *param_2)

{
  undefined4 uVar1;
  undefined8 uVar2;
  longlong lVar3;
  undefined8 *puVar4;
  
  if (*(longlong *)(param_1 + 0x20) == 0) {
    puVar4 = *(undefined8 **)(param_1 + 0x10);
    lVar3 = (**(code **)*puVar4)(puVar4);
    if ((_DAT_1439ce350 <= *(int *)(lVar3 + 0x60)) && (*(int *)(lVar3 + 0x60) <= _DAT_1439ce354)) {
      uVar2 = *param_2;
      uVar1 = *(undefined4 *)(puVar4 + 0x15);
      lVar3 = func_0x000140810240();
      puVar4 = (undefined8 *)FUN_1408112c0(lVar3 + 0xc8e8,0x16a,8,uVar1);
      if (puVar4 != (undefined8 *)0x0) {
        *puVar4 = uVar2;
      }
    }
  }
  return;
}


````
