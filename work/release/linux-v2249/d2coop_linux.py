#!/usr/bin/env python3
"""Dishonored 2 co-op: install or uninstall on Linux (Steam with Proton, for example Bazzite or SteamOS).

Does what Install-D2Coop.ps1 and Uninstall-D2Coop.ps1 do on Windows, with the same checks, backup and receipt:
- the package ships no game files; every game-derived file is built from this game folder's own files by
  bin/d2coop_setup.exe, which runs under the Wine of the Proton that Steam uses for Dishonored 2 (or under
  system Wine);
- it refuses unless Dishonored2.exe and steam_api64.dll are the exact supported builds and base/master.index
  is the original;
- nothing in the game folder changes until every file has been built and checked, and any failure while
  copying rolls back.

Usage: install-linux.sh [<Dishonored2 folder>]   uninstall-linux.sh [<Dishonored2 folder>] [--keep-backup]
"""
import datetime
import hashlib
import json
import os
import re
import shutil
import subprocess
import sys
import tempfile

APP_ID = '403640'
PACKAGE = os.path.dirname(os.path.abspath(__file__))
RECEIPT = 'd2coop-install-receipt.json'
LAUNCH_OPTIONS = 'WINEDLLOVERRIDES="dinput8=n,b" %command%'


class Refused(Exception):
    pass


def sha256(path):
    h = hashlib.sha256()
    with open(path, 'rb') as f:
        for block in iter(lambda: f.read(1 << 20), b''):
            h.update(block)
    return h.hexdigest().upper()


def resolve(root, rel):
    """The existing path under root for a relative path, matching each part without case (Windows game files on a
    case-sensitive file system). A part that does not exist keeps the given spelling."""
    path = root
    for part in rel.replace('\\', '/').split('/'):
        exact = os.path.join(path, part)
        if os.path.exists(exact) or not os.path.isdir(path):
            path = exact
            continue
        match = [n for n in os.listdir(path) if n.lower() == part.lower()]
        path = os.path.join(path, match[0] if match else part)
    return path


def steam_roots():
    home = os.path.expanduser('~')
    roots = []
    for r in ('.local/share/Steam', '.steam/steam', '.steam/root', '.steam/debian-installation',
              '.var/app/com.valvesoftware.Steam/.local/share/Steam', '.var/app/com.valvesoftware.Steam/data/Steam'):
        p = os.path.join(home, r)
        if os.path.isdir(os.path.join(p, 'steamapps')):
            real = os.path.realpath(p)
            if real not in roots:
                roots.append(real)
    return roots


def steam_libraries():
    libs = []
    for root in steam_roots():
        for p in [root] + vdf_paths(os.path.join(root, 'steamapps', 'libraryfolders.vdf')):
            real = os.path.realpath(p)
            if os.path.isdir(os.path.join(real, 'steamapps')) and real not in libs:
                libs.append(real)
    return libs


def vdf_paths(vdf):
    try:
        with open(vdf, encoding='utf-8', errors='replace') as f:
            text = f.read()
    except OSError:
        return []
    return [m.replace('\\\\', '\\') for m in re.findall(r'"path"\s+"([^"]+)"', text)]


def find_game(want_receipt=False):
    for lib in steam_libraries():
        d = os.path.join(lib, 'steamapps', 'common', 'Dishonored2')
        marker = RECEIPT if want_receipt else 'Dishonored2.exe'
        if os.path.exists(resolve(d, marker)):
            return d
    what = 'No co-op installation' if want_receipt else 'Dishonored 2'
    script = 'uninstall-linux.sh' if want_receipt else 'install-linux.sh'
    raise Refused(f'{what} was not found in your Steam libraries. Run: ./{script} "<your Dishonored2 folder>"')


def game_running(game):
    """Any process running a Dishonored2.exe counts (under Proton its command line holds a Windows path)."""
    if not os.path.isdir('/proc'):
        return False
    for pid in os.listdir('/proc'):
        if not pid.isdigit():
            continue
        try:
            with open(f'/proc/{pid}/cmdline', 'rb') as f:
                args = f.read().decode('utf-8', 'replace').split('\0')
        except OSError:
            continue
        if any(a.replace('\\', '/').lower().endswith('/dishonored2.exe') or a.lower() == 'dishonored2.exe' for a in args):
            return True
    return False


# --- running the Windows setup tool -------------------------------------------------------------------------------

