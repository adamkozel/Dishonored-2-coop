# ghidra-native-door-conflict-v749.json

Original: [work/movement-integration/ghidra-native-door-conflict-v749.json](<../../../../work/movement-integration/ghidra-native-door-conflict-v749.json>)

SHA-256: `03393F96E1C8E615F96B28FA2F94871032253B1E4BD56581FA9F91C032EDA210`; 24160 bytes.

## Original saved record

````json
{
  "scope": "Original targeted decompilation/xref results from749 plus confirmed project bookmark save762; no new decompilation or runtime acceptance implied by this export.",
  "sources": [
    {
      "source": "work/movement-integration/door-conflict-v749/v749-door-0.json",
      "sha256": "9A12E7D0D8927CCE9A1B0E22C5E6DE78DD9DF20E16EB6884F32AFCE661AF1A02",
      "transcript": {
        "status": "fulfilled",
        "value": {
          "content": [
            {
              "type": "text",
              "text": "\r\nulonglong FUN_14074a400(longlong param_1,undefined1 *param_2)\r\n\r\n{\r\n  ulonglong in_RAX;\r\n  longlong *plVar1;\r\n  longlong lVar2;\r\n  ulonglong uVar3;\r\n  \r\n  if (*(int *)(param_1 + 8) == 1) {\r\nLAB_14074a482:\r\n    uVar3 = in_RAX & 0xffffffffffffff00;\r\n  }\r\n  else {\r\n    plVar1 = (longlong *)FUN_1406df6d0();\r\n    lVar2 = FUN_1406df6d0(param_1,1);\r\n    if (plVar1 != (longlong *)0x0) {\r\n      in_RAX = (**(code **)(*plVar1 + 0x20))(plVar1,lVar2);\r\n      if ((char)in_RAX == '\\0') goto LAB_14074a482;\r\n    }\r\n    *(undefined1 *)(lVar2 + 0x10) = *param_2;\r\n    *(undefined1 *)(lVar2 + 0x11) = param_2[1];\r\n    *(undefined1 *)(lVar2 + 0x12) = param_2[2];\r\n    if ((undefined1 *)(lVar2 + 0x13) != param_2 + 3) {\r\n      *(undefined1 *)(lVar2 + 0x13) = param_2[3];\r\n    }\r\n    *(undefined4 *)(param_1 + 0xc) = 1;\r\n    uVar3 = 1;\r\n  }\r\n  return uVar3;\r\n}\r\n\r\n"
            }
          ],
          "structuredContent": {
            "result": "\r\nulonglong FUN_14074a400(longlong param_1,undefined1 *param_2)\r\n\r\n{\r\n  ulonglong in_RAX;\r\n  longlong *plVar1;\r\n  longlong lVar2;\r\n  ulonglong uVar3;\r\n  \r\n  if (*(int *)(param_1 + 8) == 1) {\r\nLAB_14074a482:\r\n    uVar3 = in_RAX & 0xffffffffffffff00;\r\n  }\r\n  else {\r\n    plVar1 = (longlong *)FUN_1406df6d0();\r\n    lVar2 = FUN_1406df6d0(param_1,1);\r\n    if (plVar1 != (longlong *)0x0) {\r\n      in_RAX = (**(code **)(*plVar1 + 0x20))(plVar1,lVar2);\r\n      if ((char)in_RAX == '\\0') goto LAB_14074a482;\r\n    }\r\n    *(undefined1 *)(lVar2 + 0x10) = *param_2;\r\n    *(undefined1 *)(lVar2 + 0x11) = param_2[1];\r\n    *(undefined1 *)(lVar2 + 0x12) = param_2[2];\r\n    if ((undefined1 *)(lVar2 + 0x13) != param_2 + 3) {\r\n      *(undefined1 *)(lVar2 + 0x13) = param_2[3];\r\n    }\r\n    *(undefined4 *)(param_1 + 0xc) = 1;\r\n    uVar3 = 1;\r\n  }\r\n  return uVar3;\r\n}\r\n\r\n"
          },
          "isError": false
        }
      }
    },
    {
      "source": "work/movement-integration/door-conflict-v749/v749-door-1.json",
      "sha256": "50ADEA5D1A1B876A6EFE5FD23C9E933760C39BACB1B045ADB934427C390E565C",
      "transcript": {
        "status": "fulfilled",
        "value": {
          "content": [
            {
              "type": "text",
              "text": "From 143f2cb00 [DATA]\nFrom 14074bb4e in FUN_14074ba10 [UNCONDITIONAL_CALL]\nFrom 14074d878 in FUN_14074d7a0 [UNCONDITIONAL_CALL]\nFrom 14074acc8 in FUN_14074ac50 [UNCONDITIONAL_CALL]"
            }
          ],
          "structuredContent": {
            "result": "From 143f2cb00 [DATA]\nFrom 14074bb4e in FUN_14074ba10 [UNCONDITIONAL_CALL]\nFrom 14074d878 in FUN_14074d7a0 [UNCONDITIONAL_CALL]\nFrom 14074acc8 in FUN_14074ac50 [UNCONDITIONAL_CALL]"
          },
          "isError": false
        }
      }
    },
    {
      "source": "work/movement-integration/door-conflict-v749/v749-use-0.json",
      "sha256": "51323949B6F03ADBA298F107D0263CAE8C3C393550F7727A925B51A0316B7974",
      "transcript": {
        "status": "fulfilled",
        "value": {
          "content": [
            {
              "type": "text",
              "text": "\r\nvoid FUN_14074b6e0(longlong param_1,longlong param_2)\r\n\r\n{\r\n  byte *pbVar1;\r\n  char cVar2;\r\n  char cVar3;\r\n  longlong lVar4;\r\n  longlong lVar5;\r\n  longlong lVar6;\r\n  longlong lVar7;\r\n  char *pcVar8;\r\n  char cVar9;\r\n  undefined1 local_res8;\r\n  undefined2 local_res18;\r\n  char local_res1a;\r\n  char local_res1b [5];\r\n  longlong *local_res20;\r\n  \r\n  *(undefined4 *)(param_1 + 0x114) = **(undefined4 **)(param_2 + 8);\r\n  lVar4 = FUN_1403da910(param_1 + 0xe8,0);\r\n  cVar9 = -1;\r\n  local_res1b[0] = -1;\r\n  local_res18 = 0;\r\n  local_res8 = 0;\r\n  lVar5 = FUN_1406dc560(*(undefined4 *)(param_1 + 0x114));\r\n  cVar2 = FUN_1405bf220(*(undefined4 *)(lVar5 + 0xa8));\r\n  if (cVar2 == '\\0') {\r\n    lVar5 = FUN_1406dc560(*(undefined4 *)(param_1 + 0x114));\r\n    cVar2 = local_res1a;\r\n    if (lVar5 != 0) {\r\n      FUN_14074dd50(param_1,&local_res18);\r\n      local_res8 = (undefined1)local_res18;\r\n      cVar2 = local_res1a;\r\n      cVar9 = local_res1b[0];\r\n    }\r\n  }\r\n  else if (*(int *)(param_1 + 0x48) == 2) {\r\n    lVar5 = FUN_1406df6d0(param_1 + 0x40,*(undefined4 *)(param_1 + 0x48));\r\n    cVar2 = *(char *)(lVar5 + 0x10);\r\n    pcVar8 = (char *)(param_1 + 0x1e2);\r\n    if (cVar2 == '\\0') {\r\n      pcVar8 = (char *)(param_1 + 0x1e3);\r\n    }\r\n    if (local_res1b != pcVar8) {\r\n      cVar9 = *pcVar8;\r\n    }\r\n  }\r\n  else {\r\n    lVar5 = FUN_1405befa0();\r\n    if ((lVar5 != 0) && (*(int *)(*(longlong *)(lVar5 + 0x2f8) + 0x5d0) != 0xe)) {\r\n      lVar5 = FUN_140824760(*(longlong *)(lVar5 + 0x2f8) + 0x410);\r\n      if (lVar5 == 0) {\r\n        *(undefined4 *)(param_1 + 0x114) = 0x3fff0010;\r\n      }\r\n      else {\r\n        *(undefined4 *)(param_1 + 0x114) = *(undefined4 *)(lVar5 + 0xa8);\r\n      }\r\n    }\r\n    lVar5 = FUN_1406dc560(*(undefined4 *)(param_1 + 0x114));\r\n    lVar6 = FUN_1406dc560(*(undefined4 *)(param_1 + 0xdc));\r\n    lVar7 = FUN_1406dc560(*(undefined4 *)(param_1 + 0xdc));\r\n    cVar2 = 0.0 <= (*(float *)(lVar6 + 0x304) - *(float *)(lVar5 + 0x304)) *\r\n                   *(float *)(lVar7 + 0x310) +\r\n                   (*(float *)(lVar6 + 0x300) - *(float *)(lVar5 + 0x300)) *\r\n                   *(float *)(lVar7 + 0x30c) +\r\n                   (*(float *)(lVar6 + 0x308) - *(float *)(lVar5 + 0x308)) *\r\n                   *(float *)(lVar7 + 0x314);\r\n    if ((*(char *)(param_1 + 0x1e8) == '\\0') && (*(char *)(param_1 + 0x118) == '\\0')) {\r\n      if ((bool)cVar2) {\r\n        pcVar8 = (char *)(param_1 + 0x1e1);\r\n      }\r\n      else {\r\n        pcVar8 = (char *)(param_1 + 0x1e0);\r\n      }\r\n    }\r\n    else {\r\n      pcVar8 = (char *)(param_1 + 0x1e5);\r\n      if (!(bool)cVar2) {\r\n        pcVar8 = (char *)(param_1 + 0x1e4);\r\n      }\r\n    }\r\n    if (local_res1b != pcVar8) {\r\n      cVar9 = *pcVar8;\r\n    }\r\n    if ((cVar9 == *(char *)(param_1 + 0x1e1)) ||\r\n       (local_res8 = 0, cVar9 == *(char *)(param_1 + 0x1e0))) {\r\n      local_res8 = 1;\r\n    }\r\n    FUN_14074c100(param_1);\r\n  }\r\n  *(undefined1 *)(param_1 + 0x1ec) = 0;\r\n  if (*(int *)(param_1 + 0x48) == 1) {\r\n    FUN_14074adf0(param_1 + 0x80);\r\n    FUN_1406df740(param_1 + 0x40);\r\n  }\r\n  if (*(int *)(param_1 + 0x48) != 1) {\r\n    local_res20 = (longlong *)FUN_1406df6d0(param_1 + 0x40);\r\n    lVar5 = FUN_1406df6d0(param_1 + 0x40,1);\r\n    if ((local_res20 == (longlong *)0x0) ||\r\n       (cVar3 = (**(code **)(*local_res20 + 0x20))(local_res20,lVar5), cVar3 != '\\0')) {\r\n      *(char *)(lVar5 + 0x12) = cVar2;\r\n      *(undefined1 *)(lVar5 + 0x10) = local_res8;\r\n      *(undefined1 *)(lVar5 + 0x11) = local_res18._1_1_;\r\n      if ((char *)(lVar5 + 0x13) != local_res1b) {\r\n        *(char *)(lVar5 + 0x13) = cVar9;\r\n      }\r\n      *(undefined4 *)(param_1 + 0x4c) = 1;\r\n      if (((lVar4 != 0) && (7 < ((byte)*(undefined4 *)(lVar4 + 0x1c) & 0xc))) &&\r\n         (*(char *)(lVar4 + 0x1e9) == '\\0')) {\r\n        *(undefined1 *)(param_1 + 0x1e9) = 1;\r\n        FUN_14074b6e0(lVar4,param_2);\r\n        *(undefined1 *)(param_1 + 0x1e9) = 0;\r\n      }\r\n      FUN_1407e79a0(*(undefined8 *)(param_1 + 0xf8));\r\n      FUN_1407e79a0(param_1);\r\n    }\r\n  }\r\n  pbVar1 = (byte *)(*(longlong *)(param_1 + 0x10) + 0xd4);\r\n  *pbVar1 = *pbVar1 | 4;\r\n  lVar4 = FUN_1406dc560(*(undefined4 *)(param_1 + 0xdc));\r\n  if (lVar4 != 0) {\r\n    lVar4 = FUN_1406dc560(*(undefined4 *)(param_1 + 0xdc));\r\n    *(byte *)(lVar4 + 0xd4) = *(byte *)(lVar4 + 0xd4) | 4;\r\n  }\r\n  return;\r\n}\r\n\r\n"
            }
          ],
          "structuredContent": {
            "result": "\r\nvoid FUN_14074b6e0(longlong param_1,longlong param_2)\r\n\r\n{\r\n  byte *pbVar1;\r\n  char cVar2;\r\n  char cVar3;\r\n  longlong lVar4;\r\n  longlong lVar5;\r\n  longlong lVar6;\r\n  longlong lVar7;\r\n  char *pcVar8;\r\n  char cVar9;\r\n  undefined1 local_res8;\r\n  undefined2 local_res18;\r\n  char local_res1a;\r\n  char local_res1b [5];\r\n  longlong *local_res20;\r\n  \r\n  *(undefined4 *)(param_1 + 0x114) = **(undefined4 **)(param_2 + 8);\r\n  lVar4 = FUN_1403da910(param_1 + 0xe8,0);\r\n  cVar9 = -1;\r\n  local_res1b[0] = -1;\r\n  local_res18 = 0;\r\n  local_res8 = 0;\r\n  lVar5 = FUN_1406dc560(*(undefined4 *)(param_1 + 0x114));\r\n  cVar2 = FUN_1405bf220(*(undefined4 *)(lVar5 + 0xa8));\r\n  if (cVar2 == '\\0') {\r\n    lVar5 = FUN_1406dc560(*(undefined4 *)(param_1 + 0x114));\r\n    cVar2 = local_res1a;\r\n    if (lVar5 != 0) {\r\n      FUN_14074dd50(param_1,&local_res18);\r\n      local_res8 = (undefined1)local_res18;\r\n      cVar2 = local_res1a;\r\n      cVar9 = local_res1b[0];\r\n    }\r\n  }\r\n  else if (*(int *)(param_1 + 0x48) == 2) {\r\n    lVar5 = FUN_1406df6d0(param_1 + 0x40,*(undefined4 *)(param_1 + 0x48));\r\n    cVar2 = *(char *)(lVar5 + 0x10);\r\n    pcVar8 = (char *)(param_1 + 0x1e2);\r\n    if (cVar2 == '\\0') {\r\n      pcVar8 = (char *)(param_1 + 0x1e3);\r\n    }\r\n    if (local_res1b != pcVar8) {\r\n      cVar9 = *pcVar8;\r\n    }\r\n  }\r\n  else {\r\n    lVar5 = FUN_1405befa0();\r\n    if ((lVar5 != 0) && (*(int *)(*(longlong *)(lVar5 + 0x2f8) + 0x5d0) != 0xe)) {\r\n      lVar5 = FUN_140824760(*(longlong *)(lVar5 + 0x2f8) + 0x410);\r\n      if (lVar5 == 0) {\r\n        *(undefined4 *)(param_1 + 0x114) = 0x3fff0010;\r\n      }\r\n      else {\r\n        *(undefined4 *)(param_1 + 0x114) = *(undefined4 *)(lVar5 + 0xa8);\r\n      }\r\n    }\r\n    lVar5 = FUN_1406dc560(*(undefined4 *)(param_1 + 0x114));\r\n    lVar6 = FUN_1406dc560(*(undefined4 *)(param_1 + 0xdc));\r\n    lVar7 = FUN_1406dc560(*(undefined4 *)(param_1 + 0xdc));\r\n    cVar2 = 0.0 <= (*(float *)(lVar6 + 0x304) - *(float *)(lVar5 + 0x304)) *\r\n                   *(float *)(lVar7 + 0x310) +\r\n                   (*(float *)(lVar6 + 0x300) - *(float *)(lVar5 + 0x300)) *\r\n                   *(float *)(lVar7 + 0x30c) +\r\n                   (*(float *)(lVar6 + 0x308) - *(float *)(lVar5 + 0x308)) *\r\n                   *(float *)(lVar7 + 0x314);\r\n    if ((*(char *)(param_1 + 0x1e8) == '\\0') && (*(char *)(param_1 + 0x118) == '\\0')) {\r\n      if ((bool)cVar2) {\r\n        pcVar8 = (char *)(param_1 + 0x1e1);\r\n      }\r\n      else {\r\n        pcVar8 = (char *)(param_1 + 0x1e0);\r\n      }\r\n    }\r\n    else {\r\n      pcVar8 = (char *)(param_1 + 0x1e5);\r\n      if (!(bool)cVar2) {\r\n        pcVar8 = (char *)(param_1 + 0x1e4);\r\n      }\r\n    }\r\n    if (local_res1b != pcVar8) {\r\n      cVar9 = *pcVar8;\r\n    }\r\n    if ((cVar9 == *(char *)(param_1 + 0x1e1)) ||\r\n       (local_res8 = 0, cVar9 == *(char *)(param_1 + 0x1e0))) {\r\n      local_res8 = 1;\r\n    }\r\n    FUN_14074c100(param_1);\r\n  }\r\n  *(undefined1 *)(param_1 + 0x1ec) = 0;\r\n  if (*(int *)(param_1 + 0x48) == 1) {\r\n    FUN_14074adf0(param_1 + 0x80);\r\n    FUN_1406df740(param_1 + 0x40);\r\n  }\r\n  if (*(int *)(param_1 + 0x48) != 1) {\r\n    local_res20 = (longlong *)FUN_1406df6d0(param_1 + 0x40);\r\n    lVar5 = FUN_1406df6d0(param_1 + 0x40,1);\r\n    if ((local_res20 == (longlong *)0x0) ||\r\n       (cVar3 = (**(code **)(*local_res20 + 0x20))(local_res20,lVar5), cVar3 != '\\0')) {\r\n      *(char *)(lVar5 + 0x12) = cVar2;\r\n      *(undefined1 *)(lVar5 + 0x10) = local_res8;\r\n      *(undefined1 *)(lVar5 + 0x11) = local_res18._1_1_;\r\n      if ((char *)(lVar5 + 0x13) != local_res1b) {\r\n        *(char *)(lVar5 + 0x13) = cVar9;\r\n      }\r\n      *(undefined4 *)(param_1 + 0x4c) = 1;\r\n      if (((lVar4 != 0) && (7 < ((byte)*(undefined4 *)(lVar4 + 0x1c) & 0xc))) &&\r\n         (*(char *)(lVar4 + 0x1e9) == '\\0')) {\r\n        *(undefined1 *)(param_1 + 0x1e9) = 1;\r\n        FUN_14074b6e0(lVar4,param_2);\r\n        *(undefined1 *)(param_1 + 0x1e9) = 0;\r\n      }\r\n      FUN_1407e79a0(*(undefined8 *)(param_1 + 0xf8));\r\n      FUN_1407e79a0(param_1);\r\n    }\r\n  }\r\n  pbVar1 = (byte *)(*(longlong *)(param_1 + 0x10) + 0xd4);\r\n  *pbVar1 = *pbVar1 | 4;\r\n  lVar4 = FUN_1406dc560(*(undefined4 *)(param_1 + 0xdc));\r\n  if (lVar4 != 0) {\r\n    lVar4 = FUN_1406dc560(*(undefined4 *)(param_1 + 0xdc));\r\n    *(byte *)(lVar4 + 0xd4) = *(byte *)(lVar4 + 0xd4) | 4;\r\n  }\r\n  return;\r\n}\r\n\r\n"
          },
          "isError": false
        }
      }
    },
    {
      "source": "work/movement-integration/door-conflict-v749/v749-use-1.json",
      "sha256": "D1CE4C594FBE197FF4D304901C20B553831123BA75F1672A9B21558F9FAAB4E0",
      "transcript": {
        "status": "fulfilled",
        "value": {
          "content": [
            {
              "type": "text",
              "text": "\r\nvoid FUN_14074ba10(longlong param_1,longlong param_2)\r\n\r\n{\r\n  byte *pbVar1;\r\n  char cVar2;\r\n  longlong lVar3;\r\n  longlong lVar4;\r\n  longlong lVar5;\r\n  undefined1 *puVar6;\r\n  byte bVar7;\r\n  undefined2 local_res8;\r\n  undefined1 local_resa;\r\n  undefined1 local_resb [5];\r\n  longlong local_res10;\r\n  \r\n  bVar7 = *(byte *)(*(longlong *)(param_1 + 8) + 0x1f0);\r\n  local_res10 = param_2;\r\n  lVar3 = FUN_1403da910(param_1 + 0xe8,0);\r\n  if (lVar3 != 0) {\r\n    bVar7 = bVar7 | *(byte *)(*(longlong *)(lVar3 + 8) + 0x1f0);\r\n  }\r\n  if ((bVar7 == 0) && (*(int *)(param_1 + 0x48) == 0)) {\r\n    *(undefined4 *)(param_1 + 0x114) = **(undefined4 **)(param_2 + 8);\r\n    lVar3 = FUN_1406dc560();\r\n    lVar4 = FUN_1406dc560(*(undefined4 *)(param_1 + 0xdc));\r\n    lVar5 = FUN_1406dc560(*(undefined4 *)(param_1 + 0xdc));\r\n    local_resb[0] = 0xff;\r\n    local_resa = 0.0 < (*(float *)(lVar4 + 0x304) - *(float *)(lVar3 + 0x304)) *\r\n                       *(float *)(lVar5 + 0x310) +\r\n                       (*(float *)(lVar4 + 0x300) - *(float *)(lVar3 + 0x300)) *\r\n                       *(float *)(lVar5 + 0x30c) +\r\n                       (*(float *)(lVar4 + 0x308) - *(float *)(lVar3 + 0x308)) *\r\n                       *(float *)(lVar5 + 0x314);\r\n    if ((*(char *)(param_1 + 0x1e8) == '\\0') && (*(char *)(param_1 + 0x118) == '\\0')) {\r\n      if ((bool)local_resa) {\r\n        puVar6 = (undefined1 *)(param_1 + 0x1e7);\r\n      }\r\n      else {\r\n        puVar6 = (undefined1 *)(param_1 + 0x1e6);\r\n      }\r\n    }\r\n    else {\r\n      puVar6 = (undefined1 *)(param_1 + 0x1e5);\r\n      if (!(bool)local_resa) {\r\n        puVar6 = (undefined1 *)(param_1 + 0x1e4);\r\n      }\r\n    }\r\n    if (local_resb != puVar6) {\r\n      local_resb[0] = *puVar6;\r\n    }\r\n    local_res8 = 1;\r\n    *(undefined1 *)(param_1 + 0x1ec) = 1;\r\n    cVar2 = FUN_14074a400(param_1 + 0x40,&local_res8);\r\n    if (cVar2 != '\\0') {\r\n      lVar3 = FUN_1403da910(param_1 + 0xe8,0);\r\n      if (((lVar3 != 0) && (7 < ((byte)*(undefined4 *)(lVar3 + 0x1c) & 0xc))) &&\r\n         (*(char *)(lVar3 + 0x1e9) == '\\0')) {\r\n        *(undefined1 *)(param_1 + 0x1e9) = 1;\r\n        FUN_14074ba10(lVar3,local_res10);\r\n        *(undefined1 *)(param_1 + 0x1e9) = 0;\r\n      }\r\n      FUN_1407e79a0(*(undefined8 *)(param_1 + 0xf8));\r\n      FUN_1407e79a0(param_1);\r\n    }\r\n    pbVar1 = (byte *)(*(longlong *)(param_1 + 0x10) + 0xd4);\r\n    *pbVar1 = *pbVar1 | 4;\r\n    lVar3 = FUN_1406dc560(*(undefined4 *)(param_1 + 0xdc));\r\n    if (lVar3 != 0) {\r\n      lVar3 = FUN_1406dc560(*(undefined4 *)(param_1 + 0xdc));\r\n      *(byte *)(lVar3 + 0xd4) = *(byte *)(lVar3 + 0xd4) | 4;\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
            }
          ],
          "structuredContent": {
            "result": "\r\nvoid FUN_14074ba10(longlong param_1,longlong param_2)\r\n\r\n{\r\n  byte *pbVar1;\r\n  char cVar2;\r\n  longlong lVar3;\r\n  longlong lVar4;\r\n  longlong lVar5;\r\n  undefined1 *puVar6;\r\n  byte bVar7;\r\n  undefined2 local_res8;\r\n  undefined1 local_resa;\r\n  undefined1 local_resb [5];\r\n  longlong local_res10;\r\n  \r\n  bVar7 = *(byte *)(*(longlong *)(param_1 + 8) + 0x1f0);\r\n  local_res10 = param_2;\r\n  lVar3 = FUN_1403da910(param_1 + 0xe8,0);\r\n  if (lVar3 != 0) {\r\n    bVar7 = bVar7 | *(byte *)(*(longlong *)(lVar3 + 8) + 0x1f0);\r\n  }\r\n  if ((bVar7 == 0) && (*(int *)(param_1 + 0x48) == 0)) {\r\n    *(undefined4 *)(param_1 + 0x114) = **(undefined4 **)(param_2 + 8);\r\n    lVar3 = FUN_1406dc560();\r\n    lVar4 = FUN_1406dc560(*(undefined4 *)(param_1 + 0xdc));\r\n    lVar5 = FUN_1406dc560(*(undefined4 *)(param_1 + 0xdc));\r\n    local_resb[0] = 0xff;\r\n    local_resa = 0.0 < (*(float *)(lVar4 + 0x304) - *(float *)(lVar3 + 0x304)) *\r\n                       *(float *)(lVar5 + 0x310) +\r\n                       (*(float *)(lVar4 + 0x300) - *(float *)(lVar3 + 0x300)) *\r\n                       *(float *)(lVar5 + 0x30c) +\r\n                       (*(float *)(lVar4 + 0x308) - *(float *)(lVar3 + 0x308)) *\r\n                       *(float *)(lVar5 + 0x314);\r\n    if ((*(char *)(param_1 + 0x1e8) == '\\0') && (*(char *)(param_1 + 0x118) == '\\0')) {\r\n      if ((bool)local_resa) {\r\n        puVar6 = (undefined1 *)(param_1 + 0x1e7);\r\n      }\r\n      else {\r\n        puVar6 = (undefined1 *)(param_1 + 0x1e6);\r\n      }\r\n    }\r\n    else {\r\n      puVar6 = (undefined1 *)(param_1 + 0x1e5);\r\n      if (!(bool)local_resa) {\r\n        puVar6 = (undefined1 *)(param_1 + 0x1e4);\r\n      }\r\n    }\r\n    if (local_resb != puVar6) {\r\n      local_resb[0] = *puVar6;\r\n    }\r\n    local_res8 = 1;\r\n    *(undefined1 *)(param_1 + 0x1ec) = 1;\r\n    cVar2 = FUN_14074a400(param_1 + 0x40,&local_res8);\r\n    if (cVar2 != '\\0') {\r\n      lVar3 = FUN_1403da910(param_1 + 0xe8,0);\r\n      if (((lVar3 != 0) && (7 < ((byte)*(undefined4 *)(lVar3 + 0x1c) & 0xc))) &&\r\n         (*(char *)(lVar3 + 0x1e9) == '\\0')) {\r\n        *(undefined1 *)(param_1 + 0x1e9) = 1;\r\n        FUN_14074ba10(lVar3,local_res10);\r\n        *(undefined1 *)(param_1 + 0x1e9) = 0;\r\n      }\r\n      FUN_1407e79a0(*(undefined8 *)(param_1 + 0xf8));\r\n      FUN_1407e79a0(param_1);\r\n    }\r\n    pbVar1 = (byte *)(*(longlong *)(param_1 + 0x10) + 0xd4);\r\n    *pbVar1 = *pbVar1 | 4;\r\n    lVar3 = FUN_1406dc560(*(undefined4 *)(param_1 + 0xdc));\r\n    if (lVar3 != 0) {\r\n      lVar3 = FUN_1406dc560(*(undefined4 *)(param_1 + 0xdc));\r\n      *(byte *)(lVar3 + 0xd4) = *(byte *)(lVar3 + 0xd4) | 4;\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
          },
          "isError": false
        }
      }
    },
    {
      "source": "work/movement-integration/door-conflict-v749/v749-use-2.json",
      "sha256": "A3F854DCA6E66038465A4C2585FAE370C7B0408FF0DC05F2399E41D55A79CE81",
      "transcript": {
        "status": "fulfilled",
        "value": {
          "content": [
            {
              "type": "text",
              "text": "From 143f2ce3c [DATA]\nFrom 14074bd32 in FUN_14074bbf0 [UNCONDITIONAL_CALL]\nFrom 14074bd56 in FUN_14074bbf0 [UNCONDITIONAL_CALL]"
            }
          ],
          "structuredContent": {
            "result": "From 143f2ce3c [DATA]\nFrom 14074bd32 in FUN_14074bbf0 [UNCONDITIONAL_CALL]\nFrom 14074bd56 in FUN_14074bbf0 [UNCONDITIONAL_CALL]"
          },
          "isError": false
        }
      }
    },
    {
      "source": "work/movement-integration/door-conflict-v749/bookmark-save-v762.json",
      "sha256": "9C3949E0478D11A2594702C6FDA52D2CC9EB684F7C1165301E1BCE61CE6B3EF4",
      "transcript": {
        "marked": {
          "content": [
            {
              "type": "text",
              "text": "{\"success\":true,\"address\":\"14074b6e0\",\"category\":\"NativeDoorUseConflict\",\"comment\":\"CONFIRMED exact Steam B671F824... Ghidra v749: ordinary native use74b6e0 stores interactor at Openable+114; Open2 closes from active Open-state side+c8; otherwise chooses opening side geometrically from player/door and entries1e0/1e1. Moving1 branch completes/commits existing native transition before inline queue, but this is NOT proof that player UI delivers use while Moving. RUNTIME752/753 both role orders: X during closing leaves revision2/close unchanged; fresh X after completion reopens both at revision3. Failed747 collision placement,750 reverse-side wall view,748/751 unsupported reversal expectations retained. RUNTIME754 simultaneous real opposite-side native openings (params7000001/6010001) both report ordered Open side1/applied3, but actual host Open side0/client side1. Logical Open alone insufficient for geometry convergence. RUNTIME755 accepted in bounded scope:756 Corvo and757 Emily each correct actual side0 to1 with two native calls/one correction;760 reverses placement and corrects side1 to0. Both actual stable Open sides equal host order revision3.758/759 ordinary busy-use regression passes with zero corrections. All five755 cases restore inventories/health14 protected hashes. Corrective motion is visible and can push a nearby player (756 about0.52 units); source camera limitations retained. No full animation or seamless-use acceptance. Audit native-door-side-audit-v762.json, doc59; automatic rearm and general door/world semantics remain outstanding. Transcripts work/movement-integration/door-conflict-v749; all observations saved under work/natural-moveset/native-door-*.\"}"
            }
          ],
          "structuredContent": {
            "result": "{\"success\":true,\"address\":\"14074b6e0\",\"category\":\"NativeDoorUseConflict\",\"comment\":\"CONFIRMED exact Steam B671F824... Ghidra v749: ordinary native use74b6e0 stores interactor at Openable+114; Open2 closes from active Open-state side+c8; otherwise chooses opening side geometrically from player/door and entries1e0/1e1. Moving1 branch completes/commits existing native transition before inline queue, but this is NOT proof that player UI delivers use while Moving. RUNTIME752/753 both role orders: X during closing leaves revision2/close unchanged; fresh X after completion reopens both at revision3. Failed747 collision placement,750 reverse-side wall view,748/751 unsupported reversal expectations retained. RUNTIME754 simultaneous real opposite-side native openings (params7000001/6010001) both report ordered Open side1/applied3, but actual host Open side0/client side1. Logical Open alone insufficient for geometry convergence. RUNTIME755 accepted in bounded scope:756 Corvo and757 Emily each correct actual side0 to1 with two native calls/one correction;760 reverses placement and corrects side1 to0. Both actual stable Open sides equal host order revision3.758/759 ordinary busy-use regression passes with zero corrections. All five755 cases restore inventories/health14 protected hashes. Corrective motion is visible and can push a nearby player (756 about0.52 units); source camera limitations retained. No full animation or seamless-use acceptance. Audit native-door-side-audit-v762.json, doc59; automatic rearm and general door/world semantics remain outstanding. Transcripts work/movement-integration/door-conflict-v749; all observations saved under work/natural-moveset/native-door-*.\"}"
          },
          "isError": false
        },
        "saved": {
          "content": [
            {
              "type": "text",
              "text": "{\"success\":true,\"program\":\"Dishonored2.exe\",\"message\":\"Program saved successfully\"}"
            }
          ],
          "structuredContent": {
            "result": "{\"success\":true,\"program\":\"Dishonored2.exe\",\"message\":\"Program saved successfully\"}"
          },
          "isError": false
        }
      }
    }
  ]
}
````

