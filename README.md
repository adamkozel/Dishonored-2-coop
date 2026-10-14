# Dishonored 2: Corvo/Emily co-op

A two-player co-op mod for Dishonored 2: Corvo and Emily play the campaign together over native Steam networking.
- Native first person, UI and Steam (the game's own `steam_api64`, never replaced).
- Partner presentation on the game's own Doppelganger rigs.

Private development repository.

## Where to start

- [Handoff documentation](documentation/handoff-2026-10-11/README.md): chapter index, current state, roadmap and runbooks.
  - [Current state](documentation/handoff-2026-10-11/01-current-state.md).
  - [Roadmap and stage exit criteria](documentation/handoff-2026-10-11/02-roadmap.md).
- [Mod source](outputs/d2-coop-lab/src): the `dinput8.dll` proxy and engine bridge, with its CMake build in `outputs/d2-coop-lab`.
- `work/movement-integration`: candidate build, stage and deploy scripts, audits, and candidate `identity.json` files.
- `work/natural-moveset`: lab harnesses (`Test-*.ps1`, `Probe-*.ps1`), bank builders and per-take evidence.
- `work/release`: release builder, installer tests and product-mode tests.

## What is not in this repository

These stay on the development machine:
- Copies of the game (`work/asset-game`, `work/lab-game`), lab instances and save profiles.
- Game-derived partner assets and release packages. Rebuild packages with `work/release/build_release_*.py` from a local game copy.
- Videos, movement banks and other binaries.
- Data dumps over 256 KB.
- Ghidra databases and installed tools.

[`tools/repo/build_repo_manifest.py`](tools/repo/build_repo_manifest.py) defines exactly what is tracked:

```
python tools/repo/build_repo_manifest.py --report
python tools/repo/build_repo_manifest.py --write files.txt --write-exclude .git/info/exclude
git add --pathspec-from-file=files.txt
```

`.gitignore` denies by default. [`tools/repo/pre-commit`](tools/repo/pre-commit) rejects binaries, oversized files, never-committed folders and any SteamID. Install it with `cp tools/repo/pre-commit .git/hooks/pre-commit`.

## Workflow

`main` holds verified work. New work goes on a branch named after the roadmap stage and candidate, for example `stage3/use-interactions-1213`, and is merged through a pull request.

Each PR includes:
- the candidate's identity;
- the harness takes with their results;
- the audit;
- the updated handoff chapters.

The agent runs the whole cycle without approval pauses:
1. branch;
2. commit;
3. push;
4. `gh pr create`;
5. wait for the checks;
6. `gh pr merge --squash` (the branch is then deleted);
7. fast-forward local `main`.

PR checks:
- **guard** runs `tools/repo/pre-commit --all` over every tracked file.
- **unit-tests** builds the lab project with MSVC and runs the self-contained unit tests. It runs only when `outputs/d2-coop-lab/` changes.

Lab takes and visual acceptance still happen on the local two-instance lab. CI does not replace them.

Line endings are committed as-is (`core.autocrlf=false`), so script hashes recorded in the evidence stay valid.
