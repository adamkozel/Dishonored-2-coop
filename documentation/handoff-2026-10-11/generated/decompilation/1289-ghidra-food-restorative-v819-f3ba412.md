# ghidra-food-restorative-v819.json

Original: [work/natural-moveset/native-pickup-research-v785/ghidra-food-restorative-v819.json](<../../../../work/natural-moveset/native-pickup-research-v785/ghidra-food-restorative-v819.json>)

SHA-256: `979A818A4AC7F3859FF99CC49531D09C66DA032CA91132F3FC31D7DF736CE17C`; 19990 bytes.

## Original saved record

````json
[{"status":"fulfilled","value":{"address":"14071a140","result":{"content":[{"type":"text","text":"\r\n/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */\r\n\r\nvoid FUN_14071a140(longlong param_1,float param_2)\r\n\r\n{\r\n  longlong lVar1;\r\n  undefined4 *puVar2;\r\n  undefined8 uVar3;\r\n  undefined1 local_e8 [32];\r\n  undefined1 local_c8 [36];\r\n  undefined4 local_a4;\r\n  undefined4 local_a0;\r\n  undefined4 local_9c;\r\n  undefined4 local_98;\r\n  undefined4 local_94;\r\n  undefined4 local_90;\r\n  undefined4 local_8c;\r\n  undefined4 local_88;\r\n  undefined4 local_84;\r\n  undefined4 local_78;\r\n  undefined4 local_74;\r\n  undefined1 local_38 [48];\r\n  \r\n  if (0.0 < param_2) {\r\n    FUN_140719fe0();\r\n    return;\r\n  }\r\n  if (param_2 < 0.0) {\r\n    lVar1 = *(longlong *)(param_1 + 0x10);\r\n    D2_DamageParams_Construct(local_c8,_DAT_14231f390);\r\n    FUN_140780a80(local_c8,-param_2,1);\r\n    puVar2 = (undefined4 *)FUN_140878230(lVar1,local_e8);\r\n    local_a4 = *puVar2;\r\n    local_a0 = puVar2[1];\r\n    local_9c = puVar2[2];\r\n    local_98 = DAT_14228a940;\r\n    local_94 = DAT_14228a944;\r\n    local_8c = DAT_1431af668;\r\n    local_90 = DAT_14228a948;\r\n    local_84 = DAT_1431af670;\r\n    local_88 = DAT_1431af66c;\r\n    if (lVar1 == 0) {\r\n      local_78 = 0x3fff0010;\r\n      local_74 = 0x3fff0010;\r\n    }\r\n    else {\r\n      local_78 = *(undefined4 *)(lVar1 + 0xa8);\r\n      local_74 = *(undefined4 *)(lVar1 + 0xa8);\r\n    }\r\n    uVar3 = FUN_140882e20(local_38,local_c8);\r\n    FUN_140884920(lVar1,local_e8,&DAT_1439a9d20,uVar3);\r\n    FUN_140882fc0(local_e8);\r\n  }\r\n  return;\r\n}\r\n\r\n"}],"structuredContent":{"result":"\r\n/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */\r\n\r\nvoid FUN_14071a140(longlong param_1,float param_2)\r\n\r\n{\r\n  longlong lVar1;\r\n  undefined4 *puVar2;\r\n  undefined8 uVar3;\r\n  undefined1 local_e8 [32];\r\n  undefined1 local_c8 [36];\r\n  undefined4 local_a4;\r\n  undefined4 local_a0;\r\n  undefined4 local_9c;\r\n  undefined4 local_98;\r\n  undefined4 local_94;\r\n  undefined4 local_90;\r\n  undefined4 local_8c;\r\n  undefined4 local_88;\r\n  undefined4 local_84;\r\n  undefined4 local_78;\r\n  undefined4 local_74;\r\n  undefined1 local_38 [48];\r\n  \r\n  if (0.0 < param_2) {\r\n    FUN_140719fe0();\r\n    return;\r\n  }\r\n  if (param_2 < 0.0) {\r\n    lVar1 = *(longlong *)(param_1 + 0x10);\r\n    D2_DamageParams_Construct(local_c8,_DAT_14231f390);\r\n    FUN_140780a80(local_c8,-param_2,1);\r\n    puVar2 = (undefined4 *)FUN_140878230(lVar1,local_e8);\r\n    local_a4 = *puVar2;\r\n    local_a0 = puVar2[1];\r\n    local_9c = puVar2[2];\r\n    local_98 = DAT_14228a940;\r\n    local_94 = DAT_14228a944;\r\n    local_8c = DAT_1431af668;\r\n    local_90 = DAT_14228a948;\r\n    local_84 = DAT_1431af670;\r\n    local_88 = DAT_1431af66c;\r\n    if (lVar1 == 0) {\r\n      local_78 = 0x3fff0010;\r\n      local_74 = 0x3fff0010;\r\n    }\r\n    else {\r\n      local_78 = *(undefined4 *)(lVar1 + 0xa8);\r\n      local_74 = *(undefined4 *)(lVar1 + 0xa8);\r\n    }\r\n    uVar3 = FUN_140882e20(local_38,local_c8);\r\n    FUN_140884920(lVar1,local_e8,&DAT_1439a9d20,uVar3);\r\n    FUN_140882fc0(local_e8);\r\n  }\r\n  return;\r\n}\r\n\r\n"},"isError":false}}},{"status":"fulfilled","value":{"address":"1407467d0","result":{"content":[{"type":"text","text":"\r\nvoid FUN_1407467d0(longlong param_1,float param_2)\r\n\r\n{\r\n  char cVar1;\r\n  longlong lVar2;\r\n  float fVar3;\r\n  float fVar4;\r\n  float fVar5;\r\n  \r\n  fVar5 = 0.0;\r\n  fVar4 = *(float *)(*(longlong *)(param_1 + 8) + 0x68);\r\n  if (*(longlong *)(param_1 + 0x48) != 0) {\r\n    if (*(int *)(param_1 + 0x68) < 0) {\r\n      fVar3 = 0.0;\r\n    }\r\n    else {\r\n      fVar3 = *(float *)(*(longlong *)(*(longlong *)(param_1 + 0x48) + 0x48) + 0x24 +\r\n                        (longlong)*(int *)(param_1 + 0x68) * 0x28);\r\n    }\r\n    fVar4 = fVar4 + fVar3;\r\n  }\r\n  fVar4 = (param_2 + *(float *)(param_1 + 0x20)) - fVar4;\r\n  fVar3 = 0.0;\r\n  if (0.0 <= fVar4) {\r\n    fVar3 = fVar4;\r\n  }\r\n  if (0.0 < fVar3) {\r\n    cVar1 = FUN_140399620(D2_GameSingletonPtr + 0x1f7b90);\r\n    if (cVar1 != '\\0') {\r\n      lVar2 = FUN_140877720(*(undefined8 *)(param_1 + 0x10),&DAT_1439ae8e0,2);\r\n      if (lVar2 != 0) {\r\n        FUN_140719fe0(lVar2,fVar3);\r\n      }\r\n    }\r\n  }\r\n  lVar2 = (**(code **)**(undefined8 **)(param_1 + 0x10))();\r\n  if ((*(int *)(lVar2 + 0x60) < DAT_1439ce350) || (DAT_1439ce354 < *(int *)(lVar2 + 0x60))) {\r\n    FUN_140746a00(param_1,param_2 + *(float *)(param_1 + 0x20),0);\r\n    *(undefined4 *)(param_1 + 0x2c) = *(undefined4 *)(param_1 + 0x20);\r\n  }\r\n  else {\r\n    fVar4 = *(float *)(*(longlong *)(param_1 + 8) + 0x68);\r\n    if (*(longlong *)(param_1 + 0x48) != 0) {\r\n      if (-1 < *(int *)(param_1 + 0x68)) {\r\n        fVar5 = *(float *)(*(longlong *)(*(longlong *)(param_1 + 0x48) + 0x48) + 0x24 +\r\n                          (longlong)*(int *)(param_1 + 0x68) * 0x28);\r\n      }\r\n      fVar4 = fVar4 + fVar5;\r\n    }\r\n    param_2 = param_2 + *(float *)(param_1 + 0x2c);\r\n    if (param_2 <= fVar4) {\r\n      fVar4 = param_2;\r\n    }\r\n    *(float *)(param_1 + 0x2c) = fVar4;\r\n    FUN_1407e79a0(param_2);\r\n  }\r\n  return;\r\n}\r\n\r\n"}],"structuredContent":{"result":"\r\nvoid FUN_1407467d0(longlong param_1,float param_2)\r\n\r\n{\r\n  char cVar1;\r\n  longlong lVar2;\r\n  float fVar3;\r\n  float fVar4;\r\n  float fVar5;\r\n  \r\n  fVar5 = 0.0;\r\n  fVar4 = *(float *)(*(longlong *)(param_1 + 8) + 0x68);\r\n  if (*(longlong *)(param_1 + 0x48) != 0) {\r\n    if (*(int *)(param_1 + 0x68) < 0) {\r\n      fVar3 = 0.0;\r\n    }\r\n    else {\r\n      fVar3 = *(float *)(*(longlong *)(*(longlong *)(param_1 + 0x48) + 0x48) + 0x24 +\r\n                        (longlong)*(int *)(param_1 + 0x68) * 0x28);\r\n    }\r\n    fVar4 = fVar4 + fVar3;\r\n  }\r\n  fVar4 = (param_2 + *(float *)(param_1 + 0x20)) - fVar4;\r\n  fVar3 = 0.0;\r\n  if (0.0 <= fVar4) {\r\n    fVar3 = fVar4;\r\n  }\r\n  if (0.0 < fVar3) {\r\n    cVar1 = FUN_140399620(D2_GameSingletonPtr + 0x1f7b90);\r\n    if (cVar1 != '\\0') {\r\n      lVar2 = FUN_140877720(*(undefined8 *)(param_1 + 0x10),&DAT_1439ae8e0,2);\r\n      if (lVar2 != 0) {\r\n        FUN_140719fe0(lVar2,fVar3);\r\n      }\r\n    }\r\n  }\r\n  lVar2 = (**(code **)**(undefined8 **)(param_1 + 0x10))();\r\n  if ((*(int *)(lVar2 + 0x60) < DAT_1439ce350) || (DAT_1439ce354 < *(int *)(lVar2 + 0x60))) {\r\n    FUN_140746a00(param_1,param_2 + *(float *)(param_1 + 0x20),0);\r\n    *(undefined4 *)(param_1 + 0x2c) = *(undefined4 *)(param_1 + 0x20);\r\n  }\r\n  else {\r\n    fVar4 = *(float *)(*(longlong *)(param_1 + 8) + 0x68);\r\n    if (*(longlong *)(param_1 + 0x48) != 0) {\r\n      if (-1 < *(int *)(param_1 + 0x68)) {\r\n        fVar5 = *(float *)(*(longlong *)(*(longlong *)(param_1 + 0x48) + 0x48) + 0x24 +\r\n                          (longlong)*(int *)(param_1 + 0x68) * 0x28);\r\n      }\r\n      fVar4 = fVar4 + fVar5;\r\n    }\r\n    param_2 = param_2 + *(float *)(param_1 + 0x2c);\r\n    if (param_2 <= fVar4) {\r\n      fVar4 = param_2;\r\n    }\r\n    *(float *)(param_1 + 0x2c) = fVar4;\r\n    FUN_1407e79a0(param_2);\r\n  }\r\n  return;\r\n}\r\n\r\n"},"isError":false}}},{"status":"fulfilled","value":{"address":"140745fb0","result":{"content":[{"type":"text","text":"\r\nfloat FUN_140745fb0(longlong param_1)\r\n\r\n{\r\n  float fVar1;\r\n  \r\n  fVar1 = *(float *)(*(longlong *)(param_1 + 8) + 0x68);\r\n  if (*(longlong *)(param_1 + 0x48) != 0) {\r\n    if (-1 < *(int *)(param_1 + 0x68)) {\r\n      return fVar1 + *(float *)(*(longlong *)(*(longlong *)(param_1 + 0x48) + 0x48) + 0x24 +\r\n                               (longlong)*(int *)(param_1 + 0x68) * 0x28);\r\n    }\r\n    fVar1 = fVar1 + 0.0;\r\n  }\r\n  return fVar1;\r\n}\r\n\r\n"}],"structuredContent":{"result":"\r\nfloat FUN_140745fb0(longlong param_1)\r\n\r\n{\r\n  float fVar1;\r\n  \r\n  fVar1 = *(float *)(*(longlong *)(param_1 + 8) + 0x68);\r\n  if (*(longlong *)(param_1 + 0x48) != 0) {\r\n    if (-1 < *(int *)(param_1 + 0x68)) {\r\n      return fVar1 + *(float *)(*(longlong *)(*(longlong *)(param_1 + 0x48) + 0x48) + 0x24 +\r\n                               (longlong)*(int *)(param_1 + 0x68) * 0x28);\r\n    }\r\n    fVar1 = fVar1 + 0.0;\r\n  }\r\n  return fVar1;\r\n}\r\n\r\n"},"isError":false}}},{"status":"fulfilled","value":{"address":"140615d90","result":{"content":[{"type":"text","text":"\r\nbool FUN_140615d90(longlong param_1,int param_2)\r\n\r\n{\r\n  longlong lVar1;\r\n  longlong *plVar2;\r\n  longlong lVar3;\r\n  longlong lVar4;\r\n  longlong *plVar5;\r\n  bool bVar6;\r\n  float fVar7;\r\n  float fVar8;\r\n  float fVar9;\r\n  float fVar10;\r\n  \r\n  bVar6 = false;\r\n  if ((-1 < param_2) && (param_2 < (int)(*(uint *)(param_1 + 0x40) & 0xffffff))) {\r\n    plVar5 = (longlong *)((longlong)param_2 * 0x38 + *(longlong *)(param_1 + 0x38));\r\n    lVar1 = *plVar5;\r\n    if (*(int *)(lVar1 + 0x60) == 6) {\r\n      plVar2 = (longlong *)FUN_1403da910(param_1 + 0xf8,2);\r\n      lVar3 = FUN_1403da910(param_1 + 0x108,2);\r\n      fVar7 = 0.0;\r\n      fVar10 = 0.0;\r\n      fVar8 = 0.0;\r\n      fVar9 = 0.0;\r\n      lVar4 = (**(code **)**(undefined8 **)(param_1 + 0x10))();\r\n      if ((DAT_1439ce350 <= *(int *)(lVar4 + 0x60)) && (*(int *)(lVar4 + 0x60) <= DAT_1439ce354)) {\r\n        fVar7 = *(float *)(plVar2 + 4);\r\n        fVar10 = (float)(**(code **)(*plVar2 + 0xd8))(plVar2);\r\n        fVar8 = (float)FUN_140745f60(lVar3);\r\n        fVar9 = (float)FUN_140745fb0(lVar3);\r\n      }\r\n      if (((0 < *(int *)(lVar1 + 0x78)) && (plVar2 != (longlong *)0x0)) && (fVar10 <= fVar7)) {\r\n        bVar6 = *(int *)((longlong)plVar5 + 0x24) <= (int)plVar5[4];\r\n      }\r\n      if (((0 < *(int *)(lVar1 + 0x7c)) && (lVar3 != 0)) && (fVar9 <= fVar8)) {\r\n        bVar6 = *(int *)((longlong)plVar5 + 0x24) <= (int)plVar5[4];\r\n      }\r\n    }\r\n    else {\r\n      bVar6 = *(int *)((longlong)plVar5 + 0x24) <= (int)plVar5[4];\r\n    }\r\n    return bVar6;\r\n  }\r\n  return true;\r\n}\r\n\r\n"}],"structuredContent":{"result":"\r\nbool FUN_140615d90(longlong param_1,int param_2)\r\n\r\n{\r\n  longlong lVar1;\r\n  longlong *plVar2;\r\n  longlong lVar3;\r\n  longlong lVar4;\r\n  longlong *plVar5;\r\n  bool bVar6;\r\n  float fVar7;\r\n  float fVar8;\r\n  float fVar9;\r\n  float fVar10;\r\n  \r\n  bVar6 = false;\r\n  if ((-1 < param_2) && (param_2 < (int)(*(uint *)(param_1 + 0x40) & 0xffffff))) {\r\n    plVar5 = (longlong *)((longlong)param_2 * 0x38 + *(longlong *)(param_1 + 0x38));\r\n    lVar1 = *plVar5;\r\n    if (*(int *)(lVar1 + 0x60) == 6) {\r\n      plVar2 = (longlong *)FUN_1403da910(param_1 + 0xf8,2);\r\n      lVar3 = FUN_1403da910(param_1 + 0x108,2);\r\n      fVar7 = 0.0;\r\n      fVar10 = 0.0;\r\n      fVar8 = 0.0;\r\n      fVar9 = 0.0;\r\n      lVar4 = (**(code **)**(undefined8 **)(param_1 + 0x10))();\r\n      if ((DAT_1439ce350 <= *(int *)(lVar4 + 0x60)) && (*(int *)(lVar4 + 0x60) <= DAT_1439ce354)) {\r\n        fVar7 = *(float *)(plVar2 + 4);\r\n        fVar10 = (float)(**(code **)(*plVar2 + 0xd8))(plVar2);\r\n        fVar8 = (float)FUN_140745f60(lVar3);\r\n        fVar9 = (float)FUN_140745fb0(lVar3);\r\n      }\r\n      if (((0 < *(int *)(lVar1 + 0x78)) && (plVar2 != (longlong *)0x0)) && (fVar10 <= fVar7)) {\r\n        bVar6 = *(int *)((longlong)plVar5 + 0x24) <= (int)plVar5[4];\r\n      }\r\n      if (((0 < *(int *)(lVar1 + 0x7c)) && (lVar3 != 0)) && (fVar9 <= fVar8)) {\r\n        bVar6 = *(int *)((longlong)plVar5 + 0x24) <= (int)plVar5[4];\r\n      }\r\n    }\r\n    else {\r\n      bVar6 = *(int *)((longlong)plVar5 + 0x24) <= (int)plVar5[4];\r\n    }\r\n    return bVar6;\r\n  }\r\n  return true;\r\n}\r\n\r\n"},"isError":false}}},{"status":"fulfilled","value":{"address":"140719fe0","result":{"content":[{"type":"text","text":"\r\nvoid FUN_140719fe0(longlong *param_1,float param_2)\r\n\r\n{\r\n  undefined4 uVar1;\r\n  char cVar2;\r\n  longlong lVar3;\r\n  float *pfVar4;\r\n  float fVar5;\r\n  float fVar6;\r\n  float fVar7;\r\n  float fVar8;\r\n  \r\n  fVar5 = (float)FUN_140719990();\r\n  if ((*(byte *)(param_1[2] + 0xd1) & 8) == 0) {\r\n    fVar6 = (float)(**(code **)(*param_1 + 0xd8))(param_1);\r\n    cVar2 = FUN_140399620(D2_GameSingletonPtr + 0x1f7b90);\r\n    if (((cVar2 != '\\0') && (0.0 < (param_2 + *(float *)(param_1 + 4)) - fVar6)) &&\r\n       (param_1[0xe] != 0)) {\r\n      FUN_1407467d0();\r\n    }\r\n    lVar3 = (*(code *)**(undefined8 **)param_1[2])();\r\n    if ((*(int *)(lVar3 + 0x60) < DAT_1439ce350) || (DAT_1439ce354 < *(int *)(lVar3 + 0x60))) {\r\n      fVar7 = param_2 + *(float *)(param_1 + 4);\r\n      if (fVar6 <= param_2 + *(float *)(param_1 + 4)) {\r\n        fVar7 = fVar6;\r\n      }\r\n      D2_Health_SetCurrent(fVar6,fVar7);\r\n    }\r\n    else {\r\n      fVar7 = *(float *)((longlong)param_1 + 0xf4);\r\n      if (*(float *)((longlong)param_1 + 0xf4) <= *(float *)(param_1 + 4)) {\r\n        fVar7 = *(float *)(param_1 + 4);\r\n      }\r\n      fVar8 = fVar7 + param_2;\r\n      if (fVar6 <= fVar7 + param_2) {\r\n        fVar8 = fVar6;\r\n      }\r\n      *(float *)((longlong)param_1 + 0xf4) = fVar8;\r\n      FUN_1407e79a0(fVar6);\r\n    }\r\n  }\r\n  fVar6 = (float)FUN_140719990(param_1);\r\n  if (0.0 < fVar6 - fVar5) {\r\n    uVar1 = *(undefined4 *)(param_1[2] + 0xa8);\r\n    lVar3 = FUN_140810240();\r\n    pfVar4 = (float *)FUN_1408112c0(lVar3 + 0xc8e8,0xa4,4,uVar1);\r\n    if (pfVar4 != (float *)0x0) {\r\n      *pfVar4 = fVar6 - fVar5;\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"}],"structuredContent":{"result":"\r\nvoid FUN_140719fe0(longlong *param_1,float param_2)\r\n\r\n{\r\n  undefined4 uVar1;\r\n  char cVar2;\r\n  longlong lVar3;\r\n  float *pfVar4;\r\n  float fVar5;\r\n  float fVar6;\r\n  float fVar7;\r\n  float fVar8;\r\n  \r\n  fVar5 = (float)FUN_140719990();\r\n  if ((*(byte *)(param_1[2] + 0xd1) & 8) == 0) {\r\n    fVar6 = (float)(**(code **)(*param_1 + 0xd8))(param_1);\r\n    cVar2 = FUN_140399620(D2_GameSingletonPtr + 0x1f7b90);\r\n    if (((cVar2 != '\\0') && (0.0 < (param_2 + *(float *)(param_1 + 4)) - fVar6)) &&\r\n       (param_1[0xe] != 0)) {\r\n      FUN_1407467d0();\r\n    }\r\n    lVar3 = (*(code *)**(undefined8 **)param_1[2])();\r\n    if ((*(int *)(lVar3 + 0x60) < DAT_1439ce350) || (DAT_1439ce354 < *(int *)(lVar3 + 0x60))) {\r\n      fVar7 = param_2 + *(float *)(param_1 + 4);\r\n      if (fVar6 <= param_2 + *(float *)(param_1 + 4)) {\r\n        fVar7 = fVar6;\r\n      }\r\n      D2_Health_SetCurrent(fVar6,fVar7);\r\n    }\r\n    else {\r\n      fVar7 = *(float *)((longlong)param_1 + 0xf4);\r\n      if (*(float *)((longlong)param_1 + 0xf4) <= *(float *)(param_1 + 4)) {\r\n        fVar7 = *(float *)(param_1 + 4);\r\n      }\r\n      fVar8 = fVar7 + param_2;\r\n      if (fVar6 <= fVar7 + param_2) {\r\n        fVar8 = fVar6;\r\n      }\r\n      *(float *)((longlong)param_1 + 0xf4) = fVar8;\r\n      FUN_1407e79a0(fVar6);\r\n    }\r\n  }\r\n  fVar6 = (float)FUN_140719990(param_1);\r\n  if (0.0 < fVar6 - fVar5) {\r\n    uVar1 = *(undefined4 *)(param_1[2] + 0xa8);\r\n    lVar3 = FUN_140810240();\r\n    pfVar4 = (float *)FUN_1408112c0(lVar3 + 0xc8e8,0xa4,4,uVar1);\r\n    if (pfVar4 != (float *)0x0) {\r\n      *pfVar4 = fVar6 - fVar5;\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"},"isError":false}}},{"status":"fulfilled","value":{"address":"140745f60","result":{"content":[{"type":"text","text":"\r\nfloat FUN_140745f60(longlong param_1)\r\n\r\n{\r\n  float fVar1;\r\n  \r\n  if (DAT_14398dbd8 != 0) {\r\n    return *(float *)(param_1 + 0x20);\r\n  }\r\n  fVar1 = *(float *)(*(longlong *)(param_1 + 8) + 0x68);\r\n  if (*(longlong *)(param_1 + 0x48) != 0) {\r\n    if (-1 < *(int *)(param_1 + 0x68)) {\r\n      return fVar1 + *(float *)(*(longlong *)(*(longlong *)(param_1 + 0x48) + 0x48) + 0x24 +\r\n                               (longlong)*(int *)(param_1 + 0x68) * 0x28);\r\n    }\r\n    fVar1 = fVar1 + 0.0;\r\n  }\r\n  return fVar1;\r\n}\r\n\r\n"}],"structuredContent":{"result":"\r\nfloat FUN_140745f60(longlong param_1)\r\n\r\n{\r\n  float fVar1;\r\n  \r\n  if (DAT_14398dbd8 != 0) {\r\n    return *(float *)(param_1 + 0x20);\r\n  }\r\n  fVar1 = *(float *)(*(longlong *)(param_1 + 8) + 0x68);\r\n  if (*(longlong *)(param_1 + 0x48) != 0) {\r\n    if (-1 < *(int *)(param_1 + 0x68)) {\r\n      return fVar1 + *(float *)(*(longlong *)(*(longlong *)(param_1 + 0x48) + 0x48) + 0x24 +\r\n                               (longlong)*(int *)(param_1 + 0x68) * 0x28);\r\n    }\r\n    fVar1 = fVar1 + 0.0;\r\n  }\r\n  return fVar1;\r\n}\r\n\r\n"},"isError":false}}},{"status":"fulfilled","value":{"address":"140746a00","result":{"content":[{"type":"text","text":"\r\nvoid FUN_140746a00(longlong param_1,float param_2,char param_3)\r\n\r\n{\r\n  float fVar1;\r\n  float fVar2;\r\n  float fVar3;\r\n  float fVar4;\r\n  undefined1 local_68 [8];\r\n  int local_60;\r\n  undefined1 local_50 [8];\r\n  int local_48;\r\n  undefined1 local_38 [48];\r\n  \r\n  fVar3 = *(float *)(*(longlong *)(param_1 + 8) + 0x68);\r\n  if (*(longlong *)(param_1 + 0x48) != 0) {\r\n    if (*(int *)(param_1 + 0x68) < 0) {\r\n      fVar1 = 0.0;\r\n    }\r\n    else {\r\n      fVar1 = *(float *)(*(longlong *)(*(longlong *)(param_1 + 0x48) + 0x48) + 0x24 +\r\n                        (longlong)*(int *)(param_1 + 0x68) * 0x28);\r\n    }\r\n    fVar3 = fVar3 + fVar1;\r\n  }\r\n  fVar1 = *(float *)(param_1 + 0x20);\r\n  if (0.0 <= param_2) {\r\n    if (fVar3 < param_2) {\r\n      param_2 = fVar3;\r\n    }\r\n  }\r\n  else {\r\n    param_2 = 0.0;\r\n  }\r\n  *(float *)(param_1 + 0x20) = param_2;\r\n  if (param_3 == '\\0') {\r\n    fVar4 = *(float *)(param_1 + 0x2c) - (fVar1 - param_2);\r\n    fVar2 = 0.0;\r\n    if ((0.0 <= fVar4) && (fVar2 = fVar4, fVar3 < fVar4)) {\r\n      fVar2 = fVar3;\r\n    }\r\n    *(float *)(param_1 + 0x2c) = fVar2;\r\n  }\r\n  fVar3 = (float)FUN_1411ad95c(param_2 + 0.5);\r\n  fVar1 = (float)FUN_1411ad95c(fVar1 + 0.5);\r\n  local_48 = (int)fVar1;\r\n  if (local_48 != (int)fVar3) {\r\n    local_68[0] = 0x69;\r\n    local_50[0] = 0x69;\r\n    local_60 = (int)fVar3;\r\n    FUN_1408848a0(*(undefined8 *)(param_1 + 0x10),local_38,0x1439ab080,local_50,local_68);\r\n    FUN_140882fc0(local_38);\r\n  }\r\n  return;\r\n}\r\n\r\n"}],"structuredContent":{"result":"\r\nvoid FUN_140746a00(longlong param_1,float param_2,char param_3)\r\n\r\n{\r\n  float fVar1;\r\n  float fVar2;\r\n  float fVar3;\r\n  float fVar4;\r\n  undefined1 local_68 [8];\r\n  int local_60;\r\n  undefined1 local_50 [8];\r\n  int local_48;\r\n  undefined1 local_38 [48];\r\n  \r\n  fVar3 = *(float *)(*(longlong *)(param_1 + 8) + 0x68);\r\n  if (*(longlong *)(param_1 + 0x48) != 0) {\r\n    if (*(int *)(param_1 + 0x68) < 0) {\r\n      fVar1 = 0.0;\r\n    }\r\n    else {\r\n      fVar1 = *(float *)(*(longlong *)(*(longlong *)(param_1 + 0x48) + 0x48) + 0x24 +\r\n                        (longlong)*(int *)(param_1 + 0x68) * 0x28);\r\n    }\r\n    fVar3 = fVar3 + fVar1;\r\n  }\r\n  fVar1 = *(float *)(param_1 + 0x20);\r\n  if (0.0 <= param_2) {\r\n    if (fVar3 < param_2) {\r\n      param_2 = fVar3;\r\n    }\r\n  }\r\n  else {\r\n    param_2 = 0.0;\r\n  }\r\n  *(float *)(param_1 + 0x20) = param_2;\r\n  if (param_3 == '\\0') {\r\n    fVar4 = *(float *)(param_1 + 0x2c) - (fVar1 - param_2);\r\n    fVar2 = 0.0;\r\n    if ((0.0 <= fVar4) && (fVar2 = fVar4, fVar3 < fVar4)) {\r\n      fVar2 = fVar3;\r\n    }\r\n    *(float *)(param_1 + 0x2c) = fVar2;\r\n  }\r\n  fVar3 = (float)FUN_1411ad95c(param_2 + 0.5);\r\n  fVar1 = (float)FUN_1411ad95c(fVar1 + 0.5);\r\n  local_48 = (int)fVar1;\r\n  if (local_48 != (int)fVar3) {\r\n    local_68[0] = 0x69;\r\n    local_50[0] = 0x69;\r\n    local_60 = (int)fVar3;\r\n    FUN_1408848a0(*(undefined8 *)(param_1 + 0x10),local_38,0x1439ab080,local_50,local_68);\r\n    FUN_140882fc0(local_38);\r\n  }\r\n  return;\r\n}\r\n\r\n"},"isError":false}}}]

