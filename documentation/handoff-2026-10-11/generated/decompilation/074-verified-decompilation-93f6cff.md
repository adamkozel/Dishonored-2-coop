# verified-decompilation.json

Original: [outputs/d2-coop-lab/analysis/ghidra/verified-decompilation.json](<../../../../outputs/d2-coop-lab/analysis/ghidra/verified-decompilation.json>)

SHA-256: `13C3EA74658CF923F96DA0E2DF0BEB77C5C81E17D41D3C43B55E90F689387768`; 1295 bytes.

## Original saved record

````json
{
  "meta": null,
  "content": [
    {
      "type": "text",
      "text": "\r\n/* Project scope: exact retail executable only. Confirmed findings have bounded static/runtime\r\n   evidence; imported hypotheses are unverified. See analysis/ghidra/native-findings.json. No\r\n   complete campaign, general combat, Emily equipment, or online session claim. */\r\n\r\nfloat D2_Health_GetRatio(D2_HealthPartial *health)\r\n\r\n{\r\n  float fVar1;\r\n  \r\n  fVar1 = (float)(**(code **)(health->vtable + 0xd8))();\r\n  if (0.0 < fVar1) {\r\n    return health->currentHealth / fVar1;\r\n  }\r\n  return 0.0;\r\n}\r\n\r\n",
      "annotations": null,
      "meta": null
    }
  ],
  "structuredContent": {
    "result": "\r\n/* Project scope: exact retail executable only. Confirmed findings have bounded static/runtime\r\n   evidence; imported hypotheses are unverified. See analysis/ghidra/native-findings.json. No\r\n   complete campaign, general combat, Emily equipment, or online session claim. */\r\n\r\nfloat D2_Health_GetRatio(D2_HealthPartial *health)\r\n\r\n{\r\n  float fVar1;\r\n  \r\n  fVar1 = (float)(**(code **)(health->vtable + 0xd8))();\r\n  if (0.0 < fVar1) {\r\n    return health->currentHealth / fVar1;\r\n  }\r\n  return 0.0;\r\n}\r\n\r\n"
  },
  "isError": false
}

````

## Unescaped text fields for reading

### text

````text

/* Project scope: exact retail executable only. Confirmed findings have bounded static/runtime
   evidence; imported hypotheses are unverified. See analysis/ghidra/native-findings.json. No
   complete campaign, general combat, Emily equipment, or online session claim. */

float D2_Health_GetRatio(D2_HealthPartial *health)

{
  float fVar1;
  
  fVar1 = (float)(**(code **)(health->vtable + 0xd8))();
  if (0.0 < fVar1) {
    return health->currentHealth / fVar1;
  }
  return 0.0;
}


````
