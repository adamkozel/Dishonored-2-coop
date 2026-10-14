# ghidra-native-carry-source-v2.json

Original: [work/natural-moveset/carry-research/ghidra-native-carry-source-v2.json](<../../../../work/natural-moveset/carry-research/ghidra-native-carry-source-v2.json>)

SHA-256: `A7AF435542821F4335BE22207E13D2FFE68430AD90D78986CC0B61E4F35B5CCB`; 15566 bytes.

## Original saved record

````json
{
  "program": "Dishonored2.exe",
  "executableSha256": "B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180",
  "scope": "Static decompilation supporting runtime-confirmed Carry45/Drop46 source observation. Remote binding remains incomplete.",
  "functions": {
    "140828770": {
      "result": "\r\nvoid FUN_140828770(longlong param_1,int param_2)\r\n\r\n{\r\n  longlong lVar1;\r\n  \r\n  lVar1 = FUN_1406dc560(*(undefined4 *)(param_1 + 0xf8));\r\n  if (lVar1 == 0) {\r\n    FUN_140828440(param_1,\"Corpse destroyed\",0,1);\r\n    return;\r\n  }\r\n                    /* WARNING: Could not recover jumptable at 0x0001408287d2. Too many branches */\r\n                    /* WARNING: Treating indirect jump as call */\r\n  (*(code *)(IMAGE_DOS_HEADER_140000000.e_magic + *(uint *)(&DAT_140828ca4 + (longlong)param_2 * 4))\r\n  )(IMAGE_DOS_HEADER_140000000.e_magic + *(uint *)(&DAT_140828ca4 + (longlong)param_2 * 4));\r\n  return;\r\n}\r\n\r\n"
    },
    "140829030": {
      "result": "\r\nvoid FUN_140829030(longlong param_1,longlong param_2)\r\n\r\n{\r\n  byte bVar1;\r\n  longlong lVar2;\r\n  char cVar3;\r\n  longlong lVar4;\r\n  undefined8 uVar5;\r\n  uint local_58 [2];\r\n  undefined1 local_50 [16];\r\n  undefined1 local_40 [40];\r\n  undefined2 local_18;\r\n  \r\n  lVar4 = FUN_1403da910(param_2 + 8,2);\r\n  lVar2 = *(longlong *)(lVar4 + 0x10);\r\n  if (lVar2 == 0) {\r\n    *(undefined4 *)(param_1 + 0xf8) = 0x3fff0010;\r\n  }\r\n  else {\r\n    *(undefined4 *)(param_1 + 0xf8) = *(undefined4 *)(lVar2 + 0xa8);\r\n  }\r\n  uVar5 = FUN_140877720(lVar2,&DAT_1439b4630,0);\r\n  FUN_1403dab80(param_1 + 0x100,uVar5);\r\n  uVar5 = *(undefined8 *)(param_1 + 0x10);\r\n  *(byte *)(param_1 + 0xd0) = *(byte *)(param_1 + 0xd0) & 0xdf;\r\n  *(byte *)(param_1 + 0xd0) = *(byte *)(param_1 + 0xd0) | 0x80;\r\n  cVar3 = FUN_140803180(uVar5,1);\r\n  if (cVar3 != '\\0') {\r\n    FUN_1408022e0(uVar5,0);\r\n  }\r\n  if ((*(byte *)(param_1 + 0xd0) & 0x10) == 0) {\r\n    local_58[1] = 0x5c;\r\n    DAT_143994a30 = DAT_143994a30 + 1 & 0x7fffffff;\r\n    if (DAT_143994a30 == 0) {\r\n      DAT_143994a30 = 1;\r\n    }\r\n    local_58[0] = DAT_143994a30;\r\n    FUN_1403da8f0(local_50,lVar4);\r\n    FUN_14077c2d0(local_40);\r\n    local_18 = 0;\r\n    FUN_1408024d0(uVar5,0x2f,local_58);\r\n  }\r\n  FUN_140828770(param_1,1);\r\n  bVar1 = *(byte *)(param_1 + 0xd0);\r\n  if ((((bVar1 & 1) != 0) || ((bVar1 & 4) != 0)) || ((bVar1 & 0x10) != 0)) {\r\n    FUN_140828770(param_1,2);\r\n  }\r\n  if ((*(byte *)(param_1 + 0xd0) & 0x10) != 0) {\r\n    FUN_140828770(param_1,3);\r\n  }\r\n  return;\r\n}\r\n\r\n"
    },
    "140829870": {
      "result": "\r\nvoid FUN_140829870(longlong param_1)\r\n\r\n{\r\n  longlong lVar1;\r\n  longlong lVar2;\r\n  byte bVar3;\r\n  char cVar4;\r\n  byte bVar5;\r\n  undefined8 uVar6;\r\n  longlong lVar7;\r\n  longlong lVar8;\r\n  \r\n  lVar1 = *(longlong *)(param_1 + 0x10);\r\n  lVar2 = *(longlong *)(param_1 + 0x18);\r\n  lVar7 = 0;\r\n  lVar8 = lVar7;\r\n  if (*(longlong *)(lVar1 + 0x90) != 0) {\r\n    lVar8 = *(longlong *)(*(longlong *)(lVar1 + 0x90) + 0x78);\r\n  }\r\n  bVar3 = FUN_140803180(lVar1,0xd);\r\n  *(byte *)(param_1 + 0xd0) = *(byte *)(param_1 + 0xd0) & 0xfe;\r\n  *(byte *)(param_1 + 0xd0) = *(byte *)(param_1 + 0xd0) | bVar3 & 1;\r\n  bVar3 = *(byte *)(param_1 + 0xd0);\r\n  if ((((bVar3 & 1) == 0) || (lVar8 == 0)) || ((*(byte *)(lVar8 + 0x13c) & 8) == 0)) {\r\n    cVar4 = '\\0';\r\n  }\r\n  else {\r\n    cVar4 = '\\x01';\r\n  }\r\n  *(byte *)(param_1 + 0xd0) = (cVar4 * '\\x02' ^ bVar3) & 2 ^ bVar3;\r\n  bVar3 = FUN_140803180(lVar1,0x29);\r\n  *(byte *)(param_1 + 0xd0) = *(byte *)(param_1 + 0xd0) & 0xfb;\r\n  *(byte *)(param_1 + 0xd0) = *(byte *)(param_1 + 0xd0) | (bVar3 & 1) << 2;\r\n  bVar5 = FUN_140803180(lVar1,0x54);\r\n  bVar3 = *(byte *)(param_1 + 0xd0);\r\n  *(undefined8 *)(param_1 + 0xd8) = 0;\r\n  *(byte *)(param_1 + 0xd0) = (bVar5 & 1) << 3 | bVar3 & 0xe7;\r\n  if ((bVar3 & 1) == 0) {\r\n    if ((bVar3 & 4) == 0) {\r\n      if ((bVar5 & 1) == 0) goto LAB_14082997d;\r\n      uVar6 = *(undefined8 *)(lVar2 + 0xa8);\r\n    }\r\n    else {\r\n      if (*(longlong *)(lVar1 + 0x90) != 0) {\r\n        lVar7 = *(longlong *)(*(longlong *)(lVar1 + 0x90) + 0x158);\r\n      }\r\n      uVar6 = FUN_14082bc80(lVar7);\r\n    }\r\n  }\r\n  else {\r\n    uVar6 = *(undefined8 *)(lVar8 + 0x130);\r\n  }\r\n  *(undefined8 *)(param_1 + 0xd8) = uVar6;\r\nLAB_14082997d:\r\n  if (*(longlong *)(param_1 + 0xd8) == 0) {\r\n    *(undefined8 *)(param_1 + 0xd8) = *(undefined8 *)(lVar2 + 0xa0);\r\n  }\r\n  return;\r\n}\r\n\r\n"
    },
    "140832550": {
      "result": "\r\nvoid FUN_140832550(longlong param_1)\r\n\r\n{\r\n  undefined4 uVar1;\r\n  longlong lVar2;\r\n  undefined8 uVar3;\r\n  longlong lVar4;\r\n  undefined4 *puVar5;\r\n  longlong lStack_18;\r\n  code *pcStack_10;\r\n  \r\n  FUN_1408541a0();\r\n  pcStack_10 = FUN_1400998b0;\r\n  lStack_18 = param_1;\r\n  uVar3 = FUN_140810240();\r\n  FUN_140811040(uVar3,0xb7,&lStack_18);\r\n  pcStack_10 = FUN_140832b10;\r\n  lStack_18 = param_1;\r\n  uVar3 = FUN_140810240();\r\n  FUN_140811040(uVar3,0xb9,&lStack_18);\r\n  pcStack_10 = FUN_140832ad0;\r\n  lStack_18 = param_1;\r\n  uVar3 = FUN_140810240();\r\n  FUN_140811040(uVar3,0xbb,&lStack_18);\r\n  lVar2 = *(longlong *)(param_1 + 0x10);\r\n  if (*(char *)(param_1 + 0x52) != '\\0') {\r\n    uVar1 = *(undefined4 *)(*(longlong *)(lVar2 + 0x10) + 0xa8);\r\n    lVar4 = FUN_140810240();\r\n    puVar5 = (undefined4 *)FUN_1408112c0(lVar4 + 0xc8e8,0x17c,4,uVar1);\r\n    if (puVar5 != (undefined4 *)0x0) {\r\n      *puVar5 = 4;\r\n    }\r\n    *(undefined1 *)(param_1 + 0x52) = 0;\r\n  }\r\n  lVar2 = *(longlong *)(lVar2 + 0x90);\r\n  lVar4 = 0;\r\n  if (lVar2 != 0) {\r\n    lVar4 = *(longlong *)(lVar2 + 0x178);\r\n  }\r\n  if (*(int *)(lVar4 + 0x110) != 0) {\r\n    if (*(char *)(param_1 + 0x51) != '\\0') {\r\n      FUN_140829f20();\r\n      return;\r\n    }\r\n    FUN_140829f90();\r\n    FUN_140854d80(param_1);\r\n    return;\r\n  }\r\n  return;\r\n}\r\n\r\n"
    },
    "140832770": {
      "result": "\r\nvoid FUN_140832770(longlong param_1)\r\n\r\n{\r\n  int iVar1;\r\n  uint uVar2;\r\n  longlong lVar3;\r\n  longlong lVar4;\r\n  undefined8 uVar5;\r\n  uint uVar6;\r\n  undefined4 local_res8 [2];\r\n  undefined1 local_38 [8];\r\n  uint local_30;\r\n  undefined1 local_20 [24];\r\n  \r\n  uVar5 = 0;\r\n  lVar4 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x90);\r\n  if (lVar4 != 0) {\r\n    uVar5 = *(undefined8 *)(lVar4 + 0x178);\r\n  }\r\n  FUN_140829e30(uVar5);\r\n  iVar1 = FUN_1403a6f10(*(undefined8 *)(param_1 + 0x30),*(undefined1 *)(param_1 + 0x50));\r\n  lVar4 = *(longlong *)(param_1 + 0x18);\r\n  if (iVar1 == 0) {\r\n    lVar4 = *(longlong *)(lVar4 + 0x70);\r\n  }\r\n  else if (iVar1 == 1) {\r\n    lVar4 = *(longlong *)(lVar4 + 0x78);\r\n  }\r\n  else if (iVar1 == 2) {\r\n    lVar4 = *(longlong *)(lVar4 + 0x80);\r\n  }\r\n  else if (iVar1 == 3) {\r\n    uVar6 = *(uint *)(lVar4 + 0x90) & 0xffffff;\r\n    if (uVar6 == 0) goto LAB_1408328d4;\r\n    uVar2 = *(int *)(D2_GameSingletonPtr + 0x600f8) * 0x19660d + 0x3c6ef35f;\r\n    *(uint *)(D2_GameSingletonPtr + 0x600f8) = uVar2;\r\n    lVar4 = *(longlong *)\r\n             (*(longlong *)(lVar4 + 0x88) +\r\n             (longlong)(int)((ulonglong)(uVar2 >> 10 & 0x7fff) % (ulonglong)(longlong)(int)uVar6) *\r\n             8);\r\n  }\r\n  else {\r\n    lVar4 = *(longlong *)(lVar4 + 0x98);\r\n  }\r\n  if (lVar4 != 0) {\r\n    lVar3 = FUN_1406dc560(*(undefined4 *)(param_1 + 0x38));\r\n    if (lVar3 != 0) {\r\n      local_res8[0] = 0;\r\n      FUN_140854ab0(param_1,local_res8,lVar4,0x25,lVar3,0);\r\n      local_30 = (uint)*(byte *)(param_1 + 0x50);\r\n      *(int *)(param_1 + 0x54) = iVar1;\r\n      *(undefined4 *)(param_1 + 0x58) = 0;\r\n      local_38[0] = 0x69;\r\n      FUN_140884920(lVar3,local_20,&DAT_1439ac250,local_38);\r\n      FUN_140882fc0(local_20);\r\n      *(byte *)(lVar3 + 0x368) = *(byte *)(lVar3 + 0x368) & 0xfb;\r\n      *(byte *)(lVar3 + 0x368) = *(byte *)(lVar3 + 0x368) | (iVar1 == 4) << 2;\r\n    }\r\n    lVar4 = FUN_1403da910(param_1 + 0x40,2);\r\n    if (lVar4 != 0) {\r\n      lVar3 = FUN_140854880(param_1);\r\n      local_res8[0] = *(undefined4 *)(lVar3 + 0xa8);\r\n      FUN_1407278e0(lVar4,local_res8);\r\n      *(byte *)(lVar4 + 0x241) = *(byte *)(lVar4 + 0x241) | 0x20;\r\n      FUN_140735830(lVar4);\r\n      if (iVar1 == 2) {\r\n        *(byte *)(lVar4 + 0x241) = *(byte *)(lVar4 + 0x241) | 2;\r\n      }\r\n    }\r\n    return;\r\n  }\r\nLAB_1408328d4:\r\n                    /* WARNING: Subroutine does not return */\r\n  FUN_140166830(\"No midnight to start for the corpse drop\");\r\n}\r\n\r\n"
    },
    "140832980": {
      "result": "\r\nvoid FUN_140832980(longlong param_1)\r\n\r\n{\r\n  undefined4 uVar1;\r\n  char cVar2;\r\n  int iVar3;\r\n  undefined8 *puVar4;\r\n  longlong lVar5;\r\n  float fVar6;\r\n  uint uStack_c;\r\n  \r\n  if (*(char *)(param_1 + 0x51) == '\\0') {\r\n    fVar6 = (float)FUN_1407d5ec0(D2_GameSingletonPtr + 0x1f5338,*(undefined4 *)(param_1 + 0x24),0);\r\n    if (fVar6 == -1.0) {\r\n      FUN_140854190(param_1);\r\n      return;\r\n    }\r\n    if (*(int *)(param_1 + 0x54) == 4) {\r\n      *(int *)(param_1 + 0x58) = *(int *)(param_1 + 0x58) + 1;\r\n      if (*(int *)(param_1 + 0x58) == 3) {\r\n        uVar1 = *(undefined4 *)(param_1 + 0x38);\r\n        lVar5 = FUN_140810240();\r\n        puVar4 = (undefined8 *)FUN_1408112c0(lVar5 + 0xc8e8,0xb9,0x10,uVar1);\r\n        if (puVar4 != (undefined8 *)0x0) {\r\n          *puVar4 = 0;\r\n          puVar4[1] = (ulonglong)uStack_c << 0x20;\r\n          return;\r\n        }\r\n      }\r\n    }\r\n    else {\r\n      iVar3 = FUN_1403a6f10(*(undefined8 *)(param_1 + 0x30),*(undefined1 *)(param_1 + 0x50));\r\n      if (((iVar3 != *(int *)(param_1 + 0x54)) &&\r\n          (lVar5 = FUN_1406dc560(*(undefined4 *)(param_1 + 0x38)), lVar5 != 0)) &&\r\n         (cVar2 = FUN_1405bf440(lVar5), cVar2 == '\\0')) {\r\n        uVar1 = *(undefined4 *)(param_1 + 0x38);\r\n        lVar5 = FUN_140810240();\r\n        puVar4 = (undefined8 *)FUN_1408112c0(lVar5 + 0xc8e8,0xb9,0x10,uVar1);\r\n        if (puVar4 != (undefined8 *)0x0) {\r\n          *puVar4 = 0;\r\n          puVar4[1] = CONCAT44(uStack_c,2);\r\n        }\r\n        FUN_140854d80(param_1);\r\n        FUN_140854190(param_1);\r\n      }\r\n    }\r\n  }\r\n  else {\r\n    cVar2 = FUN_140832420();\r\n    if (cVar2 != '\\0') {\r\n      *(undefined1 *)(param_1 + 0x51) = 0;\r\n      FUN_140832770(param_1);\r\n      return;\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
    },
    "140828d90": {
      "result": "\r\nvoid FUN_140828d90(longlong param_1)\r\n\r\n{\r\n  FUN_1408541a0();\r\n  if ((*(uint *)(param_1 + 0x110) & 0xfffffffb) != 0) {\r\n    FUN_140828440(param_1,\"State deactivated\",0,1);\r\n  }\r\n  if ((*(byte *)(param_1 + 0xd0) & 0x20) != 0) {\r\n    FUN_14060c9b0(*(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x330),param_1 + 0x118);\r\n    *(byte *)(param_1 + 0xd0) = *(byte *)(param_1 + 0xd0) & 0xdf;\r\n  }\r\n  return;\r\n}\r\n\r\n"
    },
    "140829ff0": {
      "result": "\r\nvoid FUN_140829ff0(longlong param_1)\r\n\r\n{\r\n  longlong lVar1;\r\n  \r\n  lVar1 = FUN_1406dc560(*(undefined4 *)(param_1 + 0xf8));\r\n  if (lVar1 == 0) {\r\n    FUN_140828440(param_1,\"Corpse destroyed\",0,1);\r\n    return;\r\n  }\r\n  FUN_14082a110(param_1,lVar1);\r\n  FUN_14082a040(param_1);\r\n  return;\r\n}\r\n\r\n"
    },
    "140829b10": {
      "result": "\r\n/* WARNING: Type propagation algorithm not settling */\r\n\r\nvoid FUN_140829b10(longlong param_1,longlong param_2)\r\n\r\n{\r\n  undefined8 uVar1;\r\n  longlong lVar2;\r\n  longlong *plVar3;\r\n  byte bVar4;\r\n  longlong alStackX_10 [3];\r\n  undefined1 auStack_68 [80];\r\n  \r\n  if (*(int *)(param_2 + 0x10) != 0) {\r\n    alStackX_10[0] = CONCAT44(alStackX_10[0]._4_4_,*(undefined4 *)(param_2 + 8));\r\n    uVar1 = FUN_1407b5e00(D2_GameSingletonPtr + 0x3130b0,alStackX_10);\r\n    lVar2 = FUN_1408f2e50(uVar1);\r\n    if (((lVar2 != 0) &&\r\n        (plVar3 = (longlong *)FUN_140877720(lVar2,&DAT_1439d1320,2), plVar3 != (longlong *)0x0)) &&\r\n       (*(byte *)(lVar2 + 0x368) = *(byte *)(lVar2 + 0x368) & 0xfd, *(int *)(param_2 + 0x10) < 4)) {\r\n      lVar2 = *plVar3;\r\n      alStackX_10[1] = 0;\r\n      alStackX_10[0] = 0;\r\n      uVar1 = FUN_140854880(param_1);\r\n      (**(code **)(lVar2 + 0xd0))(plVar3,uVar1,alStackX_10 + 1,alStackX_10);\r\n      if (alStackX_10[0] != 0) {\r\n        *(byte *)(param_1 + 0xd0) =\r\n             *(byte *)(param_1 + 0xd0) ^ (*(byte *)(param_2 + 0xc) ^ *(byte *)(param_1 + 0xd0)) & 1;\r\n        bVar4 = (*(byte *)(param_2 + 0xc) ^ *(byte *)(param_1 + 0xd0)) & 2 ^\r\n                *(byte *)(param_1 + 0xd0);\r\n        *(byte *)(param_1 + 0xd0) = bVar4;\r\n        bVar4 = (*(byte *)(param_2 + 0xc) ^ bVar4) & 4 ^ bVar4;\r\n        *(byte *)(param_1 + 0xd0) = bVar4;\r\n        *(byte *)(param_1 + 0xd0) = *(byte *)(param_2 + 0xc) & 8 | bVar4 & 0xf7 | 0x10;\r\n        FUN_1403b7aa0(auStack_68,plVar3,alStackX_10[0],0);\r\n        FUN_140829030(param_1,auStack_68);\r\n      }\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
    },
    "140831fe0": {
      "result": "\r\nvoid FUN_140831fe0(longlong param_1)\r\n\r\n{\r\n  undefined4 uVar1;\r\n  longlong lVar2;\r\n  char cVar3;\r\n  int iVar4;\r\n  longlong lVar5;\r\n  undefined8 uVar6;\r\n  longlong lVar7;\r\n  undefined4 *puVar8;\r\n  longlong lVar9;\r\n  longlong lStack_18;\r\n  code *pcStack_10;\r\n  \r\n  FUN_140853d40();\r\n  lVar7 = *(longlong *)(param_1 + 0x10);\r\n  lVar9 = 0;\r\n  lVar5 = lVar9;\r\n  if (*(longlong *)(lVar7 + 0x90) != 0) {\r\n    lVar5 = *(longlong *)(*(longlong *)(lVar7 + 0x90) + 0x178);\r\n  }\r\n  *(undefined4 *)(param_1 + 0x38) = *(undefined4 *)(lVar5 + 0xf8);\r\n  lVar5 = FUN_1406dc560();\r\n  if (lVar5 == 0) {\r\n    FUN_1403dab80(param_1 + 0x40,0);\r\n  }\r\n  else {\r\n    uVar6 = FUN_140877720(lVar5,&DAT_1439b4630,2);\r\n    FUN_1403dab80(param_1 + 0x40,uVar6);\r\n    uVar1 = *(undefined4 *)(lVar5 + 0xa8);\r\n    uVar6 = FUN_140810240();\r\n    FUN_1408103f0(uVar6,0x1cc,uVar1);\r\n  }\r\n  lVar2 = *(longlong *)(lVar7 + 0x2f8);\r\n  if ((((byte)(*(uint *)(lVar2 + 0x88) >> 0x13) & (byte)(*(uint *)(lVar2 + 0x80) >> 0x13) & 1) != 0)\r\n     && (*(int *)(lVar2 + 0x224) == 0)) {\r\n    lVar9 = 1;\r\n  }\r\n  *(char *)(param_1 + 0x50) = (char)lVar9;\r\n  cVar3 = FUN_140832420(param_1);\r\n  if (cVar3 == '\\0') {\r\n    *(undefined1 *)(param_1 + 0x51) = 1;\r\n  }\r\n  else {\r\n    *(undefined1 *)(param_1 + 0x51) = 0;\r\n    FUN_140832770(param_1);\r\n  }\r\n  if (lVar5 != 0) {\r\n    iVar4 = FUN_140604bd0(D2_GameSingletonPtr + 0x1f7bb8,lVar5,\r\n                          *(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x10));\r\n    if (iVar4 == 0) {\r\n      uVar1 = *(undefined4 *)(*(longlong *)(lVar7 + 0x10) + 0xa8);\r\n      lVar7 = FUN_140810240();\r\n      puVar8 = (undefined4 *)FUN_1408112c0(lVar7 + 0xc8e8,0x17b,4,uVar1);\r\n      if (puVar8 != (undefined4 *)0x0) {\r\n        *puVar8 = 4;\r\n      }\r\n      *(undefined1 *)(param_1 + 0x52) = 1;\r\n    }\r\n  }\r\n  pcStack_10 = FUN_1400998b0;\r\n  lStack_18 = param_1;\r\n  uVar6 = FUN_140810240();\r\n  FUN_1408106f0(uVar6,0xb7,&lStack_18);\r\n  pcStack_10 = FUN_140832b10;\r\n  lStack_18 = param_1;\r\n  uVar6 = FUN_140810240();\r\n  FUN_1408106f0(uVar6,0xb9,&lStack_18);\r\n  pcStack_10 = FUN_140832ad0;\r\n  lStack_18 = param_1;\r\n  uVar6 = FUN_140810240();\r\n  FUN_1408106f0(uVar6,0xbb,&lStack_18);\r\n  return;\r\n}\r\n\r\n"
    }
  }
}