````

## Unescaped text fields for reading

### text

````text

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_14071a140(longlong param_1,float param_2)

{
  longlong lVar1;
  undefined4 *puVar2;
  undefined8 uVar3;
  undefined1 local_e8 [32];
  undefined1 local_c8 [36];
  undefined4 local_a4;
  undefined4 local_a0;
  undefined4 local_9c;
  undefined4 local_98;
  undefined4 local_94;
  undefined4 local_90;
  undefined4 local_8c;
  undefined4 local_88;
  undefined4 local_84;
  undefined4 local_78;
  undefined4 local_74;
  undefined1 local_38 [48];
  
  if (0.0 < param_2) {
    FUN_140719fe0();
    return;
  }
  if (param_2 < 0.0) {
    lVar1 = *(longlong *)(param_1 + 0x10);
    D2_DamageParams_Construct(local_c8,_DAT_14231f390);
    FUN_140780a80(local_c8,-param_2,1);
    puVar2 = (undefined4 *)FUN_140878230(lVar1,local_e8);
    local_a4 = *puVar2;
    local_a0 = puVar2[1];
    local_9c = puVar2[2];
    local_98 = DAT_14228a940;
    local_94 = DAT_14228a944;
    local_8c = DAT_1431af668;
    local_90 = DAT_14228a948;
    local_84 = DAT_1431af670;
    local_88 = DAT_1431af66c;
    if (lVar1 == 0) {
      local_78 = 0x3fff0010;
      local_74 = 0x3fff0010;
    }
    else {
      local_78 = *(undefined4 *)(lVar1 + 0xa8);
      local_74 = *(undefined4 *)(lVar1 + 0xa8);
    }
    uVar3 = FUN_140882e20(local_38,local_c8);
    FUN_140884920(lVar1,local_e8,&DAT_1439a9d20,uVar3);
    FUN_140882fc0(local_e8);
  }
  return;
}


