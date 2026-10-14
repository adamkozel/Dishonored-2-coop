# ghidra-inventory-flow.json

Original: [work/ghidra-inventory-flow.json](<../../../../work/ghidra-inventory-flow.json>)

SHA-256: `AAD805CB809E35D301B91BDF67088EC60CB45E01D16B7D4693B3D332CD4648F0`; 706 bytes.

## Original saved record

````json
{
  "code": "import ghidra.app.cmd.function.CreateFunctionCmd;\ndisassemble(toAddr(0x1406182f0L)); CreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x1406182f0L)),monitor);\ndisassemble(toAddr(0x1406170b0L)); CreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x1406170b0L)),monitor);\ndisassemble(toAddr(0x140619380L)); CreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140619380L)),monitor);\ndisassemble(toAddr(0x1406139f0L)); CreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x1406139f0L)),monitor);\nprintln(\"Inventory call-path instructions and bodies materialized\");",
  "program": "Dishonored2.exe"
}

````

## Unescaped text fields for reading

### code

````text
import ghidra.app.cmd.function.CreateFunctionCmd;
disassemble(toAddr(0x1406182f0L)); CreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x1406182f0L)),monitor);
disassemble(toAddr(0x1406170b0L)); CreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x1406170b0L)),monitor);
disassemble(toAddr(0x140619380L)); CreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x140619380L)),monitor);
disassemble(toAddr(0x1406139f0L)); CreateFunctionCmd.fixupFunctionBody(currentProgram,getFunctionAt(toAddr(0x1406139f0L)),monitor);
println("Inventory call-path instructions and bodies materialized");
````
