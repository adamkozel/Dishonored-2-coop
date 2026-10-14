# ghidra-domino-target-mark-v198.json

Original: [work/ghidra-domino-target-mark-v198.json](<../../../../work/ghidra-domino-target-mark-v198.json>)

SHA-256: `A3A5450DEE2E4613D23FE381D566BBFCCEFC37CE216787FE301AC15C8D4B27C1`; 4970 bytes.

## Original saved record

````json
[
  {
    "program": "Dishonored2.exe",
    "addresses": [
      "1404dcfa0",
      "1407e7390"
    ],
    "results": [
      {
        "status": "fulfilled",
        "value": {
          "content": [
            {
              "type": "text",
              "text": "\r\nvoid FUN_1404dcfa0(undefined4 *param_1)\r\n\r\n{\r\n  undefined8 uVar1;\r\n  longlong lVar2;\r\n  undefined4 local_res8 [2];\r\n  \r\n  local_res8[0] = *param_1;\r\n  uVar1 = FUN_1407b5e00(D2_GameSingletonPtr + 0x3130b0,local_res8);\r\n  lVar2 = FUN_1404bd860();\r\n  FUN_1404db600(param_1,uVar1,lVar2 + 0x80);\r\n  FUN_1404ddf10(param_1);\r\n  FUN_1404db360(param_1);\r\n  return;\r\n}\r\n\r\n"
            }
          ],
          "structuredContent": {
            "result": "\r\nvoid FUN_1404dcfa0(undefined4 *param_1)\r\n\r\n{\r\n  undefined8 uVar1;\r\n  longlong lVar2;\r\n  undefined4 local_res8 [2];\r\n  \r\n  local_res8[0] = *param_1;\r\n  uVar1 = FUN_1407b5e00(D2_GameSingletonPtr + 0x3130b0,local_res8);\r\n  lVar2 = FUN_1404bd860();\r\n  FUN_1404db600(param_1,uVar1,lVar2 + 0x80);\r\n  FUN_1404ddf10(param_1);\r\n  FUN_1404db360(param_1);\r\n  return;\r\n}\r\n\r\n"
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
              "text": "\r\nundefined8 FUN_1407e7390(longlong param_1,int param_2)\r\n\r\n{\r\n  longlong lVar1;\r\n  uint uVar2;\r\n  \r\n  uVar2 = *(uint *)(param_1 + 0x1c);\r\n  lVar1 = (longlong)param_2 * 8;\r\n  uVar2 = (uVar2 >> 2 & 3 | (uVar2 & 3) << 2) << 2 | uVar2 >> 4 & 3;\r\n  if (((*(uint *)(&UNK_141c70e88 + lVar1) & uVar2) == *(uint *)(&UNK_141c70e88 + lVar1)) &&\r\n     ((*(uint *)(&UNK_141c70e8c + lVar1) & uVar2) == 0)) {\r\n    return 1;\r\n  }\r\n  return 0;\r\n}\r\n\r\n"
            }
          ],
          "structuredContent": {
            "result": "\r\nundefined8 FUN_1407e7390(longlong param_1,int param_2)\r\n\r\n{\r\n  longlong lVar1;\r\n  uint uVar2;\r\n  \r\n  uVar2 = *(uint *)(param_1 + 0x1c);\r\n  lVar1 = (longlong)param_2 * 8;\r\n  uVar2 = (uVar2 >> 2 & 3 | (uVar2 & 3) << 2) << 2 | uVar2 >> 4 & 3;\r\n  if (((*(uint *)(&UNK_141c70e88 + lVar1) & uVar2) == *(uint *)(&UNK_141c70e88 + lVar1)) &&\r\n     ((*(uint *)(&UNK_141c70e8c + lVar1) & uVar2) == 0)) {\r\n    return 1;\r\n  }\r\n  return 0;\r\n}\r\n\r\n"
          },
          "isError": false
        }
      }
    ]
  },
  {
    "program": "Dishonored2.exe",
    "addresses": [
      "1404db360"
    ],
    "results": [
      {
        "status": "fulfilled",
        "value": {
          "content": [
            {
              "type": "text",
              "text": "\r\nvoid FUN_1404db360(longlong param_1)\r\n\r\n{\r\n  longlong lVar1;\r\n  longlong lVar2;\r\n  longlong lVar3;\r\n  undefined4 *puVar4;\r\n  ulonglong in_stack_ffffffffffffffe0;\r\n  \r\n  lVar1 = FUN_1403da910(param_1 + 8,1);\r\n  if (lVar1 == 0) {\r\n    lVar1 = 0;\r\n  }\r\n  else {\r\n    lVar1 = *(longlong *)(lVar1 + 0x10);\r\n  }\r\n  *(byte *)(lVar1 + 0x369) = *(byte *)(lVar1 + 0x369) | 4;\r\n  lVar2 = (**(code **)(*D2_GameSingletonPtr + 0x1c8))\r\n                    (D2_GameSingletonPtr,&DAT_1439cf460,0,&DAT_1431af668,&DAT_14228a780,\r\n                     in_stack_ffffffffffffffe0 & 0xffffffffffffff00);\r\n  if (lVar2 != 0) {\r\n    lVar3 = FUN_1404bd860();\r\n    puVar4 = &DAT_14228a780;\r\n    FUN_140875e80(lVar1,lVar2,lVar3 + 0x80,&DAT_1431af668,&DAT_14228a780,1);\r\n    FUN_14087e840(lVar2,1);\r\n    FUN_14087e7e0(lVar2,1);\r\n    lVar1 = FUN_1404bd860();\r\n    FUN_1404e2940(param_1 + 0x90,*(undefined8 *)(lVar1 + 0x88),lVar2,0,\r\n                  (ulonglong)puVar4 & 0xffffffff00000000,0,0);\r\n  }\r\n  return;\r\n}\r\n\r\n"
            }
          ],
          "structuredContent": {
            "result": "\r\nvoid FUN_1404db360(longlong param_1)\r\n\r\n{\r\n  longlong lVar1;\r\n  longlong lVar2;\r\n  longlong lVar3;\r\n  undefined4 *puVar4;\r\n  ulonglong in_stack_ffffffffffffffe0;\r\n  \r\n  lVar1 = FUN_1403da910(param_1 + 8,1);\r\n  if (lVar1 == 0) {\r\n    lVar1 = 0;\r\n  }\r\n  else {\r\n    lVar1 = *(longlong *)(lVar1 + 0x10);\r\n  }\r\n  *(byte *)(lVar1 + 0x369) = *(byte *)(lVar1 + 0x369) | 4;\r\n  lVar2 = (**(code **)(*D2_GameSingletonPtr + 0x1c8))\r\n                    (D2_GameSingletonPtr,&DAT_1439cf460,0,&DAT_1431af668,&DAT_14228a780,\r\n                     in_stack_ffffffffffffffe0 & 0xffffffffffffff00);\r\n  if (lVar2 != 0) {\r\n    lVar3 = FUN_1404bd860();\r\n    puVar4 = &DAT_14228a780;\r\n    FUN_140875e80(lVar1,lVar2,lVar3 + 0x80,&DAT_1431af668,&DAT_14228a780,1);\r\n    FUN_14087e840(lVar2,1);\r\n    FUN_14087e7e0(lVar2,1);\r\n    lVar1 = FUN_1404bd860();\r\n    FUN_1404e2940(param_1 + 0x90,*(undefined8 *)(lVar1 + 0x88),lVar2,0,\r\n                  (ulonglong)puVar4 & 0xffffffff00000000,0,0);\r\n  }\r\n  return;\r\n}\r\n\r\n"
          },
          "isError": false
        }
      }
    ]
  }
]