def proton_dirs(game):
    """Proton builds to try: first the one this game's prefix was made with (compatdata config_info), then every
    Proton in the Steam libraries and compatibilitytools.d, newest name last-sorted first."""
    found = []
    lib = os.path.dirname(os.path.dirname(os.path.dirname(os.path.realpath(game))))
    info = os.path.join(lib, 'steamapps', 'compatdata', APP_ID, 'config_info')
    try:
        with open(info, encoding='utf-8', errors='replace') as f:
            for line in f:
                m = re.match(r'(.*?)/(files|dist)/', line.strip())
                if m and os.path.isdir(m.group(1)):
                    found.append(m.group(1))
                    break
    except OSError:
        pass
    candidates = []
    for l in steam_libraries():
        common = os.path.join(l, 'steamapps', 'common')
        if os.path.isdir(common):
            candidates += [os.path.join(common, n) for n in os.listdir(common) if n.lower().startswith('proton')]
    for root in steam_roots():
        tools = os.path.join(root, 'compatibilitytools.d')
        if os.path.isdir(tools):
            candidates += [os.path.join(tools, n) for n in os.listdir(tools)]
    for c in sorted(candidates, reverse=True):
        if c not in found:
            found.append(c)
    return found


def wine_in(proton):
    for sub in ('files/bin/wine64', 'files/bin/wine', 'dist/bin/wine64', 'dist/bin/wine'):
        p = os.path.join(proton, sub)
        if os.access(p, os.X_OK):
            return p
    return None


def game_prefix(game):
    lib = os.path.dirname(os.path.dirname(os.path.dirname(os.path.realpath(game))))
    pfx = os.path.join(lib, 'steamapps', 'compatdata', APP_ID, 'pfx')
    return pfx if os.path.isdir(os.path.join(pfx, 'drive_c')) else None


class SetupRunner:
    """Runs bin/d2coop_setup.exe. D2COOP_WINE picks a Wine binary; otherwise the Proton Wine with the game's own
    prefix, then system Wine with a private prefix. The tool only reads and writes files, so the prefix is not
    changed beyond what Wine itself keeps up to date."""

    def __init__(self, game):
        self.game = game
        self.temp_prefix = None
        self.choices = []
        pfx = game_prefix(game)
        forced = os.environ.get('D2COOP_WINE')
        if forced:
            self.choices.append((forced, pfx or self._private_prefix(), 'D2COOP_WINE'))
        else:
            if pfx:
                for proton in proton_dirs(game):
                    w = wine_in(proton)
                    if w:
                        self.choices.append((w, pfx, os.path.basename(proton)))
            system = shutil.which('wine64') or shutil.which('wine')
            if system:
                self.choices.append((system, self._private_prefix(), 'system Wine'))
        if os.environ.get('D2COOP_SETUP_NATIVE') == '1':  # lab test on Windows only
            self.choices = [(None, None, 'native')]
        if not self.choices:
            raise Refused('No Wine to run the setup tool: start Dishonored 2 once from Steam (so Proton makes its '
                          'prefix), then run this again; nothing changed.')
        self.used = None

    def _private_prefix(self):
        if not self.temp_prefix:
            self.temp_prefix = tempfile.mkdtemp(prefix='d2coop-wine-')
        return self.temp_prefix

    @staticmethod
    def winpath(p):
        return 'Z:' + os.path.abspath(p).replace('/', '\\')

    def run(self, args, what):
        print(f'Building {what} from your game files...', flush=True)
        exe = os.path.join(PACKAGE, 'bin', 'd2coop_setup.exe')
        errors = []
        order = [self.used] if self.used else self.choices
        for choice in order:
            wine, prefix, name = choice
            if wine is None:
                cmd, env = [exe] + args, None
            else:
                cmd = [wine, self.winpath(exe)] + [self.winpath(a) if os.path.isabs(a) else a for a in args]
                env = dict(os.environ, WINEPREFIX=prefix, WINEDEBUG='-all', WINEDLLOVERRIDES='mscoree,mshtml=')
                for k in ('WINELOADERNOEXEC', 'WINESERVER'):
                    env.pop(k, None)
            try:
                r = subprocess.run(cmd, env=env, stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=1800)
            except (OSError, subprocess.TimeoutExpired) as e:
                errors.append(f'{name}: {e}')
                continue
            out = r.stdout.decode('utf-8', 'replace').replace('\r', '').strip().splitlines()
            last = next((l for l in reversed(out) if l.startswith('{')), None)
            if r.returncode == 0 and last:
                self.used = choice
                return json.loads(last)
            tail = ' '.join((out + r.stderr.decode('utf-8', 'replace').strip().splitlines())[-6:])
            errors.append(f'{name}: exit {r.returncode} {tail}')
            if self.used:
                break
        raise Refused(f'Building {what} failed: ' + ' | '.join(errors))

    def close(self):
        if self.temp_prefix:
            shutil.rmtree(self.temp_prefix, ignore_errors=True)


