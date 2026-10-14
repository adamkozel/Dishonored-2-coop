# Historical note: work/tool-evaluation-2026-10-12/void-slice/kanban/goals/M7.md

Original: [work/tool-evaluation-2026-10-12/void-slice/kanban/goals/M7.md](<../../../../work/tool-evaluation-2026-10-12/void-slice/kanban/goals/M7.md>)

SHA-256: `547656663BC2EBBA00D074AA6AE1E588BD29B1D6AE7BC5ABE92969E9491632AC`; 1012 bytes.

````markdown
# M7 — Production hardening

M6 made deploys *safe* — every change goes through a PR, every PR has a preview, `main` is branch-protected, production only updates on merge. M7 makes them *robust*: bound the linter's worst-case diagnostic blow-up so a pathological input can't OOM the Worker, lock the API down to its own front-end origin, harden the static-asset response headers, smoke-test each prod deploy and roll back on failure, cache repeat lints, keep `compatibility_date` intentional, and forward Workers logs somewhere durable. Inputs are the deferred items captured in `cloudflare.md` § 3 before that file was retired at the close of [M6](M6.md).

**Scope:** [worker/index.js](../../worker/index.js), [wrangler.toml](../../wrangler.toml), [.github/workflows/ci.yml](../../.github/workflows/ci.yml), `web/public/_headers` (new), and [internal/parse/](../../internal/parse/) (diagnostic-count cap). Excludes feature work on the linter itself (M4) and any LSP work (M3).

**Status:** planned.

````
