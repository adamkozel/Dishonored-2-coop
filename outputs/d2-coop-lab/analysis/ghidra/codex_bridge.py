"""Serve the pinned GhidraMCP bridge (stdio) for the local saved Ghidra project, whatever state the backend is in.

The MCP connection must never depend on the Ghidra backend. Until 2026-10-24 this script ran Start-D2Ghidra.ps1
before serving and exited when it failed, and Claude Code then kept d2-ghidra failed until someone reconnected it
(/mcp). Its log (mcp-logs-d2-ghidra, 2026-10-23T20-51-39) shows both failures: a cold Ghidra start took longer than
Claude Code's 30 s connect window, and two sessions starting together launched two servers that raced for the
project lock ("Server started but failed to reopen the saved program"), the loser keeping port 8089 without the
program.

Now:
- Ghidra's tools are registered from the live /mcp/schema, or from the last one cached (schema-cache.json) while
  the backend is down, so the connection completes in seconds with the full tool list.
- The backend is checked and, if needed, started or repaired in the background and before every request. A server
  of ours on 8089 without the program is stopped and restarted. Only one bridge at a time does this (a lock file),
  so concurrent sessions never launch two servers.
- A request waits for the backend (up to 150 s, a cold start) and retries once after a refused connection.
Only the backend this project owns (127.0.0.1:8089, our server.args) is ever started or stopped.
"""
import json
import msvcrt
import os
import subprocess
import sys
import threading
import time
import urllib.request
from pathlib import Path

here = Path(__file__).resolve().parent
workspace = here.parents[3]
runtime = workspace / 'work/tools/ghidra-runtime'
runtime.mkdir(parents=True, exist_ok=True)
pwsh = Path('C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/native/powershell/pwsh.exe')
OWNED_URL = 'http://127.0.0.1:8089'
URL = os.environ.get('GHIDRA_MCP_URL') or OWNED_URL
os.environ['GHIDRA_MCP_URL'] = URL
SCHEMA_CACHE = runtime / 'schema-cache.json'
LOCK_FILE = runtime / 'bridge.lock'
LOG_FILE = runtime / 'bridge.log'
START_WAIT_S = 150  # a cold start of the headless server with the saved program
HEALTH_TTL_S = 15   # a confirmed healthy backend is trusted this long before the next check


def log(message):
    line = f"{time.strftime('%Y-%m-%d %H:%M:%S')} [{os.getpid()}] {message}"
    print(line, file=sys.stderr, flush=True)
    try:
        with open(LOG_FILE, 'a', encoding='utf-8') as f:
            f.write(line + '\n')
    except OSError:
        pass


def health():
    """The backend's /health as a dict, or None when nothing answers."""
    try:
        with urllib.request.urlopen(URL + '/health', timeout=2) as r:
            return json.loads(r.read().decode('utf-8'))
    except Exception:
        return None


def healthy(h):
    return bool(h) and h.get('version') == '6.0.0-headless' and h.get('program_name') == 'Dishonored2.exe'


def our_servers():
    """[(pid, age_s)] of this project's headless Ghidra servers (java started with our server.args)."""
    script = ("Get-CimInstance Win32_Process -Filter \"Name='java.exe'\" | Where-Object { $_.CommandLine -like '*ghidra-runtime*server.args*' } | "
              "ForEach-Object { [pscustomobject]@{ pid = $_.ProcessId; age = ((Get-Date) - $_.CreationDate).TotalSeconds } } | ConvertTo-Json -Compress")
    try:
        out = subprocess.run([str(pwsh), '-NoProfile', '-Command', script], capture_output=True, text=True, timeout=30,
                             creationflags=subprocess.CREATE_NO_WINDOW).stdout.strip()
        rows = json.loads(out) if out else []
        rows = rows if isinstance(rows, list) else [rows]
        return [(int(r['pid']), float(r['age'])) for r in rows]
    except Exception as e:
        log(f'server query failed: {e}')
        return []


def stop_servers(reason):
    for pid, age in our_servers():
        log(f'stopping our Ghidra server pid {pid} (age {age:.0f} s): {reason}')
        subprocess.run(['taskkill', '/PID', str(pid), '/F', '/T'], capture_output=True, creationflags=subprocess.CREATE_NO_WINDOW)
    time.sleep(2)


def start_server():
    """Run Start-D2Ghidra.ps1 outside this bridge's process tree, so the server outlives the session that started it.

    A client may put its MCP server in a kill-on-close job: the Python MCP client does, and its test sessions took
    the backend down when they exited. Win32_Process.Create (WMI) starts the script outside the caller's job. The
    caller polls /health; the script's own output goes to start.log.
    """
    log('starting the Ghidra backend (Start-D2Ghidra.ps1, detached)')
    command = f'cmd.exe /c ""{pwsh}" -NoProfile -File "{here / "Start-D2Ghidra.ps1"}" >> "{runtime / "start.log"}" 2>&1"'
    create = ("$si=New-CimInstance -ClassName Win32_ProcessStartup -ClientOnly -Property @{ShowWindow=[uint16]0};"
              "(Invoke-CimMethod -ClassName Win32_Process -MethodName Create -Arguments "
              "@{CommandLine=$env:D2G_COMMAND;CurrentDirectory=$env:D2G_CWD;ProcessStartupInformation=$si}).ReturnValue")
    try:
        r = subprocess.run([str(pwsh), '-NoProfile', '-Command', create], capture_output=True, text=True, timeout=60,
                           env=dict(os.environ, D2G_COMMAND=command, D2G_CWD=str(workspace)), creationflags=subprocess.CREATE_NO_WINDOW)
        if r.stdout.strip() == '0':
            return
        log(f'detached start refused ({r.stdout.strip()} {r.stderr.strip()[:200]}); starting it in-process')
    except Exception as e:
        log(f'detached start failed ({e}); starting it in-process')
    try:
        with open(runtime / 'start.log', 'a', encoding='utf-8') as out:
            subprocess.run([str(pwsh), '-NoProfile', '-File', str(here / 'Start-D2Ghidra.ps1')], cwd=workspace,
                           stdout=out, stderr=out, timeout=120, creationflags=subprocess.CREATE_NO_WINDOW)
    except Exception as e:
        log(f'Start-D2Ghidra.ps1: {e}')  # the server may still be loading; the caller keeps polling /health


