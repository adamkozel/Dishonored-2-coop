# blink-endpoint-regression-findings-v1.json

Original: [work/movement-integration/blink-endpoint-regression-findings-v1.json](<../../../../work/movement-integration/blink-endpoint-regression-findings-v1.json>)

SHA-256: `E26444ECBDC14466A87731B3F1D6F4B55D389ABD14E89BF9F404A41023FFB67F`; 2794 bytes.

## Original saved record

````json
{
  "exact_game_sha256": "B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180",
  "observed_build": "96C1FB32D85ED33B2F414955CF53E260CE2016523DD54582B98B1E25875CAD62",
  "test": "natural-moveset/blink-regression-v26-v1/result.json",
  "confirmed": "Native Blink kinematic intermediate foot sample was transmitted before reliable departure/arrival effect playback. Remote body appeared1.0447m from departure, before end confirmed, despite valid native cast and FX counts.",
  "source_start": [
    -33.5,
    -153.0,
    66.403648
  ],
  "source_end": [
    -33.500057,
    -149.205154,
    66.427681
  ],
  "violations": [
    {
      "ms": 2356,
      "render": [
        -33.500057,
        -151.955307,
        66.40863
      ],
      "endpoint_error_m": 1.0447048807304338,
      "power_variant": 3,
      "pending_fx": 2,
      "applied_fx": 0
    },
    {
      "ms": 2419,
      "render": [
        -33.500057,
        -151.955307,
        66.40863
      ],
      "endpoint_error_m": 1.0447048807304338,
      "power_variant": 3,
      "pending_fx": 2,
      "applied_fx": 0
    },
    {
      "ms": 2480,
      "render": [
        -33.500057,
        -151.955307,
        66.40863
      ],
      "endpoint_error_m": 1.0447048807304338,
      "power_variant": 3,
      "pending_fx": 2,
      "applied_fx": 0
    },
    {
      "ms": 2557,
      "render": [
        -33.500057,
        -151.955307,
        66.40863
      ],
      "endpoint_error_m": 1.0447048807304338,
      "power_variant": 3,
      "pending_fx": 2,
      "applied_fx": 0
    }
  ],
  "candidate": "Only publish exact native departure until the native completion bit confirms destination; omit incoherent Blink roots rather than publish eye-space physics origins.",
  "candidate_dll": "E8E73BE946D2DC0C7E9801B04E70A6ED264FDE7A8E7FCBFF794B75FF22A1C99A",
  "build_checks_passed": true,
  "runtime_verified": true,
  "local_physics_changed": false,
  "runtime_checks": [
    "Native Steam stream and v26 bank ready before the cast",
    "corvo native cast phases 1,2,3 drive remote whole-body presentation",
    "Native phase end restores standing movement",
    "Native X cancellation does not teleport or pull the source",
    "Both players/avatar health, source first-person sword, remote native sword and peer power quantity preserved",
    "Native Blink travel creates exactly one network departure and one arrival; aim cancellation creates neither",
    "Repeated reliable arrival packet cannot replay effects",
    "Remote Blink uses a discrete endpoint transfer with no eye-height body jump (max endpoint error 0.0136110005877573 m)"
  ],
  "fixed_evidence": "work/natural-moveset/blink-regression-v26-v2/result.json"
}
````