````

## Unescaped text fields for reading

### result

````text

void FUN_140828770(longlong param_1,int param_2)

{
  longlong lVar1;
  
  lVar1 = FUN_1406dc560(*(undefined4 *)(param_1 + 0xf8));
  if (lVar1 == 0) {
    FUN_140828440(param_1,"Corpse destroyed",0,1);
    return;
  }
                    /* WARNING: Could not recover jumptable at 0x0001408287d2. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(IMAGE_DOS_HEADER_140000000.e_magic + *(uint *)(&DAT_140828ca4 + (longlong)param_2 * 4))
  )(IMAGE_DOS_HEADER_140000000.e_magic + *(uint *)(&DAT_140828ca4 + (longlong)param_2 * 4));
  return;
}


````

### result

````text

void FUN_140829030(longlong param_1,longlong param_2)

{
  byte bVar1;
  longlong lVar2;
  char cVar3;
  longlong lVar4;
  undefined8 uVar5;
  uint local_58 [2];
  undefined1 local_50 [16];
  undefined1 local_40 [40];
  undefined2 local_18;
  
  lVar4 = FUN_1403da910(param_2 + 8,2);
  lVar2 = *(longlong *)(lVar4 + 0x10);
  if (lVar2 == 0) {
    *(undefined4 *)(param_1 + 0xf8) = 0x3fff0010;
  }
  else {
    *(undefined4 *)(param_1 + 0xf8) = *(undefined4 *)(lVar2 + 0xa8);
  }
  uVar5 = FUN_140877720(lVar2,&DAT_1439b4630,0);
  FUN_1403dab80(param_1 + 0x100,uVar5);
  uVar5 = *(undefined8 *)(param_1 + 0x10);
  *(byte *)(param_1 + 0xd0) = *(byte *)(param_1 + 0xd0) & 0xdf;
  *(byte *)(param_1 + 0xd0) = *(byte *)(param_1 + 0xd0) | 0x80;
  cVar3 = FUN_140803180(uVar5,1);
  if (cVar3 != '\0') {
    FUN_1408022e0(uVar5,0);
  }
  if ((*(byte *)(param_1 + 0xd0) & 0x10) == 0) {
    local_58[1] = 0x5c;
    DAT_143994a30 = DAT_143994a30 + 1 & 0x7fffffff;
    if (DAT_143994a30 == 0) {
      DAT_143994a30 = 1;
    }
    local_58[0] = DAT_143994a30;
    FUN_1403da8f0(local_50,lVar4);
    FUN_14077c2d0(local_40);
    local_18 = 0;
    FUN_1408024d0(uVar5,0x2f,local_58);
  }
  FUN_140828770(param_1,1);
  bVar1 = *(byte *)(param_1 + 0xd0);
  if ((((bVar1 & 1) != 0) || ((bVar1 & 4) != 0)) || ((bVar1 & 0x10) != 0)) {
    FUN_140828770(param_1,2);
  }
  if ((*(byte *)(param_1 + 0xd0) & 0x10) != 0) {
    FUN_140828770(param_1,3);
  }
  return;
}


````

### result

````text

void FUN_140829870(longlong param_1)

{
  longlong lVar1;
  longlong lVar2;
  byte bVar3;
  char cVar4;
  byte bVar5;
  undefined8 uVar6;
  longlong lVar7;
  longlong lVar8;
  
  lVar1 = *(longlong *)(param_1 + 0x10);
  lVar2 = *(longlong *)(param_1 + 0x18);
  lVar7 = 0;
  lVar8 = lVar7;
  if (*(longlong *)(lVar1 + 0x90) != 0) {
    lVar8 = *(longlong *)(*(longlong *)(lVar1 + 0x90) + 0x78);
  }
  bVar3 = FUN_140803180(lVar1,0xd);
  *(byte *)(param_1 + 0xd0) = *(byte *)(param_1 + 0xd0) & 0xfe;
  *(byte *)(param_1 + 0xd0) = *(byte *)(param_1 + 0xd0) | bVar3 & 1;
  bVar3 = *(byte *)(param_1 + 0xd0);
  if ((((bVar3 & 1) == 0) || (lVar8 == 0)) || ((*(byte *)(lVar8 + 0x13c) & 8) == 0)) {
    cVar4 = '\0';
  }
  else {
    cVar4 = '\x01';
  }
  *(byte *)(param_1 + 0xd0) = (cVar4 * '\x02' ^ bVar3) & 2 ^ bVar3;
  bVar3 = FUN_140803180(lVar1,0x29);
  *(byte *)(param_1 + 0xd0) = *(byte *)(param_1 + 0xd0) & 0xfb;
  *(byte *)(param_1 + 0xd0) = *(byte *)(param_1 + 0xd0) | (bVar3 & 1) << 2;
  bVar5 = FUN_140803180(lVar1,0x54);
  bVar3 = *(byte *)(param_1 + 0xd0);
  *(undefined8 *)(param_1 + 0xd8) = 0;
  *(byte *)(param_1 + 0xd0) = (bVar5 & 1) << 3 | bVar3 & 0xe7;
  if ((bVar3 & 1) == 0) {
    if ((bVar3 & 4) == 0) {
      if ((bVar5 & 1) == 0) goto LAB_14082997d;
      uVar6 = *(undefined8 *)(lVar2 + 0xa8);
    }
    else {
      if (*(longlong *)(lVar1 + 0x90) != 0) {
        lVar7 = *(longlong *)(*(longlong *)(lVar1 + 0x90) + 0x158);
      }
      uVar6 = FUN_14082bc80(lVar7);
    }
  }
  else {
    uVar6 = *(undefined8 *)(lVar8 + 0x130);
  }
  *(undefined8 *)(param_1 + 0xd8) = uVar6;
LAB_14082997d:
  if (*(longlong *)(param_1 + 0xd8) == 0) {
    *(undefined8 *)(param_1 + 0xd8) = *(undefined8 *)(lVar2 + 0xa0);
  }
  return;
}


````

### result

````text

void FUN_140832550(longlong param_1)

{
  undefined4 uVar1;
  longlong lVar2;
  undefined8 uVar3;
  longlong lVar4;
  undefined4 *puVar5;
  longlong lStack_18;
  code *pcStack_10;
  
  FUN_1408541a0();
  pcStack_10 = FUN_1400998b0;
  lStack_18 = param_1;
  uVar3 = FUN_140810240();
  FUN_140811040(uVar3,0xb7,&lStack_18);
  pcStack_10 = FUN_140832b10;
  lStack_18 = param_1;
  uVar3 = FUN_140810240();
  FUN_140811040(uVar3,0xb9,&lStack_18);
  pcStack_10 = FUN_140832ad0;
  lStack_18 = param_1;
  uVar3 = FUN_140810240();
  FUN_140811040(uVar3,0xbb,&lStack_18);
  lVar2 = *(longlong *)(param_1 + 0x10);
  if (*(char *)(param_1 + 0x52) != '\0') {
    uVar1 = *(undefined4 *)(*(longlong *)(lVar2 + 0x10) + 0xa8);
    lVar4 = FUN_140810240();
    puVar5 = (undefined4 *)FUN_1408112c0(lVar4 + 0xc8e8,0x17c,4,uVar1);
    if (puVar5 != (undefined4 *)0x0) {
      *puVar5 = 4;
    }
    *(undefined1 *)(param_1 + 0x52) = 0;
  }
  lVar2 = *(longlong *)(lVar2 + 0x90);
  lVar4 = 0;
  if (lVar2 != 0) {
    lVar4 = *(longlong *)(lVar2 + 0x178);
  }
  if (*(int *)(lVar4 + 0x110) != 0) {
    if (*(char *)(param_1 + 0x51) != '\0') {
      FUN_140829f20();
      return;
    }
    FUN_140829f90();
    FUN_140854d80(param_1);
    return;
  }
  return;
}


````

### result

````text

void FUN_140832770(longlong param_1)

{
  int iVar1;
  uint uVar2;
  longlong lVar3;
  longlong lVar4;
  undefined8 uVar5;
  uint uVar6;
  undefined4 local_res8 [2];
  undefined1 local_38 [8];
  uint local_30;
  undefined1 local_20 [24];
  
  uVar5 = 0;
  lVar4 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x90);
  if (lVar4 != 0) {
    uVar5 = *(undefined8 *)(lVar4 + 0x178);
  }
  FUN_140829e30(uVar5);
  iVar1 = FUN_1403a6f10(*(undefined8 *)(param_1 + 0x30),*(undefined1 *)(param_1 + 0x50));
  lVar4 = *(longlong *)(param_1 + 0x18);
  if (iVar1 == 0) {
    lVar4 = *(longlong *)(lVar4 + 0x70);
  }
  else if (iVar1 == 1) {
    lVar4 = *(longlong *)(lVar4 + 0x78);
  }
  else if (iVar1 == 2) {
    lVar4 = *(longlong *)(lVar4 + 0x80);
  }
  else if (iVar1 == 3) {
    uVar6 = *(uint *)(lVar4 + 0x90) & 0xffffff;
    if (uVar6 == 0) goto LAB_1408328d4;
    uVar2 = *(int *)(D2_GameSingletonPtr + 0x600f8) * 0x19660d + 0x3c6ef35f;
    *(uint *)(D2_GameSingletonPtr + 0x600f8) = uVar2;
    lVar4 = *(longlong *)
             (*(longlong *)(lVar4 + 0x88) +
             (longlong)(int)((ulonglong)(uVar2 >> 10 & 0x7fff) % (ulonglong)(longlong)(int)uVar6) *
             8);
  }
  else {
    lVar4 = *(longlong *)(lVar4 + 0x98);
  }
  if (lVar4 != 0) {
    lVar3 = FUN_1406dc560(*(undefined4 *)(param_1 + 0x38));
    if (lVar3 != 0) {
      local_res8[0] = 0;
      FUN_140854ab0(param_1,local_res8,lVar4,0x25,lVar3,0);
      local_30 = (uint)*(byte *)(param_1 + 0x50);
      *(int *)(param_1 + 0x54) = iVar1;
      *(undefined4 *)(param_1 + 0x58) = 0;
      local_38[0] = 0x69;
      FUN_140884920(lVar3,local_20,&DAT_1439ac250,local_38);
      FUN_140882fc0(local_20);
      *(byte *)(lVar3 + 0x368) = *(byte *)(lVar3 + 0x368) & 0xfb;
      *(byte *)(lVar3 + 0x368) = *(byte *)(lVar3 + 0x368) | (iVar1 == 4) << 2;
    }
    lVar4 = FUN_1403da910(param_1 + 0x40,2);
    if (lVar4 != 0) {
      lVar3 = FUN_140854880(param_1);
      local_res8[0] = *(undefined4 *)(lVar3 + 0xa8);
      FUN_1407278e0(lVar4,local_res8);
      *(byte *)(lVar4 + 0x241) = *(byte *)(lVar4 + 0x241) | 0x20;
      FUN_140735830(lVar4);
      if (iVar1 == 2) {
        *(byte *)(lVar4 + 0x241) = *(byte *)(lVar4 + 0x241) | 2;
      }
    }
    return;
  }
LAB_1408328d4:
                    /* WARNING: Subroutine does not return */
  FUN_140166830("No midnight to start for the corpse drop");
}


