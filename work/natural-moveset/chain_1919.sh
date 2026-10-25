#!/usr/bin/env bash
# 1919 (#230): after chain_1917d.sh ends, build and deploy candidate 1214kn (each power-effect refusal named), then the
# power effect matrix on two accounts with Route-MissionVM-v1919: Corvo host with Emily partner, then Emily host with
# Corvo partner (Edge of the World, in Karnaca after the skiff). Each stage stops the chain when it fails.
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
M="$(cd ../movement-integration && pwd)"
while ps -ef | grep -v grep | grep -q "chain_1917d.sh"; do sleep 30; done
echo "build $(date +%H:%M)"
"$P" -NoProfile -File "$M/Build-Candidate-v1214kn.ps1" > "$M/candidate-run-v1214kn.log" 2>&1 || { echo "build failed"; exit 1; }
echo "deploy $(date +%H:%M)"
"$P" -NoProfile -File "$M/Deploy-Candidate-v1214kn.ps1" > "$M/deploy-run-v1214kn.log" 2>&1 || { echo "deploy failed"; exit 1; }
for t in powerfx-corvohost-vm-v1919a powerfx-emilyhost-vm-v1919a; do if [ -e "$t" ]; then echo "$t exists"; exit 1; fi; done
echo "matrix corvo host $(date +%H:%M)"
"$P" -NoProfile -File Route-MissionVM-v1919.ps1 -Take powerfx-corvohost-vm-v1919a -MapInfo campaign/void/void_01/void_01.mapinfo -Candidate candidate-npc-combat-v1214kn -StepsFile routes/powerfx-corvohost-vm-v1919.json -CoopSaves > powerfx-corvohost-vm-v1919a.log 2>&1
echo "corvo host exit $? $(python -c "import json;r=json.load(open('powerfx-corvohost-vm-v1919a/result.json',encoding='utf-8-sig'));print(r.get('passed'),r.get('failure'))" 2>/dev/null)"
echo "matrix emily host $(date +%H:%M)"
"$P" -NoProfile -File Route-MissionVM-v1919.ps1 -Take powerfx-emilyhost-vm-v1919a -MapInfo campaign/void/void_01/void_01.mapinfo -Candidate candidate-npc-combat-v1214kn -StepsFile routes/powerfx-emilyhost-vm-v1919.json -CoopSaves -HostCharacter emily -PartnerCharacter corvo > powerfx-emilyhost-vm-v1919a.log 2>&1
echo "emily host exit $? $(python -c "import json;r=json.load(open('powerfx-emilyhost-vm-v1919a/result.json',encoding='utf-8-sig'));print(r.get('passed'),r.get('failure'))" 2>/dev/null)"
