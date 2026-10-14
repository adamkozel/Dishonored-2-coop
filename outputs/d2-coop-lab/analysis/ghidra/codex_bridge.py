"""Start/reuse the local saved Ghidra project, then serve the pinned MCP bridge."""
import os
import subprocess
import sys
from pathlib import Path

here = Path(__file__).resolve().parent
workspace = here.parents[3]
env = dict(os.environ, GHIDRA_MCP_URL='http://127.0.0.1:8089')
pwsh = Path('C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe')
subprocess.run([str(pwsh), '-NoProfile', '-File', str(here/'Start-D2Ghidra.ps1')],
               cwd=workspace, env=env, check=True, stdout=sys.stderr, stderr=sys.stderr,
               creationflags=subprocess.CREATE_NO_WINDOW, timeout=55)
from bridge_mcp_ghidra.cli import main
os.environ['GHIDRA_MCP_URL'] = env['GHIDRA_MCP_URL']
sys.argv = ['bridge-mcp-ghidra', '--transport', 'stdio']
main()
