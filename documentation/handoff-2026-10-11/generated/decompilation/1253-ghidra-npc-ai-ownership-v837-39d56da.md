# ghidra-npc-ai-ownership-v837.json

Original: [work/natural-moveset/ghidra-npc-ai-ownership-v837.json](<../../../../work/natural-moveset/ghidra-npc-ai-ownership-v837.json>)

SHA-256: `8DF009141EABC84A7BE2015BE08FC6BCFCF99BFF64822007A8D2434EC8BC5F8A`; 7087 bytes.

## Original saved record

````json
{
  "program": "Dishonored2.exe",
  "executableSha256": "B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180",
  "scope": "Existing native pause/resume decompilation reconfirmed while generalizing NPC ownership, not proof of general network authority.",
  "functions": [
    {
      "address": "1407e7800",
      "result": {
        "content": [
          {
            "type": "text",
            "text": "\r\n/* [confirmed-runtime] RVA 0x7e7800\r\n   Acquire one owned pause reference only; bounded identity checks.\r\n   Evidence: src/engine_bridge.cpp\r\n   Names are analyst-assigned semantic labels, not recovered official symbols. */\r\n\r\nvoid D2_Component_Pause(undefined8 *param_1,longlong param_2)\r\n\r\n{\r\n  char cVar1;\r\n  int iVar2;\r\n  undefined8 *puVar3;\r\n  uint uVar4;\r\n  \r\n  iVar2 = FUN_1401581c0(param_2 + 0xc08,0,1);\r\n  while (iVar2 == 1) {\r\n    iVar2 = FUN_1401581c0(param_2 + 0xc08,0,1);\r\n  }\r\n  uVar4 = *(uint *)((longlong)param_1 + 0x1c);\r\n  if (((byte)uVar4 & 0xc) < 8) {\r\n    puVar3 = (undefined8 *)(**(code **)*param_1)(param_1);\r\n                    /* WARNING: Subroutine does not return */\r\n    FUN_140166830(&DAT_141c70ec0,*puVar3);\r\n  }\r\n  uVar4 = ((uVar4 & 0xffffffc0) + 0x40 ^ uVar4) & 0x7ffffc0 ^ uVar4;\r\n  *(uint *)((longlong)param_1 + 0x1c) = uVar4;\r\n  if (((uVar4 & 0x7ffffc0) == 0x40) && (uVar4 = uVar4 >> 4 & 3, uVar4 != 1)) {\r\n    if ((uVar4 == 3) && (cVar1 = FUN_1407ec450(param_2,param_1), cVar1 != '\\0')) {\r\n      *(uint *)((longlong)param_1 + 0x1c) = *(uint *)((longlong)param_1 + 0x1c) & 0xffffffef;\r\n      *(uint *)((longlong)param_1 + 0x1c) = *(uint *)((longlong)param_1 + 0x1c) | 0x20;\r\n    }\r\n    if ((*(uint *)((longlong)param_1 + 0x1c) & 0x30) == 0) {\r\n      *(uint *)((longlong)param_1 + 0x1c) = *(uint *)((longlong)param_1 + 0x1c) & 0xffffffdf | 0x10;\r\n      FUN_1407eb390(param_2,param_1);\r\n    }\r\n  }\r\n  FUN_1401581e0(param_2 + 0xc08,0);\r\n  return;\r\n}\r\n\r\n"
          }
        ],
        "structuredContent": {
          "result": "\r\n/* [confirmed-runtime] RVA 0x7e7800\r\n   Acquire one owned pause reference only; bounded identity checks.\r\n   Evidence: src/engine_bridge.cpp\r\n   Names are analyst-assigned semantic labels, not recovered official symbols. */\r\n\r\nvoid D2_Component_Pause(undefined8 *param_1,longlong param_2)\r\n\r\n{\r\n  char cVar1;\r\n  int iVar2;\r\n  undefined8 *puVar3;\r\n  uint uVar4;\r\n  \r\n  iVar2 = FUN_1401581c0(param_2 + 0xc08,0,1);\r\n  while (iVar2 == 1) {\r\n    iVar2 = FUN_1401581c0(param_2 + 0xc08,0,1);\r\n  }\r\n  uVar4 = *(uint *)((longlong)param_1 + 0x1c);\r\n  if (((byte)uVar4 & 0xc) < 8) {\r\n    puVar3 = (undefined8 *)(**(code **)*param_1)(param_1);\r\n                    /* WARNING: Subroutine does not return */\r\n    FUN_140166830(&DAT_141c70ec0,*puVar3);\r\n  }\r\n  uVar4 = ((uVar4 & 0xffffffc0) + 0x40 ^ uVar4) & 0x7ffffc0 ^ uVar4;\r\n  *(uint *)((longlong)param_1 + 0x1c) = uVar4;\r\n  if (((uVar4 & 0x7ffffc0) == 0x40) && (uVar4 = uVar4 >> 4 & 3, uVar4 != 1)) {\r\n    if ((uVar4 == 3) && (cVar1 = FUN_1407ec450(param_2,param_1), cVar1 != '\\0')) {\r\n      *(uint *)((longlong)param_1 + 0x1c) = *(uint *)((longlong)param_1 + 0x1c) & 0xffffffef;\r\n      *(uint *)((longlong)param_1 + 0x1c) = *(uint *)((longlong)param_1 + 0x1c) | 0x20;\r\n    }\r\n    if ((*(uint *)((longlong)param_1 + 0x1c) & 0x30) == 0) {\r\n      *(uint *)((longlong)param_1 + 0x1c) = *(uint *)((longlong)param_1 + 0x1c) & 0xffffffdf | 0x10;\r\n      FUN_1407eb390(param_2,param_1);\r\n    }\r\n  }\r\n  FUN_1401581e0(param_2 + 0xc08,0);\r\n  return;\r\n}\r\n\r\n"
        },
        "isError": false
      }
    },
    {
      "address": "1407e79c0",
      "result": {
        "content": [
          {
            "type": "text",
            "text": "\r\n/* [confirmed-runtime] RVA 0x7e79c0\r\n   Release only our reference; never resume a retired stopped component after native death.\r\n   Evidence: src/engine_bridge.cpp\r\n   Names are analyst-assigned semantic labels, not recovered official symbols. */\r\n\r\nvoid D2_Component_Resume(undefined8 *param_1,longlong param_2)\r\n\r\n{\r\n  char cVar1;\r\n  int iVar2;\r\n  uint uVar3;\r\n  undefined8 *puVar4;\r\n  \r\n  iVar2 = FUN_1401581c0(param_2 + 0xc08,0,1);\r\n  while (iVar2 == 1) {\r\n    iVar2 = FUN_1401581c0(param_2 + 0xc08,0,1);\r\n  }\r\n  uVar3 = *(uint *)((longlong)param_1 + 0x1c);\r\n  if (((byte)uVar3 & 0xc) < 8) {\r\n    puVar4 = (undefined8 *)(**(code **)*param_1)(param_1);\r\n                    /* WARNING: Subroutine does not return */\r\n    FUN_140166830(&DAT_141c70f00,*puVar4);\r\n  }\r\n  if ((uVar3 >> 6 & 0x1fffff) != 0) {\r\n    uVar3 = (((uVar3 >> 6) - 1) * 0x40 ^ uVar3) & 0x7ffffc0 ^ uVar3;\r\n    *(uint *)((longlong)param_1 + 0x1c) = uVar3;\r\n    if (((uVar3 & 0x7ffffc0) == 0) && (uVar3 = uVar3 >> 4 & 3, uVar3 != 3)) {\r\n      if ((uVar3 == 1) && (cVar1 = FUN_1407ec3e0(param_2,param_1), cVar1 != '\\0')) {\r\n        *(uint *)((longlong)param_1 + 0x1c) = *(uint *)((longlong)param_1 + 0x1c) & 0xffffffcf;\r\n      }\r\n      if (((byte)*(uint *)((longlong)param_1 + 0x1c) & 0x30) == 0x20) {\r\n        *(uint *)((longlong)param_1 + 0x1c) = *(uint *)((longlong)param_1 + 0x1c) | 0x30;\r\n        FUN_1407eb3e0(param_2,param_1);\r\n      }\r\n    }\r\n  }\r\n  FUN_1401581e0(param_2 + 0xc08,0);\r\n  return;\r\n}\r\n\r\n"
          }
        ],
        "structuredContent": {
          "result": "\r\n/* [confirmed-runtime] RVA 0x7e79c0\r\n   Release only our reference; never resume a retired stopped component after native death.\r\n   Evidence: src/engine_bridge.cpp\r\n   Names are analyst-assigned semantic labels, not recovered official symbols. */\r\n\r\nvoid D2_Component_Resume(undefined8 *param_1,longlong param_2)\r\n\r\n{\r\n  char cVar1;\r\n  int iVar2;\r\n  uint uVar3;\r\n  undefined8 *puVar4;\r\n  \r\n  iVar2 = FUN_1401581c0(param_2 + 0xc08,0,1);\r\n  while (iVar2 == 1) {\r\n    iVar2 = FUN_1401581c0(param_2 + 0xc08,0,1);\r\n  }\r\n  uVar3 = *(uint *)((longlong)param_1 + 0x1c);\r\n  if (((byte)uVar3 & 0xc) < 8) {\r\n    puVar4 = (undefined8 *)(**(code **)*param_1)(param_1);\r\n                    /* WARNING: Subroutine does not return */\r\n    FUN_140166830(&DAT_141c70f00,*puVar4);\r\n  }\r\n  if ((uVar3 >> 6 & 0x1fffff) != 0) {\r\n    uVar3 = (((uVar3 >> 6) - 1) * 0x40 ^ uVar3) & 0x7ffffc0 ^ uVar3;\r\n    *(uint *)((longlong)param_1 + 0x1c) = uVar3;\r\n    if (((uVar3 & 0x7ffffc0) == 0) && (uVar3 = uVar3 >> 4 & 3, uVar3 != 3)) {\r\n      if ((uVar3 == 1) && (cVar1 = FUN_1407ec3e0(param_2,param_1), cVar1 != '\\0')) {\r\n        *(uint *)((longlong)param_1 + 0x1c) = *(uint *)((longlong)param_1 + 0x1c) & 0xffffffcf;\r\n      }\r\n      if (((byte)*(uint *)((longlong)param_1 + 0x1c) & 0x30) == 0x20) {\r\n        *(uint *)((longlong)param_1 + 0x1c) = *(uint *)((longlong)param_1 + 0x1c) | 0x30;\r\n        FUN_1407eb3e0(param_2,param_1);\r\n      }\r\n    }\r\n  }\r\n  FUN_1401581e0(param_2 + 0xc08,0);\r\n  return;\r\n}\r\n\r\n"
        },
        "isError": false
      }
    }
  ]
}

