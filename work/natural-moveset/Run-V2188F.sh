#!/usr/bin/env bash
# 2188 (#261): the possessed fish and the partner's Deadly Shade on candidate progress-v2188, two Steam accounts, Edge of
# the World, harness Route-MissionVM-v2182. First the new decls go into the lab game (Install-PowerVisual-v2188.ps1), the
# lab instances restart (Deploy-SourceClock.ps1) and the VM gets the lab's voidmod and master.index (full workspace sync).
# Takes (each judged on its status files; the spawn and end diffs are required):
#  labfish-eotw-v2188a  the partner's fish bit (coop_lab_animal fish): the host hides her and shows the cosmetic fish at her
#                       root (possessedAnimal fishResult shown, fishCreated 1), then hides it; t: twin, host coop_possess_animal off.
#  labfish-eotw-v2188r  the rat again (route labanimal-eotw-v2180), so the shared service still shows the rat.
#  deadlyshade-eotw-v2188a  Corvo host, Emily partner casts Deadly Shade: the host's puppet is deadlyshade_puppet.def
#                       (.shade-host.json deadlyPuppets >= 1, deadlyOwnDefinition 0); b: twin puppet_off (v2079's decoy);
#                       e: Emily host (the host's own clone definition is the deadly shadow, the 2077/2078 crash path).
#  corpse-eotw-v2189c  2189: corpse possession measured. The host kills entrance guard 02, gets Possession with
#                       possession_decayed_harbor, probes the corpse (.corpse-possession.json, both worlds) and casts at it
#                       with coop_lab_possess_corpse, walks 4 s; then the partner tries the same on her copy of the corpse.
# The candidate is progress-v2189 (progress-v2188 plus the corpse lab commands), so every take here runs on v2189.
# Usage: Run-V2188F.sh [takes...]   (default: install a t r sa sb se c)
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
# 2190: the candidate is progress-v2190 (v2189 plus every finished mission state written for a following partner); m re-runs
# The Clockwork Mansion on it (campaign-madinventor-vm-v2181b ended with node bf086e4f host 4, partner 0).
# 2191: candidate progress-v2191 (her own Continue when her world finished the host missions); k re-runs The Royal
# Conservatory (campaign-curator-vm-v2181b: the follow load lacked the boat state).
CAND=candidate-progress-v2192
clean(){
 rm -f ../lab-instances/host/base/savegame_coop/coop-progress-*.txt
 "$P" -NoProfile -Command '. ../../tools/vm/D2LabVM.Common.ps1; . ../../tools/vm/D2LabVM.Lab.ps1; Invoke-D2LabVM { Remove-Item C:/D2Lab/ws/work/lab-instances/client/base/savegame_coop/coop-progress-*.txt -ErrorAction SilentlyContinue }' > /dev/null 2>&1
}
install(){
 echo "candidate $CAND"
 cp "../stage8/$CAND/dinput8.dll" "../stage8/$CAND/d2_runtime_probe.exe" ../../outputs/d2-coop-lab/bin/
 "$P" -NoProfile -File ../stage8/Install-PowerVisual-v2188.ps1 > power-visual-v2188-install.log 2>&1 || { echo "asset install failed"; tail -5 power-visual-v2188-install.log; exit 1; }
 "$P" -NoProfile -File ../movement-integration/Deploy-SourceClock.ps1 > power-visual-v2188-deploy.log 2>&1 || { echo "deploy failed"; exit 1; }
 "$P" -NoProfile -File ../../tools/vm/Sync-D2LabVMWorkspace.ps1 -Role client > power-visual-v2188-vmsync.log 2>&1 || { echo "vm sync failed"; tail -5 power-visual-v2188-vmsync.log; exit 1; }
 echo "installed"
}
run(){
 take="$1";steps="$2";hc="$3";pc="$4"
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 clean
 "$P" -NoProfile -File Route-MissionVM-v2183.ps1 -Take "$take" -MapInfo "${MAPINFO:-campaign/edge_of_the_world/eotw.mapinfo}" -Candidate "$CAND" -StepsFile "$steps" -HostCharacter "$hc" -PartnerCharacter "$pc" -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'));[print(k,v) for k,v in n.items() if k.startswith('diff-')]" 2>&1 | head -c 900)"
}
want="${*:-install a t r sa sb se c m k}"
for x in $want; do
 case $x in
  install) install;;
  a) run labfish-eotw-v2188a routes/labfish-eotw-v2188.json corvo corvo;;
  t) run labfish-eotw-v2188t routes/labfish-eotw-twin-v2188.json corvo corvo;;
  r) run labfish-eotw-v2188r routes/labanimal-eotw-v2180.json corvo corvo;;
  sa) run deadlyshade-eotw-v2188a routes/deadlyshade-eotw-v2188.json corvo emily;;
  sb) run deadlyshade-eotw-v2188b routes/deadlyshade-eotw-puppetoff-v2188.json corvo emily;;
  se) run deadlyshade-eotw-v2188e routes/deadlyshade-eotw-v2188.json emily emily;;
  c) run corpse-eotw-v2189c routes/corpse-eotw-v2189.json corvo corvo;;
  k) MAPINFO=campaign/void/void_02/void_02.mapinfo run campaign-curator-vm-v2191k routes/campaign-curator-vm-v2041.json corvo emily;;
  m) MAPINFO=campaign/boat/madinventor/boat_madinventor.mapinfo run campaign-madinventor-vm-v2191m routes/campaign-madinventor-vm-v2038.json corvo emily;;
 esac
done
clean
echo done
