const fs = require('node:fs');
const path = require('node:path');
const root = 'C:/Program Files (x86)/Steam/steamapps/common/Dishonored2/base';
const entries = [];
for (const name of ['game1.index','game1_patch.index','game2.index']) {
  const buffer = fs.readFileSync(path.join(root,name));
  for (const m of buffer.toString('latin1').matchAll(/[\x20-\x7e]{5,}/g)) {
    if (m[0].length > 220) continue;
    if (/\.iggy(?:tex)?\b|(?:gui|ui|menus)\/.*(?:main|character|campaign|pause|popup|notification|loading|dialog|newgame)/i.test(m[0])) {
      entries.push({catalog:name,name:m[0],offset:m.index});
    }
  }
}
const output = 'outputs/d2-coop-lab/research/native-ui-inventory.json';
fs.writeFileSync(output,JSON.stringify(entries,null,2)+'\n');
console.log(JSON.stringify({count:entries.length,examples:entries.filter(x => /main|campaign|pause|popup|notification|character|loading|newgame/i.test(x.name)).slice(0,30)},null,2));
