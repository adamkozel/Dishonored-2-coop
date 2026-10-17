// #30: the installer's zlib/deflate decoder against streams made by Python's zlib
// (inflate_test_vectors.h, tools/release/make_inflate_vectors.py) and a stored stream built here.
#include "inflate.h"
#include "inflate_test_vectors.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

static void require(bool ok,const char* what){if(!ok){std::fprintf(stderr,"FAIL: %s\n",what);std::exit(1);}}

static bool throws(const std::vector<std::uint8_t>& stream){
    try{d2inflate::zlib(stream.data(),stream.size());}catch(const std::runtime_error&){return true;}
    return false;
}

int main(){
    for(const auto& v:inflateVectors){
        const auto out=d2inflate::zlib(v.packed,v.packedSize,v.size);
        const std::string name=v.name;
        require(out.size()==v.size,("size "+name).c_str());
        require(d2inflate::adler32(out.data(),out.size())==v.adler,("content "+name).c_str());
    }
    // A stored stream of three blocks (65535 + 65535 + 7 bytes) with pseudo-random content.
    std::vector<std::uint8_t> data(65535*2+7);std::uint32_t x=1310;
    for(auto& b:data){x=x*1664525u+1013904223u;b=std::uint8_t(x>>24);}
    std::vector<std::uint8_t> stream={0x78,0x01};
    for(std::size_t at=0;at<data.size();){
        const std::size_t n=std::min<std::size_t>(65535,data.size()-at);const bool last=at+n==data.size();
        stream.push_back(last?1:0);stream.push_back(std::uint8_t(n));stream.push_back(std::uint8_t(n>>8));
        stream.push_back(std::uint8_t(~n));stream.push_back(std::uint8_t(~n>>8));
        stream.insert(stream.end(),data.begin()+std::ptrdiff_t(at),data.begin()+std::ptrdiff_t(at+n));at+=n;
    }
    const std::uint32_t adler=d2inflate::adler32(data.data(),data.size());
    for(int s=24;s>=0;s-=8)stream.push_back(std::uint8_t(adler>>s));
    require(d2inflate::zlib(stream.data(),stream.size())==data,"stored blocks round trip");
    // Damaged streams are refused, never returned.
    auto bad=stream;bad[bad.size()-1]^=1;require(throws(bad),"checksum mismatch refused");
    bad=stream;bad.resize(bad.size()-100);require(throws(bad),"truncated stream refused");
    bad=stream;bad[0]=0x79;require(throws(bad),"bad header refused");
    std::vector<std::uint8_t> fixedShort(fixed_short_packed,fixed_short_packed+fixed_short_packed_size);
    fixedShort[3]^=0x40;require(throws(fixedShort),"corrupt fixed block refused");
    std::puts("d2_inflate_tests passed");
    return 0;
}
