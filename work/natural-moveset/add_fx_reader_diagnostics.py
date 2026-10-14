from pathlib import Path
P=Path(__file__).resolve().parents[2]/'outputs/d2-coop-lab/src'
p=P/'native_windblast_fx_records.h';s=p.read_text()
a=s.index('template<class Read,class Filter>');b=s.index('\ntemplate<class Read> bool readNativeWindblastFxRecords',a)
x=s[a:b]
assert 'failureLine' not in x
x=x.replace('Filter wanted){','Filter wanted,unsigned* failureLine=nullptr){\n    if(failureLine)*failureLine=0;const auto fail=[&](unsigned line){if(failureLine)*failureLine=line;return false;};')
start=x.index('    out.clear();')
x=x[:start]+x[start:].replace('return false;', 'return fail(__LINE__);')
s=s[:a]+x+s[b:]
s=s.replace('std::vector<NativeWindblastFxRecord>& out){','std::vector<NativeWindblastFxRecord>& out,unsigned* failureLine=nullptr){')
s=s.replace('out,nativeWindblastFxName);','out,nativeWindblastFxName,failureLine);').replace('out,nativeRangedFxName);','out,nativeRangedFxName,failureLine);')
p.write_text(s)
p=P/'runtime_probe.cpp';s=p.read_text()
s=s.replace('std::uintptr_t game=0;std::vector<NativeWindblastFxRecord> records;','std::uintptr_t game=0;std::vector<NativeWindblastFxRecord> records;unsigned failureLine=0;')
old='readNativeRangedFxRecords(reader,base,game,records):readNativeWindblastFxRecords(reader,base,game,records)))return 69;'
assert old in s
s=s.replace(old,'readNativeRangedFxRecords(reader,base,game,records,&failureLine):readNativeWindblastFxRecords(reader,base,game,records,&failureLine))){std::cout<<"{\\\"readOnly\\\":true,\\\"stable\\\":false,\\\"failureLine\\\":"<<failureLine<<"}\\n";return 69;}')
p.write_text(s)
