"""Contact sheets for review: one tiled JPEG per MP4 in a take folder
(<name>-sheet.jpg next to the video). Usage: make_review_sheets.py TAKE_DIR [fps]"""
import subprocess,sys,json
from pathlib import Path
take=Path(sys.argv[1]);fps=float(sys.argv[2]) if len(sys.argv)>2 else 1.5
for v in sorted(take.glob('*.mp4')):
    d=float(subprocess.check_output(['ffprobe','-v','error','-show_entries','format=duration','-of','csv=p=0',str(v)],text=True).strip())
    n=max(1,int(d*fps));cols=4;rows=(n+cols-1)//cols
    out=v.with_name(v.stem+'-sheet.jpg')
    subprocess.run(['ffmpeg','-hide_banner','-loglevel','error','-y','-i',str(v),'-vf',f'fps={fps},scale=440:-1,tile={cols}x{rows}','-frames:v','1','-q:v','4',str(out)],check=True)
    print(json.dumps(dict(video=v.name,duration=round(d,2),frames=n,sheet=out.name)))
