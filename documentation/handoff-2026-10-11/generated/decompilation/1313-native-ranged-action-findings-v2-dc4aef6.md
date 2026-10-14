# native-ranged-action-findings-v2.json

Original: [work/natural-moveset/native-ranged-action-findings-v2.json](<../../../../work/natural-moveset/native-ranged-action-findings-v2.json>)

SHA-256: `58FA32899E7E34EDC6EE5A921A917EBF65A0FAA8B2BFE4EAEF5F657F40923B2F`; 6344 bytes.

## Original saved record

````json
{
  "complete_remote_ranged": false,
  "scope": "Standing native source action/ownership/timing and reversible inventory fixture investigation. These are first-person reference recordings, not remote movement-port acceptance.",
  "native_action_variants": {
    "1": [
      "gun_empty",
      500
    ],
    "2": [
      "crossbow_empty",
      500
    ],
    "3": [
      "gun_fire",
      666
    ],
    "4": [
      "gun_reload",
      1500
    ],
    "5": [
      "gun_equip",
      533
    ],
    "6": [
      "gun_unequip",
      333
    ],
    "7": [
      "crossbow_fire",
      666
    ],
    "8": [
      "crossbow_reload",
      900
    ],
    "9": [
      "crossbow_equip",
      533
    ],
    "10": [
      "crossbow_unequip",
      333
    ]
  },
  "samples": [
    {
      "character": "corvo",
      "kind": "gun",
      "take": "native-ranged-corvo-gun-actions-v2",
      "recording": "native-ranged-corvo-gun-actions-v2.mp4",
      "result": "native-ranged-corvo-gun-actions-v2/result.json",
      "contact": "native-ranged-corvo-gun-actions-v2/contact.jpg",
      "checks": [
        "Exact native ranged equipment acquired; normal controller holster/draw initializes native arm presentation",
        "Two native shots and a subsequent native empty trigger action observed",
        "Owned native scene reader validates fire, reload, empty, equip and unequip identities and clocks",
        "Native cleanup restores source inventory quantity/selection; peer inventory unchanged",
        "Both player health and protected files unchanged; fixture restores its two-round ammunition grant"
      ],
      "dll": "6A8D2E361D468049BC119AE4C128F018DFC28BBAE128A0FA8CBA6DB4D09B3534",
      "contact_visually_inspected": true
    },
    {
      "character": "corvo",
      "kind": "crossbow",
      "take": "native-ranged-corvo-crossbow-actions-v2",
      "recording": "native-ranged-corvo-crossbow-actions-v2.mp4",
      "result": "native-ranged-corvo-crossbow-actions-v2/result.json",
      "contact": "native-ranged-corvo-crossbow-actions-v2/contact.jpg",
      "checks": [
        "Exact native ranged equipment acquired; normal controller holster/draw initializes native arm presentation",
        "Two native shots and a subsequent native empty trigger action observed",
        "Owned native scene reader validates fire, reload, empty, equip and unequip identities and clocks",
        "Native cleanup restores source inventory quantity/selection; peer inventory unchanged",
        "Both player health and protected files unchanged; fixture restores its two-round ammunition grant"
      ],
      "dll": "6A8D2E361D468049BC119AE4C128F018DFC28BBAE128A0FA8CBA6DB4D09B3534",
      "contact_visually_inspected": true
    },
    {
      "character": "emily",
      "kind": "gun",
      "take": "native-ranged-emily-gun-actions-v2",
      "recording": "native-ranged-emily-gun-actions-v2.mp4",
      "result": "native-ranged-emily-gun-actions-v2/result.json",
      "contact": "native-ranged-emily-gun-actions-v2/contact.jpg",
      "checks": [
        "Exact native ranged equipment acquired; normal controller holster/draw initializes native arm presentation",
        "Two native shots and a subsequent native empty trigger action observed",
        "Owned native scene reader validates fire, reload, empty, equip and unequip identities and clocks",
        "Native cleanup restores source inventory quantity/selection; peer inventory unchanged",
        "Both player health and protected files unchanged; fixture restores its two-round ammunition grant"
      ],
      "dll": "6A8D2E361D468049BC119AE4C128F018DFC28BBAE128A0FA8CBA6DB4D09B3534",
      "contact_visually_inspected": true
    },
    {
      "character": "emily",
      "kind": "crossbow",
      "take": "native-ranged-emily-crossbow-actions-v2",
      "recording": "native-ranged-emily-crossbow-actions-v2.mp4",
      "result": "native-ranged-emily-crossbow-actions-v2/result.json",
      "contact": "native-ranged-emily-crossbow-actions-v2/contact.jpg",
      "checks": [
        "Exact native ranged equipment acquired; normal controller holster/draw initializes native arm presentation",
        "Two native shots and a subsequent native empty trigger action observed",
        "Owned native scene reader validates fire, reload, empty, equip and unequip identities and clocks",
        "Native cleanup restores source inventory quantity/selection; peer inventory unchanged",
        "Both player health and protected files unchanged; fixture restores its two-round ammunition grant"
      ],
      "dll": "6A8D2E361D468049BC119AE4C128F018DFC28BBAE128A0FA8CBA6DB4D09B3534",
      "contact_visually_inspected": true
    }
  ],
  "checks": 20,
  "confirmed": [
    "Both character roles: exact selected and started inventory weapon and owned left arm bound to the same active native Midnight scene.",
    "Native acquisition requires ordinary X holster/draw to initialize visible first-person equipment presentation in this fixture.",
    "Pistol acquisition grants a role-specific default upgrade. Snapshot and restore that upgrade along with ammo/weapon selection and quantities.",
    "Two regular shots, reload, empty trigger, holster and draw observed for all four combinations.",
    "Player inventories and health restored, protected files unchanged; isolated checkpoint reloaded after shots to clear impacts/projectiles."
  ],
  "visual_notes": [
    "Pistol source recordings show native weapon, muzzle/impact flashes, smoke and reload/holster/draw.",
    "Crossbow source recordings show bolts, mechanism motion, hand presentation and native holster/draw. Crossbow is partly cropped at left in the narrow capture; no complete mechanism quality acceptance claimed.",
    "All four temporal contact sheets inspected; contact sheets do not establish continuous animation quality."
  ],
  "remaining": [
    "Remote weapon hand attachment and grip",
    "Whole-body action binding through Steam",
    "Moving/crouched/airborne variants and interruptions",
    "Weapon mechanism animation and native remote effects",
    "Upgrade and ammunition variants",
    "Remote actual-game recordings and visual acceptance"
  ]
}
````
