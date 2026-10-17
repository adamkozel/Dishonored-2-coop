// #30: the DLL's native tables reader (native_tables.h) on files written by the installer's writer (table_rebuild.h),
// plus the hand calibration and attachment math on a synthetic first-person skeleton.
#include "native_tables.h"
#include "table_rebuild.h"
#include <cstdio>
#include <cstdlib>

static void require(bool ok,const char* what){if(!ok){std::fprintf(stderr,"FAIL: %s\n",what);std::exit(1);}}

int main(){
    try{
        d2tables::Table parents;parents.name="grenade_parents";parents.i16=true;parents.dims={6};parents.s={-1,0,1,1,1,1};
        d2tables::Table clip;clip.name="grenade_idle";clip.dims={2,6,7};for(int k=0;k<2*6*7;++k)clip.f.push_back(float(k%7==3?1:k*.001));
        const auto bytes=d2tables::writeTables({parents,clip});
        std::map<std::string,NativeTable> tables;
        require(parseNativeTables(bytes,tables)&&tables.size()==2,"tables parse");
        require(tables["grenade_parents"].s==parents.s&&tables["grenade_idle"].f==clip.f&&tables["grenade_idle"].dims==clip.dims,"values and dimensions kept");
        auto bad=bytes;bad.pop_back();require(!parseNativeTables(bad,tables),"truncated file refused");
        bad=bytes;bad[0]='x';require(!parseNativeTables(bad,tables),"magic checked");
        bad=bytes;bad.push_back(0);require(!parseNativeTables(bad,tables),"trailing bytes refused");
        d2tables::Table huge=clip;huge.f[5]=1e9f;require(!parseNativeTables(d2tables::writeTables({huge}),tables),"implausible values refused");
        // Lookup by dimensions through the loader.
        const char* path="native_tables_tests.tmp";FILE* f=nullptr;require(!fopen_s(&f,path,"wb")&&f,"temp file");std::fwrite(bytes.data(),1,bytes.size(),f);std::fclose(f);
        NativeTables loaded;require(loaded.load(path)&&loaded.loaded(),"load from file");std::remove(path);
        require(loaded.find("grenade_idle",{0,6,7})&&!loaded.find("grenade_idle",{0,7,7})&&!loaded.find("grenade_idle",{2,6})&&!loaded.find("missing",{1}),"find checks dimensions");
        NativeTables none;require(!none.load("no-such-native-tables.bin")&&!none.find("grenade_idle",{0,6,7}),"missing file refuses every table");
        // The palm frame is a rotation; the attachment of a bind-pose sample is its bind offset from the wrist.
        std::vector<d2bank::Joint> js;
        const char* names[]={"origin","left_shoulder_jnt","left_arm_jnt","left_elbow_jnt","left_wrist_jnt","left_index_02_jnt","left_pinky_02_jnt","left_medium_02_jnt","left_hand_attachment_jnt"};
        const int par[]={-1,0,1,2,3,4,4,4,4};const float pos[][3]={{0,0,0},{.1f,0,0},{.2f,0,0},{.25f,0,0},{.2f,.01f,0},{.08f,.02f,.01f},{.07f,-.03f,0},{.08f,0,.012f},{.05f,.01f,-.02f}};
        for(int i=0;i<9;++i){d2bank::Joint j;j.name=names[i];j.parent=par[i];j.bind={0,0,0,1,pos[i][0],pos[i][1],pos[i][2],0,1,1,1,1};js.push_back(j);}
        const d2bank::Rig fp(js);
        d2bank::Sample s;s.joints=9;s.frames=2;s.fps=30;for(int frame=0;frame<2;++frame)for(const auto& j:js)s.values.insert(s.values.end(),j.bind.begin(),j.bind.end());
        const auto cal=d2tables::palm(fp,4,5,6,7);double det=cal[0]*(cal[4]*cal[8]-cal[5]*cal[7])-cal[1]*(cal[3]*cal[8]-cal[5]*cal[6])+cal[2]*(cal[3]*cal[7]-cal[4]*cal[6]);
        require(std::fabs(det-1)<1e-12,"palm frame is a rotation");
        const auto v=d2tables::attachment(s,fp,d2bank::eye(),.5);
        require(std::fabs(v[4]-.05)<1e-7&&std::fabs(v[5]-.01)<1e-7&&std::fabs(v[6]+.02)<1e-7&&std::fabs(std::fabs(v[3])-1)<1e-12,"bind attachment offset");
        std::puts("d2_native_tables_tests passed");
        return 0;
    }catch(const std::exception& e){std::fprintf(stderr,"FAIL: uncaught %s\n",e.what());return 1;}
}
