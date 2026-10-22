param([switch]$Force)
# 1784 (#159, #160): lab fixtures for the new pairings. Each lab instance keeps one slot per mission (SLOT81-89): the
# host's are Corvo saves, the client's Emily saves. SLOT80 (an unused slot) gets the other character at the Dunwall
# fixture: the client receives the host's SLOT81 (Corvo) and the host the client's SLOT81 (Emily), so a lab instance
# loads its swapped character with LoadGame 80 at the place the existing harnesses use (the tutorial guards). Lab
# instance data only (never the original saves or the protected files); SLOT81 stays the restore fixture. Existing
# SLOT80 folders are kept unless -Force.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$labs=Join-Path $root 'work/lab-instances'
foreach($pair in @(@('host','client'),@('client','host'))){
 $src=Join-Path $labs "$($pair[0])/base/savegame/SLOT81";$dst=Join-Path $labs "$($pair[1])/base/savegame/SLOT80"
 if((Test-Path $dst)-and!$Force){"$($pair[1]) SLOT80 kept";continue}
 if(Test-Path $dst){Remove-Item -LiteralPath $dst -Recurse -Force}
 Copy-Item -LiteralPath $src -Destination $dst -Recurse
 $character=((Get-Content (Join-Path $dst 'game.details'))-match'^character=')-replace'^character=',''
 "$($pair[1]) SLOT80 <- $($pair[0]) SLOT81 (character $character)"
}
