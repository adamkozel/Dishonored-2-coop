# native-drop-high-decompilation-v1.json

Original: [work/natural-moveset/carry-research/native-drop-high-decompilation-v1.json](<../../../../work/natural-moveset/carry-research/native-drop-high-decompilation-v1.json>)

SHA-256: `7EA87D2DD3A40A24D1D9D5B0B79D60DEDD4B827593CF4DF4D28E18786BF3536F`; 5357 bytes.

## Original saved record

````json
{
  "content": [
    {
      "type": "text",
      "text": "\r\nvoid FUN_140832770(longlong param_1)\r\n\r\n{\r\n  int iVar1;\r\n  uint uVar2;\r\n  longlong lVar3;\r\n  longlong lVar4;\r\n  undefined8 uVar5;\r\n  uint uVar6;\r\n  undefined4 local_res8 [2];\r\n  undefined1 local_38 [8];\r\n  uint local_30;\r\n  undefined1 local_20 [24];\r\n  \r\n  uVar5 = 0;\r\n  lVar4 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x90);\r\n  if (lVar4 != 0) {\r\n    uVar5 = *(undefined8 *)(lVar4 + 0x178);\r\n  }\r\n  FUN_140829e30(uVar5);\r\n  iVar1 = FUN_1403a6f10(*(undefined8 *)(param_1 + 0x30),*(undefined1 *)(param_1 + 0x50));\r\n  lVar4 = *(longlong *)(param_1 + 0x18);\r\n  if (iVar1 == 0) {\r\n    lVar4 = *(longlong *)(lVar4 + 0x70);\r\n  }\r\n  else if (iVar1 == 1) {\r\n    lVar4 = *(longlong *)(lVar4 + 0x78);\r\n  }\r\n  else if (iVar1 == 2) {\r\n    lVar4 = *(longlong *)(lVar4 + 0x80);\r\n  }\r\n  else if (iVar1 == 3) {\r\n    uVar6 = *(uint *)(lVar4 + 0x90) & 0xffffff;\r\n    if (uVar6 == 0) goto LAB_1408328d4;\r\n    uVar2 = *(int *)(D2_GameSingletonPtr + 0x600f8) * 0x19660d + 0x3c6ef35f;\r\n    *(uint *)(D2_GameSingletonPtr + 0x600f8) = uVar2;\r\n    lVar4 = *(longlong *)\r\n             (*(longlong *)(lVar4 + 0x88) +\r\n             (longlong)(int)((ulonglong)(uVar2 >> 10 & 0x7fff) % (ulonglong)(longlong)(int)uVar6) *\r\n             8);\r\n  }\r\n  else {\r\n    lVar4 = *(longlong *)(lVar4 + 0x98);\r\n  }\r\n  if (lVar4 != 0) {\r\n    lVar3 = FUN_1406dc560(*(undefined4 *)(param_1 + 0x38));\r\n    if (lVar3 != 0) {\r\n      local_res8[0] = 0;\r\n      FUN_140854ab0(param_1,local_res8,lVar4,0x25,lVar3,0);\r\n      local_30 = (uint)*(byte *)(param_1 + 0x50);\r\n      *(int *)(param_1 + 0x54) = iVar1;\r\n      *(undefined4 *)(param_1 + 0x58) = 0;\r\n      local_38[0] = 0x69;\r\n      FUN_140884920(lVar3,local_20,&DAT_1439ac250,local_38);\r\n      FUN_140882fc0(local_20);\r\n      *(byte *)(lVar3 + 0x368) = *(byte *)(lVar3 + 0x368) & 0xfb;\r\n      *(byte *)(lVar3 + 0x368) = *(byte *)(lVar3 + 0x368) | (iVar1 == 4) << 2;\r\n    }\r\n    lVar4 = FUN_1403da910(param_1 + 0x40,2);\r\n    if (lVar4 != 0) {\r\n      lVar3 = FUN_140854880(param_1);\r\n      local_res8[0] = *(undefined4 *)(lVar3 + 0xa8);\r\n      FUN_1407278e0(lVar4,local_res8);\r\n      *(byte *)(lVar4 + 0x241) = *(byte *)(lVar4 + 0x241) | 0x20;\r\n      FUN_140735830(lVar4);\r\n      if (iVar1 == 2) {\r\n        *(byte *)(lVar4 + 0x241) = *(byte *)(lVar4 + 0x241) | 2;\r\n      }\r\n    }\r\n    return;\r\n  }\r\nLAB_1408328d4:\r\n                    /* WARNING: Subroutine does not return */\r\n  FUN_140166830(\"No midnight to start for the corpse drop\");\r\n}\r\n\r\n"
    }
  ],
  "structuredContent": {
    "result": "\r\nvoid FUN_140832770(longlong param_1)\r\n\r\n{\r\n  int iVar1;\r\n  uint uVar2;\r\n  longlong lVar3;\r\n  longlong lVar4;\r\n  undefined8 uVar5;\r\n  uint uVar6;\r\n  undefined4 local_res8 [2];\r\n  undefined1 local_38 [8];\r\n  uint local_30;\r\n  undefined1 local_20 [24];\r\n  \r\n  uVar5 = 0;\r\n  lVar4 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x90);\r\n  if (lVar4 != 0) {\r\n    uVar5 = *(undefined8 *)(lVar4 + 0x178);\r\n  }\r\n  FUN_140829e30(uVar5);\r\n  iVar1 = FUN_1403a6f10(*(undefined8 *)(param_1 + 0x30),*(undefined1 *)(param_1 + 0x50));\r\n  lVar4 = *(longlong *)(param_1 + 0x18);\r\n  if (iVar1 == 0) {\r\n    lVar4 = *(longlong *)(lVar4 + 0x70);\r\n  }\r\n  else if (iVar1 == 1) {\r\n    lVar4 = *(longlong *)(lVar4 + 0x78);\r\n  }\r\n  else if (iVar1 == 2) {\r\n    lVar4 = *(longlong *)(lVar4 + 0x80);\r\n  }\r\n  else if (iVar1 == 3) {\r\n    uVar6 = *(uint *)(lVar4 + 0x90) & 0xffffff;\r\n    if (uVar6 == 0) goto LAB_1408328d4;\r\n    uVar2 = *(int *)(D2_GameSingletonPtr + 0x600f8) * 0x19660d + 0x3c6ef35f;\r\n    *(uint *)(D2_GameSingletonPtr + 0x600f8) = uVar2;\r\n    lVar4 = *(longlong *)\r\n             (*(longlong *)(lVar4 + 0x88) +\r\n             (longlong)(int)((ulonglong)(uVar2 >> 10 & 0x7fff) % (ulonglong)(longlong)(int)uVar6) *\r\n             8);\r\n  }\r\n  else {\r\n    lVar4 = *(longlong *)(lVar4 + 0x98);\r\n  }\r\n  if (lVar4 != 0) {\r\n    lVar3 = FUN_1406dc560(*(undefined4 *)(param_1 + 0x38));\r\n    if (lVar3 != 0) {\r\n      local_res8[0] = 0;\r\n      FUN_140854ab0(param_1,local_res8,lVar4,0x25,lVar3,0);\r\n      local_30 = (uint)*(byte *)(param_1 + 0x50);\r\n      *(int *)(param_1 + 0x54) = iVar1;\r\n      *(undefined4 *)(param_1 + 0x58) = 0;\r\n      local_38[0] = 0x69;\r\n      FUN_140884920(lVar3,local_20,&DAT_1439ac250,local_38);\r\n      FUN_140882fc0(local_20);\r\n      *(byte *)(lVar3 + 0x368) = *(byte *)(lVar3 + 0x368) & 0xfb;\r\n      *(byte *)(lVar3 + 0x368) = *(byte *)(lVar3 + 0x368) | (iVar1 == 4) << 2;\r\n    }\r\n    lVar4 = FUN_1403da910(param_1 + 0x40,2);\r\n    if (lVar4 != 0) {\r\n      lVar3 = FUN_140854880(param_1);\r\n      local_res8[0] = *(undefined4 *)(lVar3 + 0xa8);\r\n      FUN_1407278e0(lVar4,local_res8);\r\n      *(byte *)(lVar4 + 0x241) = *(byte *)(lVar4 + 0x241) | 0x20;\r\n      FUN_140735830(lVar4);\r\n      if (iVar1 == 2) {\r\n        *(byte *)(lVar4 + 0x241) = *(byte *)(lVar4 + 0x241) | 2;\r\n      }\r\n    }\r\n    return;\r\n  }\r\nLAB_1408328d4:\r\n                    /* WARNING: Subroutine does not return */\r\n  FUN_140166830(\"No midnight to start for the corpse drop\");\r\n}\r\n\r\n"
  },
  "isError": false
}

````

## Unescaped text fields for reading

### text

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
