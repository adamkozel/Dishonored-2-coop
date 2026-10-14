# application.log

Original: [work/research/zikq-d2coop-0.1/runtime/settings/ghidra/ghidra_12.1.4_PUBLIC/application.log](<../../../../work/research/zikq-d2coop-0.1/runtime/settings/ghidra/ghidra_12.1.4_PUBLIC/application.log>)

SHA-256: `A63C4C4EBB14CC77FDBF7A2A299705D4C78CF56503C1280D0BEF757A31AEFA7F`; 34675 bytes.

## Original saved record

````text
2026-10-13 00:57:12 INFO  (LoggingInitialization) Using log config file: jar:file:/C:/Users/adako/Documents/Codex/2026-10-08/i-x20/work/tools/ghidra_12.1.4_PUBLIC/Ghidra/Framework/Generic/lib/Generic.jar!/generic.log4j.xml  
2026-10-13 00:57:12 INFO  (LoggingInitialization) Using log file: C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\research\zikq-d2coop-0.1\runtime\settings\ghidra\ghidra_12.1.4_PUBLIC\application.log  
2026-10-13 00:57:12 DEBUG (GhidraObjectInputFilter) Including serial input filter: FileSystem/data/client.rmi.serial.filter  
2026-10-13 00:57:12 INFO  (Preferences) Loading user preferences: C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\research\zikq-d2coop-0.1\runtime\settings\ghidra\ghidra_12.1.4_PUBLIC\preferences  
2026-10-13 00:57:12 INFO  (ClassSearcher) Searching for classes...  
2026-10-13 00:57:13 INFO  (ClassSearcher) Class search complete (669 ms)  
2026-10-13 00:57:13 INFO  (DefaultSSLContextInitializer) Initializing SSL Context  
2026-10-13 00:57:13 INFO  (SecureRandomFactory) Initializing Random Number Generator...  
2026-10-13 00:57:13 INFO  (SecureRandomFactory) Random Number Generator initialization complete: SHA1PRNG  
2026-10-13 00:57:13 INFO  (DefaultTrustManagerFactory) Trust manager disabled, cacerts have not been set  
2026-10-13 00:57:13 INFO  (FileCacheMaintenanceDaemon) Starting cache cleanup: C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\research\zikq-d2coop-0.1\runtime\cache\ghidra\fscache2  
2026-10-13 00:57:13 INFO  (FileCacheMaintenanceDaemon) Finished cache cleanup, estimated storage used: 0  
2026-10-13 00:57:14 INFO  (AnalyzeHeadless) Headless startup complete (6036 ms)  
2026-10-13 00:57:14 INFO  (ClassSearcher) Class searcher loaded 59 extension points (20 false positives)  
2026-10-13 00:57:14 INFO  (HeadlessAnalyzer) HEADLESS Script Paths:
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Features\WildcardAssembler\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Debug\Debugger\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Processors\Atmel\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Processors\DATA\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Features\BytePatterns\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Features\DecompilerDependent\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Features\PDB\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Features\Base\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Features\FunctionID\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Features\MicrosoftCodeAnalyzer\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Processors\JVM\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Features\PyGhidra\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Features\SwiftDemangler\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Features\SystemEmulation\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Features\BSim\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Features\GnuDemangler\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Debug\Debugger-rmi-trace\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Features\FileFormats\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\research\zikq-d2coop-0.1
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Processors\8051\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Features\VersionTracking\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Processors\PIC\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Features\Decompiler\ghidra_scripts  
2026-10-13 00:57:14 INFO  (HeadlessAnalyzer) HEADLESS: execution starts  
2026-10-13 00:57:14 INFO  (HeadlessAnalyzer) Creating project: C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\research\zikq-d2coop-0.1\ghidra-project\ZikqReference  
2026-10-13 00:57:14 DEBUG (DefaultProjectManager) No tools found; Installing default tools  
2026-10-13 00:57:14 DEBUG (DefaultProjectManager) 	no recent project directories found  
2026-10-13 00:57:14 INFO  (DefaultProject) Creating project: C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\research\zikq-d2coop-0.1\ghidra-project\ZikqReference  
2026-10-13 00:57:14 INFO  (HeadlessAnalyzer) REPORT: Processing input files:   
2026-10-13 00:57:14 INFO  (HeadlessAnalyzer)      project: C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\research\zikq-d2coop-0.1\ghidra-project\ZikqReference  
2026-10-13 00:57:14 INFO  (HeadlessAnalyzer) IMPORTING: file:///C:/Users/adako/Documents/Codex/2026-10-08/i-x20/work/research/zikq-d2coop-0.1/archive/dinput8.dll  
2026-10-13 00:57:18 INFO  (ProgramLoader) Using Loader: Portable Executable (PE)  
2026-10-13 00:57:18 INFO  (ProgramLoader) Using Language/Compiler: x86:LE:64:default:windows  
2026-10-13 00:57:18 INFO  (ProgramLoader) Using Library Search Path: [., C:\WINDOWS\SysWOW64, C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\jdk-21.0.12.1+1\bin, C:\WINDOWS\Sun\Java\bin, C:\WINDOWS\system32, C:\WINDOWS, C:\Users\adako\.cache\codex-runtimes\codex-primary-runtime\dependencies\native\powershell, C:\Users\adako\.codex\tmp\arg0\codex-arg0wsCSmU, C:\Users\adako\.cache\codex-runtimes\codex-primary-runtime\dependencies\native\libheif\libheif\bin, C:\Users\adako\.cache\codex-runtimes\codex-primary-runtime\dependencies\native\jxrlib\jxrlib\bin, C:\Users\adako\.cache\codex-runtimes\codex-primary-runtime\dependencies\native\poppler\Library\bin, C:\Users\adako\.cache\codex-runtimes\codex-primary-runtime\dependencies\bin\override, C:\WINDOWS\System32\Wbem, C:\WINDOWS\System32\WindowsPowerShell\v1.0\, C:\WINDOWS\System32\OpenSSH\, C:\Program Files\NVIDIA Corporation\NVIDIA App\NvDLISR, C:\Program Files (x86)\ZeroTier\One\, C:\Program Files\dotnet\, C:\Program Files\Git\cmd, C:\Program Files (x86)\NVIDIA Corporation\PhysX\Common, C:\Program Files\Apollo, C:\Program Files\Apollo\tools, C:\Program Files\usbipd-win\, C:\Program Files\PowerToys\DSCModules\, C:\Program Files\Tailscale\, C:\Program Files\Topaz Labs LLC\Topaz Gigapixel\bin\, C:\Users\adako\AppData\Local\Programs\Python\Python312\Scripts\, C:\Users\adako\AppData\Local\Programs\Python\Python312\, C:\Users\adako\AppData\Local\Programs\Python\Python313\Scripts\, C:\Users\adako\AppData\Local\Programs\Python\Python313\, C:\Users\adako\AppData\Local\Programs\Python\Launcher\, C:\Users\adako\AppData\Local\hermes\hermes-agent\venv\Scripts, C:\Users\adako\AppData\Local\hermes\bin, C:\Users\adako\AppData\Local\Microsoft\WindowsApps, C:\Users\adako\AppData\Local\Python\bin, C:\Users\adako\AppData\Local\Microsoft\WinGet\Packages\aria2.aria2_Microsoft.Winget.Source_8wekyb3d8bbwe\aria2-1.37.0-win-64bit-build1, C:\Users\adako\AppData\Local\Programs\Ollama, C:\Users\adako\AppData\Local\hermes\node, C:\Users\adako\AppData\Local\Microsoft\WinGet\Packages\BurntSushi.ripgrep.MSVC_Microsoft.Winget.Source_8wekyb3d8bbwe\ripgrep-15.1.0-x86_64-pc-windows-msvc, C:\Users\adako\AppData\Local\Programs\Microsoft VS Code\bin, C:\Users\adako\AppData\Local\Microsoft\WinGet\Packages\Gyan.FFmpeg_Microsoft.Winget.Source_8wekyb3d8bbwe\ffmpeg-9.0.1-full_build\bin, C:\Users\adako\.lmstudio\bin, C:\Users\adako\.local\bin, C:\Users\adako\.cache\codex-runtimes\codex-primary-runtime\dependencies\bin\fallback, C:\Users\adako\.cache\codex-runtimes\codex-primary-runtime\dependencies\node\bin, C:\Users\adako\.cache\codex-runtimes\codex-primary-runtime\dependencies\native\git\cmd, C:\Users\adako\AppData\Local\OpenAI\Codex\bin\26f5cb65bac96647, C:\Users\adako\AppData\Local\OpenAI\Codex\bin\5ea220ae823df3d7]  
2026-10-13 00:57:18 INFO  (TLSDirectory) TLS callbacks at 1801449c8  
2026-10-13 00:57:22 INFO  (TLSDirectory) TLS callbacks at 180399b78  
2026-10-13 00:57:24 INFO  (TLSDirectory) TLS callbacks at 1800e58a8  
2026-10-13 00:57:24 DEBUG (WEVTResourceDataType) Error processing Provider Element.  
2026-10-13 00:57:25 INFO  (TLSDirectory) TLS callbacks at 180636cc0  
2026-10-13 00:57:25 WARN  (ExportDataDirectory) Invalid or missing function at 18071d6cc  
2026-10-13 00:57:27 WARN  (ExportDataDirectory) Invalid or missing function at 1800cf87c  
2026-10-13 00:57:27 DEBUG (WEVTResourceDataType) Error processing Provider Element.  
2026-10-13 00:57:28 DEBUG (WEVTResourceDataType) Error processing Provider Element.  
2026-10-13 00:57:28 WARN  (ExportDataDirectory) Invalid or missing function at 180061404  
2026-10-13 00:57:28 DEBUG (WEVTResourceDataType) Error processing Provider Element.  
2026-10-13 00:57:28 INFO  (ProgramLoader) Additional info:
Loading file:///C:/Users/adako/Documents/Codex/2026-10-08/i-x20/work/research/zikq-d2coop-0.1/archive/dinput8.dll?MD5=bacb04e1fbd3d869dfa784afd2564c32...
------------------------------------------------

