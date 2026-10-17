// #30 (stage 7 release): d2coop_setup, the installer's generator. It builds the game-derived payload from
// the player's own install, so the release ships none of it. The installer checks every output against the
// SHA-256 the release builder recorded from the lab build.
//   d2coop_setup packages <game base dir> <decl recipes> <output dir>
//       base/master.index, the four gameNvoidmod.index and gameNvoidmod.resources files
//   d2coop_setup banks <game dir> <bank patch> <output dir>
//       <role>-choke-bank.bin, rebuilt from the player's animations (bank_rebuild.h)
//   d2coop_setup tables <game dir> <table recipe> <output dir>
//       native-tables.bin: prop mechanism clips and ranged hand tracks (table_rebuild.h)
//   d2coop_setup bank-encode <game dir> <bank parameters> <lab bank> <output patch>   (release build, lab only)
//   d2coop_setup tables-encode <game dir> <table parameters> <reference tables> <output recipe>   (release build, lab only)
//   d2coop_setup master <original master.index> <output>
//   d2coop_setup index-check <index>                     (reads and writes back; must match byte for byte)
#include "bank_rebuild.h"
#include "table_rebuild.h"
#include "decl_recipe.h"
#include "inflate.h"
#include "resource_container.h"
#define NOMINMAX
#include <windows.h>
#include <bcrypt.h>
#include <cmath>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#pragma comment(lib,"bcrypt.lib")

