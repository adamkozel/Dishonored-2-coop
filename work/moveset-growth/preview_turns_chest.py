from pathlib import Path
P=Path(__file__).resolve().parent
source=(P/'preview_turns.py').read_text().replace("P/'turns'","P/'turns-chest'").replace("P/'turn-previews'","P/'turn-chest-previews'").replace('native-turn-preview.blend','native-turn-chest-preview.blend')
exec(compile(source,str(P/'preview_turns.py'),'exec'),{'__file__':str(P/'preview_turns.py'),'__name__':'__main__'})
