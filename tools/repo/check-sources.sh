#!/bin/sh
# Build inputs are committed: every source named in an add_executable/add_library
# of outputs/d2-coop-lab/CMakeLists.txt, and every quoted #include in its src/,
# resolves to a tracked file. Catches a file left out of a commit, which local
# candidate builds cannot see. (CI builds only the test targets on Windows; the
# mod DLL is built locally for every candidate.)
cd "$(git rev-parse --show-toplevel)" || exit 2
lab=outputs/d2-coop-lab
tracked=$(mktemp) || exit 2
trap 'rm -f "$tracked"' EXIT
git ls-files "$lab" > "$tracked"
{
  grep -E '^[[:space:]]*add_(executable|library)\(' "$lab/CMakeLists.txt" | grep -oE '[A-Za-z0-9_./-]+\.(cpp|cc|c|h|hpp)' | sed 's/^/S /'
  git grep -h -o -E '^[[:space:]]*#[[:space:]]*include[[:space:]]*"[^"]+"' -- "$lab/src" | sed -E 's/.*"([^"]+)"/I \1/' | sort -u
} | awk -v lab="$lab" '
  NR == FNR { t[$0] = 1; next }
  $1 == "S" { if (!((lab "/" $2) in t)) { print "check-sources: missing build source " lab "/" $2; bad = 1 } next }
  $1 == "I" { if (!((lab "/src/" $2) in t) && !((lab "/third_party/valve/" $2) in t)) { print "check-sources: unresolved include \"" $2 "\""; bad = 1 } }
  END { exit bad ? 1 : 0 }' "$tracked" - >&2
