# Current state and exact build



<!-- GRENADE-LIVE-V55 -->
## Current measured co-op continuation

Updated 2026-10-20. Live candidate-npc-combat-v1214fx ([identity](../../work/movement-integration/candidate-npc-combat-v1214fx/identity.json)), protocol 80, bank 75/382, published as [v0.8.0](https://github.com/adamkozel/Dishonored-2-coop/releases/tag/v0.8.0) ([chapter 98](98-release-without-game-files.md); install test 8/8, clean-PC take product-two-account-v1670a). 1214fr-fx ([chapter 106](106-partner-led-story-and-mid-mission-join.md)): the partner's own story steps reach the host (partner advances on D2GV, only from an unchanged host state), the partner leads all of Dunwall with both worlds equal, a late partner is placed beside the host, a real autosave crossed by either player alone pairs both worlds (#136), assassinations reach 8 m (#140), and a host-led Ramsey kill counts again (the 1214fr false advances, fixed in 1214fx). Earlier: 1214fq shares breakables between the worlds by step (#132, [chapter 105](105-breakables.md)); 1214fm-fp share Domino's linked deaths and knockouts as Linked combat requests, accept an implied Ready after a combat re-arm, and rebase the host's HostWorld baseline on a knockout instead of stopping combat (#117, [chapter 104](104-power-upgrade-world-outcomes.md)); 1214fk-fl share Shadow Kill's ash between the worlds, and Stop Time already freezes the host's NPCs in both (#117, [chapter 104](104-power-upgrade-world-outcomes.md)); 1214fc-fe make Far Reach's object and body pulls follow in the partner's world through the held-object path, and 1214fh (source equal to 1214ff) ends the host corpse hold with its window (#115, [chapter 103](103-far-reach-pulls.md)); 1214fj keeps Corvo's copy of Emily's lethal victim in her last victim pose, so it no longer stands up (#127, chapter 103); 1214fa-fb identify orphan NPCs of childless spawners and share a takedown on a vanished NPC; hound takedowns now end the same way in both worlds (#118, [chapter 102](102-scripted-npcs-in-npc-authority.md)); 1214ex-ey bind scripted civilians and NPCs at scripted posts in NPC authority, so their takedowns present on the partner (#113, [chapter 102](102-scripted-npcs-in-npc-authority.md)); 1214eu presents the Timepiece on the partner like the Heart (#102, [chapter 101](101-native-player-states.md)). Earlier: 1214dp, protocol 77, bank 74/381 (knockdown bank 1400); 1214di-dj give the Shadow Walk partner native material effect 1 (a black silhouette, survey 1381b) through the form's attacks, 1214dk presents Emily's out-of-mana response, 1214dm hands the partner's carried body to the native carry attachment in product mode, 1214dn presents the partner's knockdown (clip 380), and 1214dp keeps the movement clock eligible through a presented stun: before it every stun and the knockdown fell back to the actor's native pose after the first packet (1397k/l). 1214dq (built) adds carry restore-cause telemetry. The protected-file guard baseline was re-recorded on 2026-10-19 after the user's own game tests ([note](../../work/movement-integration/protected-baseline-rerecord-2026-10-19.json)). Latest:
- **Native player states outside the checklist (#102, [chapter 101](101-native-player-states.md)), decided:** every unlisted native player state is presented, covered by an existing row, first person only (the spyglass) or not applicable. The Timepiece (states 68-70) is presented on the partner with the Heart's held-item acting and a render-only Timepiece prop (1214eu; coverage-timepiece-v1592a in both roles; the real shift in A Crack in the Slab, takes 1593b/c).
- **Movement completion coverage (#11, [chapter 99](99-movement-completion-coverage.md)): 103 of 105 items complete; 2 are not applicable or excluded, none remain.** Coverage harnesses 1318–1333 and the Edge of the World mission harness 1328 drive locomotion, stance, gait, traversal, combat, ranged weapons, items, powers, swimming, climbing, guard fights and the sloped street under the product controller in both roles, and check the partner from its presentation state. Completion audits 1323–1393 record each item's decision in the catalog with reviewed observer video. The last items, wall and window takedowns and body sizes, were audited in 1571 and 1572 (doors in 1378, struggle and escape in 1447, the kick in 1453, the Far Reach pull in 1479, object carry in 1470). Receiver fixes that came from them:
  - 1214cg: the protocol-73 hold keeps the authored body instead of flashing the actor's native pose;
  - 1214ch: a 150 ms grace before yielding the body on a transient ineligible packet;
  - 1214ci, 1214cj, 1214cl: the body clock survives vault exits, ledge steps in long frames and jump-attack landings;
  - 1214cn, 1214co, 1214cq: the damaging landing scene (`cat_fall_strong`, 1066 ms, bound to the body, then the left and right native arms) is presented as the hard landing;
  - 1214cp: Emily's Doppelganger clone and Mesmerize spirit are shown to the partner in product mode, not only in the lab fixture (#104);
  - 1214cr, 1214cs: Emily's Domino marks and link beam in product mode, held between casts; the Far Reach aim loop presented;
  - 1214ct: Corvo's Blink Assault kick presented as the kick;
  - 1214cu: a left-hand power EquipChange beside blocks and swings no longer stops the stream;
  - 1214cv: the partner's death plays a directional native collapse and holds it (protocol 76, bank 72), and the presentation clone takes no unrouted world damage (a host guard had killed it; the joiner's missing guard damage is [#108](https://github.com/adamkozel/Dishonored-2-coop/issues/108)); 1214cw/cx: an NPC finisher on the player plays the same collapse, timed to the native body's fall;
  - 1214cy: the client binds a guard that is already fighting, so its hits reach the joiner and its death is shared (#108);
  - 1214cz: a parried or blocked swing (player state 40 Recoil) is presented as the native NPC parry-lose stagger (protocol 77, bank 73);
  - 1214da-db: Shadow Walk's attack and choke, which run in the form's own state mode at stance 2, read and presented hands-free with the `attack_out` exit (power variant 36); 1214df captures the shadow-form kill (no inflictor) so the death is shared (1254zb);
  - 1214dc, 1214dg: action packets mapped outside the presentation window are clamped (up to 1500 ms early, 1000 ms late) instead of refused, which stopped both peers' streams for about 5 s in guard fights;
  - 1214de: a stun with the left hand's EquipChange, and the left slashes that bind the struck guard, are no longer refused.
  Lab-only grants put weapons, traps, the Heart, powers and Agility in the isolated save (1214ck, 1214cm).
- **Release 0.7.0 without game files (#30, [chapter 98](98-release-without-game-files.md)):** the installer builds the voidmod packages, both movement banks and the prop tables from the player's own install, sampling animations through the player's own `Dishonored2.exe`. The release ships only our DLL, the builder, recipes and patches. `release-install-v1313c` passes 8/8 on a full copy of the original game. Published as [v0.7.0](https://github.com/adamkozel/Dishonored-2-coop/releases/tag/v0.7.0). **Clean-PC proof (take 1315b):** the VM (second account, Defender on, no detection) downloads the release from GitHub, installs it with `Install.cmd` alone, joins, follows the host into the mission as Emily, both walk, F8 leaves, and the uninstall restores the originals. Candidate 1214cf adds join-follow from the main menu, and the DLL loads its prop tables from the installer's file (1214cc–1214ce).
- **Dunwall safe-room loadout reaches the partner (#85, [audit 1311](../../work/movement-integration/dunwall-loadout-audit-v1311.json)), complete:** with a Corvo host, the Emily partner receives the sword, her own pistol, bullets and the spyglass through the Kiscule action mirror. Candidate 1214cb maps the three character-gated grants to the partner's own branch. The route harnesses steer the native character choice reliably (`-HostChoice Left`), verify both characters after the start, and search bodies with a focus-checked use ([chapter 97](97-campaign-map-sequence.md)).
- **Campaign take 1, Memories of Training (#73, [audit 1310](../../work/movement-integration/campaign-training-audit-v1310.json)), complete:** the separate training map, played as Emily with Corvo as the trainer through mobility, stealth, crossbow and combat to the main menu, in the lab (route-tut-v1310j) and on two Steam accounts (campaign-training-vm-v1310b). Candidates 1214bz (the tutorial end reaches the partner) and 1214ca (the Kiscule action mirror is host-authoritative, after the partner's own stealth restarts emptied the host's hands); the route harness gains timed host actions (`tools/diag/d2_route_actions.py`) and a partner step ([chapter 97](97-campaign-map-sequence.md)).
- **Campaign take 10, Death to the Empress (#82, [audit 1309](../../work/movement-integration/campaign-empress-audit-v1309.json)), complete:** played from the Dreadful Wale through Dunwall's streets, the Tower, the throne room and the painted world to Delilah's death, the rescue and the credits, in the lab (route-dtte-v1309e) and on two Steam accounts (campaign-empress-vm-v1309a). Candidate 1214by carries a LevelTransition whose map name uses backslashes. In the credits the partner keeps two end-game gameflow nodes active ([#94](https://github.com/adamkozel/Dishonored-2-coop/issues/94)) ([chapter 97](97-campaign-map-sequence.md)).
- **Campaign take 9, The Grand Palace (#81, [audit 1308](../../work/movement-integration/campaign-palace-audit-v1308.json)), complete:** played from the Dreadful Wale's briefing through the palace, the real Duke, the vault and Jessamine's scene to Meagan's boat and Death to the Empress, in the lab (route-pa-v1308f) and on two Steam accounts (campaign-palace-vm-v1308a). The route harness's attack can stand behind an NPC (its facing from entity +0x84) ([chapter 97](97-campaign-map-sequence.md)).
- **Campaign take 8, A Crack in the Slab (#80, [audit 1307](../../work/movement-integration/campaign-bunker-audit-v1307.json)), complete:** played from the mansion entrance through the Timepiece, the ritual in the past, the Outsider's Void and the Dust District return to the Grand Palace boat, in the lab (route-bk-v1307d) and on two Steam accounts (campaign-bunker-vm-v1307a). The ritual counts only on the hall floor, inside its trigger's real volume ([chapter 97](97-campaign-map-sequence.md); new `tools/diag/d2_trigger_box.py`).
- **Campaign take 7, The Dust District (#79, [audit 1306](../../work/movement-integration/campaign-dust-audit-v1306.json)), complete:** played from the Dreadful Wale through Meagan's briefing and the Jindosh lock to Stilton's manor, in the lab (route-dd-v1306d) and on two Steam accounts (campaign-dust-vm-v1306a). The lock is dialled natively from its own script state ([chapter 97](97-campaign-map-sequence.md)).
- **Campaign take 6, The Royal Conservatory (#78, 1214bw–1214bx, [audit 1305](../../work/movement-integration/campaign-curator-audit-v1305.json)), complete:** played from the Void through the Dreadful Wale, Ashworth and the escape to `boat_dust`, in the lab (route-cu-v1305f) and on two Steam accounts (campaign-curator-vm-v1305c). A partner who is paused when the host leaves now follows on resuming (1214bw–1214bx) ([chapter 97](97-campaign-map-sequence.md)).
- **Campaign take 5, The Clockwork Mansion (#77, [audit 1304](../../work/movement-integration/campaign-madinventor-audit-v1304.json)), complete:** played from the Dreadful Wale through the streets, the cable car, Jindosh and Sokolov back to Meagan, in the lab (route-mi-v1304s) and on two Steam accounts (campaign-madinventor-vm-v1304a). The route harness gained focus-guided use, carries, health-hold spans and a live reader for script variables ([chapter 97](97-campaign-map-sequence.md)).
- **Campaign take 4, The Good Doctor (#76, 1214bp–1214bv, [audit 1303](../../work/movement-integration/campaign-institute-audit-v1303.json)), complete:** played from the cable car through Hypatia, Vasco, the Crown Killer, the watchtower and Meagan to the Dreadful Wale, in the lab (route-institute-v1303ap) and on two Steam accounts (campaign-institute-vm-v1303a) ([chapter 97](97-campaign-map-sequence.md)). On the way:
  - gameflow states are named from the game's own trees (FNV-1a of the lowercased name);
  - the mission end is sent from the transport pump, since this mission completes inside a movie (1214bq–1214bs);
  - a gameflow node that only the partner advanced is reset (1214bt, #87).
  - the mission is named from the loaded map when the native mapinfo declaration is absent, as it is during a mission started with the `campaign` command (1214bu); this also fixes join-follow (`D2MF`).
- **Campaign take 3, Edge of the World (#75, 1214bi–1214bk, [audit 1302](../../work/movement-integration/campaign-eotw-audit-v1302.json)):** played from the Outsider visit through the Dreadful Wale and Karnaca to the Good Doctor in lab product mode. Both players now receive the Outsider's Mark and their own power (Corvo Blink, Emily Far Reach): before, only the host did.
  - Story grants that the scripts give the acting player are mirrored natively. These are the Heart, keys, loadouts and mana (script action mirror, `D2KA`, [chapter 97](97-campaign-map-sequence.md)).
  - The Mark and powers stay each player's own choice at the Outsider's offer.
  - A death after a level transition now restarts both worlds together (1214bl–1214bo): the checkpoint session re-arms on the new map, uses map and chapter as its identity, recovers from a refused attempt and follows a peer's re-arm ([chapter 88](88-mission-shared-death-and-checkpoint.md)).
- **Campaign take 2, A Long Day in Dunwall (#74):** the two-account VM take passed end to end (campaign-dunwall-vm-v1301c).
- **Campaign take 2, A Long Day in Dunwall (#74, 1214bf–1214bh, [audit 1299](../../work/movement-integration/campaign-dunwall-audit-v1299.json)):** played start to finish in lab product mode, from the coup to both worlds entering the Outsider visit. On the way:
  - all scripted trigger families are mirrored;
  - objectives follow the host (gameflow mirror);
  - script map changes take the partner along (level transition mirror).
  The two-account VM run is next ([chapter 97](97-campaign-map-sequence.md)).
- **Two-account mission end (#16, 1214bb–1214be, [audit 1296](../../work/movement-integration/mission-end-vm-audit-v1296.json)):** in product mode across this PC and the VM's second Steam account, finishing Edge of the World takes both players to The Good Doctor together.
  - Fixed on the way: checkpoint sync had swallowed the result screen's Continue (`RestartFromMemoryCheckpoint`).
  - Lab takes now host without the Steam invite overlay (`coop_session host_quiet`).
- **Chaos follows the host (#16, 1214ba, [audit 1294](../../work/movement-integration/chaos-follows-host-audit-v1294.json)):** the partner keeps the host's chaos points, so both worlds load the same low or high chaos variant of the next mission (tested with the host over the high threshold: both loaded `institute_p_highchaos`).
- **Both-ready start of the next mission (#16, 1214ay–1214az, [audit 1293](../../work/movement-integration/transition-hold-audit-v1293.json)):** whoever presses Continue first waits at the native load end until the partner has loaded too, so both start The Good Doctor together. The window stays responsive while waiting ([chapter 89](89-mission-progression-discovery.md)).
- **Mission end in both worlds (#16, 1214ax, [audit 1292](../../work/movement-integration/mission-end-mirror-audit-v1292.json)):** when one player finishes Edge of the World through the carriage, the partner's world completes its own mission natively. Each player sees their own result screen, and both load The Good Doctor with their own character state ([chapter 89](89-mission-progression-discovery.md), [chapter 97](97-campaign-map-sequence.md)). Without it (take 1291a), the partner stayed behind. Open: the boat and in-mission map changes.
- **Potion use (#11, 1214av):** the lab can drink a native potion in product mode (`coop_use_item`). The 1214aw mana pool write was ineffective and is removed in 1214ax ([audit 1290](../../work/movement-integration/mana-attempt-audit-v1290.json)).
- **Real grenades (#14, 1214au, [audit 1288](../../work/movement-integration/real-grenade-audit-v1288.json)):** a real grenade thrown at a guard in product mode lands in both worlds, from either player ([chapter 95](95-shared-combat-authority.md#real-grenades-in-product-mode-1214au-audit-1288)). The lab can grant one native grenade, health or mana potion in product mode.
- **Reading a note (#11, 1214at, [audit 1287](../../work/movement-integration/reading-audit-v1287.json)):** while the partner reads, they hold a reach instead of standing frozen; accepted in both roles ([chapter 94](94-visual-sweep-counterattack-and-gestures.md#reading-a-note-1214ar1214at-audit-1287)).
- **Crouched heavy and map-load recovery views (#11, [audit 1286](../../work/movement-integration/views-audit-v1286.json)):** both accepted in both roles. The free camera hides the observer's own arms (1214aq), and the lab health hold is confirmed at runtime ([chapter 94](94-visual-sweep-counterattack-and-gestures.md#crouched-heavy-and-map-load-recovery-audit-1286)).
- **Counter-kill parked (#9, [audit 1285](../../work/movement-integration/counter-parked-audit-v1285.json)):** Emily's counter-kill and the kick view stay open; the native god mode is stripped in this build. External research references are in [chapter 96](96-external-research-references.md).
- **Free observer camera (#61, 1214an, [audit 1284](../../work/movement-integration/free-camera-audit-v1284.json)):** the lab camera can now be held at any point (`coop_observer_camera hold_at`) without moving the observer's body. Visual takes use it with `-FreeCam` ([chapter 92](92-native-use-interactions.md)).
- **Coverage views (#11, 1214al–1214am, [audit 1283](../../work/movement-integration/coverage-audit-v1283.json)):** the long fall and landing, Corvo's key use and the door use are accepted in both roles. A use made while crouched is now presented on the partner; before, Emily's crouched lock and door uses showed nothing ([chapter 92](92-native-use-interactions.md), [chapter 94](94-visual-sweep-counterattack-and-gestures.md#long-fall-and-landing-audit-1283)).
- **Lever and valve visuals (#33, [audit 1282](../../work/movement-integration/lever-valve-audit-v1282.json)):** the Edge of the World turbine lever and the Dunwall sink tap are filmed and accepted in both roles. The observer camera is held at the moment of the teleport ([chapter 92](92-native-use-interactions.md)).
- **Client sword finishers on the host (#9, 1214ai–1214ak, [audit 1281](../../work/movement-integration/finisher-follow-audit-v1281.json)):** when Emily finishes a host-owned guard, the host's guard now follows her finisher, including an alerted guard whose AI cannot be held. The host now applies the finisher's kill, and the body stays on the floor in both worlds; before 1214ak it launched up to 26 m in her world (take finisher-follow-v1258i; [chapter 95](95-shared-combat-authority.md#client-sword-finishers-on-the-host-1214ai1214ak)).
- **Emily's counterattack and the NPC attack relay (#9, #14, 1214ah, [audit 1280](../../work/movement-integration/counter-relay-audit-v1280.json)):** the relay that carries a guard's hit into the partner's world now follows dynamic relevance; before, it stopped for good after its first retirement, so guards could not hurt Emily ([chapter 95](95-shared-combat-authority.md#the-npc-attack-relay-under-dynamic-relevance-1214ah)). Her sword finishers are shared in both worlds (take 1258l, a killing blow after a blocked hit; her native counter-kill is not yet produced) ([chapter 94](94-visual-sweep-counterattack-and-gestures.md#counterattack-sword-locks-and-b-swing-hits-9)).
- **Shared combat authority on missions (#14, stage 4, 1214n–1214ag, [chapter 95](95-shared-combat-authority.md)):** in multi-guard fights on Edge of the World, every guard now ends in the same state in both worlds (takes 1270e/1270f). A host death the combat capture missed reaches the client as a `Dead` message (1214n). Host-world damage, such as bloodflies stinging a guard (the cause in 1270c/1270e), applies natively on the host and reaches the client as a `HostWorld` health commit; the client matches the host exactly (take 1272b). Killing an unconscious guard is shared in both roles, with the bodies 0.0 m apart (take 1271m, 1214p–1214v). Grenade explosions on a bound guard now apply in both worlds from either player: natively on the host (1214w, take 1274a), and as a request the host applies for the client (1214z, take 1275c). With the players apart, a knockout or death of an NPC that cannot be streamed reaches the other world through the NPC life roster (1214ad, takes 1276i and 1276m on Edge of the World). A carried body follows in the other world whoever carries it (1214ae, takes 1277e/1277f).
- **Held objects (#11, 1214m, [audit 1269](../../work/movement-integration/object-carry-audit-v1269.json)):** holding a native movable keeps the partner presented with a held reach, and the throw finishes it, in both roles ([chapter 94](94-visual-sweep-counterattack-and-gestures.md#holding-and-throwing-objects-1214m-audit-1269)).
- **Visual-sweep fixes (#11, [audit 1267](../../work/movement-integration/sweep-defects-audit-v1267.json)):** bank 1266 removes the grenade release pop; carry, running in 8 directions, slides and NPC deaths are accepted in both roles ([chapter 94](94-visual-sweep-counterattack-and-gestures.md#sweep-defects-and-missing-views-audit-1267)).
- **Visual sweep, counterattack and item gestures (#9, #11, 1214h–1214j, audits [1260](../../work/movement-integration/visual-sweep-audit-v1260.json) and [1264](../../work/movement-integration/gestures-counter-audit-v1264.json)):**
  - the sweep accepted 55 more items from their recorded videos and found 8 concrete defects;
  - pickups, food and elixirs now show a partner gesture in both roles;
  - a counter-kill after a timed parry is presented and shared;
  - NPC combat channels now re-bind after a retirement (in take 1258d, a retired channel left the guard alive in one world);
  - sword locks and b-swing hits are presented.
  See [chapter 94](94-visual-sweep-counterattack-and-gestures.md).
- **Takedown census, variants and finishers (#11, 1214b–1214g, [audit 1257](../../work/movement-integration/takedown-variants-audit-v1257.json)):** every native takedown and sword-finisher scene (census 1255: 117 scenes, exact subscene and duration per character) is a paired partner phase. Drop and slide takedowns (choke and lethal) and sword finishers are presented and shared in both roles, and the drop and slide visuals are accepted as functional ([chapter 93](93-takedown-census-variants-and-finishers.md)).
- **Shared paired takedowns (#46, 1213u–1214a, [audit 1252](../../work/movement-integration/paired-takedowns-audit-v1252.json)):** under the product controller, a rear choke or fast-back lethal by either player on a host-owned NPC leaves one victim, in the same place and state in both worlds (choke 0.0–0.1 m apart). This needed NPC combat to follow dynamic relevance, knockout damage to pass the combat capture, and the host victim to follow the client's native victim pose. The `assassination_back` lethal variant is not presented yet ([chapter 86](86-mission-npc-authority.md#shared-paired-takedowns-46-candidates-1213u1214a-audit-1252)).
- **Crouched takedowns (#11, 1213p–1213r, [audit 1246](../../work/movement-integration/crouched-takedown-audit-v1246.json)):** rear choke and fast-back lethal from crouch are presented in both roles, and the visuals are accepted. While doing this I found and fixed a regression: every choke hold had sent root motion without a wire stance, so peers rejected those pose packets and the partner dropped ([chapter 41](41-native-rear-choke-binding.md)).
- **NPC catalog reads off the engine thread (#43, 1213n/1213o, [audit 1243](../../work/movement-integration/npc-catalog-worker-audit-v1243.json)):** on Edge of the World, both players' games no longer hitch. Before, the host stalled 90–207 ms every 2 s and the client stalled 80–925 ms about every 0.5 s. These stalls also caused the partner-presentation gaps ([chapter 86](86-mission-npc-authority.md#engine-thread-stalls-from-npc-catalog-reads-43-candidate-1213n-audit-1242)).
- **Presentation hold (1213m, [audit 1241](../../work/movement-integration/presentation-hold-audit-v1241.json)):** under the product controller the partner now holds through source gaps of up to 1 s. On the Edge of the World mantle, native-pose time fell from 29% to 1%, and the scene pose blends into locomotion ([chapter 91](91-product-presentation-recovery.md#holding-the-partner-through-source-gaps-candidate-1213m-audit-1241)).
- **Checklist ([movement requirements](generated/movement-requirements.md)):** 95 of 105 runtime-bound, 80 visually accepted, 2 not applicable or excluded by policy. `sit_stand` is not applicable: D2 has no player sitting state (confirmed static, [audit 1240](../../work/movement-integration/not-applicable-audit-v1240.json)).
- **Hand reach (#6), window traversal (#7) and cutscene entry/exit (#8):** the partner's right wrist reaches toward the used entity (audit 1235); the native confined mantle through a climbable window is presented in both roles (audit 1238; visual in #33); the partner retires for a native cinematic and is back 1.57 s after it (audit 1239, accepted on both windows).
- **Use variants on real mechanisms, both roles (#4):** a native use (player state 75, plus the focused entity) plays the matching authored interaction on the partner's body ([chapter 92](92-native-use-interactions.md), [audit 1229](../../work/movement-integration/use-variants-audit-v1229.json)):
  - generic: window;
  - lever: wind turbine;
  - crank: bench vise;
  - wheel/valve: sink tap;
  - key/lock: ring locker.

  Two visuals are accepted: Corvo at the vise and Emily at the lock. The lever and valve still need visual acceptance (#33).
- **#5 fixed in 1213h:** a use ends only on horizontal travel. The window's drop had ended the host's use early.
- **Presentation recovery (#10, #34, #35):**
  - Pause restore is about 1.6 s (it was 17–19 s), and the 60 m warp recovers in 0.33 s.
  - After the tutorial modal the partner returns in 5.1 s. 1213j re-arms the bank on a holstered partner sword; before, it never re-armed.
  - 1213i skips transient refused frames and names every refusal ([chapter 91](91-product-presentation-recovery.md)).
- **Static asset census:** [`tools/diag/d2_resource_index.py`](../../tools/diag/d2_resource_index.py) reads every mission's entity files without lab time.
- An earlier milestone (1214c): `lever_button_crank_wheel` became runtime-bound for the generic reach.
- Counterattack and knockdown (#9) have their native sources identified; neither is bound yet.
- Two machines (stage 2, 2026-10-15): the lab VM `D2CoopVM` now runs the user's second Steam account ([tools/vm](../../tools/vm/README.md)).
  - The first two-account connection passed in take [two-account-session-v1221](../../work/natural-moveset/two-account-session-v1221/result.json). The host lab instance on the main account opened a friends-only lobby, and the VM instance on the second account joined over Steam P2P. Movement synced both ways.
  - Deployed DLL: `outputs/d2-coop-lab/bin/dinput8.dll` is hash-identical to candidate-npc-combat-v1213d.
  - Stage 2 exit criteria met (2026-10-15):
    - [1213e](../../work/movement-integration/candidate-npc-combat-v1213e/identity.json) fixes the stale lab link (#23);
    - take 1222 shows disconnect and reconnect with no stale ownership;
    - [1213f](../../work/movement-integration/candidate-npc-combat-v1213f/identity.json) links the C runtime statically (#22, take 1224);
    - take [1225](../../work/release/product-two-account-v1225/result.json) covers the product path: release r6 installed in the VM, F7 host, Steam join, connected, uninstall rollback.
  - The live DLL is now 1213f. See [chapter 90](90-release-packaging-and-steam-sessions.md#two-account-connection-through-the-lab-vm-take-1221).

Previous header (1203f, history): Live candidate-npc-combat-v1203f ([identity](../../work/movement-integration/candidate-npc-combat-v1203f/identity.json)). Latest (stage 3 gate 4, product presentation):
- In product mode the partner presentation now recovers by itself after pause menus and the tutorial modal (1204f).
- It is live again 1.7 s after a native map load and 2.9 s after a shared checkpoint restart, on both peers (1199d). `map_load_checkpoint_recovery` is runtime-bound.
- Root causes fixed: an engine-thread bank stall, a stream handshake that rejected restarted peers, over-strict product freshness, bulk congestion, and usable-hello reliable flooding ([chapter 91](91-product-presentation-recovery.md)).

Open (stage 3): the remaining unbound checklist items (#9, #11) and visual acceptance (#33).

Previous header (1193, history): Live candidate-npc-combat-v1193 ([identity](../../work/movement-integration/candidate-npc-combat-v1193/identity.json)). Latest (stage 7):
- Product mode boots outside the lab environment. Steam is acquired at boot, there is no lab loopback, and the game stays idle until F7 or a Steam join (audit 1196).
- A joiner follows the host's mission natively (audit 1192).
- A 20-minute soak passed; memory growth is still unattributed (audit 1190).
- Release r4 is `outputs/release/d2coop-v1193`, with a README and scratch-tested install/rollback ([chapter 90](90-release-packaging-and-steam-sessions.md)).

Open: a control soak, two-account validation (needs the user), in-game UI and the mission transition. Earlier headers below are history.

Previous header (1181, history): Live candidate-npc-combat-v1181 ([identity](../../work/movement-integration/candidate-npc-combat-v1181/identity.json)). Latest (stage 7): a product session controller arms and maintains every co-op system by itself, verified in the lab across two shared mission restarts. Product activation (`d2coop\d2coop.cfg`, F7 host, F8 leave, Steam join) is packaged in `outputs/release/d2coop-v1181` with scratch-tested install/rollback ([chapter 90](90-release-packaging-and-steam-sessions.md), audit 1185). Open: a product-mode boot from a scratch install, the stability soak, two-account validation (needs the user), in-game UI and the mission transition. Earlier headers below are history.

Previous header (1178, history): Live candidate-npc-combat-v1178 ([identity](../../work/movement-integration/candidate-npc-combat-v1178/identity.json)). Latest (stage 7 started): a hash-guarded release package with byte-identical rollback, tested on scratch targets, and a native Steam session flow. The host creates a lobby with rich presence, opens the invite overlay and listens P2P; a same-account join is correctly refused at the second-account boundary ([chapter 90](90-release-packaging-and-steam-sessions.md), audit 1180). Open: two-account validation (needs the user), product activation without the lab environment, in-game host trigger and UI, a stability soak, and the mission transition. Earlier headers below are history.

Previous header (1171, history): Live candidate-npc-combat-v1171 ([identity](../../work/movement-integration/candidate-npc-combat-v1171/identity.json)). Latest (stage 5 shared progression): a script trigger volume entered by one player is replayed natively in the partner's world through arkCpntTrigger's own enter/exit handler. Visually confirmed: the client got the Wall of Light tutorial, an objective update and hostile territory without moving ([chapter 89](89-mission-progression-discovery.md), audit 1173). Open: the client-originated direction, mission end and next-mission transitions, and stage 7. Earlier headers below are history.

Previous header (1166b, history): Live candidate-npc-combat-v1166b ([identity](../../work/movement-integration/candidate-npc-combat-v1166b/identity.json)). Latest (stage 4 on missions): dynamic NPC relevance. The host-owned bound set follows both players, binding within 55 m and retiring beyond 75 m, and the client follows ([chapter 86](86-mission-npc-authority.md#dynamic-npc-relevance-candidates-11661166b-audit-1169), audit 1169). Mission progression research is in [chapter 89](89-mission-progression-discovery.md). Open: Kiscule progression mirroring and mission transitions, multi-enemy combat with rebinding, and stage 7. Earlier headers below are history.

Previous header (1153c, history): Live candidate-npc-combat-v1153c ([identity](../../work/movement-integration/candidate-npc-combat-v1153c/identity.json)). Latest (stage 6 on missions): after each shared restart, the partner avatar, doors (16), coins, usables and the NPC stream all renew on both peers. Door channels now tolerate transient gaps ([chapter 88](88-mission-shared-death-and-checkpoint.md), audit 1161). Open: a real script autosave trigger, client-side partner visibility after restart, and map transitions. Earlier headers below are history.

Previous header (1153, history): Live candidate-npc-combat-v1153 ([identity](../../work/movement-integration/candidate-npc-combat-v1153/identity.json)). Latest (stage 6 on missions): autosaves are paired. When one peer autosaves, the other mirrors it with the game's own RequestSave, and a shared death/restart returns both players to their own paired mid-mission checkpoint ([chapter 88](88-mission-shared-death-and-checkpoint.md), audit 1157; both origins; visually accepted). Open: a real script autosave trigger, partner avatar and door/coin/NPC renewal across mission restarts, and map transitions. Earlier headers below are history.

Previous header (1147b, history): Live candidate-npc-combat-v1147b ([identity](../../work/movement-integration/candidate-npc-combat-v1147b/identity.json)). Action protocol 73, NPC combat wire 7; bank 70/369 unchanged; all fourteen protected files unchanged. Latest (stage 6 on missions): a checkpoint session armed on a mission map turns either player's native death into a shared death, with both native GameOver screens. Each peer then restarts exactly once from its own per-character mission autosave behind the both-ready barrier, and usable sessions renew ([chapter 88](88-mission-shared-death-and-checkpoint.md), audit 1152; visually accepted for both roles). Native facts: GameOver restart is RestartFromCheckpoint followed by the memory continuation; RequestSave is at 0x14065cc50. Open: paired mid-mission autosaves, partner avatar restoration and door/coin/NPC renewal across mission restarts, and map transitions. Earlier headers below are history.

Previous header (1123e, history): Live candidate-npc-combat-v1123e ([identity](../../work/movement-integration/candidate-npc-combat-v1123e/identity.json)). Action protocol 73, NPC combat wire 7; bank 70/369 unchanged; all fourteen protected files unchanged. Latest (stage 5 on missions): usable sync uses a structural two-state toggle family instead of the Dunwall window name, and has no map gate. 145 toggles pair on Edge of the World. Ordinary native use by the host (open) and by the client (close) converges natively on the peer ([chapter 87](87-mission-shared-usables.md), audit 1145). Door, usable and coin sessions now suspend through the item wheel and tutorial modals. Native use focus is readable at arkCpntPlayerSensorInteraction+0x80. Stage-4 mission combat and corpse authority from 1113d is unchanged. Open: shared player death and checkpoints on missions (stage 6), scripted levers and multi-state devices, and dynamic NPC relevance. Earlier headers below are history.

Previous header (1113d, history): Live candidate-npc-combat-v1113d ([identity](../../work/movement-integration/candidate-npc-combat-v1113d/identity.json)). Action protocol 73, NPC combat wire 7; bank 70/369 unchanged; all fourteen protected files unchanged. Latest (stage 4 on missions): kick and sword hits from either player on a host-authoritative guard commit once and apply once; deaths are shared, and corpses are identical on both peers through corpse authority ([chapter 86](86-mission-npc-authority.md), audits 1110 and 1118). Guards now engage both players: an alerted guard natively targets the client's avatar, and its relayed hits are blocked natively or damage the client player (audit 1122). Open: sustained multi-enemy combat, dynamic NPC relevance, and shared player death/checkpoints on missions. Earlier headers below are history.

Previous header (1108, history): Live candidate-npc-combat-v1108 ([identity](../../work/movement-integration/candidate-npc-combat-v1108/identity.json)). Action protocol 73, NPC combat wire 7; bank 70/369 unchanged; all fourteen protected files unchanged. Latest (stage 4 on missions): the host's native kick and sword hits on a host-authoritative guard commit once on the host and apply once on the client, so guard health converges in both worlds. NPC authority survives the combat behaviour-tree swap ([chapter 86](86-mission-npc-authority.md), audit 1110). Open: client-sourced hits, NPC death outcomes, guards engaging both players, and shared death/checkpoints on missions. Earlier headers below are history.

Previous header (1097, history): Live candidate-npc-combat-v1097 ([identity](../../work/movement-integration/candidate-npc-combat-v1097/identity.json)). Action protocol 73; bank 70/369 unchanged; all fourteen protected files unchanged. Latest (stage 4 on missions): shared NPC authority with an NPC pose bandwidth budget holds through native guard combat on Edge of the World. Guards engage the host and blocking works; enemy-attack replication is active ([chapter 86](86-mission-npc-authority.md), audit 1100). Open: player strike outcomes on bound NPCs, client combat retirement, guards engaging both players, shared death on missions, and tutorial-modal tolerance. Earlier headers below are history.

Previous header (1091, history): Live candidate-npc-combat-v1091 ([identity](../../work/movement-integration/candidate-npc-combat-v1091/identity.json)). Action protocol 73; bank 70/369 unchanged; all fourteen protected files unchanged. Latest: the stage-4 bridge. The shared NPC authority stream runs on mission maps, binding the nearest eligible NPCs to the host, and client replicas follow the host ([chapter 86](86-mission-npc-authority.md)). Before that: partner equipment and item-wheel robustness ([85](85-mission-equipment-and-item-wheel.md)), and pickups ([83](83-mission-shared-pickups.md)) and doors ([84](84-mission-shared-doors.md)) on missions. Next: NPC eligibility for non-idle AI, dynamic relevance, NPC combat with hostile guards on missions, and checkpoint/shared death on missions. Earlier headers below are history.

Previous header (1074j, history): Live candidate-npc-combat-v1074j ([identity](../../work/movement-integration/candidate-npc-combat-v1074j/identity.json)). Action protocol 73, coin wire 7 with catalog-list wire 1; bank 70/369 unchanged. All fourteen protected save/config files unchanged. Latest: partner equipment and power presentation runs on mission maps and survives the native item wheel, verified for both roles ([chapter 85](85-mission-equipment-and-item-wheel.md)). Before that: shared pickups ([83](83-mission-shared-pickups.md)) and doors ([84](84-mission-shared-doors.md)) on missions, water exits ([82](82-water-exit-and-action-stream-hold.md)), and swimming/climbing for both roles ([50](50-swimming-climbing-mode-discovery.md)). Next: checkpoint/shared death on mission maps, the usable-sync intersection, and NPC combat authority on missions. Earlier headers below are history.

Previous header (1059c, history): Live DLL `3B8BBEE7…` (candidate-npc-combat-v1059c) matches [the preserved candidate](../../work/movement-integration/candidate-npc-combat-v1059c/identity.json). Action protocol 73, coin wire 7 with catalog-list wire 1; bank 70/369 unchanged. All fourteen protected save/config files unchanged. Latest: shared pickups now run on mission maps as the intersection of both character worlds; ordinary native pickups by both roles are verified on Edge of the World ([chapter 83](83-mission-shared-pickups.md)), and shared doors work there without code changes, verified for both roles ([chapter 84](84-mission-shared-doors.md)). Before that: water exits and the action-stream hold ([chapter 82](82-water-exit-and-action-stream-hold.md)), and swimming/climbing for both roles ([chapter 50](50-swimming-climbing-mode-discovery.md)). Next: lift the remaining Dunwall-only gates (usables, checkpoint/shared death, Doppelganger visual, props). Earlier headers below are history.

Previous header (1056, history): Live DLL `0352516B…` (candidate-npc-combat-v1056) and probe `EE8ADF67…` match [the preserved candidate](../../work/movement-integration/candidate-npc-combat-v1056/identity.json). Action protocol 73; bank 70/369 unchanged (Corvo `FD50D187…` on the client, Emily `500BEC1D…` on the host). All fourteen protected save/config files unchanged. Latest: water exits bound for both roles, and protocol-73 holds keep the action stream alive through native states no binding classifies ([chapter 82](82-water-exit-and-action-stream-hold.md)); before that, swimming/chain climbing for both roles with bounded visual acceptance ([chapter 50](50-swimming-climbing-mode-discovery.md)). Next: lift the remaining Dunwall-only gameplay gates so the existing coin, inventory, NPC and combat work (chapters 63–80) runs on mission maps. Earlier headers below are history.

Previous header (1042, history): Live DLL `A67A63DD73C6B2F47192F40E15FBC6DD154EED5DBA61A2E2718EFB07E201608B` (candidate-npc-combat-v1042) and probe `D600DA2D0664810F12D41E9B018AAB22F5E2BD238E587FA25D7C6C89830AC79A` match [the preserved candidate](../../work/movement-integration/candidate-npc-combat-v1042/identity.json). Action protocol 72; bank 70/369 (Corvo `FD50D187…` installed on the client, Emily `500BEC1D…` installed on the host). All fourteen protected save/config files unchanged. Latest result: remote native swimming (state 22) and chain climbing (state 23) verified for **both source roles** in Edge of the World with bounded actual-game visual acceptance (1043, 1046, 1049; [audit 1050](../../work/movement-integration/traversal-parity-audit-v1050.json), [chapter 50](50-swimming-climbing-mode-discovery.md)). The 1012 paragraph below is kept as history.

Previous header (history): Live DLL `989B3EBC71AF3D899940D0039028AA2C97768C5CFA6FE8481CF4DB51365D1B5C` (candidate-npc-combat-v1012) and probe `BAFEB687A25126DDFCF385E971AB0F58A5A57F5E23E2D0A40976882085F0C6AA` match [the preserved candidate](../../work/movement-integration/candidate-npc-combat-v1012/identity.json). Semantic protocol 69; enemy-attack packet v3; isolated candidate bank 67/357. All fourteen protected save/config files unchanged.

Live DLL is now 1039 `A5BD34794D97AED913BE6CD6B25355E14C1F380160F07D9D22EAF848FE173EAD` with banks 69/365 and action protocol 71. Isolated co-op now runs on real mission maps through the native `campaign` command: the bring-up is verified in Edge of the World, and remote native swimming is bound and numerically verified ([chapter 50](50-swimming-climbing-mode-discovery.md)). Defense presentation is verified for both roles. Earlier, 1026 shipped banks 68/359 and protocol 70. It adds remote-body presentation of the receiving player's native block impact and timed parry, plus a lab-only observer camera. Both are verified in scoped runs 1025 and 1028c (with host-window video); see [chapter 80](80-native-timed-parry.md). Earlier step (1018): native timed parry against relayed enemy sword hits passes 1017f ([chapter 80](80-native-timed-parry.md)). The native parry window is about 265 ms. Parry-win (blowback) has a 0% native chance in this fixture, so its host path is implemented but unexercised. Before that, held block passed (1013c). The native block is resolved by the attacker's weapon before any damage call (`1406357d0`); 1012 runs that native resolution on the receiving player, so blocked contacts cause no damage and the host's attacker receives the native blocked event. Timed parry now passes (chapter 80); parry-win blowback is unexercised. See [chapter 79](79-enemy-combat-recovery-and-defense.md).

Both roles and both standing throw branches pass network-presentation checks. Both roles additionally pass native mechanism and held-effect checks; same-world source/receiver menu recovery cases pass with fresh tokens. [Detailed grenade evidence and failures](20-grenade-stream-development.md), [native stun continuity](21-native-stun-development.md), [crouched grenade contexts](22-crouched-grenade-development.md) and [airborne grenade contexts](23-airborne-grenade-development.md) record their exact scopes. Native source projectile, explosions and world damage remain local.

[Native elixir development](24-native-elixir-investigation.md) confirms both-role health use and normal-gameplay mana use, plus remote walking/stopping continuity through both consumables. Remote drink sound, resource outcome synchronization and broader interruptions remain incomplete. [Mesmerize development](25-native-mesmerize-investigation.md) and [partner/target findings](26-mesmerize-targets-and-partner-exclusion.md) record body/world/eye presentation, walking and pause evidence. Partner exclusion through item deselection passes v95; ordinary guard targeting passes v99. Scoped readers pass v105/v107, with recorded v107 eye/link maxima of 0.3833/0.4515 ms over 219 calls each. Target beam/trail/head presentation passes v107's 14 checks; precise target alignment and shared NPC gameplay remain incomplete. [Avatar AI lifetime and interruption regression](27-presentation-ai-lifetime.md) records the v109 crash and ownership repair, followed by passing v120/v121 menu recovery with fresh active snapshots. [Cancellation, native lifetime and crouched contexts](28-mesmerize-cancel-lifetime-and-crouch.md) records v123 cancellation, v125's retained failed assertion, corrected v129 native-expiry proof, v131 phase provenance and the v133 whole-body crouch candidate. Individual receipts distinguish runtime checks from visual acceptance. Full movement and playable co-op remain incomplete.

Latest scoped results: [Chapter29](29-airborne-root-continuity.md) records armed/holstered Mesmerize air continuity. [Chapter30](30-native-domino-investigation.md) records native Domino phases, partner exclusion, target marks, directed connections and beam ownership. Domino scoped body/marks, partner exclusion, beams, aim cancellation, target deselection and standing final recovery remain preserved. Paired armed crouchv250/v256 verifies all ordinary/final gestures, continuous root, target removal and standing recovery; railing/darkness limit fine visual acceptance. Native holsteredv253 confirms ordinary phases with sword remaining unequipped in standing and crouch. Standing source passes; crouched source fails inventory audit because native X pickpockets a nearby key (checkpoint/protected14 restore). Candidate258 validates the same equipment identity across reads and enables bank48 empty-hand contexts, but its first paired run exposed a remaining source/wire armed-only gate. Candidate261 shares that equipment-context rule between sender and receiver and uses semantic protocol45/336bytes. Paired tests look away before holstering. Native v263 standing/crouched holstered final observations each pass7 checks; candidate264 accepts their exact400ms scene and grounded recovery with equipment/ownership guards retained. Bank48/301 and protocol45 remain unchanged. Completed paired contexts: domino-context-crouched-holstered-final-v264 PASS; domino-context-crouched-holstered-moving-v263 PASS; domino-context-crouched-holstered-target-v262 PASS; domino-context-crouched-holstered-v261 PASS; domino-context-standing-holstered-final-v264 PASS; domino-context-standing-holstered-v258 FAIL: Native transport/presentation retired: native_state_stale; domino-context-standing-holstered-v261 PASS. Broader air/slide/interruptions, shared NPC authority and gameplay outcomes remain unfinished. No complete-power or final visual acceptance. Refer to individual completed receipts for runtime verification. Failed fixture placements, input/state assertions and their corrections remain preserved. Native NPC positions and shared gameplay outcomes remain unsynchronized; complete movement and playable co-op are unfinished.

1401 actual-game videos are indexed. Movement and playable co-op remain incomplete; autonomous work continues beyond movement.

[Chapter31](31-native-devouring-swarm.md) records native Swarm source, body integration, test repairs and current world-component findings. Grounded armed/holstered Swarm contexts retained.25 cosmetic rats receive strict native transforms through Steam with120ms interpolation, native scale and measured-distance gait.320 passes25 checks: one native summon effect and7 native rat trails, guarded per-frame transforms, finite cleanup, duplicate refusal, bodyweight1.0, checkpoint restoration and protected14.319 trail cleanup succeeded but body eligibility failed when semantic history ended235.5ms before the root pose.320 removes idle full grenade inventory scans; observed source read maximum 100.8959ms -> 10.931ms and send gap 281ms -> 78ms. This single run supports improvement, not a universal latency guarantee. Both grenade throw regressions plus source equip/unequip reader regression pass. Source and peer trails visible in viewed temporal sheets; audio, fine grip and subjective movement acceptance remain unverified. Rat skins/advanced states, targeting marker, death effects and shared gameplay outcomes incomplete. 753 videos indexed; full movement and playable co-op remain incomplete.
[Chapter32](32-native-possession-investigation.md) records empty-target remote body binding, exact target ownership, native human-host entry/exit, fixture corrections, and outstanding remote lifecycle/VFX work. 374 partner exclusion passes8;375 ordinary crouched guard lifecycle passes20 on same DLL/bank54. Other powers, contexts, reciprocal direct interactions and collision exclusion remain unfinished.
[Partner interaction exclusion](34-partner-interaction-exclusion.md): 396: all8 partner sword contexts398/400 pass; ordinary NPC sword targeting399 retained. Rear choke387 scoped evidence preserved. Broad interaction edge-case audit deferred after user requested moving on. No direct partner combat, powers, targeting or contextual interactions. Normal movement collision is explicitly allowed. Broader direct interaction exclusions remain incomplete.
[Emily Doppelganger cast and clone lifecycle](35-doppelganger-cast-and-clone-lifecycle.md): Native493/494 show standing/crouched Emily keeps the sword sheathed throughout the466/1200/666ms Doppelganger phases. 495 appends six complete-body hands-free phase clips to bank57/331, retaining all325 previous payloads; semantic53 accepts the observed native equipment context. 496/497 pass remote body/clone/FX and standing-recovery checks. Standing/crouched armed evidence491 and continuous clone/material-reuse evidence485/486/488 remain preserved. Airborne/cancellation/upgrades/multiple clones, shared outcomes and visual acceptance remain open; whole movement/playable co-op incomplete. Native cancellation505: confirmed X-with-LT cancels with no source activation/mana/clone scene and no remote clone/FX;503 standing armed and504 crouched holstered recover full body. Wrong-input499/500 and array-assertion502 failures are preserved. Native air, other interruptions, upgrades/multiple clones, late join and shared gameplay remain unfinished. Airborne517: diagnostic512 passes paired513/510,20checks each; earlier509 landing dropout remains unresolved. Actual source/remote recordings reviewed in temporal samples; no final visual acceptance.
<!-- /GRENADE-LIVE-V55 -->



The active implementation is a guarded two-instance Dunwall development lab. Corvo and Emily use their native local first-person controls, cameras and arms. Separately owned native Doppelganger rigs represent the remote player. Native Steam networking carries state; there is no finished native host/join interface or verified two-account online session.



<!-- BENDTIME-CURRENT -->

## Current movement continuation



Updated 2026-10-11T22:41:25.511987+02:00. Measured live proxy `B25CACBF0DE2AF36CB52E538E4499474063A4BA8D9AB6479FC903EF98811D486`; probe `B3B556129D9E21526C59A2D117D8C6C35D91B28A707B049BB9CEE00BD42D46DB`. [Current code milestone](../../work/movement-integration/candidate-network-failure-fx-v48), [bank pins](../../work/natural-moveset/outofmana-context-bank-manifest-v46.json), [18-file integrity report](../../work/handoff-integrity-verification.json).



Corvo Bend Time uses exact native scene clocks over Steam, with scoped ground/air and same-world pause recovery evidence. Four standing/crouched and drawn/holstered failure-response takes pass twelve checks each in v46. Manual native failure hand FX pass six checks each for Corvo and Emily in v47; viewed temporal samples show hand smoke without obvious observer blur. v48 binds Corvo's effect once to the delayed native failure event, with duplicate/late-event rejection. Consult each take for actual runtime outcome. Emily automatic failure binding, exact tattoo alignment, continuous visual quality, contact/clearance and broader movement coverage remain open.



Prior milestones are retained. Whole movement completion remains false. Counts of clips or videos do not establish accepted movements. See [failure/recovery findings](16-power-failure-and-resume.md), [full state](../../work/current-task-state.json) and [dashboard](http://127.0.0.1:8769/review.html).

<!-- /BENDTIME-CURRENT -->



<!-- HIGH-THROW-CURRENT -->

## Historical high-throw continuation (v34)



Updated: 2026-10-11T20:01:13.677759+02:00. Live experimental proxy: `67010EF9F82D61BB6D0B415A399D209AFF7E5F4A208906F0876395223EB55D85`.



Probe: `A0D22CD8EA4203D64AD5F16DB2A55287E45F9D62BAC74F84F16B350727BF8DCD`. The bank is D2MOVE36 with234clips; all233 prior clip payloads remain byte-identical. See [bank hashes](../../work/natural-moveset/high-throw-bank-manifest-v31.json), [native throw findings](13-native-body-throw.md) and the per-take results in [current state](../../work/current-task-state.json).



The v33 physics milestone passes23 pickup checks and21 throw checks for each named Corvo/Emily standing wall-facing take. One native impulse is queued after scene retirement and ragdoll transition. Actual videos show forward travel, but hand/body alignment and complete throw quality remain unaccepted. The separately archived v34 candidate tests retaining the attachment until the ragdoll boundary. Read its own result before treating it as validated.



Preserved rollback builds: [v33 throw physics](../../work/movement-integration/candidate-high-throw-v33) and [v30 low-drop/stance](../../work/movement-integration/candidate-carry-stance-anchor-v30). Original Steam assets and14 protected save/config files are checked independently; runtime experiments are limited to the physical clone.

<!-- /HIGH-THROW-CURRENT -->



## Preserved v30 movement milestone



| Component | Current identity |

|---|---|

| Exact game executable SHA-256 | `B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180` |

| Preserved v30 proxy SHA-256 | `891A60B35E162DEE695B7D9C0E7ED67EC5F1CE5C9A05DA4BCD932D4989BD7727` |

| Preserved probe v26 SHA-256 | `CDD8D1C8BA56516EACB688F307E78D0AD009EAE891D4B57A0C010C23C5D146AF` |

| Bank format | D2MOVE35, 233 clips, crouched-pickup contact v3 |

| Corvo bank SHA-256 | `752B441F3BB5C137E3506ED82B11D6AB9738196DF9FD151E9A73F0818908A677` |

| Emily bank SHA-256 | `31035CEC92EA7678082D3D085F71F81C84C5D7FD1BBA64F3B012867EAAEB7DEB` |

| Current source | [outputs/d2-coop-lab/src](../../outputs/d2-coop-lab/src) |

| Archived v30 source and binary | [candidate-carry-stance-anchor-v30](../../work/movement-integration/candidate-carry-stance-anchor-v30) |

| Bank manifest | [crouched-pickup-bank-manifest-v3.json](../../work/natural-moveset/crouched-pickup-bank-manifest-v3.json) |

| Dashboard | [local review](http://127.0.0.1:8769/review.html) |



The dashboard contains 105 functional movement requirements, 505 motion candidates with role data, 483 authoring recordings and 296 actual game recordings after the v30 and native-throw reference indexing steps. Two of those newest recordings are first-person source references, not remote throw playback. 100 requirements have candidate material. These counts do not imply 95% completion: contextual integration, transitions, effects, authority, visual review and full role coverage remain incomplete. Use the regenerated inventory for counts after subsequent work.



## What the recent carry work establishes



The native source player picks up the known Mayhew body using ordinary controller input. Its real Carry45 preparation/hoist/loop state drives a bounded remote carrier sequence and a separately owned victim. The authored bank controls pose; the existing movement stream owns world position. On native low drop, the visible pose is synchronized to the physics bodies before ragdoll ownership returns.



v30 corrects a stance-dependent shoulder offset. v29 held the body roughly 47 cm away from the desired shoulder target after a stance change. Recomputing the anchor using the current shoulder brings the final measured residual to about 1.7 cm in the two selected cases. Corvo crouched-to-standing passes 51 scoped checks; Emily standing-to-crouched passes 48. Both actual-video temporal sheets were inspected.



This is not final carry acceptance. Corvo's standing supporting-wrist target residual remains approximately 10.5 cm; Emily's crouched residual is 3.9 cm. Targets include authoring offsets and are not a substitute for palm/finger surface inspection. Other bodies, turning, throws, interrupted ownership and general campaign integration remain open.



The first Corvo v30 capture lacked an observed received preparation packet. Its failed validator is preserved. A new harness samples the stream before slow native reads and attachment setup; a fresh run passes. No successful packet observation was invented for the original take.



## Existing broader capabilities and limits



| Area | Established evidence | Still required |

|---|---|---|

| Two instances | Isolated profiles; windowed side-by-side; background simulation; finite controller input | Fresh-launch and long-run coverage beyond the fixture |

| Shared recovery | Either reload/death coordinates native screens and one retry for pre-staged canonical Dunwall SLOT82; roles and both-ready gate restored | Arbitrary checkpoints, save transfer, campaign progression and complete cinematic recovery |

| Native transport | Game Steam user/pipe and SteamNetworkingSockets012, same-PC connection, freshness and identity checks | Host/join/invites, NAT/relay behavior and separate-account online testing |

| Avatar presentation | Native clone rigs, owned equipment, smoothing, action and bank experiments | Complete automatic gameplay integration and accepted movement library |

| Friendly fire | Attributed native health damage filtered; real sword contacts, reload and positive enemy controls tested | Every power/projectile, indirect hazards, knockouts and pre-damage choreography |

| World/combat | Bounded guard perception, damage relay, health and death experiments with duplicate/lifetime tests | General host authority, all AI, physics, inventory, quests and doors |

| Native UI | Relevant IGGY resources exported; native pause/death/retry reused | Integrated native session setup, character choice and connection recovery |



Historical fields such as the top-level `LoaderSha256`, old `Live` snapshots and early `Implemented:false` values in [current-task-state.json](../../work/current-task-state.json) are retained provenance, not reliable live status. Prefer `MovementIntegration.LatestContinuation`, the archived identity and fresh process/file checks.





<!-- EQUIPMENT-GRENADE-V52 -->

## Latest measured live build: grenade equip v52



Updated 2026-10-11T22:53:36.465244+02:00. This supersedes earlier live-build descriptions on this page. Live proxy `C43B0FB0FD03AECF8D85A7312282F688879BF725DE4ACD5BD7AB6584C15634E1` and probe `B08645C92ADEA9252563461A70EFD422C78AABF57D2B969A9E39B8911E3C1E9D` match [the preserved v52 candidate](../../work/movement-integration/candidate-grenade-equip-v52/identity.json). Protocol 36 and standard D2MOVE41/251 v46 banks are unchanged. [All eighteen protected file checks pass](../../work/handoff-integrity-verification.json).



Both characters pass seven scoped native grenade equip/unequip checks, using ordinary holster/draw input. The local first-person recordings are reviewed and indexed; they do not demonstrate remote grenade movement. Exact native scene ownership is the next binding step. Failed takes are preserved. [Grenade findings](18-native-grenade-investigation.md).



Earlier paired-role equipment, v50 crouched cast guard candidate and v51 failure-effect lifecycle evidence remains valid within each take. The pause visibility gap remains open. There are 411 indexed actual-game videos; this is not a completion percentage. Movement and playable co-op remain incomplete, with the goal active beyond movement.

<!-- /EQUIPMENT-GRENADE-V52 -->

