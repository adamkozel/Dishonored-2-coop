#!/usr/bin/env bash
# 2183 (#244, step 6): the other three character pairings on two Steam accounts, through the first (dunwall), a middle
# (curator) and the last mission (empress) from campaign-v2183-pairings-plan.txt, with Run-V2190C.sh (2190: candidate progress-v2190; was Run-V2182C.sh, candidate
# progress-v2180, harness Route-MissionVM-v2181). Take letters: c = Emily host / Corvo partner, d = Corvo / Corvo,
# e = Emily / Emily (a = the whole campaign Corvo host / Emily partner).
cd "$(dirname "$0")"
PLAN=campaign-v2183-pairings-plan.txt
for spec in "c emily corvo" "d corvo corvo" "e emily emily"; do
  set -- $spec
  echo "pairing $1: host $2 partner $3"
  bash Run-V2190C.sh "$1" "$PLAN" "$2" "$3"
done
echo alldone
