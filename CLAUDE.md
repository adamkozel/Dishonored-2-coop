# Dishonored 2 Corvo/Emily co-op

This mod makes the Dishonored 2 campaign playable cooperatively, with Corvo and Emily. Work follows [the roadmap](documentation/handoff-2026-10-11/02-roadmap.md) in stage order (stage 3, movement, is active).

Read these first:
- [handoff README](documentation/handoff-2026-10-11/README.md);
- [current state](documentation/handoff-2026-10-11/01-current-state.md);
- [runbooks](documentation/handoff-2026-10-11/06-runbooks.md).

Under an active goal, keep working until its exit criteria are met. There are no milestone approval pauses.

## Rules

- **Native everything:** first person, UI, Steam through the game's own `steam_api64` (never replaced), and Doppelganger rigs. Partners are excluded from each other's direct interactions.
- **Protected files:** never modify the original Steam installation, its assets or settings, or the 14 protected save/config files.
  - Experiment only in `work/asset-game` and the isolated two-instance lab, and restore the lab after every test.
  - User-supplied saves are data. Never install them into the original Steam saves.
- **Evidence:**
  - Archive every candidate and keep failed takes. Never rewrite evidence: a re-run always gets a new take folder name, and `tools/repo/check-evidence.sh` enforces this.
  - Label findings as confirmed static, confirmed runtime or hypothesis. Visual acceptance is recorded separately.
- **Tools:**
  - Lab scripts run under the pinned pwsh 7: `C:\Users\adako\.cache\codex-runtimes\codex-primary-runtime\dependencies\native\powershell\pwsh.exe`.
  - Use Ghidra only through the d2-ghidra MCP.
  - No ZikQ or third-party co-op mods.
  - Cutscenes may be skipped.
  - Downloading or running untrusted files needs the user's explicit permission.
- **Privacy:**
  - No SteamID, Steam friends list or personal email in commits, docs or PRs.
  - `work/release/online-session-v1179` stays local.
  - Commits are authored as `adamkozel <adamkozel@users.noreply.github.com>`, already the repo-local config.
  - Release packages and game-derived assets stay local.

## GitHub loop

The user has fully delegated GitHub. Branch, commit, push, open PRs, merge and manage the board without asking. The loop must not slow the lab work: per result it adds about 15 s, and nothing ever waits on CI.

