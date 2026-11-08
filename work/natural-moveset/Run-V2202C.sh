#!/usr/bin/env bash
# 2202: candidate progress-v2200 (the lab corpse command takes an unconscious body too) and route corpse-eotw-v2200 (the
# partner stand at eye height 20.6: the partner step sets the view, and v2197c and v2199c fell from z 19.0, the floor).
# 2199 (#261, #260): corpse possession measured again on candidate progress-v2198, harness Route-MissionVM-v2184 (Corvo host and
# Corvo partner, Edge of the World), route corpse-eotw-v2198: the host kills entrance guard 02 and the route waits 25 s
# (corpse-eotw-v2197c: no corpse in 15 m for 20 s, then possessable at 1.2 m); the host probes and casts at the corpse
# (coop_lab_possess_corpse) and walks; then the partner, at the shade test's stand 5.7 m from it (v2197c fell through the
# floor 2 m from it), probes and casts. The killed and later diffs show whether the partner still spawns a guard or
# reinforcements of her own (progress-v2198: her own corpse counts, her own spawn leaves when the host stops announcing).
# Usage: Run-V2199C.sh
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
CAND=candidate-progress-v2200
take=corpse-eotw-v2202c
if [ -e "$take" ]; then echo "$take exists, skipped"; echo done; exit 0; fi
echo "candidate $CAND"
cp "../stage8/$CAND/dinput8.dll" "../stage8/$CAND/d2_runtime_probe.exe" ../../outputs/d2-coop-lab/bin/
"$P" -NoProfile -File ../movement-integration/Deploy-SourceClock.ps1 > "corpse-v2202-deploy.log" 2>&1 || { echo "deploy failed"; exit 1; }
rm -f ../lab-instances/host/base/savegame_coop/coop-progress-*.txt
"$P" -NoProfile -Command '. ../../tools/vm/D2LabVM.Common.ps1; . ../../tools/vm/D2LabVM.Lab.ps1; Invoke-D2LabVM { Remove-Item C:/D2Lab/ws/work/lab-instances/client/base/savegame_coop/coop-progress-*.txt -ErrorAction SilentlyContinue }' > /dev/null 2>&1
"$P" -NoProfile -File Route-MissionVM-v2184.ps1 -Take "$take" -MapInfo campaign/edge_of_the_world/eotw.mapinfo -Candidate "$CAND" -StepsFile routes/corpse-eotw-v2200.json -HostCharacter corvo -PartnerCharacter corvo -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
echo "$take: exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),str(r.get('failure'))[:300],[(k,(v or {}).get('status') if isinstance(v,dict) else v) for k,v in n.items() if k.startswith('diff-')])" 2>&1 | head -c 900)"
echo done
