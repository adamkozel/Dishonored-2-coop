# health-subtract-base-1407177e0.txt

Original: [work/movement-integration/npc-damage-research-v902/health-subtract-base-1407177e0.txt](<../../../../work/movement-integration/npc-damage-research-v902/health-subtract-base-1407177e0.txt>)

SHA-256: `7217BA4E9A180217A795508AA3E0ED5EB6F1325091ABF47F59276EE78F40E198`; 136 bytes.

## Original saved record

````text
void FUN_1407177e0(longlong param_1,float param_2)

{
  D2_Health_SetCurrent(param_2,*(float *)(param_1 + 0x20) - param_2);
  return;
}

````
