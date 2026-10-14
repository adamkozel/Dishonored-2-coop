# throw-selector-0-decompilation-v2.json

Original: [work/natural-moveset/carry-research/throw-selector-0-decompilation-v2.json](<../../../../work/natural-moveset/carry-research/throw-selector-0-decompilation-v2.json>)

SHA-256: `D4E2523C1EB8915869977EAEF3A55E2CF5CC644219939A4F658589D208CC3479`; 2229 bytes.

## Original saved record

````json
{
  "status": "fulfilled",
  "value": {
    "content": [
      {
        "type": "text",
        "text": "\r\nundefined1 FUN_1403a6f10(longlong param_1,char param_2)\r\n\r\n{\r\n  byte bVar1;\r\n  undefined8 uVar2;\r\n  bool bVar3;\r\n  char cVar4;\r\n  undefined1 uVar5;\r\n  \r\n  uVar2 = *(undefined8 *)(param_1 + 0x20);\r\n  cVar4 = FUN_140803180(uVar2,10);\r\n  if (cVar4 == '\\0') {\r\n    cVar4 = FUN_140803180(uVar2,0xb);\r\n    bVar3 = false;\r\n    if (cVar4 == '\\0') goto LAB_1403a6f4d;\r\n  }\r\n  bVar3 = true;\r\nLAB_1403a6f4d:\r\n  uVar5 = 4;\r\n  if (!bVar3) {\r\n    if (param_2 == '\\0') {\r\n      bVar1 = *(byte *)(param_1 + 0xf0);\r\n      if ((bVar1 & 0x10) != 0) {\r\n        return 3;\r\n      }\r\n      if (((bVar1 & 1) != 0) && ((bVar1 & 2) != 0)) {\r\n        cVar4 = FUN_140803180(uVar2,6);\r\n        uVar5 = cVar4 != '\\0';\r\n        if (uVar5 != '\\x04') {\r\n          return (bool)uVar5;\r\n        }\r\n      }\r\n    }\r\n    if (((*(byte *)(param_1 + 0xf0) & 4) != 0) && ((*(byte *)(param_1 + 0xf0) & 8) != 0)) {\r\n      uVar5 = 2;\r\n    }\r\n  }\r\n  return uVar5;\r\n}\r\n\r\n"
      }
    ],
    "structuredContent": {
      "result": "\r\nundefined1 FUN_1403a6f10(longlong param_1,char param_2)\r\n\r\n{\r\n  byte bVar1;\r\n  undefined8 uVar2;\r\n  bool bVar3;\r\n  char cVar4;\r\n  undefined1 uVar5;\r\n  \r\n  uVar2 = *(undefined8 *)(param_1 + 0x20);\r\n  cVar4 = FUN_140803180(uVar2,10);\r\n  if (cVar4 == '\\0') {\r\n    cVar4 = FUN_140803180(uVar2,0xb);\r\n    bVar3 = false;\r\n    if (cVar4 == '\\0') goto LAB_1403a6f4d;\r\n  }\r\n  bVar3 = true;\r\nLAB_1403a6f4d:\r\n  uVar5 = 4;\r\n  if (!bVar3) {\r\n    if (param_2 == '\\0') {\r\n      bVar1 = *(byte *)(param_1 + 0xf0);\r\n      if ((bVar1 & 0x10) != 0) {\r\n        return 3;\r\n      }\r\n      if (((bVar1 & 1) != 0) && ((bVar1 & 2) != 0)) {\r\n        cVar4 = FUN_140803180(uVar2,6);\r\n        uVar5 = cVar4 != '\\0';\r\n        if (uVar5 != '\\x04') {\r\n          return (bool)uVar5;\r\n        }\r\n      }\r\n    }\r\n    if (((*(byte *)(param_1 + 0xf0) & 4) != 0) && ((*(byte *)(param_1 + 0xf0) & 8) != 0)) {\r\n      uVar5 = 2;\r\n    }\r\n  }\r\n  return uVar5;\r\n}\r\n\r\n"
    },
    "isError": false
  }
}

````

## Unescaped text fields for reading

### text

````text

undefined1 FUN_1403a6f10(longlong param_1,char param_2)

{
  byte bVar1;
  undefined8 uVar2;
  bool bVar3;
  char cVar4;
  undefined1 uVar5;
  
  uVar2 = *(undefined8 *)(param_1 + 0x20);
  cVar4 = FUN_140803180(uVar2,10);
  if (cVar4 == '\0') {
    cVar4 = FUN_140803180(uVar2,0xb);
    bVar3 = false;
    if (cVar4 == '\0') goto LAB_1403a6f4d;
  }
  bVar3 = true;
LAB_1403a6f4d:
  uVar5 = 4;
  if (!bVar3) {
    if (param_2 == '\0') {
      bVar1 = *(byte *)(param_1 + 0xf0);
      if ((bVar1 & 0x10) != 0) {
        return 3;
      }
      if (((bVar1 & 1) != 0) && ((bVar1 & 2) != 0)) {
        cVar4 = FUN_140803180(uVar2,6);
        uVar5 = cVar4 != '\0';
        if (uVar5 != '\x04') {
          return (bool)uVar5;
        }
      }
    }
    if (((*(byte *)(param_1 + 0xf0) & 4) != 0) && ((*(byte *)(param_1 + 0xf0) & 8) != 0)) {
      uVar5 = 2;
    }
  }
  return uVar5;
}


````
