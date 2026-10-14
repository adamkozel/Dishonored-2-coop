"""Refresh the dated, source-backed audit handoff without touching game data.

Only documentation/handoff-2026-10-11/generated is written. Large native assets,
saved games, credentials, vendor trees and live Ghidra databases are not copied.
"""
import collections,datetime,hashlib,json,os,re,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent.parent
D=P/'documentation/handoff-2026-10-11';G=D/'generated'
STAMP=datetime.datetime.now().astimezone().isoformat()
G.mkdir(parents=True,exist_ok=True)
manifest={};issues=[];generated=[]
def rel(p):return Path(p).relative_to(P).as_posix()
def source(p):
 p=Path(p);b=p.read_bytes();manifest[rel(p)]={'bytes':len(b),'sha256':hashlib.sha256(b).hexdigest().upper()};return b.decode('utf-8-sig',errors='replace')
def data(p):return json.loads(source(p))
def link(p,fromfile,label=None):
 p=Path(p);return '['+(label or p.name).replace('[','').replace(']','')+'](<'+os.path.relpath(p,Path(fromfile).parent).replace('\\','/')+'>)'
def write(name,body):
 p=G/name;p.parent.mkdir(parents=True,exist_ok=True);p.write_text(body,encoding='utf-8');generated.append(p);return p
def block(v,lang='json'):
 s=v if isinstance(v,str)else json.dumps(v,ensure_ascii=False,indent=2)
 fence='`'*max(4,max((len(x)for x in re.findall(r'`+',s)),default=0)+1)
 return fence+lang+'\n'+s+'\n'+fence+'\n'
def meta(p,f):
 t=source(p);return f'Original: {link(p,f,rel(p))}\n\nSHA-256: `{manifest[rel(p)]["sha256"]}`; {manifest[rel(p)]["bytes"]} bytes.\n\n',t
def slug(s):return re.sub('[^a-z0-9]+','-',s.lower()).strip('-')
SKIP={'node_modules','.venv','__pycache__','project','snapshots','input','Intermediate','Saved','Binaries','build','Engine','Content','.git'}
def walk(root,extras=()):
 for dp,ds,fs in os.walk(root,followlinks=False):
  ds[:]=[d for d in ds if d not in SKIP and d not in extras and not (Path(dp)/d).is_symlink() and not (Path(dp)/d).is_junction()]
  for n in fs:yield Path(dp)/n

# Current state: retain historical sections intact; label their attribution.
statePath=P/'work/current-task-state.json';state=data(statePath)
stateindex=['# Historical and current state records\n',f'Snapshot: {STAMP}. Source fields may be stale; preserve their build attribution.\n']
for k,v in state.items():
 f=G/'state'/f'{slug(k)}.md';body=f'# State record: {k}\n\nOriginal section of {link(statePath,f)}. Captured {STAMP}.\n\nThis is a verbatim structured record, not a new validation. Earlier fields named Current/Live can describe historical builds.\n\n'+block(v)
 write('state/'+f.name,body);stateindex.append('- '+link(f,G/'state/README.md',k))
write('state/README.md','\n'.join(stateindex)+'\n')

# Full requirement list and acceptance contract.
catalogPath=P/'work/natural-moveset/review-catalog.json';catalog=data(catalogPath)
contract=data(P/'work/natural-moveset/movement-phase-contract.json')
req=G/'movement-requirements.md'
lines=['# Complete functional movement requirements\n',f'Captured {STAMP}; source {link(catalogPath,req)}.\n',f'{len(catalog["items"])} requirements. Candidate material and recordings do not establish completion.\n','## Acceptance contract\n',block(contract),'## Requirement index\n','| ID | Family | Authoring | Runtime binding | Visual status |','|---|---|---|---|---|']
families=collections.defaultdict(list)
for item in catalog['items']:
 families[item['family']].append(item)
 lines.append('| '+ ' | '.join(str(item.get(k,''))for k in ['id','family','authoring','runtime_binding','visual_validation'])+' |')
lines+=['\n## Full per-family records\n']
for family,items in families.items():
 f=G/'requirements'/f'{family}.md';lines.append('- '+link(f,req,f'{family}: {len(items)} requirements'))
 content=[f'# {family}: complete requirement records\n',f'Source {link(catalogPath,f)}; captured {STAMP}. All fields below retain the catalog wording.\n']
 for item in items:
  content+=[f'## {item["id"]}\n',str(item.get('notes',''))+'\n',block(item)]
  for key in ['evidence','in_game_recordings']:
   for x in item.get(key,[])if isinstance(item.get(key,[]),list)else []:
    if isinstance(x,str):
     p=P/'work/natural-moveset'/x
     if p.exists():content.append('- '+link(p,f,key+': '+x))
 content.append('\n');write('requirements/'+f.name,'\n'.join(content))
