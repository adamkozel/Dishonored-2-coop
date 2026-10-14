# ghidra-equip-note.json

Original: [work/ghidra-equip-note.json](<../../../../work/ghidra-equip-note.json>)

SHA-256: `B069F67AA78F6F6F484C858F7B6023BB9E89912D005D92B3FC87848550C39D18`; 634 bytes.

## Original saved record

````json
{
  "comment": "[confirmed-static 2026-10-09, exact B671F824 executable] MCP decompilation of RVA614b20 corroborates: requireQuantity gates entry+20 > 0; declaration+68 selects slot; prior slot component receives 7e7c00; slot is replaced; new component receives 7e7b10. Event allocation 8112c0 uses owner handle and event 0x9f. Candidate names of 7e7c00/7e7b10 are Stop/Start but remain hypotheses until direct decompilation. This equips an existing native entry and does not create Emily equipment. Evidence: inventory-equip-decompilation.json, src/avatar_sword.h.",
  "address": "0x140614b20",
  "program": "Dishonored2.exe"
}

````
