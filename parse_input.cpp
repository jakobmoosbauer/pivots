#include <iostream>
#include <fstream>
#include <cstdlib>
#include <bitset>

using namespace std;
typedef unsigned long long matr;

int main(int argc, char** argv){
  if(argc != 4)
    return 1;
  int size = strtol(argv[3],NULL,0);
  ifstream input(argv[1]);
  ofstream output(argv[2]);

  int n;
  unsigned int m[8] = {};
  int wrt;
  
  while(input.read((char*)&n,4)){
    input.read((char*)m,size*4);
    input.seekg(4*size,ios::cur);
    for(int i = 0; i < 8; i++){
      wrt = m[i]>>(32-size*2);
      //cout << bitset<8>(wrt) << ",";
      output.write((char*)&wrt,1);
    }
    //cout << endl;
    //cout << size << ":" << bitset<8>(m[0]>>(32-size*2)) << "," << bitset<8>(m[1]>>(32-size*2)) << "," << bitset<8>(m[2]>>(32-size*2)) << "," << bitset<8>(m[3]>>(32-size*2)) << "," << bitset<8>(m[4]>>(32-size*2)) << "," << bitset<8>(m[5]>>(32-size*2)) << endl;
  }
  
  input.close();
  output.close();
  
  return 0;
}
