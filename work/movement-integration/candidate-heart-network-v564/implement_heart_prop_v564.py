from pathlib import Path
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab/src'
p=(S/'grenade_prop.h').read_text().replace('grenade','heart').replace('Grenade','Heart')
p=p.replace('#include "heart_prop_calibration.h"\n','').replace('#include "heart_mechanism.h"\n','').replace('variant>8','variant>3')
p=p.replace('    stopHeartMechanism();\n','')
p='\n'.join(line for line in p.splitlines() if 'publishHeartMechanism' not in line)
p=p.replace('rangedPropDefinition(3)','rangedPropDefinition(4)').replace('p.kind=3','p.kind=4')
p=p.replace('if(!heartPropIdleCalibration(localRole?1u:2u,offset,axis)){refuse("calibration_missing");return;}', '''{
            std::array<float,12> qts{};std::copy(heartHandTracks[localRole?0:1][0][0],heartHandTracks[localRole?0:1][0][0]+7,qts.begin());qts[8]=qts[9]=qts[10]=1;
            BodyMatrix m{};if(!bodyFromQts(qts,m)){refuse("calibration_missing");return;}
            for(unsigned r=0;r<3;++r){offset[r]=m.v[12+r];for(unsigned c=0;c<3;++c)axis[c*3+r]=m.v[c*4+r];}
        }''')
a=p.index('    AcquireSRWLockShared(&heartMechanismLock);');b=p.rfind('\n',a,p.index('testFixtureOnly',a))+1
p=p[:a]+p[b:]
p=p.replace('fprintf(f,"\\"testFixtureOnly', 'fprintf(f,"{\\"testFixtureOnly')
(S/'heart_prop.h').write_text(p+'\n')
q=S/'ranged_prop.h';s=q.read_text().replace('    if(kind==3)return "models/weapons/d2coop/grenade_prop.def";','    if(kind==3)return "models/weapons/d2coop/grenade_prop.def";\n    if(kind==4)return "models/weapons/d2coop/heart_prop.def";');q.write_text(s)
q=S/'engine_bridge.cpp';s=q.read_text().replace('#include "grenade_prop.h"','#include "grenade_prop.h"\n#include "heart_prop.h"').replace('serviceGrenadeProp(tick);','serviceGrenadeProp(tick);serviceHeartProp(tick);').replace('writeGrenadeProp();','writeGrenadeProp();writeHeartProp();');q.write_text(s)
print('Heart visual prop reuses existing render-only spawn, native wrist binding and hide lifecycle; no native Heart gameplay component.')