write(req.relative_to(G),'\n'.join(lines)+'\n')
motionfile=G/'motion-candidates.md';lines=['# All motion candidates and role data\n','These are source candidates, not accepted coverage.\n']
for motion in catalog['motions']:
 lines.extend([f'## {motion.get("id","unnamed")}\n',block(motion)])
write(motionfile.relative_to(G),'\n'.join(lines))

# Runtime videos: exact retained provenance and actual local file link.
vp=P/'work/natural-moveset/runtime-video-manifest.json';videos=data(vp);vf=G/'runtime-recordings.md'
lines=['# Actual game recording inventory\n',f'{len(videos)} indexed entries; source {link(vp,vf)}; captured {STAMP}.\n','Each row retains its own build, capture rate, evidence scope and acceptance state. Historical clips can be pose-only; do not infer gameplay binding.\n']
for i,v in enumerate(videos,1):
 p=P/'work/natural-moveset'/v['video'];lines+=[f'## {i}. {v.get("label",v["video"])}\n',link(p,vf,v['video'])+'\n',block(v)]
 if not p.exists():issues.append({'kind':'missing-video','path':rel(p)})
write(vf.relative_to(G),'\n'.join(lines))

# Project Ghidra notes and seed structures.
bp=P/'work/handoff-project-bookmarks-current.json';bookmarks=data(bp);bf=G/'ghidra-bookmarks.md'
lines=['# Saved Ghidra project bookmarks\n',f'{len(bookmarks)} D2/co-op bookmarks exported from the exact active program. Source {link(bp,bf)}.\n','Original categories and comments are preserved. A historical category is not automatic proof of current completeness.\n']
for b in bookmarks:
 a=b['address'];rva=hex(int(a,16)-0x140000000)
 lines += [f'## {a} / RVA {rva} — {b["category"]}\n',b['comment']+'\n']
write(bf.relative_to(G),'\n'.join(lines))
seed=P/'outputs/d2-coop-lab/analysis/ghidra/native-findings.json';f=G/'native-seed.md';s=data(seed)
write(f.relative_to(G),'# Seed functions, labels and partial structures\n\n'+link(seed,f)+'\n\nNames are analyst labels; structure sizes are minimum known views. This seed predates later bookmarks.\n\n'+block(s))
fp=P/'work/handoff-function-documentation.json';f=G/'function-documentation.md'
if fp.exists():
 values=data(fp);lines=['# Current documentation at bookmarked function addresses\n',f'Source {link(fp,f)}. Metadata export does not imply correct complete prototypes or whole-program decompilation.\n']
 for item in values:
  lines += [f'## {item["address"]}\n']
  if item.get('status')!='fulfilled':lines.append(block(item));issues.append({'kind':'function-export-failure','address':item['address']});continue
  result=item['value'];payload=result.get('structuredContent',{}).get('result')
  if payload is None:payload='\n'.join(c.get('text','')for c in result.get('content',[])if c.get('type')=='text')
  try:payload=json.loads(payload)
  except (ValueError,TypeError):pass
  lines.append(block(payload))
  if result.get('isError')or(isinstance(payload,dict)and(payload.get('success')is False or 'error'in payload)):issues.append({'kind':'function-export-backend-error','address':item['address']})
 write(f.relative_to(G),'\n'.join(lines))
else:issues.append({'kind':'pending-function-documentation-export'})
lp=P/'work/handoff-live-ghidra-verification.json';f=G/'live-ghidra-verification.md'
write(f.relative_to(G),'# Actual MCP lookup, xrefs and decompilation\n\nFresh exact-program calls for `140734d90`, whose native release behavior underlies v27.\n\n'+link(lp,f)+'\n\n'+block(data(lp)))

# Discover saved decompilation/finding text from all project-authored work areas.
roots=[P/'outputs/d2-coop-lab/analysis',P/'outputs/d2-coop-lab/research']
excluded={'asset-game','lab-game','lab-instances','tools','build','GameNetworkingSockets','Dishonored2-Debug','extracted','asset-install-stage'}
roots += [d for d in (P/'work').iterdir()if d.is_dir()and d.name not in excluded and not d.is_junction()and not d.is_symlink()]
paths=set()
# Root-level targeted Ghidra exports are project findings too. The directory
# walk below deliberately avoids the game clones and does not visit work/ itself.
for p in (P/'work').iterdir():
 if p.is_file() and p.suffix.lower()in{'.json','.txt','.asm','.md','.log'} and re.search(r'decomp|ghidra|finding|disasm|disassembl',p.name,re.I):
  if p.stat().st_size>5_000_000:issues.append({'kind':'large-finding-linked-only','path':rel(p)})
  else:paths.add(p)
