#!/usr/bin/env bash
# 2060 (#240): the join on candidate joinasks-v2060 (partner-first runs and host story asks wait for 10 s of plain
# gameplay; the host log is held while the partner loads) with harness Route-MissionVM-v2059 (probe step). Route
# join-probe-dunwall-v2059: the join route of v2056 plus --npc-authority-all on the host before the join and on both at the end,
# to see what the host's throne room actors are ([1, 0, 0] on the host, absent for the partner in v2056a).
# a: the take; b: the mutation twin, coop_join_snapshot off on both games before the join.
# 2061: candidate joinanswer-v2061 (a joiner's host story asks are answered by the host without running them).
# Usage: Run-JoinProbe-v2061.sh. Results: join-probe-dunwall-v2061a, -v2061b
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
run(){
 take="$1";steps="$2";join="$3"
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 "$P" -NoProfile -File Route-MissionVM-v2059.ps1 -Take "$take" -MapInfo campaign/dunwall/escape/tower/dunwall_escape_tower.mapinfo -Candidate candidate-joinanswer-v2061 -StepsFile "$steps" -JoinAfterStep "$join" -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'),n.get('diff-join'),n.get('diff-join60'),n.get('worldDiff'))" 2>&1 | head -c 900)"
}
run join-probe-dunwall-v2061a routes/join-probe-dunwall-v2059.json 8
python - <<'PY'
import json
r=json.load(open('routes/join-probe-dunwall-v2059.json'))
json.dump([{"do":"command","role":"host","text":"coop_join_snapshot off"},{"do":"command","role":"client","text":"coop_join_snapshot off"}]+r,open('routes/join-probe-dunwall-off-v2059.json','w'),indent=1)
PY
run join-probe-dunwall-v2061b routes/join-probe-dunwall-off-v2059.json 10
echo done
