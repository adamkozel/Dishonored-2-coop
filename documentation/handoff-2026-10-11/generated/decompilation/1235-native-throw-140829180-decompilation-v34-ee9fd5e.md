# native-throw-140829180-decompilation-v34.json

Original: [work/natural-moveset/carry-research/native-throw-140829180-decompilation-v34.json](<../../../../work/natural-moveset/carry-research/native-throw-140829180-decompilation-v34.json>)

SHA-256: `62A6757069CC99FC60EA8A014B59ACCF488BEF08BC1CBBC15EBCAE0D95E8E6BC`; 13917 bytes.

## Original saved record

````json
{
  "content": [
    {
      "type": "text",
      "text": "\r\nvoid FUN_140829180(longlong param_1,undefined8 param_2,undefined8 param_3,undefined8 *param_4)\r\n\r\n{\r\n  float fVar1;\r\n  float fVar2;\r\n  float fVar3;\r\n  float fVar4;\r\n  undefined2 uVar5;\r\n  undefined2 uVar6;\r\n  longlong lVar7;\r\n  undefined8 *puVar8;\r\n  undefined8 uVar9;\r\n  longlong lVar10;\r\n  longlong lVar11;\r\n  float *pfVar12;\r\n  undefined4 *puVar13;\r\n  float *pfVar14;\r\n  longlong lVar15;\r\n  char *pcVar16;\r\n  float fVar17;\r\n  float local_res8 [2];\r\n  undefined8 local_res10;\r\n  undefined8 local_res20;\r\n  undefined4 local_228 [2];\r\n  undefined4 local_220;\r\n  undefined4 local_21c;\r\n  float local_218;\r\n  float local_214;\r\n  float local_210;\r\n  float local_20c;\r\n  float local_208;\r\n  float local_204;\r\n  float local_200;\r\n  float local_1fc;\r\n  float local_1f8 [7];\r\n  undefined4 local_1dc;\r\n  undefined4 local_1d8;\r\n  undefined4 local_1d4;\r\n  undefined4 local_1d0;\r\n  undefined4 local_1cc;\r\n  undefined4 local_1c8;\r\n  undefined4 local_1c4;\r\n  undefined4 local_1c0;\r\n  undefined4 local_1bc;\r\n  undefined4 local_1b8;\r\n  float local_1b4;\r\n  float local_1b0;\r\n  undefined4 local_1a8;\r\n  undefined4 local_1a4;\r\n  undefined4 local_1a0;\r\n  float local_198;\r\n  float local_194;\r\n  float local_190;\r\n  float local_18c;\r\n  float local_188;\r\n  float local_184;\r\n  float local_180;\r\n  float local_17c;\r\n  float local_178;\r\n  float local_170;\r\n  undefined4 local_16c;\r\n  undefined4 local_168;\r\n  undefined4 local_164;\r\n  undefined4 local_160;\r\n  undefined4 local_15c;\r\n  undefined4 local_158;\r\n  undefined4 local_154;\r\n  undefined4 local_150;\r\n  undefined1 local_148 [8];\r\n  undefined8 local_140;\r\n  undefined8 local_138;\r\n  undefined4 local_130;\r\n  undefined4 local_12c;\r\n  undefined1 local_128;\r\n  float local_118 [7];\r\n  undefined4 local_fc;\r\n  undefined4 local_f8;\r\n  undefined4 local_f4;\r\n  undefined4 local_f0;\r\n  undefined4 local_ec;\r\n  \r\n  lVar7 = *(longlong *)(param_1 + 0x18);\r\n  pcVar16 = *(char **)(lVar7 + 0x80);\r\n  local_res10 = param_2;\r\n  if (*pcVar16 == '\\0') {\r\n    FUN_140196fc0(param_3,local_res8,0);\r\n    puVar8 = (undefined8 *)FUN_140195370(param_3,&local_res20,local_res8[0],1);\r\n    pcVar16 = (char *)*puVar8;\r\n  }\r\n  uVar9 = FUN_140854880(param_1);\r\n  lVar10 = FUN_140878470(uVar9);\r\n  lVar15 = *(longlong *)(lVar10 + 0x1fe8);\r\n  lVar11 = FUN_1401c7c80(lVar15,pcVar16,1);\r\n  local_res20 = FUN_1401c7030(lVar15,lVar11 + 8,0);\r\n  if (*(longlong *)(lVar15 + 0x50) == 0) {\r\n    local_res8[0] = (float)CONCAT22(local_res8[0]._2_2_,0xffff);\r\n    pfVar12 = local_res8;\r\n  }\r\n  else {\r\n    pfVar12 = (float *)FUN_140256080(*(longlong *)(lVar15 + 0x50),local_res8,*param_4);\r\n  }\r\n  uVar5 = *(undefined2 *)pfVar12;\r\n  if (*(longlong *)(lVar15 + 0x50) == 0) {\r\n    local_res8[0] = (float)CONCAT22(local_res8[0]._2_2_,0xffff);\r\n    pfVar12 = local_res8;\r\n  }\r\n  else {\r\n    pfVar12 = (float *)FUN_140256080(*(longlong *)(lVar15 + 0x50),local_res8,\r\n                                     *(undefined8 *)(lVar7 + 0x98));\r\n  }\r\n  uVar6 = *(undefined2 *)pfVar12;\r\n  FUN_14053c8a0(*(undefined8 *)(param_1 + 0xf0),uVar5,local_res20,0,&local_1a8,&local_170);\r\n  FUN_14053c8a0(*(undefined8 *)(param_1 + 0xf0),uVar6,local_res20,0,&local_1b8,&local_198);\r\n  local_res8[0] = local_180;\r\n  local_1f8[1] = (float)local_164;\r\n  local_res20 = CONCAT44(local_res20._4_4_,local_1b8);\r\n  local_1f8[2] = (float)local_158;\r\n  local_208 = local_194;\r\n  local_1f8[3] = (float)local_1a8;\r\n  local_200 = local_188;\r\n  local_1f8[4] = (float)local_16c;\r\n  local_204 = local_17c;\r\n  local_1f8[5] = (float)local_160;\r\n  local_210 = local_1b4;\r\n  local_1f8[6] = (float)local_154;\r\n  local_1fc = local_190;\r\n  local_1dc = local_1a4;\r\n  local_20c = local_184;\r\n  local_1d8 = local_168;\r\n  local_214 = local_178;\r\n  local_1d4 = local_15c;\r\n  local_218 = local_1b0;\r\n  local_1d0 = local_150;\r\n  local_1f8[0] = local_170;\r\n  local_1cc = local_1a0;\r\n  local_1c8 = 0;\r\n  local_1c4 = 0;\r\n  local_1c0 = 0;\r\n  local_1bc = 0x3f800000;\r\n  FUN_14010f0d0(local_1f8);\r\n  pfVar12 = local_1f8 + 2;\r\n  lVar15 = 4;\r\n  pfVar14 = local_118 + 2;\r\n  do {\r\n    fVar1 = pfVar12[-1];\r\n    fVar2 = pfVar12[-2];\r\n    fVar3 = *pfVar12;\r\n    fVar4 = pfVar12[1];\r\n    pfVar12 = pfVar12 + 4;\r\n    fVar17 = fVar4 * 0.0;\r\n    pfVar14[-2] = local_198 * fVar2 + local_208 * fVar1 + fVar3 * local_1fc + fVar17;\r\n    pfVar14[-1] = local_18c * fVar2 + local_200 * fVar1 + fVar3 * local_20c + fVar17;\r\n    *pfVar14 = local_res8[0] * fVar2 + local_204 * fVar1 + fVar3 * local_214 + fVar17;\r\n    pfVar14[1] = (float)local_res20 * fVar2 + local_210 * fVar1 + fVar3 * local_218 + fVar4;\r\n    lVar15 = lVar15 + -1;\r\n    pfVar14 = pfVar14 + 4;\r\n  } while (lVar15 != 0);\r\n  FUN_140131ae0(local_148);\r\n  FUN_140131ae0(&local_140);\r\n  FUN_140131ae0(&local_138);\r\n  uVar9 = local_res10;\r\n  local_130 = 0x7149f2ca;\r\n  local_12c = 0x7149f2ca;\r\n  local_128 = 0;\r\n  local_140 = *(undefined8 *)(lVar7 + 0x98);\r\n  local_138 = *param_4;\r\n  FUN_140755870(*(undefined8 *)(param_1 + 0xe8),local_res10,local_148);\r\n  lVar7 = *(longlong *)(*(longlong *)(param_1 + 0xe8) + 0x20);\r\n  lVar15 = *(longlong *)(*(longlong *)(lVar10 + 0x1fe8) + 0x50);\r\n  if (lVar15 == 0) {\r\n    local_res8[0] = (float)CONCAT22(local_res8[0]._2_2_,0xffff);\r\n    pfVar12 = local_res8;\r\n  }\r\n  else {\r\n    pfVar12 = (float *)FUN_140256000(lVar15,local_res8,uVar5);\r\n  }\r\n  *(undefined2 *)(lVar7 + 0x8c) = *(undefined2 *)pfVar12;\r\n  *(float *)(lVar7 + 0x70) = local_118[3];\r\n  *(undefined4 *)(lVar7 + 0x74) = local_fc;\r\n  *(undefined4 *)(lVar7 + 0x78) = local_ec;\r\n  local_1f8[0] = local_118[0];\r\n  local_1f8[3] = local_118[1];\r\n  local_1f8[6] = local_118[2];\r\n  local_1f8[1] = local_118[4];\r\n  local_1f8[4] = local_118[5];\r\n  local_1dc = local_118[6];\r\n  local_1f8[2] = (float)local_f8;\r\n  local_1f8[5] = (float)local_f4;\r\n  local_1d8 = local_f0;\r\n  puVar13 = (undefined4 *)FUN_140110080(local_1f8,local_228);\r\n  *(undefined4 *)(lVar7 + 0x7c) = *puVar13;\r\n  *(undefined4 *)(lVar7 + 0x80) = puVar13[1];\r\n  *(undefined4 *)(lVar7 + 0x84) = puVar13[2];\r\n  local_228[0] = 0;\r\n  *(undefined4 *)(lVar7 + 0x88) = puVar13[3];\r\n  lVar7 = *(longlong *)(param_1 + 0xe8);\r\n  local_220 = 7;\r\n  local_21c = CONCAT13(local_21c._3_1_,0x100);\r\n  *(undefined4 *)(lVar7 + 0x34) = 0;\r\n  *(undefined4 *)(lVar7 + 0x38) = 0;\r\n  *(undefined4 *)(lVar7 + 0x3c) = 7;\r\n  *(undefined4 *)(lVar7 + 0x40) = local_21c;\r\n  FUN_140757830(*(undefined8 *)(param_1 + 0xe8));\r\n  FUN_14080cd60(D2_GameSingletonPtr + 0x1f52e8,*(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x10)\r\n                ,uVar9);\r\n  return;\r\n}\r\n\r\n"
    }
  ],
  "structuredContent": {
    "result": "\r\nvoid FUN_140829180(longlong param_1,undefined8 param_2,undefined8 param_3,undefined8 *param_4)\r\n\r\n{\r\n  float fVar1;\r\n  float fVar2;\r\n  float fVar3;\r\n  float fVar4;\r\n  undefined2 uVar5;\r\n  undefined2 uVar6;\r\n  longlong lVar7;\r\n  undefined8 *puVar8;\r\n  undefined8 uVar9;\r\n  longlong lVar10;\r\n  longlong lVar11;\r\n  float *pfVar12;\r\n  undefined4 *puVar13;\r\n  float *pfVar14;\r\n  longlong lVar15;\r\n  char *pcVar16;\r\n  float fVar17;\r\n  float local_res8 [2];\r\n  undefined8 local_res10;\r\n  undefined8 local_res20;\r\n  undefined4 local_228 [2];\r\n  undefined4 local_220;\r\n  undefined4 local_21c;\r\n  float local_218;\r\n  float local_214;\r\n  float local_210;\r\n  float local_20c;\r\n  float local_208;\r\n  float local_204;\r\n  float local_200;\r\n  float local_1fc;\r\n  float local_1f8 [7];\r\n  undefined4 local_1dc;\r\n  undefined4 local_1d8;\r\n  undefined4 local_1d4;\r\n  undefined4 local_1d0;\r\n  undefined4 local_1cc;\r\n  undefined4 local_1c8;\r\n  undefined4 local_1c4;\r\n  undefined4 local_1c0;\r\n  undefined4 local_1bc;\r\n  undefined4 local_1b8;\r\n  float local_1b4;\r\n  float local_1b0;\r\n  undefined4 local_1a8;\r\n  undefined4 local_1a4;\r\n  undefined4 local_1a0;\r\n  float local_198;\r\n  float local_194;\r\n  float local_190;\r\n  float local_18c;\r\n  float local_188;\r\n  float local_184;\r\n  float local_180;\r\n  float local_17c;\r\n  float local_178;\r\n  float local_170;\r\n  undefined4 local_16c;\r\n  undefined4 local_168;\r\n  undefined4 local_164;\r\n  undefined4 local_160;\r\n  undefined4 local_15c;\r\n  undefined4 local_158;\r\n  undefined4 local_154;\r\n  undefined4 local_150;\r\n  undefined1 local_148 [8];\r\n  undefined8 local_140;\r\n  undefined8 local_138;\r\n  undefined4 local_130;\r\n  undefined4 local_12c;\r\n  undefined1 local_128;\r\n  float local_118 [7];\r\n  undefined4 local_fc;\r\n  undefined4 local_f8;\r\n  undefined4 local_f4;\r\n  undefined4 local_f0;\r\n  undefined4 local_ec;\r\n  \r\n  lVar7 = *(longlong *)(param_1 + 0x18);\r\n  pcVar16 = *(char **)(lVar7 + 0x80);\r\n  local_res10 = param_2;\r\n  if (*pcVar16 == '\\0') {\r\n    FUN_140196fc0(param_3,local_res8,0);\r\n    puVar8 = (undefined8 *)FUN_140195370(param_3,&local_res20,local_res8[0],1);\r\n    pcVar16 = (char *)*puVar8;\r\n  }\r\n  uVar9 = FUN_140854880(param_1);\r\n  lVar10 = FUN_140878470(uVar9);\r\n  lVar15 = *(longlong *)(lVar10 + 0x1fe8);\r\n  lVar11 = FUN_1401c7c80(lVar15,pcVar16,1);\r\n  local_res20 = FUN_1401c7030(lVar15,lVar11 + 8,0);\r\n  if (*(longlong *)(lVar15 + 0x50) == 0) {\r\n    local_res8[0] = (float)CONCAT22(local_res8[0]._2_2_,0xffff);\r\n    pfVar12 = local_res8;\r\n  }\r\n  else {\r\n    pfVar12 = (float *)FUN_140256080(*(longlong *)(lVar15 + 0x50),local_res8,*param_4);\r\n  }\r\n  uVar5 = *(undefined2 *)pfVar12;\r\n  if (*(longlong *)(lVar15 + 0x50) == 0) {\r\n    local_res8[0] = (float)CONCAT22(local_res8[0]._2_2_,0xffff);\r\n    pfVar12 = local_res8;\r\n  }\r\n  else {\r\n    pfVar12 = (float *)FUN_140256080(*(longlong *)(lVar15 + 0x50),local_res8,\r\n                                     *(undefined8 *)(lVar7 + 0x98));\r\n  }\r\n  uVar6 = *(undefined2 *)pfVar12;\r\n  FUN_14053c8a0(*(undefined8 *)(param_1 + 0xf0),uVar5,local_res20,0,&local_1a8,&local_170);\r\n  FUN_14053c8a0(*(undefined8 *)(param_1 + 0xf0),uVar6,local_res20,0,&local_1b8,&local_198);\r\n  local_res8[0] = local_180;\r\n  local_1f8[1] = (float)local_164;\r\n  local_res20 = CONCAT44(local_res20._4_4_,local_1b8);\r\n  local_1f8[2] = (float)local_158;\r\n  local_208 = local_194;\r\n  local_1f8[3] = (float)local_1a8;\r\n  local_200 = local_188;\r\n  local_1f8[4] = (float)local_16c;\r\n  local_204 = local_17c;\r\n  local_1f8[5] = (float)local_160;\r\n  local_210 = local_1b4;\r\n  local_1f8[6] = (float)local_154;\r\n  local_1fc = local_190;\r\n  local_1dc = local_1a4;\r\n  local_20c = local_184;\r\n  local_1d8 = local_168;\r\n  local_214 = local_178;\r\n  local_1d4 = local_15c;\r\n  local_218 = local_1b0;\r\n  local_1d0 = local_150;\r\n  local_1f8[0] = local_170;\r\n  local_1cc = local_1a0;\r\n  local_1c8 = 0;\r\n  local_1c4 = 0;\r\n  local_1c0 = 0;\r\n  local_1bc = 0x3f800000;\r\n  FUN_14010f0d0(local_1f8);\r\n  pfVar12 = local_1f8 + 2;\r\n  lVar15 = 4;\r\n  pfVar14 = local_118 + 2;\r\n  do {\r\n    fVar1 = pfVar12[-1];\r\n    fVar2 = pfVar12[-2];\r\n    fVar3 = *pfVar12;\r\n    fVar4 = pfVar12[1];\r\n    pfVar12 = pfVar12 + 4;\r\n    fVar17 = fVar4 * 0.0;\r\n    pfVar14[-2] = local_198 * fVar2 + local_208 * fVar1 + fVar3 * local_1fc + fVar17;\r\n    pfVar14[-1] = local_18c * fVar2 + local_200 * fVar1 + fVar3 * local_20c + fVar17;\r\n    *pfVar14 = local_res8[0] * fVar2 + local_204 * fVar1 + fVar3 * local_214 + fVar17;\r\n    pfVar14[1] = (float)local_res20 * fVar2 + local_210 * fVar1 + fVar3 * local_218 + fVar4;\r\n    lVar15 = lVar15 + -1;\r\n    pfVar14 = pfVar14 + 4;\r\n  } while (lVar15 != 0);\r\n  FUN_140131ae0(local_148);\r\n  FUN_140131ae0(&local_140);\r\n  FUN_140131ae0(&local_138);\r\n  uVar9 = local_res10;\r\n  local_130 = 0x7149f2ca;\r\n  local_12c = 0x7149f2ca;\r\n  local_128 = 0;\r\n  local_140 = *(undefined8 *)(lVar7 + 0x98);\r\n  local_138 = *param_4;\r\n  FUN_140755870(*(undefined8 *)(param_1 + 0xe8),local_res10,local_148);\r\n  lVar7 = *(longlong *)(*(longlong *)(param_1 + 0xe8) + 0x20);\r\n  lVar15 = *(longlong *)(*(longlong *)(lVar10 + 0x1fe8) + 0x50);\r\n  if (lVar15 == 0) {\r\n    local_res8[0] = (float)CONCAT22(local_res8[0]._2_2_,0xffff);\r\n    pfVar12 = local_res8;\r\n  }\r\n  else {\r\n    pfVar12 = (float *)FUN_140256000(lVar15,local_res8,uVar5);\r\n  }\r\n  *(undefined2 *)(lVar7 + 0x8c) = *(undefined2 *)pfVar12;\r\n  *(float *)(lVar7 + 0x70) = local_118[3];\r\n  *(undefined4 *)(lVar7 + 0x74) = local_fc;\r\n  *(undefined4 *)(lVar7 + 0x78) = local_ec;\r\n  local_1f8[0] = local_118[0];\r\n  local_1f8[3] = local_118[1];\r\n  local_1f8[6] = local_118[2];\r\n  local_1f8[1] = local_118[4];\r\n  local_1f8[4] = local_118[5];\r\n  local_1dc = local_118[6];\r\n  local_1f8[2] = (float)local_f8;\r\n  local_1f8[5] = (float)local_f4;\r\n  local_1d8 = local_f0;\r\n  puVar13 = (undefined4 *)FUN_140110080(local_1f8,local_228);\r\n  *(undefined4 *)(lVar7 + 0x7c) = *puVar13;\r\n  *(undefined4 *)(lVar7 + 0x80) = puVar13[1];\r\n  *(undefined4 *)(lVar7 + 0x84) = puVar13[2];\r\n  local_228[0] = 0;\r\n  *(undefined4 *)(lVar7 + 0x88) = puVar13[3];\r\n  lVar7 = *(longlong *)(param_1 + 0xe8);\r\n  local_220 = 7;\r\n  local_21c = CONCAT13(local_21c._3_1_,0x100);\r\n  *(undefined4 *)(lVar7 + 0x34) = 0;\r\n  *(undefined4 *)(lVar7 + 0x38) = 0;\r\n  *(undefined4 *)(lVar7 + 0x3c) = 7;\r\n  *(undefined4 *)(lVar7 + 0x40) = local_21c;\r\n  FUN_140757830(*(undefined8 *)(param_1 + 0xe8));\r\n  FUN_14080cd60(D2_GameSingletonPtr + 0x1f52e8,*(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x10)\r\n                ,uVar9);\r\n  return;\r\n}\r\n\r\n"
  },
  "isError": false
}

