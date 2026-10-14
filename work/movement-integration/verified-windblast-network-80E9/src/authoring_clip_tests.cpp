#include "authoring_clip.h"
#include <iostream>
#include <fstream>
#include <iterator>
#include <limits>
int main(int argc,char**argv){
 if(argc!=2&&argc!=3)return 2;std::ifstream f(argv[1],std::ios::binary);std::vector<unsigned char> data((std::istreambuf_iterator<char>(f)),{});
 AuthoringClip c;if(!c.decode(data)){std::cerr<<"Actual Blender export rejected\n";return 3;}
 std::vector<AuthoringClip::Joint> pose;if(!c.sample(0,pose)||pose.size()!=215||!c.sample(3.067f,pose)||!c.sample(6,pose)||c.sample(-.1f,pose)||c.sample(6.1f,pose))return 4;
 auto wrong=data;wrong.pop_back();if(c.decode(wrong))return 5;
 wrong=data;wrong.push_back(0);if(c.decode(wrong))return 6;
 wrong=data;wrong[0]='X';if(c.decode(wrong))return 7;
 wrong=data;float bad=.1f;std::memcpy(wrong.data()+32+4*4,&bad,4);if(c.decode(wrong))return 8;
 wrong=data;bad=std::numeric_limits<float>::quiet_NaN();std::memcpy(wrong.data()+32+215*7*4+16,&bad,4);if(c.decode(wrong))return 9;
 wrong=data;bad=.5f;std::memcpy(wrong.data()+32+(2*7+3)*4,&bad,4);if(c.decode(wrong))return 10;
 if(!c.decode(data))return 11;std::cout<<"Authored clip validation: real export, sample boundaries, truncated/extra data, wrong format, root motion, NaN and invalid quaternion passed\n";
 if(argc==3){
  std::ifstream generated(argv[2],std::ios::binary);std::vector<unsigned char> v2((std::istreambuf_iterator<char>(generated)),{});
  if(!c.decode(v2)||!c.generated()||c.frameCount!=481||!c.sample(8,pose)||c.sample(8.01f,pose))return 20;
  float phase=0;if(!c.secondsAtDistance(0,phase)||phase!=2||!c.secondsAtDistance(c.distanceCurve[360],phase)||phase!=6)return 21;
  if(!c.secondsAtDistance(c.distanceCurve[220],phase)||std::fabs(phase-220.f/60)>.001f||c.secondsAtDistance(-.01f,phase)||c.secondsAtDistance(6,phase))return 22;
  wrong=v2;wrong[32]=0;if(c.decode(wrong))return 23;
  wrong=v2;bad=-1;std::memcpy(wrong.data()+48+200*4,&bad,4);if(c.decode(wrong))return 24;
  wrong=v2;bad=std::numeric_limits<float>::quiet_NaN();std::memcpy(wrong.data()+48+200*4,&bad,4);if(c.decode(wrong))return 25;
  wrong=v2;bad=.01f;std::memcpy(wrong.data()+48,&bad,4);if(c.decode(wrong))return 26;
  wrong=v2;bad=5.5f;std::memcpy(wrong.data()+48+479*4,&bad,4);if(c.decode(wrong))return 27;
  wrong=v2;bad=.1f;std::memcpy(wrong.data()+48+481*4+4*4,&bad,4);if(c.decode(wrong))return 28;
  wrong=v2;wrong.pop_back();if(c.decode(wrong))return 29;
  if(!c.decode(v2))return 30;
  std::cout<<"Generated clip validation: real export, distance phase endpoints/interpolation, wrong metadata, reverse/NaN/unbounded travel, root mutation and truncation passed\n";
 }
}