````

### text

````text

void FUN_1407467d0(longlong param_1,float param_2)

{
  char cVar1;
  longlong lVar2;
  float fVar3;
  float fVar4;
  float fVar5;
  
  fVar5 = 0.0;
  fVar4 = *(float *)(*(longlong *)(param_1 + 8) + 0x68);
  if (*(longlong *)(param_1 + 0x48) != 0) {
    if (*(int *)(param_1 + 0x68) < 0) {
      fVar3 = 0.0;
    }
    else {
      fVar3 = *(float *)(*(longlong *)(*(longlong *)(param_1 + 0x48) + 0x48) + 0x24 +
                        (longlong)*(int *)(param_1 + 0x68) * 0x28);
    }
    fVar4 = fVar4 + fVar3;
  }
  fVar4 = (param_2 + *(float *)(param_1 + 0x20)) - fVar4;
  fVar3 = 0.0;
  if (0.0 <= fVar4) {
    fVar3 = fVar4;
  }
  if (0.0 < fVar3) {
    cVar1 = FUN_140399620(D2_GameSingletonPtr + 0x1f7b90);
    if (cVar1 != '\0') {
      lVar2 = FUN_140877720(*(undefined8 *)(param_1 + 0x10),&DAT_1439ae8e0,2);
      if (lVar2 != 0) {
        FUN_140719fe0(lVar2,fVar3);
      }
    }
  }
  lVar2 = (**(code **)**(undefined8 **)(param_1 + 0x10))();
  if ((*(int *)(lVar2 + 0x60) < DAT_1439ce350) || (DAT_1439ce354 < *(int *)(lVar2 + 0x60))) {
    FUN_140746a00(param_1,param_2 + *(float *)(param_1 + 0x20),0);
    *(undefined4 *)(param_1 + 0x2c) = *(undefined4 *)(param_1 + 0x20);
  }
  else {
    fVar4 = *(float *)(*(longlong *)(param_1 + 8) + 0x68);
    if (*(longlong *)(param_1 + 0x48) != 0) {
      if (-1 < *(int *)(param_1 + 0x68)) {
        fVar5 = *(float *)(*(longlong *)(*(longlong *)(param_1 + 0x48) + 0x48) + 0x24 +
                          (longlong)*(int *)(param_1 + 0x68) * 0x28);
      }
      fVar4 = fVar4 + fVar5;
    }
    param_2 = param_2 + *(float *)(param_1 + 0x2c);
    if (param_2 <= fVar4) {
      fVar4 = param_2;
    }
    *(float *)(param_1 + 0x2c) = fVar4;
    FUN_1407e79a0(param_2);
  }
  return;
}


