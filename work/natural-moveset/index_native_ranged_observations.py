"""Index actual native-source investigations without claiming remote integration."""
import json
from pathlib import Path
P=Path(__file__).resolve().parent
rows=[]
for role in ('corvo','emily'):
 for kind in ('gun','crossbow'):
  take=f'native-ranged-{role}-{kind}-actions-v2'
  result=json.loads((P/take/'result.json').read_text(encoding='utf-8-sig'))
  assert result['passed'] and len(result['checks'])==5
  assert (P/f'{take}.mp4').stat().st_size>10000
  assert (P/take/'contact.jpg').exists()
  rows.append(dict(character=role,kind=kind,take=take,recording=take+'.mp4',result=take+'/result.json',contact=take+'/contact.jpg',checks=result['checks'],dll=result['dll'],contact_visually_inspected=True))
report=dict(complete_remote_ranged=False,scope='Standing native source action/ownership/timing and reversible inventory fixture investigation. These are first-person reference recordings, not remote movement-port acceptance.',
 native_action_variants={1:['gun_empty',500],2:['crossbow_empty',500],3:['gun_fire',666],4:['gun_reload',1500],5:['gun_equip',533],6:['gun_unequip',333],7:['crossbow_fire',666],8:['crossbow_reload',900],9:['crossbow_equip',533],10:['crossbow_unequip',333]},
 samples=rows,checks=20,
 confirmed=['Both character roles: exact selected and started inventory weapon and owned left arm bound to the same active native Midnight scene.',
 'Native acquisition requires ordinary X holster/draw to initialize visible first-person equipment presentation in this fixture.',
 'Pistol acquisition grants a role-specific default upgrade. Snapshot and restore that upgrade along with ammo/weapon selection and quantities.',
 'Two regular shots, reload, empty trigger, holster and draw observed for all four combinations.',
 'Player inventories and health restored, protected files unchanged; isolated checkpoint reloaded after shots to clear impacts/projectiles.'],
 visual_notes=['Pistol source recordings show native weapon, muzzle/impact flashes, smoke and reload/holster/draw.',
 'Crossbow source recordings show bolts, mechanism motion, hand presentation and native holster/draw. Crossbow is partly cropped at left in the narrow capture; no complete mechanism quality acceptance claimed.',
 'All four temporal contact sheets inspected; contact sheets do not establish continuous animation quality.'],
 remaining=['Remote weapon hand attachment and grip','Whole-body action binding through Steam','Moving/crouched/airborne variants and interruptions','Weapon mechanism animation and native remote effects','Upgrade and ammunition variants','Remote actual-game recordings and visual acceptance'])
(P/'native-ranged-action-findings-v2.json').write_text(json.dumps(report,indent=2),encoding='utf-8')
print('Indexed four native-source observations /20 checks; remote integration remains incomplete')
