# Historical note: work/tool-evaluation-2026-10-12/void-slice/kanban/goals/M3.md

Original: [work/tool-evaluation-2026-10-12/void-slice/kanban/goals/M3.md](<../../../../work/tool-evaluation-2026-10-12/void-slice/kanban/goals/M3.md>)

SHA-256: `25E1CA973758F401489C96EE422AB7982D08BA877252F8C4C5E0FC16F040EB6F`; 879 bytes.

````markdown
# M3 — Editor integration (LSP)

Make void-slice usable as you type. The CLI (M1) and the playground (M2) both require a paste-or-run loop; this goal closes that loop by feeding the linter into the editor's diagnostics panel over the Language Server Protocol. VS Code is the reference client. Success looks like: open a `.decl` file in any LSP-capable editor, see lint squiggles inline, no manual invocation.

**Scope:** `internal/lsp/`, `voidslice-vscode/`, integration tests harnessing the LSP subprocess. Excludes hover, go-to-definition, completions — diagnostics-only is the deliberate floor for this goal.

**Status:** active — JSON-RPC server, subprocess integration tests, and a buildable VS Code extension shipped. Remaining work: editor-setup docs for Neovim/Helix/Zed, VS Code e2e smoke test, server stderr logging, release pipeline for binary + `.vsix`.

````
