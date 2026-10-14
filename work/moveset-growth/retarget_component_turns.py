from pathlib import Path
P=Path(__file__).resolve().parent
source=(P/'retarget_turns.py').read_text().replace("OUT=P/'turns'","OUT=P/'turns-component'").replace("P/'donors-feedback'","P/'donors-component-feedback'")
exec(compile(source,str(P/'retarget_turns.py'),'exec'),{'__file__':str(P/'retarget_turns.py'),'__name__':'__main__'})
