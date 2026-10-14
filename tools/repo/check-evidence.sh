#!/bin/sh
# Evidence guard: recorded takes and candidate archives are append-only.
# A take folder is any directory holding result.json or run-script.ps1. It and
# everything below it, plus work/movement-integration/candidate-*/, may gain new
# files, but existing files may not be modified, deleted or renamed.
#   check-evidence.sh --staged     index against HEAD (pre-commit)
#   check-evidence.sh BASE HEAD    merge-base(BASE, HEAD) against HEAD (PR check)
# EVIDENCE_AMEND=1 reports violations without failing: deliberate, explained
# amendments only (e.g. a privacy scrub). CI sets it from the evidence-amend label.
tmp=$(mktemp) || exit 2
trap 'rm -f "$tmp" "$tmp.c" "$tmp.v"' EXIT
if [ "$1" = "--staged" ]; then
  base=$(git rev-parse -q --verify HEAD) || exit 0
  git -c core.quotepath=off diff --cached --name-status --no-renames > "$tmp.c" || exit 2
elif [ $# -eq 2 ]; then
  base=$(git merge-base "$1" "$2") || exit 2
  git -c core.quotepath=off diff --name-status --no-renames "$base" "$2" > "$tmp.c" || exit 2
else
  echo "usage: check-evidence.sh --staged | BASE HEAD" >&2; exit 2
fi
[ -s "$tmp.c" ] || exit 0
git -c core.quotepath=off ls-tree -r --name-only "$base" > "$tmp" || exit 2
if awk -F '\t' '
  NR == FNR { n = split($0, p, "/"); f = p[n]
    if (n > 1 && (f == "result.json" || f == "run-script.ps1")) take[substr($0, 1, length($0) - length(f) - 1)] = 1
    next }
  $1 == "A" { next }
  { path = $2; frozen = path ~ /^work\/movement-integration\/candidate-[^\/]+\//
    n = split(path, p, "/"); s = ""
    for (i = 1; i < n && !frozen; i++) { s = (i == 1 ? p[i] : s "/" p[i]); if (s in take) frozen = 1 }
    if (frozen) { print "  " $1 "  " path; bad = 1 } }
  END { exit bad ? 1 : 0 }' "$tmp" "$tmp.c" > "$tmp.v"; then
  exit 0
fi
echo "check-evidence: these files belong to recorded takes or candidate archives, which are append-only:" >&2
cat "$tmp.v" >&2
if [ "${EVIDENCE_AMEND:-0}" = 1 ]; then
  echo "check-evidence: allowed by EVIDENCE_AMEND=1 (deliberate amendment)" >&2; exit 0
fi
echo "Record new results in a new take folder. A deliberate amendment (e.g. a privacy scrub) needs EVIDENCE_AMEND=1 locally, the evidence-amend label on the PR, and the reason in the PR description." >&2
exit 1
