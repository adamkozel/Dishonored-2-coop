#pragma once
// #30: game-derived tables rebuilt by the installer from the player's own animations (d2coop_setup tables,
// table_rebuild.h) instead of being compiled into the DLL: the crossbow and grenade mechanism clips with their
// skeletons' parents and bind poses, and the ranged hand tracks. Product mode reads <data dir>\native-tables.bin,
// lab mode <status path>.tables.bin. Without the file every user of a table refuses (no guessed pose).
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <map>
#include <string>
#include <vector>

struct NativeTable {std::vector<std::uint32_t> dims;std::vector<float> f;std::vector<std::int16_t> s;};

// Parses D2NTB001: magic, u32 count, per table: u32 name length + name, u8 type (0 f32, 1 i16), u32 rank, dims, data.
inline bool parseNativeTables(const std::vector<unsigned char>& d,std::map<std::string,NativeTable>& out){
    out.clear();std::size_t at=8;
    auto need=[&](std::size_t n){return at+n<=d.size()&&at+n>=at;};
    auto u32=[&](std::uint32_t& v){if(!need(4))return false;std::memcpy(&v,d.data()+at,4);at+=4;return true;};
    std::uint32_t count=0;if(d.size()<12||std::memcmp(d.data(),"D2NTB001",8)||!u32(count)||count>256)return false;
    for(std::uint32_t k=0;k<count;++k){
        std::uint32_t len=0,rank=0;if(!u32(len)||len>128||!need(len))return false;std::string name(reinterpret_cast<const char*>(d.data()+at),len);at+=len;
        if(!need(1))return false;const auto type=d[at++];if(type>1||!u32(rank)||!rank||rank>5)return false;
        NativeTable t;std::uint64_t n=1;for(std::uint32_t r=0;r<rank;++r){std::uint32_t v=0;if(!u32(v)||!v)return false;t.dims.push_back(v);n*=v;if(n>(1u<<22))return false;}
        const std::size_t bytes=std::size_t(n)*(type?2:4);if(!need(bytes))return false;
        if(type){t.s.resize(std::size_t(n));std::memcpy(t.s.data(),d.data()+at,bytes);}else{t.f.resize(std::size_t(n));std::memcpy(t.f.data(),d.data()+at,bytes);}
        at+=bytes;
        for(float v:t.f)if(!(v>-1000.f&&v<1000.f))return false;
        out.emplace(std::move(name),std::move(t));
    }
    return at==d.size();
}

class NativeTables {
public:
    // Loads once; later calls report the first result.
    bool load(const char* path){
        if(tried)return ok;tried=true;
        FILE* f=nullptr;if(fopen_s(&f,path,"rb")||!f)return false;
        std::vector<unsigned char> d;unsigned char buffer[65536];std::size_t n;
        while((n=std::fread(buffer,1,sizeof(buffer),f))>0&&d.size()<(64u<<20))d.insert(d.end(),buffer,buffer+n);
        std::fclose(f);ok=parseNativeTables(d,tables);return ok;
    }
    // A table with the expected dimensions (0 = any), or nullptr.
    const NativeTable* find(const char* name,std::initializer_list<std::uint32_t> dims) const {
        if(!ok)return nullptr;const auto it=tables.find(name);if(it==tables.end()||it->second.dims.size()!=dims.size())return nullptr;
        std::size_t k=0;for(auto d:dims){if(d&&it->second.dims[k]!=d)return nullptr;++k;}return &it->second;
    }
    bool loaded() const {return ok;}
private:
    bool tried=false,ok=false;std::map<std::string,NativeTable> tables;
};
