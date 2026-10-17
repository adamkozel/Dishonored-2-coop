#pragma once
// #30 (stage 7 release): Dishonored 2 resource containers, read and written by the installer
// (work/release/payload-classification-v1312/README.md; confirmed static against the game's files).
// - gameN*.index: magic 05 'SER', big-endian size (the file size - 32), 24 zero bytes, big-endian entry
//   count; per entry a big-endian id, three little-endian-length strings (type, source, destination),
//   big-endian u64 offset, u32 size, u32 packed size, u64 flags1, u16 flags2. Packed entries (packed !=
//   size) are zlib streams; flags2 names the .resources file (bits 0-1: game number - 1, bits 2+: patch level).
// - gameN*.resources: magic 04 'SER', then entry data at the offsets the index gives.
// - master.index: magic 04 'SER', u16 version, u16 base count, base (index, resources) name pairs, a
//   big-endian u32 patch count, patch (resources name, u16 game) entries, then a tail kept as bytes.
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace d2res {

using Bytes=std::vector<std::uint8_t>;

struct Reader {
    const Bytes& d;std::size_t at=0;
    void need(std::size_t n) const {if(at+n>d.size())throw std::runtime_error("container: truncated");}
    std::uint64_t be(int n){need(std::size_t(n));std::uint64_t v=0;for(int k=0;k<n;++k)v=(v<<8)|d[at++];return v;}
    std::uint32_t le32(){need(4);std::uint32_t v=std::uint32_t(d[at])|(std::uint32_t(d[at+1])<<8)|(std::uint32_t(d[at+2])<<16)|(std::uint32_t(d[at+3])<<24);at+=4;return v;}
    std::string str(){const std::uint32_t n=le32();if(n>4096)throw std::runtime_error("container: string too long");need(n);std::string s(reinterpret_cast<const char*>(d.data()+at),n);at+=n;return s;}
};
struct Writer {
    Bytes d;
    void be(std::uint64_t v,int n){for(int k=n-1;k>=0;--k)d.push_back(std::uint8_t(v>>(8*k)));}
    void le32(std::uint32_t v){for(int k=0;k<4;++k)d.push_back(std::uint8_t(v>>(8*k)));}
    void str(const std::string& s){le32(std::uint32_t(s.size()));d.insert(d.end(),s.begin(),s.end());}
    void raw(const void* p,std::size_t n){const auto* b=static_cast<const std::uint8_t*>(p);d.insert(d.end(),b,b+n);}
};

struct Entry {std::uint32_t id=0;std::string type,source,destination;std::uint64_t offset=0;std::uint32_t size=0,packed=0;std::uint64_t flags1=0;std::uint16_t flags2=0;};

inline std::vector<Entry> readIndex(const Bytes& d){
    if(d.size()<0x24||std::memcmp(d.data(),"\x05SER",4))throw std::runtime_error("index: bad magic");
    Reader r{d};r.at=4;
    if(r.be(4)!=d.size()-32)throw std::runtime_error("index: size field mismatch");
    for(std::size_t k=8;k<0x20;++k)if(d[k])throw std::runtime_error("index: reserved bytes not zero");
    r.at=0x20;const auto count=r.be(4);if(count>1000000)throw std::runtime_error("index: entry count");
    std::vector<Entry> out;out.reserve(std::size_t(count));
    for(std::uint64_t k=0;k<count;++k){
        Entry e;e.id=std::uint32_t(r.be(4));e.type=r.str();e.source=r.str();e.destination=r.str();
        e.offset=r.be(8);e.size=std::uint32_t(r.be(4));e.packed=std::uint32_t(r.be(4));e.flags1=r.be(8);e.flags2=std::uint16_t(r.be(2));
        out.push_back(std::move(e));
    }
    if(r.at!=d.size())throw std::runtime_error("index: trailing bytes");
    return out;
}

inline Bytes writeIndex(const std::vector<Entry>& entries){
    Writer w;w.raw("\x05SER",4);w.be(0,4);for(int k=0;k<24;++k)w.d.push_back(0);w.be(entries.size(),4);
    for(const auto& e:entries){w.be(e.id,4);w.str(e.type);w.str(e.source);w.str(e.destination);w.be(e.offset,8);w.be(e.size,4);w.be(e.packed,4);w.be(e.flags1,8);w.be(e.flags2,2);}
    const std::uint64_t size=w.d.size()-32;for(int k=0;k<4;++k)w.d[4+k]=std::uint8_t(size>>(8*(3-k)));
    return w.d;
}

// The .resources file an entry's data lives in.
inline std::string resourcesName(std::uint16_t flags2){
    static const char* suffix[]={"","_001","_002","_003","_004","_005","_patch","voidmod"};
    if((flags2>>2)>=8)throw std::runtime_error("index: unknown patch level");
    return "game"+std::to_string((flags2&3)+1)+suffix[flags2>>2]+".resources";
}

struct Master {std::uint16_t version=0;std::vector<std::pair<std::string,std::string>> bases;std::vector<std::pair<std::string,std::uint16_t>> patches;Bytes tail;};

inline Master readMaster(const Bytes& d){
    if(d.size()<8||std::memcmp(d.data(),"\x04SER",4))throw std::runtime_error("master: bad magic");
    Reader r{d};r.at=4;Master m;m.version=std::uint16_t(r.be(2));const auto bases=r.be(2);
    for(std::uint64_t k=0;k<bases;++k){auto a=r.str();auto b=r.str();m.bases.emplace_back(std::move(a),std::move(b));}
    const auto patches=r.be(4);if(patches>4096)throw std::runtime_error("master: patch count");
    for(std::uint64_t k=0;k<patches;++k){auto n=r.str();m.patches.emplace_back(std::move(n),std::uint16_t(r.be(2)));}
    m.tail.assign(d.begin()+std::ptrdiff_t(r.at),d.end());
    return m;
}

inline Bytes writeMaster(const Master& m){
    Writer w;w.raw("\x04SER",4);w.be(m.version,2);w.be(m.bases.size(),2);
    for(const auto& b:m.bases){w.str(b.first);w.str(b.second);}
    w.be(m.patches.size(),4);for(const auto& p:m.patches){w.str(p.first);w.be(p.second,2);}
    w.raw(m.tail.data(),m.tail.size());return w.d;
}

// The mod's master.index from the original: each game's newest index is its voidmod index, and the four
// voidmod resources files join the patch list (confirmed static: the lab's master.index is exactly this).
inline Bytes voidmodMaster(const Bytes& original){
    auto m=readMaster(original);
    for(auto& b:m.bases){
        const auto at=b.first.find("_patch.index");if(at==std::string::npos)throw std::runtime_error("master: base index is not a patch index");
        b.first=b.first.substr(0,at)+"voidmod.index";
    }
    for(const auto& p:m.patches)if(p.first.find("voidmod")!=std::string::npos)throw std::runtime_error("master: already modded");
    for(std::uint16_t g=0;g<m.bases.size();++g)m.patches.emplace_back("game"+std::to_string(g+1)+"voidmod.resources",g);
    return writeMaster(m);
}

}  // namespace d2res
