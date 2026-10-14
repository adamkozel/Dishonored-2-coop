# possession-base-methods-decompile-v320.txt

Original: [work/movement-integration/possession-base-methods-decompile-v320.txt](<../../../../work/movement-integration/possession-base-methods-decompile-v320.txt>)

SHA-256: `6AFA34F0521081F59344A92A00F003D50D12527816C779A8C9A3DAD05C978E07`; 1636 bytes.

## Original saved record

````text
{"0x1408eb670":"Error: Function not found","0x140447b70":"\r\nundefined8 FUN_140447b70(undefined8 param_1,uint param_2)\r\n\r\n{\r\n  FUN_14061a120();\r\n  if ((param_2 & 1) != 0) {\r\n    FUN_1400f1520(param_1);\r\n  }\r\n  return param_1;\r\n}\r\n\r\n","0x14061c1f0":"\r\nvoid FUN_14061c1f0(longlong param_1)\r\n\r\n{\r\n  undefined4 *puVar1;\r\n  undefined1 local_res8 [32];\r\n  \r\n  FUN_14061aed0();\r\n  puVar1 = (undefined4 *)FUN_14060ce50(*(undefined8 *)(param_1 + 0x30),local_res8,&DAT_143987d58);\r\n  if ((undefined4 *)(param_1 + 0x68) != puVar1) {\r\n    *(undefined4 *)(param_1 + 0x68) = *puVar1;\r\n  }\r\n  puVar1 = (undefined4 *)FUN_14060ce50(*(undefined8 *)(param_1 + 0x30),local_res8,&DAT_143987d60);\r\n  if ((undefined4 *)(param_1 + 0x98) != puVar1) {\r\n    *(undefined4 *)(param_1 + 0x98) = *puVar1;\r\n  }\r\n  puVar1 = (undefined4 *)FUN_14060ce50(*(undefined8 *)(param_1 + 0x30),local_res8,&DAT_143987d68);\r\n  if ((undefined4 *)(param_1 + 0x9c) != puVar1) {\r\n    *(undefined4 *)(param_1 + 0x9c) = *puVar1;\r\n  }\r\n  puVar1 = (undefined4 *)FUN_14060ce50(*(undefined8 *)(param_1 + 0x30),local_res8,&DAT_143987d70);\r\n  if ((undefined4 *)(param_1 + 0xa0) != puVar1) {\r\n    *(undefined4 *)(param_1 + 0xa0) = *puVar1;\r\n  }\r\n  return;\r\n}\r\n\r\n","0x14061c2a0":"\r\nvoid FUN_14061c2a0(longlong *param_1)\r\n\r\n{\r\n  FUN_14061b010();\r\n  if (*(char *)((longlong)param_1 + 0xa4) != '\\0') {\r\n    (**(code **)(*param_1 + 0x128))(param_1);\r\n  }\r\n  *(undefined4 *)(param_1 + 0xd) = 0xffffffff;\r\n  param_1[0x13] = -1;\r\n  *(undefined4 *)(param_1 + 0x14) = 0xffffffff;\r\n  return;\r\n}\r\n\r\n"}

````
