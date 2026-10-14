#pragma once
#include "body_retarget.h"
constexpr unsigned kDoppelgangerJoints=219;
constexpr std::uint64_t kDoppelgangerRig=0x5921d3450a60d7e2ull;
constexpr const char* kDoppelgangerModel="models/characters/female/emily/head01/emily_head01.md6";
constexpr const char* kDoppelgangerSkeleton="models/characters/female/emily/head01/emily_head01.md6skl";
inline BodyMatrix doppelgangerLocalMatrix(const std::array<float,12>& source){
    BodyMatrix m{};for(unsigned c=0;c<4;++c)for(unsigned r=0;r<3;++r)m.v[c*4+r]=source[c*3+r];return m;
}
inline bool doppelgangerJointValid(unsigned joint,const BodyMatrix& local){
    // Only for kDoppelgangerRig. Native480 captured rigid leaf-marker offsets
    // up to4.18m: anchor_jnt(1), left/right_hand_attachment_jnt(10/17).
    // They do not parent skin/corrective bones. Keep all other bones at3m;
    // allow these markers10m while retaining finite, orthonormal and det checks.
    return joint<kDoppelgangerJoints&&bodyRigid(local,(joint==1||joint==10||joint==17)?10.f:3.f);
}
inline std::uint64_t doppelgangerRig(const NativeSkeleton& s,bool canonicalZero=true){
    if(std::strcmp(s.modelName,kDoppelgangerModel)||std::strcmp(s.skeletonName,kDoppelgangerSkeleton)||
       s.parents.size()!=kDoppelgangerJoints||s.bindLocal.size()!=kDoppelgangerJoints)return 0;
    auto hash=coopMapId(s.skeletonName);const auto byte=[&](unsigned char b){hash^=b;hash*=1099511628211ull;};
    for(unsigned j=0;j<kDoppelgangerJoints;++j){const auto parent=s.parents[j];if(parent<-1||parent>=static_cast<int>(j))return 0;
        byte(static_cast<unsigned char>(parent));byte(static_cast<unsigned char>(parent>>8));BodyMatrix bind{};if(!bodyFromQts(s.bindLocal[j],bind))return 0;
        // JSON captures and native SIMD data can disagree only on the sign of
        // zero. It has no effect on the bind transform; canonicalize just zero,
        // retaining exact bits for every nonzero value and all hierarchy data.
        for(unsigned i=0;i<11;++i)if(i!=7){std::uint32_t bits=0;std::memcpy(&bits,&s.bindLocal[j][i],4);if(canonicalZero&&s.bindLocal[j][i]==0.f)bits=0;for(unsigned n=0;n<4;++n)byte(static_cast<unsigned char>(bits>>(n*8)));}
    }return hash;
}