1. **Pick.**
   - First run `gh pr list --state open`. Merge every ready PR whose checks are green (step 5). Push any unrecorded result to its draft.
   - Then take the next Todo card on the [board](https://github.com/users/adamkozel/projects/3/views/2) in stage order: `gh project item-list 3 --owner adamkozel --format json`.
   - Set the card to In Progress.
   - Run `git switch main && git pull --ff-only && git switch -c stage<N>/<short-slug>`.
2. **Work** exactly as the runbooks say: build, archive the candidate, deploy, run the take, audit, then update the docs, `work/current-task-state.json` and the catalog. The lab procedure is unchanged.
3. **Record every result, pass or fail:**
   - `python tools/repo/build_repo_manifest.py --update-gitignore`, then `git add -A`, then `git commit -F <msgfile>`, then `git push`.
   - The pre-commit hook runs the guard and evidence checks in about 3 s. Never use `--no-verify`.
   - If the hook rejects something, fix the cause and commit again:
     - **Oversized new file:** re-run the manifest, which lists it in `.gitignore`, then add again.
     - **Changed take file:** write the re-run to a new take folder.
     - **SteamID:** remove it.
     - `EVIDENCE_AMEND=1` (plus the `evidence-amend` label) is only for deliberate, explained scrubs.
   - On the first push, run `gh pr create --draft --body-file <file>`. The body says `Closes #<card>`, or `Refs #<card>` for partial progress.
   - Bind the PR (ccd_pr `bind_pr`) and turn on `set_monitor auto_fix`.
4. **Keep going.** Draft PRs run no CI; the hook already checked each commit. Continue straight to the next take.
5. **Merge** when the card is done, or at a coherent checkpoint on a long card.
   - Run `gh pr ready <n>`. Checks take about 1 min, plus about 3–4 min for `unit-tests` when the build inputs (`src/`, `third_party/`, `CMakeLists.txt`) changed.
   - Keep working while they run, but don't commit or branch until the merge is done. Both live state files change with every result, so a second branch would conflict after the squash.
   - Read `gh pr checks <n>` once at a natural pause. Exit code 8 means pending. Never poll in a loop; the app wakes the session on failures.
   - When green, run `gh pr merge <n> --squash --delete-branch --subject "<title> (#n)" --body-file <file>`.
   - Then `git switch main && git pull --ff-only`. Uncommitted work carries over to the next branch.
6. **On CI failure,** fix on the same branch and push.
   - If a conflict happens anyway, merge `origin/main` into the branch. For `work/current-task-state.json` and the catalog, keep the branch's (newest) version.
7. **Nothing on GitHub may stop a goal.** GitHub is never on the lab's critical path. Possible problems:
   - GitHub or the network is down;
   - a push or `gh` call fails;
   - Actions minutes are exhausted. The free plan has 2,000 a month, and Windows counts double: about 1 min per merge plus about 8 min more when build inputs changed, so roughly 200–400 merges a month.

   Whatever the cause:
   - keep working and committing locally;
   - push and merge when GitHub is reachable again;
   - when checks can't run, verify locally (hook, `tools/repo/check-sources.sh`, the local unit-test executables), say so in the PR, and merge.
8. **Needs-you items** (for example two-account validation, #12) need an action only the user can take.
   - Move the card to Needs you and continue with the next card.
   - They never stop the goal, and they are never marked done without the user.
9. **New work found:** open an issue, add it to the board with Stage set, and leave it in Todo. The 105-item movement checklist stays in `documentation/.../generated/movement-requirements.md`, not in issues.

### Board IDs

- **Project:** `PVT_kwHOAZkW5s4BnFAz`.
- **Status field** `PVTSSF_lAHOAZkW5s4BnFAzzhlrReY`:

  | Option | ID |
  |---|---|
  | Todo | `f75ad846` |
  | In Progress | `47fc9ee4` |
  | Needs you | `9786d219` |
  | Done | `98236657` |

- **Stage field** `PVTSSF_lAHOAZkW5s4BnFAzzhlrRvY`:

  | Stage | Option ID |
  |---|---|
  | 1 | `41365850` |
  | 2 | `efd15fd8` |
  | 3 | `15dbd8c1` |
  | 4 | `d4827ec3` |
  | 5 | `cc49928c` |
  | 6 | `c7ca9fa4` |
  | 7 | `1566a025` |

- **Set a field:** `gh project item-edit --id <item> --project-id <project> --field-id <field> --single-select-option-id <option>`.
- Merged PRs and closed issues move to Done automatically. The project index can lag a minute or two behind item-add.

### Tooling notes

- **gh:** `C:\Program Files\GitHub CLI\gh.exe` (2.102+). Call it by full path; shells started before the install don't have it on PATH.
  - Prompts are disabled.
  - The keyring token has the repo, workflow and project scopes.
- **Git sign-in:** git authenticates through gh's token (repo-local `credential.https://github.com.helper`), never through a Git Credential Manager window.
- **Editor:** `core.editor=true`, so no git command can wait on an editor.
- **Message text:**
  - Write commit and PR text to files with the Write tool, then pass them with `-F` or `--body-file`.
  - Windows PowerShell's `Set-Content -Encoding UTF8` adds a BOM to the subject.
  - Here-strings piped as `-F -` do not work.
- **Git Bash:** drop the leading `/` from `gh api` paths.
- **Do not:** enable GitHub auto-merge (a private repo on the free plan can't have required checks or rulesets, so it would merge at once), force-push `main`, or commit lab instances, game copies, binaries or videos (the manifest and hook enforce this).
