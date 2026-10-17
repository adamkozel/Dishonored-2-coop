#pragma once
// #30 (stage 7 release): a zlib (RFC 1950) / deflate (RFC 1951) decoder for the
// installer. The game's resource containers store packed entries as zlib streams
// (tools/diag/d2_resource_index.py), and the release's patches are zlib streams
// written by the release builder. Written for this project (no third-party code):
// a canonical-Huffman bit decoder that reads codes one bit at a time.
#include <algorithm>
#include <cstdint>
#include <stdexcept>
#include <vector>

namespace d2inflate {

struct Input {
    const std::uint8_t* data;std::size_t size,pos=0;std::uint32_t buffer=0;int count=0;
    int bits(int need){
        std::uint32_t value=buffer;
        while(count<need){if(pos>=size)throw std::runtime_error("inflate: input ended");value|=std::uint32_t(data[pos++])<<count;count+=8;}
        buffer=need<32?value>>need:0;count-=need;return int(value&((need<32?(1u<<need):0u)-1u));
    }
};

struct Huffman {std::uint16_t count[16]{};std::uint16_t symbol[320]{};};

// Codes are stored most-significant bit first, so a code is built one bit at a time.
inline int decode(Input& in,const Huffman& h){
    int code=0,first=0,index=0;
    for(int length=1;length<=15;++length){
        code|=in.bits(1);const int count=h.count[length];
        if(code-count<first)return h.symbol[index+(code-first)];
        index+=count;first+=count;first<<=1;code<<=1;
    }
    throw std::runtime_error("inflate: invalid code");
}

// Builds the decoding tables; returns 0 for a complete code, > 0 for an incomplete one, < 0 for an over-subscribed one.
inline int build(Huffman& h,const std::uint16_t* lengths,int n){
    for(auto& c:h.count)c=0;
    for(int s=0;s<n;++s)h.count[lengths[s]]++;
    if(h.count[0]==n)return 0;
    int left=1;
    for(int length=1;length<=15;++length){left<<=1;left-=h.count[length];if(left<0)return left;}
    std::uint16_t offsets[16]{};
    for(int length=1;length<15;++length)offsets[length+1]=std::uint16_t(offsets[length]+h.count[length]);
    for(int s=0;s<n;++s)if(lengths[s])h.symbol[offsets[lengths[s]]++]=std::uint16_t(s);
    return left;
}

inline void codes(Input& in,std::vector<std::uint8_t>& out,const Huffman& lengthCode,const Huffman& distanceCode){
    static const std::uint16_t lengthBase[29]={3,4,5,6,7,8,9,10,11,13,15,17,19,23,27,31,35,43,51,59,67,83,99,115,131,163,195,227,258};
    static const std::uint16_t lengthExtra[29]={0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,0};
    static const std::uint16_t distanceBase[30]={1,2,3,4,5,7,9,13,17,25,33,49,65,97,129,193,257,385,513,769,1025,1537,2049,3073,4097,6145,8193,12289,16385,24577};
    static const std::uint16_t distanceExtra[30]={0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13};
    for(;;){
        int symbol=decode(in,lengthCode);
        if(symbol<256){out.push_back(std::uint8_t(symbol));continue;}
        if(symbol==256)return;
        symbol-=257;if(symbol>=29)throw std::runtime_error("inflate: invalid length symbol");
        const std::size_t length=lengthBase[symbol]+std::size_t(in.bits(lengthExtra[symbol]));
        const int d=decode(in,distanceCode);if(d>=30)throw std::runtime_error("inflate: invalid distance symbol");
        const std::size_t distance=distanceBase[d]+std::size_t(in.bits(distanceExtra[d]));
        if(distance>out.size())throw std::runtime_error("inflate: distance before the start");
        const std::size_t from=out.size()-distance;
        for(std::size_t k=0;k<length;++k)out.push_back(out[from+k]);  // overlapping copies repeat bytes
    }
}

inline void stored(Input& in,std::vector<std::uint8_t>& out){
    in.buffer=0;in.count=0;  // to the next byte boundary
    if(in.pos+4>in.size)throw std::runtime_error("inflate: stored header ended");
    const unsigned length=in.data[in.pos]|(in.data[in.pos+1]<<8),inverse=in.data[in.pos+2]|(in.data[in.pos+3]<<8);in.pos+=4;
    if(length!=(~inverse&0xffffu))throw std::runtime_error("inflate: stored length check");
    if(in.pos+length>in.size)throw std::runtime_error("inflate: stored block ended");
    out.insert(out.end(),in.data+in.pos,in.data+in.pos+length);in.pos+=length;
}

inline void fixed(Input& in,std::vector<std::uint8_t>& out){
    static Huffman lengthCode,distanceCode;static bool ready=false;
    if(!ready){
        std::uint16_t lengths[288];
        for(int s=0;s<144;++s)lengths[s]=8;for(int s=144;s<256;++s)lengths[s]=9;for(int s=256;s<280;++s)lengths[s]=7;for(int s=280;s<288;++s)lengths[s]=8;
        build(lengthCode,lengths,288);
        for(int s=0;s<30;++s)lengths[s]=5;
        build(distanceCode,lengths,30);ready=true;
    }
    codes(in,out,lengthCode,distanceCode);
}

inline void dynamic(Input& in,std::vector<std::uint8_t>& out){
    static const int order[19]={16,17,18,0,8,7,9,6,10,5,11,4,12,3,13,2,14,1,15};
    const int lengthCount=in.bits(5)+257,distanceCount=in.bits(5)+1,codeCount=in.bits(4)+4;
    if(lengthCount>286||distanceCount>30)throw std::runtime_error("inflate: bad counts");
    std::uint16_t lengths[320]{};
    for(int k=0;k<codeCount;++k)lengths[order[k]]=std::uint16_t(in.bits(3));
    Huffman codeLengths;if(build(codeLengths,lengths,19)!=0)throw std::runtime_error("inflate: incomplete code-length code");
    for(int k=0;k<lengthCount+distanceCount;){
        int symbol=decode(in,codeLengths);
        if(symbol<16){lengths[k++]=std::uint16_t(symbol);continue;}
        std::uint16_t value=0;int repeat=0;
        if(symbol==16){if(!k)throw std::runtime_error("inflate: repeat with no previous length");value=lengths[k-1];repeat=3+in.bits(2);}
        else if(symbol==17)repeat=3+in.bits(3);
        else repeat=11+in.bits(7);
        if(k+repeat>lengthCount+distanceCount)throw std::runtime_error("inflate: too many lengths");
        while(repeat--)lengths[k++]=value;
    }
    if(!lengths[256])throw std::runtime_error("inflate: no end-of-block code");
    Huffman lengthCode,distanceCode;
    const int a=build(lengthCode,lengths,lengthCount);if(a<0||(a>0&&lengthCount-lengthCode.count[0]!=1))throw std::runtime_error("inflate: bad literal/length code");
    const int b=build(distanceCode,lengths+lengthCount,distanceCount);if(b<0||(b>0&&distanceCount-distanceCode.count[0]!=1))throw std::runtime_error("inflate: bad distance code");
    codes(in,out,lengthCode,distanceCode);
}

// Raw deflate data; returns the bytes consumed.
inline std::size_t raw(const std::uint8_t* data,std::size_t size,std::vector<std::uint8_t>& out){
    Input in{data,size};
    for(int last=0;!last;){
        last=in.bits(1);const int type=in.bits(2);
        if(type==0)stored(in,out);else if(type==1)fixed(in,out);else if(type==2)dynamic(in,out);
        else throw std::runtime_error("inflate: invalid block type");
    }
    return in.pos;
}

inline std::uint32_t adler32(const std::uint8_t* data,std::size_t size){
    std::uint32_t a=1,b=0;
    for(std::size_t k=0;k<size;){const std::size_t n=std::min<std::size_t>(size-k,5552);for(std::size_t e=k+n;k<e;++k){a+=data[k];b+=a;}a%=65521;b%=65521;}
    return (b<<16)|a;
}

// A zlib stream (header, deflate data, big-endian Adler-32 of the output).
inline std::vector<std::uint8_t> zlib(const std::uint8_t* data,std::size_t size,std::size_t expected=0){
    if(size<6)throw std::runtime_error("inflate: zlib stream too short");
    const unsigned cmf=data[0],flg=data[1];
    if((cmf&15)!=8||(cmf>>4)>7||((cmf<<8)|flg)%31||(flg&0x20))throw std::runtime_error("inflate: not a plain zlib stream");
    std::vector<std::uint8_t> out;out.reserve(expected);
    const std::size_t used=2+raw(data+2,size-2,out);
    if(used+4>size)throw std::runtime_error("inflate: missing checksum");
    const std::uint32_t check=(std::uint32_t(data[used])<<24)|(std::uint32_t(data[used+1])<<16)|(std::uint32_t(data[used+2])<<8)|data[used+3];
    if(check!=adler32(out.data(),out.size()))throw std::runtime_error("inflate: checksum mismatch");
    if(expected&&out.size()!=expected)throw std::runtime_error("inflate: unexpected size");
    return out;
}

}  // namespace d2inflate
