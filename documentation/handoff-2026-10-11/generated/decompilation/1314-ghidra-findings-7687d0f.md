# ghidra-findings.json

Original: [work/natural-moveset/pair-research/ghidra-findings.json](<../../../../work/natural-moveset/pair-research/ghidra-findings.json>)

SHA-256: `C7F81DEAD3E89F81ABFACD4BF817F52F8304CBABE4C200B68A635701745A3881`; 900 bytes.

## Original saved record

````json
{
 "program":"Dishonored2.exe",
 "sha256":"B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180",
 "confirmed": {
  "anchor_string":"0x141b4a1d0: anchor_jnt",
  "xref":"0x14040ad72 within 0x14040ace0",
  "constructor":"0x14040ace0",
  "RTTI":"arkTreeNodeSettings_Action_CombatStealth, type name at 0x142302f40, locator 0x141f96398",
  "settings_vtable":"0x141b4a228",
  "anchor_member":"+0x70 initialized to anchor_jnt",
  "defaults":{"0x60":1.5,"0x64":15,"0x68":0.2},
  "saved_program_comment":true
 },
 "hypotheses":["Inverse initial victim anchor is a usable partner-to-victim reference transform: supported by native paired preview contact, not yet traced through native alignment code"],
 "not_confirmed":["Meaning of the three float settings", "Runtime paired alignment ownership", "Mapping between AI scene placement and player-driven co-op takedowns"],
 "runtime_changes":false
}

````
