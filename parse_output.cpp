#include "pivot_base.h"

int main(int argc, char** argv){
  int mode;
  if(argc != 3)
    return 1;
  ifstream input(argv[1]);
  ofstream output(argv[2]);

  char* in = new char[56];
  
  while(input.read(in,56)){
      for(int i = 0; i < 8; i++)
	output << bitset<8>(in[i]) << ",";
      output << max_f(in+8) << ",";
      output << (int)max_f_r(in+8) << ",";
      output << (int)max_f_c(in+8) << ",";
      output << med_f(in+8) << ",";
      output << min_f(in+8) << ",";
      output << (int)min_f_r(in+8) << ",";
      output << (int)min_f_c(in+8) << ",";
      output << fillin_max_f(in+8) << ",";
      output << (int)fillin_max_f_r(in+8) << ",";
      output << (int)fillin_max_f_c(in+8) << ",";
      output << fillin_med_f(in+8) << ",";
      output << fillin_min_f(in+8) << ",";
      output << (int)fillin_min_f_r(in+8) << ",";
      output << (int)fillin_min_f_c(in+8) << ",";
      output << max_r(in+8) << ",";
      output << (int)max_r_r(in+8) << ",";
      output << (int)max_r_c(in+8) << ",";
      output << med_r(in+8) << ",";
      output << min_r(in+8) << ",";
      output << (int)min_r_r(in+8) << ",";
      output << (int)min_r_c(in+8) << ",";
      output << fillin_max_r(in+8) << ",";
      output << (int)fillin_max_r_r(in+8) << ",";
      output << (int)fillin_max_r_c(in+8) << ",";
      output << fillin_med_r(in+8) << ",";
      output << fillin_min_r(in+8) << ",";
      output << (int)fillin_min_r_r(in+8) << ",";
      output << (int)fillin_min_r_c(in+8) << endl;
  }

  input.close();
  output.close();
  
  return 0;
}
