#!/usr/bin/env bash
# 2126: S2 again on harness v2115, which finds partner NPCs in the entity list by definition name (Run-V2121 b: the
# npc-activity catalog names spawners, so partnerattack found no mortimer_ramsey).
# 2123: moved to candidate progress-v2123 and harness v2113 after Run-V2116 (wrap fix, prompt re-arm, armwait).
# 2121 (#264, sweep S2): the partner does the story beats, in all four pairings. Route sweep-s2-dunwall-v2121 is the
# first-mission route with the partner entering every trigger volume, killing Ramsey (partnerattack) and looting him
# (partnerloot: done when the host's gameflow completes the node); the host still uses the ring lockers and talks to
# Meagan. Diffs at every stage. Candidate progress-v2113, harness v2112 (trigger role, partnerloot).
# 2130: S2 again on progress-v2126 and harness v2118 (the partner's health hold stays on after the fight) with up to 160
# attacks (route sweep-s2-dunwall-v2130). Run-V2126: Ramsey's guards killed the partner after the fight in b, c and d,
# and in a Ramsey blocked at 10 health for the last 50 of 80 attacks.
# 2132: S2 again on candidate progress-v2132 (character-variant story pickups). Run-V2130 a and b: the partner took
# Ramsey's ring (her character's variant) and the host, holding no ring, could not open the safe room (Task_DW_OpenVault).
# 2133: S2 again on candidate progress-v2133 (item family 11, the ring's, is a story family) and harness v2133 (the
# partner changes sides after 12 presses without damage; the second loot pass circles the body). Run-V2132: the
# 2132 catch-up never planned the ring (d), Ramsey held at 20 health (a), the partner's loot missed (c), and the VM
# game crashed in a scene start at ld_trigger_bedroom (b). d runs first.
# 2134: S2 again on candidate progress-v2134 (the probe fails a digest whose NPC catalog is unreadable) and harness
# v2134 (the partner's bounce sends at once; the partner stands at a holding NPC before every press). Run-V2133: the
# host got the ring in b and c, which passed the safe room and the balcony; the partner then fell to its death at
# ld_trigger_reachstreets. a held Ramsey at 10 health, d's loot missed. b runs first.
# 2135 (S4): S2 on progress-v2135 (b on; a ran on progress-v2134) with harness v2135 -LookStages: after every diff each game's observer camera films
# the other player from three sides (look-<stage>-<viewer>-<n> frames) for the visual review. Run-V2134 passed S2 in
# all four pairings (a, c, d, f). Takes s4-dunwall-v2135a-d.
# 2136 (#271): route pistol-facing-dunwall-v2136 (S2 plus the partner 2 m in front of the host on the streets, then a
# diff) with -LookStages, to film the remote held-pistol pose from the front and both sides. Takes pistol-facing-dunwall-v2136a-h.
# 2137 (#271): route pistol-walk-dunwall-v2137 (a plain step list; pistol-facing-dunwall-v2136 wrapped it in {note,
# steps}, which the harness read as one empty step, so take v2136a ran nothing after the start). After the host stands at
# Meagan, the partner stands 2 m in front facing it and a diff films both (-LookStages), then each game samples the
# other rig in its engine frame (coop_anim_sample, with head, nose, arm and wrist joints since progress-v2136) while it
# stands 1 s, walks forward 1.8 s and back 1.8 s, pistol selected. Before: progress-v2136 (sampler only). After:
# progress-v2137 (MovementBank::keepUnder). The candidate is deployed per take.
# Usage: Run-V2137.sh [a b c d]
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
# The partner's progress files are lab state; every take starts and ends without them.
clean(){
 rm -f ../lab-instances/host/base/savegame_coop/coop-progress-*.txt
 "$P" -NoProfile -Command '. ../../tools/vm/D2LabVM.Common.ps1; . ../../tools/vm/D2LabVM.Lab.ps1; Invoke-D2LabVM { Remove-Item C:/D2Lab/ws/work/lab-instances/client/base/savegame_coop/coop-progress-*.txt -ErrorAction SilentlyContinue }' > /dev/null 2>&1
}
run(){
 take="$1";steps="$2";hc="$3";pc="$4";side="$5";CAND="$6"
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 echo "candidate $CAND"
 cp "../stage8/$CAND/dinput8.dll" "../stage8/$CAND/d2_runtime_probe.exe" ../../outputs/d2-coop-lab/bin/
 "$P" -NoProfile -File ../movement-integration/Deploy-SourceClock.ps1 > "$take-deploy.log" 2>&1 || { echo "deploy failed"; return; }
 clean
 "$P" -NoProfile -File Route-MissionVM-v2135.ps1 -LookStages -Take "$take" -MapInfo campaign/dunwall/escape/tower/dunwall_escape_tower.mapinfo -Candidate "$CAND" -StepsFile "$steps" -HostCharacter "$hc" -HostChoice "$side" -PartnerCharacter "$pc" -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'));[print(k,v) for k,v in n.items() if k.startswith(('diff-','diffwait-'))];f=n.get('final',{});print({x:(f[x].get('map'),f[x].get('hold')) for x in f})" 2>&1 | head -c 2500)"
 grep -h "world-digest:" "$take.log" | head -3; ls "$take" | grep -i box
 for t in mayhew ramsey-loot saferoom balcony streets end; do [ -f "$take/diff-$t.txt" ] && { echo "$t:"; head -6 "$take/diff-$t.txt"; }; done
 for f in "$take"/step2[0-9]-*-pickupcatchup-host.json; do [ -f "$f" ] && echo "$(basename "$f"): $(python -c "import json,sys;d=json.load(open(sys.argv[1]));print(d['result'],d['planned'],d['awarded'],d['awardedNames'],d['failedNames'])" "$f")"; done
 last=$(ls "$take" | grep -o "^step[0-9]*-[a-z]*-missiondecision-client.json" | sort | tail -1); [ -n "$last" ] && echo "decision: $(tr -d '\r\n ' < "$take/$last" | head -c 600)"
}
want="${*:-a b c}"
for x in $want; do
 case $x in
  a) run pistol-walk-dunwall-v2137a routes/pistol-walk-dunwall-v2137.json corvo emily Left candidate-progress-v2136;;
  b) run pistol-walk-dunwall-v2137b routes/pistol-walk-dunwall-v2137.json corvo emily Left candidate-progress-v2137;;
  c) run pistol-walk-dunwall-v2137c routes/pistol-walk-dunwall-v2137.json emily corvo Right candidate-progress-v2137;;
  d) run pistol-walk-dunwall-v2137d routes/pistol-walk-dunwall-v2137.json emily corvo Right candidate-progress-v2136;;
  e) run pistol-walk-dunwall-v2137e routes/pistol-walk-dunwall-v2137.json corvo corvo Left candidate-progress-v2137;;
  f) run pistol-walk-dunwall-v2137f routes/pistol-walk-dunwall-v2137.json emily emily Right candidate-progress-v2137;;
  g) run pistol-walk-dunwall-v2137g routes/pistol-walk-dunwall-v2137.json corvo emily Left candidate-progress-v2137;;
  h) run pistol-walk-dunwall-v2137h routes/pistol-walk-dunwall-v2137.json emily corvo Right candidate-progress-v2137;;
 esac
done
clean
echo done
