# ghidra-inventory-clamp-v809.json

Original: [work/natural-moveset/native-pickup-research-v785/ghidra-inventory-clamp-v809.json](<../../../../work/natural-moveset/native-pickup-research-v785/ghidra-inventory-clamp-v809.json>)

SHA-256: `FEE3E688D7E778F5A8BB61F093C7A79704EDF375C5A5C4D6BE121C431EA1C019`; 14430 bytes.

## Original saved record

````json
{
  "disassembly": {
    "content": [
      {
        "type": "text",
        "text": "{\"success\":true,\"start_address\":\"14063c9f0\",\"end_address\":\"14063ca8f\",\"bytes_disassembled\":160,\"message\":\"Successfully disassembled 160 byte(s)\",\"instructions\":[{\"address\":\"14063c9f0\",\"mnemonic\":\"MOV\",\"operands\":\"EAX, dword ptr [RCX + 0x20]\",\"length\":3,\"bytes\":\"8b4120\"},{\"address\":\"14063c9f3\",\"mnemonic\":\"MOV\",\"operands\":\"R8D, dword ptr [RCX + 0x24]\",\"length\":4,\"bytes\":\"448b4124\"},{\"address\":\"14063c9f7\",\"mnemonic\":\"LEA\",\"operands\":\"R9D, [RAX + RDX*0x1]\",\"length\":4,\"bytes\":\"448d0c10\"},{\"address\":\"14063c9fb\",\"mnemonic\":\"CMP\",\"operands\":\"R9D, R8D\",\"length\":3,\"bytes\":\"453bc8\"},{\"address\":\"14063c9fe\",\"mnemonic\":\"JG\",\"operands\":\"0x14063ca06\",\"length\":2,\"bytes\":\"7f06\"},{\"address\":\"14063ca00\",\"mnemonic\":\"MOV\",\"operands\":\"dword ptr [RCX + 0x20], R9D\",\"length\":4,\"bytes\":\"44894920\"},{\"address\":\"14063ca04\",\"mnemonic\":\"JMP\",\"operands\":\"0x14063ca0f\",\"length\":2,\"bytes\":\"eb09\"},{\"address\":\"14063ca06\",\"mnemonic\":\"MOV\",\"operands\":\"EDX, R8D\",\"length\":3,\"bytes\":\"418bd0\"},{\"address\":\"14063ca09\",\"mnemonic\":\"MOV\",\"operands\":\"dword ptr [RCX + 0x20], R8D\",\"length\":4,\"bytes\":\"44894120\"},{\"address\":\"14063ca0d\",\"mnemonic\":\"SUB\",\"operands\":\"EDX, EAX\",\"length\":2,\"bytes\":\"2bd0\"},{\"address\":\"14063ca0f\",\"mnemonic\":\"TEST\",\"operands\":\"EAX, EAX\",\"length\":2,\"bytes\":\"85c0\"},{\"address\":\"14063ca11\",\"mnemonic\":\"JNZ\",\"operands\":\"0x14063ca1e\",\"length\":2,\"bytes\":\"750b\"},{\"address\":\"14063ca13\",\"mnemonic\":\"TEST\",\"operands\":\"EDX, EDX\",\"length\":2,\"bytes\":\"85d2\"},{\"address\":\"14063ca15\",\"mnemonic\":\"JLE\",\"operands\":\"0x14063ca1e\",\"length\":2,\"bytes\":\"7e07\"},{\"address\":\"14063ca17\",\"mnemonic\":\"MOV\",\"operands\":\"byte ptr [RCX + 0x28], 0x1\",\"length\":4,\"bytes\":\"c6412801\"},{\"address\":\"14063ca1b\",\"mnemonic\":\"MOV\",\"operands\":\"EAX, EDX\",\"length\":2,\"bytes\":\"8bc2\"},{\"address\":\"14063ca1d\",\"mnemonic\":\"RET\",\"operands\":\"\",\"length\":1,\"bytes\":\"c3\"},{\"address\":\"14063ca1e\",\"mnemonic\":\"MOV\",\"operands\":\"byte ptr [RCX + 0x28], 0x0\",\"length\":4,\"bytes\":\"c6412800\"},{\"address\":\"14063ca22\",\"mnemonic\":\"MOV\",\"operands\":\"EAX, EDX\",\"length\":2,\"bytes\":\"8bc2\"},{\"address\":\"14063ca24\",\"mnemonic\":\"RET\",\"operands\":\"\",\"length\":1,\"bytes\":\"c3\"},{\"address\":\"14063ca25\",\"mnemonic\":\"INT3\",\"operands\":\"\",\"length\":1,\"bytes\":\"cc\"},{\"address\":\"14063ca26\",\"mnemonic\":\"INT3\",\"operands\":\"\",\"length\":1,\"bytes\":\"cc\"},{\"address\":\"14063ca27\",\"mnemonic\":\"INT3\",\"operands\":\"\",\"length\":1,\"bytes\":\"cc\"},{\"address\":\"14063ca28\",\"mnemonic\":\"INT3\",\"operands\":\"\",\"length\":1,\"bytes\":\"cc\"},{\"address\":\"14063ca29\",\"mnemonic\":\"INT3\",\"operands\":\"\",\"length\":1,\"bytes\":\"cc\"},{\"address\":\"14063ca2a\",\"mnemonic\":\"INT3\",\"operands\":\"\",\"length\":1,\"bytes\":\"cc\"},{\"address\":\"14063ca2b\",\"mnemonic\":\"INT3\",\"operands\":\"\",\"length\":1,\"bytes\":\"cc\"},{\"address\":\"14063ca2c\",\"mnemonic\":\"INT3\",\"operands\":\"\",\"length\":1,\"bytes\":\"cc\"},{\"address\":\"14063ca2d\",\"mnemonic\":\"INT3\",\"operands\":\"\",\"length\":1,\"bytes\":\"cc\"},{\"address\":\"14063ca2e\",\"mnemonic\":\"INT3\",\"operands\":\"\",\"length\":1,\"bytes\":\"cc\"},{\"address\":\"14063ca2f\",\"mnemonic\":\"INT3\",\"operands\":\"\",\"length\":1,\"bytes\":\"cc\"},{\"address\":\"14063ca30\",\"mnemonic\":\"PUSH\",\"operands\":\"RBX\",\"length\":2,\"bytes\":\"4053\"},{\"address\":\"14063ca32\",\"mnemonic\":\"SUB\",\"operands\":\"RSP, 0x20\",\"length\":4,\"bytes\":\"4883ec20\"},{\"address\":\"14063ca36\",\"mnemonic\":\"LEA\",\"operands\":\"RBX, [RCX + 0x168]\",\"length\":7,\"bytes\":\"488d9968010000\"},{\"address\":\"14063ca3d\",\"mnemonic\":\"MOV\",\"operands\":\"RCX, RBX\",\"length\":3,\"bytes\":\"488bcb\"},{\"address\":\"14063ca40\",\"mnemonic\":\"CALL\",\"operands\":\"0x140143bb0\",\"length\":5,\"bytes\":\"e86b71b0ff\"},{\"address\":\"14063ca45\",\"mnemonic\":\"TEST\",\"operands\":\"AL, AL\",\"length\":2,\"bytes\":\"84c0\"},{\"address\":\"14063ca47\",\"mnemonic\":\"JNZ\",\"operands\":\"0x14063ca68\",\"length\":2,\"bytes\":\"751f\"},{\"address\":\"14063ca49\",\"mnemonic\":\"MOV\",\"operands\":\"RCX, RBX\",\"length\":3,\"bytes\":\"488bcb\"},{\"address\":\"14063ca4c\",\"mnemonic\":\"CALL\",\"operands\":\"0x1401d9090\",\"length\":5,\"bytes\":\"e83fc6b9ff\"}],\"instructions_total\":58,\"truncated\":true,\"truncation_note\":\"max_instructions=40 reached; raise the cap to get the rest.\"}"
      }
    ],
    "structuredContent": {
      "result": "{\"success\":true,\"start_address\":\"14063c9f0\",\"end_address\":\"14063ca8f\",\"bytes_disassembled\":160,\"message\":\"Successfully disassembled 160 byte(s)\",\"instructions\":[{\"address\":\"14063c9f0\",\"mnemonic\":\"MOV\",\"operands\":\"EAX, dword ptr [RCX + 0x20]\",\"length\":3,\"bytes\":\"8b4120\"},{\"address\":\"14063c9f3\",\"mnemonic\":\"MOV\",\"operands\":\"R8D, dword ptr [RCX + 0x24]\",\"length\":4,\"bytes\":\"448b4124\"},{\"address\":\"14063c9f7\",\"mnemonic\":\"LEA\",\"operands\":\"R9D, [RAX + RDX*0x1]\",\"length\":4,\"bytes\":\"448d0c10\"},{\"address\":\"14063c9fb\",\"mnemonic\":\"CMP\",\"operands\":\"R9D, R8D\",\"length\":3,\"bytes\":\"453bc8\"},{\"address\":\"14063c9fe\",\"mnemonic\":\"JG\",\"operands\":\"0x14063ca06\",\"length\":2,\"bytes\":\"7f06\"},{\"address\":\"14063ca00\",\"mnemonic\":\"MOV\",\"operands\":\"dword ptr [RCX + 0x20], R9D\",\"length\":4,\"bytes\":\"44894920\"},{\"address\":\"14063ca04\",\"mnemonic\":\"JMP\",\"operands\":\"0x14063ca0f\",\"length\":2,\"bytes\":\"eb09\"},{\"address\":\"14063ca06\",\"mnemonic\":\"MOV\",\"operands\":\"EDX, R8D\",\"length\":3,\"bytes\":\"418bd0\"},{\"address\":\"14063ca09\",\"mnemonic\":\"MOV\",\"operands\":\"dword ptr [RCX + 0x20], R8D\",\"length\":4,\"bytes\":\"44894120\"},{\"address\":\"14063ca0d\",\"mnemonic\":\"SUB\",\"operands\":\"EDX, EAX\",\"length\":2,\"bytes\":\"2bd0\"},{\"address\":\"14063ca0f\",\"mnemonic\":\"TEST\",\"operands\":\"EAX, EAX\",\"length\":2,\"bytes\":\"85c0\"},{\"address\":\"14063ca11\",\"mnemonic\":\"JNZ\",\"operands\":\"0x14063ca1e\",\"length\":2,\"bytes\":\"750b\"},{\"address\":\"14063ca13\",\"mnemonic\":\"TEST\",\"operands\":\"EDX, EDX\",\"length\":2,\"bytes\":\"85d2\"},{\"address\":\"14063ca15\",\"mnemonic\":\"JLE\",\"operands\":\"0x14063ca1e\",\"length\":2,\"bytes\":\"7e07\"},{\"address\":\"14063ca17\",\"mnemonic\":\"MOV\",\"operands\":\"byte ptr [RCX + 0x28], 0x1\",\"length\":4,\"bytes\":\"c6412801\"},{\"address\":\"14063ca1b\",\"mnemonic\":\"MOV\",\"operands\":\"EAX, EDX\",\"length\":2,\"bytes\":\"8bc2\"},{\"address\":\"14063ca1d\",\"mnemonic\":\"RET\",\"operands\":\"\",\"length\":1,\"bytes\":\"c3\"},{\"address\":\"14063ca1e\",\"mnemonic\":\"MOV\",\"operands\":\"byte ptr [RCX + 0x28], 0x0\",\"length\":4,\"bytes\":\"c6412800\"},{\"address\":\"14063ca22\",\"mnemonic\":\"MOV\",\"operands\":\"EAX, EDX\",\"length\":2,\"bytes\":\"8bc2\"},{\"address\":\"14063ca24\",\"mnemonic\":\"RET\",\"operands\":\"\",\"length\":1,\"bytes\":\"c3\"},{\"address\":\"14063ca25\",\"mnemonic\":\"INT3\",\"operands\":\"\",\"length\":1,\"bytes\":\"cc\"},{\"address\":\"14063ca26\",\"mnemonic\":\"INT3\",\"operands\":\"\",\"length\":1,\"bytes\":\"cc\"},{\"address\":\"14063ca27\",\"mnemonic\":\"INT3\",\"operands\":\"\",\"length\":1,\"bytes\":\"cc\"},{\"address\":\"14063ca28\",\"mnemonic\":\"INT3\",\"operands\":\"\",\"length\":1,\"bytes\":\"cc\"},{\"address\":\"14063ca29\",\"mnemonic\":\"INT3\",\"operands\":\"\",\"length\":1,\"bytes\":\"cc\"},{\"address\":\"14063ca2a\",\"mnemonic\":\"INT3\",\"operands\":\"\",\"length\":1,\"bytes\":\"cc\"},{\"address\":\"14063ca2b\",\"mnemonic\":\"INT3\",\"operands\":\"\",\"length\":1,\"bytes\":\"cc\"},{\"address\":\"14063ca2c\",\"mnemonic\":\"INT3\",\"operands\":\"\",\"length\":1,\"bytes\":\"cc\"},{\"address\":\"14063ca2d\",\"mnemonic\":\"INT3\",\"operands\":\"\",\"length\":1,\"bytes\":\"cc\"},{\"address\":\"14063ca2e\",\"mnemonic\":\"INT3\",\"operands\":\"\",\"length\":1,\"bytes\":\"cc\"},{\"address\":\"14063ca2f\",\"mnemonic\":\"INT3\",\"operands\":\"\",\"length\":1,\"bytes\":\"cc\"},{\"address\":\"14063ca30\",\"mnemonic\":\"PUSH\",\"operands\":\"RBX\",\"length\":2,\"bytes\":\"4053\"},{\"address\":\"14063ca32\",\"mnemonic\":\"SUB\",\"operands\":\"RSP, 0x20\",\"length\":4,\"bytes\":\"4883ec20\"},{\"address\":\"14063ca36\",\"mnemonic\":\"LEA\",\"operands\":\"RBX, [RCX + 0x168]\",\"length\":7,\"bytes\":\"488d9968010000\"},{\"address\":\"14063ca3d\",\"mnemonic\":\"MOV\",\"operands\":\"RCX, RBX\",\"length\":3,\"bytes\":\"488bcb\"},{\"address\":\"14063ca40\",\"mnemonic\":\"CALL\",\"operands\":\"0x140143bb0\",\"length\":5,\"bytes\":\"e86b71b0ff\"},{\"address\":\"14063ca45\",\"mnemonic\":\"TEST\",\"operands\":\"AL, AL\",\"length\":2,\"bytes\":\"84c0\"},{\"address\":\"14063ca47\",\"mnemonic\":\"JNZ\",\"operands\":\"0x14063ca68\",\"length\":2,\"bytes\":\"751f\"},{\"address\":\"14063ca49\",\"mnemonic\":\"MOV\",\"operands\":\"RCX, RBX\",\"length\":3,\"bytes\":\"488bcb\"},{\"address\":\"14063ca4c\",\"mnemonic\":\"CALL\",\"operands\":\"0x1401d9090\",\"length\":5,\"bytes\":\"e83fc6b9ff\"}],\"instructions_total\":58,\"truncated\":true,\"truncation_note\":\"max_instructions=40 reached; raise the cap to get the rest.\"}"
    },
    "isError": false
  },
  "addClamped": {
    "status": "fulfilled",
    "value": {
      "content": [
        {
          "type": "text",
          "text": "\r\nint D2_InventoryEntry_AddClamped(longlong param_1,int param_2)\r\n\r\n{\r\n  int iVar1;\r\n  int iVar2;\r\n  \r\n  iVar1 = *(int *)(param_1 + 0x20);\r\n  iVar2 = *(int *)(param_1 + 0x24);\r\n  if (iVar2 < iVar1 + param_2) {\r\n    *(int *)(param_1 + 0x20) = iVar2;\r\n    param_2 = iVar2 - iVar1;\r\n  }\r\n  else {\r\n    *(int *)(param_1 + 0x20) = iVar1 + param_2;\r\n  }\r\n  if ((iVar1 == 0) && (0 < param_2)) {\r\n    *(undefined1 *)(param_1 + 0x28) = 1;\r\n    return param_2;\r\n  }\r\n  *(undefined1 *)(param_1 + 0x28) = 0;\r\n  return param_2;\r\n}\r\n\r\n"
        }
      ],
      "structuredContent": {
        "result": "\r\nint D2_InventoryEntry_AddClamped(longlong param_1,int param_2)\r\n\r\n{\r\n  int iVar1;\r\n  int iVar2;\r\n  \r\n  iVar1 = *(int *)(param_1 + 0x20);\r\n  iVar2 = *(int *)(param_1 + 0x24);\r\n  if (iVar2 < iVar1 + param_2) {\r\n    *(int *)(param_1 + 0x20) = iVar2;\r\n    param_2 = iVar2 - iVar1;\r\n  }\r\n  else {\r\n    *(int *)(param_1 + 0x20) = iVar1 + param_2;\r\n  }\r\n  if ((iVar1 == 0) && (0 < param_2)) {\r\n    *(undefined1 *)(param_1 + 0x28) = 1;\r\n    return param_2;\r\n  }\r\n  *(undefined1 *)(param_1 + 0x28) = 0;\r\n  return param_2;\r\n}\r\n\r\n"
      },
      "isError": false
    }
  },
  "isFull": {
    "status": "fulfilled",
    "value": {
      "content": [
        {
          "type": "text",
          "text": "\r\nbool FUN_140615d90(longlong param_1,int param_2)\r\n\r\n{\r\n  longlong lVar1;\r\n  longlong *plVar2;\r\n  longlong lVar3;\r\n  longlong lVar4;\r\n  longlong *plVar5;\r\n  bool bVar6;\r\n  float fVar7;\r\n  float fVar8;\r\n  float fVar9;\r\n  float fVar10;\r\n  \r\n  bVar6 = false;\r\n  if ((-1 < param_2) && (param_2 < (int)(*(uint *)(param_1 + 0x40) & 0xffffff))) {\r\n    plVar5 = (longlong *)((longlong)param_2 * 0x38 + *(longlong *)(param_1 + 0x38));\r\n    lVar1 = *plVar5;\r\n    if (*(int *)(lVar1 + 0x60) == 6) {\r\n      plVar2 = (longlong *)FUN_1403da910(param_1 + 0xf8,2);\r\n      lVar3 = FUN_1403da910(param_1 + 0x108,2);\r\n      fVar7 = 0.0;\r\n      fVar10 = 0.0;\r\n      fVar8 = 0.0;\r\n      fVar9 = 0.0;\r\n      lVar4 = (**(code **)**(undefined8 **)(param_1 + 0x10))();\r\n      if ((DAT_1439ce350 <= *(int *)(lVar4 + 0x60)) && (*(int *)(lVar4 + 0x60) <= DAT_1439ce354)) {\r\n        fVar7 = *(float *)(plVar2 + 4);\r\n        fVar10 = (float)(**(code **)(*plVar2 + 0xd8))(plVar2);\r\n        fVar8 = (float)FUN_140745f60(lVar3);\r\n        fVar9 = (float)FUN_140745fb0(lVar3);\r\n      }\r\n      if (((0 < *(int *)(lVar1 + 0x78)) && (plVar2 != (longlong *)0x0)) && (fVar10 <= fVar7)) {\r\n        bVar6 = *(int *)((longlong)plVar5 + 0x24) <= (int)plVar5[4];\r\n      }\r\n      if (((0 < *(int *)(lVar1 + 0x7c)) && (lVar3 != 0)) && (fVar9 <= fVar8)) {\r\n        bVar6 = *(int *)((longlong)plVar5 + 0x24) <= (int)plVar5[4];\r\n      }\r\n    }\r\n    else {\r\n      bVar6 = *(int *)((longlong)plVar5 + 0x24) <= (int)plVar5[4];\r\n    }\r\n    return bVar6;\r\n  }\r\n  return true;\r\n}\r\n\r\n"
        }
      ],
      "structuredContent": {
        "result": "\r\nbool FUN_140615d90(longlong param_1,int param_2)\r\n\r\n{\r\n  longlong lVar1;\r\n  longlong *plVar2;\r\n  longlong lVar3;\r\n  longlong lVar4;\r\n  longlong *plVar5;\r\n  bool bVar6;\r\n  float fVar7;\r\n  float fVar8;\r\n  float fVar9;\r\n  float fVar10;\r\n  \r\n  bVar6 = false;\r\n  if ((-1 < param_2) && (param_2 < (int)(*(uint *)(param_1 + 0x40) & 0xffffff))) {\r\n    plVar5 = (longlong *)((longlong)param_2 * 0x38 + *(longlong *)(param_1 + 0x38));\r\n    lVar1 = *plVar5;\r\n    if (*(int *)(lVar1 + 0x60) == 6) {\r\n      plVar2 = (longlong *)FUN_1403da910(param_1 + 0xf8,2);\r\n      lVar3 = FUN_1403da910(param_1 + 0x108,2);\r\n      fVar7 = 0.0;\r\n      fVar10 = 0.0;\r\n      fVar8 = 0.0;\r\n      fVar9 = 0.0;\r\n      lVar4 = (**(code **)**(undefined8 **)(param_1 + 0x10))();\r\n      if ((DAT_1439ce350 <= *(int *)(lVar4 + 0x60)) && (*(int *)(lVar4 + 0x60) <= DAT_1439ce354)) {\r\n        fVar7 = *(float *)(plVar2 + 4);\r\n        fVar10 = (float)(**(code **)(*plVar2 + 0xd8))(plVar2);\r\n        fVar8 = (float)FUN_140745f60(lVar3);\r\n        fVar9 = (float)FUN_140745fb0(lVar3);\r\n      }\r\n      if (((0 < *(int *)(lVar1 + 0x78)) && (plVar2 != (longlong *)0x0)) && (fVar10 <= fVar7)) {\r\n        bVar6 = *(int *)((longlong)plVar5 + 0x24) <= (int)plVar5[4];\r\n      }\r\n      if (((0 < *(int *)(lVar1 + 0x7c)) && (lVar3 != 0)) && (fVar9 <= fVar8)) {\r\n        bVar6 = *(int *)((longlong)plVar5 + 0x24) <= (int)plVar5[4];\r\n      }\r\n    }\r\n    else {\r\n      bVar6 = *(int *)((longlong)plVar5 + 0x24) <= (int)plVar5[4];\r\n    }\r\n    return bVar6;\r\n  }\r\n  return true;\r\n}\r\n\r\n"
      },
      "isError": false
    }
  }
}

