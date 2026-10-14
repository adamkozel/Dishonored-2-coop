# native-throw-140832b10-decompilation-v2.json

Original: [work/natural-moveset/carry-research/native-throw-140832b10-decompilation-v2.json](<../../../../work/natural-moveset/carry-research/native-throw-140832b10-decompilation-v2.json>)

SHA-256: `E439D32A226D7CC6E4EFEA8ADD4C45AAB6A8EE5A219249F60901E30540C65B1F`; 875 bytes.

## Original saved record

````json
{
  "content": [
    {
      "type": "text",
      "text": "\r\nvoid FUN_140832b10(longlong param_1,longlong param_2)\r\n\r\n{\r\n  longlong lVar1;\r\n  \r\n  if (*(int *)(param_2 + 0x10) == *(int *)(param_1 + 0x38)) {\r\n    lVar1 = FUN_1406dc560(*(int *)(param_1 + 0x38));\r\n    if ((lVar1 != 0) && (*(int *)(*(longlong *)(param_2 + 8) + 8) == 0)) {\r\n      FUN_140832290(param_1);\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
    }
  ],
  "structuredContent": {
    "result": "\r\nvoid FUN_140832b10(longlong param_1,longlong param_2)\r\n\r\n{\r\n  longlong lVar1;\r\n  \r\n  if (*(int *)(param_2 + 0x10) == *(int *)(param_1 + 0x38)) {\r\n    lVar1 = FUN_1406dc560(*(int *)(param_1 + 0x38));\r\n    if ((lVar1 != 0) && (*(int *)(*(longlong *)(param_2 + 8) + 8) == 0)) {\r\n      FUN_140832290(param_1);\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
  },
  "isError": false
}

````

## Unescaped text fields for reading

### text

````text

void FUN_140832b10(longlong param_1,longlong param_2)

{
  longlong lVar1;
  
  if (*(int *)(param_2 + 0x10) == *(int *)(param_1 + 0x38)) {
    lVar1 = FUN_1406dc560(*(int *)(param_1 + 0x38));
    if ((lVar1 != 0) && (*(int *)(*(longlong *)(param_2 + 8) + 8) == 0)) {
      FUN_140832290(param_1);
    }
  }
  return;
}


````