for root in roots:
 for p in walk(root,extras={'src','bin','exports','frames','screens'}):
  # Targeted native investigations often name transcripts by function address
  # (e.g. load-manager-140659190.txt), without "decomp" in the filename.
  finding=re.search(r'decomp|ghidra|finding|disasm|disassembl',rel(p),re.I) or re.search(r'(?:^|/)[^/]*-research-v\d+(?:/|$)',rel(p),re.I)
  if p.suffix.lower()in{'.json','.txt','.asm','.md','.log'} and finding:
   if p.name in {'mcp-tools.json','codex-mcp-configuration.json'}:continue
   if p.stat().st_size>5_000_000:issues.append({'kind':'large-finding-linked-only','path':rel(p)});continue
   paths.add(p)
df=G/'decompilation/README.md';lines=['# Saved decompilation and finding transcripts\n','Full source text is preserved in Markdown, with original path and digest. Some records are failed calls, administrative setup or historical duplicates. They remain labelled by their original content. They are not newly confirmed findings.\n']
for i,p in enumerate(sorted(paths),1):
 name=f'{i:03d}-{slug(p.stem)[:100]}-{hashlib.sha256(rel(p).encode()).hexdigest()[:7]}.md';f=G/'decompilation'/name;head,t=meta(p,f)
 body=f'# {p.name}\n\n'+head+'## Original saved record\n\n'+block(t,'json'if p.suffix=='.json'else'text')
 # MCP JSON often nests the decompilation in a string: add an unescaped view.
 if p.suffix=='.json':
  try:
   obj=json.loads(t);strings=[]
   def collect(x):
    if isinstance(x,dict):
     for k,v in x.items():
      if k in {'text','decompilation','decompiled','pseudocode','code','result'}and isinstance(v,str)and len(v)>150:strings.append((k,v))
      elif isinstance(v,(dict,list)):collect(v)
    elif isinstance(x,list):
     for v in x:collect(v)
   collect(obj)
   unique={}
   for k,v in strings:unique.setdefault(v,k)
   if unique:body+='\n## Unescaped text fields for reading\n\n'+ '\n'.join('### '+k+'\n\n'+block(v,'text')for v,k in unique.items())
  except ValueError:pass
 write('decompilation/'+name,body);lines.append('- '+link(f,df,rel(p)))
write(df.relative_to(G),'\n'.join(lines)+'\n')

# Original narrative notes including preserved previous milestones.
notes=set()
for root in [P/'outputs/d2-coop-lab',P/'sites/d2-animation-review']+roots[2:]:
 for p in walk(root,extras={'src','bin','third_party','exports','frames','screens'}):
  if p.suffix.lower()=='.md'or(p.name.lower()in{'readme.txt','process.txt','notes.txt'}):
   if p.stat().st_size<2_000_000:notes.add(p)
hf=G/'historical-notes.md';lines=['# Original historical narratives\n','These are historical documents reproduced verbatim inside fenced Markdown/text blocks. Their original relative links remain text; use the source link to navigate. “Current” in a historical note is not a claim about this handoff.\n']
for i,p in enumerate(sorted(notes),1):
 f=G/'history'/f'{i:03d}-{slug(p.parent.name)}-{slug(p.stem)}.md';head,t=meta(p,f)
 write(f.relative_to(G),f'# Historical note: {rel(p)}\n\n'+head+block(t,'markdown'if p.suffix=='.md'else'text'))
 lines.append('- '+link(f,hf,rel(p)))
write(hf.relative_to(G),'\n'.join(lines)+'\n')

