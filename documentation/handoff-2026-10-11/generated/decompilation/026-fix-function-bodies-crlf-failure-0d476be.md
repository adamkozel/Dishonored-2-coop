# fix-function-bodies-crlf-failure.json

Original: [outputs/d2-coop-lab/analysis/ghidra/fix-function-bodies-crlf-failure.json](<../../../../outputs/d2-coop-lab/analysis/ghidra/fix-function-bodies-crlf-failure.json>)

SHA-256: `2A714A232F7B35618F2DCD69C4485CD34F8BBF017670159DB5C20C518E757B85`; 9329 bytes.

## Original saved record

````json
{
  "meta": null,
  "content": [
    {
      "type": "text",
      "text": "=== GHIDRA SCRIPT EXECUTION ===\nScript: C:\\Users\\adako\\Documents\\Codex\\2026-10-08\\i-x20\\work\\tools\\ghidra-runtime\\ghidra_scripts\\McpInline_4a7149e1eb64.java\nProgram: Dishonored2.exe\nTime: Fri Oct 09 07:47:21 CEST 2026\n\nFound script: C:\\Users\\adako\\Documents\\Codex\\2026-10-08\\i-x20\\work\\tools\\ghidra-runtime\\ghidra_scripts\\McpInline_4a7149e1eb64.java\nSize: 3606 bytes\n\nScript provider: JavaScriptProvider\n\n--- SCRIPT BUILD OUTPUT ---\nMcpInline_4a7149e1eb64.java:5: error: illegal start of expression\nimport ghidra.app.cmd.function.CreateFunctionCmd;\n^\nMcpInline_4a7149e1eb64.java:5: error: not a statement\nimport ghidra.app.cmd.function.CreateFunctionCmd;\n                              ^\nskipping C:\\Users\\adako\\Documents\\Codex\\2026-10-08\\i-x20\\work\\tools\\ghidra-runtime\\ghidra_scripts\\McpInline_4a7149e1eb64.java\n\n\n=== SCRIPT EXECUTION ERROR ===\nError: GhidraScriptLoadException: The class could not be found. It must be the public class of the .java file: McpInline_4a7149e1eb64 not found by c62890c1 [1]\nStack trace:\nghidra.app.script.GhidraScriptLoadException: The class could not be found. It must be the public class of the .java file: McpInline_4a7149e1eb64 not found by c62890c1 [1]\r\n\tat ghidra.app.script.JavaScriptProvider.getScriptInstance(JavaScriptProvider.java:114)\r\n\tat com.xebyte.core.ProgramScriptService.lambda$runGhidraScript$11(ProgramScriptService.java:2038)\r\n\tat java.desktop/java.awt.event.InvocationEvent.dispatch(InvocationEvent.java:308)\r\n\tat java.desktop/java.awt.EventQueue.dispatchEventImpl(EventQueue.java:773)\r\n\tat java.desktop/java.awt.EventQueue$4.run(EventQueue.java:720)\r\n\tat java.desktop/java.awt.EventQueue$4.run(EventQueue.java:714)\r\n\tat java.base/java.security.AccessController.doPrivileged(AccessController.java:400)\r\n\tat java.base/java.security.ProtectionDomain$JavaSecurityAccessImpl.doIntersectionPrivilege(ProtectionDomain.java:87)\r\n\tat java.desktop/java.awt.EventQueue.dispatchEvent(EventQueue.java:742)\r\n\tat java.desktop/java.awt.EventDispatchThread.pumpOneEventForFilters(EventDispatchThread.java:203)\r\n\tat java.desktop/java.awt.EventDispatchThread.pumpEventsForFilter(EventDispatchThread.java:124)\r\n\tat java.desktop/java.awt.EventDispatchThread.pumpEventsForHierarchy(EventDispatchThread.java:113)\r\n\tat java.desktop/java.awt.EventDispatchThread.pumpEvents(EventDispatchThread.java:109)\r\n\tat java.desktop/java.awt.EventDispatchThread.pumpEvents(EventDispatchThread.java:101)\r\n\tat java.desktop/java.awt.EventDispatchThread.run(EventDispatchThread.java:90)\r\nCaused by: java.lang.ClassNotFoundException: McpInline_4a7149e1eb64 not found by c62890c1 [1]\r\n\tat org.apache.felix.framework.BundleWiringImpl.findClassOrResourceByDelegation(BundleWiringImpl.java:1591)\r\n\tat org.apache.felix.framework.BundleWiringImpl.access$300(BundleWiringImpl.java:79)\r\n\tat org.apache.felix.framework.BundleWiringImpl$BundleClassLoader.loadClass(BundleWiringImpl.java:1976)\r\n\tat java.base/java.lang.ClassLoader.loadClass(ClassLoader.java:526)\r\n\tat org.apache.felix.framework.Felix.loadBundleClass(Felix.java:2116)\r\n\tat org.apache.felix.framework.BundleImpl.loadClass(BundleImpl.java:986)\r\n\tat ghidra.app.script.JavaScriptProvider.loadClass(JavaScriptProvider.java:166)\r\n\tat ghidra.app.script.JavaScriptProvider.getScriptInstance(JavaScriptProvider.java:97)\r\n\t... 14 more\r\n\n--- BUILD/ACTIVATE OUTPUT ---\nMcpInline_4a7149e1eb64.java:5: error: illegal start of expression\nimport ghidra.app.cmd.function.CreateFunctionCmd;\n^\nMcpInline_4a7149e1eb64.java:5: error: not a statement\nimport ghidra.app.cmd.function.CreateFunctionCmd;\n                              ^\nskipping C:\\Users\\adako\\Documents\\Codex\\2026-10-08\\i-x20\\work\\tools\\ghidra-runtime\\ghidra_scripts\\McpInline_4a7149e1eb64.java\n\n\n--- CONSOLE OUTPUT ---\nSLF4J: Class path contains multiple SLF4J bindings.\r\nSLF4J: Found binding in [jar:file:/C:/Users/adako/Documents/Codex/2026-10-08/i-x20/work/tools/ghidra_12.1.4_PUBLIC/Ghidra/Features/Base/lib/slf4j-nop-1.7.25.jar!/org/slf4j/impl/StaticLoggerBinder.class]\r\nSLF4J: Found binding in [jar:file:/C:/Users/adako/Documents/Codex/2026-10-08/i-x20/work/tools/ghidra_12.1.4_PUBLIC/Ghidra/Features/GhidraServer/data/yajsw-stable-13.18/lib/extended/slf4j/slf4j-simple-1.7.30.jar!/org/slf4j/impl/StaticLoggerBinder.class]\r\nSLF4J: See http://www.slf4j.org/codes.html#multiple_bindings for an explanation.\r\nSLF4J: Actual binding is of type [org.slf4j.helpers.NOPLoggerFactory]",
      "annotations": null,
      "meta": null
    }
  ],
  "structuredContent": {
    "result": "=== GHIDRA SCRIPT EXECUTION ===\nScript: C:\\Users\\adako\\Documents\\Codex\\2026-10-08\\i-x20\\work\\tools\\ghidra-runtime\\ghidra_scripts\\McpInline_4a7149e1eb64.java\nProgram: Dishonored2.exe\nTime: Fri Oct 09 07:47:21 CEST 2026\n\nFound script: C:\\Users\\adako\\Documents\\Codex\\2026-10-08\\i-x20\\work\\tools\\ghidra-runtime\\ghidra_scripts\\McpInline_4a7149e1eb64.java\nSize: 3606 bytes\n\nScript provider: JavaScriptProvider\n\n--- SCRIPT BUILD OUTPUT ---\nMcpInline_4a7149e1eb64.java:5: error: illegal start of expression\nimport ghidra.app.cmd.function.CreateFunctionCmd;\n^\nMcpInline_4a7149e1eb64.java:5: error: not a statement\nimport ghidra.app.cmd.function.CreateFunctionCmd;\n                              ^\nskipping C:\\Users\\adako\\Documents\\Codex\\2026-10-08\\i-x20\\work\\tools\\ghidra-runtime\\ghidra_scripts\\McpInline_4a7149e1eb64.java\n\n\n=== SCRIPT EXECUTION ERROR ===\nError: GhidraScriptLoadException: The class could not be found. It must be the public class of the .java file: McpInline_4a7149e1eb64 not found by c62890c1 [1]\nStack trace:\nghidra.app.script.GhidraScriptLoadException: The class could not be found. It must be the public class of the .java file: McpInline_4a7149e1eb64 not found by c62890c1 [1]\r\n\tat ghidra.app.script.JavaScriptProvider.getScriptInstance(JavaScriptProvider.java:114)\r\n\tat com.xebyte.core.ProgramScriptService.lambda$runGhidraScript$11(ProgramScriptService.java:2038)\r\n\tat java.desktop/java.awt.event.InvocationEvent.dispatch(InvocationEvent.java:308)\r\n\tat java.desktop/java.awt.EventQueue.dispatchEventImpl(EventQueue.java:773)\r\n\tat java.desktop/java.awt.EventQueue$4.run(EventQueue.java:720)\r\n\tat java.desktop/java.awt.EventQueue$4.run(EventQueue.java:714)\r\n\tat java.base/java.security.AccessController.doPrivileged(AccessController.java:400)\r\n\tat java.base/java.security.ProtectionDomain$JavaSecurityAccessImpl.doIntersectionPrivilege(ProtectionDomain.java:87)\r\n\tat java.desktop/java.awt.EventQueue.dispatchEvent(EventQueue.java:742)\r\n\tat java.desktop/java.awt.EventDispatchThread.pumpOneEventForFilters(EventDispatchThread.java:203)\r\n\tat java.desktop/java.awt.EventDispatchThread.pumpEventsForFilter(EventDispatchThread.java:124)\r\n\tat java.desktop/java.awt.EventDispatchThread.pumpEventsForHierarchy(EventDispatchThread.java:113)\r\n\tat java.desktop/java.awt.EventDispatchThread.pumpEvents(EventDispatchThread.java:109)\r\n\tat java.desktop/java.awt.EventDispatchThread.pumpEvents(EventDispatchThread.java:101)\r\n\tat java.desktop/java.awt.EventDispatchThread.run(EventDispatchThread.java:90)\r\nCaused by: java.lang.ClassNotFoundException: McpInline_4a7149e1eb64 not found by c62890c1 [1]\r\n\tat org.apache.felix.framework.BundleWiringImpl.findClassOrResourceByDelegation(BundleWiringImpl.java:1591)\r\n\tat org.apache.felix.framework.BundleWiringImpl.access$300(BundleWiringImpl.java:79)\r\n\tat org.apache.felix.framework.BundleWiringImpl$BundleClassLoader.loadClass(BundleWiringImpl.java:1976)\r\n\tat java.base/java.lang.ClassLoader.loadClass(ClassLoader.java:526)\r\n\tat org.apache.felix.framework.Felix.loadBundleClass(Felix.java:2116)\r\n\tat org.apache.felix.framework.BundleImpl.loadClass(BundleImpl.java:986)\r\n\tat ghidra.app.script.JavaScriptProvider.loadClass(JavaScriptProvider.java:166)\r\n\tat ghidra.app.script.JavaScriptProvider.getScriptInstance(JavaScriptProvider.java:97)\r\n\t... 14 more\r\n\n--- BUILD/ACTIVATE OUTPUT ---\nMcpInline_4a7149e1eb64.java:5: error: illegal start of expression\nimport ghidra.app.cmd.function.CreateFunctionCmd;\n^\nMcpInline_4a7149e1eb64.java:5: error: not a statement\nimport ghidra.app.cmd.function.CreateFunctionCmd;\n                              ^\nskipping C:\\Users\\adako\\Documents\\Codex\\2026-10-08\\i-x20\\work\\tools\\ghidra-runtime\\ghidra_scripts\\McpInline_4a7149e1eb64.java\n\n\n--- CONSOLE OUTPUT ---\nSLF4J: Class path contains multiple SLF4J bindings.\r\nSLF4J: Found binding in [jar:file:/C:/Users/adako/Documents/Codex/2026-10-08/i-x20/work/tools/ghidra_12.1.4_PUBLIC/Ghidra/Features/Base/lib/slf4j-nop-1.7.25.jar!/org/slf4j/impl/StaticLoggerBinder.class]\r\nSLF4J: Found binding in [jar:file:/C:/Users/adako/Documents/Codex/2026-10-08/i-x20/work/tools/ghidra_12.1.4_PUBLIC/Ghidra/Features/GhidraServer/data/yajsw-stable-13.18/lib/extended/slf4j/slf4j-simple-1.7.30.jar!/org/slf4j/impl/StaticLoggerBinder.class]\r\nSLF4J: See http://www.slf4j.org/codes.html#multiple_bindings for an explanation.\r\nSLF4J: Actual binding is of type [org.slf4j.helpers.NOPLoggerFactory]"
  },
  "isError": false
}

