"""Audit 1240 (#11): sit_stand is not applicable to the D2 player. Confirmed static (d2-ghidra string census of the
retail executable): no arkPlayerState* class is a sitting state, and the only sitting strings are the NPC AI
tactical-info providers. Writes the audit and the catalog annotation, then rebuilds the review catalog."""
from pathlib import Path
import json, datetime, subprocess, sys
R = Path(__file__).resolve().parents[2]; W = R / 'work/movement-integration'; N = R / 'work/natural-moveset'
rd = lambda p: json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p, v): p.write_text(json.dumps(v, indent=2), encoding='utf-8')
STATES = ['Action', 'Assassination', 'AutoCrouch', 'BlackMarket', 'BlackMarketGoto', 'BlackMarketInteract', 'Blinking', 'Block',
          'BloodthirstyFatality', 'BriskEye', 'Cancel', 'CancelClimb', 'CancelFireWeapon', 'CancelGrapple', 'CarryCorpse', 'CatFall',
          'CharacterChoice', 'Choke', 'Cinematic', 'Climb', 'Crank', 'CureChoke', 'Dead', 'DoppelgangerSwap', 'Draw', 'DropChoke',
          'DropCorpse', 'DropReady', 'EatRat', 'EnterPossessedHost', 'EquipChange', 'Falling', 'FireWeapon', 'FishSwimming', 'Flying',
          'GrabGrappledEntity', 'Grapple', 'HoldObject', 'Idle', 'InAir', 'InBendTime', 'Interact', 'Journal', 'Jumping', 'KillCam',
          'KnockedDown', 'Leaning', 'LockHeartTarget', 'LootObject', 'Mantle', 'Navigation', 'NpcFatality', 'Pathing', 'Peeking',
          'PlaceWeapon', 'PlaceWeaponReady', 'PlayHeartBark', 'Powerwheel', 'ReadNote', 'Recoil', 'ReleaseObject', 'ReloadWeapon',
          'RequestReloadWeapon', 'Running', 'ShadowHide', 'ShadowWalk', 'Slash', 'Sliding', 'Slipping', 'Sneaking', 'Sprinting', 'Spyglass',
          'SpyglassZoom', 'StoneShroud', 'Stumble', 'Stun', 'SwimMantle', 'Swimming', 'SyncAnim', 'ThrowWeapon', 'Undraw',
          'UnfoldTimeShiftDevice', 'UnfoldTimeShiftLoop', 'UseHeart', 'UsePhantasmalLink', 'UsePower', 'UseTimeShiftDevice',
          'UseWindblast', 'Vault', 'Versus', 'VersusGrab', 'WallCrouch', 'Whisper']
at = datetime.datetime.now().astimezone().isoformat()
wr(W / 'not-applicable-audit-v1240.json', dict(at=at, requirement='sit_stand',
    method='d2-ghidra string census of the retail Dishonored2.exe: regex arkPlayerState[A-Za-z]* (178 strings: class names, '
           'method/param/save-data names and container types) and (?i)(sitting|sit_|_sit|issit|sitdown|sit down|chair).',
    player_state_classes=STATES,
    sitting_strings=['TIProvide_ProvideIsSitting', 'TIProvide_ProvideTargetIsSitting'],
    findings=['Confirmed static: none of the player state classes is a sitting or seated state; the player has no native sit/stand action to bind.',
              'Confirmed static: the only sitting strings are NPC AI tactical-info providers (TIProvide_*), used for NPC distraction behaviour.',
              'The six authored sit_stand candidates are NPC bench-distraction clips (empty_distraction_sitting_bench_*); they stay in the library but have no player state to bind to.',
              'Scripted moments where a player character is seated play as native cinematics (logic 5), where the partner presentation is retired and restored (audit 1239, cutscene_enter_exit).'],
    ruling='not_applicable_no_native_player_state', complete=True))
p = N / 'review-evidence-annotations.json'; an = rd(p); it = an['items']['sit_stand']
it['phase_status'] = 'not_applicable'
it['deliverable_kind'] = 'not_applicable_no_native_player_state'
it['notes'] = ('Not applicable (audit 1240, confirmed static): D2 has no player sitting state among its arkPlayerState classes; the only sitting '
               'strings are NPC AI providers. Seated story moments are native cinematics, covered by cutscene_enter_exit. The authored '
               'NPC bench clips remain in the library unbound.')
if '../movement-integration/not-applicable-audit-v1240.json' not in it.setdefault('evidence', []): it['evidence'].append('../movement-integration/not-applicable-audit-v1240.json')
wr(p, an)
out = subprocess.run([sys.executable, '-X', 'utf8', str(N / 'build_review_catalog.py')], cwd=N, capture_output=True, text=True)
print(out.stdout[-160:], out.stderr[-300:]); assert out.returncode == 0
