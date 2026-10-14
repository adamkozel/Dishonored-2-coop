# handoff-live-ghidra-verification.json

Original: [work/handoff-live-ghidra-verification.json](<../../../../work/handoff-live-ghidra-verification.json>)

SHA-256: `D8588DF94812B95340BCE634302043B0C19A05FF45344AD042DBAEDCA1C21848`; 6187 bytes.

## Original saved record

````json
[
  {
    "status": "fulfilled",
    "value": {
      "content": [
        {
          "type": "text",
          "text": "Function: FUN_140734d90 at 140734d90\nSignature: undefined FUN_140734d90(void)\nEntry: 140734d90\nBody: 140734d90 - 140734f10"
        }
      ],
      "structuredContent": {
        "result": "Function: FUN_140734d90 at 140734d90\nSignature: undefined FUN_140734d90(void)\nEntry: 140734d90\nBody: 140734d90 - 140734f10"
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
          "text": "From 143f2b414 [DATA]\nFrom 140736dc5 [UNCONDITIONAL_CALL]"
        }
      ],
      "structuredContent": {
        "result": "From 143f2b414 [DATA]\nFrom 140736dc5 [UNCONDITIONAL_CALL]"
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
          "text": "\r\nvoid FUN_140734d90(longlong param_1)\r\n\r\n{\r\n  longlong lVar1;\r\n  undefined8 uVar2;\r\n  longlong lVar3;\r\n  longlong *plVar4;\r\n  longlong *plVar5;\r\n  char cVar6;\r\n  longlong lVar7;\r\n  short sVar8;\r\n  uint local_res8 [2];\r\n  undefined2 local_b8;\r\n  undefined4 local_b6;\r\n  uint local_b0;\r\n  undefined1 local_ac;\r\n  undefined8 local_a8;\r\n  undefined8 uStack_a0;\r\n  undefined8 local_98;\r\n  undefined8 uStack_90;\r\n  undefined4 local_88;\r\n  undefined4 uStack_84;\r\n  undefined4 uStack_80;\r\n  undefined4 uStack_7c;\r\n  undefined4 local_78;\r\n  undefined4 uStack_74;\r\n  undefined4 uStack_70;\r\n  undefined4 uStack_6c;\r\n  undefined8 local_68;\r\n  undefined8 uStack_60;\r\n  undefined8 local_58;\r\n  undefined8 uStack_50;\r\n  undefined4 local_48;\r\n  undefined4 uStack_44;\r\n  undefined4 uStack_40;\r\n  undefined4 uStack_3c;\r\n  undefined4 local_38;\r\n  undefined4 uStack_34;\r\n  undefined4 uStack_30;\r\n  undefined4 uStack_2c;\r\n  \r\n  uVar2 = *(undefined8 *)(D2_GameSingletonPtr + 0x60410);\r\n  lVar3 = *(longlong *)(param_1 + 0x100);\r\n  plVar4 = *(longlong **)(param_1 + 0x148);\r\n  plVar5 = *(longlong **)(param_1 + 0x150);\r\n  lVar1 = D2_GameSingletonPtr + 0x60280;\r\n  cVar6 = FUN_1408a7820();\r\n  sVar8 = 0;\r\n  if (0 < *(int *)(param_1 + 0x138)) {\r\n    do {\r\n      lVar7 = (longlong)sVar8;\r\n      local_res8[0] = *(uint *)(*(longlong *)(lVar3 + 0x28) + lVar7 * 4);\r\n      if ((local_res8[0] & 0xffffff) != 0xffffff) {\r\n        FUN_1407697d0(param_1,*(undefined2 *)(*plVar4 + lVar7 * 2),lVar7 * 0x40 + *plVar5,&local_68)\r\n        ;\r\n        if (cVar6 == '\\0') {\r\n          FUN_140c85860(uVar2,local_res8[0],&local_68,0);\r\n        }\r\n        else {\r\n          local_b8 = 0x50;\r\n          local_a8 = local_68;\r\n          uStack_a0 = uStack_60;\r\n          local_98 = local_58;\r\n          uStack_90 = uStack_50;\r\n          local_b6 = 0x60200;\r\n          local_b0 = local_res8[0];\r\n          local_ac = 0;\r\n          local_88 = local_48;\r\n          uStack_84 = uStack_44;\r\n          uStack_80 = uStack_40;\r\n          uStack_7c = uStack_3c;\r\n          local_78 = local_38;\r\n          uStack_74 = uStack_34;\r\n          uStack_70 = uStack_30;\r\n          uStack_6c = uStack_2c;\r\n          FUN_140090fb0(lVar1,&local_b8);\r\n        }\r\n        FUN_14018d020(lVar1,local_res8,&DAT_1423ccc20,&DAT_1423ccc20,cVar6);\r\n      }\r\n      sVar8 = sVar8 + 1;\r\n    } while ((int)sVar8 < *(int *)(param_1 + 0x138));\r\n  }\r\n  return;\r\n}\r\n\r\n"
        }
      ],
      "structuredContent": {
        "result": "\r\nvoid FUN_140734d90(longlong param_1)\r\n\r\n{\r\n  longlong lVar1;\r\n  undefined8 uVar2;\r\n  longlong lVar3;\r\n  longlong *plVar4;\r\n  longlong *plVar5;\r\n  char cVar6;\r\n  longlong lVar7;\r\n  short sVar8;\r\n  uint local_res8 [2];\r\n  undefined2 local_b8;\r\n  undefined4 local_b6;\r\n  uint local_b0;\r\n  undefined1 local_ac;\r\n  undefined8 local_a8;\r\n  undefined8 uStack_a0;\r\n  undefined8 local_98;\r\n  undefined8 uStack_90;\r\n  undefined4 local_88;\r\n  undefined4 uStack_84;\r\n  undefined4 uStack_80;\r\n  undefined4 uStack_7c;\r\n  undefined4 local_78;\r\n  undefined4 uStack_74;\r\n  undefined4 uStack_70;\r\n  undefined4 uStack_6c;\r\n  undefined8 local_68;\r\n  undefined8 uStack_60;\r\n  undefined8 local_58;\r\n  undefined8 uStack_50;\r\n  undefined4 local_48;\r\n  undefined4 uStack_44;\r\n  undefined4 uStack_40;\r\n  undefined4 uStack_3c;\r\n  undefined4 local_38;\r\n  undefined4 uStack_34;\r\n  undefined4 uStack_30;\r\n  undefined4 uStack_2c;\r\n  \r\n  uVar2 = *(undefined8 *)(D2_GameSingletonPtr + 0x60410);\r\n  lVar3 = *(longlong *)(param_1 + 0x100);\r\n  plVar4 = *(longlong **)(param_1 + 0x148);\r\n  plVar5 = *(longlong **)(param_1 + 0x150);\r\n  lVar1 = D2_GameSingletonPtr + 0x60280;\r\n  cVar6 = FUN_1408a7820();\r\n  sVar8 = 0;\r\n  if (0 < *(int *)(param_1 + 0x138)) {\r\n    do {\r\n      lVar7 = (longlong)sVar8;\r\n      local_res8[0] = *(uint *)(*(longlong *)(lVar3 + 0x28) + lVar7 * 4);\r\n      if ((local_res8[0] & 0xffffff) != 0xffffff) {\r\n        FUN_1407697d0(param_1,*(undefined2 *)(*plVar4 + lVar7 * 2),lVar7 * 0x40 + *plVar5,&local_68)\r\n        ;\r\n        if (cVar6 == '\\0') {\r\n          FUN_140c85860(uVar2,local_res8[0],&local_68,0);\r\n        }\r\n        else {\r\n          local_b8 = 0x50;\r\n          local_a8 = local_68;\r\n          uStack_a0 = uStack_60;\r\n          local_98 = local_58;\r\n          uStack_90 = uStack_50;\r\n          local_b6 = 0x60200;\r\n          local_b0 = local_res8[0];\r\n          local_ac = 0;\r\n          local_88 = local_48;\r\n          uStack_84 = uStack_44;\r\n          uStack_80 = uStack_40;\r\n          uStack_7c = uStack_3c;\r\n          local_78 = local_38;\r\n          uStack_74 = uStack_34;\r\n          uStack_70 = uStack_30;\r\n          uStack_6c = uStack_2c;\r\n          FUN_140090fb0(lVar1,&local_b8);\r\n        }\r\n        FUN_14018d020(lVar1,local_res8,&DAT_1423ccc20,&DAT_1423ccc20,cVar6);\r\n      }\r\n      sVar8 = sVar8 + 1;\r\n    } while ((int)sVar8 < *(int *)(param_1 + 0x138));\r\n  }\r\n  return;\r\n}\r\n\r\n"
      },
      "isError": false
    }
  }
]