````

## Unescaped text fields for reading

### text

````text

/* [confirmed-runtime] RVA 0x7e7800
   Acquire one owned pause reference only; bounded identity checks.
   Evidence: src/engine_bridge.cpp
   Names are analyst-assigned semantic labels, not recovered official symbols. */

void D2_Component_Pause(undefined8 *param_1,longlong param_2)

{
  char cVar1;
  int iVar2;
  undefined8 *puVar3;
  uint uVar4;
  
  iVar2 = FUN_1401581c0(param_2 + 0xc08,0,1);
  while (iVar2 == 1) {
    iVar2 = FUN_1401581c0(param_2 + 0xc08,0,1);
  }
  uVar4 = *(uint *)((longlong)param_1 + 0x1c);
  if (((byte)uVar4 & 0xc) < 8) {
    puVar3 = (undefined8 *)(**(code **)*param_1)(param_1);
                    /* WARNING: Subroutine does not return */
    FUN_140166830(&DAT_141c70ec0,*puVar3);
  }
  uVar4 = ((uVar4 & 0xffffffc0) + 0x40 ^ uVar4) & 0x7ffffc0 ^ uVar4;
  *(uint *)((longlong)param_1 + 0x1c) = uVar4;
  if (((uVar4 & 0x7ffffc0) == 0x40) && (uVar4 = uVar4 >> 4 & 3, uVar4 != 1)) {
    if ((uVar4 == 3) && (cVar1 = FUN_1407ec450(param_2,param_1), cVar1 != '\0')) {
      *(uint *)((longlong)param_1 + 0x1c) = *(uint *)((longlong)param_1 + 0x1c) & 0xffffffef;
      *(uint *)((longlong)param_1 + 0x1c) = *(uint *)((longlong)param_1 + 0x1c) | 0x20;
    }
    if ((*(uint *)((longlong)param_1 + 0x1c) & 0x30) == 0) {
      *(uint *)((longlong)param_1 + 0x1c) = *(uint *)((longlong)param_1 + 0x1c) & 0xffffffdf | 0x10;
      FUN_1407eb390(param_2,param_1);
    }
  }
  FUN_1401581e0(param_2 + 0xc08,0);
  return;
}


