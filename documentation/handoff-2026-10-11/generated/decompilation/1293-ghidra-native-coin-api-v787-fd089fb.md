# ghidra-native-coin-api-v787.json

Original: [work/natural-moveset/native-pickup-research-v785/ghidra-native-coin-api-v787.json](<../../../../work/natural-moveset/native-pickup-research-v785/ghidra-native-coin-api-v787.json>)

SHA-256: `DD929BA1148710A876712AC01C7A3FD9A963BF654AC3B89AEFECAC04B0F2B028`; 3191 bytes.

## Original saved record

````json
{
  "140841030": {
    "status": "fulfilled",
    "value": {
      "content": [
        {
          "type": "text",
          "text": "{\"address\":\"140841030\",\"length\":32,\"data\":[68,136,68,36,24,85,86,87,65,84,65,85,65,86,65,87,72,129,236,176,0,0,0,72,199,68,36,104,254,255,255,255],\"hex\":\"448844241855565741544155415641574881ecb000000048c7442468feffffff\"}"
        }
      ],
      "structuredContent": {
        "result": "{\"address\":\"140841030\",\"length\":32,\"data\":[68,136,68,36,24,85,86,87,65,84,65,85,65,86,65,87,72,129,236,176,0,0,0,72,199,68,36,104,254,255,255,255],\"hex\":\"448844241855565741544155415641574881ecb000000048c7442468feffffff\"}"
      },
      "isError": false
    }
  },
  "14076e2b0": {
    "status": "fulfilled",
    "value": {
      "content": [
        {
          "type": "text",
          "text": "{\"address\":\"14076e2b0\",\"length\":32,\"data\":[72,137,92,36,8,72,137,108,36,16,72,137,116,36,24,87,72,131,236,64,72,139,121,16,72,139,105,8,72,139,217,72],\"hex\":\"48895c240848896c24104889742418574883ec40488b7910488b6908488bd948\"}"
        }
      ],
      "structuredContent": {
        "result": "{\"address\":\"14076e2b0\",\"length\":32,\"data\":[72,137,92,36,8,72,137,108,36,16,72,137,116,36,24,87,72,131,236,64,72,139,121,16,72,139,105,8,72,139,217,72],\"hex\":\"48895c240848896c24104889742418574883ec40488b7910488b6908488bd948\"}"
      },
      "isError": false
    }
  },
  "14076ec20": {
    "status": "fulfilled",
    "value": {
      "content": [
        {
          "type": "text",
          "text": "{\"address\":\"14076ec20\",\"length\":32,\"data\":[64,83,72,131,236,32,128,161,208,0,0,0,253,72,139,217,132,210,116,37,72,139,73,16,72,139,1,255,144,128,0,0],\"hex\":\"40534883ec2080a1d0000000fd488bd984d27425488b4910488b01ff90800000\"}"
        }
      ],
      "structuredContent": {
        "result": "{\"address\":\"14076ec20\",\"length\":32,\"data\":[64,83,72,131,236,32,128,161,208,0,0,0,253,72,139,217,132,210,116,37,72,139,73,16,72,139,1,255,144,128,0,0],\"hex\":\"40534883ec2080a1d0000000fd488bd984d27425488b4910488b01ff90800000\"}"
      },
      "isError": false
    }
  },
  "ownerVirtual": {
    "content": [
      {
        "type": "text",
        "text": "{\"address\":\"141c5e2d8\",\"length\":8,\"data\":[176,132,135,64,1,0,0,0],\"hex\":\"b084874001000000\"}"
      }
    ],
    "structuredContent": {
      "result": "{\"address\":\"141c5e2d8\",\"length\":8,\"data\":[176,132,135,64,1,0,0,0],\"hex\":\"b084874001000000\"}"
    },
    "isError": false
  },
  "ownerRetire": {
    "content": [
      {
        "type": "text",
        "text": "\r\nvoid FUN_1408784b0(longlong param_1)\r\n\r\n{\r\n  if ((*(byte *)(param_1 + 0xd0) & 1) == 0) {\r\n    *(byte *)(param_1 + 0xd0) = *(byte *)(param_1 + 0xd0) | 1;\r\n    FUN_14087e040();\r\n    return;\r\n  }\r\n  return;\r\n}\r\n\r\n"
      }
    ],
    "structuredContent": {
      "result": "\r\nvoid FUN_1408784b0(longlong param_1)\r\n\r\n{\r\n  if ((*(byte *)(param_1 + 0xd0) & 1) == 0) {\r\n    *(byte *)(param_1 + 0xd0) = *(byte *)(param_1 + 0xd0) | 1;\r\n    FUN_14087e040();\r\n    return;\r\n  }\r\n  return;\r\n}\r\n\r\n"
    },
    "isError": false
  }
}

````

## Unescaped text fields for reading

### text

````text
{"address":"140841030","length":32,"data":[68,136,68,36,24,85,86,87,65,84,65,85,65,86,65,87,72,129,236,176,0,0,0,72,199,68,36,104,254,255,255,255],"hex":"448844241855565741544155415641574881ecb000000048c7442468feffffff"}
````

### text

````text
{"address":"14076e2b0","length":32,"data":[72,137,92,36,8,72,137,108,36,16,72,137,116,36,24,87,72,131,236,64,72,139,121,16,72,139,105,8,72,139,217,72],"hex":"48895c240848896c24104889742418574883ec40488b7910488b6908488bd948"}
````

### text

````text
{"address":"14076ec20","length":32,"data":[64,83,72,131,236,32,128,161,208,0,0,0,253,72,139,217,132,210,116,37,72,139,73,16,72,139,1,255,144,128,0,0],"hex":"40534883ec2080a1d0000000fd488bd984d27425488b4910488b01ff90800000"}
````

### text

````text

void FUN_1408784b0(longlong param_1)

{
  if ((*(byte *)(param_1 + 0xd0) & 1) == 0) {
    *(byte *)(param_1 + 0xd0) = *(byte *)(param_1 + 0xd0) | 1;
    FUN_14087e040();
    return;
  }
  return;
}


````
