# native-throw-1408112c0-decompilation-v31.json

Original: [work/natural-moveset/carry-research/native-throw-1408112c0-decompilation-v31.json](<../../../../work/natural-moveset/carry-research/native-throw-1408112c0-decompilation-v31.json>)

SHA-256: `4D69931383EE3E7D829778842EAEB27D6743A0EF8DD7FEE27B28BA2244FE6C21`; 1161 bytes.

## Original saved record

````json
{
  "content": [
    {
      "type": "text",
      "text": "\r\nundefined2 * FUN_1408112c0(longlong param_1,undefined2 param_2,int param_3,undefined4 param_4)\r\n\r\n{\r\n  undefined2 *puVar1;\r\n  undefined2 *puVar2;\r\n  \r\n  FUN_140158200(param_1 + 0x28);\r\n  puVar1 = (undefined2 *)FUN_1400fa4f0(param_1,param_3 + 8);\r\n  if (param_3 < 1) {\r\n    puVar2 = (undefined2 *)0x0;\r\n  }\r\n  else {\r\n    puVar2 = puVar1 + 4;\r\n  }\r\n  *puVar1 = param_2;\r\n  puVar1[1] = (short)param_3;\r\n  *(undefined4 *)(puVar1 + 2) = param_4;\r\n  return puVar2;\r\n}\r\n\r\n"
    }
  ],
  "structuredContent": {
    "result": "\r\nundefined2 * FUN_1408112c0(longlong param_1,undefined2 param_2,int param_3,undefined4 param_4)\r\n\r\n{\r\n  undefined2 *puVar1;\r\n  undefined2 *puVar2;\r\n  \r\n  FUN_140158200(param_1 + 0x28);\r\n  puVar1 = (undefined2 *)FUN_1400fa4f0(param_1,param_3 + 8);\r\n  if (param_3 < 1) {\r\n    puVar2 = (undefined2 *)0x0;\r\n  }\r\n  else {\r\n    puVar2 = puVar1 + 4;\r\n  }\r\n  *puVar1 = param_2;\r\n  puVar1[1] = (short)param_3;\r\n  *(undefined4 *)(puVar1 + 2) = param_4;\r\n  return puVar2;\r\n}\r\n\r\n"
  },
  "isError": false
}

````

## Unescaped text fields for reading

### text

````text

undefined2 * FUN_1408112c0(longlong param_1,undefined2 param_2,int param_3,undefined4 param_4)

{
  undefined2 *puVar1;
  undefined2 *puVar2;
  
  FUN_140158200(param_1 + 0x28);
  puVar1 = (undefined2 *)FUN_1400fa4f0(param_1,param_3 + 8);
  if (param_3 < 1) {
    puVar2 = (undefined2 *)0x0;
  }
  else {
    puVar2 = puVar1 + 4;
  }
  *puVar1 = param_2;
  puVar1[1] = (short)param_3;
  *(undefined4 *)(puVar1 + 2) = param_4;
  return puVar2;
}


````
