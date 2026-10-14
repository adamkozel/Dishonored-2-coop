#include "review_clip.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <limits>
void check(bool ok,const char* text){if(!ok)throw std::runtime_error(text);}
template<class T>void put(std::vector<unsigned char>& b,size_t at,T value){std::memcpy(b.data()+at,&value,sizeof(value));}
int main(int argc,char** argv){try{
    for(unsigned n:{215u,219u}){
        const auto layout=n==215?0x00db32d37719931full:0x84db84f5d1336b80ull;
        std::vector<unsigned char> good(28+3*n*28);std::memcpy(good.data(),"D2NATV01",8);put(good,8,n);put(good,12,3u);put(good,16,30.f);put(good,20,layout);
        for(unsigned f=0;f<3;++f)for(unsigned j=0;j<n;++j)put(good,28+(f*n+j)*28+12,1.f);
        put(good,28+(n+2)*28+16,.1f);put(good,28+(2*n+2)*28+16,.2f);
        ReviewClip clip;check(clip.decode(good,n,layout),"valid role rejected");check(std::fabs(clip.sample(2,.5f/30)[4]-.05f)<1e-6,"half-frame interpolation");check(clip.sample(2,9)[4]==.2f,"endpoint clamp");
        check(!clip.decode(good,n==215?219:215,layout),"wrong role accepted");
        auto bad=good;bad.push_back(0);check(!clip.decode(bad,n,layout),"trailing bytes accepted");
        bad=good;put(bad,16,0.f);check(!clip.decode(bad,n,layout),"zero fps");
        bad=good;put(bad,12,1802u);check(!clip.decode(bad,n,layout),"excess frames");
        bad=good;put(bad,28+16,.01f);check(!clip.decode(bad,n,layout),"actor root motion accepted");
        bad=good;put(bad,28+28+16,.01f);check(!clip.decode(bad,n,layout),"anchor motion accepted");
        bad=good;put(bad,28+2*28,std::numeric_limits<float>::quiet_NaN());check(!clip.decode(bad,n,layout),"NaN accepted");
        bad=good;put(bad,28+2*28+12,.5f);check(!clip.decode(bad,n,layout),"invalid rotation accepted");
        bad=good;put(bad,20,layout+1);check(!clip.decode(bad,n,layout),"unknown layout accepted");
        check(clip.values.empty(),"failed load retained stale poses");
    }
    for(int i=1;i<argc;++i){std::ifstream in(argv[i],std::ios::binary);check(bool(in),"asset open");std::vector<unsigned char>b((std::istreambuf_iterator<char>(in)),{});unsigned n=0;check(b.size()>=28,"short asset");std::memcpy(&n,b.data()+8,4);ReviewClip clip;
        check(clip.decode(b,n,n==215?0x00db32d37719931full:0x84db84f5d1336b80ull),"real authored asset rejected");std::cout<<"Validated "<<argv[i]<<" "<<n<<" joints "<<clip.frames<<" frames\n";}
    std::cout<<"Review clip validation passed\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
