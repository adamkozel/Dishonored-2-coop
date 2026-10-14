// Included after the runtime probe's read-only memory helpers.
// Reconstructs the position path in native 0x76a180 / 0x53e020 / 0x53d6a0.
int inspectBones(HANDLE process,std::uintptr_t base,std::uint32_t handle) {
    std::uintptr_t game=0,actor=0,components=0,physics=0,anim=0,owner=0;
    std::uint32_t actual=0,flags=0;const auto slot=static_cast<std::int32_t>(handle)>>16;
    if(slot<0||slot>=16384||!read(process,base+0x39a5290,&game,8)||!game||
       !read(process,game+0x3130b0+slot*8,&actor,8)||!actor||!read(process,actor+0xa8,&actual,4)||actual!=handle||
       !read(process,actor+0x268,&components,8)||!read(process,actor+0x270,&flags,4)||(flags&0xffffff)>256)return 30;
    for(unsigned i=0;i<(flags&0xffffff);++i) {
        std::uintptr_t component=0,vt=0;
        if(read(process,components+i*8,&component,8)&&component&&read(process,component,&vt,8)&&vt==base+0x1c54008)physics=component;
    }
    std::uintptr_t binding=0,offsetsHeader=0,boneIds=0,offsets=0,instance=0,world=0,bodyIds=0,bodies=0,cache=0,remapHeaders=0,remap=0,ragdoll=0;
    std::uint32_t count=0,boneCount=0,offsetCount=0,cacheCount=0,remapCount=0,remapHeaderCount=0;unsigned char phase=0;
    if(!physics||!read(process,physics+0x10,&owner,8)||owner!=actor||
       !read(process,physics+0xf8,&anim,8)||objectType(process,base,anim)!=".?AVarkCpntAnimEvolved@@"||
       !read(process,anim+0x10,&owner,8)||owner!=actor||!read(process,physics+0x1d0,&ragdoll,8)||ragdoll||
       !read(process,physics+0x138,&count,4)||!count||count>64||
       !read(process,physics+0x148,&binding,8)||!binding||!read(process,binding,&boneIds,8)||!boneIds||
       !read(process,binding+8,&boneCount,4)||boneCount!=count||
       !read(process,physics+0x150,&offsetsHeader,8)||!offsetsHeader||!read(process,offsetsHeader,&offsets,8)||!offsets||
       !read(process,offsetsHeader+8,&offsetCount,4)||offsetCount!=count||
       !read(process,physics+0x100,&instance,8)||!instance||!read(process,instance+0x20,&world,8)||!world||
       !read(process,instance+0x28,&bodyIds,8)||!bodyIds||!read(process,world+0x28,&bodies,8)||!bodies||
       !read(process,physics+0x1e4,&phase,1)||!read(process,anim+0x2d8,&cache,8)||!cache||
       !read(process,anim+0x2e0,&cacheCount,4)||!(cacheCount&0xffffff)||(cacheCount&0xffffff)>4096||
       !read(process,anim+0x2e8,&remapHeaders,8)||!remapHeaders||!read(process,anim+0x2f0,&remapHeaderCount,4)||
       (remapHeaderCount&0xffffff)>16||phase>=(remapHeaderCount&0xffffff)||
       !read(process,remapHeaders+phase*16,&remap,8)||!remap||!read(process,remapHeaders+phase*16+8,&remapCount,4)||
       !(remapCount&0xffffff)||(remapCount&0xffffff)>4096)return 31;
    cacheCount&=0xffffff;remapCount&=0xffffff;
    std::vector<float> matrices(cacheCount*16),localOffsets(count*16);
    std::vector<std::int16_t> mapping(remapCount);std::vector<std::uint16_t> joints(count);std::vector<std::uint32_t> ids(count);
    float root[12]{},animOffset[3]{},overridePosition[3]{},sentinel[3]{};
    if(!read(process,actor+0x300,root,sizeof(root))||!read(process,anim+0x3b0,animOffset,sizeof(animOffset))||
       !read(process,anim+0x3bc,overridePosition,sizeof(overridePosition))||!read(process,base+0x31af620,sentinel,sizeof(sentinel))||
       !read(process,cache,matrices.data(),matrices.size()*4)||!read(process,offsets,localOffsets.data(),localOffsets.size()*4)||
       !read(process,remap,mapping.data(),mapping.size()*2)||!read(process,boneIds,joints.data(),joints.size()*2)||
       !read(process,bodyIds,ids.data(),ids.size()*4))return 32;
    for(float f:root)if(!std::isfinite(f))return 33;
    for(float f:matrices)if(!std::isfinite(f))return 33;
    for(float f:localOffsets)if(!std::isfinite(f))return 33;
    for(float f:animOffset)if(!std::isfinite(f))return 33;
    for(float f:overridePosition)if(!std::isfinite(f))return 33;
    const bool overrideActive=std::memcmp(overridePosition,sentinel,sizeof(sentinel))!=0;
    struct Body {float current[3],expected[3],local[3];};std::vector<Body> result(count);
    for(unsigned i=0;i<count;++i) {
        if(joints[i]>=remapCount||mapping[joints[i]]<0||static_cast<unsigned>(mapping[joints[i]])>=cacheCount||
           (ids[i]&0xffffff)==0xffffff||!read(process,bodies+(ids[i]&0xffffff)*0xb0+0x30,result[i].current,12))return 34;
        const auto* m=matrices.data()+mapping[joints[i]]*16;const auto* offset=localOffsets.data()+i*16;
        for(unsigned j=0;j<3;++j)result[i].local[j]=m[12+j]+m[j]*offset[12]+m[4+j]*offset[13]+m[8+j]*offset[14];
        for(unsigned j=0;j<3;++j)result[i].expected[j]=(overrideActive?overridePosition[j]:root[j]+animOffset[j])+
            root[3+j]*result[i].local[0]+root[6+j]*result[i].local[1]+root[9+j]*result[i].local[2];
    }
    for(const auto& body:result)for(unsigned j=0;j<3;++j)
        if(!std::isfinite(body.current[j])||!std::isfinite(body.expected[j]))return 33;
    // Report races instead of quietly treating a mixed animation snapshot as truth.
    std::vector<float> finalMatrices(matrices.size());float finalRoot[12]{};
    std::vector<std::int16_t> finalMapping(mapping.size());std::uintptr_t finalCache=0,finalRemap=0;std::uint32_t finalCacheCount=0,finalRemapCount=0;
    if(!read(process,cache,finalMatrices.data(),finalMatrices.size()*4)||!read(process,actor+0x300,finalRoot,sizeof(finalRoot))||
       !read(process,remap,finalMapping.data(),finalMapping.size()*2)||!read(process,anim+0x2d8,&finalCache,8)||
       !read(process,anim+0x2e0,&finalCacheCount,4)||!read(process,remapHeaders+phase*16,&finalRemap,8)||
       !read(process,remapHeaders+phase*16+8,&finalRemapCount,4))return 35;
    // The matrices can remain unchanged while physics publishes its new
    // body positions. Require both halves, offsets and phase to remain stable
    // across this external snapshot; a stable root alone is insufficient.
    bool bodiesStable=true;float finalAnimOffset[3]{},finalOverride[3]{};unsigned char finalPhase=0;
    for(unsigned i=0;i<count;++i) {
        float current[3]{};unsigned id=0;
        if(!read(process,bodyIds+i*4,&id,4)||id!=ids[i]||
           !read(process,bodies+(id&0xffffff)*0xb0+0x30,current,12))return 35;
        if(std::memcmp(current,result[i].current,12))bodiesStable=false;
    }
    if(!read(process,anim+0x3b0,finalAnimOffset,12)||!read(process,anim+0x3bc,finalOverride,12)||
       !read(process,physics+0x1e4,&finalPhase,1))return 35;
    const bool stable=bodiesStable&&finalPhase==phase&&std::memcmp(animOffset,finalAnimOffset,12)==0&&
        std::memcmp(overridePosition,finalOverride,12)==0&&finalCache==cache&&finalRemap==remap&&(finalCacheCount&0xffffff)==cacheCount&&(finalRemapCount&0xffffff)==remapCount&&
        mapping==finalMapping&&std::memcmp(matrices.data(),finalMatrices.data(),matrices.size()*4)==0&&std::memcmp(root,finalRoot,sizeof(root))==0;
    auto vector=[](const float* p,unsigned n){std::cout<<'[';for(unsigned j=0;j<n;++j)std::cout<<(j?",":"")<<p[j];std::cout<<']';};
    std::cout.precision(9);
    std::cout<<"{\"handle\":"<<handle<<",\"stable\":"<<(stable?"true":"false")<<",\"phase\":"<<static_cast<unsigned>(phase)
        <<",\"cacheCount\":"<<cacheCount<<",\"overridePosition\":"<<(overrideActive?"true":"false")<<",\"root\":";vector(root,12);
    std::cout<<",\"bodies\":[";
    for(unsigned i=0;i<count;++i){if(i)std::cout<<',';std::cout<<"{\"id\":"<<ids[i]<<",\"joint\":"<<joints[i]<<",\"cacheIndex\":"<<mapping[joints[i]]<<",\"position\":";vector(result[i].current,3);
        std::cout<<",\"expectedPosition\":";vector(result[i].expected,3);std::cout<<",\"localPosition\":";vector(result[i].local,3);std::cout<<'}';}
    std::cout<<"]}\n";return 0;
}
