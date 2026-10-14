from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration'
s=(P/'Observe-PossessionFx-v348.ps1').read_text()
s=s.replace('@(@(-3.0,0.0),@(0.0,-3.0),@(3.0,0.0),@(0.0,3.0))','@(@(0.0,-3.0),@(-3.0,0.0),@(3.0,0.0),@(0.0,3.0))',1)
old='[Math]::Abs([double]$ground.position[2]-66.4)-lt.1){$floorFound=$true;break}'
assert s.count(old)==1
s=s.replace(old,'[Math]::Abs([double]$ground.position[2]-66.4)-lt.1-and[Math]::Abs([double]$ground.position[0]-$sourceX)-lt.08-and[Math]::Abs([double]$ground.position[1]-$sourceY)-lt.08){$floorFound=$true;break}')
(P/'Observe-PossessionFx-v349.ps1').write_text(s,encoding='utf-8')
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text().replace('Observe-PossessionFx-v348.ps1','Observe-PossessionFx-v349.ps1');p.write_text(s,encoding='utf-8')
print('349 rejects wall-displaced source floors in addition to grounded height; prioritizes proven south approach')
