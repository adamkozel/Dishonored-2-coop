# native-throw-140727100-decompilation-defined-v31.json

Original: [work/natural-moveset/carry-research/native-throw-140727100-decompilation-defined-v31.json](<../../../../work/natural-moveset/carry-research/native-throw-140727100-decompilation-defined-v31.json>)

SHA-256: `C8717272443440F28547E4ECBEA1FA08FCD94B3660A9EDC08BFCA60C19C4C643`; 3879 bytes.

## Original saved record

````json
{
  "content": [
    {
      "type": "text",
      "text": "\r\nvoid FUN_140727100(longlong *param_1,longlong param_2)\r\n\r\n{\r\n  byte bVar1;\r\n  undefined1 uVar2;\r\n  float *pfVar3;\r\n  \r\n  pfVar3 = *(float **)(param_2 + 8);\r\n  *(undefined1 *)((longlong)param_1 + 0xb2) = 1;\r\n  if (*(char *)((longlong)pfVar3 + 0x19) != '\\0') {\r\n    *(float *)(param_1 + 0x13) = *pfVar3 + *(float *)(param_1 + 0x13);\r\n    *(float *)((longlong)param_1 + 0x9c) = pfVar3[1] + *(float *)((longlong)param_1 + 0x9c);\r\n    *(float *)(param_1 + 0x14) = pfVar3[2] + *(float *)(param_1 + 0x14);\r\n    *(float *)((longlong)param_1 + 0xa4) = *(float *)((longlong)param_1 + 0xa4) + pfVar3[3];\r\n    *(float *)(param_1 + 0x15) = pfVar3[4] + *(float *)(param_1 + 0x15);\r\n    *(float *)((longlong)param_1 + 0xac) = pfVar3[5] + *(float *)((longlong)param_1 + 0xac);\r\n    bVar1 = *(byte *)(pfVar3 + 6);\r\n    *(undefined1 *)((longlong)param_1 + 0xb1) = 1;\r\n    *(byte *)(param_1 + 0x16) = *(byte *)(param_1 + 0x16) | bVar1;\r\n                    /* WARNING: Could not recover jumptable at 0x0001407271ae. Too many branches */\r\n                    /* WARNING: Treating indirect jump as call */\r\n    (**(code **)(*param_1 + 0x130))();\r\n    return;\r\n  }\r\n  *(float *)(param_1 + 0x13) = *pfVar3;\r\n  *(float *)((longlong)param_1 + 0x9c) = pfVar3[1];\r\n  *(float *)(param_1 + 0x14) = pfVar3[2];\r\n  *(float *)((longlong)param_1 + 0xa4) = pfVar3[3];\r\n  *(float *)(param_1 + 0x15) = pfVar3[4];\r\n  *(float *)((longlong)param_1 + 0xac) = pfVar3[5];\r\n  uVar2 = *(undefined1 *)(pfVar3 + 6);\r\n  *(undefined1 *)((longlong)param_1 + 0xb1) = 0;\r\n  *(undefined1 *)(param_1 + 0x16) = uVar2;\r\n                    /* WARNING: Could not recover jumptable at 0x000140727205. Too many branches */\r\n                    /* WARNING: Treating indirect jump as call */\r\n  (**(code **)(*param_1 + 0x130))();\r\n  return;\r\n}\r\n\r\n"
    }
  ],
  "structuredContent": {
    "result": "\r\nvoid FUN_140727100(longlong *param_1,longlong param_2)\r\n\r\n{\r\n  byte bVar1;\r\n  undefined1 uVar2;\r\n  float *pfVar3;\r\n  \r\n  pfVar3 = *(float **)(param_2 + 8);\r\n  *(undefined1 *)((longlong)param_1 + 0xb2) = 1;\r\n  if (*(char *)((longlong)pfVar3 + 0x19) != '\\0') {\r\n    *(float *)(param_1 + 0x13) = *pfVar3 + *(float *)(param_1 + 0x13);\r\n    *(float *)((longlong)param_1 + 0x9c) = pfVar3[1] + *(float *)((longlong)param_1 + 0x9c);\r\n    *(float *)(param_1 + 0x14) = pfVar3[2] + *(float *)(param_1 + 0x14);\r\n    *(float *)((longlong)param_1 + 0xa4) = *(float *)((longlong)param_1 + 0xa4) + pfVar3[3];\r\n    *(float *)(param_1 + 0x15) = pfVar3[4] + *(float *)(param_1 + 0x15);\r\n    *(float *)((longlong)param_1 + 0xac) = pfVar3[5] + *(float *)((longlong)param_1 + 0xac);\r\n    bVar1 = *(byte *)(pfVar3 + 6);\r\n    *(undefined1 *)((longlong)param_1 + 0xb1) = 1;\r\n    *(byte *)(param_1 + 0x16) = *(byte *)(param_1 + 0x16) | bVar1;\r\n                    /* WARNING: Could not recover jumptable at 0x0001407271ae. Too many branches */\r\n                    /* WARNING: Treating indirect jump as call */\r\n    (**(code **)(*param_1 + 0x130))();\r\n    return;\r\n  }\r\n  *(float *)(param_1 + 0x13) = *pfVar3;\r\n  *(float *)((longlong)param_1 + 0x9c) = pfVar3[1];\r\n  *(float *)(param_1 + 0x14) = pfVar3[2];\r\n  *(float *)((longlong)param_1 + 0xa4) = pfVar3[3];\r\n  *(float *)(param_1 + 0x15) = pfVar3[4];\r\n  *(float *)((longlong)param_1 + 0xac) = pfVar3[5];\r\n  uVar2 = *(undefined1 *)(pfVar3 + 6);\r\n  *(undefined1 *)((longlong)param_1 + 0xb1) = 0;\r\n  *(undefined1 *)(param_1 + 0x16) = uVar2;\r\n                    /* WARNING: Could not recover jumptable at 0x000140727205. Too many branches */\r\n                    /* WARNING: Treating indirect jump as call */\r\n  (**(code **)(*param_1 + 0x130))();\r\n  return;\r\n}\r\n\r\n"
  },
  "isError": false
}

