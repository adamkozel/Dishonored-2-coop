"""Read-only structured extraction of exact-game midnight timing metadata."""
import re,json,hashlib
from pathlib import Path
P=Path(__file__).resolve().parent
def parse(text):
    tokens=re.findall(r'//[^\n]*|"(?:\\.|[^"\\])*"|[A-Za-z_][A-Za-z_0-9]*(?:\[\d+\])?|[-+]?\d+(?:\.\d+)?(?:[eE][-+]?\d+)?|[{}=;]',text);tokens=[t for t in tokens if not t.startswith('//')];i=0
    def value():
        nonlocal i
        token=tokens[i];i+=1
        if token=='{':
            out={}
            while tokens[i]!='}':
                key=tokens[i];i+=1;assert tokens[i]=='=',(key,tokens[i]);i+=1;assert key not in out,key;out[key]=value()
                if tokens[i]==';':i+=1
            i+=1;return out
        if token.startswith('"'):return json.loads(token)
        if re.fullmatch(r'[-+]?\d+(?:\.\d+)?(?:[eE][-+]?\d+)?',token):return float(token)if any(c in token for c in '.eE')else int(token)
        return token
    result=value();assert i==len(tokens),(i,len(tokens));return result
def walk(d):
    if not isinstance(d,dict):return
    yield d
    for v in d.values():yield from walk(v)
def main():
    manifest=json.loads((P.parent.parent/'outputs/d2-coop-lab/research/asset-exports/natural-drop-pair-scenes/manifest.json').read_text(encoding='utf-8-sig'));out=[]
    for r in manifest['Resources']:
        if not r['Name'].endswith(('choke_drop_back.midnightscene','assassination_drop_back.midnightscene')):continue
        path=Path(r['ExportFile']);d=parse(path.read_text());rows=[];mapping={};info=d['edit']['m_groupsInfo'];groups={v['m_UID']['m_objUID']:v['m_name']for k,v in info.items()if k.startswith('item[')}
        for obj in walk(d):
            if 'm_md6'in obj and 'm_nextInfo'in obj:mapping[obj['m_nextInfo']['m_nextCPoint']['sceneUID']]=obj['m_md6']
            if 'm_keyProp'not in obj:continue
            prop=obj['m_keyProp'];cls=prop.get('m_class','');body=prop.get('m_object',{});h=obj['m_handle']
            if groups.get(h.get('groupUID'),'').startswith('AI') and ('StartRagdoll'in cls or cls=='arkMidnightKeyPropAnim'):
                rows.append(dict(scene=h['sceneUID'],group=h['groupUID'],group_name=groups[h['groupUID']],key=h['keyUID'],class_name=cls,start_ms=obj['m_startTime'],duration_ms=obj['m_duration'],properties=body))
        for row in rows:row['player_body']=mapping.get(row['scene'])
        out.append(dict(asset=r['Name'],sha256=hashlib.sha256(path.read_bytes()).hexdigest(),rows=rows))
    (P/'pair-research/native-drop-event-times.json').write_text(json.dumps(out,indent=2))
    for r in out:
        print(r['asset'])
        for row in r['rows']:print(row['player_body'],row['class_name'],row['start_ms'],row['duration_ms'],row['properties'])

if __name__=="__main__":main()
