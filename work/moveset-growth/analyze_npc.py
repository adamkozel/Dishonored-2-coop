from pathlib import Path
P=Path(__file__).resolve().parent
source=(P/'analyze_donors.py').read_text().replace("P/'donors'","P/'donors-npc'").replace("P/'donor-analysis.json'","P/'donor-analysis-npc.json'")
exec(compile(source,str(P/'analyze_donors.py'),'exec'))