````

### result

````text

void FUN_140832980(longlong param_1)

{
  undefined4 uVar1;
  char cVar2;
  int iVar3;
  undefined8 *puVar4;
  longlong lVar5;
  float fVar6;
  uint uStack_c;
  
  if (*(char *)(param_1 + 0x51) == '\0') {
    fVar6 = (float)FUN_1407d5ec0(D2_GameSingletonPtr + 0x1f5338,*(undefined4 *)(param_1 + 0x24),0);
    if (fVar6 == -1.0) {
      FUN_140854190(param_1);
      return;
    }
    if (*(int *)(param_1 + 0x54) == 4) {
      *(int *)(param_1 + 0x58) = *(int *)(param_1 + 0x58) + 1;
      if (*(int *)(param_1 + 0x58) == 3) {
        uVar1 = *(undefined4 *)(param_1 + 0x38);
        lVar5 = FUN_140810240();
        puVar4 = (undefined8 *)FUN_1408112c0(lVar5 + 0xc8e8,0xb9,0x10,uVar1);
        if (puVar4 != (undefined8 *)0x0) {
          *puVar4 = 0;
          puVar4[1] = (ulonglong)uStack_c << 0x20;
          return;
        }
      }
    }
    else {
      iVar3 = FUN_1403a6f10(*(undefined8 *)(param_1 + 0x30),*(undefined1 *)(param_1 + 0x50));
      if (((iVar3 != *(int *)(param_1 + 0x54)) &&
          (lVar5 = FUN_1406dc560(*(undefined4 *)(param_1 + 0x38)), lVar5 != 0)) &&
         (cVar2 = FUN_1405bf440(lVar5), cVar2 == '\0')) {
        uVar1 = *(undefined4 *)(param_1 + 0x38);
        lVar5 = FUN_140810240();
        puVar4 = (undefined8 *)FUN_1408112c0(lVar5 + 0xc8e8,0xb9,0x10,uVar1);
        if (puVar4 != (undefined8 *)0x0) {
          *puVar4 = 0;
          puVar4[1] = CONCAT44(uStack_c,2);
        }
        FUN_140854d80(param_1);
        FUN_140854190(param_1);
      }
    }
  }
  else {
    cVar2 = FUN_140832420();
    if (cVar2 != '\0') {
      *(undefined1 *)(param_1 + 0x51) = 0;
      FUN_140832770(param_1);
      return;
    }
  }
  return;
}


