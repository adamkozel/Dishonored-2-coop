#!/usr/bin/env bash
# 1922 (#230): after chain_1921.sh ends, the whole power effect matrix on 1214ko with Route-MissionVM-v1921 (per-power
# aim, role-aware Bend Time, every miss listed at the end): Corvo host with Emily partner, then Emily host with Corvo
# partner.
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
while ps -ef | grep -v grep | grep -q "chain_1921.sh"; do sleep 20; done
for t in powerfx-corvohost-vm-v1921a powerfx-emilyhost-vm-v1921a; do if [ -e "$t" ]; then echo "$t exists"; exit 1; fi; done
echo "matrix corvo host $(date +%H:%M)"
"$P" -NoProfile -File Route-MissionVM-v1921.ps1 -Take powerfx-corvohost-vm-v1921a -MapInfo campaign/void/void_01/void_01.mapinfo -Candidate candidate-npc-combat-v1214ko -StepsFile routes/powerfx-corvohost-vm-v1921.json -CoopSaves > powerfx-corvohost-vm-v1921a.log 2>&1
echo "corvo host exit $? $(python -c "import json;r=json.load(open('powerfx-corvohost-vm-v1921a/result.json',encoding='utf-8-sig'));print(r.get('passed'),r.get('failure'))" 2>/dev/null)"
echo "matrix emily host $(date +%H:%M)"
"$P" -NoProfile -File Route-MissionVM-v1921.ps1 -Take powerfx-emilyhost-vm-v1921a -MapInfo campaign/void/void_01/void_01.mapinfo -Candidate candidate-npc-combat-v1214ko -StepsFile routes/powerfx-emilyhost-vm-v1921.json -CoopSaves -HostCharacter emily -PartnerCharacter corvo > powerfx-emilyhost-vm-v1921a.log 2>&1
echo "emily host exit $? $(python -c "import json;r=json.load(open('powerfx-emilyhost-vm-v1921a/result.json',encoding='utf-8-sig'));print(r.get('passed'),r.get('failure'))" 2>/dev/null)"
echo "chain done $(date +%H:%M)"
