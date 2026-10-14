#define WIN32_LEAN_AND_MEAN
#define DIRECTINPUT_VERSION 0x0800
#include <windows.h>
#include <dinput.h>
#include <iostream>
#include "lab_input.h"
#include "coop_protocol.h"
#include <limits>

int wmain(int argc, wchar_t** argv) {
    if (argc != 2) { std::cerr << "Usage: d2_proxy_smoke absolute-dinput8.dll-path\n"; return 2; }
    SetEnvironmentVariableA("D2COOP_LAB_ROLE", "host");
    HMODULE proxy = LoadLibraryW(argv[1]);
    if (!proxy) { std::cerr << "Proxy load failed: " << GetLastError() << '\n'; return 3; }
    HANDLE gameMutex = CreateMutexA(nullptr, FALSE, "Dishonored 2");
    HANDLE scoped = OpenMutexA(SYNCHRONIZE, FALSE, "Dishonored 2-D2CoopLab-host");
    if (!gameMutex || !scoped) { std::cerr << "Game mutex was not scoped\n"; return 4; }
    CloseHandle(scoped);
    CloseHandle(gameMutex);
    char otherName[128]{};
    sprintf_s(otherName, "D2CoopLabSmoke-%lu", GetCurrentProcessId());
    HANDLE otherMutex = CreateMutexA(nullptr, FALSE, otherName);
    HANDLE unchanged = OpenMutexA(SYNCHRONIZE, FALSE, otherName);
    if (!otherMutex || !unchanged) { std::cerr << "Unrelated mutex changed\n"; return 5; }
    CloseHandle(unchanged);
    CloseHandle(otherMutex);
    using DirectInputProc = HRESULT (WINAPI*)(HINSTANCE, DWORD, REFIID, LPVOID*, LPUNKNOWN);
    auto create = reinterpret_cast<DirectInputProc>(GetProcAddress(proxy, "DirectInput8Create"));
    IDirectInput8W* input = nullptr;
    const HRESULT status = create ? create(GetModuleHandleW(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8W, reinterpret_cast<void**>(&input), nullptr) : E_FAIL;
    if (FAILED(status) || !input) { std::cerr << "DirectInput forwarding failed: 0x" << std::hex << status << '\n'; return 6; }
    input->Release();
    LabInputPacket packet{0x49433244,1,40,1,2000,XINPUT_GAMEPAD_A,0,0,12000,0,0,0,0};
    XINPUT_GAMEPAD pad{};
    if(!decodeLabInput(packet,1000,pad) || pad.wButtons!=XINPUT_GAMEPAD_A || pad.sThumbLX!=12000) return 7;
    if(decodeLabInput(packet,2000,pad) || pad.wButtons || pad.sThumbLX) return 8;
    packet.reserved=1;
    if(decodeLabInput(packet,1000,pad)) return 9;
    packet.reserved=0; packet.expiresFileTime=600001001;
    if(decodeLabInput(packet,1000,pad)) return 10;
    CoopPosePacket pose{};pose.role=1;pose.sequence=1;pose.session=1;pose.worldEpoch=1;pose.cinematicEpoch=1;pose.mapId=coopMapId("campaign/test");pose.flags=1;
    pose.orientation[0]=pose.orientation[4]=pose.orientation[8]=1;pose.gameLogicState=1;pose.cinematicMetadataValid=1;
    if(!validCoopPose(pose,0))return 11;
    const auto valid=pose;
    pose.build=0;if(validCoopPose(pose,0))return 12;pose=valid;
    pose.role=0;if(validCoopPose(pose,0))return 13;pose=valid;
    pose.position[0]=std::numeric_limits<float>::quiet_NaN();if(validCoopPose(pose,0))return 14;pose=valid;
    pose.size=95;if(validCoopPose(pose,0))return 15;pose=valid;
    pose.orientation[4]=0;pose.orientation[3]=1;if(validCoopPose(pose,0))return 16;pose=valid;
    pose.orientation[0]=-1;if(validCoopPose(pose,0))return 17;pose=valid;
    pose.flags=2;if(validCoopPose(pose,0))return 18;pose=valid;
    pose.mapId=0;if(validCoopPose(pose,0))return 20;pose=valid;
    pose.version=1;if(validCoopPose(pose,0))return 21;pose=valid;
    pose.gameLogicState=17;if(validCoopPose(pose,0))return 22;pose=valid;
    pose.pendingGameLogicState=17;if(validCoopPose(pose,0))return 23;pose=valid;
    if(!coopGameplayReady(pose))return 24;
    pose.gameLogicState=5;if(!validCoopPose(pose,0)||coopGameplayReady(pose))return 25;pose=valid;
    pose.gameLogicState=2;if(!validCoopPose(pose,0)||coopGameplayReady(pose))return 26;pose=valid;
    pose.pendingGameLogicState=5;if(!validCoopPose(pose,0)||coopGameplayReady(pose))return 27;pose=valid;
    pose.cinematicMetadataValid=2;if(validCoopPose(pose,0))return 28;pose=valid;
    pose.cinematicMetadataValid=0;pose.cinematicSignature=1;if(validCoopPose(pose,0))return 29;pose=valid;
    pose.cinematicNodeCount=9;pose.cinematicSignature=1;if(validCoopPose(pose,0))return 30;pose=valid;
    pose.cinematicNodeCount=1;if(validCoopPose(pose,0))return 31;pose=valid;
    pose.cinematicSignature=1;if(validCoopPose(pose,0))return 32;pose=valid;
    pose.cinematicNodeCount=1;pose.cinematicSignature=1;if(!validCoopPose(pose,0)||coopGameplayReady(pose))return 33;pose=valid;
    pose.cinematicMetadataValid=0;if(!validCoopPose(pose,0)||coopGameplayReady(pose))return 34;pose=valid;
    pose.version=3;if(validCoopPose(pose,0))return 35;pose=valid;
    const CoopCinematicKey keysA[]={{coopMapId("scene_b"),77},{coopMapId("scene_a"),2012}};
    const CoopCinematicKey keysB[]={keysA[1],keysA[0]};
    CoopCinematicKey changed[]={keysA[0],keysA[1]};changed[1].nodeId++;
    const auto signature=coopCinematicSignature(keysA,2);
    if(!signature||signature!=coopCinematicSignature(keysB,2)||signature==coopCinematicSignature(changed,2)||
       coopCinematicSignature(keysA,0)||coopCinematicSignature(keysA,9))return 36;
    pose.version=4;if(validCoopPose(pose,0))return 37;pose=valid;
    pose.cinematicEpoch=0;if(validCoopPose(pose,0))return 38;pose=valid;
    pose.cinematicCanSkip=1;if(validCoopPose(pose,0))return 39;pose=valid;
    pose.cinematicSyncEnabled=2;if(validCoopPose(pose,0))return 40;pose=valid;
    auto peer=valid;peer.role=0;peer.session=2;peer.worldEpoch=7;peer.cinematicEpoch=9;
    pose.gameLogicState=peer.gameLogicState=5;pose.cinematicNodeCount=peer.cinematicNodeCount=1;
    pose.cinematicSignature=peer.cinematicSignature=signature;pose.cinematicCanSkip=peer.cinematicCanSkip=1;
    pose.cinematicSyncEnabled=peer.cinematicSyncEnabled=1;
    pose.sequenceMetadataValid=peer.sequenceMetadataValid=1;pose.sequenceCount=peer.sequenceCount=1;
    pose.sequenceSignature=peer.sequenceSignature=pose.sequenceBoundaryId=peer.sequenceBoundaryId=77;
    if(!coopSharedSkipReady(pose,peer))return 41;
    CoopSkipPacket skip{};skip.role=peer.role;skip.sequence=1;skip.sourceSession=peer.session;skip.targetSession=pose.session;
    skip.sourceWorldEpoch=peer.worldEpoch;skip.targetWorldEpoch=pose.worldEpoch;
    skip.sourceCinematicEpoch=peer.cinematicEpoch;skip.targetCinematicEpoch=pose.cinematicEpoch;
    skip.mapId=pose.mapId;skip.cinematicSceneId=signature;
    skip.sequenceBoundaryId=pose.sequenceBoundaryId;
    if(!validCoopSkip(skip,pose,peer))return 42;
    const auto validSkip=skip;
    skip.targetCinematicEpoch++;if(validCoopSkip(skip,pose,peer))return 43;skip=validSkip;
    skip.sourceSession++;if(validCoopSkip(skip,pose,peer))return 44;skip=validSkip;
    skip.targetWorldEpoch++;if(validCoopSkip(skip,pose,peer))return 45;skip=validSkip;
    skip.cinematicSceneId++;if(validCoopSkip(skip,pose,peer))return 46;skip=validSkip;
    skip.sequenceBoundaryId++;if(validCoopSkip(skip,pose,peer))return 69;skip=validSkip;
    skip.version=2;if(validCoopSkip(skip,pose,peer))return 70;skip=validSkip;
    peer.cinematicSignature++;if(validCoopSkip(skip,pose,peer))return 47;peer.cinematicSignature--;
    peer.cinematicSyncEnabled=0;if(validCoopSkip(skip,pose,peer))return 48;peer.cinematicSyncEnabled=1;
    peer.cinematicCanSkip=0;if(!validCoopSkip(skip,pose,peer))return 49;peer.cinematicCanSkip=1;
    pose.cinematicCanSkip=0;if(validCoopSkip(skip,pose,peer))return 50;pose.cinematicCanSkip=1;
    skip.version=1;if(validCoopSkip(skip,pose,peer))return 51;skip=validSkip;
    pose.mapId=peer.mapId=coopMapId("campaign/dunwall/escape/tower/dunwall_escape_tower_p");
    const CoopCinematicKey corvoScene[]={{coopMapId("ld_kiscule_coup"),190612608}};
    const CoopCinematicKey emilyScene[]={corvoScene[0],{corvoScene[0].ownerId,582079941}};
    pose.cinematicSignature=coopCinematicSignature(corvoScene,1);peer.cinematicSignature=coopCinematicSignature(emilyScene,2);peer.cinematicNodeCount=2;
    if(!coopSharedSkipReady(pose,peer))return 52;
    pose.mapId=peer.mapId=coopMapId("campaign/another_map");if(coopSharedSkipReady(pose,peer))return 53;
    pose.mapId=peer.mapId=coopMapId("campaign/dunwall/escape/tower/dunwall_escape_tower_p");
    peer.cinematicNodeCount=3;if(coopSharedSkipReady(pose,peer))return 54;peer.cinematicNodeCount=2;
    peer.cinematicSignature++;if(coopSharedSkipReady(pose,peer))return 55;
    peer.cinematicSignature--;
    peer.sequenceBoundaryId++;if(coopSharedSkipReady(pose,peer))return 56;peer.sequenceBoundaryId--;
    peer.sequenceMetadataValid=0;if(coopSharedSkipReady(pose,peer))return 57;peer.sequenceMetadataValid=1;
    auto sequencePose=valid;
    sequencePose.version=5;if(validCoopPose(sequencePose,0))return 58;sequencePose=valid;
    sequencePose.sequenceSignature=1;if(validCoopPose(sequencePose,0))return 59;sequencePose=valid;
    sequencePose.sequenceMetadataValid=1;sequencePose.sequenceCount=1;sequencePose.sequenceSignature=1;
    if(validCoopPose(sequencePose,0))return 60;
    sequencePose.sequenceBoundaryId=1;if(!validCoopPose(sequencePose,0))return 61;
    sequencePose.sequenceCount=3;if(validCoopPose(sequencePose,0))return 62;
    CoopSequenceKey corvoSequence{coopMapId("midnight/campaign/dunwall/escape/tower/coup_main_post_fight.midnightscene"),coopMapId("player_is_corvo"),28000,3};
    auto emilySequence=corvoSequence;emilySequence.subsceneId=coopMapId("player_is_emily");
    const auto openingMap=coopMapId("campaign/dunwall/escape/tower/dunwall_escape_tower_p");
    const auto corvoBoundary=coopSequenceSignature(&corvoSequence,1,openingMap,true);
    if(!corvoBoundary||corvoBoundary!=coopSequenceSignature(&emilySequence,1,openingMap,true)||
       coopSequenceSignature(&corvoSequence,1)==coopSequenceSignature(&emilySequence,1))return 63;
    if(corvoBoundary==coopSequenceSignature(&emilySequence,1,coopMapId("campaign/other"),true))return 64;
    emilySequence.durationMs++;if(corvoBoundary==coopSequenceSignature(&emilySequence,1,openingMap,true))return 65;emilySequence.durationMs--;
    emilySequence.assetId++;if(corvoBoundary==coopSequenceSignature(&emilySequence,1,openingMap,true))return 66;emilySequence.assetId--;
    emilySequence.channels=1;if(corvoBoundary==coopSequenceSignature(&emilySequence,1,openingMap,true))return 67;
    const CoopSequenceKey sequenceSetA[]={corvoSequence,emilySequence},sequenceSetB[]={emilySequence,corvoSequence};
    if(coopSequenceSignature(sequenceSetA,2)!=coopSequenceSignature(sequenceSetB,2)||
       coopSequenceSignature(sequenceSetA,0)||coopSequenceSignature(sequenceSetA,3))return 68;
    sequencePose=valid;sequencePose.version=6;if(validCoopPose(sequencePose,0))return 71;
    sequencePose=valid;sequencePose.barrierHeld=1;if(validCoopPose(sequencePose,0))return 72;
    sequencePose=valid;sequencePose.sequenceTimeMs=1;if(validCoopPose(sequencePose,0))return 73;
    pose.sequenceBoundaryId=peer.sequenceBoundaryId=corvoBoundary;
    pose.sequencePaused=peer.sequencePaused=pose.barrierEnabled=peer.barrierEnabled=pose.barrierHeld=peer.barrierHeld=1;
    pose.cinematicCanSkip=peer.cinematicCanSkip=0;
    if(!validCoopPose(pose,0)||!validCoopPose(peer,1)||!coopBarrierReady(pose,peer))return 74;
    CoopBarrierPacket release{};release.role=0;release.sequence=1;release.sourceSession=peer.session;release.targetSession=pose.session;
    release.sourceWorldEpoch=peer.worldEpoch;release.targetWorldEpoch=pose.worldEpoch;
    release.sourceCinematicEpoch=peer.cinematicEpoch;release.targetCinematicEpoch=pose.cinematicEpoch;
    release.mapId=pose.mapId;release.cinematicSceneId=coopSkipSceneId(pose);release.sequenceBoundaryId=corvoBoundary;
    if(!validCoopBarrier(release,pose,peer))return 75;
    const auto validRelease=release;
    release.role=1;if(validCoopBarrier(release,pose,peer))return 76;release=validRelease;
    release.targetSession++;if(validCoopBarrier(release,pose,peer))return 77;release=validRelease;
    release.targetCinematicEpoch++;if(validCoopBarrier(release,pose,peer))return 78;release=validRelease;
    release.targetWorldEpoch++;if(validCoopBarrier(release,pose,peer))return 79;release=validRelease;
    release.sequenceBoundaryId++;if(validCoopBarrier(release,pose,peer))return 80;release=validRelease;
    release.magic=0x53433244;if(validCoopBarrier(release,pose,peer))return 81;release=validRelease;
    pose.barrierHeld=0;if(validCoopBarrier(release,pose,peer))return 82;pose.barrierHeld=1;
    pose.barrierEnabled=0;if(validCoopBarrier(release,pose,peer))return 83;pose.barrierEnabled=1;
    peer.barrierHeld=peer.sequencePaused=0;if(!validCoopBarrier(release,pose,peer))return 84;
    peer.sequenceBoundaryId++;if(validCoopBarrier(release,pose,peer))return 85;
    pose=valid;
    pose.flags=0;pose.worldEpoch=0;for(float& f:pose.orientation)f=0;
    if(!validCoopPose(pose,0))return 19;
    // The proxy remains loaded because its import hook must remain valid until exit.
    std::cout << "PASS: game mutex scoped, unrelated mutex unchanged, system DirectInput forwarded\n";
    std::cout << "PASS: simulated input preserves controls; expired, malformed, and excessive leases release input\n";
    std::cout << "PASS: pose protocol rejects incompatible builds, own role, NaNs, wrong size, invalid flags and malformed rotation bases\n";
    std::cout << "PASS: native cinematic, pause, and pending state transitions block movement follow\n";
    std::cout << "PASS: cinematic identities ignore observer order, distinguish native nodes, and reject inconsistent metadata\n";
    std::cout << "PASS: skip relay rejects mismatched scenes, sessions, world epochs, cinematic epochs, and disabled peers\n";
    std::cout << "PASS: active sequence boundaries distinguish subscenes and channels; branch equivalence is limited to the observed opening pair\n";
    std::cout << "PASS: opening rendezvous requires both native holds and rejects releases for another role, session, epoch, or segment\n";
    return 0;
}
