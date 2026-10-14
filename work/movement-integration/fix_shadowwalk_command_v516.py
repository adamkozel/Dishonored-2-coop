from pathlib import Path
import shutil
R=Path(__file__).resolve().parents[2]
p=R/'outputs/d2-coop-lab/Send-D2Command.ps1'
s=p.read_text(encoding='utf-8-sig')
assert 'coop_shadowwalk_fixture' not in s
backup=R/'work/movement-integration/candidate-shadowwalk-inventory-v515/Send-D2Command-before-shadowwalk.ps1'
assert not backup.exists();shutil.copy2(p,backup)
s=s.replace('|coop_doppelganger_fixture|','|coop_doppelganger_fixture|coop_shadowwalk_fixture|')
line="if($Command -like 'coop_doppelganger_fixture*' -and $Command -notmatch '^coop_doppelganger_fixture (20000|40000|off)$'){throw 'Doppelganger fixture accepts only finite research durations or off.'}"
assert s.count(line)==1
s=s.replace(line,line+'\n'+line.replace('doppelganger','shadowwalk').replace('Doppelganger','Shadow Walk'))
p.write_text(s,encoding='utf-8')
shutil.copy2(p,backup.with_name('Send-D2Command.ps1'))
print('Added only the finite Shadow Walk research command to the client wrapper; prior wrapper archived.')
