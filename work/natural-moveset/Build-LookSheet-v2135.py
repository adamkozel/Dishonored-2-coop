# 2135 (S4, #264): a contact sheet of a -LookStages take. One row per stage: the host's three views of the partner
# (look-<stage>-host-<n>-host.png), then the partner's three views of the host (look-<stage>-client-<n>-vm.png, the
# VM's game window cropped from its desktop screen). A missing frame is a black tile.
# Usage: python Build-LookSheet-v2135.py <take folder> <out png> (outside the take: takes are evidence)
import pathlib
import re
import subprocess
import sys

take = pathlib.Path(sys.argv[1])
out = pathlib.Path(sys.argv[2])
font = "C\\:/Windows/Fonts/arial.ttf"
W, H = 400, 240
stages = []
for f in sorted(take.glob("look-*-host-1-host.png"), key=lambda p: p.stat().st_mtime):
    stages.append(re.match(r"look-(.+)-host-1-host\.png$", f.name).group(1))
if not stages:
    sys.exit("no look frames in " + str(take))
work = out.parent / (take.name + "-looks")
work.mkdir(exist_ok=True)
tiles = []
for stage in stages:
    for viewer, suffix, crop in (("host", "host", ""), ("client", "vm", "crop=628:358:648:27,")):
        for n in (1, 2, 3):
            src = take / ("look-%s-%s-%d-%s.png" % (stage, viewer, n, suffix))
            tile = work / ("%s-%s-%d.png" % (stage, viewer, n))
            label = "%s %s %d" % (stage, "host sees partner" if viewer == "host" else "partner sees host", n)
            text = "drawtext=fontfile='" + font + "':text='" + label + "':x=6:y=6:fontsize=16:fontcolor=yellow:box=1:boxcolor=black@0.6"
            if src.exists():
                args = ["-i", str(src), "-vf", crop + "scale=%d:%d,setsar=1,%s" % (W, H, text)]
            else:
                args = ["-f", "lavfi", "-i", "color=c=black:s=%dx%d:d=1" % (W, H), "-vf", text]
            subprocess.run(["ffmpeg", "-hide_banner", "-loglevel", "error", "-y"] + args + ["-frames:v", "1", str(tile)], check=True)
            tiles.append(tile)
inputs = []
for t in tiles:
    inputs += ["-i", str(t)]
layout = "|".join("%d_%d" % ((i % 6) * W, (i // 6) * H) for i in range(len(tiles)))
graph = "".join("[%d:v]" % i for i in range(len(tiles))) + "xstack=inputs=%d:layout=%s:fill=black[o]" % (len(tiles), layout)
subprocess.run(["ffmpeg", "-hide_banner", "-loglevel", "error", "-y"] + inputs +
               ["-filter_complex", graph, "-map", "[o]", "-frames:v", "1", str(out)], check=True)
print(out, len(stages), "stages")
