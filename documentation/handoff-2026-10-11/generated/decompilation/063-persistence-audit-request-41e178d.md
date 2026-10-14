# persistence-audit-request.json

Original: [outputs/d2-coop-lab/analysis/ghidra/persistence-audit-request.json](<../../../../outputs/d2-coop-lab/analysis/ghidra/persistence-audit-request.json>)

SHA-256: `E2D3859ED3C0AFA2E335F66661C0F6FBAA7C062C7F4BFB673806BE2466CEBDEA`; 1087 bytes.

## Original saved record

````json
{
  "program": "Dishonored2.exe",
  "code": "import ghidra.program.model.data.*;\nprintln(\"HASH=\"+currentProgram.getExecutableSHA256());\nprintln(\"BASE=\"+currentProgram.getImageBase());\nprintln(\"LANG=\"+currentProgram.getLanguageID());\nprintln(\"FUNCTION=\"+getFunctionAt(toAddr(0x140717d50L)).getPrototypeString(false,false));\nprintln(\"BODY=\"+getFunctionAt(toAddr(0x140717d50L)).getBody());\nString[] names={\"D2_ComponentPartial\",\"D2_HealthPartial\",\"D2_InventoryEntryPartial\",\"D2_InventoryPartial\",\"D2_EntityPartial\",\"D2_ReferenceFramePartial\",\"D2_AnimationComponentPartial\"};\nfor(String name:names){DataType d=currentProgram.getDataTypeManager().getDataType(\"/D2Coop/ConfirmedPartial/\"+name);if(d==null)throw new Exception(\"Missing type \"+name);println(\"TYPE=\"+name+\" MIN_VIEW=\"+d.getLength());}\nString hypotheses=currentProgram.getOptions(\"D2Coop Research\").getString(\"Unverified hypotheses\",\"\");if(hypotheses.isEmpty())throw new Exception(\"Missing hypotheses\");println(\"HYPOTHESES_PRESERVED=true\");\nprintln(\"D2_SETUP_VERIFIED\");"
}

````

## Unescaped text fields for reading

### code

````text
import ghidra.program.model.data.*;
println("HASH="+currentProgram.getExecutableSHA256());
println("BASE="+currentProgram.getImageBase());
println("LANG="+currentProgram.getLanguageID());
println("FUNCTION="+getFunctionAt(toAddr(0x140717d50L)).getPrototypeString(false,false));
println("BODY="+getFunctionAt(toAddr(0x140717d50L)).getBody());
String[] names={"D2_ComponentPartial","D2_HealthPartial","D2_InventoryEntryPartial","D2_InventoryPartial","D2_EntityPartial","D2_ReferenceFramePartial","D2_AnimationComponentPartial"};
for(String name:names){DataType d=currentProgram.getDataTypeManager().getDataType("/D2Coop/ConfirmedPartial/"+name);if(d==null)throw new Exception("Missing type "+name);println("TYPE="+name+" MIN_VIEW="+d.getLength());}
String hypotheses=currentProgram.getOptions("D2Coop Research").getString("Unverified hypotheses","");if(hypotheses.isEmpty())throw new Exception("Missing hypotheses");println("HYPOTHESES_PRESERVED=true");
println("D2_SETUP_VERIFIED");
````
