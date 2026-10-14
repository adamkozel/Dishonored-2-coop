# ghidra-creation-note.json

Original: [work/ghidra-creation-note.json](<../../../../work/ghidra-creation-note.json>)

SHA-256: `4AE15650C28EB829D707A147D736F4FEE459977F04D182D9E77F7AC9156D1B8E`; 931 bytes.

## Original saved record

````json
{
  "program": "Dishonored2.exe",
  "comment": "[confirmed-static behavior; semantic name hypothesis] RVA619380 only enters if itemDeclaration+80 exists and entry weaponHandle does not resolve. With null third argument it calls the game spawn virtual, initializes transform and parentHandle, registers the spawned entity, finds its equipment component by RTTI, checks componentDeclaration+11c and slot<16, appends the existing entry to inventory equipment list+58, then links it through63cb90. It requires an existing item entry and is not a general inventory-add operation. Runtime read-only observation of the current owned Emily NPC found exactly eight hat/accessory items and zero equipment entries, no sword. No runtime calls to619380 have been made. Next: identify native item insertion and its inverse, declaration lifetime and removal invariants before a bounded Emily equip experiment.",
  "address": "0x140619380"
}

````