namespace {
namespace fs=std::filesystem;
using d2res::Bytes;

Bytes load(const fs::path& p){
    std::ifstream f(p,std::ios::binary|std::ios::ate);if(!f)throw std::runtime_error("cannot open "+p.string());
    const auto n=f.tellg();if(n<0||n>(std::streamoff(1)<<32))throw std::runtime_error("bad size "+p.string());
    Bytes b(static_cast<std::size_t>(n));f.seekg(0);f.read(reinterpret_cast<char*>(b.data()),std::streamsize(b.size()));
    if(!f)throw std::runtime_error("cannot read "+p.string());return b;
}
Bytes loadRange(const fs::path& p,std::uint64_t offset,std::uint32_t size){
    std::ifstream f(p,std::ios::binary);if(!f)throw std::runtime_error("cannot open "+p.string());
    f.seekg(std::streamoff(offset));Bytes b(size);f.read(reinterpret_cast<char*>(b.data()),std::streamsize(size));
    if(!f)throw std::runtime_error("cannot read entry data in "+p.string());return b;
}
void save(const fs::path& p,const Bytes& b){
    if(fs::exists(p))throw std::runtime_error("output exists: "+p.string());
    std::ofstream f(p,std::ios::binary);f.write(reinterpret_cast<const char*>(b.data()),std::streamsize(b.size()));
    if(!f)throw std::runtime_error("cannot write "+p.string());
}
d2recipe::Hash sha256(const Bytes& b){
    BCRYPT_ALG_HANDLE alg=nullptr;BCRYPT_HASH_HANDLE h=nullptr;d2recipe::Hash out{};
    if(BCryptOpenAlgorithmProvider(&alg,BCRYPT_SHA256_ALGORITHM,nullptr,0))throw std::runtime_error("SHA-256 provider");
    const bool ok=!BCryptCreateHash(alg,&h,nullptr,0,nullptr,0,0)&&!BCryptHashData(h,const_cast<PUCHAR>(b.data()),ULONG(b.size()),0)&&
        !BCryptFinishHash(h,out.data(),ULONG(out.size()),0);
    if(h)BCryptDestroyHash(h);BCryptCloseAlgorithmProvider(alg,0);
    if(!ok)throw std::runtime_error("SHA-256 failed");return out;
}
std::string hex(const d2recipe::Hash& h){static const char* x="0123456789abcdef";std::string s;for(auto c:h){s+=x[c>>4];s+=x[c&15];}return s;}

// The player's resources: each (type, name) in the newest index of games 1-3, unpacked on demand.
struct Decls {
    fs::path base;std::vector<d2res::Entry> entries;std::multimap<std::pair<std::string,std::string>,std::size_t> byName;
    explicit Decls(const fs::path& b):base(b){
        for(int g=1;g<=3;++g)for(auto& e:d2res::readIndex(load(base/("game"+std::to_string(g)+"_patch.index")))){
            byName.emplace(std::make_pair(e.type,e.source),entries.size());entries.push_back(std::move(e));}
    }
    Bytes unpacked(const d2res::Entry& e) const {
        auto raw=loadRange(base/d2res::resourcesName(e.flags2),e.offset,e.packed);
        return e.packed==e.size?raw:d2inflate::zlib(raw.data(),raw.size(),e.size);
    }
    // The entry with this content (SHA-256), or with `any` the first of that type and name.
    Bytes find(const std::string& type,const std::string& name,const d2recipe::Hash* sha) const {
        auto range=byName.equal_range({type,name});
        for(auto it=range.first;it!=range.second;++it){auto data=unpacked(entries[it->second]);if(!sha||sha256(data)==*sha)return data;}
        throw std::runtime_error("not found in this install: "+type+" "+name);
    }
    Bytes find(const d2recipe::Source& s) const {return find(s.type,s.name,&s.sha);}
};

// ---- Native samples through the player's own Dishonored2.exe Edge evaluator (work/natural-moveset/sample_edge.cpp):
// the executable is mapped without running it (DONT_RESOLVE_DLL_REFERENCES: no entry point, imports or game state)
// and only its pure animation evaluator is called. It is the build the patch names (SHA-256), so the evaluator's
// address is known.
constexpr std::uint32_t evaluatorRva=0x12e2c30;
struct Aligned {
    unsigned char* data;std::size_t size;
    explicit Aligned(std::size_t n):size((n+31)&~std::size_t(15)){data=static_cast<unsigned char*>(_aligned_malloc(size,16));if(!data)throw std::bad_alloc();std::memset(data,0,size);}
    ~Aligned(){_aligned_free(data);}
    Aligned(const Aligned&)=delete;Aligned& operator=(const Aligned&)=delete;
};
using Evaluate=void(*)(const void*,const void*,void**,float);
bool safeEvaluate(Evaluate eval,const void* a,const void* s,void** out,float time){
    __try{eval(a,s,out,time);return true;}__except(EXCEPTION_EXECUTE_HANDLER){return false;}
}
struct Evaluator {
    HMODULE mapped=nullptr;Evaluate eval=nullptr;
    explicit Evaluator(const fs::path& exe){
        mapped=LoadLibraryExW(exe.c_str(),nullptr,DONT_RESOLVE_DLL_REFERENCES);
        if(!mapped)throw std::runtime_error("cannot map "+exe.string());
        eval=reinterpret_cast<Evaluate>(reinterpret_cast<unsigned char*>(mapped)+evaluatorRva);
    }
    ~Evaluator(){if(mapped)FreeLibrary(mapped);}
    Evaluator(const Evaluator&)=delete;Evaluator& operator=(const Evaluator&)=delete;
    d2bank::Sample sample(const Bytes& a,const Bytes& s) const {
        auto u16=[](const Bytes& b,std::size_t o){if(o+2>b.size())throw std::runtime_error("edge: truncated");return unsigned(b[o])|(unsigned(b[o+1])<<8);};
        auto u32=[](const Bytes& b,std::size_t o){if(o+4>b.size())throw std::runtime_error("edge: truncated");std::uint32_t v;std::memcpy(&v,b.data()+o,4);return v;};
        if(u32(a,0)!=0x45413038||u32(s,0)!=0x45533036)throw std::runtime_error("edge: wrong format");
        const unsigned joints=u16(s,0x10),curves=u16(s,0x12),frames=u16(a,0x10),animJoints=u16(a,0xe),chunks=u16(a,0x12);
        float fps;std::memcpy(&fps,a.data()+8,4);
        if(!joints||joints>512||animJoints>joints||!frames||frames>18000||!chunks||chunks>frames||curves>4096||!std::isfinite(fps)||fps<1||fps>240)throw std::runtime_error("edge: dimensions");
        if(u32(s,0x18)+std::size_t(0x18)+std::size_t(joints)*48>s.size())throw std::runtime_error("edge: skeleton rest data");
        for(std::size_t o:{0x38,0x3c,0x40,0x44,0x48,0x4c,0x50}){const auto v=u32(a,o);if(v&&v+o>=a.size())throw std::runtime_error("edge: relative offset");}
        if(std::size_t(u32(a,0x38))+0x38+chunks*8>a.size()||std::size_t(u32(a,0x3c))+0x3c+chunks*4>a.size())throw std::runtime_error("edge: chunk tables");
        // Aligned copies keep embedded relative offsets and read-ahead padding valid.
        Aligned ab(a.size()+256),sb(s.size()+256),pose((joints+8)*48),curve((curves+8)*4),masks(32768),flags(16);
        std::memcpy(ab.data,a.data(),a.size());std::memcpy(sb.data,s.data(),s.size());
        void* buffers[6]={pose.data,masks.data,curve.data,masks.data+8192,flags.data,masks.data+16384};
        d2bank::Sample out;out.joints=joints;out.frames=frames;out.fps=fps;out.values.resize(std::size_t(frames)*joints*12);
        for(unsigned f=0;f<frames;++f){
            if(!safeEvaluate(eval,ab.data,sb.data,buffers,float(f)/fps))throw std::runtime_error("edge: evaluator exception");
            const auto* v=reinterpret_cast<const float*>(pose.data);
            for(unsigned j=0;j<joints;++j){float n=0;for(unsigned k=0;k<12;++k){if(!std::isfinite(v[j*12+k])||std::fabs(v[j*12+k])>100000)throw std::runtime_error("edge: implausible pose");if(k<4)n+=v[j*12+k]*v[j*12+k];}
                if(std::fabs(n-1)>.03f)throw std::runtime_error("edge: invalid quaternion");}
            std::memcpy(out.values.data()+std::size_t(f)*joints*12,v,std::size_t(joints)*48);
        }
        return out;
    }
};

d2recipe::Hash fileSha(const fs::path& p){return sha256(load(p));}

// Encoding (lab): rebuild every clip's base from the original install and measure the residual against our bank.
// Decoding (installer): rebuild the bank from the player's install and the patch.
struct BankInputs {
    std::vector<std::vector<d2bank::Joint>> skeletons;std::vector<Bytes> skeletonBytes,animations;std::vector<d2recipe::Hash> skeletonSha,assetSha;
    BankInputs(const Decls& decls,const std::vector<std::string>& skeletonNames,const std::vector<std::string>& assetNames,
               const std::vector<d2recipe::Hash>* expectSkeletons,const std::vector<d2recipe::Hash>* expectAssets){
        for(std::size_t k=0;k<skeletonNames.size();++k){
            auto b=decls.find("skeleton",skeletonNames[k],expectSkeletons?&(*expectSkeletons)[k]:nullptr);
            skeletonSha.push_back(sha256(b));skeletons.push_back(d2bank::readSkeleton(b));skeletonBytes.push_back(std::move(b));}
        for(std::size_t k=0;k<assetNames.size();++k){
            auto b=decls.find("anim",assetNames[k],expectAssets?&(*expectAssets)[k]:nullptr);assetSha.push_back(sha256(b));animations.push_back(std::move(b));}
    }
};
std::vector<std::string> assetNames(const d2bank::Params& p){std::vector<std::string> out;for(const auto& a:p.assets)out.push_back(a.name);return out;}
std::vector<std::string> assetNames(const d2tables::Params& p){std::vector<std::string> out;for(const auto& a:p.assets)out.push_back(a.first);return out;}
Bytes bankBytes(const std::array<std::uint8_t,32>& header,const std::vector<std::array<std::uint8_t,24>>& clipHeaders,const std::vector<std::vector<float>>& clips){
    Bytes out(header.begin(),header.end());
    for(std::size_t c=0;c<clips.size();++c){out.insert(out.end(),clipHeaders[c].begin(),clipHeaders[c].end());
        const auto* p=reinterpret_cast<const std::uint8_t*>(clips[c].data());out.insert(out.end(),p,p+clips[c].size()*4);}
    return out;
}
std::array<double,2> sums(const std::vector<float>& v){std::array<double,2> s{};for(float x:v){s[0]+=x;s[1]+=double(x)*x;}return s;}

int bankEncode(const fs::path& game,const fs::path& paramsFile,const fs::path& labBank,const fs::path& out){
    const Bytes paramsBytes=load(paramsFile);d2bank::In in{paramsBytes};const auto params=d2bank::readParams(in);
    if(in.at!=paramsBytes.size())throw std::runtime_error("parameters: trailing bytes");
    const Bytes lab=load(labBank);const std::size_t J=params.joints,clipSize=24+std::size_t(d2bank::frames)*J*28;
    if(lab.size()!=32+params.clips.size()*clipSize)throw std::runtime_error("lab bank size does not match the parameters");
    const Decls decls(game/"base");const BankInputs inputs(decls,params.skeletons,assetNames(params),nullptr,nullptr);const Evaluator evaluator(game/"Dishonored2.exe");
    d2bank::Rebuilder rebuild(params,inputs.skeletons,[&](std::size_t a){return evaluator.sample(inputs.animations[a],inputs.skeletonBytes[params.assets[a].skeleton]);});
    d2bank::Patch patch;patch.exe=fileSha(game/"Dishonored2.exe");patch.skeletons=inputs.skeletonSha;patch.assets=inputs.assetSha;
    std::memcpy(patch.header.data(),lab.data(),32);patch.params=params;
    std::vector<std::vector<float>> clips;double worst=0;
    for(std::size_t c=0;c<params.clips.size();++c){
        const auto* rec=lab.data()+32+c*clipSize;std::array<std::uint8_t,24> h;std::memcpy(h.data(),rec,24);patch.clipHeaders.push_back(h);
        std::vector<float> labValues(std::size_t(d2bank::frames)*J*7);std::memcpy(labValues.data(),rec+24,labValues.size()*4);
        std::vector<std::uint8_t> flips;std::vector<std::int32_t> residual;
        auto v=rebuild.clip(c,labValues.data(),patch.step,flips,residual);
        for(std::size_t k=0;k<v.size();++k)worst=std::max(worst,double(std::fabs(v[k]-labValues[k])));
        patch.flips.push_back(std::move(flips));patch.residual.push_back(std::move(residual));patch.sums.push_back(sums(v));clips.push_back(std::move(v));
    }
    save(out,d2bank::writePatch(patch,paramsBytes));
    const Bytes bank=bankBytes(patch.header,patch.clipHeaders,clips);
    std::printf("{\"clips\":%zu,\"maximumError\":%.9g,\"rebuiltSha256\":\"%s\",\"patch\":\"%s\"}\n",clips.size(),worst,hex(sha256(bank)).c_str(),hex(fileSha(out)).c_str());
    return 0;
}

// Native tables (table_rebuild.h): the encoder measures hand-track residuals against our reference tables.
std::vector<d2tables::Table> readTables(const Bytes& d){
    d2bank::In r{d};r.need(8);if(std::memcmp(d.data(),"D2NTB001",8))throw std::runtime_error("tables: bad magic");r.at=8;std::vector<d2tables::Table> out;
    for(auto n=r.get<std::uint32_t>();n--;){d2tables::Table t;t.name=r.str();t.i16=r.get<std::uint8_t>()!=0;std::size_t count=1;
        for(auto k=r.get<std::uint32_t>();k--;){t.dims.push_back(r.get<std::uint32_t>());count*=t.dims.back();}
        if(count>(1u<<26))throw std::runtime_error("tables: size");
        if(t.i16){r.need(count*2);t.s.resize(count);std::memcpy(t.s.data(),d.data()+r.at,count*2);r.at+=count*2;}
        else{r.need(count*4);t.f.resize(count);std::memcpy(t.f.data(),d.data()+r.at,count*4);r.at+=count*4;}
        out.push_back(std::move(t));}
    if(r.at!=d.size())throw std::runtime_error("tables: trailing bytes");
    return out;
}
int tablesEncode(const fs::path& game,const fs::path& paramsFile,const fs::path& referenceFile,const fs::path& out){
    const Bytes paramsBytes=load(paramsFile);d2bank::In in{paramsBytes};const auto params=d2tables::readParams(in);
    if(in.at!=paramsBytes.size())throw std::runtime_error("table parameters: trailing bytes");
    const auto reference=readTables(load(referenceFile));
    const Decls decls(game/"base");const BankInputs inputs(decls,params.skeletons,assetNames(params),nullptr,nullptr);const Evaluator evaluator(game/"Dishonored2.exe");
    d2tables::Builder build(params,inputs.skeletons,[&](std::size_t a){return evaluator.sample(inputs.animations[a],inputs.skeletonBytes[params.assets[a].second]);});
    d2tables::Recipe recipe;recipe.exe=fileSha(game/"Dishonored2.exe");recipe.skeletons=inputs.skeletonSha;recipe.assets=inputs.assetSha;recipe.params=params;
    std::vector<d2tables::Table> tables;double worst=0;
    for(std::size_t k=0;k<params.entries.size();++k){
        const auto& name=params.entries[k].name;const d2tables::Table* ref=nullptr;for(const auto& t:reference)if(t.name==name)ref=&t;
        if(!ref)throw std::runtime_error("no reference table "+name);
        std::vector<std::uint8_t> flips;std::vector<std::int32_t> residual;auto t=build.build(k,ref,recipe.step,flips,residual);
        if(t.dims!=ref->dims||t.i16!=ref->i16)throw std::runtime_error("table layout differs from the reference: "+name);
        for(std::size_t i=0;i<t.f.size();++i)worst=std::max(worst,double(std::fabs(t.f[i]-ref->f[i])));
        if(t.s!=ref->s)throw std::runtime_error("table differs from the reference: "+name);
        recipe.flips.push_back(std::move(flips));recipe.residual.push_back(std::move(residual));recipe.sums.push_back(d2tables::sums(t));tables.push_back(std::move(t));
    }
    save(out,d2tables::writeRecipe(recipe,paramsBytes));
    std::printf("{\"tables\":%zu,\"maximumError\":%.9g,\"rebuiltSha256\":\"%s\",\"recipe\":\"%s\"}\n",tables.size(),worst,hex(sha256(d2tables::writeTables(tables))).c_str(),hex(fileSha(out)).c_str());
    return 0;
}
int tablesBuild(const fs::path& game,const fs::path& recipeFile,const fs::path& out){
    const auto recipe=d2tables::readRecipe(load(recipeFile),[](const std::uint8_t* d,std::size_t n,std::size_t e){return d2inflate::zlib(d,n,e);});
    const auto& params=recipe.params;
    if(fileSha(game/"Dishonored2.exe")!=recipe.exe)throw std::runtime_error("Dishonored2.exe is not the supported game build");
    const Decls decls(game/"base");const BankInputs inputs(decls,params.skeletons,assetNames(params),&recipe.skeletons,&recipe.assets);const Evaluator evaluator(game/"Dishonored2.exe");
    d2tables::Builder build(params,inputs.skeletons,[&](std::size_t a){return evaluator.sample(inputs.animations[a],inputs.skeletonBytes[params.assets[a].second]);});
    std::vector<d2tables::Table> tables;
    for(std::size_t k=0;k<params.entries.size();++k){
        auto flips=recipe.flips[k];auto residual=recipe.residual[k];auto t=build.build(k,nullptr,recipe.step,flips,residual);
        const auto s=d2tables::sums(t);const double count=double(t.f.size()+t.s.size());
        if(std::fabs(s[0]-recipe.sums[k][0])>1e-6*count||std::fabs(s[1]-recipe.sums[k][1])>1e-6*count)throw std::runtime_error("rebuilt table "+t.name+" differs from the release build");
        tables.push_back(std::move(t));
    }
    fs::create_directories(out);const Bytes file=d2tables::writeTables(tables);save(out/"native-tables.bin",file);
    std::printf("{\"tables\":%zu,\"sha256\":\"%s\"}\n",tables.size(),hex(sha256(file)).c_str());
    return 0;
}

int banks(const fs::path& game,const fs::path& patchFile,const fs::path& out){
    const auto patch=d2bank::readPatch(load(patchFile),[](const std::uint8_t* d,std::size_t n,std::size_t e){return d2inflate::zlib(d,n,e);});
    const auto& params=patch.params;
    if(fileSha(game/"Dishonored2.exe")!=patch.exe)throw std::runtime_error("Dishonored2.exe is not the supported game build");
    const Decls decls(game/"base");const BankInputs inputs(decls,params.skeletons,assetNames(params),&patch.skeletons,&patch.assets);const Evaluator evaluator(game/"Dishonored2.exe");
    d2bank::Rebuilder rebuild(params,inputs.skeletons,[&](std::size_t a){return evaluator.sample(inputs.animations[a],inputs.skeletonBytes[params.assets[a].skeleton]);});
    std::vector<std::vector<float>> clips;
    for(std::size_t c=0;c<params.clips.size();++c){
        auto flips=patch.flips[c];auto residual=patch.residual[c];
        auto v=rebuild.clip(c,nullptr,patch.step,flips,residual);
        // Native samples may differ in their last bits between CPUs (RCPPS/RSQRTPS): sums, not hashes.
        const auto s=sums(v);const double count=double(v.size());
        if(std::fabs(s[0]-patch.sums[c][0])>1e-6*count||std::fabs(s[1]-patch.sums[c][1])>1e-6*count)throw std::runtime_error("rebuilt clip "+std::to_string(c)+" differs from the release build");
        clips.push_back(std::move(v));
    }
    fs::create_directories(out);
    const Bytes bank=bankBytes(patch.header,patch.clipHeaders,clips);const auto name=params.role+"-choke-bank.bin";
    save(out/name,bank);std::printf("{\"bank\":\"%s\",\"clips\":%zu,\"sha256\":\"%s\"}\n",name.c_str(),clips.size(),hex(sha256(bank)).c_str());
    return 0;
}

int packages(const fs::path& base,const fs::path& recipeFile,const fs::path& out){
    const auto recipes=d2recipe::readRecipes(load(recipeFile));
    const Decls decls(base);
    fs::create_directories(out);
    // game1voidmod: the player's game1 patch entries, then ours, stored unpacked in our resources file.
    auto game1=d2res::readIndex(load(base/"game1_patch.index"));
    Bytes resources={0x04,'S','E','R'};
    for(const auto& r:recipes){
        std::vector<Bytes> sources;for(const auto& s:r.sources)sources.push_back(decls.find(s));
        const auto decl=d2recipe::build(r,sources);
        if(sha256(decl)!=r.sha)throw std::runtime_error("rebuilt decl differs: "+r.source);
        d2res::Entry e;e.id=r.id;e.type=r.type;e.source=r.source;e.destination=r.destination;
        e.offset=resources.size();e.size=e.packed=std::uint32_t(decl.size());e.flags2=28;  // game 1, voidmod level
        game1.push_back(std::move(e));resources.insert(resources.end(),decl.begin(),decl.end());
    }
    std::map<std::string,Bytes> files;
    files["game1voidmod.index"]=d2res::writeIndex(game1);files["game1voidmod.resources"]=resources;
    for(int g=2;g<=4;++g){
        const auto name="game"+std::to_string(g);
        files[name+"voidmod.index"]=load(base/(name+"_patch.index"));
        files[name+"voidmod.resources"]={0x04,'S','E','R'};
    }
    files["master.index"]=d2res::voidmodMaster(load(base/"master.index"));
    std::printf("{\"decls\":%zu,\"files\":{",recipes.size());
    bool first=true;
    for(const auto& [name,bytes]:files){save(out/name,bytes);std::printf("%s\"base/%s\":\"%s\"",first?"":",",name.c_str(),hex(sha256(bytes)).c_str());first=false;}
    std::puts("}}");
    return 0;
}
}  // namespace