class _Lock:
    """Cross-process lock: one bridge at a time checks, starts or repairs the backend."""
    def __init__(self, deadline):
        self.deadline, self.f = deadline, None

    def __enter__(self):
        self.f = open(LOCK_FILE, 'a+b')
        while True:
            try:
                self.f.seek(0)
                msvcrt.locking(self.f.fileno(), msvcrt.LK_NBLCK, 1)
                return True
            except OSError:
                if time.time() > self.deadline:
                    return False
                time.sleep(0.5)

    def __exit__(self, *exc):
        try:
            self.f.seek(0)
            msvcrt.locking(self.f.fileno(), msvcrt.LK_UNLCK, 1)
        except OSError:
            pass
        self.f.close()


_local = threading.Lock()
_confirmed_at = 0.0


def ensure_backend(wait_s=START_WAIT_S, force=False):
    """True once the backend answers with Dishonored2.exe loaded; starts or repairs our own server if needed."""
    global _confirmed_at
    if not force and time.time() - _confirmed_at < HEALTH_TTL_S:
        return True
    deadline = time.time() + wait_s
    with _local:
        h = health()
        if healthy(h):
            _confirmed_at = time.time()
            return True
        if URL != OWNED_URL:
            return False  # not ours to start
        with _Lock(deadline) as locked:
            started = False
            while time.time() < deadline:
                h = health()
                if healthy(h):
                    _confirmed_at = time.time()
                    log('backend healthy')
                    return True
                if not locked:
                    time.sleep(2)  # another bridge is starting it
                    continue
                servers = our_servers()
                if h and h.get('version') != '6.0.0-headless':
                    log(f'port 8089 belongs to another server ({h.get("version")}); not touching it')
                    return False
                if h and servers and min(a for _, a in servers) > 20:
                    stop_servers('it answers without Dishonored2.exe loaded (lost the project lock)')
                    servers = []
                elif not h and servers and min(a for _, a in servers) > START_WAIT_S:
                    stop_servers('it has not answered since its start')
                    servers = []
                if not servers and not started:
                    start_server()
                    started = True
                    continue
                time.sleep(2)
            log(f'backend not healthy after {wait_s} s (last health: {h})')
            return False


def main():
    from bridge_mcp_ghidra import registry, state, transport
    from bridge_mcp_ghidra import static_tools  # noqa: F401  (registers the static tools)
    from bridge_mcp_ghidra.server import mcp

    state._active_tcp = URL
    state._transport_mode = 'tcp'
    original = transport.do_request

    def do_request(method, endpoint, params=None, json_data=None, timeout=30):
        if not ensure_backend():
            raise ConnectionError('The d2-ghidra backend is not available (see work/tools/ghidra-runtime/bridge.log); retry shortly.')
        try:
            return original(method, endpoint, params, json_data, timeout)
        except ConnectionRefusedError:
            pass  # never reached the server, so even a write is safe to resend
        except (ConnectionError, OSError):
            if method != 'GET':
                raise  # a write may have been applied before the drop
        log(f'{method} {endpoint}: connection lost; checking the backend')
        if not ensure_backend(force=True):
            raise ConnectionError('The d2-ghidra backend did not come back (see work/tools/ghidra-runtime/bridge.log).')
        return original(method, endpoint, params, json_data, timeout)

    transport.do_request = do_request

    count, source = 0, None
    if healthy(health()):
        try:
            text, status = original('GET', '/mcp/schema', timeout=10)
            if status == 200:
                raw = json.loads(text)
                count = registry.register_tools_from_schema(registry._parse_schema(raw), groups=None)
                SCHEMA_CACHE.write_text(json.dumps(raw), encoding='utf-8')
                source = 'live'
        except Exception as e:
            log(f'live schema fetch failed: {e}')
    if not source and SCHEMA_CACHE.exists():
        raw = json.loads(SCHEMA_CACHE.read_text(encoding='utf-8'))
        count = registry.register_tools_from_schema(registry._parse_schema(raw), groups=None)
        source = 'cache'
    log(f'serving d2-ghidra: {count} tools from the {source or "static set only"} schema; backend {URL}')
    if source != 'live':
        threading.Thread(target=ensure_backend, daemon=True).start()
    mcp.settings.log_level = 'INFO'
    mcp.run(transport='stdio')


if __name__ == '__main__':
    main()