Searching 47 paths for library ADVAPI32.DLL...
Skipping library which is the wrong architecture: file:///C:/Windows/SysWOW64/advapi32.dll
Loading file:///C:/Windows/System32/advapi32.dll?MD5=d62ed0cb4f11fb712094de1eeba6ddad...
Created exports file: C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\research\zikq-d2coop-0.1\runtime\settings\ghidra\ghidra_12.1.4_PUBLIC\symbols\win64\advapi32.exports
Library not saved to project.
------------------------------------------------

Searching 47 paths for library D3D11.DLL...
Skipping library which is the wrong architecture: file:///C:/Windows/SysWOW64/d3d11.dll
Loading file:///C:/Windows/System32/d3d11.dll?MD5=9a26da23a0075ce89dd6a8a7198d216a...
Index 4 out of bounds for length 4
Created exports file: C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\research\zikq-d2coop-0.1\runtime\settings\ghidra\ghidra_12.1.4_PUBLIC\symbols\win64\d3d11.exports
Library not saved to project.
------------------------------------------------

Searching 47 paths for library D3DCOMPILER_47.DLL...
Skipping library which is the wrong architecture: file:///C:/Windows/SysWOW64/D3DCompiler_47.dll
Loading file:///C:/Windows/System32/D3DCompiler_47.dll?MD5=6cb3211e9100b2dbb93464b0e51fcb55...
Created exports file: C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\research\zikq-d2coop-0.1\runtime\settings\ghidra\ghidra_12.1.4_PUBLIC\symbols\win64\d3dcompiler_47.exports
Library not saved to project.
------------------------------------------------

