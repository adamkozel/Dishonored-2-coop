"""Composes RELEASE-NOTES-1.0.0.md: the 1.0 head (RELEASE-NOTES-1.0.0-head.md) plus the 0.9.6 notes' reference sections
(requirements, install, play, saves, uninstall, troubleshooting, what the installer builds), edited for 1.0, and the 1.0
known limitations. build_release_v2195.py ships RELEASE-NOTES.md, so the result is copied there at release time.
Usage: python -I compose_notes_v1000.py"""
from pathlib import Path
import re, sys

D = Path(__file__).resolve().parent
old = (D / 'RELEASE-NOTES.md').read_text(encoding='utf-8')
if not old.startswith('# Dishonored 2 Corvo/Emily co-op 0.9.6\n'):
    sys.exit('RELEASE-NOTES.md is not the 0.9.6 notes')
head = (D / 'RELEASE-NOTES-1.0.0-head.md').read_text(encoding='utf-8')


def section(title):
    m = re.search(r'^## ' + re.escape(title) + r'\n(.*?)(?=^## |\Z)', old, re.S | re.M)
    if not m:
        sys.exit('missing section ' + title)
    return '## ' + title + '\n' + m.group(1)


def rep(text, a, b):
    if text.count(a) != 1:
        sys.exit('anchor %d: %r' % (text.count(a), a[:60]))
    return text.replace(a, b)


install = section('Install (both players)')
install = rep(install, '1. Close Dishonored 2. If an earlier version of the mod is installed, run its `Uninstall.cmd` first.',
              '1. Close Dishonored 2. An earlier version of the mod is updated in one step (see *Updating* above).')
install = rep(install, 'd2coop-v0.9.6.zip', 'd2coop-v1.0.0.zip')

play = section('Play')
play = play.rstrip('\n') + '\n\n' + (
    'How the two games share the world:\n\n'
    '- Partners cannot affect each other directly: no damage, powers, takedowns or prompts on each other. Normal '
    'collision is kept.\n'
    '- *Memories of Training* (the tutorial) is a stealth lesson for two: its trainer notices either player, and a '
    'noticed lesson restarts for both. Let one player take the stealth lessons while the other stays out of sight.\n\n')

saves = section('Saves, deaths and missions')
saves = rep(saves, '- Either player can do the story: objectives, story items and level changes completed by the partner count for both worlds.',
            '- Either player can do the story. The host\'s game records it for both: objectives, story items, level changes and '
            'the outcomes of characters (spared, killed, knocked out) reach the host\'s world whoever caused them.')

uninstall = section('Uninstall')
trouble = section('Troubleshooting')
trouble = rep(trouble, '| "Co-op is already installed here" | Run `Uninstall.cmd` first. |',
              '| "Co-op 1.0.0 is already installed here" | This version is already installed. To reinstall it, run `Uninstall.cmd` first. |')
trouble = rep(trouble, 'To go back to the 0.9.6 behaviour for one area', 'To go back to the 0.9.6 behaviour for one area')

limits = ('## Known limitations\n\n'
          '- Two players only.\n'
          '- The game uses a little more memory over long sessions, with or without the mod. Restart the game after several hours.\n\n')
about = ('## About the partner animation\n\n'
         '- The partner\'s movement banks are rebuilt from your game\'s animations. On a different CPU the values can differ '
         'from ours in their last digits, which is invisible.\n'
         '- Some partner movement clips are retargeted from Adobe Mixamo motion.\n\n')
builds = section('What the installer builds')
req = section('Requirements')

out = '\n'.join(s.rstrip('\n') + '\n' for s in (head.rstrip('\n'), req, install, play, saves, uninstall, trouble, limits, about, builds))
assert out.startswith('# Dishonored 2 Corvo/Emily co-op 1.0.0\n')
assert re.findall(r'd2coop-v[0-9.]+\.zip', out) == ['d2coop-v1.0.0.zip'], re.findall(r'd2coop-v[0-9.]+\.zip', out)
(D / 'RELEASE-NOTES-1.0.0.md').write_text(out, encoding='utf-8', newline='\n')
print('ok', len(out.splitlines()), 'lines')
