#pragma once
// #30 (stage 7 release): rebuilds our decls from decl recipes (tools/release/build_decl_recipes.py, format
// D2RCP004) and the player's own decls. A decl is rebuilt line by line: indentation + body + trailing whitespace +
// line ending, where bodies come from copies of a source decl's line bodies or from our text, and indentation,
// trailing whitespace and line endings come from runs. The caller supplies each source's unpacked bytes (found by
// type, name and SHA-256 in the player's containers) and checks the result's SHA-256.
#include <array>
#include <cstdint>
#include <cstring>
#include <functional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace d2recipe {

using Bytes=std::vector<std::uint8_t>;
using Hash=std::array<std::uint8_t,32>;

struct Source {std::string type,name;Hash sha{};};
struct Op {bool copy=false;std::string text;std::uint32_t source=0,first=0,end=0;};
struct Recipe {
    std::uint32_t id=0,size=0,lines=0;std::string type,source,destination;Hash sha{};bool finalNewline=false;
    std::vector<std::pair<std::uint32_t,bool>> endings;std::vector<std::pair<std::uint32_t,std::string>> indents,trails;
    std::vector<Source> sources;std::vector<Op> ops;
};

struct Reader {
    const Bytes& d;std::size_t at=0;
    void need(std::size_t n) const {if(at+n>d.size()||at+n<at)throw std::runtime_error("recipe: truncated");}
    std::uint32_t u32(){need(4);std::uint32_t v=0;for(int k=3;k>=0;--k)v=(v<<8)|d[at+std::size_t(k)];at+=4;return v;}
    std::uint8_t u8(){need(1);return d[at++];}
    std::string str(){const auto n=u32();if(n>(1u<<24))throw std::runtime_error("recipe: string too long");need(n);std::string s(reinterpret_cast<const char*>(d.data()+at),n);at+=n;return s;}
    Hash hash(){need(32);Hash h{};std::memcpy(h.data(),d.data()+at,32);at+=32;return h;}
};

inline std::vector<Recipe> readRecipes(const Bytes& d){
    if(d.size()<12||std::memcmp(d.data(),"D2RCP004",8))throw std::runtime_error("recipe: bad magic");
    Reader r{d};r.at=8;const auto count=r.u32();if(count>4096)throw std::runtime_error("recipe: entry count");
    std::vector<Recipe> out(count);
    for(auto& e:out){
        e.id=r.u32();e.type=r.str();e.source=r.str();e.destination=r.str();e.size=r.u32();e.sha=r.hash();
        e.lines=r.u32();e.finalNewline=r.u8()!=0;
        for(auto n=r.u32();n--;){const auto c=r.u32();e.endings.emplace_back(c,r.u8()!=0);}
        for(auto n=r.u32();n--;){const auto c=r.u32();e.indents.emplace_back(c,r.str());}
        for(auto n=r.u32();n--;){const auto c=r.u32();e.trails.emplace_back(c,r.str());}
        for(auto n=r.u32();n--;){Source s;s.type=r.str();s.name=r.str();s.sha=r.hash();e.sources.push_back(std::move(s));}
        for(auto n=r.u32();n--;){
            Op o;const auto kind=r.u8();
            if(kind==0)o.text=r.str();
            else if(kind==1){o.copy=true;o.source=r.u32();o.first=r.u32();o.end=r.u32();}
            else throw std::runtime_error("recipe: op kind");
            e.ops.push_back(std::move(o));
        }
    }
    if(r.at!=d.size())throw std::runtime_error("recipe: trailing bytes");
    return out;
}

inline bool space(char c){return c==' '||c=='\t'||c=='\r';}

// A decl's line bodies: split after each LF; a CR before the LF goes with the ending; spaces, tabs and CRs at
// both ends are trimmed (Python's bytes.strip(b' \t\r') in the builder).
inline std::vector<std::string> bodies(const Bytes& b){
    std::vector<std::string> out;std::size_t at=0;
    while(at<b.size()){
        std::size_t end=at;while(end<b.size()&&b[end]!='\n')++end;
        std::size_t stop=end;if(end<b.size()&&stop>at&&b[stop-1]=='\r')--stop;
        std::size_t first=at,last=stop;
        while(first<last&&space(char(b[first])))++first;
        while(last>first&&space(char(b[last-1])))--last;
        out.emplace_back(reinterpret_cast<const char*>(b.data()+first),last-first);
        at=end+1;
    }
    return out;
}

template<class T> std::vector<T> expand(const std::vector<std::pair<std::uint32_t,T>>& runs,std::uint32_t lines){
    std::vector<T> out;out.reserve(lines);
    for(const auto& run:runs)for(std::uint32_t k=0;k<run.first;++k)out.push_back(run.second);
    if(out.size()!=lines)throw std::runtime_error("recipe: runs do not cover the lines");
    return out;
}

// Builds a decl from its recipe and its sources' unpacked bytes (in the recipe's source order).
inline Bytes build(const Recipe& e,const std::vector<Bytes>& sources){
    if(sources.size()!=e.sources.size())throw std::runtime_error("recipe: sources missing");
    std::vector<std::vector<std::string>> split;for(const auto& s:sources)split.push_back(bodies(s));
    std::vector<std::string> body;
    for(const auto& o:e.ops){
        if(o.copy){
            if(o.source>=split.size()||o.first>o.end||o.end>split[o.source].size())throw std::runtime_error("recipe: copy outside its source");
            body.insert(body.end(),split[o.source].begin()+o.first,split[o.source].begin()+o.end);
        }else{
            std::size_t at=0;
            for(;;){const auto n=o.text.find('\n',at);body.push_back(o.text.substr(at,n==std::string::npos?std::string::npos:n-at));if(n==std::string::npos)break;at=n+1;}
        }
    }
    if(body.size()!=e.lines)throw std::runtime_error("recipe: line count");
    const auto crlf=expand(e.endings,e.lines);const auto indents=expand(e.indents,e.lines);const auto trails=expand(e.trails,e.lines);
    std::string out;out.reserve(e.size);
    for(std::uint32_t k=0;k<e.lines;++k){
        out+=indents[k];out+=body[k];out+=trails[k];
        if(k+1<e.lines||e.finalNewline)out+=crlf[k]?"\r\n":"\n";
    }
    if(out.size()!=e.size)throw std::runtime_error("recipe: size mismatch for "+e.source);
    return Bytes(out.begin(),out.end());
}

}  // namespace d2recipe
