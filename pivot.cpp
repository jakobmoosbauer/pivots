/* 
 * pivot.cpp
 * 
 * Created: 28.02.2019
 * Author : Jakob Moosbauer
 */

#include "pivot_base.h"

int main(int argc, char** argv){
  int size = strtol(argv[1],NULL,0);
  unsigned long long n_inputs = strtoull(argv[2],NULL,0);

  fstream input("input");
  ifstream in_mats("in"+to_string(size)+".db");

  in_mats.seekg(0,ios::end);
  unsigned long long max_input = in_mats.tellg()/8;
  in_mats.seekg(0,ios::beg);

  unsigned long long n;
  input.read((char*)&n,8);

  if(n+n_inputs >= max_input){
    cout << endl << "REACHED MAX " << max_input << endl;
    n_inputs = max_input-n;
  }

  unsigned long long max_n = n + n_inputs;
  input.seekp(0);
  input.write((char*)&max_n,8);
  input.close();

  in_mats.seekg(8*n,ios::beg);

  unsigned char* mat = new unsigned char[8];
  unsigned char* out = new unsigned char[56*n_inputs];

  map2 db = load_db2("db",size);
  
  for(int i = 0; n < max_n; n++,i++){
    in_mats.read((char*)mat,8);
    canonisation(mat,mat);
    *(matr*)(out+56*i)=*(matr*)mat;
    test_pivots(mat, 8, db, (char*)(out+56*i+8));
    /*printmat(out+48*i,8);
    cout << *(short*)(out+48*i+8) << ",";
    cout << (int)out[48*i+10] << ",";
    cout << (int)out[48*i+11] << ",";
    cout << *(short*)(out+48*i+12) << ",";
    cout << *(short*)(out+48*i+14) << ",";
    cout << (int)*(out+48*i+16) << ",";
    cout << (int)*(out+48*i+17) << ",";
    cout << *(short*)(out+48*i+18) << ",";
    cout << (int)*(out+48*i+20) << ",";
    cout << (int)*(out+48*i+21) << ",";
    cout << *(short*)(out+48*i+22) << ",";
    cout << *(short*)(out+48*i+24) << ",";
    cout << (int)*(out+48*i+26) << ",";
    cout << (int)*(out+48*i+27) << ",";
    cout << *(short*)(out+48*i+28) << ",";
    cout << (int)*(out+48*i+30) << ",";
    cout << (int)*(out+48*i+31) << ",";
    cout << *(short*)(out+48*i+32) << ",";
    cout << *(short*)(out+48*i+34) << ",";
    cout << (int)*(out+48*i+36) << ",";
    cout << (int)*(out+48*i+37) << ",";
    cout << *(short*)(out+48*i+38) << ",";
    cout << (int)*(out+48*i+40) << ",";
    cout << (int)*(out+48*i+41) << ",";
    cout << *(short*)(out+48*i+42) << ",";
    cout << *(short*)(out+48*i+44) << ",";
    cout << (int)*(out+48*i+46) << ",";
    cout << (int)*(out+48*i+47) << ",";
    cout << endl;*/
  }

  in_mats.close();

  ofstream output("db" + to_string(size),ios::app);
  output.write((char*)out,56*n_inputs);
  output.close();
  
  /*
  for(int in = 0; in < n_inputs; in++){
    increase(mat,size);
    if(mat[0] == 1<<size){
      cout << endl << "REACHED MAX " << in+1 << endl;
      n_inputs=in+1;
    }
  }
  
  input.seekp(0);
  input.write((char*)mat,8);
  input.close();
  
  cout << "read input"  << endl;

  map2 db = load_db2("out", size);
  
  *(matr*)mat = inputmat;

  
  
  matr* mats= new matr[OUTPUTS];
  char* out = new char[OUTPUTS];
  set processed;

  for(int in = 0; in < n_inputs; in++){
    mata tempmat = new unsigned char[8];
    canonisation(mat,tempmat);
    if(processed.count(*(matr*)tempmat)==0){
      processed.insert(*(matr*)tempmat);
      test_pivots(tempmat,size,db,out+48*in);
    mats[in] = *(matr*)mat;
    increase(mat,size);
  }

  write_result("db_opt", n_inputs, mats, size, ops, rows, cols);
  */
  return 0;
}