## Unescaped text fields for reading

### text

````text

ulonglong FUN_14074a400(longlong param_1,undefined1 *param_2)

{
  ulonglong in_RAX;
  longlong *plVar1;
  longlong lVar2;
  ulonglong uVar3;
  
  if (*(int *)(param_1 + 8) == 1) {
LAB_14074a482:
    uVar3 = in_RAX & 0xffffffffffffff00;
  }
  else {
    plVar1 = (longlong *)FUN_1406df6d0();
    lVar2 = FUN_1406df6d0(param_1,1);
    if (plVar1 != (longlong *)0x0) {
      in_RAX = (**(code **)(*plVar1 + 0x20))(plVar1,lVar2);
      if ((char)in_RAX == '\0') goto LAB_14074a482;
    }
    *(undefined1 *)(lVar2 + 0x10) = *param_2;
    *(undefined1 *)(lVar2 + 0x11) = param_2[1];
    *(undefined1 *)(lVar2 + 0x12) = param_2[2];
    if ((undefined1 *)(lVar2 + 0x13) != param_2 + 3) {
      *(undefined1 *)(lVar2 + 0x13) = param_2[3];
    }
    *(undefined4 *)(param_1 + 0xc) = 1;
    uVar3 = 1;
  }
  return uVar3;
}


