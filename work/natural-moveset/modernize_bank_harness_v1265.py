"""Derives a current-build copy of an older isolated-mode harness (visual sweep 1260 re-records): the partner bank becomes the
current version-71 bank (use-banks-v1212/<character>-choke-bank.bin) instead of the harness-era manifest bank, and every
bank version check expects 71. Everything else is unchanged. Usage: modernize_bank_harness_v1265.py <old.ps1> <new.ps1> <note>"""
import re, sys
from pathlib import Path
P = Path(__file__).resolve().parent
src, dst, note = P / sys.argv[1], P / sys.argv[2], sys.argv[3]
s = src.read_bytes().decode('utf-8'); nl = '\r\n' if '\r\n' in s else '\n'
pat = re.compile(r'\$entry=Get-Content "\$PSScriptRoot/[a-z0-9-]+-bank-manifest-v\d+\.json" -Raw\|ConvertFrom-Json\|Where-Object role -eq \$character\s*' + '(?:' + re.escape(nl) + r'\s*)?'
                 r'\$bank=Join-Path \$PSScriptRoot \$entry\.bank;if\(\(Get-FileHash \$bank\)\.Hash-ne\$entry\.sha256\)\{throw \'[^\']*\'\}')
s, n = pat.subn('$bank="$PSScriptRoot/use-banks-v1212/$character-choke-bank.bin";if(!(Test-Path -LiteralPath $bank)){throw \'Current bank missing\'}', s)
assert n >= 1, 'bank selection not found'
s, m = re.subn(r'\$m\.version-(eq|ne)\d+', lambda x: f'$m.version-{x.group(1)}71', s)
assert m >= 1, 'version checks not found'
lines = s.split(nl); lines.insert(1, f'# {note}')
dst.write_bytes(nl.join(lines).encode('utf-8'))
print(f'ok: {n} bank selection(s), {m} version check(s)')
