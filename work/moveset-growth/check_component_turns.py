from pathlib import Path
P=Path(__file__).resolve().parent
source=(P/'check_turns.py').read_text().replace("P/'turns'","P/'turns-component'").replace("P/'turns/quality.json'","P/'turns-component/quality.json'").replace("P/'donors-feedback'","P/'donors-component-feedback'")
exec(compile(source,str(P/'check_turns.py'),'exec'),{'__file__':str(P/'check_turns.py'),'__name__':'__main__'})
