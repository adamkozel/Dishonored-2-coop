# ghidra-inventory-restore-note.json

Original: [work/ghidra-inventory-restore-note.json](<../../../../work/ghidra-inventory-restore-note.json>)

SHA-256: `224BBD669EB348847B326C2B710583795ED927E3E717E7DACD2D3614B649F11A`; 689 bytes.

## Original saved record

````json
{
  "program": "Dishonored2.exe",
  "comment": "[confirmed-static behavior; semantic name remains hypothesis] Exact B671F824 RVA6182f0: traverses existing inventory items at +38 with 56-byte stride, matches per-item native name hash to 32-byte records, updates quantities through 63cbe0/63cc00 plus notification6170b0, and calls Inventory_Equip(entry,false) when record+16 is set. This restores state into pre-existing entries; it does not demonstrate creation of missing Emily equipment. PE unwind range61832a is a secondary range inside the function, not its true entry6182f0. Evidence: inventory-restore-decompilation.json and dumpbin6182f0..618458.",
  "address": "0x1406182f0"
}

````
