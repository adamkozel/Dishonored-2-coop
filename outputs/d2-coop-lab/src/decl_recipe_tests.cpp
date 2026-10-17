// #30: decl recipes (D2RCP004) rebuilt from synthetic sources: copies, our text, endings, indentation, trailing runs.
#include "decl_recipe.h"
#include <cstdio>
#include <cstdlib>

static void require(bool ok,const char* what){if(!ok){std::fprintf(stderr,"FAIL: %s\n",what);std::exit(1);}}
template<class F>static bool throws(F f){try{f();}catch(const std::runtime_error&){return true;}return false;}

struct W {
    d2recipe::Bytes d;
    void u32(std::uint32_t v){for(int k=0;k<4;++k)d.push_back(std::uint8_t(v>>(8*k)));}
    void u8(std::uint8_t v){d.push_back(v);}
    void str(const std::string& s){u32(std::uint32_t(s.size()));d.insert(d.end(),s.begin(),s.end());}
    void hash(){for(int k=0;k<32;++k)d.push_back(std::uint8_t(k));}
};

static int run();
int main(){
    try{return run();}catch(const std::exception& e){std::fprintf(stderr,"FAIL: uncaught %s\n",e.what());return 1;}
}
static int run(){
    using namespace d2recipe;
    // The game's source uses CRLF and tabs; our decl re-indents with spaces, uses LF, inserts one line of ours and
    // keeps a CR CR LF ending on its last line.
    const std::string source="{\r\n\tm_a = 1;\r\n\tm_b = 2;\r\n\tm_c = 3;\r\n}\r\n";
    const std::string expected="{\n  m_a = 1;\n  m_ours = 7;\n  m_c = 3;\n}\r\r\n";
    W w;w.d={'D','2','R','C','P','0','0','4'};w.u32(1);
    w.u32(36063);w.str("entityDef");w.str("models/d2coop/x.def");w.str("generated/x.decl");w.u32(std::uint32_t(expected.size()));w.hash();
    w.u32(5);w.u8(1);                                        // five lines, final newline
    w.u32(2);w.u32(4);w.u8(0);w.u32(1);w.u8(1);             // endings: four LF, one CRLF
    w.u32(3);w.u32(1);w.str("");w.u32(3);w.str("  ");w.u32(1);w.str("");  // indents
    w.u32(2);w.u32(4);w.str("");w.u32(1);w.str("\r");        // trails: the last line keeps one CR
    w.u32(1);w.str("entityDef");w.str("models/source.def");w.hash();
    w.u32(3);
    w.u8(1);w.u32(0);w.u32(0);w.u32(2);                      // copy "{", "m_a = 1;"
    w.u8(0);w.str("m_ours = 7;");                            // our line
    w.u8(1);w.u32(0);w.u32(3);w.u32(5);                      // copy "m_c = 3;", "}"
    const auto recipes=readRecipes(w.d);
    require(recipes.size()==1&&recipes[0].id==36063&&recipes[0].sources.size()==1&&recipes[0].ops.size()==3,"recipe read");
    const Bytes src(source.begin(),source.end());
    const auto b=bodies(src);
    require(b.size()==5&&b[1]=="m_a = 1;"&&b[4]=="}","source bodies trim indentation and the CR of CRLF");
    const auto out=build(recipes[0],{src});
    require(std::string(out.begin(),out.end())==expected,"decl rebuilt byte for byte");
    require(throws([&]{build(recipes[0],{});}),"missing source refused");
    auto bad=w.d;bad.push_back(0);require(throws([&]{readRecipes(bad);}),"trailing byte refused");
    auto wrong=recipes[0];wrong.ops[2].end=9;require(throws([&]{build(wrong,{src});}),"copy outside the source refused");
    wrong=recipes[0];wrong.lines=6;require(throws([&]{build(wrong,{src});}),"line count mismatch refused");
    std::puts("d2_decl_recipe_tests passed");
    return 0;
}
