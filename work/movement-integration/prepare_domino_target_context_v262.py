from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration'
s=(P/'Test-DominoContextNetwork-v258.ps1').read_text(encoding='utf-8-sig')
t=(P/'Test-DominoCrouchFinalNetwork-v256.ps1').read_text(encoding='utf-8-sig')
a=t.index(' $dirty=$true\n # Suspending AI');b=t.index(" $before=Q client '--player-domino'",a)
placement=t[a:b]
a=s.index(" $dirty=$true;C client 'setviewpos");b=s.index(" $before=Q client '--player-domino'",a);s=s[:a]+placement+s[b:]
s=s.replace('[double]$grounded.position[2]-66.4370575','[double]$grounded.position[2]-[double]$anchor[2]')
s=s.replace('-Pitch 12 -Evidence "$out/holster-look-restored.json"','-Pitch 0 -Evidence "$out/holster-look-restored.json"')
s=s.replace(" $phases=@(19,20,22);if(@($scenes|Where-Object variant -eq 21).Count){$phases=@(19,20,21,22)}", " $phases=@(19,20,21,22)")
s=s.replace(" # Native target selection is allowed; existing scoped cosmetic replication is recorded separately.", " if(!$nativeMarks.Count){throw 'Targeted context did not produce native guard mark'}")
s=s.replace('It permits native target selection and records its effects.', 'This targeted take requires the ordinary selection phase and native mark; it places the source on a verified floor near a settled guard and holsters while looking upward.')
(P/'Test-DominoTargetContextNetwork-v262.ps1').write_text(s,encoding='utf-8')
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text(encoding='utf-8-sig').replace("'DominoObserve','DominoContext'","'DominoObserve','DominoContextTarget','DominoContext'")
s=s.replace("  'DominoContext'{", "  'DominoContextTarget'{& \"$root/work/natural-moveset/Test-DominoTargetContextNetwork-v262.ps1\" -Take $Take -Pitch $Pitch -Stance $Stance -Holstered:$Holstered -Move $Move}\n  'DominoContext'{");p.write_text(s,encoding='utf-8')
print('Targeted stance/equipment context now requires all four native phases and a source mark, with protected holster look-away.')
