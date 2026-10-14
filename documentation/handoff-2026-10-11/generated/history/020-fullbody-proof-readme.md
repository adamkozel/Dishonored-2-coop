# Historical note: work/fullbody-proof/README.txt

Original: [work/fullbody-proof/README.txt](<../../../../work/fullbody-proof/README.txt>)

SHA-256: `2AED8FE967BEB1EDCE7523A0BAA0A0266D0E431FCAB738441E125F5D65BB6F4C`; 8209 bytes.

````text
DISHONORED 2 — ONE FULL-BODY AUTHORING PROOF
2026-10-09

Watch d2-fullbody-sequence.mp4 for the actual isolated D2 run, and
blender-sequence.mp4 for the editable authoring sequence. Open
corvo-fullbody-authored.blend in Blender 5.2.0 LTS.

Result
------
One Corvo ready / short forward advance / sword guard / recovery sequence is
authored as a complete native skeleton in Blender and played on the owned
remote Doppelganger in D2. The guard's wrist and blade orientation/timing use
captured native first-person motion as reference. The legs, balance and
offhand are authored offline. This is not an AnimGen output or a Mixamo clip.
The first-person rigs, camera, controls, native sword equipment and Steam root
transport are retained. No animation damage events are emitted by playback.

The authored action is six seconds / 361 samples / 60 Hz / 215 joints. Runtime
step phase follows actual forward displacement received through the existing
native Steam connection. Root and anchor are not written. The complete clip
is sampled after native IK, then native correctives and weapon publication
run. Entry/exit blend with the native pose over 300 ms.

Video interpretation
--------------------
The D2 recording shows native baseline, the authored sequence and return to
native baseline. The existing baseline's relaxed/open-hand idle is still
visible before/after the proof; it is not the authored ready or recovery.
The camera includes head and feet throughout the sequence. Screenshots were
captured at about 14.25 Hz and encoded using their actual timestamps; the
30 fps container does not mean 30 unique captures per second. No speedup or
audio. See controller-video-4/timestamps.json and Encode-Evidence.ps1.

The Blender preview uses the high-detail native non-cloth body and unmasked
head, approximate corrective helpers and flat materials. D2 retains its
masked Doppelganger, cloth, textures and exact native correctives. Comparison
cameras show the same side of the character, but are not an exact optical
match. Low-detail complete body is also preserved, hidden from rendering.

Visual review
-------------
Reviewed ready, entry, both advancing foot phases, settled stance, guard and
recovery in native captures, plus the skinned authoring render. Knees/boots
retain their shape; the guard hand closes around the hilt; the whole body
supports the action. This is a useful proof to review before scaling up.
The short advance and guard are deliberately narrow: no claim of final
combat acting, turning, running, crouching, Emily support or a complete set.
The offhand and narrow stance are still fairly restrained. A close-up sword
occlusion check through every future action remains necessary. User visual
acceptance is pending; the stable production animation path was not replaced.

Reproduction
------------
1. Use the saved authored .blend (its rest axes are repaired), with the pinned
   inputs/ directory, clean-take-2 reference captures and native mesh JSONs.
   Run author_sequence.py through the Blender MCP Python execution tool.
   The script rebakes only the named proof rig and sword in the proof scene.
   It must reproduce clip SHA-256 below. It renders five representative poses.
2. For a fresh empty authoring scene, run import_native.py first. It refuses
   duplicate proof rigs and verifies every native rest matrix. It creates
   the proof base .blend, then author_sequence.py adds motion and sword.
   Do not use the earlier corvo-body.fbx (failed exporter) or the old base
   corvo-fullbody-proof.blend as an accepted rig: use the final authored file
   or regenerate with the fixed importer.
3. render_preview.py regenerates the matching-camera authoring video frames.
   Run using a globals dictionary containing __file__ = its absolute path.
4. Runtime DLL is F4F below. The fixed clip filename is
   work/lab-instances/client/coop-status.json.authoring.bin.
   Commands accepted by Send-D2Command.ps1: coop_authoring_proof play,
   hold_ready, hold_guard, off. They are developer-only and bounded to 10 s.
