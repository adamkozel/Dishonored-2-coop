# native-doppelganger-14043e0c0-decompile-v404.txt

Original: [work/movement-integration/native-doppelganger-14043e0c0-decompile-v404.txt](<../../../../work/movement-integration/native-doppelganger-14043e0c0-decompile-v404.txt>)

SHA-256: `AC9ECA050DA84A86ED5BB33DC650E399DC62882968409CEBC47393BF2670ED99`; 2839 bytes.

## Original saved record

````text

void FUN_14043e0c0(longlong param_1,longlong *param_2)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  longlong lVar5;
  longlong lVar6;
  undefined4 *puVar7;
  longlong lVar8;
  longlong lVar9;
  undefined8 *puVar10;
  undefined8 uVar11;
  undefined8 uStack_58;
  undefined4 uStack_50;
  undefined4 uStack_4c;
  undefined8 uStack_48;
  undefined4 uStack_40;
  undefined4 uStack_3c;
  undefined4 uStack_38;
  undefined4 uStack_34;
  undefined4 uStack_30;
  undefined4 uStack_2c;
  undefined4 uStack_28;
  undefined4 uStack_24;
  undefined4 uStack_20;
  
  lVar6 = *param_2;
  uVar1 = *(undefined4 *)(lVar6 + 0x300);
  uVar2 = *(undefined4 *)(lVar6 + 0x304);
  uVar3 = *(undefined4 *)(lVar6 + 0x308);
  uVar4 = *(undefined4 *)(lVar6 + 0xa8);
  lVar6 = FUN_140810240();
  puVar7 = (undefined4 *)FUN_1408112c0(lVar6 + 0xc8e8,0x73,0x10,uVar4);
  if (puVar7 != (undefined4 *)0x0) {
    *puVar7 = 5;
    puVar7[1] = uVar1;
    puVar7[2] = uVar2;
    puVar7[3] = uVar3;
  }
  lVar6 = FUN_14043de90(param_1);
  if (lVar6 == 0) {
    lVar6 = FUN_140877720(*param_2,&DAT_1439e27c0,0);
    if (lVar6 != 0) {
      FUN_14087cb10(*param_2);
    }
  }
  else {
    lVar8 = FUN_140877720(lVar6,&DAT_1439ae070,2);
    if (lVar8 != 0) {
      lVar8 = **(longlong **)(lVar8 + 0x6d8);
      if (lVar8 == 0) {
        uVar11 = D2_InternedName_Resolve(lVar6 + 0x20);
                    /* WARNING: Subroutine does not return */
        FUN_140166830("arkCpntPowerDoppelganger::OnCloneSpawned: No clone entity def for %s",uVar11)
        ;
      }
      lVar9 = FUN_1406dc560(*(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0xac));
      lVar5 = *(longlong *)(param_1 + 200);
      uStack_40 = *(undefined4 *)(lVar9 + 0x30c);
      uStack_3c = *(undefined4 *)(lVar9 + 0x310);
      uStack_38 = *(undefined4 *)(lVar9 + 0x314);
      uStack_34 = *(undefined4 *)(lVar9 + 0x318);
      uStack_30 = *(undefined4 *)(lVar9 + 0x31c);
      uStack_2c = *(undefined4 *)(lVar9 + 800);
      uStack_28 = *(undefined4 *)(lVar9 + 0x324);
      uStack_24 = *(undefined4 *)(lVar9 + 0x328);
      uStack_20 = *(undefined4 *)(lVar9 + 0x32c);
      FUN_14010fae0(&uStack_40,&DAT_14228a940,0x40490fdb);
      lVar8 = FUN_1408ae9b0(D2_GameSingletonPtr,lVar8,lVar5 + 0x94,&uStack_40,lVar6,0,0,0,0);
      FUN_1408847f0(*param_2,&uStack_58,&DAT_1439a6f30);
      FUN_140882fc0(&uStack_58);
      *param_2 = lVar8;
      uStack_58 = DAT_14397ebc8;
      uStack_50 = 0x5f;
      FUN_140131ae0(&uStack_48);
      uVar1 = *(undefined4 *)(lVar6 + 0xa8);
      lVar6 = FUN_140810240();
      puVar10 = (undefined8 *)FUN_1408112c0(lVar6 + 0xc8e8,0x4f,0x18,uVar1);
      if (puVar10 != (undefined8 *)0x0) {
        *puVar10 = uStack_58;
        puVar10[1] = CONCAT44(uStack_4c,uStack_50);
        puVar10[2] = uStack_48;
      }
    }
  }
  return;
}



````