# --- install / uninstall ------------------------------------------------------------------------------------------

def load_manifest():
    with open(os.path.join(PACKAGE, 'manifest.json'), encoding='utf-8-sig') as f:
        return json.load(f)


def check_game(game, manifest):
    if game_running(game):
        raise Refused('Close Dishonored 2 first; nothing changed.')
    if sha256(resolve(game, 'Dishonored2.exe')) != manifest['requiredExecutableSha256'].upper():
        raise Refused('Unsupported Dishonored2.exe build (the mod needs the current Steam version); nothing changed.')
    if sha256(resolve(game, 'steam_api64.dll')) != manifest['requiredSteamApiSha256'].upper():
        raise Refused('Unexpected steam_api64.dll; nothing changed.')


def install(game):
    manifest = load_manifest()
    check_game(game, manifest)
    receipt_path = resolve(game, RECEIPT)
    if os.path.exists(receipt_path):
        with open(receipt_path, encoding='utf-8-sig') as f:
            old = json.load(f)
        if str(old.get('version')) == str(manifest['version']):
            raise Refused(f"Co-op {manifest['version']} is already installed here (receipt present); nothing changed.")
        print(f"Updating co-op {old.get('version')} to {manifest['version']}: removing {old.get('version')} first...")
        uninstall(game, keep_backup=False)
    if sha256(resolve(game, 'base/master.index')) != manifest['originalMasterIndexSha256'].upper():
        raise Refused('base/master.index is not the original; restore it (Steam: Properties > Installed Files > '
                      'Verify integrity) first.')
    shipped = [{'path': 'files/' + f['path'], 'sha256': f['sha256']} for f in manifest['files']]
    for f in shipped + manifest['tools'] + manifest['data']:
        if sha256(os.path.join(PACKAGE, f['path'])) != f['sha256'].upper():
            raise Refused(f"Package file corrupted: {f['path']}; download the release again.")

    stage = os.path.join(game, 'd2coop-install-staging')
    shutil.rmtree(stage, ignore_errors=True)
    os.makedirs(os.path.join(stage, 'd2coop'))
    runner = SetupRunner(game)
    try:
        runner.run(['packages', resolve(game, 'base'), os.path.join(PACKAGE, 'data', 'decl-recipes.bin'),
                    os.path.join(stage, 'base')], 'the co-op resource packages')
        print(f'(setup tool ran under {runner.used[2]})')
        for g in manifest['generated']:
            if g.get('sha256') and sha256(os.path.join(stage, g['path'])) != g['sha256'].upper():
                raise Refused(f"Built file differs from the release: {g['path']}; nothing changed.")
        for role in ('corvo', 'emily'):
            runner.run(['banks', game, os.path.join(PACKAGE, 'data', f'{role}-bank.patch'), os.path.join(stage, 'd2coop')],
                       f'the {role} movement bank')
        runner.run(['tables', game, os.path.join(PACKAGE, 'data', 'native-tables.recipe'), os.path.join(stage, 'd2coop')],
                   'the prop animation tables')
        for g in manifest['generated']:
            if not os.path.exists(os.path.join(stage, g['path'])):
                raise Refused(f"Missing built file: {g['path']}; nothing changed.")
    except BaseException:
        shutil.rmtree(stage, ignore_errors=True)
        raise
    finally:
        runner.close()

    items = [(f['path'], os.path.join(PACKAGE, 'files', f['path'])) for f in manifest['files']]
    items += [(g['path'], os.path.join(stage, g['path'])) for g in manifest['generated']]
    stamp = datetime.datetime.now().strftime('%Y%m%d-%H%M%S')
    backup_rel = f'd2coop-backup/{stamp}'
    backup = os.path.join(game, backup_rel)
    os.makedirs(backup)
    backed_up, added, installed = [], [], []
    for rel, source in items:
        target = resolve(game, rel)
        if os.path.exists(target):
            dest = os.path.join(backup, rel)
            os.makedirs(os.path.dirname(dest), exist_ok=True)
            shutil.copy2(target, dest)
            backed_up.append({'path': rel, 'sha256': sha256(target)})
        else:
            added.append(rel)
        installed.append({'path': rel, 'sha256': sha256(source)})
    receipt = {'installed': datetime.datetime.now().astimezone().isoformat(), 'version': manifest['version'],
               'candidate': manifest.get('candidate'), 'backup': backup_rel, 'backedUp': backed_up, 'added': added,
               'files': installed, 'installer': 'linux'}
    with open(os.path.join(game, RECEIPT), 'w', encoding='utf-8') as f:
        json.dump(receipt, f, indent=2)
    try:
        for rel, source in items:
            dest = resolve(game, rel)
            os.makedirs(os.path.dirname(dest), exist_ok=True)
            shutil.copyfile(source, dest)
        for i in installed:
            if sha256(resolve(game, i['path'])) != i['sha256']:
                raise Refused(f"Installed file mismatch: {i['path']}")
    except BaseException as e:
        for a in added:
            t = resolve(game, a)
            if os.path.exists(t):
                os.remove(t)
        for b in backed_up:
            shutil.copyfile(os.path.join(backup, b['path']), resolve(game, b['path']))
        restored = all(sha256(resolve(game, b['path'])) == b['sha256'] for b in backed_up)
        if restored:
            os.remove(os.path.join(game, RECEIPT))
            shutil.rmtree(backup, ignore_errors=True)
        shutil.rmtree(stage, ignore_errors=True)
        if restored:
            raise Refused(f'Install failed and was rolled back: {e}')
        raise Refused(f'Install failed: {e}. Run uninstall-linux.sh to restore the original files.')
    shutil.rmtree(stage, ignore_errors=True)
    print(f"Installed Dishonored 2 co-op {manifest['version']}: {len(installed)} files; originals backed up in {backup_rel}")
    print()
    print('One more step in Steam (Proton loads its own dinput8.dll unless told otherwise):')
    print('  Dishonored 2 > Properties > General > Launch Options, set:')
    print(f'  {LAUNCH_OPTIONS}')