````

### text

````text
From 143f2cb00 [DATA]
From 14074bb4e in FUN_14074ba10 [UNCONDITIONAL_CALL]
From 14074d878 in FUN_14074d7a0 [UNCONDITIONAL_CALL]
From 14074acc8 in FUN_14074ac50 [UNCONDITIONAL_CALL]
````

### text

````text

void FUN_14074b6e0(longlong param_1,longlong param_2)

{
  byte *pbVar1;
  char cVar2;
  char cVar3;
  longlong lVar4;
  longlong lVar5;
  longlong lVar6;
  longlong lVar7;
  char *pcVar8;
  char cVar9;
  undefined1 local_res8;
  undefined2 local_res18;
  char local_res1a;
  char local_res1b [5];
  longlong *local_res20;
  
  *(undefined4 *)(param_1 + 0x114) = **(undefined4 **)(param_2 + 8);
  lVar4 = FUN_1403da910(param_1 + 0xe8,0);
  cVar9 = -1;
  local_res1b[0] = -1;
  local_res18 = 0;
  local_res8 = 0;
  lVar5 = FUN_1406dc560(*(undefined4 *)(param_1 + 0x114));
  cVar2 = FUN_1405bf220(*(undefined4 *)(lVar5 + 0xa8));
  if (cVar2 == '\0') {
    lVar5 = FUN_1406dc560(*(undefined4 *)(param_1 + 0x114));
    cVar2 = local_res1a;
    if (lVar5 != 0) {
      FUN_14074dd50(param_1,&local_res18);
      local_res8 = (undefined1)local_res18;
      cVar2 = local_res1a;
      cVar9 = local_res1b[0];
    }
  }
  else if (*(int *)(param_1 + 0x48) == 2) {
    lVar5 = FUN_1406df6d0(param_1 + 0x40,*(undefined4 *)(param_1 + 0x48));
    cVar2 = *(char *)(lVar5 + 0x10);
    pcVar8 = (char *)(param_1 + 0x1e2);
    if (cVar2 == '\0') {
      pcVar8 = (char *)(param_1 + 0x1e3);
    }
    if (local_res1b != pcVar8) {
      cVar9 = *pcVar8;
    }
  }
  else {
    lVar5 = FUN_1405befa0();
    if ((lVar5 != 0) && (*(int *)(*(longlong *)(lVar5 + 0x2f8) + 0x5d0) != 0xe)) {
      lVar5 = FUN_140824760(*(longlong *)(lVar5 + 0x2f8) + 0x410);
      if (lVar5 == 0) {
        *(undefined4 *)(param_1 + 0x114) = 0x3fff0010;
      }
      else {
        *(undefined4 *)(param_1 + 0x114) = *(undefined4 *)(lVar5 + 0xa8);
      }
    }
    lVar5 = FUN_1406dc560(*(undefined4 *)(param_1 + 0x114));
    lVar6 = FUN_1406dc560(*(undefined4 *)(param_1 + 0xdc));
    lVar7 = FUN_1406dc560(*(undefined4 *)(param_1 + 0xdc));
    cVar2 = 0.0 <= (*(float *)(lVar6 + 0x304) - *(float *)(lVar5 + 0x304)) *
                   *(float *)(lVar7 + 0x310) +
                   (*(float *)(lVar6 + 0x300) - *(float *)(lVar5 + 0x300)) *
                   *(float *)(lVar7 + 0x30c) +
                   (*(float *)(lVar6 + 0x308) - *(float *)(lVar5 + 0x308)) *
                   *(float *)(lVar7 + 0x314);
    if ((*(char *)(param_1 + 0x1e8) == '\0') && (*(char *)(param_1 + 0x118) == '\0')) {
      if ((bool)cVar2) {
        pcVar8 = (char *)(param_1 + 0x1e1);
      }
      else {
        pcVar8 = (char *)(param_1 + 0x1e0);
      }
    }
    else {
      pcVar8 = (char *)(param_1 + 0x1e5);
      if (!(bool)cVar2) {
        pcVar8 = (char *)(param_1 + 0x1e4);
      }
    }
    if (local_res1b != pcVar8) {
      cVar9 = *pcVar8;
    }
    if ((cVar9 == *(char *)(param_1 + 0x1e1)) ||
       (local_res8 = 0, cVar9 == *(char *)(param_1 + 0x1e0))) {
      local_res8 = 1;
    }
    FUN_14074c100(param_1);
  }
  *(undefined1 *)(param_1 + 0x1ec) = 0;
  if (*(int *)(param_1 + 0x48) == 1) {
    FUN_14074adf0(param_1 + 0x80);
    FUN_1406df740(param_1 + 0x40);
  }
  if (*(int *)(param_1 + 0x48) != 1) {
    local_res20 = (longlong *)FUN_1406df6d0(param_1 + 0x40);
    lVar5 = FUN_1406df6d0(param_1 + 0x40,1);
    if ((local_res20 == (longlong *)0x0) ||
       (cVar3 = (**(code **)(*local_res20 + 0x20))(local_res20,lVar5), cVar3 != '\0')) {
      *(char *)(lVar5 + 0x12) = cVar2;
      *(undefined1 *)(lVar5 + 0x10) = local_res8;
      *(undefined1 *)(lVar5 + 0x11) = local_res18._1_1_;
      if ((char *)(lVar5 + 0x13) != local_res1b) {
        *(char *)(lVar5 + 0x13) = cVar9;
      }
      *(undefined4 *)(param_1 + 0x4c) = 1;
      if (((lVar4 != 0) && (7 < ((byte)*(undefined4 *)(lVar4 + 0x1c) & 0xc))) &&
         (*(char *)(lVar4 + 0x1e9) == '\0')) {
        *(undefined1 *)(param_1 + 0x1e9) = 1;
        FUN_14074b6e0(lVar4,param_2);
        *(undefined1 *)(param_1 + 0x1e9) = 0;
      }
      FUN_1407e79a0(*(undefined8 *)(param_1 + 0xf8));
      FUN_1407e79a0(param_1);
    }
  }
  pbVar1 = (byte *)(*(longlong *)(param_1 + 0x10) + 0xd4);
  *pbVar1 = *pbVar1 | 4;
  lVar4 = FUN_1406dc560(*(undefined4 *)(param_1 + 0xdc));
  if (lVar4 != 0) {
    lVar4 = FUN_1406dc560(*(undefined4 *)(param_1 + 0xdc));
    *(byte *)(lVar4 + 0xd4) = *(byte *)(lVar4 + 0xd4) | 4;
  }
  return;
}