Searching 47 paths for library IMM32.DLL...
Skipping library which is the wrong architecture: file:///C:/Windows/SysWOW64/imm32.dll
Loading file:///C:/Windows/System32/imm32.dll?MD5=304c1e068240b294c589cf8e999b3c61...
Created exports file: C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\research\zikq-d2coop-0.1\runtime\settings\ghidra\ghidra_12.1.4_PUBLIC\symbols\win64\imm32.exports
Library not saved to project.
------------------------------------------------

Searching 47 paths for library IPHLPAPI.DLL...
Skipping library which is the wrong architecture: file:///C:/Windows/SysWOW64/IPHLPAPI.DLL
Loading file:///C:/Windows/System32/IPHLPAPI.DLL?MD5=ebbdd51a2a6665d7e787c4932a0ea84d...
Created exports file: C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\research\zikq-d2coop-0.1\runtime\settings\ghidra\ghidra_12.1.4_PUBLIC\symbols\win64\iphlpapi.exports
Library not saved to project.
------------------------------------------------

Searching 47 paths for library KERNEL32.DLL...
Skipping library which is the wrong architecture: file:///C:/Windows/SysWOW64/kernel32.dll
Loading file:///C:/Windows/System32/kernel32.dll?MD5=35662cbfdc02cddcf0c43e7687fc2e10...
Created exports file: C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\research\zikq-d2coop-0.1\runtime\settings\ghidra\ghidra_12.1.4_PUBLIC\symbols\win64\kernel32.exports
Library not saved to project.
------------------------------------------------

