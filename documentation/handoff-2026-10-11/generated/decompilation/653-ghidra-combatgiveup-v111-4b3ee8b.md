# ghidra-combatgiveup-v111.json

Original: [work/ghidra-combatgiveup-v111.json](<../../../../work/ghidra-combatgiveup-v111.json>)

SHA-256: `79B13D0DD80F5B904544C2B66A6E583F2881C84ADA8181007D5E6CE5F7B9C685`; 5542 bytes.

## Original saved record

````json
{
  "address": "1405a4b00",
  "executableSha256": "B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180",
  "decompilation": "\r\nundefined4 * FUN_1405a4b00(longlong param_1,undefined4 *param_2)\r\n\r\n{\r\n  int iVar1;\r\n  bool bVar2;\r\n  char cVar3;\r\n  char *pcVar4;\r\n  undefined8 uVar5;\r\n  longlong lVar6;\r\n  undefined4 uVar7;\r\n  uint uStack_204;\r\n  undefined **ppuStack_1f8;\r\n  undefined4 uStack_1f0;\r\n  undefined4 uStack_1ec;\r\n  undefined8 uStack_1e8;\r\n  undefined4 uStack_1e0;\r\n  undefined4 uStack_1dc;\r\n  undefined4 uStack_1d8;\r\n  undefined4 uStack_1d4;\r\n  undefined8 uStack_1d0;\r\n  undefined4 uStack_1c8;\r\n  undefined4 uStack_1c4;\r\n  undefined4 uStack_1c0;\r\n  undefined4 uStack_1bc;\r\n  undefined8 uStack_1b8;\r\n  undefined4 uStack_1b0;\r\n  undefined4 uStack_1ac;\r\n  undefined4 uStack_1a8;\r\n  undefined4 uStack_1a4;\r\n  undefined8 uStack_1a0;\r\n  undefined8 uStack_198;\r\n  undefined8 uStack_190;\r\n  undefined8 uStack_188;\r\n  undefined4 uStack_180;\r\n  undefined8 uStack_178;\r\n  undefined4 uStack_170;\r\n  undefined8 uStack_168;\r\n  undefined4 uStack_160;\r\n  byte bStack_158;\r\n  undefined8 uStack_150;\r\n  undefined1 auStack_148 [8];\r\n  undefined4 uStack_140;\r\n  undefined4 uStack_13c;\r\n  undefined8 uStack_138;\r\n  undefined4 uStack_130;\r\n  undefined4 uStack_12c;\r\n  undefined4 uStack_128;\r\n  undefined4 uStack_124;\r\n  undefined8 uStack_120;\r\n  undefined4 uStack_118;\r\n  undefined4 uStack_114;\r\n  undefined4 uStack_110;\r\n  undefined4 uStack_10c;\r\n  undefined8 uStack_108;\r\n  undefined4 uStack_100;\r\n  undefined4 uStack_fc;\r\n  undefined4 uStack_f8;\r\n  undefined4 uStack_f4;\r\n  undefined8 uStack_f0;\r\n  undefined8 uStack_e8;\r\n  undefined8 uStack_e0;\r\n  undefined8 uStack_d8;\r\n  undefined4 uStack_d0;\r\n  undefined8 uStack_c8;\r\n  undefined4 uStack_c0;\r\n  undefined8 uStack_b8;\r\n  undefined4 uStack_b0;\r\n  undefined4 uStack_a8;\r\n  \r\n  uStack_150 = 0xfffffffffffffffe;\r\n  uStack_204 = uStack_204 & 0xffffff00;\r\n  uVar5 = *(undefined8 *)(param_1 + 0x20);\r\n  FUN_140559760(auStack_148);\r\n  FUN_140571960(param_1,auStack_148);\r\n  pcVar4 = (char *)FUN_14055f8c0(*(undefined8 *)(param_1 + 0x28));\r\n  uVar7 = 0;\r\n  if (*pcVar4 == '\\0') {\r\n    iVar1 = *(int *)(param_1 + 0xb0);\r\n    if (iVar1 == 1) {\r\n      lVar6 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x10);\r\n      cVar3 = FUN_140584660(*(undefined8 *)(param_1 + 0xb8));\r\n      if ((cVar3 == '\\0') ||\r\n         (*(longlong *)(*(longlong *)(param_1 + 0xb8) + 0x28) != *(longlong *)(lVar6 + 0x38))) {\r\n        bVar2 = false;\r\n      }\r\n      else {\r\n        bVar2 = true;\r\n      }\r\n      cVar3 = FUN_1405bfdd0(uStack_a8);\r\n      if ((cVar3 == '\\0') || (bVar2)) {\r\n        cVar3 = FUN_14055e850(auStack_148);\r\n        if (cVar3 == '\\0') {\r\n          lVar6 = FUN_140571910(param_1);\r\n          lVar6 = *(longlong *)(lVar6 + 0x3a8);\r\n        }\r\n        else {\r\n          lVar6 = FUN_140571910(param_1);\r\n          lVar6 = *(longlong *)(lVar6 + 0x3a0);\r\n        }\r\n      }\r\n      else {\r\n        lVar6 = FUN_140571910(param_1);\r\n        lVar6 = *(longlong *)(lVar6 + 0x3b0);\r\n      }\r\n      if (lVar6 == 0) {\r\n        FUN_1405a48e0(param_1,auStack_148);\r\n        uVar7 = 1;\r\n      }\r\n      else {\r\n        FUN_1407c7e50(param_1 + 0x40,*(longlong *)(*(longlong *)(param_1 + 0x10) + 0x10) + 0x40,\r\n                      uVar5);\r\n        cVar3 = FUN_140565950(param_1,lVar6,0,0,0);\r\n        uVar7 = 0;\r\n        if (cVar3 != '\\0') {\r\n          uVar7 = 2;\r\n          *(undefined4 *)(param_1 + 0xb0) = 2;\r\n        }\r\n      }\r\n    }\r\n    else if (iVar1 == 2) {\r\n      uVar7 = 2;\r\n    }\r\n    else if (iVar1 == 3) {\r\n      FUN_1407c7770(param_1 + 0x40,1);\r\n    }\r\n    else {\r\n      FUN_1407c7770(param_1 + 0x40,1);\r\n      uStack_1f0 = uStack_140;\r\n      uStack_1ec = uStack_13c;\r\n      uStack_1e8 = uStack_138;\r\n      uStack_1e0 = uStack_130;\r\n      uStack_1dc = uStack_12c;\r\n      uStack_1d8 = uStack_128;\r\n      uStack_1d4 = uStack_124;\r\n      uStack_1d0 = uStack_120;\r\n      uStack_1c8 = uStack_118;\r\n      uStack_1c4 = uStack_114;\r\n      uStack_1c0 = uStack_110;\r\n      uStack_1bc = uStack_10c;\r\n      uStack_1b8 = uStack_108;\r\n      uStack_1b0 = uStack_100;\r\n      uStack_1ac = uStack_fc;\r\n      uStack_1a8 = uStack_f8;\r\n      uStack_1a4 = uStack_f4;\r\n      uStack_1a0 = uStack_f0;\r\n      uStack_198 = uStack_e8;\r\n      uStack_190 = uStack_e0;\r\n      uStack_188 = uStack_d8;\r\n      uStack_180 = uStack_d0;\r\n      uStack_178 = uStack_c8;\r\n      uStack_170 = uStack_c0;\r\n      uStack_168 = uStack_b8;\r\n      uStack_160 = uStack_b0;\r\n      ppuStack_1f8 = &PTR_FUN_141b44720;\r\n      bStack_158 = bStack_158 & 0xfe;\r\n      FUN_1403e4da0(*(undefined8 *)(param_1 + 0x30),&ppuStack_1f8);\r\n      uVar7 = *(undefined4 *)(*(longlong *)(param_1 + 0x20) + 0xa8);\r\n      uVar5 = FUN_140810240();\r\n      FUN_1408103f0(uVar5,0x30,uVar7);\r\n      uVar7 = 1;\r\n    }\r\n  }\r\n  *param_2 = uVar7;\r\n  param_2[1] = uStack_204;\r\n  *(undefined8 *)(param_2 + 2) = 0xffffffffffffffff;\r\n  return param_2;\r\n}\r\n\r\n",
  "runtimeEvidence": "work/natural-moveset/mesmerize-links-receiver-pause-v109/crash-snapshot/client/loader.log",
  "confirmedActor": "Corvo custom presentation clone handle0c8311e6 =209916390",
  "fixCandidate": "candidate-avatar-ai-lifetime-v111",
  "runtimeVerified": false
}

