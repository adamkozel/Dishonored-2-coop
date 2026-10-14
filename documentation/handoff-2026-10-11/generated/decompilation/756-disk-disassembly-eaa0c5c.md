# disk-disassembly.json

Original: [work/movement-integration/checkpoint-continuation-v740/disk-disassembly.json](<../../../../work/movement-integration/checkpoint-continuation-v740/disk-disassembly.json>)

SHA-256: `AEDC0CAD8DDC650EE39D991DAB76E29FFC1C9FE9DFC2E34C993600071C95C693`; 10501 bytes.

## Original saved record

````json
{
  "exe": "C:\\Users\\adako\\Documents\\Codex\\2026-10-08\\i-x20\\work\\asset-game\\Dishonored2.exe",
  "sha256": "B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180",
  "capstone": "5.0.6",
  "rows": [
    {
      "rva": "0x65db00",
      "hex": "4883ec28488b05857734034885c0745d48895c2420488d98d8752e004885db7447488b0d08b93203488b01ff50584885c07f15488d0d6e6c5d01488b5c24204883c428e9e88cb0ff4183c8ff4c8d0df54c4501488bcb418d5003488b5c24204883c428e928b6ffff488b5c24204883c4",
      "instructions": [
        "14065db00 4883ec28 sub rsp, 0x28",
        "14065db04 488b0585773403 mov rax, qword ptr [rip + 0x3347785]",
        "14065db0b 4885c0 test rax, rax",
        "14065db0e 745d je 0x14065db6d",
        "14065db10 48895c2420 mov qword ptr [rsp + 0x20], rbx",
        "14065db15 488d98d8752e00 lea rbx, [rax + 0x2e75d8]",
        "14065db1c 4885db test rbx, rbx",
        "14065db1f 7447 je 0x14065db68",
        "14065db21 488b0d08b93203 mov rcx, qword ptr [rip + 0x332b908]",
        "14065db28 488b01 mov rax, qword ptr [rcx]",
        "14065db2b ff5058 call qword ptr [rax + 0x58]",
        "14065db2e 4885c0 test rax, rax",
        "14065db31 7f15 jg 0x14065db48",
        "14065db33 488d0d6e6c5d01 lea rcx, [rip + 0x15d6c6e]",
        "14065db3a 488b5c2420 mov rbx, qword ptr [rsp + 0x20]",
        "14065db3f 4883c428 add rsp, 0x28",
        "14065db43 e9e88cb0ff jmp 0x140166830",
        "14065db48 4183c8ff or r8d, 0xffffffff",
        "14065db4c 4c8d0df54c4501 lea r9, [rip + 0x1454cf5]",
        "14065db53 488bcb mov rcx, rbx",
        "14065db56 418d5003 lea edx, [r8 + 3]",
        "14065db5a 488b5c2420 mov rbx, qword ptr [rsp + 0x20]",
        "14065db5f 4883c428 add rsp, 0x28",
        "14065db63 e928b6ffff jmp 0x140659190",
        "14065db68 488b5c2420 mov rbx, qword ptr [rsp + 0x20]"
      ]
    },
    {
      "rva": "0x659190",
      "hex": "4c894c2420554154415541564157488dac24e061ffffb8209f0000e81046b500482be048c74580feffffff48899c24509f00004889b424589f00004889bc24609f0000498bd9458bf0448be24c8bf9488b8180790200f64010010f8520070000488b4908e8b75dffff84c00f850f07000041f78728010000ffffff000f85fe06",
      "instructions": [
        "140659190 4c894c2420 mov qword ptr [rsp + 0x20], r9",
        "140659195 55 push rbp",
        "140659196 4154 push r12",
        "140659198 4155 push r13",
        "14065919a 4156 push r14",
        "14065919c 4157 push r15",
        "14065919e 488dac24e061ffff lea rbp, [rsp - 0x9e20]",
        "1406591a6 b8209f0000 mov eax, 0x9f20",
        "1406591ab e81046b500 call 0x1411ad7c0",
        "1406591b0 482be0 sub rsp, rax",
        "1406591b3 48c74580feffffff mov qword ptr [rbp - 0x80], 0xfffffffffffffffe",
        "1406591bb 48899c24509f0000 mov qword ptr [rsp + 0x9f50], rbx",
        "1406591c3 4889b424589f0000 mov qword ptr [rsp + 0x9f58], rsi",
        "1406591cb 4889bc24609f0000 mov qword ptr [rsp + 0x9f60], rdi",
        "1406591d3 498bd9 mov rbx, r9",
        "1406591d6 458bf0 mov r14d, r8d",
        "1406591d9 448be2 mov r12d, edx",
        "1406591dc 4c8bf9 mov r15, rcx",
        "1406591df 488b8180790200 mov rax, qword ptr [rcx + 0x27980]",
        "1406591e6 f6401001 test byte ptr [rax + 0x10], 1",
        "1406591ea 0f8520070000 jne 0x140659910",
        "1406591f0 488b4908 mov rcx, qword ptr [rcx + 8]",
        "1406591f4 e8b75dffff call 0x14064efb0",
        "1406591f9 84c0 test al, al",
        "1406591fb 0f850f070000 jne 0x140659910",
        "140659201 41f78728010000ffffff00 test dword ptr [r15 + 0x128], 0xffffff"
      ]
    },
    {
      "rva": "0x6597ca",
      "hex": "443925b795ad020f95c0488d156dba5d010fb6c8e8fdd0b0ff488b3d46fc32034885ff0f84f1000000488b07488bcfff50584885c00f84df000000488bcfe87327b0ff4c8d8da00000004c8d442430488bd7498bcfe83cf7ffff84c0750c488d0d69ba5d01e9b70000008b5c244883fb010f94c241b001488d0db84cc702e8d38caaff33c98bc183fb010f94c089051f72c2018b44244c89051972c201390d1b96ad02480f44f941b00133d2488d0d63b73403e85e90aaff4181a7e0790200000000ff4181a7f0790200000000ff488d8db0000000e80ca3aeff488b0d75abc10184c0488b017405ff5040eb104c8bc7488d95a0000000ff90a8010000b301eb34833db694ad02000f95c0488d1544b95d010fb6c8e8fccfb0ff488d0d85b95d01e840cfb0ffba04000000498bcfe83366000032db488d8da0000000e815549eff0fb6c3eb6e4585e475104c8bc3418bd6498bcfe88d130000eb1a4183fc027514488b0dae21c301488b01488d154cae5d01ff5070488b0d9a21c301488b01488d15c4b75d01ff5070498b8f80790200833903751d0fb64110a8017415f20f1049180f57c0660f2ec87a0775050c0288411032c04c8d9c24209f0000498b5b30498b7338498b7b40498be3415f415e415d415c5dc3",
      "instructions": [
        "1406597ca 443925b795ad02 cmp dword ptr [rip + 0x2ad95b7], r12d",
        "1406597d1 0f95c0 setne al",
        "1406597d4 488d156dba5d01 lea rdx, [rip + 0x15dba6d]",
        "1406597db 0fb6c8 movzx ecx, al",
        "1406597de e8fdd0b0ff call 0x1401668e0",
        "1406597e3 488b3d46fc3203 mov rdi, qword ptr [rip + 0x332fc46]",
        "1406597ea 4885ff test rdi, rdi",
        "1406597ed 0f84f1000000 je 0x1406598e4",
        "1406597f3 488b07 mov rax, qword ptr [rdi]",
        "1406597f6 488bcf mov rcx, rdi",
        "1406597f9 ff5058 call qword ptr [rax + 0x58]",
        "1406597fc 4885c0 test rax, rax",
        "1406597ff 0f84df000000 je 0x1406598e4",
        "140659805 488bcf mov rcx, rdi",
        "140659808 e87327b0ff call 0x14015bf80",
        "14065980d 4c8d8da0000000 lea r9, [rbp + 0xa0]",
        "140659814 4c8d442430 lea r8, [rsp + 0x30]",
        "140659819 488bd7 mov rdx, rdi",
        "14065981c 498bcf mov rcx, r15",
        "14065981f e83cf7ffff call 0x140658f60",
        "140659824 84c0 test al, al",
        "140659826 750c jne 0x140659834",
        "140659828 488d0d69ba5d01 lea rcx, [rip + 0x15dba69]",
        "14065982f e9b7000000 jmp 0x1406598eb",
        "140659834 8b5c2448 mov ebx, dword ptr [rsp + 0x48]",
        "140659838 83fb01 cmp ebx, 1",
        "14065983b 0f94c2 sete dl",
        "14065983e 41b001 mov r8b, 1",
        "140659841 488d0db84cc702 lea rcx, [rip + 0x2c74cb8]",
        "140659848 e8d38caaff call 0x140102520",
        "14065984d 33c9 xor ecx, ecx",
        "14065984f 8bc1 mov eax, ecx",
        "140659851 83fb01 cmp ebx, 1",
        "140659854 0f94c0 sete al",
        "140659857 89051f72c201 mov dword ptr [rip + 0x1c2721f], eax",
        "14065985d 8b44244c mov eax, dword ptr [rsp + 0x4c]",
        "140659861 89051972c201 mov dword ptr [rip + 0x1c27219], eax",
        "140659867 390d1b96ad02 cmp dword ptr [rip + 0x2ad961b], ecx",
        "14065986d 480f44f9 cmove rdi, rcx",
        "140659871 41b001 mov r8b, 1",
        "140659874 33d2 xor edx, edx",
        "140659876 488d0d63b73403 lea rcx, [rip + 0x334b763]",
        "14065987d e85e90aaff call 0x1401028e0",
        "140659882 4181a7e0790200000000ff and dword ptr [r15 + 0x279e0], 0xff000000",
        "14065988d 4181a7f0790200000000ff and dword ptr [r15 + 0x279f0], 0xff000000",
        "140659898 488d8db0000000 lea rcx, [rbp + 0xb0]",
        "14065989f e80ca3aeff call 0x140143bb0",
        "1406598a4 488b0d75abc101 mov rcx, qword ptr [rip + 0x1c1ab75]",
        "1406598ab 84c0 test al, al",
        "1406598ad 488b01 mov rax, qword ptr [rcx]",
        "1406598b0 7405 je 0x1406598b7",
        "1406598b2 ff5040 call qword ptr [rax + 0x40]",
        "1406598b5 eb10 jmp 0x1406598c7",
        "1406598b7 4c8bc7 mov r8, rdi",
        "1406598ba 488d95a0000000 lea rdx, [rbp + 0xa0]",
        "1406598c1 ff90a8010000 call qword ptr [rax + 0x1a8]",
        "1406598c7 b301 mov bl, 1",
        "1406598c9 eb34 jmp 0x1406598ff",
        "1406598cb 833db694ad0200 cmp dword ptr [rip + 0x2ad94b6], 0",
        "1406598d2 0f95c0 setne al",
        "1406598d5 488d1544b95d01 lea rdx, [rip + 0x15db944]",
        "1406598dc 0fb6c8 movzx ecx, al",
        "1406598df e8fccfb0ff call 0x1401668e0",
        "1406598e4 488d0d85b95d01 lea rcx, [rip + 0x15db985]",
        "1406598eb e840cfb0ff call 0x140166830",
        "1406598f0 ba04000000 mov edx, 4",
        "1406598f5 498bcf mov rcx, r15",
        "1406598f8 e833660000 call 0x14065ff30",
        "1406598fd 32db xor bl, bl",
        "1406598ff 488d8da0000000 lea rcx, [rbp + 0xa0]",
        "140659906 e815549eff call 0x14003ed20",
        "14065990b 0fb6c3 movzx eax, bl",
        "14065990e eb6e jmp 0x14065997e",
        "140659910 4585e4 test r12d, r12d",
        "140659913 7510 jne 0x140659925",
        "140659915 4c8bc3 mov r8, rbx",
        "140659918 418bd6 mov edx, r14d",
        "14065991b 498bcf mov rcx, r15",
        "14065991e e88d130000 call 0x14065acb0",
        "140659923 eb1a jmp 0x14065993f",
        "140659925 4183fc02 cmp r12d, 2",
        "140659929 7514 jne 0x14065993f",
        "14065992b 488b0dae21c301 mov rcx, qword ptr [rip + 0x1c321ae]",
        "140659932 488b01 mov rax, qword ptr [rcx]",
        "140659935 488d154cae5d01 lea rdx, [rip + 0x15dae4c]",
        "14065993c ff5070 call qword ptr [rax + 0x70]",
        "14065993f 488b0d9a21c301 mov rcx, qword ptr [rip + 0x1c3219a]",
        "140659946 488b01 mov rax, qword ptr [rcx]",
        "140659949 488d15c4b75d01 lea rdx, [rip + 0x15db7c4]",
        "140659950 ff5070 call qword ptr [rax + 0x70]",
        "140659953 498b8f80790200 mov rcx, qword ptr [r15 + 0x27980]",
        "14065995a 833903 cmp dword ptr [rcx], 3",
        "14065995d 751d jne 0x14065997c",
        "14065995f 0fb64110 movzx eax, byte ptr [rcx + 0x10]",
        "140659963 a801 test al, 1",
        "140659965 7415 je 0x14065997c",
        "140659967 f20f104918 movsd xmm1, qword ptr [rcx + 0x18]",
        "14065996c 0f57c0 xorps xmm0, xmm0",
        "14065996f 660f2ec8 ucomisd xmm1, xmm0",
        "140659973 7a07 jp 0x14065997c",
        "140659975 7505 jne 0x14065997c",
        "140659977 0c02 or al, 2",
        "140659979 884110 mov byte ptr [rcx + 0x10], al",
        "14065997c 32c0 xor al, al",
        "14065997e 4c8d9c24209f0000 lea r11, [rsp + 0x9f20]",
        "140659986 498b5b30 mov rbx, qword ptr [r11 + 0x30]",
        "14065998a 498b7338 mov rsi, qword ptr [r11 + 0x38]",
        "14065998e 498b7b40 mov rdi, qword ptr [r11 + 0x40]",
        "140659992 498be3 mov rsp, r11",
        "140659995 415f pop r15",
        "140659997 415e pop r14",
        "140659999 415d pop r13",
        "14065999b 415c pop r12",
        "14065999d 5d pop rbp",
        "14065999e c3 ret "
      ]
    }
  ]
}
````