Searching 47 paths for library OLE32.DLL...
Skipping library which is the wrong architecture: file:///C:/Windows/SysWOW64/ole32.dll
Loading file:///C:/Windows/System32/ole32.dll?MD5=d253151969e40bd5cae04588c660e769...
[ole32.dll]: failed to create WEVTResource at 180184740: Failed to resolve data length for WEVTResource
Created exports file: C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\research\zikq-d2coop-0.1\runtime\settings\ghidra\ghidra_12.1.4_PUBLIC\symbols\win64\ole32.exports
Library not saved to project.
------------------------------------------------

Searching 47 paths for library SHELL32.DLL...
Skipping library which is the wrong architecture: file:///C:/Windows/SysWOW64/shell32.dll
Loading file:///C:/Windows/System32/shell32.dll?MD5=09c85e822918ae574a1caa0f6943354a...
Created exports file: C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\research\zikq-d2coop-0.1\runtime\settings\ghidra\ghidra_12.1.4_PUBLIC\symbols\win64\shell32.exports
Library not saved to project.
------------------------------------------------

Searching 47 paths for library USER32.DLL...
Skipping library which is the wrong architecture: file:///C:/Windows/SysWOW64/user32.dll
Loading file:///C:/Windows/System32/user32.dll?MD5=57c789795a3089648fe8e67f19661977...
[user32.dll]: failed to create WEVTResource at 1800efa70: Failed to resolve data length for WEVTResource
Created exports file: C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\research\zikq-d2coop-0.1\runtime\settings\ghidra\ghidra_12.1.4_PUBLIC\symbols\win64\user32.exports
Library not saved to project.
------------------------------------------------

Searching 47 paths for library WINMM.DLL...
Skipping library which is the wrong architecture: file:///C:/Windows/SysWOW64/winmm.dll
Loading file:///C:/Windows/System32/winmm.dll?MD5=a2e3d363cdb7dd10d285b1cad7139846...
[winmm.dll]: failed to create WEVTResource at 180031310: Failed to resolve data length for WEVTResource
Created exports file: C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\research\zikq-d2coop-0.1\runtime\settings\ghidra\ghidra_12.1.4_PUBLIC\symbols\win64\winmm.exports
Library not saved to project.
------------------------------------------------

Searching 47 paths for library WS2_32.DLL...
Skipping library which is the wrong architecture: file:///C:/Windows/SysWOW64/ws2_32.dll
Loading file:///C:/Windows/System32/ws2_32.dll?MD5=caed1304c3bd789368b88b25da1f479f...
[ws2_32.dll]: failed to create WEVTResource at 18006c4b8: Failed to resolve data length for WEVTResource
Created exports file: C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\research\zikq-d2coop-0.1\runtime\settings\ghidra\ghidra_12.1.4_PUBLIC\symbols\win64\ws2_32.exports
Library not saved to project.
------------------------------------------------

