"""Loopback-only review server with byte ranges for reliable video scrubbing."""
from http.server import SimpleHTTPRequestHandler,ThreadingHTTPServer
from pathlib import Path
import argparse,re
ROOT=Path(__file__).resolve().parent
class ReviewHandler(SimpleHTTPRequestHandler):
 def __init__(self,*args,**kwargs):super().__init__(*args,directory=str(ROOT),**kwargs)
 def send_head(self):
  self.byte_range=None
  path=Path(self.translate_path(self.path)).resolve()
  if not path.is_relative_to(ROOT):self.send_error(403);return None
  if not path.is_file():return super().send_head()
  file=path.open('rb');size=path.stat().st_size;start=0;end=size-1;partial=False
  header=self.headers.get('Range')
  if header:
   match=re.fullmatch(r'bytes=(\d*)-(\d*)',header.strip())
   if not match or not any(match.groups()):file.close();self.send_error(400,'Unsupported range');return None
   a,b=match.groups()
   if a:start=int(a);end=min(int(b),size-1)if b else size-1
   else:start=max(0,size-int(b))
   if start>=size or end<start:
    file.close();self.send_response(416);self.send_header('Content-Range',f'bytes */{size}');self.send_header('Content-Length','0');self.end_headers();return None
   partial=True;self.byte_range=(start,end);file.seek(start)
  self.send_response(206 if partial else 200);self.send_header('Content-Type',self.guess_type(str(path)));self.send_header('Accept-Ranges','bytes');self.send_header('Content-Length',str(end-start+1));self.send_header('Cache-Control','no-cache')
  if partial:self.send_header('Content-Range',f'bytes {start}-{end}/{size}')
  self.end_headers();return file
 def copyfile(self,source,outputfile):
  if self.byte_range is None:return super().copyfile(source,outputfile)
  remaining=self.byte_range[1]-self.byte_range[0]+1
  while remaining:
   block=source.read(min(65536,remaining))
   if not block:break
   outputfile.write(block);remaining-=len(block)
 def log_message(self,fmt,*args):
  if len(args)>1 and str(args[1])not in ['200','206','304']:super().log_message(fmt,*args)
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--port',type=int,default=8769);args=p.parse_args()
 print(f'Review library: http://127.0.0.1:{args.port}/review.html',flush=True)
 ThreadingHTTPServer(('127.0.0.1',args.port),ReviewHandler).serve_forever()