````

### text

````text

void FUN_14074ba10(longlong param_1,longlong param_2)

{
  byte *pbVar1;
  char cVar2;
  longlong lVar3;
  longlong lVar4;
  longlong lVar5;
  undefined1 *puVar6;
  byte bVar7;
  undefined2 local_res8;
  undefined1 local_resa;
  undefined1 local_resb [5];
  longlong local_res10;
  
  bVar7 = *(byte *)(*(longlong *)(param_1 + 8) + 0x1f0);
  local_res10 = param_2;
  lVar3 = FUN_1403da910(param_1 + 0xe8,0);
  if (lVar3 != 0) {
    bVar7 = bVar7 | *(byte *)(*(longlong *)(lVar3 + 8) + 0x1f0);
  }
  if ((bVar7 == 0) && (*(int *)(param_1 + 0x48) == 0)) {
    *(undefined4 *)(param_1 + 0x114) = **(undefined4 **)(param_2 + 8);
    lVar3 = FUN_1406dc560();
    lVar4 = FUN_1406dc560(*(undefined4 *)(param_1 + 0xdc));
    lVar5 = FUN_1406dc560(*(undefined4 *)(param_1 + 0xdc));
    local_resb[0] = 0xff;
    local_resa = 0.0 < (*(float *)(lVar4 + 0x304) - *(float *)(lVar3 + 0x304)) *
                       *(float *)(lVar5 + 0x310) +
                       (*(float *)(lVar4 + 0x300) - *(float *)(lVar3 + 0x300)) *
                       *(float *)(lVar5 + 0x30c) +
                       (*(float *)(lVar4 + 0x308) - *(float *)(lVar3 + 0x308)) *
                       *(float *)(lVar5 + 0x314);
    if ((*(char *)(param_1 + 0x1e8) == '\0') && (*(char *)(param_1 + 0x118) == '\0')) {
      if ((bool)local_resa) {
        puVar6 = (undefined1 *)(param_1 + 0x1e7);
      }
      else {
        puVar6 = (undefined1 *)(param_1 + 0x1e6);
      }
    }
    else {
      puVar6 = (undefined1 *)(param_1 + 0x1e5);
      if (!(bool)local_resa) {
        puVar6 = (undefined1 *)(param_1 + 0x1e4);
      }
    }
    if (local_resb != puVar6) {
      local_resb[0] = *puVar6;
    }
    local_res8 = 1;
    *(undefined1 *)(param_1 + 0x1ec) = 1;
    cVar2 = FUN_14074a400(param_1 + 0x40,&local_res8);
    if (cVar2 != '\0') {
      lVar3 = FUN_1403da910(param_1 + 0xe8,0);
      if (((lVar3 != 0) && (7 < ((byte)*(undefined4 *)(lVar3 + 0x1c) & 0xc))) &&
         (*(char *)(lVar3 + 0x1e9) == '\0')) {
        *(undefined1 *)(param_1 + 0x1e9) = 1;
        FUN_14074ba10(lVar3,local_res10);
        *(undefined1 *)(param_1 + 0x1e9) = 0;
      }
      FUN_1407e79a0(*(undefined8 *)(param_1 + 0xf8));
      FUN_1407e79a0(param_1);
    }
    pbVar1 = (byte *)(*(longlong *)(param_1 + 0x10) + 0xd4);
    *pbVar1 = *pbVar1 | 4;
    lVar3 = FUN_1406dc560(*(undefined4 *)(param_1 + 0xdc));
    if (lVar3 != 0) {
      lVar3 = FUN_1406dc560(*(undefined4 *)(param_1 + 0xdc));
      *(byte *)(lVar3 + 0xd4) = *(byte *)(lVar3 + 0xd4) | 4;
    }
  }
  return;
}


