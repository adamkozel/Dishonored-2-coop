# ghidra-food-overflow-v824.json

Original: [work/natural-moveset/native-pickup-research-v785/ghidra-food-overflow-v824.json](<../../../../work/natural-moveset/native-pickup-research-v785/ghidra-food-overflow-v824.json>)

SHA-256: `C283F3F5A57B0ED0E7B5D04F7FCFCAF6CD09B1487585FF0DBBAE574AE9F69769`; 3089 bytes.

## Original saved record

````json
{
  "address": "140399620",
  "program": "Dishonored2.exe",
  "executableSha256": "B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180",
  "decompilation": {
    "content": [
      {
        "type": "text",
        "text": "\r\nundefined1 FUN_140399620(void)\r\n\r\n{\r\n  return 0;\r\n}\r\n\r\n"
      }
    ],
    "structuredContent": {
      "result": "\r\nundefined1 FUN_140399620(void)\r\n\r\n{\r\n  return 0;\r\n}\r\n\r\n"
    },
    "isError": false
  },
  "disassembly": {
    "content": [
      {
        "type": "text",
        "text": "{\"success\":true,\"start_address\":\"140399620\",\"end_address\":\"14039962f\",\"bytes_disassembled\":16,\"message\":\"Successfully disassembled 16 byte(s)\",\"instructions\":[{\"address\":\"140399620\",\"mnemonic\":\"XOR\",\"operands\":\"AL, AL\",\"length\":2,\"bytes\":\"32c0\"},{\"address\":\"140399622\",\"mnemonic\":\"RET\",\"operands\":\"\",\"length\":1,\"bytes\":\"c3\"},{\"address\":\"140399623\",\"mnemonic\":\"INT3\",\"operands\":\"\",\"length\":1,\"bytes\":\"cc\"},{\"address\":\"140399624\",\"mnemonic\":\"INT3\",\"operands\":\"\",\"length\":1,\"bytes\":\"cc\"},{\"address\":\"140399625\",\"mnemonic\":\"INT3\",\"operands\":\"\",\"length\":1,\"bytes\":\"cc\"},{\"address\":\"140399626\",\"mnemonic\":\"INT3\",\"operands\":\"\",\"length\":1,\"bytes\":\"cc\"},{\"address\":\"140399627\",\"mnemonic\":\"INT3\",\"operands\":\"\",\"length\":1,\"bytes\":\"cc\"},{\"address\":\"140399628\",\"mnemonic\":\"INT3\",\"operands\":\"\",\"length\":1,\"bytes\":\"cc\"}],\"instructions_total\":15,\"truncated\":true,\"truncation_note\":\"max_instructions=8 reached; raise the cap to get the rest.\"}"
      }
    ],
    "structuredContent": {
      "result": "{\"success\":true,\"start_address\":\"140399620\",\"end_address\":\"14039962f\",\"bytes_disassembled\":16,\"message\":\"Successfully disassembled 16 byte(s)\",\"instructions\":[{\"address\":\"140399620\",\"mnemonic\":\"XOR\",\"operands\":\"AL, AL\",\"length\":2,\"bytes\":\"32c0\"},{\"address\":\"140399622\",\"mnemonic\":\"RET\",\"operands\":\"\",\"length\":1,\"bytes\":\"c3\"},{\"address\":\"140399623\",\"mnemonic\":\"INT3\",\"operands\":\"\",\"length\":1,\"bytes\":\"cc\"},{\"address\":\"140399624\",\"mnemonic\":\"INT3\",\"operands\":\"\",\"length\":1,\"bytes\":\"cc\"},{\"address\":\"140399625\",\"mnemonic\":\"INT3\",\"operands\":\"\",\"length\":1,\"bytes\":\"cc\"},{\"address\":\"140399626\",\"mnemonic\":\"INT3\",\"operands\":\"\",\"length\":1,\"bytes\":\"cc\"},{\"address\":\"140399627\",\"mnemonic\":\"INT3\",\"operands\":\"\",\"length\":1,\"bytes\":\"cc\"},{\"address\":\"140399628\",\"mnemonic\":\"INT3\",\"operands\":\"\",\"length\":1,\"bytes\":\"cc\"}],\"instructions_total\":15,\"truncated\":true,\"truncation_note\":\"max_instructions=8 reached; raise the cap to get the rest.\"}"
    },
    "isError": false
  },
  "finding": "Confirmed executable returns false unconditionally. Food health/mana overflow conversion branch is inactive for this executable; candidate821 guards bytes32c0c3. Native health queue at+f4 and mana queue at+2c remain active."
}

````

## Unescaped text fields for reading

### text

````text
{"success":true,"start_address":"140399620","end_address":"14039962f","bytes_disassembled":16,"message":"Successfully disassembled 16 byte(s)","instructions":[{"address":"140399620","mnemonic":"XOR","operands":"AL, AL","length":2,"bytes":"32c0"},{"address":"140399622","mnemonic":"RET","operands":"","length":1,"bytes":"c3"},{"address":"140399623","mnemonic":"INT3","operands":"","length":1,"bytes":"cc"},{"address":"140399624","mnemonic":"INT3","operands":"","length":1,"bytes":"cc"},{"address":"140399625","mnemonic":"INT3","operands":"","length":1,"bytes":"cc"},{"address":"140399626","mnemonic":"INT3","operands":"","length":1,"bytes":"cc"},{"address":"140399627","mnemonic":"INT3","operands":"","length":1,"bytes":"cc"},{"address":"140399628","mnemonic":"INT3","operands":"","length":1,"bytes":"cc"}],"instructions_total":15,"truncated":true,"truncation_note":"max_instructions=8 reached; raise the cap to get the rest."}
````
