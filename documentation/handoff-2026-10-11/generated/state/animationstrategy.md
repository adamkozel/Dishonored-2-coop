# State record: AnimationStrategy

Original section of [current-task-state.json](<../../../../work/current-task-state.json>). Captured 2026-10-14T01:55:24.397473+02:00.

This is a verbatim structured record, not a new validation. Earlier fields named Current/Live can describe historical builds.

````json
{
  "At": "2026-10-10T00:42:46.4758843+02:00",
  "Decision": "Stop pose-by-pose animation polishing as the production strategy; evaluate full-body authoring and offline AnimGen generation",
  "UserFeedback": "Current visual quality and iteration cost are unacceptable",
  "Preserved": [
    "work/friendly-fire-345C",
    "work/sword-grip-fit/789d-grip-milestone",
    "work/sword-grip-fit/2997-combined-research"
  ],
  "Keep": [
    "Native first-person controls and rigs",
    "Native Steam transport",
    "Owned Doppelganger characters and native equipment",
    "Guarded remote pose playback and original asset/save protection"
  ],
  "Engine": {
    "LauncherRegistered": "UE 5.8.3",
    "RegisteredPath": "C:\\Users\\adako\\Documents\\Codex\\Tools\\UE_5.8",
    "EditorExists": true,
    "ExistingProject": "C:\\Users\\adako\\Documents\\Unreal Projects\\MyProject",
    "ExistingProjectModified": false,
    "FreeDiskGB": 105.96
  },
  "KnownPipeline": {
    "NativeSkeletonNamesParentsBindTransformsVerified": true,
    "EvaluatedFirstPersonPoseCapture": true,
    "CompleteFirstPersonClipBatchExportVerified": false,
    "ExternalBakedMotionPlayedOnNativeAvatars": true,
    "NativeSkinnedCharacterExportToAuthoringToolVerified": true,
    "AnimGenOutputBakingAndExportVerified": true,
    "AnimGenRuntimePortToD2Verified": false,
    "AnimGenExportMechanism": "Evaluated pose/root/contact JSON into Blender; generated FBX/Sequencer baking not established",
    "OfflineGeneratedAnimGenPosePlaybackInD2Verified": true
  },
  "EvaluationOrder": [
    "Resolve actual Unreal Editor installation and compatible AnimGen sample",
    "Prove skeletal motion recording/export on a supplied example before training",
    "Establish a viewable skinned D2 target character and weapon rig in the authoring environment",
    "Retarget and visually validate a coherent full-body locomotion set; use first-person poses as timing/contact references",
    "Bring one validated result into the isolated D2 lab and compare equal cameras and travel",
    "Expand only after the same motion retains quality in D2"
  ],
  "Acceptance": [
    "Foot contact and travel agree",
    "Hands stay on the hilt",
    "Body posture and proportions remain coherent",
    "Start/stop/turn/stance transitions look natural",
    "No first-person animation or network-root replacement"
  ],
  "UnresolvedRuntimeBug": "nativePlayerMotion rejects two-state Sneaking+Slash/Block; identified, not fixed; keep separate from motion quality evaluation",
  "Sources": [
    "https://www.fab.com/listings/df37eb46-09bf-4604-9307-cdc39c769790",
    "https://dev.epicgames.com/documentation/unreal-engine/API/Plugins/AnimGen/UAnimGenController",
    "https://dev.epicgames.com/documentation/unreal-engine/API/Plugins/AnimGen/UAnimGenControllerTrainingSettin-"
  ],
  "LatestProof": "work/animgen-feasibility/result.json",
  "UnrealBlocker": {
    "resolved": true,
    "reason": "UE 5.8.3 installation and .NET SDK repair complete; editor module compiled and actual generated poses captured",
    "existingProjectsModified": false
  },
  "WorkingVersions": "work/animgen-feasibility/working-versions.json",
  "SamplePipelineStatus": "work/animgen-feasibility/sample-pipeline-status.json"
}
````
