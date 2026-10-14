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
- Data dumps over 256 KB. The exceptions are the live `work/current-task-state.json` and `work/natural-moveset/review-catalog.json`, which are tracked up to 45 MB (GitHub warns at 50 MB).
- Ghidra databases and installed tools.

[`tools/repo/build_repo_manifest.py`](tools/repo/build_repo_manifest.py) defines exactly what is tracked. It writes the oversized files into a generated section of `.gitignore`, so `git add -A` stages exactly the selection (about 10 s):

```
python tools/repo/build_repo_manifest.py --update-gitignore
git add -A
```

`--report` summarizes what is included and skipped. `--write files.txt` lists the selection.

`.gitignore` denies by default. [`tools/repo/pre-commit`](tools/repo/pre-commit) rejects binaries, oversized files, never-committed folders and any SteamID. Install it with `cp tools/repo/pre-commit .git/hooks/pre-commit`.

## Workflow

`main` holds verified work. New work goes on a branch named after the roadmap stage and candidate, for example `stage3/use-interactions-1213`, and is merged through a pull request.

Each PR includes:
- the candidate's identity;
- the harness takes with their results;
- the audit;
- the updated handoff chapters.

The agent runs the whole cycle without approval pauses. The exact loop is in [CLAUDE.md](CLAUDE.md):
1. Branch per board card.
2. Commit and push after every result, to a **draft** PR. Drafts run no CI; the pre-commit hook has already checked each commit.
3. `gh pr ready` when the card is done.
4. Squash-merge once the checks are green. Work continues while they run; nothing waits on CI.

PR checks run when a PR is ready to merge:
- **checks / repo** (ubuntu, about 1 min):
  - the guard: `tools/repo/pre-commit --all` over every tracked file;
  - `tools/repo/check-sources.sh`, which confirms every build input of the lab project is committed;
  - the evidence self-test and the evidence check.
- **unit-tests** (windows, about 3–4 min): a sparse checkout that builds only the test targets with MSVC and runs the self-contained unit tests.
  - It runs only when `src/`, `third_party/` or `CMakeLists.txt` of the lab project change.
  - The mod DLL is built locally for every candidate.

Both are kept lean for the free plan's 2,000 Actions minutes a month.

New pushes cancel superseded runs. The evidence check works like this:
  - It covers every folder holding `result.json` or `run-script.ps1`, and `work/movement-integration/candidate-*/`.
  - A PR may add files there but not modify, delete or rename existing ones.
  - The pre-commit hook runs the same check (`tools/repo/check-evidence.sh`) locally.
  - A deliberate amendment, such as a privacy scrub, needs `EVIDENCE_AMEND=1` locally, the `evidence-amend` label on the PR, and the reason in the PR description.

Lab takes and visual acceptance still happen on the local two-instance lab. CI does not replace them.

The [roadmap board](https://github.com/users/adamkozel/projects/3/views/2) shows the work at a glance:
- **Columns:** Todo, In Progress, Needs you, Done.
- **Stage field:** 1–7.
- **Cards:** one issue per open item at working size, plus one umbrella issue per later stage. The 105-item movement checklist stays in [movement-requirements.md](documentation/handoff-2026-10-11/generated/movement-requirements.md).
- **Updates:** a PR says `Closes #n`, so merging it moves the card to Done. "Needs you" holds items that only the user can do.
- **Source of truth:** the roadmap and handoff docs; the board is a view of them.

Line endings are committed as-is (`core.autocrlf=false`), so script hashes recorded in the evidence stay valid.
