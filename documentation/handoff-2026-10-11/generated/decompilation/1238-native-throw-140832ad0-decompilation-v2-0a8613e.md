# native-throw-140832ad0-decompilation-v2.json

Original: [work/natural-moveset/carry-research/native-throw-140832ad0-decompilation-v2.json](<../../../../work/natural-moveset/carry-research/native-throw-140832ad0-decompilation-v2.json>)

SHA-256: `C89E4BDC09F2CC33B2ED8D99094EE0308AB6FC6429C6E2FBA8998F9126136875`; 861 bytes.

## Original saved record

````json
{
  "content": [
    {
      "type": "text",
      "text": "\r\nvoid FUN_140832ad0(longlong param_1,longlong param_2)\r\n\r\n{\r\n  longlong lVar1;\r\n  \r\n  if (*(int *)(param_2 + 0x10) == *(int *)(param_1 + 0x38)) {\r\n    lVar1 = FUN_1406dc560(*(int *)(param_1 + 0x38));\r\n    if (lVar1 != 0) {\r\n      FUN_14080e050(D2_GameSingletonPtr + 0x1f52e8,lVar1);\r\n      return;\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
    }
  ],
  "structuredContent": {
    "result": "\r\nvoid FUN_140832ad0(longlong param_1,longlong param_2)\r\n\r\n{\r\n  longlong lVar1;\r\n  \r\n  if (*(int *)(param_2 + 0x10) == *(int *)(param_1 + 0x38)) {\r\n    lVar1 = FUN_1406dc560(*(int *)(param_1 + 0x38));\r\n    if (lVar1 != 0) {\r\n      FUN_14080e050(D2_GameSingletonPtr + 0x1f52e8,lVar1);\r\n      return;\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
  },
  "isError": false
}

````

## Unescaped text fields for reading

### text

````text

void FUN_140832ad0(longlong param_1,longlong param_2)

{
  longlong lVar1;
  
  if (*(int *)(param_2 + 0x10) == *(int *)(param_1 + 0x38)) {
    lVar1 = FUN_1406dc560(*(int *)(param_1 + 0x38));
    if (lVar1 != 0) {
      FUN_14080e050(D2_GameSingletonPtr + 0x1f52e8,lVar1);
      return;
    }
  }
  return;
}


````
