#!/usr/bin/env bash
# 2056 (#240): the join snapshot on candidate joinsnapshot-v2055. Route as join-measure-dunwall-v2055a (the host kills and
# loots Ramsey alone, the VM partner joins after it), with the world diff 60 s after the join required.
# a: the take; b: the mutation twin, coop_join_snapshot off on both games before the join (must be red).
# 2058: candidate joinhold-v2058 (joinquiet-v2057: partner-first runs after 10 s of gameplay; the host log held until the
# partner first reaches plain gameplay). v2058c: the take again (v2058a stopped on the Ramsey loot step before the join).
# Usage: Run-JoinSnapshot-v2058c.sh. Result: join-snapshot-dunwall-v2058c
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
run(){
 take="$1";steps="$2";join="$3"
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 "$P" -NoProfile -File Route-MissionVM-v2054.ps1 -Take "$take" -MapInfo campaign/dunwall/escape/tower/dunwall_escape_tower.mapinfo -Candidate candidate-joinhold-v2058 -StepsFile "$steps" -JoinAfterStep "$join" -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'),n.get('diff-join'),n.get('diff-join60'),n.get('worldDiff'))" 2>&1 | head -c 900)"
}
run join-snapshot-dunwall-v2058c routes/join-snapshot-dunwall-v2056.json 7
echo done