Applying cached symbols from D3D11.DLL
Applying cached symbols from D3DCOMPILER_47.DLL
Applying cached symbols from WS2_32.DLL
Applying cached symbols from WINMM.DLL
Applying cached symbols from IPHLPAPI.DLL
Applying cached symbols from SHELL32.DLL
Applying cached symbols from OLE32.DLL
Applying cached symbols from KERNEL32.DLL
Applying cached symbols from USER32.DLL
Applying cached symbols from IMM32.DLL
Applying C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\research\zikq-d2coop-0.1\runtime\settings\ghidra\ghidra_12.1.4_PUBLIC\symbols\win64\advapi32.exports
Linking the External Programs of 'dinput8.dll' to imported libraries...
  [D3D11.DLL] -> not found in project
  [D3DCOMPILER_47.DLL] -> not found in project
  [WS2_32.DLL] -> not found in project
  [WINMM.DLL] -> not found in project
  [IPHLPAPI.DLL] -> not found in project
  [SHELL32.DLL] -> not found in project
  [OLE32.DLL] -> not found in project
  [KERNEL32.DLL] -> not found in project
  [USER32.DLL] -> not found in project
  [IMM32.DLL] -> not found in project
  [ADVAPI32.DLL] -> not found in project
------------------------------------------------

  
2026-10-13 00:57:28 INFO  (HeadlessAnalyzer) IMPORTING: Loaded 0 additional files  
2026-10-13 00:57:29 INFO  (HeadlessAnalyzer) ANALYZING all memory and code: file:///C:/Users/adako/Documents/Codex/2026-10-08/i-x20/work/research/zikq-d2coop-0.1/archive/dinput8.dll  
2026-10-13 00:57:29 WARN  (NoReturnFunctionAnalyzer) Ignoring leading '_' chars on no-return name '___raise_securityfailure' specified in file: C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Features\Base\data\PEFunctionsThatDoNotReturn  
2026-10-13 00:57:29 WARN  (NoReturnFunctionAnalyzer) Ignoring leading '_' chars on no-return name '___report_rangecheckfailure' specified in file: C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Features\Base\data\PEFunctionsThatDoNotReturn  
2026-10-13 00:57:29 DEBUG (SymbolServerService) SymbolServerService: querying Program's Import Location - C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\research\zikq-d2coop-0.1\archive for dinput8.pdb, 509f9ea3-6d91-4aa5-ba94-b895cd659e1c, 3, 0, ???  
2026-10-13 00:57:29 DEBUG (SymbolServerService) SymbolServerService: got 0 results from Program's Import Location - C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\research\zikq-d2coop-0.1\archive  
2026-10-13 00:57:29 DEBUG (SymbolServerService) SymbolServerService: found 0 matches  
2026-10-13 00:57:29 INFO  (PdbUniversalAnalyzer) Skipping PDB processing: failed to locate PDB file in configured locations  
2026-10-13 00:57:29 INFO  (PdbUniversalAnalyzer) Use a script to set the PDB file location. I.e.,
    PdbAnalyzer.setPdbFileOption(currentProgram, new File("/path/to/pdb/file.pdb")); or
    PdbUniversalAnalyzer.setPdbFileOption(currentProgram, new File("/path/to/pdb/file.pdb"));
Or set the symbol server search configuration using:    PdbPlugin.saveSymbolServerServiceConfig(...);
 This must be done using a pre-script (prior to analysis).  