````

### text

````text
{"success":true,"address":"14074b6e0","category":"NativeDoorUseConflict","comment":"CONFIRMED exact Steam B671F824... Ghidra v749: ordinary native use74b6e0 stores interactor at Openable+114; Open2 closes from active Open-state side+c8; otherwise chooses opening side geometrically from player/door and entries1e0/1e1. Moving1 branch completes/commits existing native transition before inline queue, but this is NOT proof that player UI delivers use while Moving. RUNTIME752/753 both role orders: X during closing leaves revision2/close unchanged; fresh X after completion reopens both at revision3. Failed747 collision placement,750 reverse-side wall view,748/751 unsupported reversal expectations retained. RUNTIME754 simultaneous real opposite-side native openings (params7000001/6010001) both report ordered Open side1/applied3, but actual host Open side0/client side1. Logical Open alone insufficient for geometry convergence. RUNTIME755 accepted in bounded scope:756 Corvo and757 Emily each correct actual side0 to1 with two native calls/one correction;760 reverses placement and corrects side1 to0. Both actual stable Open sides equal host order revision3.758/759 ordinary busy-use regression passes with zero corrections. All five755 cases restore inventories/health14 protected hashes. Corrective motion is visible and can push a nearby player (756 about0.52 units); source camera limitations retained. No full animation or seamless-use acceptance. Audit native-door-side-audit-v762.json, doc59; automatic rearm and general door/world semantics remain outstanding. Transcripts work/movement-integration/door-conflict-v749; all observations saved under work/natural-moveset/native-door-*."}
````
