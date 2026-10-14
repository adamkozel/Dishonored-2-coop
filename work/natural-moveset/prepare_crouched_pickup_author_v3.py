"""Fit contacts to v29's stable orientation and completed carrier publication."""
from pathlib import Path
P=Path(__file__).resolve().parent
t=(P/'author_crouched_pickup_v2.py').read_text()
t=t.replace('crouched-pickup-sources-v2','crouched-pickup-sources-v3').replace('crouched-pickup-banks-v2','crouched-pickup-banks-v3')
t=t.replace("f'native-crouched-pickup-drop-{role}-v28/contact-analysis.json'", "f'native-crouched-pickup-{role}-orientation-v29/contact-analysis.json'")
start=t.index(' held=read(');end=t.index(' def support(p):',start)
t=t[:start]+""" held=read(P/f'native-crouched-pickup-{role}-orientation-v29/final-loaded-contact-analysis.json')
 support_delta=Vector(held['support_delta'])
"""+t[end:]
t=t.replace("held_contact=held['source']","held_contact=held['source_sha256']")
t=t.replace('crouched-pickup-source-manifest-v2','crouched-pickup-source-manifest-v3')
t=t.replace("crouched-pickup-bank-manifest-v2.json').write_text", "crouched-pickup-bank-manifest-v3.json').write_text")
t=t.replace('crouched-pickup-v2-authoring','crouched-pickup-v3-authoring')
t=t.replace('actual v28 remote crouched victim contact paths','actual v29 remote crouched victim contact paths')
(P/'author_crouched_pickup_v3.py').write_text(t)
print('V29_FINAL_CONTACT_AUTHOR_READY')
