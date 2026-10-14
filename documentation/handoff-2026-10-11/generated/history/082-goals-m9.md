# Historical note: work/tool-evaluation-2026-10-12/void-slice/kanban/goals/M9.md

Original: [work/tool-evaluation-2026-10-12/void-slice/kanban/goals/M9.md](<../../../../work/tool-evaluation-2026-10-12/void-slice/kanban/goals/M9.md>)

SHA-256: `36ACD7ADA157F4BA292AF4E15FF8B12E67BCD3B41D5123AC8B24BEC1313EF1BD`; 1800 bytes.

````markdown
# M9 — Concurrent agent workflow

Multiple Claude Code agents serialize today because tickets share `m<N>-dev` integration branches; two `/implement-ticket` runs in parallel produce conflict-laden merges. Flip the model: a goal owns a branch and a PR, tickets target the goal branch, and the goal lands to `main` as a single human-reviewed unit. The goal becomes the unit of parallelism — agents on different goals provably don't collide because the dispatcher refuses to spawn goals whose declared `Paths:` overlap. State lives in durable artifacts (ticket file, PR, CI status), so a 24-hour body of work runs as many short stateless sessions, not one long context-bound deliberation. The unit of value is **fan-out without coordination**: several `/dispatch-goal` calls produce several green PRs without human babysitting, and `main` is provably unbreakable by an agent acting in good faith.

**Scope:** kanban schema (`Paths:` at goal level, surfaced at `/goal-define`), dispatcher and orchestration scripts (`tools/`, `.claude/commands/`), `.github/` branch-protection-as-code, and the agent's CI-feedback loop (read checks → fix → push). Excludes unsupervised merge to `main` (human-gated for v1; branch protection is the hard backstop), per-ticket branches and stacked PRs (superseded by goal-as-unit), cross-goal scheduling beyond overlap *surfacing* (no priority queues, dependency graphs, auto-rebase), and pre-CI logical/semantic conflict detection beyond what `Paths:` catches. Adjacent to [M5](M5.md), which owned dev tooling broadly and has now closed; this is the focused parallel-agent slice.

**Paths:**
- .claude/commands/**
- .claude/plans/**
- .github/**
- kanban/README.md
- kanban/goals/M9.md
- kanban/todo/M9.*
- tools/**
- CLAUDE.md

**Status:** done.

````
