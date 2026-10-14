# swarm-decompile-self-binding-v306.txt

Original: [work/movement-integration/swarm-decompile-self-binding-v306.txt](<../../../../work/movement-integration/swarm-decompile-self-binding-v306.txt>)

SHA-256: `D68CECCCB87783E64C42EB022DA4498F4D6C5EF102B30D365EC583402E13C3E3`; 1892 bytes.

## Original saved record

````text
{"140751140":"\r\nulonglong FUN_140751140(longlong param_1)\r\n\r\n{\r\n  ulonglong in_RAX;\r\n  float *pfVar1;\r\n  longlong lVar2;\r\n  uint uVar3;\r\n  \r\n  if (*(char *)(param_1 + 0x248) != '\\0') {\r\n    lVar2 = 0;\r\n    uVar3 = *(uint *)(param_1 + 0xf0) & 0xffffff;\r\n    if (uVar3 != 0) {\r\n      pfVar1 = (float *)(*(longlong *)(param_1 + 0xe8) + 0x14);\r\n      do {\r\n        in_RAX = (ulonglong)(uint)*pfVar1;\r\n        if (0.0 <= *pfVar1) goto LAB_1407511e0;\r\n        lVar2 = lVar2 + 1;\r\n        pfVar1 = pfVar1 + 8;\r\n      } while (lVar2 < (longlong)(ulonglong)uVar3);\r\n    }\r\n  }\r\n  if (*(char *)(param_1 + 0x3b8) != '\\0') {\r\n    lVar2 = 0;\r\n    uVar3 = *(uint *)(param_1 + 0x260) & 0xffffff;\r\n    if (uVar3 != 0) {\r\n      pfVar1 = (float *)(*(longlong *)(param_1 + 600) + 0x14);\r\n      do {\r\n        in_RAX = (ulonglong)(uint)*pfVar1;\r\n        if (0.0 <= *pfVar1) {\r\nLAB_1407511e0:\r\n          return CONCAT71((int7)(in_RAX >> 8),1);\r\n        }\r\n        lVar2 = lVar2 + 1;\r\n        pfVar1 = pfVar1 + 8;\r\n      } while (lVar2 < (longlong)(ulonglong)uVar3);\r\n    }\r\n  }\r\n  return in_RAX & 0xffffffffffffff00;\r\n}\r\n\r\n","140752260":"\r\nvoid FUN_140752260(longlong param_1)\r\n\r\n{\r\n  int iVar1;\r\n  char cVar2;\r\n  longlong lVar3;\r\n  \r\n  iVar1 = *(int *)(D2_GameSingletonPtr + 0x600f0);\r\n  if (iVar1 <= *(int *)(param_1 + 0x418)) {\r\n    return;\r\n  }\r\n  lVar3 = 0;\r\n  if (*(longlong *)(param_1 + 0x3c8) != 0) {\r\n    lVar3 = *(longlong *)(*(longlong *)(param_1 + 0x3c8) + 0x358);\r\n  }\r\n  if (lVar3 != 0) {\r\n    if (iVar1 <= *(int *)(lVar3 + 0x418)) {\r\n      return;\r\n    }\r\n    cVar2 = FUN_140751980(lVar3,iVar1);\r\n    if (cVar2 != '\\0') {\r\n      return;\r\n    }\r\n  }\r\n  *(int *)(param_1 + 0x418) = iVar1;\r\n  FUN_1408a60e0(D2_GameSingletonPtr,param_1);\r\n  return;\r\n}\r\n\r\n"}

````