# Current source map, tests and line-linked address contexts.
sf=G/'source-map.md';af=G/'source-addresses.md';src=P/'outputs/d2-coop-lab/src'
sl=['# Current source module index\n','Source digests are in source-manifest.json. Read the code and per-build archive before attributing behavior.\n','| Source | Lines | Leading purpose comment |','|---|---:|---|']
al=['# Address-like constants in current source\n','Search aid only. This includes native RVAs, data/vtables, masks and possible false positives. A source occurrence is not a confirmed API. Line numbers and nearby text refer to this snapshot.\n']
for p in sorted(src.glob('*')):
 if not p.is_file()or p.suffix not in{'.h','.cpp','.c','.def'}:continue
 t=source(p);ls=t.splitlines();comments=[x.strip().removeprefix('//').strip()for x in ls[:35]if x.strip().startswith('//')]
 sl.append('| '+link(p,sf,p.name)+' | '+str(len(ls))+' | '+' '.join(comments)[:250].replace('|','\\|')+' |')
 matches=[(i,l)for i,l in enumerate(ls,1)if re.search(r'0x[0-9a-fA-F]{5,16}\b|\b14[01][0-9a-fA-F]{6}\b',l)]
 if matches:
  al+=[f'## {p.name}\n',link(p,af)+'\n']
  for i,l in matches:
   context='\n'.join(f'{j+1}: {ls[j]}'for j in range(max(0,i-3),min(len(ls),i+1)))
   al.append(block(context,'cpp'))
write(sf.relative_to(G),'\n'.join(sl)+'\n');write(af.relative_to(G),'\n'.join(al))

# Index all local evidence and harness files without loading giant telemetry.
ef=G/'evidence-index.md';tf=G/'test-and-tool-scripts.md';ev=[];scripts=[]
auditroots=[P/'outputs/d2-coop-lab/research',P/'outputs/d2-coop-lab/analysis',P/'work/natural-moveset',P/'work/movement-integration',P/'work/animgen-feasibility',P/'work/fullbody-proof',P/'work/mixamo-crouch',P/'work/moveset-growth']
for root in auditroots:
 for p in walk(root,extras={'src','bin','exports','frames','screens','native-retargeted','mixamo-retargeted'}):
  if p.suffix.lower()in{'.json','.md','.txt','.log','.ps1','.py','.cjs'}:ev.append(p)
  if p.suffix.lower()in{'.ps1','.py','.cjs'}:scripts.append(p)
scripts+=list((P/'outputs/d2-coop-lab').glob('*.ps1'))+list((P/'outputs/d2-coop-lab/tooling').glob('*'))+list((P/'work').glob('*.ps1'))
lines=['# Evidence file index\n',f'{len(set(ev))} files indexed by path/size without loading large telemetry. Not every file is a passing result. Whole-game indices and binary assets are excluded.\n','| File | Bytes |','|---|---:|']
for p in sorted(set(ev)):lines.append('| '+link(p,ef,rel(p))+' | '+str(p.stat().st_size)+' |')
write(ef.relative_to(G),'\n'.join(lines)+'\n')
lines=['# Test harnesses and project tools\n','Scripts are recorded, not automatically executed. Native mutation and reset tools must be serialized with captures. Read parameters and cleanup before use.\n']
for p in sorted(set(scripts)):
 if not p.is_file()or p.suffix.lower()not in{'.ps1','.py','.cjs','.js'}:continue
 t=source(p);lines.extend([f'## {rel(p)}\n',link(p,tf)+'\n',block('\n'.join(t.splitlines()[:20]),'text')])
write(tf.relative_to(G),'\n'.join(lines))

# Milestone directories and structured reports; preserve original aggregate values.
mf=G/'milestones.md';lines=['# Milestones, source archives and report summaries\n','A directory or passing historical report does not imply current integration. Inspect exact binary/source identities before restoration.\n']
milestoneroot=P/'outputs/d2-coop-lab/research/milestones'
for d in sorted(milestoneroot.glob('*')):
 if not d.is_dir():continue
 lines+=[f'## Research milestone {d.name}\n',link(d,mf)+'\n']
 for p in sorted(d.glob('*summary.json')):
  lines+=[link(p,mf)+'\n',block(data(p))]
for d in sorted((P/'work').iterdir()):
 if d.is_dir()and re.search('verified|native-|doppelganger|checkpoint|friendly-fire|shared-recovery|pre-|sword-|combat-',d.name)and d.name not in excluded:
  lines+=['\n## '+d.name+'\n',link(d,mf)+'\n']
  for p in sorted(d.glob('*')):
   if p.is_file()and p.suffix in{'.json','.md','.txt'}and re.search('identity|manifest|readme|summary|scope',p.name,re.I):lines.append('- '+link(p,mf))
for d in sorted((P/'work/movement-integration').iterdir()):
 if not d.is_dir():continue
 lines+=['\n## Movement archive '+d.name+'\n',link(d,mf)+'\n']
 p=d/'identity.json'
 if p.exists():lines.append(block(data(p)))
write(mf.relative_to(G),'\n'.join(lines)+'\n')

