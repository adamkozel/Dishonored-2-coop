#!/usr/bin/env bash
# 2226 (#244, step 6): the 1.0.0 package and its local proofs, after the last lab take on candidate progress-v2226 (the lab
# game's DLL must be the candidate's; build_release_v2195.py checks it).
#  1. build: outputs/release/d2coop-v1.0.0/ and d2coop-v1.0.0.zip from candidate-progress-v2226, setup-tool-v2209,
#     decl-recipes-v3, and RELEASE-NOTES.md (the 1.0.0 notes).
#  2. install test on a scratch copy of the original game (Test-ReleaseInstall-v2214).
#  3. update test from 0.9.6 on a scratch copy (Test-ReleaseUpdate-v2190).
# Each test copies the whole original game (43 GB) and removes it afterwards; they run one after the other.
# Usage: Run-Release-v2226.sh
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
R="$(cd ../.. && pwd -W)"
head -1 RELEASE-NOTES.md | grep -q '^# Dishonored 2 Corvo/Emily co-op 1.0.0$' || { echo "RELEASE-NOTES.md is not the 1.0.0 notes"; exit 1; }
if [ -e ../../outputs/release/d2coop-v1.0.0.zip ]; then echo "package exists, build skipped"; else
 python -I build_release_v2195.py candidate-progress-v2226 setup-tool-v2209 > build-release-v2226.log 2>&1; echo "build: exit $? $(tail -1 build-release-v2226.log | head -c 400)"; fi
[ -e ../../outputs/release/d2coop-v1.0.0.zip ] || { echo "no package"; echo done; exit 1; }
"$P" -NoProfile -File Test-ReleaseInstall-v2214.ps1 -Take release-install-v2226a -Package "$R/outputs/release/d2coop-v1.0.0" > release-install-v2226a.log 2>&1
echo "install: exit $? $(tail -3 release-install-v2226a.log | tr '\n' ' ' | head -c 600)"
"$P" -NoProfile -File Test-ReleaseUpdate-v2190.ps1 -Take release-update-v2226a -Package "$R/outputs/release/d2coop-v1.0.0" > release-update-v2226a.log 2>&1
echo "update: exit $? $(tail -3 release-update-v2226a.log | tr '\n' ' ' | head -c 600)"
echo done
