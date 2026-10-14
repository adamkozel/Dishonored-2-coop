"""Extract a reproducible, labelled temporal sheet from a completed review video."""
import argparse, subprocess, io
from pathlib import Path
from PIL import Image, ImageDraw

p = argparse.ArgumentParser()
p.add_argument('video'); p.add_argument('output'); p.add_argument('times', nargs='+', type=float)
a = p.parse_args()
video = Path(a.video).resolve(); output = Path(a.output).resolve()
duration = float(subprocess.check_output(['ffprobe', '-v', 'error', '-show_entries', 'format=duration', '-of', 'default=noprint_wrappers=1:nokey=1', str(video)]))
frames = []
for i, t in enumerate(a.times):
    t = max(0, min(t, duration - .05))
    result = subprocess.run(['ffmpeg', '-v', 'error', '-ss', str(t), '-i', str(video), '-frames:v', '1', '-f', 'image2pipe', '-vcodec', 'png', '-'], check=True, capture_output=True)
    frame = Image.open(io.BytesIO(result.stdout)).convert('RGB'); frame.thumbnail((800, 450))
    tile = Image.new('RGB', (800, 478), '#171717'); tile.paste(frame, (0, 28))
    ImageDraw.Draw(tile).text((12, 8), f'{video.stem}  |  {t:.2f}s', fill='white')
    frames.append(tile)
sheet = Image.new('RGB', (1600, 478 * ((len(frames) + 1) // 2)), '#171717')
for i, tile in enumerate(frames): sheet.paste(tile, ((i % 2) * 800, (i // 2) * 478))
sheet.save(output, quality=92)
print(output)
