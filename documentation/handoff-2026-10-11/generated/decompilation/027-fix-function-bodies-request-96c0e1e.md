# fix-function-bodies-request.json

Original: [outputs/d2-coop-lab/analysis/ghidra/fix-function-bodies-request.json](<../../../../outputs/d2-coop-lab/analysis/ghidra/fix-function-bodies-request.json>)

SHA-256: `1F6FD68BE36A80801C176D9D03EE50F5F11AE273226FDCAF0BEE3A662ACB776A`; 3563 bytes.

## Original saved record

````json
{
  "code": "import ghidra.app.cmd.function.CreateFunctionCmd;\r\nCreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0xff8f0L)),monitor);\r\nCreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x48ad0L)),monitor);\r\nCreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x753c10L)),monitor);\r\nCreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x540f20L)),monitor);\r\nCreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x9cfa0L)),monitor);\r\nCreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x9dc40L)),monitor);\r\nCreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x23cb00L)),monitor);\r\nCreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x1d9090L)),monitor);\r\nCreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x717950L)),monitor);\r\nCreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x7185b0L)),monitor);\r\nCreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x717d50L)),monitor);\r\nCreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x7197f0L)),monitor);\r\nCreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x71a9e0L)),monitor);\r\nCreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x780260L)),monitor);\r\nCreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x780470L)),monitor);\r\nCreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x614b20L)),monitor);\r\nCreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x619640L)),monitor);\r\nCreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x1599460L)),monitor);\r\nCreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x7e7800L)),monitor);\r\nCreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x7e79c0L)),monitor);\r\nCreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x87d200L)),monitor);\r\nCreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x8a78f0L)),monitor);\r\nCreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x8a7650L)),monitor);\r\nCreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x8ab380L)),monitor);\r\nCreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x4f2980L)),monitor);\r\nCreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x78a5b0L)),monitor);\r\nCreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x7d71f0L)),monitor);\r\nCreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x7d8950L)),monitor);\r\nCreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x605990L)),monitor);\r\nCreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x6060a0L)),monitor);\r\nCreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x16fec0L)),monitor);\r\nprintln(\"Targeted function bodies rebuilt: 31\");",
  "program": "Dishonored2.exe"
}

````

## Unescaped text fields for reading

### code

````text
import ghidra.app.cmd.function.CreateFunctionCmd;
CreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0xff8f0L)),monitor);
CreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x48ad0L)),monitor);
CreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x753c10L)),monitor);
CreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x540f20L)),monitor);
CreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x9cfa0L)),monitor);
CreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x9dc40L)),monitor);
CreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x23cb00L)),monitor);
CreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x1d9090L)),monitor);
CreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x717950L)),monitor);
CreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x7185b0L)),monitor);
CreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x717d50L)),monitor);
CreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x7197f0L)),monitor);
CreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x71a9e0L)),monitor);
CreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x780260L)),monitor);
CreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x780470L)),monitor);
CreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x614b20L)),monitor);
CreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x619640L)),monitor);
CreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x1599460L)),monitor);
CreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x7e7800L)),monitor);
CreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x7e79c0L)),monitor);
CreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x87d200L)),monitor);
CreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x8a78f0L)),monitor);
CreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x8a7650L)),monitor);
CreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x8ab380L)),monitor);
CreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x4f2980L)),monitor);
CreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x78a5b0L)),monitor);
CreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x7d71f0L)),monitor);
CreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x7d8950L)),monitor);
CreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x605990L)),monitor);
CreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x6060a0L)),monitor);
CreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140000000L+0x16fec0L)),monitor);
println("Targeted function bodies rebuilt: 31");
````
