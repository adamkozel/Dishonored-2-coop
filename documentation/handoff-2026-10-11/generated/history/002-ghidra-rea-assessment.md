# Historical note: outputs/d2-coop-lab/analysis/ghidra/rea-assessment.md

Original: [outputs/d2-coop-lab/analysis/ghidra/rea-assessment.md](<../../../../outputs/d2-coop-lab/analysis/ghidra/rea-assessment.md>)

SHA-256: `C4BC981376A0D2DCD43F96530A1ABD04CB4F7B0A336EE609FF035A1364593FB0`; 1159 bytes.

````markdown
# REA tool assessment
Inspected 2026-10-09 for the Doppelganger work, following the user's optional suggestion.
Sources: https://github.com/morluto/rea and https://github.com/morluto/rea/blob/main/docs/windows-ghidra-p0.md

REA's documented Windows boundary supports x64 PE applications with Ghidra 12.1.x (verified upstream with 12.1.4) and JDK21+. This fits our installed engine. It exposes 25 read-only Ghidra analysis operations using its own headless snapshot/session. GUI state and annotations are outside that Windows boundary; runtime process capture is documented for Linux/macOS.

Decision: defer installation for this blocker. Existing d2-ghidra MCP already supplies live function lookup, cross-references, decompilation, inline scripts, persistent naming/types and project saves, alongside the lab's runtime probes and exception stacks. REA would currently duplicate static reads and start another Ghidra import without adding Windows runtime tracing or preserving our active project annotations. Keep it as a possible independent-read/checking backend if that need arises. No REA setup, MCP registration, global install or restart was performed.

````
