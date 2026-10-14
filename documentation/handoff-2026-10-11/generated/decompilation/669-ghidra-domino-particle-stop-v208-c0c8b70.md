# ghidra-domino-particle-stop-v208.json

Original: [work/ghidra-domino-particle-stop-v208.json](<../../../../work/ghidra-domino-particle-stop-v208.json>)

SHA-256: `C0FCB84B3E23D3521308A17CEBBE90528CA8C8AF61DB7919A6609D5AA3DCAB2A`; 5750 bytes.

## Original saved record

````json
{
  "functions": [
    {
      "status": "fulfilled",
      "value": {
        "content": [
          {
            "type": "text",
            "text": "\r\nvoid FUN_140220ca0(ushort param_1,longlong *param_2,undefined4 param_3,undefined8 param_4)\r\n\r\n{\r\n  for (; param_1 != 0xffff; param_1 = *(ushort *)(param_2[0x6c02] + (ulonglong)param_1 * 2)) {\r\n    FUN_140228080((ulonglong)param_1 * 0xd8 + *param_2,param_3,param_4);\r\n  }\r\n  return;\r\n}\r\n\r\n"
          }
        ],
        "structuredContent": {
          "result": "\r\nvoid FUN_140220ca0(ushort param_1,longlong *param_2,undefined4 param_3,undefined8 param_4)\r\n\r\n{\r\n  for (; param_1 != 0xffff; param_1 = *(ushort *)(param_2[0x6c02] + (ulonglong)param_1 * 2)) {\r\n    FUN_140228080((ulonglong)param_1 * 0xd8 + *param_2,param_3,param_4);\r\n  }\r\n  return;\r\n}\r\n\r\n"
        },
        "isError": false
      }
    },
    {
      "status": "fulfilled",
      "value": {
        "content": [
          {
            "type": "text",
            "text": "\r\nvoid FUN_140221690(ushort param_1,longlong *param_2,undefined4 param_3,undefined8 param_4)\r\n\r\n{\r\n  char cVar1;\r\n  longlong lVar2;\r\n  \r\n  for (; param_1 != 0xffff; param_1 = *(ushort *)(param_2[0x6c02] + (ulonglong)param_1 * 2)) {\r\n    lVar2 = (ulonglong)param_1 * 0xd8 + *param_2;\r\n    cVar1 = FUN_140227510(lVar2);\r\n    if (cVar1 != '\\0') {\r\n      FUN_140228080(lVar2,param_3,param_4);\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
          }
        ],
        "structuredContent": {
          "result": "\r\nvoid FUN_140221690(ushort param_1,longlong *param_2,undefined4 param_3,undefined8 param_4)\r\n\r\n{\r\n  char cVar1;\r\n  longlong lVar2;\r\n  \r\n  for (; param_1 != 0xffff; param_1 = *(ushort *)(param_2[0x6c02] + (ulonglong)param_1 * 2)) {\r\n    lVar2 = (ulonglong)param_1 * 0xd8 + *param_2;\r\n    cVar1 = FUN_140227510(lVar2);\r\n    if (cVar1 != '\\0') {\r\n      FUN_140228080(lVar2,param_3,param_4);\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
        },
        "isError": false
      }
    },
    {
      "status": "fulfilled",
      "value": {
        "content": [
          {
            "type": "text",
            "text": "\r\nint * FUN_1401a2970(longlong *param_1,uint *param_2,undefined2 *param_3)\r\n\r\n{\r\n  longlong lVar1;\r\n  undefined2 uVar2;\r\n  uint uVar3;\r\n  int *piVar4;\r\n  undefined8 uVar5;\r\n  uint *puVar6;\r\n  longlong *plVar7;\r\n  \r\n  uVar3 = *param_2;\r\n  lVar1 = (longlong)(int)(*(uint *)(param_1 + 2) & uVar3) * 8;\r\n  plVar7 = (longlong *)(*param_1 + lVar1);\r\n  piVar4 = (int *)*plVar7;\r\n  do {\r\n    if (piVar4 == (int *)0x0) {\r\nLAB_1401a29c5:\r\n      *(int *)((longlong)param_1 + 0xc) = *(int *)((longlong)param_1 + 0xc) + 1;\r\n      puVar6 = (uint *)FUN_1400f14b0(0x10,*(undefined1 *)((longlong)param_1 + 0x14));\r\n      if (puVar6 != (uint *)0x0) {\r\n        uVar5 = *(undefined8 *)(lVar1 + *param_1);\r\n        *puVar6 = *param_2;\r\n        uVar2 = *param_3;\r\n        *(undefined8 *)(puVar6 + 2) = uVar5;\r\n        *(undefined2 *)(puVar6 + 1) = uVar2;\r\n      }\r\n      *plVar7 = (longlong)puVar6;\r\n      *(int **)(puVar6 + 2) = piVar4;\r\n      return (int *)(*plVar7 + 4);\r\n    }\r\n    if ((int)uVar3 <= *piVar4) {\r\n      if (*piVar4 <= (int)uVar3) {\r\n        *(undefined2 *)(piVar4 + 1) = *param_3;\r\n        return piVar4 + 1;\r\n      }\r\n      goto LAB_1401a29c5;\r\n    }\r\n    plVar7 = (longlong *)(piVar4 + 2);\r\n    piVar4 = *(int **)(piVar4 + 2);\r\n  } while( true );\r\n}\r\n\r\n"
          }
        ],
        "structuredContent": {
          "result": "\r\nint * FUN_1401a2970(longlong *param_1,uint *param_2,undefined2 *param_3)\r\n\r\n{\r\n  longlong lVar1;\r\n  undefined2 uVar2;\r\n  uint uVar3;\r\n  int *piVar4;\r\n  undefined8 uVar5;\r\n  uint *puVar6;\r\n  longlong *plVar7;\r\n  \r\n  uVar3 = *param_2;\r\n  lVar1 = (longlong)(int)(*(uint *)(param_1 + 2) & uVar3) * 8;\r\n  plVar7 = (longlong *)(*param_1 + lVar1);\r\n  piVar4 = (int *)*plVar7;\r\n  do {\r\n    if (piVar4 == (int *)0x0) {\r\nLAB_1401a29c5:\r\n      *(int *)((longlong)param_1 + 0xc) = *(int *)((longlong)param_1 + 0xc) + 1;\r\n      puVar6 = (uint *)FUN_1400f14b0(0x10,*(undefined1 *)((longlong)param_1 + 0x14));\r\n      if (puVar6 != (uint *)0x0) {\r\n        uVar5 = *(undefined8 *)(lVar1 + *param_1);\r\n        *puVar6 = *param_2;\r\n        uVar2 = *param_3;\r\n        *(undefined8 *)(puVar6 + 2) = uVar5;\r\n        *(undefined2 *)(puVar6 + 1) = uVar2;\r\n      }\r\n      *plVar7 = (longlong)puVar6;\r\n      *(int **)(puVar6 + 2) = piVar4;\r\n      return (int *)(*plVar7 + 4);\r\n    }\r\n    if ((int)uVar3 <= *piVar4) {\r\n      if (*piVar4 <= (int)uVar3) {\r\n        *(undefined2 *)(piVar4 + 1) = *param_3;\r\n        return piVar4 + 1;\r\n      }\r\n      goto LAB_1401a29c5;\r\n    }\r\n    plVar7 = (longlong *)(piVar4 + 2);\r\n    piVar4 = *(int **)(piVar4 + 2);\r\n  } while( true );\r\n}\r\n\r\n"
        },
        "isError": false
      }
    }
  ],
  "detail": {
    "status": "fulfilled",
    "value": {
      "content": [
        {
          "type": "text",
          "text": "\r\nvoid FUN_140228080(longlong param_1,undefined4 param_2)\r\n\r\n{\r\n  FUN_140216f80(param_1 + 0x30,*(longlong *)(param_1 + 0x28) + 0x2c,param_2);\r\n  *(byte *)(param_1 + 0x24) = *(byte *)(param_1 + 0x24) & 0xfb;\r\n  return;\r\n}\r\n\r\n"
        }
      ],
      "structuredContent": {
        "result": "\r\nvoid FUN_140228080(longlong param_1,undefined4 param_2)\r\n\r\n{\r\n  FUN_140216f80(param_1 + 0x30,*(longlong *)(param_1 + 0x28) + 0x2c,param_2);\r\n  *(byte *)(param_1 + 0x24) = *(byte *)(param_1 + 0x24) & 0xfb;\r\n  return;\r\n}\r\n\r\n"
      },
      "isError": false
    }
  }
}

