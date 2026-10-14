
void FUN_14085b400(undefined8 param_1,undefined8 param_2,undefined4 param_3,float param_4)

{
  int local_res20 [2];
  
  if (param_4 != 1.0) {
    local_res20[0] = 0;
    FUN_140854be0(0,local_res20,param_2,param_3,0);
    if (local_res20[0] != 0) {
      FUN_1407d8fe0(D2_GameSingletonPtr + 0x1f5338,local_res20[0],param_4);
    }
    return;
  }
  local_res20[0] = 0;
  FUN_140854be0(0,local_res20,param_2,param_3,0);
  return;
}


