from pathlib import Path
P=Path(__file__).resolve().parent
source=(P/'preview_turns.py').read_text().replace("P/'turns'","P/'turns-component'").replace("P/'turn-previews'","P/'turn-component-previews'").replace('native-turn-preview.blend','native-turn-component-preview.blend')
exec(compile(source,str(P/'preview_turns.py'),'exec'),{'__file__':str(P/'preview_turns.py'),'__name__':'__main__'})
