# State record: PartnerInteractionExclusion

Original section of [current-task-state.json](<../../../../work/current-task-state.json>). Captured 2026-10-14T01:55:24.397473+02:00.

This is a verbatim structured record, not a new validation. Earlier fields named Current/Live can describe historical builds.

````json
{
  "requirement": "No direct partner combat, powers, targeting or contextual interactions. Normal movement collision is explicitly allowed.",
  "collision": "Native movement collision intentionally retained; no collision hooks.",
  "receipt": "work/movement-integration/candidate-partner-interaction-v396/runtime-verification-v402.json",
  "complete": false,
  "next": "Resume movement: native Emily Doppelganger cast and world presentation, then remaining movement and shared-world co-op. Keep broader mutual interaction exclusions in backlog rather than gating further development.",
  "live": "B8120436A3242BDD30342F11A20638B32872255641EE6E969B163B1B7EE02289",
  "latest": "396: all8 partner sword contexts398/400 pass; ordinary NPC sword targeting399 retained. Rear choke387 scoped evidence preserved. Broad interaction edge-case audit deferred after user requested moving on.",
  "fatalityFailure": "work/natural-moveset/partner-sword-contexts-host-v392/interaction-analysis.json",
  "priority": "User asked to retain basic safeguards and move on,12 October. Do not expand interaction-only testing now."
}
````
