#!/usr/bin/env bash
# 2194 (#261): the partner's Deadly Shade again on candidate progress-v2192, harness v2183 (Corvo host, Emily partner,
# Edge of the World). deadlyshade-eotw-v2188a/b stopped at "No partner NPC matches addermirestation_entrance_guard_02":
# the partnerface step now looks the guard up in the partner's entity list, where it is
# guard_male_small_01.def_<n> (v2079a faced it at 58.7, 24.0), not the spawner name of the older NPC-activity lookup.
# The routes name guard_male_small near that spot. a: the take (the host's puppet is deadlyshade_puppet.def:
# .shade-host.json deadlyPuppets >= 1); b: the twin, host coop_shade_native puppet_off (the plain decoy).
# c: corpse possession measured (route corpse-eotw-v2189, Corvo host and partner; corpse-eotw-v2189c stopped at the same
#    spawner-name lookup in its attack step, now guard_male_small near 58.7, 24.0).
# 2193: on candidate progress-v2193 (her own Continue first, then the host map). m: The Clockwork Mansion (her own
#    Continue reaches the host map; campaign-madinventor-vm-v2191m lost node bf086e4f by following), n: A Crack in the Slab
#    (her own Continue reloads Dust District, so she follows the host to the Grand Palace).
# 2194: candidate progress-v2194 (the boat audiograph counterpart): k The Royal Conservatory with a Corvo host and an Emily
#    partner, q with an Emily host and a Corvo partner (campaign-curator-vm-v2191k, -v2190c: the partner kept neither audiograph).
# Usage: Run-V2194S.sh [a b c m n k q]
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
CAND=candidate-progress-v2194
echo "candidate $CAND"
cp "../stage8/$CAND/dinput8.dll" "../stage8/$CAND/d2_runtime_probe.exe" ../../outputs/d2-coop-lab/bin/
"$P" -NoProfile -File ../movement-integration/Deploy-SourceClock.ps1 > "shade-v2194-deploy.log" 2>&1 || { echo "deploy failed"; exit 1; }
clean(){
 rm -f ../lab-instances/host/base/savegame_coop/coop-progress-*.txt
 "$P" -NoProfile -Command '. ../../tools/vm/D2LabVM.Common.ps1; . ../../tools/vm/D2LabVM.Lab.ps1; Invoke-D2LabVM { Remove-Item C:/D2Lab/ws/work/lab-instances/client/base/savegame_coop/coop-progress-*.txt -ErrorAction SilentlyContinue }' > /dev/null 2>&1
}
run(){
 take="$1";steps="$2"
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 clean
 "$P" -NoProfile -File Route-MissionVM-v2183.ps1 -Take "$take" -MapInfo "${MAPINFO:-campaign/edge_of_the_world/eotw.mapinfo}" -Candidate "$CAND" -StepsFile "$steps" -HostCharacter "${HC:-corvo}" -PartnerCharacter "${PC:-emily}" -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'));[print(k,v) for k,v in n.items() if k.startswith('diff-')]" 2>&1 | head -c 900)"
}
for x in ${*:-a b c m n k q}; do
 case $x in
  a) run deadlyshade-eotw-v2194a routes/deadlyshade-eotw-v2188.json;;
  b) run deadlyshade-eotw-v2194b routes/deadlyshade-eotw-puppetoff-v2188.json;;
  c) HC=corvo PC=corvo run corpse-eotw-v2194c routes/corpse-eotw-v2189.json;;
  m) MAPINFO=campaign/boat/madinventor/boat_madinventor.mapinfo run campaign-madinventor-vm-v2194m routes/campaign-madinventor-vm-v2038.json;;
  n) MAPINFO=campaign/bunker/bunker.mapinfo run campaign-bunker-vm-v2194n routes/campaign-bunker-vm-v1926.json;;
  k) MAPINFO=campaign/void/void_02/void_02.mapinfo run campaign-curator-vm-v2194k routes/campaign-curator-vm-v2041.json;;
  q) HC=emily PC=corvo MAPINFO=campaign/void/void_02/void_02.mapinfo run campaign-curator-vm-v2194q routes/campaign-curator-vm-v2041.json;;
 esac
done
clean
echo done