````

### result

````text

void FUN_140828d90(longlong param_1)

{
  FUN_1408541a0();
  if ((*(uint *)(param_1 + 0x110) & 0xfffffffb) != 0) {
    FUN_140828440(param_1,"State deactivated",0,1);
  }
  if ((*(byte *)(param_1 + 0xd0) & 0x20) != 0) {
    FUN_14060c9b0(*(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x330),param_1 + 0x118);
    *(byte *)(param_1 + 0xd0) = *(byte *)(param_1 + 0xd0) & 0xdf;
  }
  return;
}


````

### result

````text

void FUN_140829ff0(longlong param_1)

{
  longlong lVar1;
  
  lVar1 = FUN_1406dc560(*(undefined4 *)(param_1 + 0xf8));
  if (lVar1 == 0) {
    FUN_140828440(param_1,"Corpse destroyed",0,1);
    return;
  }
  FUN_14082a110(param_1,lVar1);
  FUN_14082a040(param_1);
  return;
}


````

### result

````text

/* WARNING: Type propagation algorithm not settling */

void FUN_140829b10(longlong param_1,longlong param_2)

{
  undefined8 uVar1;
  longlong lVar2;
  longlong *plVar3;
  byte bVar4;
  longlong alStackX_10 [3];
  undefined1 auStack_68 [80];
  
  if (*(int *)(param_2 + 0x10) != 0) {
    alStackX_10[0] = CONCAT44(alStackX_10[0]._4_4_,*(undefined4 *)(param_2 + 8));
    uVar1 = FUN_1407b5e00(D2_GameSingletonPtr + 0x3130b0,alStackX_10);
    lVar2 = FUN_1408f2e50(uVar1);
    if (((lVar2 != 0) &&
        (plVar3 = (longlong *)FUN_140877720(lVar2,&DAT_1439d1320,2), plVar3 != (longlong *)0x0)) &&
       (*(byte *)(lVar2 + 0x368) = *(byte *)(lVar2 + 0x368) & 0xfd, *(int *)(param_2 + 0x10) < 4)) {
      lVar2 = *plVar3;
      alStackX_10[1] = 0;
      alStackX_10[0] = 0;
      uVar1 = FUN_140854880(param_1);
      (**(code **)(lVar2 + 0xd0))(plVar3,uVar1,alStackX_10 + 1,alStackX_10);
      if (alStackX_10[0] != 0) {
        *(byte *)(param_1 + 0xd0) =
             *(byte *)(param_1 + 0xd0) ^ (*(byte *)(param_2 + 0xc) ^ *(byte *)(param_1 + 0xd0)) & 1;
        bVar4 = (*(byte *)(param_2 + 0xc) ^ *(byte *)(param_1 + 0xd0)) & 2 ^
                *(byte *)(param_1 + 0xd0);
        *(byte *)(param_1 + 0xd0) = bVar4;
        bVar4 = (*(byte *)(param_2 + 0xc) ^ bVar4) & 4 ^ bVar4;
        *(byte *)(param_1 + 0xd0) = bVar4;
        *(byte *)(param_1 + 0xd0) = *(byte *)(param_2 + 0xc) & 8 | bVar4 & 0xf7 | 0x10;
        FUN_1403b7aa0(auStack_68,plVar3,alStackX_10[0],0);
        FUN_140829030(param_1,auStack_68);
      }
    }
  }
  return;
}


