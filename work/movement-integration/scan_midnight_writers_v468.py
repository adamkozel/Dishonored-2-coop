from pathlib import Path
import json
source=Path(__file__).with_name('scan_midnight_flags_v458.py').read_text(encoding='utf-8')
source=source.replace('(0x2b2,0x2b3,0x2b4,0x2b5)', '(0x27c,0x2a8)')
source=source.replace('midnight-flag-sites-v458.json','midnight-writer-sites-v468.json')
namespace={'__file__':__file__}
exec(compile(source, __file__, 'exec'), namespace)