````

### text

````text

float FUN_140745fb0(longlong param_1)

{
  float fVar1;
  
  fVar1 = *(float *)(*(longlong *)(param_1 + 8) + 0x68);
  if (*(longlong *)(param_1 + 0x48) != 0) {
    if (-1 < *(int *)(param_1 + 0x68)) {
      return fVar1 + *(float *)(*(longlong *)(*(longlong *)(param_1 + 0x48) + 0x48) + 0x24 +
                               (longlong)*(int *)(param_1 + 0x68) * 0x28);
    }
    fVar1 = fVar1 + 0.0;
  }
  return fVar1;
}


````

### text

````text

bool FUN_140615d90(longlong param_1,int param_2)

{
  longlong lVar1;
  longlong *plVar2;
  longlong lVar3;
  longlong lVar4;
  longlong *plVar5;
  bool bVar6;
  float fVar7;
  float fVar8;
  float fVar9;
  float fVar10;
  
  bVar6 = false;
  if ((-1 < param_2) && (param_2 < (int)(*(uint *)(param_1 + 0x40) & 0xffffff))) {
    plVar5 = (longlong *)((longlong)param_2 * 0x38 + *(longlong *)(param_1 + 0x38));
    lVar1 = *plVar5;
    if (*(int *)(lVar1 + 0x60) == 6) {
      plVar2 = (longlong *)FUN_1403da910(param_1 + 0xf8,2);
      lVar3 = FUN_1403da910(param_1 + 0x108,2);
      fVar7 = 0.0;
      fVar10 = 0.0;
      fVar8 = 0.0;
      fVar9 = 0.0;
      lVar4 = (**(code **)**(undefined8 **)(param_1 + 0x10))();
      if ((DAT_1439ce350 <= *(int *)(lVar4 + 0x60)) && (*(int *)(lVar4 + 0x60) <= DAT_1439ce354)) {
        fVar7 = *(float *)(plVar2 + 4);
        fVar10 = (float)(**(code **)(*plVar2 + 0xd8))(plVar2);
        fVar8 = (float)FUN_140745f60(lVar3);
        fVar9 = (float)FUN_140745fb0(lVar3);
      }
      if (((0 < *(int *)(lVar1 + 0x78)) && (plVar2 != (longlong *)0x0)) && (fVar10 <= fVar7)) {
        bVar6 = *(int *)((longlong)plVar5 + 0x24) <= (int)plVar5[4];
      }
      if (((0 < *(int *)(lVar1 + 0x7c)) && (lVar3 != 0)) && (fVar9 <= fVar8)) {
        bVar6 = *(int *)((longlong)plVar5 + 0x24) <= (int)plVar5[4];
      }
    }
    else {
      bVar6 = *(int *)((longlong)plVar5 + 0x24) <= (int)plVar5[4];
    }
    return bVar6;
  }
  return true;
}


