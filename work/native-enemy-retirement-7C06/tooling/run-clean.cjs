const {spawnSync} = require('node:child_process');
// Windows environment keys are case-insensitive. MSBuild rejects duplicate
// Path/PATH entries inherited from some task hosts; normalize only the child.
const environment = {};
for (const [key,value] of Object.entries(process.env)) environment[key.toUpperCase()] = value;
const result = spawnSync(process.argv[2], process.argv.slice(3), {env:environment,stdio:'inherit',shell:false});
if (result.error) throw result.error;
process.exitCode = result.status ?? 1;