````

## Unescaped text fields for reading

### text

````text
=== GHIDRA SCRIPT EXECUTION ===
Script: C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra-runtime\ghidra_scripts\McpInline_4a7149e1eb64.java
Program: Dishonored2.exe
Time: Fri Oct 09 07:47:21 CEST 2026

Found script: C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra-runtime\ghidra_scripts\McpInline_4a7149e1eb64.java
Size: 3606 bytes

Script provider: JavaScriptProvider

--- SCRIPT BUILD OUTPUT ---
McpInline_4a7149e1eb64.java:5: error: illegal start of expression
import ghidra.app.cmd.function.CreateFunctionCmd;
^
McpInline_4a7149e1eb64.java:5: error: not a statement
import ghidra.app.cmd.function.CreateFunctionCmd;
                              ^
skipping C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra-runtime\ghidra_scripts\McpInline_4a7149e1eb64.java


=== SCRIPT EXECUTION ERROR ===
Error: GhidraScriptLoadException: The class could not be found. It must be the public class of the .java file: McpInline_4a7149e1eb64 not found by c62890c1 [1]
Stack trace:
ghidra.app.script.GhidraScriptLoadException: The class could not be found. It must be the public class of the .java file: McpInline_4a7149e1eb64 not found by c62890c1 [1]
	at ghidra.app.script.JavaScriptProvider.getScriptInstance(JavaScriptProvider.java:114)
	at com.xebyte.core.ProgramScriptService.lambda$runGhidraScript$11(ProgramScriptService.java:2038)
	at java.desktop/java.awt.event.InvocationEvent.dispatch(InvocationEvent.java:308)
	at java.desktop/java.awt.EventQueue.dispatchEventImpl(EventQueue.java:773)
	at java.desktop/java.awt.EventQueue$4.run(EventQueue.java:720)
	at java.desktop/java.awt.EventQueue$4.run(EventQueue.java:714)
	at java.base/java.security.AccessController.doPrivileged(AccessController.java:400)
	at java.base/java.security.ProtectionDomain$JavaSecurityAccessImpl.doIntersectionPrivilege(ProtectionDomain.java:87)
	at java.desktop/java.awt.EventQueue.dispatchEvent(EventQueue.java:742)
	at java.desktop/java.awt.EventDispatchThread.pumpOneEventForFilters(EventDispatchThread.java:203)
	at java.desktop/java.awt.EventDispatchThread.pumpEventsForFilter(EventDispatchThread.java:124)
	at java.desktop/java.awt.EventDispatchThread.pumpEventsForHierarchy(EventDispatchThread.java:113)
	at java.desktop/java.awt.EventDispatchThread.pumpEvents(EventDispatchThread.java:109)
	at java.desktop/java.awt.EventDispatchThread.pumpEvents(EventDispatchThread.java:101)
	at java.desktop/java.awt.EventDispatchThread.run(EventDispatchThread.java:90)