def uninstall(game, keep_backup):
    if game_running(game):
        raise Refused('Close Dishonored 2 first; nothing changed.')
    receipt_path = resolve(game, RECEIPT)
    if not os.path.exists(receipt_path):
        raise Refused('No co-op install receipt here; nothing to roll back.')
    with open(receipt_path, encoding='utf-8-sig') as f:
        receipt = json.load(f)
    backup = os.path.join(game, receipt['backup'].replace('\\', '/'))
    for b in receipt['backedUp']:
        src = resolve(backup, b['path'])
        if not os.path.exists(src) or sha256(src) != b['sha256'].upper():
            raise Refused(f"Backup missing or changed: {b['path']}; nothing changed.")
    for a in receipt['added']:
        t = resolve(game, a)
        if os.path.exists(t):
            os.remove(t)
    for b in receipt['backedUp']:
        shutil.copyfile(resolve(backup, b['path']), resolve(game, b['path']))
    for b in receipt['backedUp']:
        if sha256(resolve(game, b['path'])) != b['sha256'].upper():
            raise Refused(f"Restore mismatch: {b['path']}")
    for a in receipt['added']:
        if os.path.exists(resolve(game, a)):
            raise Refused(f'Added file still present: {a}')
    data = resolve(game, 'd2coop')
    if os.path.isdir(data) and any(a.replace('\\', '/').startswith('d2coop/') for a in receipt['added']):
        for n in os.listdir(data):
            if n.startswith('coop-status.json'):
                os.remove(os.path.join(data, n))
        if not os.listdir(data):
            os.rmdir(data)
    os.remove(receipt_path)
    if not keep_backup:
        shutil.rmtree(backup, ignore_errors=True)
        root = os.path.join(game, 'd2coop-backup')
        if os.path.isdir(root) and not os.listdir(root):
            os.rmdir(root)
    print(f"Rolled back: {len(receipt['added'])} added files removed, {len(receipt['backedUp'])} originals restored.")
    print(f'You can now clear the Dishonored 2 launch option {LAUNCH_OPTIONS} in Steam.')


def main(argv):
    if len(argv) < 2 or argv[1] not in ('install', 'uninstall'):
        print('usage: d2coop_linux.py install|uninstall [<Dishonored2 folder>] [--keep-backup]', file=sys.stderr)
        return 2
    action, rest = argv[1], argv[2:]
    keep = '--keep-backup' in rest
    folders = [a for a in rest if a != '--keep-backup']
    try:
        if folders:
            game = os.path.abspath(os.path.expanduser(folders[0]))
            if not os.path.isdir(game):
                raise Refused(f'Not a folder: {game}')
        else:
            game = find_game(want_receipt=(action == 'uninstall'))
            print(('Co-op installation found: ' if action == 'uninstall' else 'Dishonored 2 found: ') + game)
        if action == 'install':
            install(game)
        else:
            uninstall(game, keep)
        return 0
    except Refused as e:
        print(f'ERROR: {e}', file=sys.stderr)
        return 1


if __name__ == '__main__':
    sys.exit(main(sys.argv))