````

## Unescaped text fields for reading

### text

````text

void FUN_140829180(longlong param_1,undefined8 param_2,undefined8 param_3,undefined8 *param_4)

{
  float fVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  undefined2 uVar5;
  undefined2 uVar6;
  longlong lVar7;
  undefined8 *puVar8;
  undefined8 uVar9;
  longlong lVar10;
  longlong lVar11;
  float *pfVar12;
  undefined4 *puVar13;
  float *pfVar14;
  longlong lVar15;
  char *pcVar16;
  float fVar17;
  float local_res8 [2];
  undefined8 local_res10;
  undefined8 local_res20;
  undefined4 local_228 [2];
  undefined4 local_220;
  undefined4 local_21c;
  float local_218;
  float local_214;
  float local_210;
  float local_20c;
  float local_208;
  float local_204;
  float local_200;
  float local_1fc;
  float local_1f8 [7];
  undefined4 local_1dc;
  undefined4 local_1d8;
  undefined4 local_1d4;
  undefined4 local_1d0;
  undefined4 local_1cc;
  undefined4 local_1c8;
  undefined4 local_1c4;
  undefined4 local_1c0;
  undefined4 local_1bc;
  undefined4 local_1b8;
  float local_1b4;
  float local_1b0;
  undefined4 local_1a8;
  undefined4 local_1a4;
  undefined4 local_1a0;
  float local_198;
  float local_194;
  float local_190;
  float local_18c;
  float local_188;
  float local_184;
  float local_180;
  float local_17c;
  float local_178;
  float local_170;
  undefined4 local_16c;
  undefined4 local_168;
  undefined4 local_164;
  undefined4 local_160;
  undefined4 local_15c;
  undefined4 local_158;
  undefined4 local_154;
  undefined4 local_150;
  undefined1 local_148 [8];
  undefined8 local_140;
  undefined8 local_138;
  undefined4 local_130;
  undefined4 local_12c;
  undefined1 local_128;
  float local_118 [7];
  undefined4 local_fc;
  undefined4 local_f8;
  undefined4 local_f4;
  undefined4 local_f0;
  undefined4 local_ec;
  
  lVar7 = *(longlong *)(param_1 + 0x18);
  pcVar16 = *(char **)(lVar7 + 0x80);
  local_res10 = param_2;
  if (*pcVar16 == '\0') {
    FUN_140196fc0(param_3,local_res8,0);
    puVar8 = (undefined8 *)FUN_140195370(param_3,&local_res20,local_res8[0],1);
    pcVar16 = (char *)*puVar8;
  }
  uVar9 = FUN_140854880(param_1);
  lVar10 = FUN_140878470(uVar9);
  lVar15 = *(longlong *)(lVar10 + 0x1fe8);
  lVar11 = FUN_1401c7c80(lVar15,pcVar16,1);
  local_res20 = FUN_1401c7030(lVar15,lVar11 + 8,0);
  if (*(longlong *)(lVar15 + 0x50) == 0) {
    local_res8[0] = (float)CONCAT22(local_res8[0]._2_2_,0xffff);
    pfVar12 = local_res8;
  }
  else {
    pfVar12 = (float *)FUN_140256080(*(longlong *)(lVar15 + 0x50),local_res8,*param_4);
  }
  uVar5 = *(undefined2 *)pfVar12;
  if (*(longlong *)(lVar15 + 0x50) == 0) {
    local_res8[0] = (float)CONCAT22(local_res8[0]._2_2_,0xffff);
    pfVar12 = local_res8;
  }
  else {
    pfVar12 = (float *)FUN_140256080(*(longlong *)(lVar15 + 0x50),local_res8,
                                     *(undefined8 *)(lVar7 + 0x98));
  }
  uVar6 = *(undefined2 *)pfVar12;
  FUN_14053c8a0(*(undefined8 *)(param_1 + 0xf0),uVar5,local_res20,0,&local_1a8,&local_170);
  FUN_14053c8a0(*(undefined8 *)(param_1 + 0xf0),uVar6,local_res20,0,&local_1b8,&local_198);
  local_res8[0] = local_180;
  local_1f8[1] = (float)local_164;
  local_res20 = CONCAT44(local_res20._4_4_,local_1b8);
  local_1f8[2] = (float)local_158;
  local_208 = local_194;
  local_1f8[3] = (float)local_1a8;
  local_200 = local_188;
  local_1f8[4] = (float)local_16c;
  local_204 = local_17c;
  local_1f8[5] = (float)local_160;
  local_210 = local_1b4;
  local_1f8[6] = (float)local_154;
  local_1fc = local_190;
  local_1dc = local_1a4;
  local_20c = local_184;
  local_1d8 = local_168;
  local_214 = local_178;
  local_1d4 = local_15c;
  local_218 = local_1b0;
  local_1d0 = local_150;
  local_1f8[0] = local_170;
  local_1cc = local_1a0;
  local_1c8 = 0;
  local_1c4 = 0;
  local_1c0 = 0;
  local_1bc = 0x3f800000;
  FUN_14010f0d0(local_1f8);
  pfVar12 = local_1f8 + 2;
  lVar15 = 4;
  pfVar14 = local_118 + 2;
  do {
    fVar1 = pfVar12[-1];
    fVar2 = pfVar12[-2];
    fVar3 = *pfVar12;
    fVar4 = pfVar12[1];
    pfVar12 = pfVar12 + 4;
    fVar17 = fVar4 * 0.0;
    pfVar14[-2] = local_198 * fVar2 + local_208 * fVar1 + fVar3 * local_1fc + fVar17;
    pfVar14[-1] = local_18c * fVar2 + local_200 * fVar1 + fVar3 * local_20c + fVar17;
    *pfVar14 = local_res8[0] * fVar2 + local_204 * fVar1 + fVar3 * local_214 + fVar17;
    pfVar14[1] = (float)local_res20 * fVar2 + local_210 * fVar1 + fVar3 * local_218 + fVar4;
    lVar15 = lVar15 + -1;
    pfVar14 = pfVar14 + 4;
  } while (lVar15 != 0);
  FUN_140131ae0(local_148);
  FUN_140131ae0(&local_140);
  FUN_140131ae0(&local_138);
  uVar9 = local_res10;
  local_130 = 0x7149f2ca;
  local_12c = 0x7149f2ca;
  local_128 = 0;
  local_140 = *(undefined8 *)(lVar7 + 0x98);
  local_138 = *param_4;
  FUN_140755870(*(undefined8 *)(param_1 + 0xe8),local_res10,local_148);
  lVar7 = *(longlong *)(*(longlong *)(param_1 + 0xe8) + 0x20);
  lVar15 = *(longlong *)(*(longlong *)(lVar10 + 0x1fe8) + 0x50);
  if (lVar15 == 0) {
    local_res8[0] = (float)CONCAT22(local_res8[0]._2_2_,0xffff);
    pfVar12 = local_res8;
  }
  else {
    pfVar12 = (float *)FUN_140256000(lVar15,local_res8,uVar5);
  }
  *(undefined2 *)(lVar7 + 0x8c) = *(undefined2 *)pfVar12;
  *(float *)(lVar7 + 0x70) = local_118[3];
  *(undefined4 *)(lVar7 + 0x74) = local_fc;
  *(undefined4 *)(lVar7 + 0x78) = local_ec;
  local_1f8[0] = local_118[0];
  local_1f8[3] = local_118[1];
  local_1f8[6] = local_118[2];
  local_1f8[1] = local_118[4];
  local_1f8[4] = local_118[5];
  local_1dc = local_118[6];
  local_1f8[2] = (float)local_f8;
  local_1f8[5] = (float)local_f4;
  local_1d8 = local_f0;
  puVar13 = (undefined4 *)FUN_140110080(local_1f8,local_228);
  *(undefined4 *)(lVar7 + 0x7c) = *puVar13;
  *(undefined4 *)(lVar7 + 0x80) = puVar13[1];
  *(undefined4 *)(lVar7 + 0x84) = puVar13[2];
  local_228[0] = 0;
  *(undefined4 *)(lVar7 + 0x88) = puVar13[3];
  lVar7 = *(longlong *)(param_1 + 0xe8);
  local_220 = 7;
  local_21c = CONCAT13(local_21c._3_1_,0x100);
  *(undefined4 *)(lVar7 + 0x34) = 0;
  *(undefined4 *)(lVar7 + 0x38) = 0;
  *(undefined4 *)(lVar7 + 0x3c) = 7;
  *(undefined4 *)(lVar7 + 0x40) = local_21c;
  FUN_140757830(*(undefined8 *)(param_1 + 0xe8));
  FUN_14080cd60(D2_GameSingletonPtr + 0x1f52e8,*(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x10)
                ,uVar9);
  return;
}


````
