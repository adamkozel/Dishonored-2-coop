# vault-integration-findings.json

Original: [work/movement-integration/vault-integration-findings.json](<../../../../work/movement-integration/vault-integration-findings.json>)

SHA-256: `44E634DB2983268AA4D288DB90C4DD2EE9D42C3ABBC376476B57EC22E205BF1B`; 1254 bytes.

## Original saved record

````json
{
  "dll_sha256": "B1CB32BAF8E581F46759DCFA019D84BD59229DBFE13E546FF36C1459ACC0CF31",
  "bank_format": "D2MOVE14 / 117 clips",
  "action_protocol": "13 / 224 bytes",
  "tests": [
    "vault-v14c-test",
    "vault-v14c-priority"
  ],
  "checks": 29,
  "complete": false,
  "visual_accepted": false,
  "protected_saves_unchanged": true,
  "binding": "Native state9 VT1c86e18. Approach/exit have no scene handle; active vault_1/2 has exact player and both owned arms, Corvo Scene_2 / Emily Scene_1, declared933ms. Animation phase follows start/end-plane distance, not the prematurely ended native scene clock.",
  "evidence": [
    "integrated-vault-corvo-v3/take.json",
    "integrated-vault-corvo-unarmed-v3/take.json",
    "integrated-vault-emily-v3/take.json",
    "integrated-vault-emily-unarmed-v3/take.json"
  ],
  "limits": [
    "High/window variants and arbitrary support contacts remain unfinished.",
    "v14a failed because an early action handoff reset blend weight and test progress used integer overloads. v14c corrects both; telemetry now samples movement at50ms while active.",
    "Fine support and landing contact remains partly occluded by desk and props."
  ],
  "created": "2026-10-10T23:20:07.669693+02:00"
}
````
