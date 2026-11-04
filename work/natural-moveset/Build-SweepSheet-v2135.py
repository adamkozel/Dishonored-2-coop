# 2135 (S4, #264): a contact sheet of both screens at each stage of a sweep take. Each tile is the host frame (left)
# and the VM's game window cropped from its desktop screen (right), labelled with the step. Stage steps: the diffs,
# the partner's fight and loot, and the end status.
# Usage: python Build-SweepSheet-v2135.py <take folder> <out png> (outside the take: takes are evidence)
import pathlib
import re
import subprocess
import sys

take = pathlib.Path(sys.argv[1])
out = pathlib.Path(sys.argv[2])
font = "C\\:/Windows/Fonts/arial.ttf"
steps = []
for host in sorted(take.glob("step*-host.png")):
    m = re.match(r"(step\d\d-([a-z]+))-host\.png$", host.name)
    if not m or m.group(2) not in ("diff", "partnerattack", "partnerloot", "status", "talk", "die"):
        continue
    vm = take / (m.group(1) + "-vm.png")
    if vm.exists():
        steps.append((m.group(1), host, vm))
if not steps:
    sys.exit("no paired stage frames in " + str(take))
tiles = []
work = out.parent / (take.name + "-tiles")
work.mkdir(exist_ok=True)
for name, host, vm in steps:
    tile = work / (name + ".png")
    # The VM screen is the 1280x720 desktop; the game window's client area is at 648,27 (628x358).
    graph = ("[0:v]scale=480:324,setsar=1[h];[1:v]crop=628:358:648:27,scale=568:324,setsar=1[v];"
             "[h][v]hstack=inputs=2,drawtext=fontfile='" + font + "':text='" + name + "':x=8:y=8:fontsize=22:fontcolor=yellow:box=1:boxcolor=black@0.6[o]")
    subprocess.run(["ffmpeg", "-hide_banner", "-loglevel", "error", "-y", "-i", str(host), "-i", str(vm),
                    "-filter_complex", graph, "-map", "[o]", "-frames:v", "1", str(tile)], check=True)
    tiles.append(tile)
cols = 2
rows = (len(tiles) + cols - 1) // cols
inputs = []
for t in tiles:
    inputs += ["-i", str(t)]
blank = len(tiles) % cols
graph = ""
labels = ["[%d:v]" % i for i in range(len(tiles))]
if blank:
    graph += "color=c=black:s=1048x324:d=1[pad];"
    labels.append("[pad]")
layout = "|".join("%d_%d" % ((i % cols) * 1048, (i // cols) * 324) for i in range(len(labels)))
graph += "".join(labels) + "xstack=inputs=%d:layout=%s:fill=black[o]" % (len(labels), layout)
subprocess.run(["ffmpeg", "-hide_banner", "-loglevel", "error", "-y"] + inputs +
               ["-filter_complex", graph, "-map", "[o]", "-frames:v", "1", str(out)], check=True)
print(out, len(tiles), "stages")
