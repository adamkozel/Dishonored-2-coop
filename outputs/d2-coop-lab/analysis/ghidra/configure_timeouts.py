"""Set only the registered project MCP server's startup and call timeouts."""
import re
import tomllib
from pathlib import Path

config = Path('C:/Users/adako/.codex/config.toml')
text = config.read_text(encoding='utf-8')
parsed = tomllib.loads(text)
server = parsed['mcp_servers']['d2-ghidra']
assert 'ghidra-mcp-6.0.0' in server['command']
pattern = r'(\[mcp_servers\.d2-ghidra\]\s*\n)([^\[]*)'
match = re.search(pattern, text)
if not match:
    raise RuntimeError('Expected Codex MCP server section not found')
body = match[2]
body = re.sub(r'(?m)^(startup_timeout_sec|tool_timeout_sec)\s*=.*\n?', '', body)
replacement = match[1] + 'startup_timeout_sec = 60\ntool_timeout_sec = 180\n' + body
updated = text[:match.start()] + replacement + text[match.end():]
tomllib.loads(updated)
config.write_text(updated, encoding='utf-8')
print('Updated d2-ghidra timeouts only')