Caused by: java.lang.ClassNotFoundException: McpInline_4a7149e1eb64 not found by c62890c1 [1]
	at org.apache.felix.framework.BundleWiringImpl.findClassOrResourceByDelegation(BundleWiringImpl.java:1591)
	at org.apache.felix.framework.BundleWiringImpl.access$300(BundleWiringImpl.java:79)
	at org.apache.felix.framework.BundleWiringImpl$BundleClassLoader.loadClass(BundleWiringImpl.java:1976)
	at java.base/java.lang.ClassLoader.loadClass(ClassLoader.java:526)
	at org.apache.felix.framework.Felix.loadBundleClass(Felix.java:2116)
	at org.apache.felix.framework.BundleImpl.loadClass(BundleImpl.java:986)
	at ghidra.app.script.JavaScriptProvider.loadClass(JavaScriptProvider.java:166)
	at ghidra.app.script.JavaScriptProvider.getScriptInstance(JavaScriptProvider.java:97)
	... 14 more

--- BUILD/ACTIVATE OUTPUT ---
McpInline_4a7149e1eb64.java:5: error: illegal start of expression
import ghidra.app.cmd.function.CreateFunctionCmd;
^
McpInline_4a7149e1eb64.java:5: error: not a statement
import ghidra.app.cmd.function.CreateFunctionCmd;
                              ^
skipping C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra-runtime\ghidra_scripts\McpInline_4a7149e1eb64.java


--- CONSOLE OUTPUT ---
SLF4J: Class path contains multiple SLF4J bindings.
SLF4J: Found binding in [jar:file:/C:/Users/adako/Documents/Codex/2026-10-08/i-x20/work/tools/ghidra_12.1.4_PUBLIC/Ghidra/Features/Base/lib/slf4j-nop-1.7.25.jar!/org/slf4j/impl/StaticLoggerBinder.class]
SLF4J: Found binding in [jar:file:/C:/Users/adako/Documents/Codex/2026-10-08/i-x20/work/tools/ghidra_12.1.4_PUBLIC/Ghidra/Features/GhidraServer/data/yajsw-stable-13.18/lib/extended/slf4j/slf4j-simple-1.7.30.jar!/org/slf4j/impl/StaticLoggerBinder.class]
SLF4J: See http://www.slf4j.org/codes.html#multiple_bindings for an explanation.
SLF4J: Actual binding is of type [org.slf4j.helpers.NOPLoggerFactory]
````
