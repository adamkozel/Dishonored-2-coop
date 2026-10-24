#!/usr/bin/env bash
# 1894 (#197, #209): the partner-led Ramsey non-lethal take (routes/dunwall-nonlethal-partner-v1894.json, candidate
# 1214kd) repeated with new take letters until one passes; the fight decides whether Ramsey is off-balance near the
# partner, so a take may end without a knockout. Takes: route-dunwall-nonlethal-v1894<letter>.
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
for L in "$@"; do
  take="route-dunwall-nonlethal-v1894$L"
  if [ -e "$take" ]; then echo "$take exists, skipped"; continue; fi
  "$P" -NoProfile -File Route-Mission-v1883.ps1 -Take "$take" -Candidate candidate-npc-combat-v1214kd -StepsFile routes/dunwall-nonlethal-partner-v1894.json -LeadRole client > "$take.log" 2>&1
  r=$(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));print(r.get('passed'),r.get('failure'))" 2>/dev/null)
  echo "$take: $r"
  case "$r" in True*) break;; esac
done