# Inspect currently resolvable utility versions, no installation or global changes.
versions={}
for cmd in [['ffmpeg','-version'],['ffprobe','-version'],['node','--version'],['rg','--version']]:
 try:
  r=subprocess.run(cmd,capture_output=True,text=True,timeout=12);versions[cmd[0]]={'exit':r.returncode,'output':(r.stdout+r.stderr).splitlines()[:3]}
 except Exception as e:versions[cmd[0]]={'unavailable':type(e).__name__}
write('observed-utility-versions.md','# Utility versions observed during documentation\n\n'+block(versions))

# Generated entry point and counts.
summary={'generatedAt':STAMP,'movementRequirements':len(catalog['items']),'motionCandidates':len(catalog['motions']),'actualGameRecordings':len(videos),'ghidraProjectBookmarks':len(bookmarks),'savedDecompilationTranscripts':len(paths),'historicalNarratives':len(notes),'indexedEvidenceFiles':len(set(ev)),'completeCoop':False,'completeMovement':False}
entries=[('movement-requirements.md','Complete moveset checklist and acceptance contract'),('motion-candidates.md','All candidate motions and per-role source data'),('runtime-recordings.md','All indexed actual game recordings'),('state/README.md','All preserved state sections'),('ghidra-bookmarks.md','Saved Ghidra project findings'),('native-seed.md','Named functions, labels and partial structures'),('decompilation/README.md','All discovered saved analysis transcripts'),('function-documentation.md','Current bookmarked function metadata'),('live-ghidra-verification.md','Fresh actual MCP verification'),('source-map.md','Current implementation modules'),('source-addresses.md','Native-address source contexts'),('historical-notes.md','Original narratives and README history'),('milestones.md','Preserved build/milestone summaries'),('evidence-index.md','Detailed evidence file paths'),('test-and-tool-scripts.md','Test and tool scripts with parameter/source beginnings'),('observed-utility-versions.md','Current utility version observations'),('source-manifest.json','SHA-256 of every source read by this generator'),('generation-audit.md','Snapshot/link audit')]
write('README.md','# Generated handoff evidence\n\n'+block(summary)+'\n'+'\n'.join('- '+link(G/n,G/'README.md',label)for n,label in entries)+'\n')
write('source-manifest.json',json.dumps({'generatedAt':STAMP,'sources':manifest},indent=2))

# Remove only orphaned pages created by earlier runs of this generator. Source
# records remain untouched. Resolve every candidate within the managed subtree.
active={p.resolve()for p in generated};removed=[]
for sub in ['decompilation','state','history','requirements']:
 managed=(G/sub).resolve()
 for old in (G/sub).glob('*.md'):
  resolved=old.resolve()
  if resolved not in active:
   if not resolved.is_relative_to(managed)or old.is_symlink():raise RuntimeError('Unsafe generated-page cleanup target')
   removed.append(str(old.relative_to(G)));old.unlink()

# Check actual Markdown links outside fenced source transcripts.
missing=[];checked=0
for f in D.rglob('*.md'):
 text=f.read_text(encoding='utf-8');clean=[];fence=None
 for line in text.splitlines():
  m=re.match(r'^\s*(`{3,}|~{3,})',line)
  if m:
   if fence is None:fence=m.group(1)
   elif m.group(1)[0]==fence[0]and len(m.group(1))>=len(fence):fence=None
   continue
  if fence is None:clean.append(line)
 for target in re.findall(r'\]\((<[^>]+>|[^)]+)\)','\n'.join(clean)):
  target=target.strip('<>')
  if target.startswith(('http://','https://','app://','#','mailto:')):continue
  target=target.split('#')[0]
  if not target:continue
  p=Path(target)if re.match(r'^[A-Za-z]:[/\\]',target)else(f.parent/target)
  checked+=1
  if not p.exists():missing.append({'document':str(f.relative_to(D)).replace('\\','/'),'target':target})
audit={'generatedAt':STAMP,'localLinksChecked':checked,'missingLocalLinks':missing,'issues':issues,'orphanedGeneratedPagesRemoved':removed,'sourcesHashed':len(manifest),'generatedMarkdownFiles':len(list(G.rglob('*.md'))),'scope':'Project-authored saved artifacts; vendor installations, credentials, game assets, saves and giant raw telemetry not copied; unsaved history not reconstructable.'}
write('generation-audit.json',json.dumps(audit,indent=2));write('generation-audit.md','# Generation and local-link audit\n\n'+block(audit))
print(json.dumps({**summary,'sourcesHashed':len(manifest),'markdownFiles':len(list(D.rglob('*.md'))),'missingLocalLinks':len(missing),'issues':len(issues)},indent=2))
