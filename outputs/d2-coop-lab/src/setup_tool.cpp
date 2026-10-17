// #30 (stage 7 release): d2coop_setup, the installer's generator. It builds the game-derived payload from
// the player's own install, so the release ships none of it. The installer checks every output against the
// SHA-256 the release builder recorded from the lab build.
//   d2coop_setup packages <game base dir> <decl recipes> <output dir>
//       base/master.index, the four gameNvoidmod.index and gameNvoidmod.resources files
//   d2coop_setup master <original master.index> <output>
//   d2coop_setup index-check <index>                     (reads and writes back; must match byte for byte)
#include "decl_recipe.h"
#include "inflate.h"
#include "resource_container.h"
#include <windows.h>
#include <bcrypt.h>
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

// The player's decls: each (type, name) in the newest index of games 1-3, unpacked on demand.
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
    Bytes find(const d2recipe::Source& s) const {
        auto range=byName.equal_range({s.type,s.name});
        for(auto it=range.first;it!=range.second;++it){auto data=unpacked(entries[it->second]);if(sha256(data)==s.sha)return data;}
        throw std::runtime_error("source decl not found in this install: "+s.type+" "+s.name);
    }
};

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
        if(command==L"master"&&argc==4){save(argv[3],d2res::voidmodMaster(load(argv[2])));std::puts("{\"master\":\"written\"}");return 0;}
        if(command==L"index-check"&&argc==3){
            const auto bytes=load(argv[2]);const auto entries=d2res::readIndex(bytes);
            const bool same=d2res::writeIndex(entries)==bytes;
            std::printf("{\"entries\":%zu,\"roundTrip\":%s}\n",entries.size(),same?"true":"false");return same?0:1;
        }
        std::cerr<<"usage: d2coop_setup packages GAMEBASE RECIPES OUTDIR | master ORIGINAL OUTPUT | index-check INDEX\n";return 2;
    }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
}