````

## Unescaped text fields for reading

### text

````text

void FUN_140727100(longlong *param_1,longlong param_2)

{
  byte bVar1;
  undefined1 uVar2;
  float *pfVar3;
  
  pfVar3 = *(float **)(param_2 + 8);
  *(undefined1 *)((longlong)param_1 + 0xb2) = 1;
  if (*(char *)((longlong)pfVar3 + 0x19) != '\0') {
    *(float *)(param_1 + 0x13) = *pfVar3 + *(float *)(param_1 + 0x13);
    *(float *)((longlong)param_1 + 0x9c) = pfVar3[1] + *(float *)((longlong)param_1 + 0x9c);
    *(float *)(param_1 + 0x14) = pfVar3[2] + *(float *)(param_1 + 0x14);
    *(float *)((longlong)param_1 + 0xa4) = *(float *)((longlong)param_1 + 0xa4) + pfVar3[3];
    *(float *)(param_1 + 0x15) = pfVar3[4] + *(float *)(param_1 + 0x15);
    *(float *)((longlong)param_1 + 0xac) = pfVar3[5] + *(float *)((longlong)param_1 + 0xac);
    bVar1 = *(byte *)(pfVar3 + 6);
    *(undefined1 *)((longlong)param_1 + 0xb1) = 1;
    *(byte *)(param_1 + 0x16) = *(byte *)(param_1 + 0x16) | bVar1;
                    /* WARNING: Could not recover jumptable at 0x0001407271ae. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (**(code **)(*param_1 + 0x130))();
    return;
  }
  *(float *)(param_1 + 0x13) = *pfVar3;
  *(float *)((longlong)param_1 + 0x9c) = pfVar3[1];
  *(float *)(param_1 + 0x14) = pfVar3[2];
  *(float *)((longlong)param_1 + 0xa4) = pfVar3[3];
  *(float *)(param_1 + 0x15) = pfVar3[4];
  *(float *)((longlong)param_1 + 0xac) = pfVar3[5];
  uVar2 = *(undefined1 *)(pfVar3 + 6);
  *(undefined1 *)((longlong)param_1 + 0xb1) = 0;
  *(undefined1 *)(param_1 + 0x16) = uVar2;
                    /* WARNING: Could not recover jumptable at 0x000140727205. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(*param_1 + 0x130))();
  return;
}


````