````

## Unescaped text fields for reading

### text

````text

void FUN_1404dcfa0(undefined4 *param_1)

{
  undefined8 uVar1;
  longlong lVar2;
  undefined4 local_res8 [2];
  
  local_res8[0] = *param_1;
  uVar1 = FUN_1407b5e00(D2_GameSingletonPtr + 0x3130b0,local_res8);
  lVar2 = FUN_1404bd860();
  FUN_1404db600(param_1,uVar1,lVar2 + 0x80);
  FUN_1404ddf10(param_1);
  FUN_1404db360(param_1);
  return;
}


````

### text

````text

undefined8 FUN_1407e7390(longlong param_1,int param_2)

{
  longlong lVar1;
  uint uVar2;
  
  uVar2 = *(uint *)(param_1 + 0x1c);
  lVar1 = (longlong)param_2 * 8;
  uVar2 = (uVar2 >> 2 & 3 | (uVar2 & 3) << 2) << 2 | uVar2 >> 4 & 3;
  if (((*(uint *)(&UNK_141c70e88 + lVar1) & uVar2) == *(uint *)(&UNK_141c70e88 + lVar1)) &&
     ((*(uint *)(&UNK_141c70e8c + lVar1) & uVar2) == 0)) {
    return 1;
  }
  return 0;
}


````

### text

````text

void FUN_1404db360(longlong param_1)

{
  longlong lVar1;
  longlong lVar2;
  longlong lVar3;
  undefined4 *puVar4;
  ulonglong in_stack_ffffffffffffffe0;
  
  lVar1 = FUN_1403da910(param_1 + 8,1);
  if (lVar1 == 0) {
    lVar1 = 0;
  }
  else {
    lVar1 = *(longlong *)(lVar1 + 0x10);
  }
  *(byte *)(lVar1 + 0x369) = *(byte *)(lVar1 + 0x369) | 4;
  lVar2 = (**(code **)(*D2_GameSingletonPtr + 0x1c8))
                    (D2_GameSingletonPtr,&DAT_1439cf460,0,&DAT_1431af668,&DAT_14228a780,
                     in_stack_ffffffffffffffe0 & 0xffffffffffffff00);
  if (lVar2 != 0) {
    lVar3 = FUN_1404bd860();
    puVar4 = &DAT_14228a780;
    FUN_140875e80(lVar1,lVar2,lVar3 + 0x80,&DAT_1431af668,&DAT_14228a780,1);
    FUN_14087e840(lVar2,1);
    FUN_14087e7e0(lVar2,1);
    lVar1 = FUN_1404bd860();
    FUN_1404e2940(param_1 + 0x90,*(undefined8 *)(lVar1 + 0x88),lVar2,0,
                  (ulonglong)puVar4 & 0xffffffff00000000,0,0);
  }
  return;
}


````