5. Both isolated physical-copy instances must be ready, with native Corvo on
   host and Emily on client, owned Doppelgangers, exact follow/locomotion on,
   body stream and competing shadow action/stream off. The Corvo sword must
   already be equipped/started. No mutation of original Steam files required.
6. Replay-Controller-Proof.ps1 -Take <new-name> makes one settled placement,
   starts the clip, and applies native host controller LeftY 9750 for 1500 ms.
   Never overwrite an existing take. Position travels through native movement
   and the existing Steam stream. Pair with Computer Use window capture, as
   used for controller-video-4. Repeated setviewpos is not a motion driver.
7. Build: outputs/d2-coop-lab/Build-D2Lab.ps1. Clip loader checks:
   work/build/Release/d2_authoring_clip_tests.exe <path-to-clip>.
   Runtime checks: Test-ProofSafety.ps1 (requires isolated ready instances).

Validation and exact versions
-----------------------------
Blender 5.2.0 LTS; FFmpeg 9.0.1; Void Explorer library 1.2.0.0.
Native exe SHA-256:
B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180
Runtime DLL SHA-256:
F4F035D954BFC39F59E170E494C6ED1B0093E332C55167649D1C43F3C5A1817D
Clip SHA-256:
109956C449EC4AB9DA9D68F944C173F844D391074B3A5D3310264B9F180484FF
Layout hash (64-bit exact hex): 0x00db32d37719931f

controller-take-4 / runtime-verification.json: 385 coherent final-publication
samples, 0.4074097 m actual advance, zero vertical root drift, zero anatomical
translation changes. Stance foot axis ranges <= 7.3 mm; final left contact
constant. These exclude entry/exit blend and do not substitute for visuals.
No playback refusals during the four successful controller-driven takes.
The safety tests deliberately trigger refusals. Both native pause paths,
wrong-role scope, explicit cancel and invalid magic checks pass. A no-motion
play attempt expires at its fixed 10-second bound. Stale-network/load guards
exist in code, but were not separately exercised for this new proof.
Current F4F native first-person regression: both arm rigs, native slash and
health 56 passed (research/player-arm-observation-verification.json).
Baseline integrity: all eight protected original files, installed executable,
Steam DLL and staged loader match. Earlier 345C / 789D / 2997 archives retained.

Important limitations
---------------------
This is a bounded developer experiment, not automatic gameplay integration.
The source player really drives the advance, but the guard/recovery portion
is scheduled by the proof, not yet selected by incoming combat action state.
Single forward step on flat ground; no arbitrary speed/direction/terrain.
Native helper recomputation is required; Blender helper preview is approximate.
The existing crouched-combat multi-state decoder bug remains separate/unfixed.
Native UI/session setup, full moveset, campaign and friendly-fire gaps remain.
No new game UI was introduced. No replacement Steam networking was introduced.

Unreal / AnimGen blocker
-----------------------
UE 5.8.2 is registered in Epic Launcher but its editor directory is absent.
Repair failed IS-0002-DP-01; Windows denied creating its missing Program Files
path. Automatic approval review rejected removal of the stale entry because
the confirmation deletes/uninstalls engine data. That removal was cancelled
and not retried. Existing Unreal projects were not modified. Blender supplied
the authoring fallback. No AnimGen generation, training or export is claimed.

Rejected investigations retained for provenance
---------------------------------------------
runtime-take-1/2 repeatedly drove setviewpos and produced height oscillation;
do not use them as evidence of movement quality. controller-video-3 missed
the action due to capture scheduling; controller-video-4 is the final take.
The initial Blender importer assigned a matrix to a zero-length edit bone,
losing rest axes and distorting skin. Fixed by setting length before matrix,
then verified full rest-matrix error < 1e-5. A global pose check alone had
missed the inverse-bind error. Confirmed setviewpos findings are saved in the
persistent Ghidra program at 0x140886d10.

````
