# possession-target-helpers-decompile-v324.txt

Original: [work/movement-integration/possession-target-helpers-decompile-v324.txt](<../../../../work/movement-integration/possession-target-helpers-decompile-v324.txt>)

SHA-256: `6E413B402AF5591F3389F9C8F62048F5F8BA2C68F52363F4C0677DC5DD821752`; 595 bytes.

## Original saved record

````text
{"0x1404481f0":"\r\nvoid thunk_FUN_14061c1a0(void)\r\n\r\n{\r\n  FUN_14061c1a0();\r\n  return;\r\n}\r\n\r\n","0x1407e7390":"\r\nundefined8 FUN_1407e7390(longlong param_1,int param_2)\r\n\r\n{\r\n  longlong lVar1;\r\n  uint uVar2;\r\n  \r\n  uVar2 = *(uint *)(param_1 + 0x1c);\r\n  lVar1 = (longlong)param_2 * 8;\r\n  uVar2 = (uVar2 >> 2 & 3 | (uVar2 & 3) << 2) << 2 | uVar2 >> 4 & 3;\r\n  if (((*(uint *)(&UNK_141c70e88 + lVar1) & uVar2) == *(uint *)(&UNK_141c70e88 + lVar1)) &&\r\n     ((*(uint *)(&UNK_141c70e8c + lVar1) & uVar2) == 0)) {\r\n    return 1;\r\n  }\r\n  return 0;\r\n}\r\n\r\n"}

````