````

### result

````text

void FUN_140831fe0(longlong param_1)

{
  undefined4 uVar1;
  longlong lVar2;
  char cVar3;
  int iVar4;
  longlong lVar5;
  undefined8 uVar6;
  longlong lVar7;
  undefined4 *puVar8;
  longlong lVar9;
  longlong lStack_18;
  code *pcStack_10;
  
  FUN_140853d40();
  lVar7 = *(longlong *)(param_1 + 0x10);
  lVar9 = 0;
  lVar5 = lVar9;
  if (*(longlong *)(lVar7 + 0x90) != 0) {
    lVar5 = *(longlong *)(*(longlong *)(lVar7 + 0x90) + 0x178);
  }
  *(undefined4 *)(param_1 + 0x38) = *(undefined4 *)(lVar5 + 0xf8);
  lVar5 = FUN_1406dc560();
  if (lVar5 == 0) {
    FUN_1403dab80(param_1 + 0x40,0);
  }
  else {
    uVar6 = FUN_140877720(lVar5,&DAT_1439b4630,2);
    FUN_1403dab80(param_1 + 0x40,uVar6);
    uVar1 = *(undefined4 *)(lVar5 + 0xa8);
    uVar6 = FUN_140810240();
    FUN_1408103f0(uVar6,0x1cc,uVar1);
  }
  lVar2 = *(longlong *)(lVar7 + 0x2f8);
  if ((((byte)(*(uint *)(lVar2 + 0x88) >> 0x13) & (byte)(*(uint *)(lVar2 + 0x80) >> 0x13) & 1) != 0)
     && (*(int *)(lVar2 + 0x224) == 0)) {
    lVar9 = 1;
  }
  *(char *)(param_1 + 0x50) = (char)lVar9;
  cVar3 = FUN_140832420(param_1);
  if (cVar3 == '\0') {
    *(undefined1 *)(param_1 + 0x51) = 1;
  }
  else {
    *(undefined1 *)(param_1 + 0x51) = 0;
    FUN_140832770(param_1);
  }
  if (lVar5 != 0) {
    iVar4 = FUN_140604bd0(D2_GameSingletonPtr + 0x1f7bb8,lVar5,
                          *(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x10));
    if (iVar4 == 0) {
      uVar1 = *(undefined4 *)(*(longlong *)(lVar7 + 0x10) + 0xa8);
      lVar7 = FUN_140810240();
      puVar8 = (undefined4 *)FUN_1408112c0(lVar7 + 0xc8e8,0x17b,4,uVar1);
      if (puVar8 != (undefined4 *)0x0) {
        *puVar8 = 4;
      }
      *(undefined1 *)(param_1 + 0x52) = 1;
    }
  }
  pcStack_10 = FUN_1400998b0;
  lStack_18 = param_1;
  uVar6 = FUN_140810240();
  FUN_1408106f0(uVar6,0xb7,&lStack_18);
  pcStack_10 = FUN_140832b10;
  lStack_18 = param_1;
  uVar6 = FUN_140810240();
  FUN_1408106f0(uVar6,0xb9,&lStack_18);
  pcStack_10 = FUN_140832ad0;
  lStack_18 = param_1;
  uVar6 = FUN_140810240();
  FUN_1408106f0(uVar6,0xbb,&lStack_18);
  return;
}


````
