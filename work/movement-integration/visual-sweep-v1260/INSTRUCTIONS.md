# Visual review sweep: Dishonored 2 co-op partner presentation

You are reviewing real game recordings from a Dishonored 2 co-op mod. In these videos, one player's character is presented inside the
other player's game window as a full-body "partner" (Corvo or Emily) that mirrors what the remote player is doing. Your job is
to decide, per checklist item and per character, whether the partner's full-body motion in the recordings is
**functionally acceptable**.

## Inputs
- Your group file (JSON) lists checklist items: `id`, `title`, `runtime_binding`, `notes`, and up to 8 `videos` each with
  `path` (relative to the repo root `C:\Users\adako\Documents\Codex\2026-10-08\i-x20`), `character` (the partner character shown),
  `label`, `scope` and any `prior_review`.
- Repo root: `C:\Users\adako\Documents\Codex\2026-10-08\i-x20`. ffmpeg/ffprobe are on PATH (use the Bash tool).

## Method
1. For each item, pick at most 2 videos per character (prefer the most recent candidate, observer/partner views; skip videos whose
   file name ends in `-source.mp4` or whose label says it is the source's own first-person view - those don't show the partner).
   If a file is missing, note it and move on.
2. Make contact sheets into YOUR OWN scratch folder `...\scratchpad\sweep\<group>-sheets\` (never write inside the repo), e.g.
   `ffmpeg -hide_banner -loglevel error -y -i <video> -vf "fps=2,scale=480:-1,tile=4x4" -frames:v 1 sheet.jpg`. Use `-ss/-t` to cover the
   action; if the partner is small or dark, crop around them and brighten (`crop=...,eq=brightness=0.1:gamma=1.4`). Look at the
   sheets with the Read tool. Use ffprobe for duration. Keep it efficient: 1-3 sheets per video.
3. Judge the partner's body (not the first-person hands):
   - **accept** when the action is recognizable and reads naturally enough to ship as functional coverage: correct action, plausible
     timing, intact limbs, feet roughly planted when standing, smooth enough entry/exit. Minor polish issues (slight foot slide,
     generic donor motion, small clipping) are acceptable; note them. The user explicitly wants functional-first coverage with polish
     later by an animator.
   - **reject** for clear defects: T-pose/bind pose, frozen pose while travelling, broken/twisted limbs, popping/teleporting,
     the partner missing/flickering, the wrong action, or a pose that stays stuck after the action.
   - **insufficient** when the partner is not visible or can't be judged even after cropping.
4. Do not touch the game, the lab, git, or any repo file. Read-only except your scratch folder and your output file.

## Output
Write `...\scratchpad\sweep\<group>-review.json`: a JSON list with one object per item:
`{"id": ..., "verdict": "accept"|"reject"|"insufficient", "characters": {"corvo": "accept|reject|insufficient|n/a", "emily": "..."},
  "videos": [{"path": ..., "character": ..., "verdict": ..., "review": "<one or two sentences describing what the partner does>"}],
  "summary": "<one sentence>"}`.
The item verdict is "accept" only if every applicable character is accepted (powers are owned by one character: the other is n/a).
Finish with a short text summary of counts (accept/reject/insufficient) and the rejected items with their defects.
