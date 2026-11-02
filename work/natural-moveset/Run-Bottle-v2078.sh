#!/usr/bin/env bash
# 2078 (#260): env_bottle_04_19 was absent on the host at the Edge of the World spawn in 3 of 4 takes. Candidate
# fragile-v2078 (prop rest leaves breakables alone; the host sends a vanished breakable without a noted hit). Three takes
# with the spawn diff and a diff 30 s later required; .prop-rest.json and .breakable-sync.json kept.
# Usage: Run-Bottle-v2078.sh. Results: bottle-eotw-v2078a, -b, -c
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
for L in a b c; do
 take=bottle-eotw-v2078$L
 if [ -e "$take" ]; then echo "$take exists, skipped";continue;fi
 "$P" -NoProfile -File Route-MissionVM-v2074.ps1 -Take "$take" -MapInfo campaign/edge_of_the_world/eotw.mapinfo -Candidate candidate-fragile-v2078 -StepsFile routes/bottle-eotw-v2078.json -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'),n.get('diff-spawn'),n.get('diff-later'))" 2>&1 | head -c 600)"
done
echo done
