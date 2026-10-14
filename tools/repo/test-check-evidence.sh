#!/bin/sh
# Self-test for check-evidence.sh in a throwaway repository (no real evidence touched).
# Usage: sh tools/repo/test-check-evidence.sh
check="$(cd "$(dirname "$0")" && pwd)/check-evidence.sh"
dir=$(mktemp -d) || exit 2
trap 'rm -rf "$dir"' EXIT
cd "$dir" && git init -q -b main && git config user.name test && git config user.email test@example.invalid && git config core.autocrlf false || exit 2
mkdir -p "work/natural-moveset/take-v1/sub" work/natural-moveset/take-v2 "work/natural-moveset/take with space" \
  work/movement-integration/candidate-x-v1 work/release src
echo '{}' > work/natural-moveset/take-v1/result.json; echo a > work/natural-moveset/take-v1/samples.json
echo a > work/natural-moveset/take-v1/sub/note.json
echo 'x' > work/natural-moveset/take-v2/run-script.ps1; echo a > work/natural-moveset/take-v2/failure.json
echo '{}' > "work/natural-moveset/take with space/result.json"; echo a > "work/natural-moveset/take with space/notes.json"
echo a > work/movement-integration/candidate-x-v1/Build.ps1
echo a > work/release/air-result.json; echo a > work/natural-moveset/Harness-v1.ps1; echo a > src/code.h
git add -A && git commit -q -m base || exit 2
fails=0
# expect NAME WANT(0 pass / 1 fail) [ENV=VALUE]: stage the tree, run the staged mode, commit it on a
# pr branch, run the BASE HEAD mode against main, then drop the branch.
expect() {
  git add -A
  if [ -n "$3" ]; then env "$3" sh "$check" --staged 2>/dev/null; else sh "$check" --staged 2>/dev/null; fi; got=$?
  git switch -q -c pr && git commit -q -m "$1" --allow-empty
  if [ -n "$3" ]; then env "$3" sh "$check" main HEAD 2>/dev/null; else sh "$check" main HEAD 2>/dev/null; fi; got2=$?
  if [ "$got" -eq "$2" ] && [ "$got2" -eq "$2" ]; then echo "ok   $1"; else echo "FAIL $1 (staged $got, range $got2, want $2)"; fails=1; fi
  git reset -q --hard main && git switch -q main && git branch -q -D pr
}
echo b > work/natural-moveset/new-take-v3.json; mkdir -p work/natural-moveset/take-v3; echo '{}' > work/natural-moveset/take-v3/result.json
expect "new take folder is allowed" 0
echo a > work/natural-moveset/take-v1/review.json;           expect "new file in an existing take is allowed" 0
echo b > work/natural-moveset/Harness-v1.ps1; echo b > src/code.h; echo b > work/release/air-result.json
expect "non-evidence edits are allowed (incl. *-result.json outside takes)" 0
echo b > work/natural-moveset/take-v1/samples.json;          expect "modifying a take file fails" 1
echo b > work/natural-moveset/take-v1/result.json;           expect "modifying result.json fails" 1
echo b > work/natural-moveset/take-v1/sub/note.json;         expect "modifying a take subfolder file fails" 1
rm work/natural-moveset/take-v2/failure.json;                expect "deleting from a run-script take fails" 1
git mv work/natural-moveset/take-v1 work/natural-moveset/take-v1-moved; expect "renaming a take fails" 1
echo b > "work/natural-moveset/take with space/notes.json";  expect "paths with spaces are checked" 1
echo b > work/movement-integration/candidate-x-v1/Build.ps1; expect "modifying a candidate archive fails" 1
echo b > work/natural-moveset/take-v1/samples.json;          expect "EVIDENCE_AMEND=1 allows an amendment" 0 EVIDENCE_AMEND=1
[ "$fails" -eq 0 ] && echo "check-evidence self-test passed"
exit $fails
