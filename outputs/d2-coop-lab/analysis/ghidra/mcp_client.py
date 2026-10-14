"""Project-local MCP client for the pinned Ghidra bridge (also usable in existing chats)."""
import argparse
import asyncio
import json
import os
import sys
from datetime import timedelta
from pathlib import Path
from mcp import ClientSession, StdioServerParameters
from mcp.client.stdio import stdio_client

HERE = Path(__file__).resolve().parent
WORKSPACE = HERE.parents[3]
PYTHON = WORKSPACE / 'work/tools/ghidra-mcp-6.0.0/.venv/Scripts/python.exe'

async def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('tool')
    parser.add_argument('--args', default='{}')
    parser.add_argument('--args-file')
    parser.add_argument('--output')
    parser.add_argument('--match', default='')
    ns = parser.parse_args()
    env = dict(os.environ, GHIDRA_MCP_URL='http://127.0.0.1:8089')
    params = StdioServerParameters(command=str(PYTHON), args=[str(HERE/'codex_bridge.py')], env=env)
    async with stdio_client(params) as (read, write):
        async with ClientSession(read, write, read_timeout_seconds=timedelta(seconds=300)) as session:
            await session.initialize()
            if ns.tool == 'catalog':
                result = (await session.list_tools()).model_dump(mode='json')
                if ns.match:
                    result['tools'] = [t for t in result['tools'] if any(x in t['name'] for x in ns.match.split(','))]
            else:
                arguments = json.loads(Path(ns.args_file).read_text(encoding='utf-8-sig')) if ns.args_file else json.loads(ns.args)
                if ns.tool == 'run_script_inline' and 'code' in arguments:
                    arguments['code'] = arguments['code'].replace('\r\n', '\n')
                result = (await session.call_tool(ns.tool, arguments)).model_dump(mode='json')
            # The bridge sometimes returns HTTP errors inside a text block
            # while MCP isError remains false (notably an import timeout).
            for block in result.get('content', []):
                if block.get('type') != 'text':
                    continue
                if '=== SCRIPT EXECUTION ERROR ===' in block.get('text', ''):
                    result['isError'] = True
                try:
                    inner = json.loads(block['text'])
                except (ValueError, KeyError):
                    continue
                if isinstance(inner, dict) and (inner.get('error') or inner.get('success') is False):
                    result['isError'] = True
            encoded = json.dumps(result, indent=2, ensure_ascii=False)
            if ns.output:
                Path(ns.output).write_text(encoded + '\n', encoding='utf-8')
                print(json.dumps({'tool': ns.tool, 'output': str(Path(ns.output).resolve()), 'isError': result.get('isError', False), 'tools': len(result.get('tools', []))}))
            else:
                print(encoded)
            failed = result.get('isError', False)
    return 1 if failed else 0

if __name__ == '__main__':
    sys.exit(asyncio.run(main()))
