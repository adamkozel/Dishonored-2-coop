# Historical note: work/tool-evaluation-2026-10-12/void-slice/kanban/goals/M1.md

Original: [work/tool-evaluation-2026-10-12/void-slice/kanban/goals/M1.md](<../../../../work/tool-evaluation-2026-10-12/void-slice/kanban/goals/M1.md>)

SHA-256: `4F22CDA9965F2978038B0415CBC369798AA7CAB3A3A83DE4912F01AC50B8A6AC`; 647 bytes.

````markdown
# M1 — Linter engine + CLI

The reason void-slice exists. Tokenize, parse, and validate Dishonored 2 / DOTO text-archive files (`.entities`, `.decl`, `.entitydef`) and surface diagnostics from a single CLI binary. Success looks like: a modder can run `voidslice lint <file>` and get useful, actionable lint output without setting up an editor or a server.

**Scope:** `internal/scan/`, `internal/parse/`, `internal/validate/`, `internal/report/`, `internal/lint/`, `cmd/voidslice/`. Anything that runs the linter as a CLI process. Excludes the HTTP server (M2), the LSP server (M3), and rule-set policy decisions (M4).

**Status:** done.

````
