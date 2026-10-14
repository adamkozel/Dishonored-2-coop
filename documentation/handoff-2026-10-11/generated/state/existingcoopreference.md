# State record: ExistingCoopReference

Original section of [current-task-state.json](<../../../../work/current-task-state.json>). Captured 2026-10-14T01:55:24.397473+02:00.

This is a verbatim structured record, not a new validation. Earlier fields named Current/Live can describe historical builds.

````json
{
  "at": "2026-10-13T01:00:36.304568+02:00",
  "source": "https://www.nexusmods.com/dishonored2/mods/222",
  "archiveSha256": "65cc2b01ca65b180e8fbde3439054f9739bab7c565ecdd6924c9c6f035564d15",
  "report": "documentation/handoff-2026-10-11/37-existing-coop-mod-assessment.md",
  "staticOnly": true,
  "executed": false,
  "integrated": false,
  "decompiledFunctions": 6,
  "findings": "Native locomotion and hidden NPC/overseer pose donors are useful leads. Traced Shadow Walk path hides the character and tracked models; no complete third-person shadow form demonstrated. Source absent; author permission required by page for modification/assets. Preserve native Steam/native game UI requirements.",
  "next": "Resume current movement work; revisit donor paths only for a concrete missing action and world-ownership leads when useful. Avoid a full third-party DLL reverse-engineering detour.",
  "userSteering": "User explicitly stopped further third-party mod investigation; resume existing pipeline and prioritize broad functional movement coverage, cosmetic polish later."
}
````