2026-10-13 00:57:29 INFO  (PackedDatabaseCache) Packed database cache: C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\research\zikq-d2coop-0.1\runtime\cache\ghidra\packed-db-cache  
2026-10-13 00:57:29 DEBUG (PackedDatabaseCache) Caching packed database: C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Features\Base\data\typeinfo\win32\windows_vs12_64.gdt  
2026-10-13 00:57:29 DEBUG (PackedDatabase) Unpacking database C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Features\Base\data\typeinfo\win32\windows_vs12_64.gdt -> C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\research\zikq-d2coop-0.1\runtime\cache\ghidra\packed-db-cache\pdb5ACD4CF0\db.1.gbf  
2026-10-13 00:57:29 DEBUG (PackedDatabaseCache) Cache update completed: C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Features\Base\data\typeinfo\win32\windows_vs12_64.gdt  
2026-10-13 00:57:38 WARN  (DecompileCallback) Decompiling 18010b7b0: Unable to read bytes at ram:183020103  
2026-10-13 00:57:47 INFO  (TypeDescriptorModel) Unprocessed TypeDescriptor: void (__cdecl *)(class std::span<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,-1>)  
2026-10-13 00:57:47 INFO  (TypeDescriptorModel) Unprocessed TypeDescriptor: void (__cdecl *)(class std::span<class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> > const ,-1>)  
2026-10-13 00:57:47 INFO  (TypeDescriptorModel) Unprocessed TypeDescriptor: void (__cdecl *)(class std::span<unsigned char const ,-1>,double)  
2026-10-13 00:57:47 INFO  (TypeDescriptorModel) Unprocessed TypeDescriptor: void (__cdecl *)(class std::span<unsigned char const ,-1>,double)  
2026-10-13 00:57:53 INFO  (ApplyDataArchiveAnalyzer) Applied data type archive: windows_vs12_64  
2026-10-13 00:57:53 INFO  (ApplyDataArchiveAnalyzer) Applied data type archive: windows_vs12_64  
2026-10-13 00:58:03 WARN  (DecompileCallback) Decompiling 18010b7b0: Unable to read bytes at ram:183020103  
2026-10-13 00:58:29 INFO  (AutoAnalysisManager) -----------------------------------------------------
    ASCII Strings                              0.597 secs
    Apply Data Archives                        0.179 secs
    Call Convention ID                         0.048 secs
    Call-Fixup Installer                       0.027 secs
    Create Address Tables                      0.261 secs
    Create Address Tables - One Time           0.094 secs
    Create Function                            0.108 secs
    Data Reference                             0.378 secs
    Decompiler Parameter ID                   21.606 secs
    Decompiler Switch Analysis                 2.655 secs
    Demangler Microsoft                        0.030 secs
    Disassemble Entry Points                   4.391 secs
    Embedded Media                             0.010 secs
    External Entry References                  0.000 secs
    Function ID                                1.931 secs
    Function Start Search                      0.070 secs
    Non-Returning Functions - Discovered       1.254 secs
    Non-Returning Functions - Known            0.020 secs
    PDB Universal                              0.015 secs
    Reference                                  0.309 secs
    Scalar Operand References                  1.612 secs
    Shared Return Calls                        0.301 secs
    Stack                                      9.161 secs
    Subroutine References                      0.188 secs
    Subroutine References - One Time           0.009 secs
    Windows x86 PE Exception Handling          1.029 secs
    Windows x86 PE RTTI Analyzer               2.756 secs
    Windows x86 Thread Environment Block (TEB) Analyzer     0.021 secs
    WindowsResourceReference                   4.083 secs
    x86 Constant Reference Analyzer            7.374 secs
-----------------------------------------------------
     Total Time   60 secs
-----------------------------------------------------
  
