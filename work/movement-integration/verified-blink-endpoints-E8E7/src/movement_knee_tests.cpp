#include "movement_knee_pole.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
void need(bool v,const char* message){if(!v){std::cerr<<message<<'\n';std::exit(1);}}
float distance(const BodyMatrix& a,const BodyMatrix& b){float d=0;for(unsigned k=12;k<15;++k)d+=(a.v[k]-b.v[k])*(a.v[k]-b.v[k]);return std::sqrt(d);}
void check(const BodyMatrix& parent,const BodyMatrix* ref,const BodyMatrix* solved){
    BodyMatrix local[3]{},out[3]{};float angle=0;
    need(movementKneePole(parent,ref,solved,local,angle),"Valid captured chain refused");
    for(unsigned j=0;j<3;++j)out[j]=bodyMultiply(j?out[j-1]:parent,local[j]);
    need(distance(out[0],solved[0])<.00001f&&distance(out[2],solved[2])<.00001f,"Hip or ankle endpoint moved");
    for(unsigned k=0;k<12;++k)if(k%4!=3)need(std::fabs(out[2].v[k]-solved[2].v[k])<.00001f,"Native ankle rotation changed");
    for(unsigned j=1;j<3;++j)need(std::fabs(distance(out[j],out[j-1])-distance(solved[j],solved[j-1]))<.00001f,"Native segment length changed");
    float axis[3]{},a[3]{},b[3]{},length=0,da=0,db=0;
    for(unsigned k=0;k<3;++k){axis[k]=out[2].v[12+k]-out[0].v[12+k];length+=axis[k]*axis[k];a[k]=out[1].v[12+k]-out[0].v[12+k];b[k]=ref[1].v[12+k]-ref[0].v[12+k];}
    length=std::sqrt(length);for(unsigned k=0;k<3;++k){axis[k]/=length;da+=axis[k]*a[k];db+=axis[k]*b[k];}
    float an=0,bn=0,dot=0;for(unsigned k=0;k<3;++k){a[k]-=da*axis[k];b[k]-=db*axis[k];an+=a[k]*a[k];bn+=b[k]*b[k];dot+=a[k]*b[k];}
    if(an>.000025f&&bn>.000025f)need(dot/std::sqrt(an*bn)>.99999f,"Authored knee direction not restored");
}
int main(int argc,char** argv){
    BodyMatrix parent=bodyIdentity(),ref[3]{bodyIdentity(),bodyIdentity(),bodyIdentity()},solved[3];
    ref[0].v[14]=1;ref[1].v[12]=.3f;ref[1].v[14]=.5f;
    std::copy(std::begin(ref),std::end(ref),solved);solved[1].v[12]=0;solved[1].v[13]=.3f;
    check(parent,ref,solved);
    solved[1].v[13]=0;check(parent,ref,solved); // Straight leg: no unstable rotation.
    BodyMatrix out[3];float angle=0;solved[2]=solved[0];need(!movementKneePole(parent,ref,solved,out,angle),"Zero-length chain accepted");
    unsigned captures=0;
    if(argc==2){std::ifstream f(argv[1],std::ios::binary);need(bool(f.read(reinterpret_cast<char*>(&captures),4))&&captures>0&&captures<10000,"Invalid fixture header");
        for(unsigned i=0;i<captures;++i){BodyMatrix row[7];need(bool(f.read(reinterpret_cast<char*>(row),sizeof(row))),"Truncated capture");check(row[0],row+1,row+4);}
        need(f.peek()==std::char_traits<char>::eof(),"Trailing fixture data");
    }
    std::cout<<"PASS_KNEE_POLE: endpoints, native lengths, ankle orientation, authored direction, degenerate refusal; captured="<<captures<<'\n';
}
