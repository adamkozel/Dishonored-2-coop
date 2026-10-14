# swarm-decompile-1406be910-v280.txt

Original: [work/movement-integration/swarm-decompile-1406be910-v280.txt](<../../../../work/movement-integration/swarm-decompile-1406be910-v280.txt>)

SHA-256: `CBCEE6BA606CA8B26956FEDD41DF331F756CC74DCA1257DCFE18FA25974B883B`; 711 bytes.

## Original saved record

````text

uint FUN_1406be910(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
                  undefined8 *param_5)

{
  char cVar1;
  uint uVar2;
  uint uVar3;
  
  uVar2 = FUN_140ff2200(param_1,param_2,*param_5,0);
  if ((int)(*(uint *)((longlong)param_5 + 0xc) >> 8) < (int)uVar2) {
    cVar1 = FUN_140384fe0(param_5,uVar2);
    if (cVar1 == '\0') goto code_r0x0001406be966;
  }
  *(uint *)(param_5 + 1) = *(uint *)(param_5 + 1) & 0xff000000;
  uVar3 = *(uint *)((longlong)param_5 + 0xc) >> 8;
  if ((int)uVar2 < (int)uVar3) {
    uVar3 = uVar2;
  }
  *(uint *)(param_5 + 1) = *(uint *)(param_5 + 1) | uVar3 & 0xffffff;
code_r0x0001406be966:
  FUN_140ff2730(param_1);
  return uVar2;
}



````
