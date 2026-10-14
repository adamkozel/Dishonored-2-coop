
void FUN_14085eaf0(longlong param_1)

{
  char cVar1;
  longlong lVar2;
  bool bVar3;
  int iVar4;
  undefined8 *puVar5;
  longlong lVar6;
  longlong lVar7;
  longlong lVar8;
  undefined8 uVar9;
  float *pfVar10;
  longlong lVar11;
  byte bVar12;
  float fVar13;
  int aiStackX_8 [2];
  float afStackX_10 [2];
  undefined1 *puVar14;
  undefined8 uStack_288;
  undefined4 uStack_280;
  undefined8 uStack_278;
  undefined1 auStack_270 [56];
  undefined1 *puStack_238;
  undefined4 uStack_230;
  uint uStack_22c;
  undefined1 auStack_228 [496];
  
  uStack_278 = 0xfffffffffffffffe;
  lVar2 = *(longlong *)(param_1 + 0x10);
  lVar7 = *(longlong *)(lVar2 + 0x300);
  if ((*(byte *)(param_1 + 0x118) & 4) == 0) {
    puVar5 = (undefined8 *)FUN_14085e930(auStack_270,*(undefined8 *)(param_1 + 0x108));
    *(undefined8 *)(param_1 + 0xac) = *puVar5;
    *(undefined4 *)(param_1 + 0xb4) = *(undefined4 *)(puVar5 + 1);
    *(undefined4 *)(param_1 + 0xb8) = *(undefined4 *)((longlong)puVar5 + 0xc);
    *(undefined4 *)(param_1 + 0xbc) = *(undefined4 *)(puVar5 + 2);
    *(undefined4 *)(param_1 + 0xc0) = *(undefined4 *)((longlong)puVar5 + 0x14);
    *(undefined4 *)(param_1 + 0xc4) = *(undefined4 *)(puVar5 + 3);
    *(undefined4 *)(param_1 + 200) = *(undefined4 *)((longlong)puVar5 + 0x1c);
    *(undefined4 *)(param_1 + 0xcc) = *(undefined4 *)(puVar5 + 4);
    *(undefined4 *)(param_1 + 0xd0) = *(undefined4 *)((longlong)puVar5 + 0x24);
    *(undefined4 *)(param_1 + 0xd4) = *(undefined4 *)(puVar5 + 5);
    *(undefined4 *)(param_1 + 0xd8) = *(undefined4 *)((longlong)puVar5 + 0x2c);
    *(byte *)(param_1 + 0xdc) =
         *(byte *)(param_1 + 0xdc) ^ (*(byte *)(param_1 + 0xdc) ^ *(byte *)(puVar5 + 6)) & 1;
    bVar12 = (*(byte *)(param_1 + 0xdc) ^ *(byte *)(puVar5 + 6)) & 2 ^ *(byte *)(param_1 + 0xdc);
    *(byte *)(param_1 + 0xdc) = bVar12;
    *(byte *)(param_1 + 0xdc) = (bVar12 ^ *(byte *)(puVar5 + 6)) & 4 ^ bVar12;
  }
  FUN_140853d40(param_1);
  lVar11 = *(longlong *)(param_1 + 0x18);
  if ((*(int *)(param_1 + 0xd8) == 0) && ((*(byte *)(lVar11 + 0x65) & 0x10) != 0)) {
    FUN_140854d10(param_1,0);
  }
  iVar4 = FUN_140802de0(lVar2);
  bVar3 = false;
  if (iVar4 == 2) {
    *(undefined8 *)(param_1 + 0xb4) = 2;
  }
  uStack_288 = 0;
  uStack_280 = 0;
  FUN_1407527e0(*(undefined8 *)(lVar2 + 800),"arkPlayerStateMantle",0,&uStack_288);
  lVar6 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x2f8);
  if ((*(int *)(param_1 + 0xb8) != 10) &&
     ((*(ulonglong *)(lVar6 + 0xa0) >> ((ulonglong)*(uint *)(lVar6 + 0x6c) & 0x3f) & 1) != 0)) {
    *(undefined4 *)(param_1 + 0xb8) = 6;
  }
  if ((((*(longlong *)(lVar2 + 0x90) != 0) && (*(int *)(param_1 + 0xb8) < 0x5a)) &&
      (lVar6 = *(longlong *)
                (*(longlong *)(lVar2 + 0x90) + 0x10 + (longlong)*(int *)(param_1 + 0xb8) * 8),
      lVar6 != 0)) && (cVar1 = *(char *)(lVar6 + 0x34), -1 < cVar1)) {
    if (*(int *)(param_1 + 0xd8) == 0) {
      FUN_140774970();
    }
    else {
      FUN_140774760(lVar7,cVar1,7);
    }
  }
  lVar6 = FUN_140854880(param_1);
  FUN_140770990(lVar7,lVar6 + 0x300,*(undefined8 *)(lVar11 + 200));
  *(undefined1 *)(lVar7 + 0xc48) = 0;
  *(undefined8 *)(lVar7 + 0xd98) = 0;
  *(undefined8 *)(lVar7 + 0xda0) = 0;
  lVar7 = FUN_140854880(param_1);
  *(undefined4 *)(param_1 + 0xe8) = *(undefined4 *)(lVar7 + 0x300);
  *(undefined4 *)(param_1 + 0xec) = *(undefined4 *)(lVar7 + 0x304);
  *(undefined4 *)(param_1 + 0xf0) = *(undefined4 *)(lVar7 + 0x308);
  puVar5 = (undefined8 *)FUN_1406dc560();
  if (puVar5 == (undefined8 *)0x0) {
    lVar7 = 0;
  }
  else {
    lVar7 = FUN_140877720(puVar5,&DAT_1439aee80,2);
    lVar6 = (**(code **)*puVar5)(puVar5);
    if (((DAT_1439ce230 <= *(int *)(lVar6 + 0x60)) && (*(int *)(lVar6 + 0x60) <= DAT_1439ce234)) ||
       ((lVar7 != 0 && (*(int *)(*(longlong *)(lVar7 + 8) + 0x78) == 5)))) {
      bVar3 = true;
    }
  }
  lVar6 = *(longlong *)(param_1 + 0x18);
  if (*(int *)(param_1 + 0xb8) == 6) {
    iVar4 = 1;
  }
  else {
    iVar4 = 0;
    if (*(int *)(param_1 + 0xb8) != 10) {
      iVar4 = 2;
    }
  }
  lVar8 = (longlong)iVar4;
  if (*(int *)(param_1 + 0xd8) == 0) {
    lVar6 = *(longlong *)(lVar6 + 0x70 + lVar8 * 8);
  }
  else if (*(int *)(param_1 + 0xd8) == 1) {
    lVar6 = *(longlong *)(lVar6 + 0x88 + lVar8 * 8);
  }
  else {
    lVar6 = *(longlong *)(lVar6 + 0xa0 + lVar8 * 8);
  }
  if (lVar6 != 0) {
    if (*(longlong *)(param_1 + 0xe0) != 0) {
      uVar9 = FUN_14010d490(param_1 + 200,auStack_270);
      FUN_14087d1e0(*(undefined8 *)(param_1 + 0xe0),param_1 + 0xbc,uVar9,0);
      if ((bVar3) || (*(int *)(*(longlong *)(lVar2 + 0x2f8) + 0x2d0) != 0)) {
        if ((DAT_142433ed0 & 1) == 0) {
          DAT_142433ed0 = DAT_142433ed0 | 1;
          FUN_140131ae0(&DAT_142433ec8);
        }
        FUN_140875e80(puVar5,*(undefined8 *)(param_1 + 0xe0),&DAT_142433ec8,0,0,1);
      }
      FUN_14080cd60(D2_GameSingletonPtr + 0x1f52e8,*(undefined8 *)(lVar2 + 0x10),
                    *(undefined8 *)(param_1 + 0xe0));
    }
    uStack_230 = 0x5000000;
    puStack_238 = auStack_228;
    uStack_22c = 0xa40;
    puVar14 = &LAB_1400ecab0;
    FUN_1411ad700(auStack_228,0x30,10,FUN_140389210,&LAB_1400ecab0);
    FUN_140860030(param_1,&puStack_238);
    afStackX_10[0] = 0.0;
    FUN_1408549c0(param_1,aiStackX_8,lVar6,&puStack_238,(ulonglong)puVar14 & 0xffffffff00000000);
    if (aiStackX_8[0] != 0) {
      if (*(int *)(param_1 + 0x110) < 0) {
        fVar13 = 1.0;
      }
      else {
        fVar13 = *(float *)(*(longlong *)
                             (*(longlong *)(*(longlong *)(param_1 + 0x10) + 0x330) + 0x48) + 0x24 +
                           (longlong)*(int *)(param_1 + 0x110) * 0x28);
        if (fVar13 != 1.0) {
          FUN_1407d8fe0(D2_GameSingletonPtr + 0x1f5338,aiStackX_8[0],fVar13);
        }
      }
      pfVar10 = (float *)FUN_1407d5e40(D2_GameSingletonPtr + 0x1f5338,afStackX_10,aiStackX_8[0],1);
      afStackX_10[0] = fVar13 * *pfVar10;
      *(float *)(param_1 + 0x114) = afStackX_10[0];
    }
    FUN_140860680(param_1);
    FUN_1411ad55c(auStack_228,0x30,10,&LAB_1400ecab0);
    if (((uStack_22c & 0x40) == 0) && (puStack_238 != (undefined1 *)0x0)) {
      FUN_1409d8700(puStack_238,uStack_22c >> 8);
    }
  }
  lVar6 = *(longlong *)(lVar2 + 0x308);
  FUN_1407fcbe0(lVar6);
  FUN_1407ff080(lVar6,&DAT_1431af668);
  aiStackX_8[0] = 0;
  uVar9 = 2;
  if (*(int *)(param_1 + 0xb4) != 2) {
    uVar9 = 1;
  }
  FUN_1407ff0e0(lVar6,uVar9,0);
  FUN_1407fd830(lVar6);
  *(undefined4 *)(lVar6 + 0x38c) = 0x42c80000;
  lVar8 = *(longlong *)(lVar2 + 0x310);
  if (*(int *)(param_1 + 0xd8) == 0) {
    FUN_1407f01e0(lVar8 + 0x251c0,lVar11 + 0xb8);
    uVar9 = 6;
  }
  else {
    uVar9 = 2;
  }
  aiStackX_8[0] = -0x40800000;
  FUN_1407f43b0(lVar8,uVar9,0xbf800000);
  if (puVar5 != (undefined8 *)0x0) {
    lVar11 = (**(code **)*puVar5)(puVar5);
    if ((*(int *)(lVar11 + 0x60) < DAT_1439b8470) || (DAT_1439b8474 < *(int *)(lVar11 + 0x60))) {
      if (lVar7 != 0) {
        FUN_1407fd780(lVar6,*(undefined4 *)(lVar7 + 0xf8));
      }
    }
    else {
      FUN_1407fd740(lVar6,3);
    }
  }
  lVar7 = *(longlong *)(lVar2 + 0x328);
  if (((*(int *)(param_1 + 0xd8) == 0) || ((*(byte *)(param_1 + 0xdc) & 1) == 0)) ||
     (*(longlong *)(lVar7 + 0x70) == 0)) {
    *(byte *)(param_1 + 0x118) = *(byte *)(param_1 + 0x118) & 0xfd;
  }
  else {
    FUN_140835590(auStack_270,0,0x21,*(undefined4 *)(param_1 + 8));
    FUN_1408024d0(lVar2,0x40,auStack_270);
    *(byte *)(param_1 + 0x118) = *(byte *)(param_1 + 0x118) | 2;
  }
  if (((*(int *)(param_1 + 0xd8) == 0) || ((*(byte *)(param_1 + 0xdc) & 2) == 0)) ||
     (*(longlong *)(lVar7 + 0x68) == 0)) {
    *(byte *)(param_1 + 0x118) = *(byte *)(param_1 + 0x118) & 0xfe;
  }
  else {
    FUN_140835590(auStack_270,0,0x21,*(undefined4 *)(param_1 + 8));
    FUN_1408024d0(lVar2,0x35,auStack_270);
    *(byte *)(param_1 + 0x118) = *(byte *)(param_1 + 0x118) | 1;
  }
  return;
}