````

## Unescaped text fields for reading

### text

````text

void FUN_140734d90(longlong param_1)

{
  longlong lVar1;
  undefined8 uVar2;
  longlong lVar3;
  longlong *plVar4;
  longlong *plVar5;
  char cVar6;
  longlong lVar7;
  short sVar8;
  uint local_res8 [2];
  undefined2 local_b8;
  undefined4 local_b6;
  uint local_b0;
  undefined1 local_ac;
  undefined8 local_a8;
  undefined8 uStack_a0;
  undefined8 local_98;
  undefined8 uStack_90;
  undefined4 local_88;
  undefined4 uStack_84;
  undefined4 uStack_80;
  undefined4 uStack_7c;
  undefined4 local_78;
  undefined4 uStack_74;
  undefined4 uStack_70;
  undefined4 uStack_6c;
  undefined8 local_68;
  undefined8 uStack_60;
  undefined8 local_58;
  undefined8 uStack_50;
  undefined4 local_48;
  undefined4 uStack_44;
  undefined4 uStack_40;
  undefined4 uStack_3c;
  undefined4 local_38;
  undefined4 uStack_34;
  undefined4 uStack_30;
  undefined4 uStack_2c;
  
  uVar2 = *(undefined8 *)(D2_GameSingletonPtr + 0x60410);
  lVar3 = *(longlong *)(param_1 + 0x100);
  plVar4 = *(longlong **)(param_1 + 0x148);
  plVar5 = *(longlong **)(param_1 + 0x150);
  lVar1 = D2_GameSingletonPtr + 0x60280;
  cVar6 = FUN_1408a7820();
  sVar8 = 0;
  if (0 < *(int *)(param_1 + 0x138)) {
    do {
      lVar7 = (longlong)sVar8;
      local_res8[0] = *(uint *)(*(longlong *)(lVar3 + 0x28) + lVar7 * 4);
      if ((local_res8[0] & 0xffffff) != 0xffffff) {
        FUN_1407697d0(param_1,*(undefined2 *)(*plVar4 + lVar7 * 2),lVar7 * 0x40 + *plVar5,&local_68)
        ;
        if (cVar6 == '\0') {
          FUN_140c85860(uVar2,local_res8[0],&local_68,0);
        }
        else {
          local_b8 = 0x50;
          local_a8 = local_68;
          uStack_a0 = uStack_60;
          local_98 = local_58;
          uStack_90 = uStack_50;
          local_b6 = 0x60200;
          local_b0 = local_res8[0];
          local_ac = 0;
          local_88 = local_48;
          uStack_84 = uStack_44;
          uStack_80 = uStack_40;
          uStack_7c = uStack_3c;
          local_78 = local_38;
          uStack_74 = uStack_34;
          uStack_70 = uStack_30;
          uStack_6c = uStack_2c;
          FUN_140090fb0(lVar1,&local_b8);
        }
        FUN_14018d020(lVar1,local_res8,&DAT_1423ccc20,&DAT_1423ccc20,cVar6);
      }
      sVar8 = sVar8 + 1;
    } while ((int)sVar8 < *(int *)(param_1 + 0x138));
  }
  return;
}


````
