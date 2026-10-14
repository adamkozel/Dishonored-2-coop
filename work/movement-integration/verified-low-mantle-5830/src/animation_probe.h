// Read-only view of the build-verified native animation tree bindings.
int inspectAnimation(HANDLE process,std::uintptr_t base,std::uint32_t handle) {
    std::uintptr_t game=0,actor=0,components=0,anim=0,owner=0,tree=0,data=0,wrapper=0;
    std::uint32_t actual=0,flags=0;const auto slot=static_cast<std::int32_t>(handle)>>16;
    if(slot<0||slot>=16384||!read(process,base+0x39a5290,&game,8)||!game||
       !read(process,game+0x3130b0+slot*8,&actor,8)||!actor||!read(process,actor+0xa8,&actual,4)||actual!=handle||
       !read(process,actor+0x268,&components,8)||!read(process,actor+0x270,&flags,4)||(flags&0xffffff)>256)return 40;
    for(unsigned i=0;i<(flags&0xffffff);++i) {
        std::uintptr_t component=0,vt=0;
        if(read(process,components+i*8,&component,8)&&component&&read(process,component,&vt,8)&&vt==base+0x1b80408)anim=component;
    }
    if(!anim||!read(process,anim+0x10,&owner,8)||owner!=actor||
       !read(process,anim+0x2728+0x338,&wrapper,8)||wrapper!=anim+0x338||
       !read(process,wrapper,&tree,8)||!tree||!read(process,tree,&data,8)||!data)return 41;
    struct Binding {unsigned field;const char* name;unsigned floats;};
    const Binding bindings[]={{0x220,"forwardOrientation",3},{0x228,"targLinearVelocity",3},{0x2a0,"desiredOrientation",3},
        {0x268,"targSpeed",1},{0x270,"targAngle",1},{0x2a8,"requestedVelocity",1},{0x2b0,"locoPlayRateMultiplier",1},
        {0x2b8,"linearVelocity",3},{0x2d8,"speed",1},{0x4c8,"midnightWeight",1},{0x4c0,"locomotionInputData",17}};
    std::cout.precision(9);
    std::cout<<"{\"handle\":"<<handle<<",\"animation\":\"0x"<<std::hex<<anim<<"\",\"data\":\"0x"<<data<<std::dec<<"\",\"bindings\":[";
    bool first=true;
    for(const auto& b:bindings) {
        std::uintptr_t descriptor=0,name=0;std::int32_t offset=-1;std::uint32_t type=0;float values[17]{};
        if(!read(process,anim+0x2728+b.field,&descriptor,8)||!descriptor||!read(process,descriptor-8,&name,8)||
           readString(process,name)!=b.name||!read(process,descriptor,&type,4)||!read(process,descriptor+4,&offset,4)||
           offset<0||offset>0x100000||!read(process,data+offset,values,b.floats*4))return 42;
        for(unsigned i=0;i<b.floats;++i)if(!std::isfinite(values[i]))return 43;
        if(!first)std::cout<<',';first=false;
        std::cout<<"{\"name\":"<<jsonString(b.name)<<",\"type\":"<<type<<",\"offset\":"<<offset<<",\"values\":[";
        for(unsigned i=0;i<b.floats;++i)std::cout<<(i?",":"")<<values[i];
        std::cout<<"]}";
    }
    std::cout<<"],\"declaredInputs\":[";
    // Walk the bounded declaration hash table rather than guessing offsets for
    // unbound graph parameters (native lookup 0x6e0ce0).
    std::uintptr_t declaration=0,buckets=0;std::uint32_t mask=0;
    if(!read(process,wrapper+8,&declaration,8)||!declaration||!read(process,declaration+0x70,&buckets,8)||
       !read(process,declaration+0x80,&mask,4)||mask>4095||!buckets)return 44;
    first=true;std::unordered_set<std::uintptr_t> seen;
    for(unsigned bucket=0;bucket<=mask;++bucket) {
        std::uintptr_t entry=0;if(!read(process,buckets+bucket*8,&entry,8))return 44;
        for(unsigned depth=0;entry&&depth<128;++depth) {
            if(!seen.insert(entry).second)break;
            std::uintptr_t name=0,next=0;std::int32_t offset=-1;std::uint32_t type=0;
            if(!read(process,entry,&name,8)||!read(process,entry+8,&type,4)||!read(process,entry+12,&offset,4)||
               !read(process,entry+0x970,&next,8)||offset<0||offset>0x100000)return 44;
            if(!first)std::cout<<',';first=false;
            std::cout<<"{\"name\":"<<jsonString(readString(process,name))<<",\"type\":"<<type<<",\"offset\":"<<offset;
            if(type==1) {float value=0;if(read(process,data+offset,&value,4)&&std::isfinite(value))std::cout<<",\"value\":"<<value;}
            if(type==12){unsigned words[6]{};if(read(process,data+offset,words,sizeof(words))){std::cout<<",\"rawWords\":[";for(unsigned j=0;j<6;++j)std::cout<<(j?",":"")<<words[j];std::cout<<']';}}
            std::cout<<'}';entry=next;
        }
    }
    std::uintptr_t descriptor=0;std::int32_t outputOffset=-1;std::uint32_t words[0x130/4]{};
    std::cout<<"],\"modernOutputWords\":[";
    if(read(process,anim+0x2728+0x508,&descriptor,8)&&descriptor&&read(process,descriptor+4,&outputOffset,4)&&
       outputOffset>=0&&outputOffset<=0x100000&&read(process,data+outputOffset,words,sizeof(words))) {
        for(unsigned i=0;i<sizeof(words)/4;++i)std::cout<<(i?",":"")<<words[i];
    }
    std::cout<<"],\"locomotionNodes\":[";
    // Native 0x6eb070 copies each node's outputs to its declared output slot.
    // The modern node stores its output at +0xca8 (0x46a9e8).
    std::uintptr_t copies=0;std::int32_t copyCount=0;first=true;
    if(read(process,tree+8,&copyCount,4)&&copyCount>=0&&copyCount<=1024&&read(process,tree+0x10,&copies,8)) {
        for(int i=0;i<copyCount;++i) {
            std::uintptr_t src=0,dst=0;std::uint32_t size=0;
            if(!read(process,copies+i*0x20,&src,8)||!read(process,copies+i*0x20+8,&dst,8)||!read(process,copies+i*0x20+16,&size,4))continue;
            if(dst!=data+outputOffset||size!=0x130||src<0xca8)continue;
            const auto node=src-0xca8;std::uintptr_t input=0,controller=0,model=0,modelName=0;
            float requested[6]{},actualVelocity[3]{};std::uint32_t mode=0,stance=0,equipment=0;unsigned char requestFlags=0;
            if(!read(process,node+0xca0,&input,8)||!read(process,node+0xdd8,&controller,8)||
               !read(process,node+0xbd0,requested,sizeof(requested))||!read(process,node+0xae8,actualVelocity,12)||
               !read(process,node+0xbf0,&mode,4)||!read(process,node+0xc74,&stance,4)||!read(process,node+0xc78,&equipment,4)||
               !read(process,node+0xc98,&requestFlags,1))continue;
            read(process,node+8,&model,8);read(process,model+8,&modelName,8);
            if(!first)std::cout<<',';first=false;
            std::cout<<"{\"instance\":\"0x"<<std::hex<<node<<"\",\"input\":\"0x"<<input<<"\",\"controller\":\"0x"<<controller<<std::dec
                <<"\",\"model\":"<<jsonString(readString(process,modelName))<<",\"requestMode\":"<<mode<<",\"stance\":"<<stance
                <<",\"equipment\":"<<equipment<<",\"requestFlags\":"<<static_cast<unsigned>(requestFlags)<<",\"requested\":[";
            for(unsigned j=0;j<6;++j)std::cout<<(j?",":"")<<(std::isfinite(requested[j])?requested[j]:0);
            std::cout<<"],\"actualVelocity\":[";
            for(unsigned j=0;j<3;++j)std::cout<<(j?",":"")<<(std::isfinite(actualVelocity[j])?actualVelocity[j]:0);
            std::cout<<"],\"graphs\":[";
            std::uintptr_t graphList=0;unsigned graphFlags=0;
            if(controller&&read(process,controller+0x280,&graphList,8)&&graphList&&
               read(process,controller+0x288,&graphFlags,4)&&(graphFlags&0xffffff)<=2){
                for(unsigned gi=0;gi<(graphFlags&0xffffff);++gi){
                    std::uintptr_t graph=0,active=0;unsigned stance=0,equipment=0;
                    read(process,graphList+gi*8,&graph,8);read(process,graph+0x2e8,&active,8);
                    read(process,graph+0xd80,&stance,4);read(process,graph+0xd84,&equipment,4);
                    if(gi)std::cout<<',';
                    std::cout<<"{\"address\":\"0x"<<std::hex<<graph<<std::dec<<"\",\"stance\":"<<stance<<",\"equipment\":"<<equipment
                        <<",\"activeType\":"<<jsonString(objectType(process,base,active))<<",\"candidates\":[";
                    for(unsigned ci=0;ci<4;++ci){std::uintptr_t mode=0,vt=0,score=0;
                        read(process,graph+ci*8,&mode,8);read(process,mode,&vt,8);read(process,vt+0x20,&score,8);
                        std::cout<<(ci?",":"")<<"{\"type\":"<<jsonString(objectType(process,base,mode))<<",\"scoreRva\":\"0x"<<std::hex
                            <<(score>=base&&score<base+0x3000000?score-base:0)<<std::dec<<"\"}";
                    }std::cout<<"]}";
                }
            }
            std::cout<<"]}";
        }
    }
    std::cout<<"],\"locomotionStances\":[";
    // Modern node initialization 0x46a0d0 takes model+0x140 (the older
    // declaration at +0x138 is null for these actors). Modern stance names
    // are +0x78/+0x80; +0x88/+0x90 names equipment, not physical crouch.
    std::uintptr_t render=0,modelData=0,loco=0,names=0;std::uint32_t stanceCount=0;
    if(read(process,anim+0x38,&render,8)&&objectType(process,base,render)==".?AVarkSkinnedRenderModel@@"&&
       read(process,render+0x1fe8,&modelData,8)&&objectType(process,base,modelData)==".?AVidDeclMD6@@"&&
       read(process,modelData+0x140,&loco,8)&&objectType(process,base,loco)==".?AVarkDeclLocomotion@@"&&
       read(process,loco+0x78,&names,8)&&names&&read(process,loco+0x80,&stanceCount,4)&&(stanceCount&0xffffff)<=127) {
        for(unsigned i=0;i<(stanceCount&0xffffff);++i) {
            std::uintptr_t name=0;if(!read(process,names+i*8,&name,8))return 51;
            std::cout<<(i?",":"")<<"{\"index\":"<<i<<",\"name\":"<<jsonString(readString(process,name))<<'}';
        }
    }
    std::cout<<"],\"modelConfig\":{";bool configFirst=true;
    if(modelData){
        for(unsigned field: {0x138u,0x140u,0x148u}){
            std::uintptr_t value=0,name=0;read(process,modelData+field,&value,8);read(process,value+8,&name,8);
            if(!configFirst)std::cout<<',';configFirst=false;
            std::cout<<'"'<<std::hex<<field<<"\":{\"address\":\"0x"<<value<<std::dec<<"\",\"type\":"<<jsonString(objectType(process,base,value))<<",\"name\":"<<jsonString(readString(process,name));
            if(field==0x148&&value){std::uintptr_t entries=0;unsigned count=0;
                if(read(process,value+0x58,&entries,8)&&read(process,value+0x60,&count,4)&&entries&&(count&0xffffff)<128){
                    std::cout<<",\"entries\":[";
                    for(unsigned j=0;j<(count&0xffffff);++j){std::uintptr_t n=0;read(process,entries+j*16,&n,8);std::cout<<(j?",":"")<<jsonString(readString(process,n));}std::cout<<']';
                }
            }std::cout<<'}';
        }
    }std::cout<<"}}\n";return 0;
}
