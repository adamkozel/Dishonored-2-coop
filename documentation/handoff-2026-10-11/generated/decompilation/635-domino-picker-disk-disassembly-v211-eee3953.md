# domino-picker-disk-disassembly-v211.json

Original: [work/domino-picker-disk-disassembly-v211.json](<../../../../work/domino-picker-disk-disassembly-v211.json>)

SHA-256: `27EF7BE2E237BB9FCE3018E8B84CBAC0974906F97A9A7567A0A398B89518FE72`; 4925 bytes.

## Original saved record

````json
{
  "exe": "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Dishonored2\\Dishonored2.exe",
  "exeSha256": "B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180",
  "capstone": "5.0.6",
  "wheelSha256": "761C3DEAE00B22AC697081CDAE1383BB90659DD0D79387A09CF5BDBB22B17064",
  "rows": [
    {
      "rva": "0x447250",
      "hex": "405341574883ec284c8bf9488b89b80000000fb6dae86666070080783800750a",
      "instructions": [
        {
          "address": "0x140447250",
          "size": 2,
          "bytes": "4053",
          "mnemonic": "push",
          "operands": "rbx"
        },
        {
          "address": "0x140447252",
          "size": 2,
          "bytes": "4157",
          "mnemonic": "push",
          "operands": "r15"
        },
        {
          "address": "0x140447254",
          "size": 4,
          "bytes": "4883ec28",
          "mnemonic": "sub",
          "operands": "rsp, 0x28"
        },
        {
          "address": "0x140447258",
          "size": 3,
          "bytes": "4c8bf9",
          "mnemonic": "mov",
          "operands": "r15, rcx"
        },
        {
          "address": "0x14044725b",
          "size": 7,
          "bytes": "488b89b8000000",
          "mnemonic": "mov",
          "operands": "rcx, qword ptr [rcx + 0xb8]"
        },
        {
          "address": "0x140447262",
          "size": 3,
          "bytes": "0fb6da",
          "mnemonic": "movzx",
          "operands": "ebx, dl"
        },
        {
          "address": "0x140447265",
          "size": 5,
          "bytes": "e866660700",
          "mnemonic": "call",
          "operands": "0x1404bd8d0"
        },
        {
          "address": "0x14044726a",
          "size": 4,
          "bytes": "80783800",
          "mnemonic": "cmp",
          "operands": "byte ptr [rax + 0x38], 0"
        },
        {
          "address": "0x14044726e",
          "size": 2,
          "bytes": "750a",
          "mnemonic": "jne",
          "operands": "0x14044727a"
        }
      ]
    },
    {
      "rva": "0x4478c0",
      "hex": "40574883ec40488b81c000000048895c2458488bf9f74028ffffff0048897424600f9fc2e867f9ffff8b8fc8000000488bd8e8694c2900483bc30f84da000000",
      "instructions": [
        {
          "address": "0x1404478c0",
          "size": 2,
          "bytes": "4057",
          "mnemonic": "push",
          "operands": "rdi"
        },
        {
          "address": "0x1404478c2",
          "size": 4,
          "bytes": "4883ec40",
          "mnemonic": "sub",
          "operands": "rsp, 0x40"
        },
        {
          "address": "0x1404478c6",
          "size": 7,
          "bytes": "488b81c0000000",
          "mnemonic": "mov",
          "operands": "rax, qword ptr [rcx + 0xc0]"
        },
        {
          "address": "0x1404478cd",
          "size": 5,
          "bytes": "48895c2458",
          "mnemonic": "mov",
          "operands": "qword ptr [rsp + 0x58], rbx"
        },
        {
          "address": "0x1404478d2",
          "size": 3,
          "bytes": "488bf9",
          "mnemonic": "mov",
          "operands": "rdi, rcx"
        },
        {
          "address": "0x1404478d5",
          "size": 7,
          "bytes": "f74028ffffff00",
          "mnemonic": "test",
          "operands": "dword ptr [rax + 0x28], 0xffffff"
        },
        {
          "address": "0x1404478dc",
          "size": 5,
          "bytes": "4889742460",
          "mnemonic": "mov",
          "operands": "qword ptr [rsp + 0x60], rsi"
        },
        {
          "address": "0x1404478e1",
          "size": 3,
          "bytes": "0f9fc2",
          "mnemonic": "setg",
          "operands": "dl"
        },
        {
          "address": "0x1404478e4",
          "size": 5,
          "bytes": "e867f9ffff",
          "mnemonic": "call",
          "operands": "0x140447250"
        },
        {
          "address": "0x1404478e9",
          "size": 6,
          "bytes": "8b8fc8000000",
          "mnemonic": "mov",
          "operands": "ecx, dword ptr [rdi + 0xc8]"
        },
        {
          "address": "0x1404478ef",
          "size": 3,
          "bytes": "488bd8",
          "mnemonic": "mov",
          "operands": "rbx, rax"
        },
        {
          "address": "0x1404478f2",
          "size": 5,
          "bytes": "e8694c2900",
          "mnemonic": "call",
          "operands": "0x1406dc560"
        },
        {
          "address": "0x1404478f7",
          "size": 3,
          "bytes": "483bc3",
          "mnemonic": "cmp",
          "operands": "rax, rbx"
        },
        {
          "address": "0x1404478fa",
          "size": 6,
          "bytes": "0f84da000000",
          "mnemonic": "je",
          "operands": "0x1404479da"
        }
      ]
    }
  ]
}
````