````

## Unescaped text fields for reading

### text

````text
{"success":true,"start_address":"14063c9f0","end_address":"14063ca8f","bytes_disassembled":160,"message":"Successfully disassembled 160 byte(s)","instructions":[{"address":"14063c9f0","mnemonic":"MOV","operands":"EAX, dword ptr [RCX + 0x20]","length":3,"bytes":"8b4120"},{"address":"14063c9f3","mnemonic":"MOV","operands":"R8D, dword ptr [RCX + 0x24]","length":4,"bytes":"448b4124"},{"address":"14063c9f7","mnemonic":"LEA","operands":"R9D, [RAX + RDX*0x1]","length":4,"bytes":"448d0c10"},{"address":"14063c9fb","mnemonic":"CMP","operands":"R9D, R8D","length":3,"bytes":"453bc8"},{"address":"14063c9fe","mnemonic":"JG","operands":"0x14063ca06","length":2,"bytes":"7f06"},{"address":"14063ca00","mnemonic":"MOV","operands":"dword ptr [RCX + 0x20], R9D","length":4,"bytes":"44894920"},{"address":"14063ca04","mnemonic":"JMP","operands":"0x14063ca0f","length":2,"bytes":"eb09"},{"address":"14063ca06","mnemonic":"MOV","operands":"EDX, R8D","length":3,"bytes":"418bd0"},{"address":"14063ca09","mnemonic":"MOV","operands":"dword ptr [RCX + 0x20], R8D","length":4,"bytes":"44894120"},{"address":"14063ca0d","mnemonic":"SUB","operands":"EDX, EAX","length":2,"bytes":"2bd0"},{"address":"14063ca0f","mnemonic":"TEST","operands":"EAX, EAX","length":2,"bytes":"85c0"},{"address":"14063ca11","mnemonic":"JNZ","operands":"0x14063ca1e","length":2,"bytes":"750b"},{"address":"14063ca13","mnemonic":"TEST","operands":"EDX, EDX","length":2,"bytes":"85d2"},{"address":"14063ca15","mnemonic":"JLE","operands":"0x14063ca1e","length":2,"bytes":"7e07"},{"address":"14063ca17","mnemonic":"MOV","operands":"byte ptr [RCX + 0x28], 0x1","length":4,"bytes":"c6412801"},{"address":"14063ca1b","mnemonic":"MOV","operands":"EAX, EDX","length":2,"bytes":"8bc2"},{"address":"14063ca1d","mnemonic":"RET","operands":"","length":1,"bytes":"c3"},{"address":"14063ca1e","mnemonic":"MOV","operands":"byte ptr [RCX + 0x28], 0x0","length":4,"bytes":"c6412800"},{"address":"14063ca22","mnemonic":"MOV","operands":"EAX, EDX","length":2,"bytes":"8bc2"},{"address":"14063ca24","mnemonic":"RET","operands":"","length":1,"bytes":"c3"},{"address":"14063ca25","mnemonic":"INT3","operands":"","length":1,"bytes":"cc"},{"address":"14063ca26","mnemonic":"INT3","operands":"","length":1,"bytes":"cc"},{"address":"14063ca27","mnemonic":"INT3","operands":"","length":1,"bytes":"cc"},{"address":"14063ca28","mnemonic":"INT3","operands":"","length":1,"bytes":"cc"},{"address":"14063ca29","mnemonic":"INT3","operands":"","length":1,"bytes":"cc"},{"address":"14063ca2a","mnemonic":"INT3","operands":"","length":1,"bytes":"cc"},{"address":"14063ca2b","mnemonic":"INT3","operands":"","length":1,"bytes":"cc"},{"address":"14063ca2c","mnemonic":"INT3","operands":"","length":1,"bytes":"cc"},{"address":"14063ca2d","mnemonic":"INT3","operands":"","length":1,"bytes":"cc"},{"address":"14063ca2e","mnemonic":"INT3","operands":"","length":1,"bytes":"cc"},{"address":"14063ca2f","mnemonic":"INT3","operands":"","length":1,"bytes":"cc"},{"address":"14063ca30","mnemonic":"PUSH","operands":"RBX","length":2,"bytes":"4053"},{"address":"14063ca32","mnemonic":"SUB","operands":"RSP, 0x20","length":4,"bytes":"4883ec20"},{"address":"14063ca36","mnemonic":"LEA","operands":"RBX, [RCX + 0x168]","length":7,"bytes":"488d9968010000"},{"address":"14063ca3d","mnemonic":"MOV","operands":"RCX, RBX","length":3,"bytes":"488bcb"},{"address":"14063ca40","mnemonic":"CALL","operands":"0x140143bb0","length":5,"bytes":"e86b71b0ff"},{"address":"14063ca45","mnemonic":"TEST","operands":"AL, AL","length":2,"bytes":"84c0"},{"address":"14063ca47","mnemonic":"JNZ","operands":"0x14063ca68","length":2,"bytes":"751f"},{"address":"14063ca49","mnemonic":"MOV","operands":"RCX, RBX","length":3,"bytes":"488bcb"},{"address":"14063ca4c","mnemonic":"CALL","operands":"0x1401d9090","length":5,"bytes":"e83fc6b9ff"}],"instructions_total":58,"truncated":true,"truncation_note":"max_instructions=40 reached; raise the cap to get the rest."}
````

### text

````text

int D2_InventoryEntry_AddClamped(longlong param_1,int param_2)

{
  int iVar1;
  int iVar2;
  
  iVar1 = *(int *)(param_1 + 0x20);
  iVar2 = *(int *)(param_1 + 0x24);
  if (iVar2 < iVar1 + param_2) {
    *(int *)(param_1 + 0x20) = iVar2;
    param_2 = iVar2 - iVar1;
  }
  else {
    *(int *)(param_1 + 0x20) = iVar1 + param_2;
  }
  if ((iVar1 == 0) && (0 < param_2)) {
    *(undefined1 *)(param_1 + 0x28) = 1;
    return param_2;
  }
  *(undefined1 *)(param_1 + 0x28) = 0;
  return param_2;
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
