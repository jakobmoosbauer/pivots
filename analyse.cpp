#include "pivot_base.h"

int main(int argc, char** argv){
  int mode;
  if(argc != 2)
    return 1;
  int size = strtol(argv[1],NULL,0);
  ifstream input("db"+to_string(size));

  input.seekg(0,ios::end);
  unsigned long long n = input.tellg()/56;
  input.seekg(0,ios::beg);
  input.close();

  cout << "Analysing " << n << " matrices of size " << size << endl;

  map2 db = load_db2("db",size+1);
  values v;
  
  unsigned long long nocost = 0;
  double mean_ring = 0;
  double mean_field = 0;
  unsigned long long fillin_optimal_field = 0;
  unsigned long long fillin_optimal_ring = 0;
  int max_field = 0;
  int max_ring = 0;
  double mean_improvement_ring = 0;
  double mean_improvement_field = 0;
  double mean_fillin_improvement_ring = 0;
  double mean_fillin_improvement_field = 0;
  unsigned long long fillin_worse_field = 0;
  unsigned long long fillin_worse_ring = 0;
  
  for(auto it = db.begin(); it != db.end(); it++) {
    v = it->second;
#ifdef DATACHECK
    if(v.max_field < v.med_field || v.med_field < v.min_field || v.max_field<v.fillin_max_field
       || v.fillin_max_field<v.fillin_med_field || v.fillin_med_field<v.fillin_min_field ||
       v.fillin_min_field<v.min_field || v.max_ring < v.med_ring || v.med_ring < v.min_ring ||
       v.max_ring<v.fillin_max_ring || v.fillin_max_ring<v.fillin_med_ring ||
       v.fillin_med_ring<v.fillin_min_ring || v.fillin_min_ring<v.min_ring){
      cout << "Datacheck failed! Data is incosistent!" << endl;
      return 1;
    }
#endif
   if(v.max_field == 0)
      nocost++;
    else{
      mean_field += v.med_field;
      mean_ring += v.med_ring;
    }

    if(v.fillin_min_field == v.min_field)
      fillin_optimal_field++;
    if(v.fillin_min_ring == v.min_ring)
      fillin_optimal_ring++;
    if(v.max_field > max_field)
      max_field = v.max_field;
    if(v.max_ring > max_ring)
      max_ring = v.max_ring;
    if(v.fillin_med_field == 0){
      mean_improvement_field++;
      mean_fillin_improvement_field++;
    }
    else{
      mean_improvement_field += v.min_field/v.fillin_med_field;
      mean_fillin_improvement_field += v.fillin_med_field/v.med_field;
    }
    if(v.fillin_med_ring == 0){
      mean_improvement_ring++;
      mean_fillin_improvement_ring++;
    }
    else{
      mean_improvement_ring += v.min_ring/v.fillin_med_ring;
      mean_fillin_improvement_ring += v.fillin_med_ring/v.med_ring;
    }
    if(v.fillin_med_field > v.med_field)
      fillin_worse_field++;
    if(v.fillin_med_ring > v.med_ring)
      fillin_worse_ring++;
  }

  mean_field /= n;
  mean_ring /= n;
  mean_improvement_ring /= n;
  mean_improvement_field /= n;
  mean_fillin_improvement_field /= n;
  mean_fillin_improvement_ring /= n;
  

  cout << nocost << " matrices can be reduced for free." << endl;
  cout << "The mean reduction cost over a field is " << mean_field << "." << endl;
  cout << "The mean reduction cost over a ring is " << mean_ring << "." << endl;
  cout << "In " << fillin_optimal_field <<
    " cases the optimal reduction over a field produces minimum fillin in every step." << endl;
  cout << "In " << fillin_optimal_ring <<
    " cases the optimal reduction over a ring produces minimum fillin in every step." << endl;
  cout << "The maximal cost of reduction over a field is " << max_field << "." << endl;
  cout << "The maximal cost of reduction over a ring is " << max_ring << "." << endl;
  cout << "The optimal pivot takes " << (1-mean_improvement_field)*100 << " percent less operations than min_fillin over a field." << endl;
  cout << "The optimal pivot takes " << (1-mean_improvement_ring)*100 << " percent less operations than min_fillin over a ring." << endl;
  cout << "Min fillin is " << (1-mean_fillin_improvement_field)*100 << " percent better than an arbitrary pivot over a field." << endl;
  cout << "Min fillin is " << (1-mean_fillin_improvement_ring)*100 << " percent better than an arbitrary pivot over a ring." << endl;
  cout << "In " << fillin_worse_field << " cases min_fillin is worse than an arbitrary pivot over a field." << endl;
  cout << "In " << fillin_worse_ring << " cases min_fillin is worse than an arbitrary pivot over a ring." << endl;
  return 0;
}