````

## Unescaped text fields for reading

### text

````text

void FUN_140220ca0(ushort param_1,longlong *param_2,undefined4 param_3,undefined8 param_4)

{
  for (; param_1 != 0xffff; param_1 = *(ushort *)(param_2[0x6c02] + (ulonglong)param_1 * 2)) {
    FUN_140228080((ulonglong)param_1 * 0xd8 + *param_2,param_3,param_4);
  }
  return;
}


````

### text

````text

void FUN_140221690(ushort param_1,longlong *param_2,undefined4 param_3,undefined8 param_4)

{
  char cVar1;
  longlong lVar2;
  
  for (; param_1 != 0xffff; param_1 = *(ushort *)(param_2[0x6c02] + (ulonglong)param_1 * 2)) {
    lVar2 = (ulonglong)param_1 * 0xd8 + *param_2;
    cVar1 = FUN_140227510(lVar2);
    if (cVar1 != '\0') {
      FUN_140228080(lVar2,param_3,param_4);
    }
  }
  return;
}


````

### text

````text

int * FUN_1401a2970(longlong *param_1,uint *param_2,undefined2 *param_3)

{
  longlong lVar1;
  undefined2 uVar2;
  uint uVar3;
  int *piVar4;
  undefined8 uVar5;
  uint *puVar6;
  longlong *plVar7;
  
  uVar3 = *param_2;
  lVar1 = (longlong)(int)(*(uint *)(param_1 + 2) & uVar3) * 8;
  plVar7 = (longlong *)(*param_1 + lVar1);
  piVar4 = (int *)*plVar7;
  do {
    if (piVar4 == (int *)0x0) {
LAB_1401a29c5:
      *(int *)((longlong)param_1 + 0xc) = *(int *)((longlong)param_1 + 0xc) + 1;
      puVar6 = (uint *)FUN_1400f14b0(0x10,*(undefined1 *)((longlong)param_1 + 0x14));
      if (puVar6 != (uint *)0x0) {
        uVar5 = *(undefined8 *)(lVar1 + *param_1);
        *puVar6 = *param_2;
        uVar2 = *param_3;
        *(undefined8 *)(puVar6 + 2) = uVar5;
        *(undefined2 *)(puVar6 + 1) = uVar2;
      }
      *plVar7 = (longlong)puVar6;
      *(int **)(puVar6 + 2) = piVar4;
      return (int *)(*plVar7 + 4);
    }
    if ((int)uVar3 <= *piVar4) {
      if (*piVar4 <= (int)uVar3) {
        *(undefined2 *)(piVar4 + 1) = *param_3;
        return piVar4 + 1;
      }
      goto LAB_1401a29c5;
    }
    plVar7 = (longlong *)(piVar4 + 2);
    piVar4 = *(int **)(piVar4 + 2);
  } while( true );
}


````

### text

````text

void FUN_140228080(longlong param_1,undefined4 param_2)

{
  FUN_140216f80(param_1 + 0x30,*(longlong *)(param_1 + 0x28) + 0x2c,param_2);
  *(byte *)(param_1 + 0x24) = *(byte *)(param_1 + 0x24) & 0xfb;
  return;
}


````
