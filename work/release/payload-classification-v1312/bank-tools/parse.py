import struct, sys, hashlib, os
def parse(path, want_hash=True):
    size=os.path.getsize(path)
    with open(path,'rb') as f:
        h=f.read(32)
        magic,v,n,count,fps,lay=struct.unpack('<8sIIIIQ',h)
        clips=[]
        off=32
        for i in range(count):
            ch=f.read(24)
            cid,frames,a,b,f1,f2=struct.unpack('<IIIIff',ch)
            ln=frames*n*28
            data=f.read(ln) if want_hash else None
            if not want_hash: f.seek(ln,1)
            clips.append(dict(i=i,cid=cid,frames=frames,a=a,b=b,f1=f1,f2=f2,off=off,
                              ph=hashlib.sha1(data).hexdigest() if want_hash else None,
                              hh=hashlib.sha1(ch).hexdigest()))
            off+=24+ln
        tail=f.read()
    return dict(magic=magic,v=v,n=n,count=count,fps=fps,layout=lay,clips=clips,size=size,end=off,tail=len(tail))
if __name__=='__main__':
    r=parse(sys.argv[1], want_hash=False)
    print(r['magic'],r['v'],r['n'],r['count'],r['fps'],r['layout'],r['size'],r['end'],r['tail'])
    import collections
    print(collections.Counter(c['frames'] for c in r['clips']))
    print(collections.Counter((c['a'],c['b']) for c in r['clips']))
    print(all(c['cid']==c['i'] for c in r['clips']))
    for c in r['clips'][:5]+r['clips'][-6:]: print(c)
