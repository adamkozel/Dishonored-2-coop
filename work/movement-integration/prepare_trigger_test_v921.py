from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
s=(N/'Test-NpcCombat-v920.ps1').read_text().replace('v920','v921')
s=s.replace(' # Observe the active map near the target; no scripted damage injection.',''' # Activate the exact observed authored map trigger once via native console.
 # This is a trigger fixture, not a naturally occurring campaign event.
 C host 'trigger esc_env_trigger_4'
 Save "$out/trigger-fixture.json" @{command='trigger esc_env_trigger_4';role='host';scope='Native console activation, consumption is not evidence of damage. Verify actual native outcomes separately.'}''')
s=s.replace("scope='Natural map wait, no event injection. Zero outcomes is explicitly unexercised.'","scope='Single native trigger activation fixture. Zero outcomes is explicitly unexercised.'")
(N/'Test-NpcCombat-v921.ps1').write_text(s)
