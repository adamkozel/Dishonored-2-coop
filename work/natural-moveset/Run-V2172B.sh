#!/usr/bin/env bash
# 2172 (#261): the Blink Assault knockdown. The native knockdown is arkAIHitReact::PlayThrowHit (0x140593bd0), which
# ragdolls the guard only when the damage parameters' throw-hit block (+0x58, arkThrowHitInfo, 0x30 bytes) is set; the
# host's npcCombatApplyPeerNative never writes it (d2-ghidra, confirmed static). blinkkick-params-v2080b's recorded kick
# rows were hits between two other NPCs, so the partner's real kick bytes were never read. Candidate progress-v2171 keeps
# her last kick's 144 parameter bytes (npc-combat blinkKickInfo). The local lab (Corvo host, Corvo partner, Edge of the
# World checkpoint guard) runs Test-MissionPartnerPowers-v2081 with blinkkick on it.
# blinkkick-v2172a: her kick did not land (health 40 -> 40 in both worlds, nothing captured; the guard was displaced
# 5.54 m in both). b, c and d try again.
# Usage: Run-V2172B.sh [a b c d]
cd "$(dirname "$0")"
P="C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe"
CAND=candidate-progress-v2171
run(){
 take="$1";shift
 if [ -e "$take" ]; then echo "$take exists, skipped";return;fi
 echo "candidate $CAND"
 cp "../stage8/$CAND/dinput8.dll" "../stage8/$CAND/d2_runtime_probe.exe" ../../outputs/d2-coop-lab/bin/
 "$P" -NoProfile -File ../movement-integration/Deploy-SourceClock.ps1 > "$take-deploy.log" 2>&1 || { echo "deploy failed"; return; }
 "$P" -NoProfile -File Test-MissionPartnerPowers-v2081.ps1 -Take "$take" -Candidate "$CAND" -Powers blinkkick "$@" > "$take.log" 2>&1
 echo "$take exit $?"
 python -I -c "import json,sys;d=json.load(open(sys.argv[1],encoding='utf-8-sig'));print({k:d.get(k) for k in ('blinkKicksSent','blinkKicksApplied','blinkKickInfoCaptures')});h=d.get('blinkKickInfo','');print('info',h)" "$take/blinkkick-after-client.npc-combat.json" 2>&1 | head -c 800
}
want="${*:-a}"
for x in $want; do
 case $x in
  a) run blinkkick-v2172a;;
  b) run blinkkick-v2172b;;
  c) run blinkkick-v2172c;;
  d) run blinkkick-v2172d;;
 esac
done
echo done
