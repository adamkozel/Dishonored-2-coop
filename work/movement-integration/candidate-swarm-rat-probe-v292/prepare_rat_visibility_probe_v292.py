from pathlib import Path
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab/src'
p=S/'native_skeleton.h';s=p.read_text(encoding='utf-8-sig')
s=s.replace('Actor,PlayerArm,CosmeticRanged,PlayerRanged','Actor,PlayerArm,CosmeticRanged,PlayerRanged,CosmeticRat')
s=s.replace('const bool arm=scope==NativeSkeletonScope::PlayerArm;','const bool arm=scope==NativeSkeletonScope::PlayerArm;const bool rat=scope==NativeSkeletonScope::CosmeticRat;')
s=s.replace('base+(cosmetic?0x1c87d58:', 'base+((cosmetic||rat)?0x1c87d58:')
anchor='    for(unsigned i=0;i<(flags&0xffffff);++i) {'
guard='''    if(rat){std::uintptr_t definition=0,pointer=0;std::string text;unsigned parent=0;
        if(handle==1||(flags&0xffffff)!=4||!read(s.actor+0xac,&parent,4)||parent!=0x3fff0010||
           !read(s.actor+0x2e8,&definition,8)||!definition||!read(definition,&vt,8)||vt!=base+0x1ab58f8||
           !read(definition+8,&pointer,8)||!worldIdentityString(read,pointer,text)||text!="models/powers/d2coop/swarm_rat_visual.def")return fail(__LINE__);
        unsigned seen=0;constexpr std::uintptr_t expected[]={0x1b80408,0x1c59ac8,0x1c4e588,0x1c573e8};
        for(unsigned i=0;i<4;++i){std::uintptr_t component=0;
            if(!read(list+i*8,&component,8)||!component||!read(component,&vt,8)||!read(component+0x10,&owner,8)||owner!=s.actor)return fail(__LINE__);
            unsigned j=0;while(j<4&&vt!=base+expected[j])++j;if(j==4||(seen&(1u<<j)))return fail(__LINE__);seen|=1u<<j;
        }if(seen!=15)return fail(__LINE__);
    }
'''
assert s.count(anchor)==1;s=s.replace(anchor,guard+anchor)
anchor='    if(cosmetic&&(s.animation!=prop.animation'
assert s.count(anchor)==1
s=s.replace(anchor,'    if(rat&&(std::strcmp(s.modelName,"models/characters/animals/rat/rat.md6")||std::strcmp(s.skeletonName,"models/characters/animals/rat/rat_skeleton.md6skl")||(jointCount&0xffffff)>128))return fail(__LINE__);\n'+anchor)
p.write_text(s)
p=S/'runtime_probe.cpp';s=p.read_text(encoding='utf-8-sig')
s=s.replace('&&std::strcmp(argv[2],"--skeleton")&&','&&std::strcmp(argv[2],"--skeleton")&&std::strcmp(argv[2],"--rat-skeleton")&&')
s=s.replace('std::strcmp(argv[2],"--skeleton")==0||','std::strcmp(argv[2],"--skeleton")==0||std::strcmp(argv[2],"--rat-skeleton")==0||')
s=s.replace(')),s,std::strcmp(argv[2],"--weapon-skeleton")', ')),s,std::strcmp(argv[2],"--rat-skeleton")==0?NativeSkeletonScope::CosmeticRat:std::strcmp(argv[2],"--weapon-skeleton")')
assert 'NativeSkeletonScope::CosmeticRat' in s;p.write_text(s)
p=R/'work/natural-moveset/Test-SwarmRatVisualNetwork-v291.ps1';s=p.read_text(encoding='utf-8-sig')
anchor=" $rats|ConvertTo-Json -Depth 8|Set-Content \"$out/visual-entities.json\""
assert s.count(anchor)==1
s=s.replace(anchor,anchor+'''
 if($rats.Count-eq1){
  Q client '--object' $rats[0].pointer|ConvertTo-Json -Depth 12|Set-Content "$out/visual-entity-object.json"
  & "$lab/bin/d2_runtime_probe.exe" $pids.client '--rat-skeleton' $visual.handle > "$out/visual-skeleton-0.json"
  Start-Sleep -Milliseconds 120
  & "$lab/bin/d2_runtime_probe.exe" $pids.client '--rat-skeleton' $visual.handle > "$out/visual-skeleton-1.json"
 }
''')
# Turn slightly toward the source's ground swarm rather than toward Corvo.
s=s.replace('68.1 203 0','68.1 180 0').replace('-Role client -Pitch 23','-Role client -Pitch 26')
p=R/'work/natural-moveset/Test-SwarmRatVisualNetwork-v292.ps1';assert not p.exists();p.write_text(s)
p=R/'work/movement-integration/Invoke-NativeLabOperation.ps1';s=p.read_text(encoding='utf-8-sig').replace('Test-SwarmRatVisualNetwork-v291.ps1','Test-SwarmRatVisualNetwork-v292.ps1');p.write_text(s)
print('Read-only rat skeleton diagnostic added; gameplay DLL unchanged')
