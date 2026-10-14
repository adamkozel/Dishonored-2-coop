# Development history and decisions

This chronology is reconstructed from preserved notes, per-build reports, source archives and user steering. [Historical notes](generated/historical-notes.md), [state sections](generated/state/README.md) and [milestone inventory](generated/milestones.md) retain much greater detail and exact report paths. Dates on copied records refer to those records, not a new audit of their claims.

## October 8: baseline, dual-instance lab, transport and recovery

The Steam installation was first tested by the user. Development created a source-built `dinput8.dll` loader and isolated profiles. The exact retail mutex was scoped by role, the native focus-loss pause branch and window positioning were adjusted inside the test processes, and a finite controller packet path allowed independent automation without physical controllers. Windowed low-quality instances were arranged side by side.

Native Steam transport was acquired through the game's existing user/pipe and Steam client rather than replacing the original Steam DLL. Engine-thread command execution, player and world observations, map/epoch identities and remote presentation experiments followed. Canonical checkpoint work established that changing save metadata was insufficient; native role restoration before gameplay and a both-ready loading barrier were needed. Rejected metadata and intermediate checkpoint fixtures remain archived.

Shared checkpoint/death experiments eventually used identical pre-staged SLOT82 saves, deduplicated requests and restored Corvo/Emily. The scope remained one Dunwall checkpoint: it did not solve arbitrary save transfer or the campaign.

## October 9: native world, equipment and Doppelganger pivot

The lab progressed through world identity/health readers, short AI decision/idle holds, one-guard pose links, relationship/perception experiments, sword damage relay, nonfatal damage and native death outcome studies. Tests guarded leases, pauses, world replacement, duplicate packets and actual peer termination. These are bounded prerequisites, not a complete shared simulation.

Early remote avatars used story-character NPCs. Corvo had an authored tutorial sword; Emily's story inventory contained accessories without a sword. Native inventory and attachment analysis established entry/component lifecycle and child-frame update requirements. Separate first-person arm/hand streams proved transfer accuracy, but the visual grip was unsatisfactory.

Ghidra, Java and isolated Python were installed and pinned. A persistent exact-executable project was imported and verified with real lookup, xref and decompilation. Void Explorer/Installer libraries enabled exact asset and skeleton inspection, then isolated resource packs. REA was assessed and deferred because it duplicated static reads without solving the Windows runtime blocker.

The user selected native Doppelganger rigs as the preferred remote presentation. Custom uniquely named definitions removed unwanted clone lifecycle/AI components while retaining native animation and equipment. A malformed MD6 declaration initially left controllers null and caused native foot-IK faults. Correcting fixed parser order restored valid controllers and removed the temporary diagnostic IK disable.

Real native Slash/Block and Jump/Fall states drove clone animations through Steam. Direct snapshot root updates visibly stuttered; 80 ms interpolation and matching action delay improved native/render transform cadence. Recovery preserved clone identity for the fixed shared checkpoint. Whole-body and lower-body crouch reconstruction experiments exposed posture and helper-bone problems.

The friendly-fire health-entry filter blocked attributed player/owned-clone damage while preserving enemy positive controls. Unattributed hazards, non-health actions and complete power coverage remained unverified.

## Mixamo crouch experiments

The selected donor was Mixamo **Crouched Walking**, description **Crouched Walk**, not Rifle Crouch Walk. The user preferred that posture, asked for slower travel-matched playback, mirroring so the sword hand leads, improved crouched combat grip and Posture 100. The user explicitly approved retaining Corvo Arm-Space 70 and Emily 50. In-place export preserved network ownership of position.

Blender MCP, FBX import, retargeting, native-length correction and pose-bank playback were validated. These iterations improved the crouch, but the user rejected the overall appearance and the cost of reconstructing every motion this way. They did not approve the result as a production moveset. Later first-person-inspired authoring also looked unnaturally anchored, leading to the explicit requirement for natural full-body motion with first-person data used only as inspiration.

## October 10: full-body authoring and AnimGen proof

A complete-body authoring-to-D2 proof was preserved separately. Unreal installation and build blockers were resolved, including engine repair and the required .NET SDK. The actual experimental Epic AnimGen controller from the supplied example generated a locomotion candidate. Evaluated native pose/root/contact data was exported through a helper after the generated skeletal-mesh FBX route asserted. A separate supplied-animation FBX round trip was not mislabelled generated-motion evidence.

The candidate was retargeted onto Corvo's native 215-joint rig in Blender and exported as D2PROOF2. Runtime phase followed actual travel; root/anchor remained identity. The preserved first proof used FP wrist/blade constraints, later rejected as the default artistic approach. Source/video provenance documents that approximately 12 Hz actual captures encoded at 30 FPS were not 30 Hz footage.

The library expanded using native assets, generated motion, Mixamo donors and authored transitions. Corvo's calf/boot deformation was investigated separately at preview and native post-IK stages. Source candidates and videos remained distinguishable from actual gameplay bindings.

## October 11: movement integration and current body-handling work

Integration expanded gait/turn/stance, traversal, equipment, ranged and power presentation, including native effect investigations. The generated requirement and evidence records provide exact current scopes. Important recent corrections include native knee-plane recovery after IK, native feet/terrain handling, keyhole/power context, ranged prop/muzzle state and body handling.

Recent carry revisions are particularly well isolated:

| Revision | Change and evidence |
|---|---|
| v25 | Supported pickup-to-loaded settle using a native source clock and a 350 ms transition |
| v26 | Hide/show the exact native sword inventory child during holster/carry; preserve first-person |
| v27 | Synchronize visible pose to rigid bodies before ragdoll release; fix the rejected 5.35 m launched-corpse case |
| v28 | D2MOVE35 adds separate crouched hoist; exact Mayhew native reference uses a 0.674 m lower victim root; initial contact and orientation still faulty |
| v29 | Keep native anchor rotation independent of current victim-root feedback; fix approximately 63-degree drift; refit contact v3; stance change then exposes approximately 47 cm load offset |
| v30 | Recompute load position from the moving shoulder through stance changes; both selected role/direction cases pass; grip and full carry remain unfinished |

The native throw investigation continues alongside this documentation. See [current session](11-current-session.md) for its actual evidence rather than assuming every native drop variant is a throw.
