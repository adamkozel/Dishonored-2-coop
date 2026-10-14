"""Keep original screenshot times; 30fps container does not imply 30Hz capture."""
from pathlib import Path
import json,subprocess
P=Path(__file__).resolve().parent
report=[]
for take,name in [('corvo-video-1','corvo-walking'),('emily-video-2','emily-walking')]:
 rows=json.loads((P/take/'frames.json').read_text());lines=['ffconcat version 1.0']
 for i,row in enumerate(rows):
  lines.append("file 'frames/"+row['file']+"'")
  lines.append('duration '+str((rows[i+1]['time']-row['time'])/1000 if i+1<len(rows) else .08))
 lines.append("file 'frames/"+rows[-1]['file']+"'")
 (P/take/'video.ffconcat').write_text('\n'.join(lines))
 subprocess.run(['ffmpeg','-hide_banner','-loglevel','error','-y','-safe','0','-i',str(P/take/'video.ffconcat'),'-vf','pad=ceil(iw/2)*2:ceil(ih/2)*2','-c:v','libx264','-crf','20','-pix_fmt','yuv420p','-r','30',str(P/(name+'.mp4'))],check=True)
 report.append({'take':take,'output':name+'.mp4','frames':len(rows),'capture_hz':(len(rows)-1)*1000/(rows[-1]['time']-rows[0]['time']),'container_fps':30,'timing':'Original screenshot intervals retained; no speedup'})
(P/'video-manifest.json').write_text(json.dumps(report,indent=2));print(json.dumps(report,indent=2))