````

## Unescaped text fields for reading

### decompilation

````text

undefined4 * FUN_1405a4b00(longlong param_1,undefined4 *param_2)

{
  int iVar1;
  bool bVar2;
  char cVar3;
  char *pcVar4;
  undefined8 uVar5;
  longlong lVar6;
  undefined4 uVar7;
  uint uStack_204;
  undefined **ppuStack_1f8;
  undefined4 uStack_1f0;
  undefined4 uStack_1ec;
  undefined8 uStack_1e8;
  undefined4 uStack_1e0;
  undefined4 uStack_1dc;
  undefined4 uStack_1d8;
  undefined4 uStack_1d4;
  undefined8 uStack_1d0;
  undefined4 uStack_1c8;
  undefined4 uStack_1c4;
  undefined4 uStack_1c0;
  undefined4 uStack_1bc;
  undefined8 uStack_1b8;
  undefined4 uStack_1b0;
  undefined4 uStack_1ac;
  undefined4 uStack_1a8;
  undefined4 uStack_1a4;
  undefined8 uStack_1a0;
  undefined8 uStack_198;
  undefined8 uStack_190;
  undefined8 uStack_188;
  undefined4 uStack_180;
  undefined8 uStack_178;
  undefined4 uStack_170;
  undefined8 uStack_168;
  undefined4 uStack_160;
  byte bStack_158;
  undefined8 uStack_150;
  undefined1 auStack_148 [8];
  undefined4 uStack_140;
  undefined4 uStack_13c;
  undefined8 uStack_138;
  undefined4 uStack_130;
  undefined4 uStack_12c;
  undefined4 uStack_128;
  undefined4 uStack_124;
  undefined8 uStack_120;
  undefined4 uStack_118;
  undefined4 uStack_114;
  undefined4 uStack_110;
  undefined4 uStack_10c;
  undefined8 uStack_108;
  undefined4 uStack_100;
  undefined4 uStack_fc;
  undefined4 uStack_f8;
  undefined4 uStack_f4;
  undefined8 uStack_f0;
  undefined8 uStack_e8;
  undefined8 uStack_e0;
  undefined8 uStack_d8;
  undefined4 uStack_d0;
  undefined8 uStack_c8;
  undefined4 uStack_c0;
  undefined8 uStack_b8;
  undefined4 uStack_b0;
  undefined4 uStack_a8;
  
  uStack_150 = 0xfffffffffffffffe;
  uStack_204 = uStack_204 & 0xffffff00;
  uVar5 = *(undefined8 *)(param_1 + 0x20);
  FUN_140559760(auStack_148);
  FUN_140571960(param_1,auStack_148);
  pcVar4 = (char *)FUN_14055f8c0(*(undefined8 *)(param_1 + 0x28));
  uVar7 = 0;
  if (*pcVar4 == '\0') {
    iVar1 = *(int *)(param_1 + 0xb0);
    if (iVar1 == 1) {
      lVar6 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x10);
      cVar3 = FUN_140584660(*(undefined8 *)(param_1 + 0xb8));
      if ((cVar3 == '\0') ||
         (*(longlong *)(*(longlong *)(param_1 + 0xb8) + 0x28) != *(longlong *)(lVar6 + 0x38))) {
        bVar2 = false;
      }
      else {
        bVar2 = true;
      }
      cVar3 = FUN_1405bfdd0(uStack_a8);
      if ((cVar3 == '\0') || (bVar2)) {
        cVar3 = FUN_14055e850(auStack_148);
        if (cVar3 == '\0') {
          lVar6 = FUN_140571910(param_1);
          lVar6 = *(longlong *)(lVar6 + 0x3a8);
        }
        else {
          lVar6 = FUN_140571910(param_1);
          lVar6 = *(longlong *)(lVar6 + 0x3a0);
        }
      }
      else {
        lVar6 = FUN_140571910(param_1);
        lVar6 = *(longlong *)(lVar6 + 0x3b0);
      }
      if (lVar6 == 0) {
        FUN_1405a48e0(param_1,auStack_148);
        uVar7 = 1;
      }
      else {
        FUN_1407c7e50(param_1 + 0x40,*(longlong *)(*(longlong *)(param_1 + 0x10) + 0x10) + 0x40,
                      uVar5);
        cVar3 = FUN_140565950(param_1,lVar6,0,0,0);
        uVar7 = 0;
        if (cVar3 != '\0') {
          uVar7 = 2;
          *(undefined4 *)(param_1 + 0xb0) = 2;
        }
      }
    }
    else if (iVar1 == 2) {
      uVar7 = 2;
    }
    else if (iVar1 == 3) {
      FUN_1407c7770(param_1 + 0x40,1);
    }
    else {
      FUN_1407c7770(param_1 + 0x40,1);
      uStack_1f0 = uStack_140;
      uStack_1ec = uStack_13c;
      uStack_1e8 = uStack_138;
      uStack_1e0 = uStack_130;
      uStack_1dc = uStack_12c;
      uStack_1d8 = uStack_128;
      uStack_1d4 = uStack_124;
      uStack_1d0 = uStack_120;
      uStack_1c8 = uStack_118;
      uStack_1c4 = uStack_114;
      uStack_1c0 = uStack_110;
      uStack_1bc = uStack_10c;
      uStack_1b8 = uStack_108;
      uStack_1b0 = uStack_100;
      uStack_1ac = uStack_fc;
      uStack_1a8 = uStack_f8;
      uStack_1a4 = uStack_f4;
      uStack_1a0 = uStack_f0;
      uStack_198 = uStack_e8;
      uStack_190 = uStack_e0;
      uStack_188 = uStack_d8;
      uStack_180 = uStack_d0;
      uStack_178 = uStack_c8;
      uStack_170 = uStack_c0;
      uStack_168 = uStack_b8;
      uStack_160 = uStack_b0;
      ppuStack_1f8 = &PTR_FUN_141b44720;
      bStack_158 = bStack_158 & 0xfe;
      FUN_1403e4da0(*(undefined8 *)(param_1 + 0x30),&ppuStack_1f8);
      uVar7 = *(undefined4 *)(*(longlong *)(param_1 + 0x20) + 0xa8);
      uVar5 = FUN_140810240();
      FUN_1408103f0(uVar5,0x30,uVar7);
      uVar7 = 1;
    }
  }
  *param_2 = uVar7;
  param_2[1] = uStack_204;
  *(undefined8 *)(param_2 + 2) = 0xffffffffffffffff;
  return param_2;
}


````