int wmain(int argc,wchar_t** argv){
    try{
        const std::wstring command=argc>1?argv[1]:L"";
        if(command==L"packages"&&argc==5)return packages(argv[2],argv[3],argv[4]);
        if(command==L"banks"&&argc==5)return banks(argv[2],argv[3],argv[4]);
        if(command==L"bank-encode"&&argc==6)return bankEncode(argv[2],argv[3],argv[4],argv[5]);
        if(command==L"tables"&&argc==5)return tablesBuild(argv[2],argv[3],argv[4]);
        if(command==L"tables-encode"&&argc==6)return tablesEncode(argv[2],argv[3],argv[4],argv[5]);
        if(command==L"master"&&argc==4){save(argv[3],d2res::voidmodMaster(load(argv[2])));std::puts("{\"master\":\"written\"}");return 0;}
        if(command==L"index-check"&&argc==3){
            const auto bytes=load(argv[2]);const auto entries=d2res::readIndex(bytes);
            const bool same=d2res::writeIndex(entries)==bytes;
            std::printf("{\"entries\":%zu,\"roundTrip\":%s}\n",entries.size(),same?"true":"false");return same?0:1;
        }
        std::cerr<<"usage: d2coop_setup packages GAMEBASE RECIPES OUTDIR | banks GAMEDIR PATCH OUTDIR | tables GAMEDIR RECIPE OUTDIR\n"
                   "       | bank-encode GAMEDIR PARAMS LABBANK OUTPATCH | tables-encode GAMEDIR PARAMS REFERENCE OUTRECIPE\n"
                   "       | master ORIGINAL OUTPUT | index-check INDEX\n";return 2;
    }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
}