````

### text

````text

/* [confirmed-runtime] RVA 0x7e79c0
   Release only our reference; never resume a retired stopped component after native death.
   Evidence: src/engine_bridge.cpp
   Names are analyst-assigned semantic labels, not recovered official symbols. */

void D2_Component_Resume(undefined8 *param_1,longlong param_2)

{
  char cVar1;
  int iVar2;
  uint uVar3;
  undefined8 *puVar4;
  
  iVar2 = FUN_1401581c0(param_2 + 0xc08,0,1);
  while (iVar2 == 1) {
    iVar2 = FUN_1401581c0(param_2 + 0xc08,0,1);
  }
  uVar3 = *(uint *)((longlong)param_1 + 0x1c);
  if (((byte)uVar3 & 0xc) < 8) {
    puVar4 = (undefined8 *)(**(code **)*param_1)(param_1);
                    /* WARNING: Subroutine does not return */
    FUN_140166830(&DAT_141c70f00,*puVar4);
  }
  if ((uVar3 >> 6 & 0x1fffff) != 0) {
    uVar3 = (((uVar3 >> 6) - 1) * 0x40 ^ uVar3) & 0x7ffffc0 ^ uVar3;
    *(uint *)((longlong)param_1 + 0x1c) = uVar3;
    if (((uVar3 & 0x7ffffc0) == 0) && (uVar3 = uVar3 >> 4 & 3, uVar3 != 3)) {
      if ((uVar3 == 1) && (cVar1 = FUN_1407ec3e0(param_2,param_1), cVar1 != '\0')) {
        *(uint *)((longlong)param_1 + 0x1c) = *(uint *)((longlong)param_1 + 0x1c) & 0xffffffcf;
      }
      if (((byte)*(uint *)((longlong)param_1 + 0x1c) & 0x30) == 0x20) {
        *(uint *)((longlong)param_1 + 0x1c) = *(uint *)((longlong)param_1 + 0x1c) | 0x30;
        FUN_1407eb3e0(param_2,param_1);
      }
    }
  }
  FUN_1401581e0(param_2 + 0xc08,0);
  return;
}


````
