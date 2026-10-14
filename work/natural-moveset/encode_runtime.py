"""Preserve actual capture intervals; label pose-only runtime review honestly."""
from pathlib import Path
import json,subprocess,argparse
P=Path(__file__).resolve().parent
parser=argparse.ArgumentParser();parser.add_argument('takes',nargs='*');args=parser.parse_args()
prior=P/'runtime-video-manifest.json';report=json.loads(prior.read_text())if prior.exists()else[]
for take in (args.takes or ['native-corvo-1','native-emily-1']):
 d=P/take;rows=json.loads((d/'frames.json').read_text());events=json.loads((d/'take.json').read_text(encoding='utf-8-sig'));assert not events['failure']
 lines=['ffconcat version 1.0']
 for i,row in enumerate(rows):
  lines.append("file 'frames/"+row['file']+"'");lines.append('duration '+str((rows[i+1]['time']-row['time'])/1000 if i+1<len(rows) else .08))
 lines.append("file 'frames/"+rows[-1]['file']+"'");(d/'video.ffconcat').write_text('\n'.join(lines))
 out=P/(take+'.mp4')
 subprocess.run(['ffmpeg','-hide_banner','-loglevel','error','-y','-safe','0','-i',str(d/'video.ffconcat'),'-vf','pad=ceil(iw/2)*2:ceil(ih/2)*2','-c:v','libx264','-crf','19','-pix_fmt','yuv420p','-r','30','-movflags','+faststart',str(out)],check=True)
 report=[r for r in report if r['video']!=out.name]
 report.append(dict(video=out.name,character=events['character'],alias=events.get('alias'),actual_capture_hz=(len(rows)-1)*1000/(rows[-1]['time']-rows[0]['time']),container_fps=30,timing='Actual capture intervals, no speedup',scope=events['scope']))
(P/'runtime-video-manifest.json').write_text(json.dumps(report,indent=2));print(json.dumps(report,indent=2))
