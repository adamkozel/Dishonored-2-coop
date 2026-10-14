# findings.json

Original: [outputs/d2-coop-lab/analysis/ghidra/findings.json](<../../../../outputs/d2-coop-lab/analysis/ghidra/findings.json>)

SHA-256: `DF5C0266C24FEB2C7F3A79A9D143E9D13C5458043AD5C5D14105FF8F184E3DC4`; 3388 bytes.

## Original saved record

````json
[
  {
    "Confidence": "confirmed-static",
    "RuntimeValidated": "Existing Corvo/guard equip only; no new Emily equipment mutation",
    "Finding": "[confirmed-static 2026-10-09, exact B671F824 executable] MCP decompilation of RVA614b20 corroborates: requireQuantity gates entry+20 > 0; declaration+68 selects slot; prior slot component receives 7e7c00; slot is replaced; new component receives 7e7b10. Event allocation 8112c0 uses owner handle and event 0x9f. Candidate names of 7e7c00/7e7b10 are Stop/Start but remain hypotheses until direct decompilation. This equips an existing native entry and does not create Emily equipment. Evidence: inventory-equip-decompilation.json, src/avatar_sword.h.",
    "At": "2026-10-09T07:51:50.6072676+02:00",
    "FunctionRva": "0x614b20"
  },
  {
    "FunctionRva": "0x6182f0",
    "Finding": "[confirmed-static behavior; semantic name remains hypothesis] Exact B671F824 RVA6182f0: traverses existing inventory items at +38 with 56-byte stride, matches per-item native name hash to 32-byte records, updates quantities through 63cbe0/63cc00 plus notification6170b0, and calls Inventory_Equip(entry,false) when record+16 is set. This restores state into pre-existing entries; it does not demonstrate creation of missing Emily equipment. PE unwind range61832a is a secondary range inside the function, not its true entry6182f0. Evidence: inventory-restore-decompilation.json and dumpbin6182f0..618458.",
    "At": "2026-10-09T07:55:05.1451281+02:00",
    "RuntimeValidated": false,
    "Confidence": "confirmed-static behavior; semantic name hypothesis"
  },
  {
    "RuntimeReadOnlyEvidence": "emily-native-inventory-observation.json",
    "RuntimeMutationValidated": false,
    "Confidence": "confirmed-static behavior; semantic name hypothesis",
    "FunctionRva": "0x619380",
    "Finding": "[confirmed-static behavior; semantic name hypothesis] RVA619380 only enters if itemDeclaration+80 exists and entry weaponHandle does not resolve. With null third argument it calls the game spawn virtual, initializes transform and parentHandle, registers the spawned entity, finds its equipment component by RTTI, checks componentDeclaration+11c and slot<16, appends the existing entry to inventory equipment list+58, then links it through63cb90. It requires an existing item entry and is not a general inventory-add operation. Runtime read-only observation of the current owned Emily NPC found exactly eight hat/accessory items and zero equipment entries, no sword. No runtime calls to619380 have been made. Next: identify native item insertion and its inverse, declaration lifetime and removal invariants before a bounded Emily equip experiment.",
    "At": "2026-10-09T07:57:53.0087986+02:00"
  },
  {
    "RuntimeMutationValidated": false,
    "At": "2026-10-09T07:59:23.7243719+02:00",
    "Confidence": "confirmed-static",
    "Finding": "[confirmed-static] RVA614c20 selects already-present inventory entries either by a declaration reference lookup at614f30 or by declaration type-range matching, then calls619380 and collects the resulting component. It does not append a new item to inventory+38. This narrows the remaining Emily sword prerequisite to native item insertion rather than this wrapper. Evidence: inventory-item-wrapper-decompilation.json; no runtime mutation.",
    "FunctionRva": "0x614c20"
  }
]

````