2026-10-13 00:58:29 INFO  (HeadlessAnalyzer) REPORT: Analysis succeeded for file: file:///C:/Users/adako/Documents/Codex/2026-10-08/i-x20/work/research/zikq-d2coop-0.1/archive/dinput8.dll  
2026-10-13 00:58:29 INFO  (HeadlessAnalyzer) REPORT: Execute script: InspectZikqLeads.java 'C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\research\zikq-d2coop-0.1/decompilation' '18014bb00' '18014bb48' '18014c588' '18014d930'   
2026-10-13 00:58:30 INFO  (HeadlessAnalyzer) SCRIPT: C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\research\zikq-d2coop-0.1\InspectZikqLeads.java  
2026-10-13 00:58:31 INFO  (HeadlessAnalyzer) ANALYZING changes made by post scripts: file:///C:/Users/adako/Documents/Codex/2026-10-08/i-x20/work/research/zikq-d2coop-0.1/archive/dinput8.dll  
2026-10-13 00:58:31 INFO  (HeadlessAnalyzer) REPORT: Post-analysis succeeded for file: file:///C:/Users/adako/Documents/Codex/2026-10-08/i-x20/work/research/zikq-d2coop-0.1/archive/dinput8.dll  
2026-10-13 00:58:31 INFO  (LocalFileSystem) /dinput8.dll: file created (adako)  
2026-10-13 00:58:31 INFO  (HeadlessAnalyzer) REPORT: Save succeeded for: /dinput8.dll (ZikqReference:/dinput8.dll)  
2026-10-13 00:58:31 INFO  (HeadlessAnalyzer) REPORT: Import succeeded  
2026-10-13 00:59:18 INFO  (LoggingInitialization) Using log config file: jar:file:/C:/Users/adako/Documents/Codex/2026-10-08/i-x20/work/tools/ghidra_12.1.4_PUBLIC/Ghidra/Framework/Generic/lib/Generic.jar!/generic.log4j.xml  
2026-10-13 00:59:18 INFO  (LoggingInitialization) Using log file: C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\research\zikq-d2coop-0.1\runtime\settings\ghidra\ghidra_12.1.4_PUBLIC\application.log  
2026-10-13 00:59:18 DEBUG (GhidraObjectInputFilter) Including serial input filter: FileSystem/data/client.rmi.serial.filter  
2026-10-13 00:59:18 INFO  (Preferences) Loading user preferences: C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\research\zikq-d2coop-0.1\runtime\settings\ghidra\ghidra_12.1.4_PUBLIC\preferences  
2026-10-13 00:59:18 INFO  (ClassSearcher) Searching for classes...  
2026-10-13 00:59:19 INFO  (ClassSearcher) Class search complete (806 ms)  
2026-10-13 00:59:19 INFO  (DefaultSSLContextInitializer) Initializing SSL Context  
2026-10-13 00:59:19 INFO  (SecureRandomFactory) Initializing Random Number Generator...  
2026-10-13 00:59:19 INFO  (SecureRandomFactory) Random Number Generator initialization complete: SHA1PRNG  
2026-10-13 00:59:19 INFO  (DefaultTrustManagerFactory) Trust manager disabled, cacerts have not been set  
2026-10-13 00:59:19 INFO  (AnalyzeHeadless) Headless startup complete (3881 ms)  
2026-10-13 00:59:19 INFO  (ClassSearcher) Class searcher loaded 59 extension points (20 false positives)  
2026-10-13 00:59:20 INFO  (HeadlessAnalyzer) HEADLESS Script Paths:
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Features\WildcardAssembler\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Debug\Debugger\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Processors\Atmel\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Processors\DATA\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Features\BytePatterns\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Features\DecompilerDependent\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Features\PDB\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Features\Base\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Features\FunctionID\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Features\MicrosoftCodeAnalyzer\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Processors\JVM\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Features\PyGhidra\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Features\SwiftDemangler\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Features\SystemEmulation\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Features\BSim\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Features\GnuDemangler\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Debug\Debugger-rmi-trace\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Features\FileFormats\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\research\zikq-d2coop-0.1
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Processors\8051\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Features\VersionTracking\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Processors\PIC\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Features\Decompiler\ghidra_scripts  
2026-10-13 00:59:20 INFO  (HeadlessAnalyzer) HEADLESS: execution starts  
2026-10-13 00:59:20 INFO  (HeadlessAnalyzer) Opening existing project: C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\research\zikq-d2coop-0.1\ghidra-project\ZikqReference  
2026-10-13 00:59:20 INFO  (HeadlessProject) Opening project: C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\research\zikq-d2coop-0.1\ghidra-project\ZikqReference  
2026-10-13 00:59:23 INFO  (HeadlessAnalyzer) REPORT: Processing read-only project file: /dinput8.dll  
2026-10-13 00:59:24 INFO  (HeadlessAnalyzer) REPORT: Execute script: InspectZikqLeads.java 'C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\research\zikq-d2coop-0.1/decompilation-followup' 'fn:18003c240' 'fn:180033420'   
2026-10-13 00:59:25 INFO  (HeadlessAnalyzer) SCRIPT: C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\research\zikq-d2coop-0.1\InspectZikqLeads.java  
2026-10-13 00:59:51 INFO  (LoggingInitialization) Using log config file: jar:file:/C:/Users/adako/Documents/Codex/2026-10-08/i-x20/work/tools/ghidra_12.1.4_PUBLIC/Ghidra/Framework/Generic/lib/Generic.jar!/generic.log4j.xml  
2026-10-13 00:59:51 INFO  (LoggingInitialization) Using log file: C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\research\zikq-d2coop-0.1\runtime\settings\ghidra\ghidra_12.1.4_PUBLIC\application.log  
2026-10-13 00:59:51 DEBUG (GhidraObjectInputFilter) Including serial input filter: FileSystem/data/client.rmi.serial.filter  
2026-10-13 00:59:51 INFO  (Preferences) Loading user preferences: C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\research\zikq-d2coop-0.1\runtime\settings\ghidra\ghidra_12.1.4_PUBLIC\preferences  
2026-10-13 00:59:51 INFO  (ClassSearcher) Searching for classes...  
2026-10-13 00:59:52 INFO  (ClassSearcher) Class search complete (692 ms)  
2026-10-13 00:59:52 INFO  (DefaultSSLContextInitializer) Initializing SSL Context  
2026-10-13 00:59:52 INFO  (SecureRandomFactory) Initializing Random Number Generator...  
2026-10-13 00:59:52 INFO  (SecureRandomFactory) Random Number Generator initialization complete: SHA1PRNG  
2026-10-13 00:59:52 INFO  (DefaultTrustManagerFactory) Trust manager disabled, cacerts have not been set  
2026-10-13 00:59:53 INFO  (AnalyzeHeadless) Headless startup complete (3419 ms)  
2026-10-13 00:59:53 INFO  (ClassSearcher) Class searcher loaded 59 extension points (20 false positives)  
2026-10-13 00:59:53 INFO  (HeadlessAnalyzer) HEADLESS Script Paths:
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Features\WildcardAssembler\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Debug\Debugger\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Processors\Atmel\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Processors\DATA\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Features\BytePatterns\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Features\DecompilerDependent\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Features\PDB\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Features\Base\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Features\FunctionID\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Features\MicrosoftCodeAnalyzer\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Processors\JVM\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Features\PyGhidra\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Features\SwiftDemangler\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Features\SystemEmulation\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Features\BSim\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Features\GnuDemangler\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Debug\Debugger-rmi-trace\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Features\FileFormats\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\research\zikq-d2coop-0.1
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Processors\8051\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Features\VersionTracking\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Processors\PIC\ghidra_scripts
    C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\tools\ghidra_12.1.4_PUBLIC\Ghidra\Features\Decompiler\ghidra_scripts  
2026-10-13 00:59:53 INFO  (HeadlessAnalyzer) HEADLESS: execution starts  
2026-10-13 00:59:53 INFO  (HeadlessAnalyzer) Opening existing project: C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\research\zikq-d2coop-0.1\ghidra-project\ZikqReference  
2026-10-13 00:59:53 INFO  (HeadlessProject) Opening project: C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\research\zikq-d2coop-0.1\ghidra-project\ZikqReference  
2026-10-13 00:59:56 INFO  (HeadlessAnalyzer) REPORT: Processing read-only project file: /dinput8.dll  
2026-10-13 00:59:57 INFO  (HeadlessAnalyzer) REPORT: Execute script: InspectZikqLeads.java 'C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\research\zikq-d2coop-0.1/decompilation-followup' 'fn:18003c5a0'   
2026-10-13 00:59:58 INFO  (HeadlessAnalyzer) SCRIPT: C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\research\zikq-d2coop-0.1\InspectZikqLeads.java  

````
