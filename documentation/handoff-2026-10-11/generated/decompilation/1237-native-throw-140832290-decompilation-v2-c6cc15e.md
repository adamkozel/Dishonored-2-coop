# native-throw-140832290-decompilation-v2.json

Original: [work/natural-moveset/carry-research/native-throw-140832290-decompilation-v2.json](<../../../../work/natural-moveset/carry-research/native-throw-140832290-decompilation-v2.json>)

SHA-256: `BB2A62C6934C7E7856FF18CA9E60938955EF1534C0C22261048A589BD4DD8FAB`; 3469 bytes.

## Original saved record

````json
{
  "content": [
    {
      "type": "text",
      "text": "\r\nvoid FUN_140832290(longlong param_1)\r\n\r\n{\r\n  float fVar1;\r\n  float fVar2;\r\n  float fVar3;\r\n  float fVar4;\r\n  int iVar5;\r\n  undefined4 uVar6;\r\n  float fVar7;\r\n  float fVar8;\r\n  float fVar9;\r\n  longlong lVar10;\r\n  longlong lVar11;\r\n  float *pfVar12;\r\n  float local_10;\r\n  \r\n  if (*(char *)(param_1 + 0x50) != '\\0') {\r\n    lVar11 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x330);\r\n    iVar5 = *(int *)(lVar11 + 0x118);\r\n    if ((-1 < iVar5) &&\r\n       (lVar11 = *(longlong *)(lVar11 + 0x48),\r\n       *(float *)(lVar11 + 0x24 + (longlong)iVar5 * 0x28) !=\r\n       *(float *)(lVar11 + 8 + (longlong)iVar5 * 0x28))) {\r\n      lVar10 = FUN_1406dc560(*(undefined4 *)(param_1 + 0x38));\r\n      fVar9 = DAT_1431af670;\r\n      fVar8 = DAT_1431af66c;\r\n      fVar7 = DAT_1431af668;\r\n      lVar11 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x310);\r\n      fVar1 = *(float *)(*(longlong *)(param_1 + 0x18) + 0xa0);\r\n      fVar2 = *(float *)(lVar11 + 0x25288);\r\n      fVar3 = *(float *)(lVar11 + 0x2528c);\r\n      local_10 = (float)CONCAT22(local_10._2_2_,0x101);\r\n      uVar6 = *(undefined4 *)(lVar10 + 0xa8);\r\n      fVar4 = *(float *)(lVar11 + 0x25290);\r\n      lVar11 = FUN_140810240();\r\n      pfVar12 = (float *)FUN_1408112c0(lVar11 + 0xc8e8,0x149,0x1c,uVar6);\r\n      if (pfVar12 != (float *)0x0) {\r\n        *pfVar12 = fVar2 * fVar1;\r\n        pfVar12[1] = fVar3 * fVar1;\r\n        pfVar12[2] = fVar4 * fVar1;\r\n        pfVar12[3] = fVar7;\r\n        pfVar12[4] = fVar8;\r\n        pfVar12[5] = fVar9;\r\n        pfVar12[6] = local_10;\r\n      }\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
    }
  ],
  "structuredContent": {
    "result": "\r\nvoid FUN_140832290(longlong param_1)\r\n\r\n{\r\n  float fVar1;\r\n  float fVar2;\r\n  float fVar3;\r\n  float fVar4;\r\n  int iVar5;\r\n  undefined4 uVar6;\r\n  float fVar7;\r\n  float fVar8;\r\n  float fVar9;\r\n  longlong lVar10;\r\n  longlong lVar11;\r\n  float *pfVar12;\r\n  float local_10;\r\n  \r\n  if (*(char *)(param_1 + 0x50) != '\\0') {\r\n    lVar11 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x330);\r\n    iVar5 = *(int *)(lVar11 + 0x118);\r\n    if ((-1 < iVar5) &&\r\n       (lVar11 = *(longlong *)(lVar11 + 0x48),\r\n       *(float *)(lVar11 + 0x24 + (longlong)iVar5 * 0x28) !=\r\n       *(float *)(lVar11 + 8 + (longlong)iVar5 * 0x28))) {\r\n      lVar10 = FUN_1406dc560(*(undefined4 *)(param_1 + 0x38));\r\n      fVar9 = DAT_1431af670;\r\n      fVar8 = DAT_1431af66c;\r\n      fVar7 = DAT_1431af668;\r\n      lVar11 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x310);\r\n      fVar1 = *(float *)(*(longlong *)(param_1 + 0x18) + 0xa0);\r\n      fVar2 = *(float *)(lVar11 + 0x25288);\r\n      fVar3 = *(float *)(lVar11 + 0x2528c);\r\n      local_10 = (float)CONCAT22(local_10._2_2_,0x101);\r\n      uVar6 = *(undefined4 *)(lVar10 + 0xa8);\r\n      fVar4 = *(float *)(lVar11 + 0x25290);\r\n      lVar11 = FUN_140810240();\r\n      pfVar12 = (float *)FUN_1408112c0(lVar11 + 0xc8e8,0x149,0x1c,uVar6);\r\n      if (pfVar12 != (float *)0x0) {\r\n        *pfVar12 = fVar2 * fVar1;\r\n        pfVar12[1] = fVar3 * fVar1;\r\n        pfVar12[2] = fVar4 * fVar1;\r\n        pfVar12[3] = fVar7;\r\n        pfVar12[4] = fVar8;\r\n        pfVar12[5] = fVar9;\r\n        pfVar12[6] = local_10;\r\n      }\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
  },
  "isError": false
}

````

## Unescaped text fields for reading

### text

````text

void FUN_140832290(longlong param_1)

{
  float fVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  int iVar5;
  undefined4 uVar6;
  float fVar7;
  float fVar8;
  float fVar9;
  longlong lVar10;
  longlong lVar11;
  float *pfVar12;
  float local_10;
  
  if (*(char *)(param_1 + 0x50) != '\0') {
    lVar11 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x330);
    iVar5 = *(int *)(lVar11 + 0x118);
    if ((-1 < iVar5) &&
       (lVar11 = *(longlong *)(lVar11 + 0x48),
       *(float *)(lVar11 + 0x24 + (longlong)iVar5 * 0x28) !=
       *(float *)(lVar11 + 8 + (longlong)iVar5 * 0x28))) {
      lVar10 = FUN_1406dc560(*(undefined4 *)(param_1 + 0x38));
      fVar9 = DAT_1431af670;
      fVar8 = DAT_1431af66c;
      fVar7 = DAT_1431af668;
      lVar11 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x310);
      fVar1 = *(float *)(*(longlong *)(param_1 + 0x18) + 0xa0);
      fVar2 = *(float *)(lVar11 + 0x25288);
      fVar3 = *(float *)(lVar11 + 0x2528c);
      local_10 = (float)CONCAT22(local_10._2_2_,0x101);
      uVar6 = *(undefined4 *)(lVar10 + 0xa8);
      fVar4 = *(float *)(lVar11 + 0x25290);
      lVar11 = FUN_140810240();
      pfVar12 = (float *)FUN_1408112c0(lVar11 + 0xc8e8,0x149,0x1c,uVar6);
      if (pfVar12 != (float *)0x0) {
        *pfVar12 = fVar2 * fVar1;
        pfVar12[1] = fVar3 * fVar1;
        pfVar12[2] = fVar4 * fVar1;
        pfVar12[3] = fVar7;
        pfVar12[4] = fVar8;
        pfVar12[5] = fVar9;
        pfVar12[6] = local_10;
      }
    }
  }
  return;
}


````
