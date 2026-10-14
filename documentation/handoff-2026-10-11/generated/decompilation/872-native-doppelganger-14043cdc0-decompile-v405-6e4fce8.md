# native-doppelganger-14043cdc0-decompile-v405.txt

Original: [work/movement-integration/native-doppelganger-14043cdc0-decompile-v405.txt](<../../../../work/movement-integration/native-doppelganger-14043cdc0-decompile-v405.txt>)

SHA-256: `16CE0B44CB37E2619FE94C3A3655A0C996807CD4216CACA20CA8061C6FFD875B`; 569 bytes.

## Original saved record

````text

longlong FUN_14043cdc0(longlong *param_1,undefined8 param_2,undefined8 param_3)

{
  undefined8 uVar1;
  longlong lVar2;
  
  uVar1 = (**(code **)(*param_1 + 0x1b0))();
  lVar2 = FUN_1408ae9b0(D2_GameSingletonPtr,uVar1,param_2,param_3,0,0,0,0,0);
  if (lVar2 == 0) {
                    /* WARNING: Subroutine does not return */
    FUN_140166830(
                 "Can\'t create clone entity for the power. Please check you picked an idEntityDef in arkDeclCpntPowerDecoy."
                 );
  }
  (**(code **)(*param_1 + 0x180))(param_1,lVar2);
  return lVar2;
}



````
