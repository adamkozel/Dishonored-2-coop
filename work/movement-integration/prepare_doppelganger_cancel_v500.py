from pathlib import Path
R=Path(__file__).resolve().parents[2]; W=R/'work/movement-integration'; N=R/'work/natural-moveset'
read=lambda p:p.read_text(encoding='utf-8-sig')
def new(p,s):
    assert not p.exists(),p
    p.write_text(s,encoding='utf-8')
s=read(N/'Observe-NativeDoppelgangerCancel-v499.ps1')
s=s.replace('-Button Neutral -LeftTrigger 255 -RightTrigger 255 -DurationMs 250','-Button LeftThumb -LeftTrigger 255 -DurationMs 250')
s=s.replace('Native RT cancel while LT remains held','Native LeftThumb cancel candidate while LT remains held')
s=s.replace('Native RT cancellation input observation','Native LeftThumb cancellation input observation')
s=s.replace('Where-Object id -eq63','Where-Object id -eq 63')
needle=" $clones=@($rows.entities|Where-Object {$_.definitionName-match'^models/powers/doppelganger/(decoy_entity|deadlyshadow_entity)(|_corvo)\\.def$'})"
assert s.count(needle)==1
s=s.replace(needle,needle+'''
 $cloneScenes=@($samples.instances|Where-Object {$_.state-eq3-and$_.asset-match'^midnight/gameplay/human/common/doppelganger/doppelganger_(spawn|dispawn|death_fx)\\.midnightscene$'})
 $counterRows=@($rows|ForEach-Object {$word=@($_.component.words|Where-Object offset -eq '0x60');if($word.Count-ne1){throw 'Native power base counter snapshot absent'};[pscustomobject]@{videoTime=$_.videoTime;counter=([Convert]::ToUInt64($word[0].word,16)-shr32)}})
 $counterBefore=$counterRows[0].counter;$counterAfter=$counterRows[-1].counter
 [ordered]@{cloneSceneSamples=$cloneScenes.Count;cloneScenes=@($cloneScenes|Group-Object asset|ForEach-Object {[ordered]@{asset=$_.Name;samples=$_.Count;ids=@($_.Group.id|Sort-Object -Unique)}});baseCounterBefore=$counterBefore;baseCounterAfter=$counterAfter;counterSamples=$counterRows;scope='Native base function61bea0 increments component+64 on power activation. Source499 wrongly treated late clone absence as cancellation despite observed spawn/death scenes and an increment. This stronger check requires neither.'}|ConvertTo-Json -Depth 10|Set-Content "$out/no-spawn-proof.json"
''')
s=s.replace("if($clones.Count){throw 'A clone was spawned despite the attempted native cancellation'}", "if($clones.Count-or$cloneScenes.Count-or@($counterRows|Where-Object {$_.counter-ne$counterBefore}).Count){throw 'Native power activated or clone scene ran despite attempted cancellation'}")
s=s.replace('Native RT cancels aimed Doppelganger before a native clone is enumerated; original stance/equipment recover and actual exit scene evidence is retained',
            'Native LeftThumb cancels aimed Doppelganger: no native spawn/death scenes, no base activation counter increment, original stance/equipment recover; exact exit scene evidence retained')
new(N/'Observe-NativeDoppelgangerCancel-v500.ps1',s)
w=read(W/'Run-DoppelgangerCancelObserve-v499.ps1').replace('v499','v500')
new(W/'Run-DoppelgangerCancelObserve-v500.ps1',w)
print('Prepared LeftThumb candidate with full scene/activation proof; retained failed RT take499 unchanged.')