````

### text

````text

void FUN_140719fe0(longlong *param_1,float param_2)

{
  undefined4 uVar1;
  char cVar2;
  longlong lVar3;
  float *pfVar4;
  float fVar5;
  float fVar6;
  float fVar7;
  float fVar8;
  
  fVar5 = (float)FUN_140719990();
  if ((*(byte *)(param_1[2] + 0xd1) & 8) == 0) {
    fVar6 = (float)(**(code **)(*param_1 + 0xd8))(param_1);
    cVar2 = FUN_140399620(D2_GameSingletonPtr + 0x1f7b90);
    if (((cVar2 != '\0') && (0.0 < (param_2 + *(float *)(param_1 + 4)) - fVar6)) &&
       (param_1[0xe] != 0)) {
      FUN_1407467d0();
    }
    lVar3 = (*(code *)**(undefined8 **)param_1[2])();
    if ((*(int *)(lVar3 + 0x60) < DAT_1439ce350) || (DAT_1439ce354 < *(int *)(lVar3 + 0x60))) {
      fVar7 = param_2 + *(float *)(param_1 + 4);
      if (fVar6 <= param_2 + *(float *)(param_1 + 4)) {
        fVar7 = fVar6;
      }
      D2_Health_SetCurrent(fVar6,fVar7);
    }
    else {
      fVar7 = *(float *)((longlong)param_1 + 0xf4);
      if (*(float *)((longlong)param_1 + 0xf4) <= *(float *)(param_1 + 4)) {
        fVar7 = *(float *)(param_1 + 4);
      }
      fVar8 = fVar7 + param_2;
      if (fVar6 <= fVar7 + param_2) {
        fVar8 = fVar6;
      }
      *(float *)((longlong)param_1 + 0xf4) = fVar8;
      FUN_1407e79a0(fVar6);
    }
  }
  fVar6 = (float)FUN_140719990(param_1);
  if (0.0 < fVar6 - fVar5) {
    uVar1 = *(undefined4 *)(param_1[2] + 0xa8);
    lVar3 = FUN_140810240();
    pfVar4 = (float *)FUN_1408112c0(lVar3 + 0xc8e8,0xa4,4,uVar1);
    if (pfVar4 != (float *)0x0) {
      *pfVar4 = fVar6 - fVar5;
    }
  }
  return;
}


