# possession-weak-reference-decompile-v374.txt

Original: [work/movement-integration/possession-weak-reference-decompile-v374.txt](<../../../../work/movement-integration/possession-weak-reference-decompile-v374.txt>)

SHA-256: `27DD224BAC59509B90B59C3C9C9C8F39DD158B08614515A3EF7B0A5C6641F689`; 793 bytes.

## Original saved record

````text
{"status":"fulfilled","value":{"content":[{"type":"text","text":"\r\nlonglong FUN_1403da910(int *param_1)\r\n\r\n{\r\n  longlong lVar1;\r\n  char cVar2;\r\n  \r\n  if ((*param_1 != -1) && (lVar1 = *(longlong *)(param_1 + 2), *param_1 == *(int *)(lVar1 + 0x18)))\r\n  {\r\n    cVar2 = FUN_1407e7390(lVar1);\r\n    if (cVar2 != '\\0') {\r\n      return lVar1;\r\n    }\r\n  }\r\n  return 0;\r\n}\r\n\r\n"}],"structuredContent":{"result":"\r\nlonglong FUN_1403da910(int *param_1)\r\n\r\n{\r\n  longlong lVar1;\r\n  char cVar2;\r\n  \r\n  if ((*param_1 != -1) && (lVar1 = *(longlong *)(param_1 + 2), *param_1 == *(int *)(lVar1 + 0x18)))\r\n  {\r\n    cVar2 = FUN_1407e7390(lVar1);\r\n    if (cVar2 != '\\0') {\r\n      return lVar1;\r\n    }\r\n  }\r\n  return 0;\r\n}\r\n\r\n"},"isError":false}}

````
