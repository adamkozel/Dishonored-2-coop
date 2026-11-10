# 2249: d2coop-v1.0.0-linux.zip = the released d2coop-v1.0.0 package plus the Linux installer (d2coop_linux.py,
# install-linux.sh, uninstall-linux.sh, README-LINUX.md), with Unix modes so the scripts run after unpacking.
import hashlib
import os
import shutil
import sys
import zipfile

here = os.path.dirname(os.path.abspath(__file__))
root = os.path.abspath(os.path.join(here, '..', '..', '..'))
package = os.path.join(root, 'outputs', 'release', 'd2coop-v1.0.0')
stage = os.path.join(here, 'pkg')
out = os.path.join(root, 'outputs', 'release', 'd2coop-v1.0.0-linux.zip')

shutil.rmtree(stage, ignore_errors=True)
shutil.copytree(package, os.path.join(stage, 'd2coop-v1.0.0'))
for f in ('d2coop_linux.py', 'install-linux.sh', 'uninstall-linux.sh', 'README-LINUX.md'):
    data = open(os.path.join(here, f), 'rb').read()
    assert b'\r\n' not in data, f
    shutil.copyfile(os.path.join(here, f), os.path.join(stage, 'd2coop-v1.0.0', f))
if os.path.exists(out):
    sys.exit('package exists: ' + out)
with zipfile.ZipFile(out, 'w', zipfile.ZIP_DEFLATED) as z:
    for d, dirs, files in os.walk(stage):
        dirs.sort()
        for f in sorted(files):
            p = os.path.join(d, f)
            arc = os.path.relpath(p, stage).replace(os.sep, '/')
            zi = zipfile.ZipInfo.from_file(p, arc)
            zi.compress_type = zipfile.ZIP_DEFLATED
            zi.create_system = 3
            mode = 0o755 if f.endswith(('.sh', '.py')) else 0o644
            zi.external_attr = (0o100000 | mode) << 16
            with open(p, 'rb') as fh:
                z.writestr(zi, fh.read())
print(out, os.path.getsize(out), hashlib.sha256(open(out, 'rb').read()).hexdigest().upper())
for i in zipfile.ZipFile(out).infolist():
    print(oct(i.external_attr >> 16), i.filename)
