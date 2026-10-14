# swarm-decompile-1408cf620-v267.txt

Original: [work/movement-integration/swarm-decompile-1408cf620-v267.txt](<../../../../work/movement-integration/swarm-decompile-1408cf620-v267.txt>)

SHA-256: `EAAD57850A2D711B9230778DF92880899E800B20F6A07A5C256AB465665A5AE8`; 17553 bytes.

## Original saved record

````text

undefined1 * FUN_1408cf620(longlong *param_1,undefined1 *param_2,longlong param_3,longlong param_4)

{
  byte bVar1;
  int iVar2;
  char cVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  undefined4 *puVar6;
  undefined8 *puVar7;
  byte *pbVar8;
  undefined8 uVar9;
  undefined4 local_res8 [2];
  undefined8 in_stack_fffffffffffffe88;
  undefined4 uVar10;
  undefined8 in_stack_fffffffffffffe90;
  undefined4 uVar11;
  undefined1 local_158;
  undefined1 local_157;
  undefined1 local_156;
  undefined1 local_155;
  undefined1 local_154;
  undefined1 local_153;
  undefined1 local_152;
  undefined1 local_151;
  undefined1 local_150;
  undefined1 local_14f;
  undefined1 local_14e;
  undefined1 local_14d;
  undefined1 local_14c;
  undefined1 local_14b;
  undefined1 local_14a;
  undefined1 local_149;
  undefined1 local_148;
  undefined1 local_147;
  undefined1 local_146;
  undefined1 local_145;
  undefined1 local_144;
  undefined1 local_143;
  undefined1 local_142;
  undefined1 local_141;
  undefined1 local_140;
  undefined1 local_13f;
  undefined1 local_13e;
  undefined1 local_13d;
  undefined1 local_13c;
  undefined1 local_13b;
  undefined1 local_13a;
  undefined1 local_139;
  undefined1 local_138;
  undefined1 local_137;
  undefined1 local_136;
  undefined1 local_135;
  undefined1 local_134;
  undefined1 local_133;
  undefined1 local_132;
  undefined1 local_131;
  undefined1 local_130;
  undefined1 local_12f;
  undefined1 local_12e;
  undefined1 local_12d;
  undefined1 local_12c;
  undefined1 local_12b;
  undefined1 local_12a;
  undefined1 local_129;
  undefined1 local_128;
  undefined1 local_127;
  undefined1 local_126;
  undefined1 local_125;
  undefined1 local_124;
  undefined1 local_123;
  undefined1 local_122;
  undefined1 local_121;
  undefined1 local_120;
  undefined1 local_11f;
  undefined1 local_11e;
  undefined1 local_11d;
  undefined1 local_11c;
  undefined1 local_11b;
  undefined1 local_11a;
  undefined1 local_119;
  undefined4 local_118;
  undefined4 local_114;
  undefined4 local_110;
  undefined4 local_10c;
  undefined4 local_108;
  undefined4 local_104;
  undefined4 local_100;
  undefined4 local_fc;
  undefined4 local_f8;
  undefined4 local_f4;
  undefined4 local_f0;
  undefined4 local_ec;
  undefined4 local_e8;
  undefined1 local_e4 [4];
  undefined1 local_e0 [8];
  undefined8 local_d8;
  undefined8 local_d0;
  undefined8 local_c8;
  undefined1 local_c0 [8];
  undefined1 local_b8 [8];
  undefined1 local_b0 [8];
  undefined8 local_a8;
  undefined1 local_a0 [8];
  undefined1 local_98 [8];
  undefined1 local_90 [8];
  undefined1 local_88 [12];
  undefined1 local_7c [12];
  undefined1 local_70 [40];
  undefined1 local_48 [32];
  
  uVar10 = (undefined4)((ulonglong)in_stack_fffffffffffffe88 >> 0x20);
  uVar11 = (undefined4)((ulonglong)in_stack_fffffffffffffe90 >> 0x20);
  local_a8 = 0xfffffffffffffffe;
  local_e8 = 0;
  cVar3 = (**(code **)(*param_1 + 0x30))(param_1,param_3);
  if (cVar3 == '\0') {
    switch(*(undefined4 *)(param_3 + 0x2c)) {
    case 0:
      FUN_140876fb0(param_1,&local_13d);
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    default:
      FUN_1409154b0(param_1,param_2,param_3,param_4);
      break;
    case 0x17:
      FUN_1407ac710(param_1,&local_148,*(undefined4 *)(param_4 + 8));
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0x18:
      FUN_1407ac710(param_1,&local_146,*(undefined4 *)(param_4 + 8),*(undefined4 *)(param_4 + 0x20))
      ;
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0x19:
      FUN_1407ac710(param_1,&local_144,*(undefined4 *)(param_4 + 8));
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0x1a:
      FUN_1407ac710(param_1,&local_142,*(undefined4 *)(param_4 + 8));
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0x1b:
      FUN_1407ac710(param_1,&local_140,*(undefined4 *)(param_4 + 8));
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0x92:
      FUN_1408759f0(param_1,&local_157,param_4 + 8,*(undefined4 *)(param_4 + 0x20),
                    CONCAT44(uVar10,*(undefined4 *)(param_4 + 0x38)),
                    CONCAT44(uVar11,*(undefined4 *)(param_4 + 0x50)),*(undefined4 *)(param_4 + 0x68)
                   );
    case 0x1d:
    case 0x8e:
    case 0xa7:
    case 0xa8:
    case 0xd0:
    case 0xd1:
    case 0xd2:
    case 0xd3:
    case 0xd4:
    case 0xd5:
    case 0xdc:
    case 0xdd:
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0x93:
      uVar9 = FUN_1408832b0(param_4);
      uVar9 = FUN_140876630(param_1,local_48,uVar9);
      FUN_140882f20(param_2,uVar9);
      break;
    case 0x94:
      uVar9 = FUN_1408832b0(param_4 + 0x18);
      uVar4 = FUN_1408832b0(param_4);
      FUN_140876660(param_1,&local_12d,uVar4,uVar9);
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0x95:
      uVar9 = FUN_1408832b0(param_4);
      FUN_1408769f0(param_1,&local_155,uVar9);
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0x96:
      uVar9 = FUN_1408832b0(param_4 + 0x30);
      uVar4 = FUN_1408832b0(param_4 + 0x18);
      uVar5 = FUN_1408832c0(param_4);
      FUN_1408767a0(param_1,&local_119,uVar5,uVar4,uVar9);
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0x97:
      uVar9 = FUN_1408832c0(param_4);
      FUN_1407ac710(param_1,&local_153,uVar9);
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0x98:
      uVar9 = FUN_1408832c0(param_4);
      FUN_1407ac710(param_1,&local_11c,uVar9);
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0x99:
      uVar9 = FUN_1408832c0(param_4);
      FUN_1407ac710(param_1,&local_151,uVar9);
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0x9a:
      uVar9 = FUN_1408832c0(param_4);
      FUN_1407ac710(param_1,&local_12a,uVar9);
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0x9b:
      uVar9 = FUN_1408832c0(param_4);
      FUN_1407ac710(param_1,&local_14f,uVar9);
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0x9c:
      FUN_1407ac710(param_1,&local_11d);
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0x9d:
      FUN_1407ac710(param_1,&local_14d);
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0x9e:
      FUN_1407ac710(param_1,&local_128,param_4 + 8);
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0x9f:
      FUN_1409fbf50(local_res8,*(byte *)((longlong)param_1 + 0xd1) >> 5 & 1);
      *param_2 = 0x69;
      *(uint *)(param_2 + 8) = (uint)(byte)local_res8[0];
      break;
    case 0xa0:
      uVar9 = FUN_1408832c0(param_4);
      FUN_140877430(param_1,&local_14b,uVar9);
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0xa1:
      pbVar8 = (byte *)FUN_140876bf0(param_1,&local_11e);
      bVar1 = *pbVar8;
      *param_2 = 0x69;
      *(uint *)(param_2 + 8) = (uint)bVar1;
      break;
    case 0xa2:
      FUN_1408756a0(param_1,&local_149,param_4 + 8);
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0xa3:
      FUN_140875b30(param_1,&local_126,param_4 + 8);
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0xa4:
      FUN_140876fb0(param_1,&local_147,param_4 + 8);
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0xa5:
      FUN_1408759c0(param_1,&local_11a,param_4 + 8,*(undefined4 *)(param_4 + 0x20));
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0xa6:
      FUN_140875460(param_1,&local_145,param_4 + 8,*(undefined4 *)(param_4 + 0x20));
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0xa9:
      FUN_140875760(param_1,&local_124,param_4 + 8,*(undefined4 *)(param_4 + 0x20));
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0xaa:
      FUN_14013f9d0(local_70,*(undefined8 *)(param_4 + 0x20));
      FUN_140875ba0(param_1,&local_143,param_4 + 8,local_70,*(undefined8 *)(param_4 + 0x38));
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      FUN_14013fd10(local_70);
      break;
    case 0xab:
      local_d8 = *(undefined8 *)(param_4 + 0x50);
      local_c8 = *(undefined8 *)(param_4 + 0x38);
      FUN_1408755c0(param_1,&local_120,param_4 + 8,*(undefined8 *)(param_4 + 0x20),&local_c8,
                    &local_d8,*(int *)(param_4 + 0x68) != 0);
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0xac:
      FUN_140875b50(param_1,&local_141,param_4 + 8,*(undefined4 *)(param_4 + 0x20));
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0xad:
      uVar11 = *(undefined4 *)(param_4 + 0x38);
      FUN_140131f90(local_e0,*(undefined8 *)(param_4 + 0x20),0);
      FUN_140875540(param_1,&local_122,param_4 + 8,local_e0,CONCAT44(uVar10,uVar11));
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0xae:
      FUN_140875940(param_1,&local_13f,param_4 + 8);
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0xaf:
      FUN_140875900(param_1,&local_11f,param_4 + 8);
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0xb0:
      FUN_140876fd0(param_1,&local_12c);
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0xb1:
      local_res8[0] = *(undefined4 *)(param_4 + 8);
      FUN_140877140(param_1,&local_13b,local_res8[0],*(undefined4 *)(param_4 + 0x20));
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0xb2:
      FUN_140876ff0(param_1,&local_12b);
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0xb3:
      puVar7 = (undefined8 *)FUN_140876830(param_1,local_90,*(undefined8 *)(param_4 + 8));
      FUN_140882fa0(param_2,*puVar7);
      break;
    case 0xb4:
      puVar7 = (undefined8 *)FUN_140876940(param_1,local_98);
      uVar9 = *puVar7;
      *param_2 = 0x73;
      *(undefined8 *)(param_2 + 8) = uVar9;
      break;
    case 0xb5:
      FUN_140877220(param_1,&local_139,*(undefined8 *)(param_4 + 8));
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0xb6:
      uVar9 = FUN_1408832c0(param_4);
      FUN_140876430(param_1,&local_129,uVar9);
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0xb7:
      uVar9 = FUN_1408832c0(param_4);
      FUN_140876450(param_1,&local_137,uVar9);
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0xb8:
      FUN_140876d40(param_1,&local_127,*(int *)(param_4 + 8) != 0);
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0xb9:
      puVar6 = (undefined4 *)FUN_140876d80(param_1,local_e4);
      uVar10 = *puVar6;
      *param_2 = 0x69;
      *(undefined4 *)(param_2 + 8) = uVar10;
      break;
    case 0xba:
      puVar7 = (undefined8 *)FUN_140876970(param_1,local_b8,*(undefined4 *)(param_4 + 8));
      FUN_140882fa0(param_2,*puVar7);
      break;
    case 0xbb:
      uVar9 = FUN_1408832c0(param_4);
      pbVar8 = (byte *)FUN_140877050(param_1,&local_135,uVar9);
      bVar1 = *pbVar8;
      *param_2 = 0x69;
      *(uint *)(param_2 + 8) = (uint)bVar1;
      break;
    case 0xbc:
      uVar9 = FUN_1408832c0(param_4);
      FUN_140876470(param_1,&local_125,uVar9);
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0xbd:
      puVar7 = (undefined8 *)FUN_140876e30(param_1,local_b0,*(undefined8 *)(param_4 + 8));
      FUN_140882fa0(param_2,*puVar7);
      break;
    case 0xbe:
      FUN_140877200(param_1,&local_133,*(undefined8 *)(param_4 + 8));
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0xbf:
      puVar7 = (undefined8 *)FUN_140876890(param_1,local_a0);
      uVar9 = *puVar7;
      *param_2 = 0x73;
      *(undefined8 *)(param_2 + 8) = uVar9;
      break;
    case 0xc0:
      FUN_1408772d0(param_1,&local_123,*(undefined4 *)(param_4 + 8),*(undefined4 *)(param_4 + 0x20),
                    CONCAT44(uVar10,*(undefined4 *)(param_4 + 0x38)),
                    CONCAT44(uVar11,*(undefined4 *)(param_4 + 0x50)));
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0xc1:
      pbVar8 = (byte *)FUN_140876bc0(param_1,&local_131);
      bVar1 = *pbVar8;
      *param_2 = 0x69;
      *(uint *)(param_2 + 8) = (uint)bVar1;
      break;
    case 0xc2:
      FUN_1408769d0(param_1,&local_121);
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0xc3:
      FUN_140877350(param_1,&local_12f);
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0xc4:
      FUN_1408756a0(param_1,&local_11b);
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0xc5:
      FUN_140875b30(param_1,&local_158);
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0xc6:
      local_100 = *(undefined4 *)(param_4 + 8);
      local_fc = *(undefined4 *)(param_4 + 0xc);
      local_f8 = *(undefined4 *)(param_4 + 0x10);
      puVar6 = (undefined4 *)FUN_140876c60(param_1,local_88,&local_100);
      *param_2 = 0x76;
      *(undefined4 *)(param_2 + 8) = *puVar6;
      *(undefined4 *)(param_2 + 0xc) = puVar6[1];
      *(undefined4 *)(param_2 + 0x10) = puVar6[2];
      break;
    case 199:
      puVar6 = (undefined4 *)FUN_1408768e0(param_1,local_7c);
      *param_2 = 0x76;
      *(undefined4 *)(param_2 + 8) = *puVar6;
      *(undefined4 *)(param_2 + 0xc) = puVar6[1];
      *(undefined4 *)(param_2 + 0x10) = puVar6[2];
      break;
    case 200:
      local_10c = *(undefined4 *)(param_4 + 8);
      local_108 = *(undefined4 *)(param_4 + 0xc);
      local_104 = *(undefined4 *)(param_4 + 0x10);
      FUN_140877330(param_1,&local_156,&local_10c);
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0xc9:
      local_f4 = *(undefined4 *)(param_4 + 0x20);
      local_f0 = *(undefined4 *)(param_4 + 0x24);
      local_ec = *(undefined4 *)(param_4 + 0x28);
      local_118 = *(undefined4 *)(param_4 + 8);
      local_114 = *(undefined4 *)(param_4 + 0xc);
      local_110 = *(undefined4 *)(param_4 + 0x10);
      FUN_1408773d0(param_1,&local_154,&local_118,&local_f4);
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0xca:
      FUN_140877370(param_1,&local_152);
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0xcb:
      puVar7 = (undefined8 *)FUN_140876860(param_1,local_c0);
      uVar9 = *puVar7;
      *param_2 = 0x73;
      *(undefined8 *)(param_2 + 8) = uVar9;
      break;
    case 0xcc:
      pbVar8 = (byte *)FUN_140876b10(param_1,&local_150,*(undefined8 *)(param_4 + 8));
      bVar1 = *pbVar8;
      *param_2 = 0x69;
      *(uint *)(param_2 + 8) = (uint)bVar1;
      break;
    case 0xcd:
      pbVar8 = (byte *)FUN_140876c20(param_1,&local_14e,*(undefined8 *)(param_4 + 8));
      bVar1 = *pbVar8;
      *param_2 = 0x69;
      *(uint *)(param_2 + 8) = (uint)bVar1;
      break;
    case 0xce:
      FUN_140877240(param_1,&local_14c,*(undefined8 *)(param_4 + 8),*(undefined4 *)(param_4 + 0x20),
                    CONCAT44(uVar10,*(undefined4 *)(param_4 + 0x38)),
                    CONCAT44(uVar11,*(undefined4 *)(param_4 + 0x50)),*(undefined4 *)(param_4 + 0x68)
                   );
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0xcf:
      local_d0 = *(undefined8 *)(param_4 + 0x50);
      FUN_1408756c0(param_1,&local_14a,param_4 + 8,*(undefined4 *)(param_4 + 0x20),
                    CONCAT44(uVar10,*(undefined4 *)(param_4 + 0x38)),&local_d0);
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0xd6:
      uVar9 = FUN_1408832c0(param_4);
      FUN_1407ac710(param_1,&local_13e,uVar9);
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0xd7:
      uVar9 = FUN_1408832c0(param_4);
      FUN_1407ac710(param_1,&local_13c,uVar9);
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0xd8:
      uVar9 = FUN_1408832c0(param_4);
      FUN_1407ac710(param_1,&local_13a,uVar9);
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0xd9:
      FUN_140876df0(param_1,&local_138);
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0xda:
      FUN_140876db0(param_1,&local_136);
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0xdb:
      iVar2 = *(int *)(param_4 + 8);
      uVar9 = FUN_1408832c0(param_4 + 0x18);
      FUN_1407ac710(param_1,&local_134,iVar2 != 0,uVar9);
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0xde:
      FUN_1407ac710(param_1,&local_132);
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0xdf:
      FUN_1407ac710(param_1,&local_130);
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
      break;
    case 0xe0:
      FUN_1407ac710(param_1,&local_12e);
      *param_2 = 0;
      *(undefined8 *)(param_2 + 8) = 0;
    }
  }
  else {
    (**(code **)(*param_1 + 0x28))(param_1,param_2,param_3,param_4);
  }
  return param_2;
}



````
