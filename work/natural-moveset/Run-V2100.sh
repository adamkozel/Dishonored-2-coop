#!/usr/bin/env bash
# 2100 (#243, #264): candidate progress-v2100 on two Steam accounts.
# notice-eotw-v2100a: the co-op notice, now with its own text (v2097a showed the box reading "Character"). Visual
# acceptance from the frames step04-press-box1-host and step11-press-box2-vm; Check-Notice-v2097.ps1 counts only
# this take's log lines.
# firstmission-dunwall-v2100a / b (K3): the first mission, host Corvo / partner Emily. a turns coop_usable_safe_doors
# on in both games (the default), b, the twin, turns it off; both routes are the v2095 route with the two commands
# after its first wait. a must have the saferoom, balcony and streets diffs equal; b must show the three tower
# safe-room doors again.
# Usage: Run-V2100.sh. Result: notice-eotw-v2100a, firstmission-dunwall-v2100a, firstmission-dunwall-v2100b
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
CAND=candidate-progress-v2100
cp "../stage8/$CAND/dinput8.dll" "../stage8/$CAND/d2_runtime_probe.exe" ../../outputs/d2-coop-lab/bin/
"$P" -NoProfile -File ../movement-integration/Deploy-SourceClock.ps1 > v2100-deploy.log 2>&1 || { echo "deploy failed"; exit 1; }
# The partner's progress files are lab state; every take starts and ends without them.
clean(){
 rm -f ../lab-instances/host/base/savegame_coop/coop-progress-*.txt
 "$P" -NoProfile -Command '. ../../tools/vm/D2LabVM.Common.ps1; . ../../tools/vm/D2LabVM.Lab.ps1; Invoke-D2LabVM { Remove-Item C:/D2Lab/ws/work/lab-instances/client/base/savegame_coop/coop-progress-*.txt -ErrorAction SilentlyContinue }' > /dev/null 2>&1
}
notice(){
 take="$1"
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 clean
 hf=$(wc -l < ../lab-instances/host/engine.log)
 vf=$("$P" -NoProfile -Command ". ../../tools/vm/D2LabVM.Common.ps1;. ../../tools/vm/D2LabVM.Lab.ps1;Invoke-D2LabVM {\$p='C:\D2Lab\ws\work\lab-instances\client\engine.log';if(Test-Path \$p){@(Get-Content \$p).Count}else{0}}" 2>/dev/null | tail -1 | tr -d '\r')
 "$P" -NoProfile -File Route-MissionVM-v2099.ps1 -Take "$take" -MapInfo campaign/edge_of_the_world/eotw.mapinfo -Candidate "$CAND" -StepsFile routes/notice-eotw-v2097.json -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 echo "$take exit $? from host $hf vm $vf $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));print(r.get('passed'),r.get('failure'))" 2>&1 | head -c 600)"
 "$P" -NoProfile -File Check-Notice-v2097.ps1 -Take "$take" -HostFrom "${hf:-0}" -VmFrom "${vf:-0}" 2>&1 | tail -c 1500
}
mission(){
 take="$1";steps="$2"
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 clean
 "$P" -NoProfile -File Route-MissionVM-v2099.ps1 -Take "$take" -MapInfo campaign/dunwall/escape/tower/dunwall_escape_tower.mapinfo -Candidate "$CAND" -StepsFile "$steps" -HostCharacter corvo -HostChoice Left -PartnerCharacter emily -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'));[print(k,v) for k,v in n.items() if k.startswith('diff-')]" 2>&1 | head -c 2500)"
 for t in saferoom balcony streets; do [ -f "$take/diff-$t.txt" ] && { echo "$t:"; head -6 "$take/diff-$t.txt"; }; done
}
notice notice-eotw-v2100a
mission firstmission-dunwall-v2100a routes/firstmission-safedoors-v2100.json
mission firstmission-dunwall-v2100b routes/firstmission-safedoors-twin-v2100.json
clean
echo done