````

### text

````text

float FUN_140745f60(longlong param_1)

{
  float fVar1;
  
  if (DAT_14398dbd8 != 0) {
    return *(float *)(param_1 + 0x20);
  }
  fVar1 = *(float *)(*(longlong *)(param_1 + 8) + 0x68);
  if (*(longlong *)(param_1 + 0x48) != 0) {
    if (-1 < *(int *)(param_1 + 0x68)) {
      return fVar1 + *(float *)(*(longlong *)(*(longlong *)(param_1 + 0x48) + 0x48) + 0x24 +
                               (longlong)*(int *)(param_1 + 0x68) * 0x28);
    }
    fVar1 = fVar1 + 0.0;
  }
  return fVar1;
}


````

### text

````text

void FUN_140746a00(longlong param_1,float param_2,char param_3)

{
  float fVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  undefined1 local_68 [8];
  int local_60;
  undefined1 local_50 [8];
  int local_48;
  undefined1 local_38 [48];
  
  fVar3 = *(float *)(*(longlong *)(param_1 + 8) + 0x68);
  if (*(longlong *)(param_1 + 0x48) != 0) {
    if (*(int *)(param_1 + 0x68) < 0) {
      fVar1 = 0.0;
    }
    else {
      fVar1 = *(float *)(*(longlong *)(*(longlong *)(param_1 + 0x48) + 0x48) + 0x24 +
                        (longlong)*(int *)(param_1 + 0x68) * 0x28);
    }
    fVar3 = fVar3 + fVar1;
  }
  fVar1 = *(float *)(param_1 + 0x20);
  if (0.0 <= param_2) {
    if (fVar3 < param_2) {
      param_2 = fVar3;
    }
  }
  else {
    param_2 = 0.0;
  }
  *(float *)(param_1 + 0x20) = param_2;
  if (param_3 == '\0') {
    fVar4 = *(float *)(param_1 + 0x2c) - (fVar1 - param_2);
    fVar2 = 0.0;
    if ((0.0 <= fVar4) && (fVar2 = fVar4, fVar3 < fVar4)) {
      fVar2 = fVar3;
    }
    *(float *)(param_1 + 0x2c) = fVar2;
  }
  fVar3 = (float)FUN_1411ad95c(param_2 + 0.5);
  fVar1 = (float)FUN_1411ad95c(fVar1 + 0.5);
  local_48 = (int)fVar1;
  if (local_48 != (int)fVar3) {
    local_68[0] = 0x69;
    local_50[0] = 0x69;
    local_60 = (int)fVar3;
    FUN_1408848a0(*(undefined8 *)(param_1 + 0x10),local_38,0x1439ab080,local_50,local_68);
    FUN_140882fc0(local_38);
  }
  return;
}


````
