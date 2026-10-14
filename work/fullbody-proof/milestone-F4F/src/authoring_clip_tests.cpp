#include "authoring_clip.h"
#include <iostream>
#include <fstream>
#include <iterator>
#include <limits>
int main(int argc,char**argv){
 if(argc!=2)return 2;std::ifstream f(argv[1],std::ios::binary);std::vector<unsigned char> data((std::istreambuf_iterator<char>(f)),{});
 AuthoringClip c;if(!c.decode(data)){std::cerr<<"Actual Blender export rejected\n";return 3;}
 std::vector<AuthoringClip::Joint> pose;if(!c.sample(0,pose)||pose.size()!=215||!c.sample(3.067f,pose)||!c.sample(6,pose)||c.sample(-.1f,pose)||c.sample(6.1f,pose))return 4;
 auto wrong=data;wrong.pop_back();if(c.decode(wrong))return 5;
 wrong=data;wrong.push_back(0);if(c.decode(wrong))return 6;
 wrong=data;wrong[0]='X';if(c.decode(wrong))return 7;
 wrong=data;float bad=.1f;std::memcpy(wrong.data()+32+4*4,&bad,4);if(c.decode(wrong))return 8;
 wrong=data;bad=std::numeric_limits<float>::quiet_NaN();std::memcpy(wrong.data()+32+215*7*4+16,&bad,4);if(c.decode(wrong))return 9;
 wrong=data;bad=.5f;std::memcpy(wrong.data()+32+(2*7+3)*4,&bad,4);if(c.decode(wrong))return 10;
 if(!c.decode(data))return 11;std::cout<<"Authored clip validation: real export, sample boundaries, truncated/extra data, wrong format, root motion, NaN and invalid quaternion passed\n";
}
