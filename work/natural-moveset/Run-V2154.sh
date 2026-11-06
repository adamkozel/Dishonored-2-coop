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
# progress-v2137 (MovementBank::keepUnder). The candidate is deployed per take. Harness v2137 keeps the partner's and the
# host's combat counters on every press of the partner's Ramsey fight and her frame every 8th press (the held fights of
# s4-dunwall-v2135c, d and g). e (Corvo / Corvo) and f (Emily / Emily) are also the S4 reruns of those pairings.
# 2138 (#271, S4): Run-V2137 a-f failed on harness faults: e and f found the rig handle at 0 while the product respawned
# the other player's rig after a teleport, c's death wait timed out on a stalled VM read (both games were on GameOver),
# and b's host A landed on Restart Mission (its dialog held 180 s); a lost the start to #263. Harness v2138 waits for a
# stable handle, reads once more before failing the death wait and answers the Restart Mission dialog. e's one sample
# showed the partner holding the sword: the route never selected the pistol. Route pistol-walk-dunwall-v2138 presses
# D-pad Left (the pistol) for both players before the pistol-facing diff and keeps both avatar-weapon sidecars.
# a: before (progress-v2136); b, c: after (progress-v2137); d (Corvo / Corvo) and e (Emily / Emily): the S4 reruns.
# 2139 (#271, S4): Run-V2138 a-e failed on harness faults again. a and e: the partner's teleport to the boat landed
# before the VM streamed the floor in and she fell through the world (view z -194651), so the host's copy of her rig
# respawned every second and the sampler found no stable handle. b: ld_trigger_balcony was absent from the host's
# entity list. c and d: Ramsey ran down a floor before he died and no blind loot press registered. And D-pad Left put
# the pistol away (it is in hand from the safe room on). Harness v2139 teleports a fallen partner again, reads a
# missing trigger again, aims each loot press by the partner's native use focus, and its pistol step checks each
# game's movement status (rangedKind) for the other player's pistol, pressing Left only while it is not shown.
# Route pistol-walk-dunwall-v2139. Same cases as Run-V2138.
# 2140 (#271, S4): Run-V2139 a-e failed again. a and d: the partner fell through the world at the boat even after four
# re-teleports. The host's boat stand starts the mission-end sequence (both games show the same fixed cabin view and
# Escape Dunwall completes), so her teleport there lands outside the world. b: the pistol step pressed D-pad Left three
# times with rangedKind 0 and put the pistol away. c and e: VM remote-session faults. Harness v2140 records the pistol
# stance without pressing, and its facepartner step brings the host to the partner on the street (4.5 m, facing her,
# on her floor) and turns her to the host. Route pistol-walk-dunwall-v2140 samples there and goes to the boat after.
# 2141 (#271, S4): Run-V2140 a reached both pistol samples (host sample PASS, rangedKind 1 in both games) and stopped at
# the rest guard (host 5.3 mm once). b: the partner held Ramsey for 160 presses. c, d and e: PowerShell Direct faults in
# the partner's fight. Harness v2141 retries such a VM call and reads the rest guard up to three times.
# 2142 (S4): Run-V2141 b, d and e passed (both pistol samples). a and c: the partner froze in place during Ramsey's
# fight, he died of an incendiary bolt and her loot never focused his body. Harness v2142 keeps both full npc-combat
# statuses and her player states while the fight holds, her logic state each press, and her view when the loot fails.
# 2143 (S4, #264): Run-V2142 a (v2136) passed with both pistol samples. c held Ramsey at 8 for 55 presses: the host
# retired his npc-combat channel (hit_damage_parameters) and re-armed it, the partner's stayed ready and answered its
# Hellos with Ready only, which the fresh channel refused, so her kill never reached the host (ramsey-loot diff: host
# alive, partner dead). Candidate progress-v2143 resets such a stale peer with one Stop. Every case runs on v2143.
# 2144 (S4, #264): Run-V2143 on progress-v2143: b (Corvo / Emily), c (Emily / Corvo) and d (Corvo / Corvo) passed, every
# diff equal; the host reset Ramsey's stale channel once in e (staleReadyResets 1) and the fight ended. e (Emily / Emily)
# then never focused his body in either loot pass. Harness v2144 adds a wider third pass. e and g rerun Emily / Emily.
# 2146 (S4, #264): Run-V2144 g (Emily / Emily) passed. e held Ramsey at 10 for 130 presses with the partner frozen in
# PlayerStateAssassination: her scene keytarget_finish_mortimer_ramsey (npc, 8500 ms) stayed at phase 2, which native
# 1407662f0 keeps while an animation channel on the NPC outranks the scene. Candidate progress-v2145 adds the per-NPC
# key-target finishers to the takedown census; harness v2146 keeps Ramsey's animation channels during holds.
# Emily-partner cases (a, b Corvo / Emily; e, g Emily / Emily) and the Corvo-partner ones (c Emily / Corvo, d Corvo / Corvo).
# 2147 (S4, #264): Run-V2146 a b c d passed; e held Ramsey again with the partner in his key-target finisher at
# phase 2, and g died in step33. The channel table native 1407662f0 checks is on Ramsey's scene proxy
# (arkCpntMidnightProxy), not his animation component, so harness v2147 keeps that proxy, its request list,
# --takedown-gates and the client's running scenes during holds.
# g died in ld_trigger_reachstreets (step33) with the bounce teleports 1.2 s apart: a partner bounce now sends both
# teleports from one guest process, 250 ms apart.
# 2148 (S4, #264): Run-V2147 e (Emily / Emily) passed, Ramsey died at press 21 with no hold, and its streets bounce
# held. g lost the host game at press 20 of the Ramsey fight (M10 again: no dump, no Windows event). Harness v2148
# keeps the host's exit code and loader log on a failure. Emily-partner cases repeat to catch the finisher hold.
# 2149 (S4, #264): pistol-walk-dunwall-v2148e held Ramsey's finisher at phase 2 with its clock at -1 and his proxy
# slots empty, so the start gate 1407cbbe0 (LOD driver / required animation byte per bound actor) holds it, not a
# slot conflict. Harness v2149 keeps the waiting finisher's actors, LOD drivers and those bytes.
# 2150 (S4, #264): Run-V2149 e and g passed; h held the finisher. Its bound groups confirmed the cause: Ramsey's
# group asks for equipment 1 (Sword, +0x189) and stance 0, while every player binding is ready (+0x188 = -1). The
# host's Ramsey draws his sword by his own AI, which the replica's lease pauses. Candidate progress-v2150 requests
# that stance and equipment on the replica while her own takedown scene waits on it. Every pairing runs on v2150.
# 2151 (S4, #264): Run-V2150 e g h a c passed; d failed the loot after the finisher waited to press 70 although the
# request fired (graph 0, group still 0x100), and a waited to press 90. The replica's graph changes equipment only
# through its paused AI. Candidate progress-v2151 also marks the replica's group ready (+0x188 = 0xffff).
# 2152 (S4, #264): Run-V2151 passed d a e g h c, every diff equal, and the finisher never waited. The done-when asks
# for two clean passes in a row on one candidate, so every pairing runs again on progress-v2151.
# 2153 (S4, #264): Run-V2152 a c d e h passed, every diff equal; g (Emily / Emily) killed Ramsey by the finisher but
# no loot stand focused his body by the planters (harness, as v2143e). Harness v2152 aims the later passes from
# the partner's eye. Two clean passes in a row on progress-v2151: Run-V2153, then Run-V2154.
# 2154 (S4, #264): the second of the two clean passes on progress-v2151 and harness v2152 (see 2153).
# 2154 (later): Run-V2153 g passed; a stopped on the VM's pause menu (no route input pauses it, hypothesis: the pad
# dropped in a 4.2 s VM stall), c was lost to a Steam restart on the host PC, and d passed every diff but the rest guard
# before step45-animsample read 5.15 mm on its third reading. Harness v2153 allows six readings. Run-V2154 and
# Run-V2155 are the two clean passes on progress-v2151.
# Usage: Run-V2154.sh [a b c d e f g h]
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
 "$P" -NoProfile -File Route-MissionVM-v2153.ps1 -LookStages -Take "$take" -MapInfo campaign/dunwall/escape/tower/dunwall_escape_tower.mapinfo -Candidate "$CAND" -StepsFile "$steps" -HostCharacter "$hc" -HostChoice "$side" -PartnerCharacter "$pc" -CoopSaves -StoryFlags -HostStory -ItemsPerPlayer > "$take.log" 2>&1
 echo "$take exit $? $(python -c "import json;r=json.load(open('$take/result.json',encoding='utf-8-sig'));n=r.get('notes',{});print(r.get('passed'),r.get('failure'));[print(k,v) for k,v in n.items() if k.startswith(('diff-','diffwait-'))];f=n.get('final',{});print({x:(f[x].get('map'),f[x].get('hold')) for x in f})" 2>&1 | head -c 2500)"
 grep -h "world-digest:" "$take.log" | head -3; ls "$take" | grep -i box
 for t in mayhew ramsey-loot saferoom balcony streets end; do [ -f "$take/diff-$t.txt" ] && { echo "$t:"; head -6 "$take/diff-$t.txt"; }; done
 for f in "$take"/step2[0-9]-*-pickupcatchup-host.json; do [ -f "$f" ] && echo "$(basename "$f"): $(python -c "import json,sys;d=json.load(open(sys.argv[1]));print(d['result'],d['planned'],d['awarded'],d['awardedNames'],d['failedNames'])" "$f")"; done
 last=$(ls "$take" | grep -o "^step[0-9]*-[a-z]*-missiondecision-client.json" | sort | tail -1); [ -n "$last" ] && echo "decision: $(tr -d '\r\n ' < "$take/$last" | head -c 600)"
}
want="${*:-a b d e c}"
for x in $want; do
 case $x in
  a) run pistol-walk-dunwall-v2154a routes/pistol-walk-dunwall-v2140.json corvo emily Left candidate-progress-v2151;;
  b) run pistol-walk-dunwall-v2154b routes/pistol-walk-dunwall-v2140.json corvo emily Left candidate-progress-v2151;;
  c) run pistol-walk-dunwall-v2154c routes/pistol-walk-dunwall-v2140.json emily corvo Right candidate-progress-v2151;;
  d) run pistol-walk-dunwall-v2154d routes/pistol-walk-dunwall-v2140.json corvo corvo Left candidate-progress-v2151;;
  e) run pistol-walk-dunwall-v2154e routes/pistol-walk-dunwall-v2140.json emily emily Right candidate-progress-v2151;;
  f) run pistol-walk-dunwall-v2154f routes/pistol-walk-dunwall-v2140.json corvo emily Left candidate-progress-v2151;;
  g) run pistol-walk-dunwall-v2154g routes/pistol-walk-dunwall-v2140.json emily emily Right candidate-progress-v2151;;
  h) run pistol-walk-dunwall-v2154h routes/pistol-walk-dunwall-v2140.json emily emily Right candidate-progress-v2151;;
 esac
done
clean
echo done
