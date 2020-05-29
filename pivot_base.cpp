/* 
 * pivot_base.cpp
 * 
 * Created: 28.02.2019
 * Author : Jakob Moosbauer
 */

#include "pivot_base.h"

matr invcolmask[] = {0xFEFEFEFEFEFEFEFEULL,0xFDFDFDFDFDFDFDFDULL,0xFBFBFBFBFBFBFBFBULL,
		  0xF7F7F7F7F7F7F7F7ULL,0xEFEFEFEFEFEFEFEFULL,0xDFDFDFDFDFDFDFDFULL,
		  0xBFBFBFBFBFBFBFBFULL,0x7F7F7F7F7F7F7F7FULL};

matr colmask[] = {0x0101010101010101ULL,0x0202020202020202ULL,0x0404040404040404ULL,
		  0x0808080808080808ULL,0x1010101010101010ULL,0x2020202020202020ULL,
		  0x4040404040404040ULL,0x8080808080808080ULL};

/*
 * Modified version of the function converse from the nauty package.
 * Completes a digraph with all edges in one d to a graph.
 */
void completion(graph *g, int n){
  int i,j;
  graph *gi,*gj;
  for (i = 0, gi = g; i < n; ++i, ++gi)
    for (j = i+1, gj = gi+1; j < n; ++j, ++gj)
      if ((ISELEMENT(gi,j)!=0) + (ISELEMENT(gj,i)!=0) == 1)
	  {
	    FLIPELEMENT(gj,i);
	  }
}

void canonisation(mata m, mata cm){
  graph g[MAXN];
  int i;
  for(i = 0; i < 8; i++)
    g[i] = m[i];
  for(; i < MAXN; i++)
    g[i] = 0;
  completion(g, MAXN);

  graph cg[MAXN];

  int orbits[MAXN];
  int lab[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
  int ptn[] = {1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,0};
  static DEFAULTOPTIONS_GRAPH(options);
  options.getcanon = true;
  options.defaultptn = false;
  statsblk stats;
  
  densenauty(g,lab,ptn,orbits,&options,&stats,1,MAXN,cg);

  for(i = 0; i < 8; i++){
    cm[i]=((mata)cg)[2*i];
  }
}

map load_db(string name, int size){
  ifstream input(name+to_string(size-1));
  matr m;
  short s;
  map db;
  while(input.read((char*)&m,8)) {
    input.read((char*)&s,2);
    input.seekg(2,ios::cur);
    db.insert({m,s});
  }
  return db;
}

map2 load_db2(string name, int size){
  ifstream input(name+to_string(size-1));
  matr m;
  values v;
  map2 db;
  while(input.read((char*)&m,8)) {
    input.read((char*)&v.max_field,2);
    input.seekg(2,ios::cur);
    input.read((char*)&v.med_field,4);
    input.read((char*)&v.min_field,2);
    input.seekg(2,ios::cur);
    input.read((char*)&v.fillin_max_field,2);
    input.seekg(2,ios::cur);
    input.read((char*)&v.fillin_med_field,4);
    input.read((char*)&v.fillin_min_field,2);
    input.seekg(2,ios::cur);
    input.read((char*)&v.max_ring,2);
    input.seekg(2,ios::cur);
    input.read((char*)&v.med_ring,4);
    input.read((char*)&v.min_ring,2);
    input.seekg(2,ios::cur);
    input.read((char*)&v.fillin_max_ring,2);
    input.seekg(2,ios::cur);
    input.read((char*)&v.fillin_med_ring,4);
    input.read((char*)&v.fillin_min_ring,2);
    input.seekg(2,ios::cur);
    db.insert({m,v});
  }
  input.close();
  return db;
}

void increase(mata m, char n){
  int i = n-1;
  while(i > 0 && m[i] == m[i-1]){
    m[i]=0;
    i--;
  }
  m[i]++;
  return;
}

int elim(mata m, char n){
  int count=0;
  for(int i = 1; i < n; i++){
    if(m[i]%2){
      count += POPCNT(m[i])+POPCNT(m[0]);
      m[i]=(m[i]|m[0]);
    }
    m[i]>>=1;
  }
  m[0]=0;
  return count;
}



void swap_pivot(mata m, char i, char j){
  if(i>0){
    m[i]^=m[0];
    m[0]^=m[i];
    m[i]^=m[0];
  }
  if(j>0){
    matr x = (*(matr*)m ^ (*(matr*)m>>j)) & 0x0101010101010101ULL;
    *(matr*)m^=x|(x<<j);
  }
}

void printmat(mata m, char n){
  cout << endl;
  for(int i = 0; i < n; i++)
    cout << bitset<8>(m[i]) << endl;
  return;
}

int test_pivots(const mata m, char n, map2& db, char* ops){
    for(int i = 0; i < n; i++){
      if(POPCNT(m[i])==1){
	//cout << endl << "found unit vector in row " << i;
	int j=0;
	while(m[i]>>=1)
	  j++;
	*(matr*)m &= invcolmask[j];
	//cout << " at pos " << j << endl;
	canonisation(m, m);
	values v = db[*(matr*)m];
	
	max_f(ops) = v.max_field;
        max_f_r(ops) = i;
	max_f_c(ops) = j;
	med_f(ops) = v.med_field;
	min_f(ops) = v.min_field;
	min_f_r(ops) = i;
	min_f_c(ops) = j;
	fillin_max_f(ops) = v.fillin_max_field;
        fillin_max_f_r(ops) = i;
	fillin_max_f_c(ops) = j;
   	fillin_med_f(ops) = v.fillin_med_field;
	fillin_min_f(ops) = v.fillin_min_field;
        fillin_min_f_r(ops) = i;
	fillin_min_f_c(ops) = j;
	max_r(ops) = v.max_ring;
        max_r_r(ops) = i;
	max_r_c(ops) = j;
	med_r(ops) = v.med_ring;
	min_r(ops) = v.min_ring;
        min_r_r(ops) = i;
	min_r_c(ops) = j;
	fillin_max_r(ops) = v.fillin_max_ring;
        fillin_max_r_r(ops) = i;
	fillin_max_r_c(ops) = j;
	fillin_med_r(ops) = v.fillin_med_ring;
	fillin_min_r(ops) = v.fillin_min_ring;
        fillin_min_r_r(ops) = i;
	fillin_min_r_c(ops) = j;
	return 1;
      }
    }

    for(int j = 0; j < n; j++){
      if(POPCNTLL((*(matr*)m)&colmask[j])==1){
	//cout << "found unit vector in column " << j;
	matr tempmat = *(matr*)m & colmask[j];
	int i = 0;
	while(tempmat >>= 8)
	  i++;
       	m[i] = 0;
	*(matr*)m &= invcolmask[j];

	//cout << " at pos " << i << endl;
	
	canonisation(m,m);
	values v = db[*(matr*)m];

	max_f(ops) = v.max_field;
        max_f_r(ops) = i;
	max_f_c(ops) = j;
	med_f(ops) = v.med_field;
	min_f(ops) = v.min_field;
	min_f_r(ops) = i;
	min_f_c(ops) = j;
	fillin_max_f(ops) = v.fillin_max_field;
        fillin_max_f_r(ops) = i;
	fillin_max_f_c(ops) = j;
   	fillin_med_f(ops) = v.fillin_med_field;
	fillin_min_f(ops) = v.fillin_min_field;
        fillin_min_f_r(ops) = i;
	fillin_min_f_c(ops) = j;
	max_r(ops) = v.max_ring;
        max_r_r(ops) = i;
	max_r_c(ops) = j;
	med_r(ops) = v.med_ring;
	min_r(ops) = v.min_ring;
        min_r_r(ops) = i;
	min_r_c(ops) = j;
	fillin_max_r(ops) = v.fillin_max_ring;
        fillin_max_r_r(ops) = i;
	fillin_max_r_c(ops) = j;
	fillin_med_r(ops) = v.fillin_med_ring;
	fillin_min_r(ops) = v.fillin_min_ring;
        fillin_min_r_r(ops) = i;
	fillin_min_r_c(ops) = j;
	return 1;
      }
    }
    
    float count_field[64];
    float count_ring[64] = {};
    float count_fillin_field[64];
    float count_fillin_ring[64];
    int fillin;
    int min_fillin = INT_MAX;
    int count_both;
    int tmp;
    int k = 0;
    int num_fillin = 0;
    mata tempmat = new unsigned char[8];

    values v;
    min_f(ops) = SHRT_MAX;
    fillin_min_f(ops) = SHRT_MAX;
    min_r(ops) = SHRT_MAX;
    fillin_min_r(ops) = SHRT_MAX;
    max_f(ops) = 0;
    fillin_max_f(ops) = 0;
    max_r(ops) = 0;
    fillin_max_r(ops) = 0;


    for(int i=0; i<8; i++){
      for(int j=0; j<8; j++){
	if((m[i]>>j)&1){
	  *(matr*)tempmat = *(matr*)m;
	  count_both = 0;
	  for(int r=0; r<8; r++){
	    if(r!=i && ((m[r]>>j)&1)){
	      count_both += POPCNT(m[r]&m[i])-1;
	      count_ring[k] += POPCNT(m[r])-1;
	      tempmat[r]|=m[i];
	    }
	  }
	  tmp = POPCNTLL(colmask[j]&(*(matr*)m))-1;
	  fillin = (POPCNT(m[i])-1) * tmp;
	  count_field[k] = count_both + fillin + tmp;
	  count_ring[k] += count_both + fillin;

	  //cout << "piv:" << k << ", " << i << "," << j << ", f: " << count_field[k] << ", r: " << count_ring[k] << ", fillin: " << fillin << endl;
	  
	  tempmat[i] = 0;
	  *(matr*)tempmat &= invcolmask[j];
	  canonisation(tempmat,tempmat);
	  v = db[*(matr*)tempmat];

  	  if(max_f(ops) < v.max_field + count_field[k]){
	    max_f(ops) = v.max_field + count_field[k];
	    max_f_r(ops) = i;
	    max_f_c(ops) = j;
	  }
	  
	  if(min_f(ops) > v.min_field + count_field[k]){
	    min_f(ops) = v.min_field + count_field[k];
	    min_f_r(ops) = i;
	    min_f_c(ops) = j;
	  }

	  if(max_r(ops) < v.max_ring + count_ring[k]){
	    max_r(ops) = v.max_ring + count_ring[k];
	    max_r_r(ops) = i;
	    max_r_c(ops) = j;
	  }
	  
	  if(min_r(ops) > v.min_ring + count_ring[k]){
	    min_r(ops) = v.min_ring + count_ring[k];
	    min_r_r(ops) = i;
	    min_r_c(ops) = j;
	  }

	  if(fillin < min_fillin){
	    min_fillin = fillin;
	    fillin_min_f(ops) = v.fillin_min_field + count_field[k];
	    fillin_min_f_r(ops) = i;
	    fillin_min_f_c(ops) = j;
	    fillin_max_f(ops) = v.fillin_max_field + count_field[k];
	    fillin_max_f_r(ops) = i;
	    fillin_max_f_c(ops) = j;
	    fillin_min_r(ops) = v.fillin_min_ring + count_ring[k];
	    fillin_min_r_r(ops) = i;
	    fillin_min_r_c(ops) = j;
	    fillin_max_r(ops) = v.fillin_max_ring + count_ring[k];
	    fillin_max_r_r(ops) = i;
	    fillin_max_r_c(ops) = j;

	    count_fillin_field[0] = v.fillin_med_field + count_field[k];
	    count_fillin_ring[0] = v.fillin_med_ring + count_ring[k];
	    num_fillin = 1;
	  }
	  else if(fillin == min_fillin){
	    count_fillin_field[num_fillin] = v.fillin_med_field + count_field[k];
	    count_fillin_ring[num_fillin] = v.fillin_med_ring + count_ring[k];
	    num_fillin++;
	    
	    if(fillin_min_f(ops) > v.fillin_min_field + count_field[k]){
	      fillin_min_f(ops) = v.fillin_min_field + count_field[k];
	      fillin_min_f_r(ops) = i;
	      fillin_min_f_c(ops) = j;
	    }
	    
	    if(fillin_max_f(ops) < v.fillin_max_field + count_field[k]){
	      fillin_max_f(ops) = v.fillin_max_field + count_field[k];
	      fillin_max_f_r(ops) = i;
	      fillin_max_f_c(ops) = j;
	    }
	    
	    if(fillin_min_r(ops) > v.fillin_min_ring + count_ring[k]){
	      fillin_min_r(ops) = v.fillin_min_ring + count_ring[k];
	      fillin_min_r_r(ops) = i;
	      fillin_min_r_c(ops) = j;
	    }
	    
	    if(fillin_max_r(ops) < v.fillin_max_ring + count_ring[k]){
	      fillin_max_r(ops) = v.fillin_max_ring + count_ring[k];
	      fillin_max_r_r(ops) = i;
	      fillin_max_r_c(ops) = j;
	    }
      	  }
	  count_field[k] += v.med_field;
	  count_ring[k] += v.med_ring;
	  k++;
	}
      }
    }
    
    sort(count_field, count_field+k);
    sort(count_ring, count_ring+k);

    med_f(ops) = (count_field[k/2] + count_field[(k-1)/2])/2.0;
    med_r(ops) = (count_ring[k/2] + count_ring[(k-1)/2])/2.0;
    sort(count_fillin_field, count_fillin_field+num_fillin);
    sort(count_fillin_ring, count_fillin_ring+num_fillin);
    fillin_med_f(ops) = (count_fillin_field[num_fillin/2] + count_fillin_field[(num_fillin-1)/2])/2;
    fillin_med_r(ops) = (count_fillin_ring[num_fillin/2] + count_fillin_ring[(num_fillin-1)/2])/2;
    
    return 1;
}

void find_best_pivot(mata m, char n, map& db, short* ops, char* row, char* col){
  mata tempmat = new unsigned char[8];
  int count;
  *ops = 0x7fff;
  for(int i=0; i < n; i++){
    for(int j=0; j < n; j++){
      if((m[i]>>j)%2){
	*(matr*)tempmat = *(matr*)m;
       	swap_pivot(tempmat,i,j);
	count = elim(tempmat,n);
	sort(tempmat,tempmat+n,std::greater<unsigned char>());
	count += db[*(matr*)tempmat];
	if(count<*ops){
	  *ops=count;
	  *row=i;
	  *col=j;
	}
      }
    }
  }
  delete[] tempmat;
}

void select_pivot(mata m, char n, map& db, short* ops, char* row, char* col){
  mata tempmat = new unsigned char[8];
  int tmp;
  int new_zeros = 0x7fffffff;
  int count;
  for(int i=0; i < n; i++){
    for(int j=0; j < n; j++){
      if((m[i]>>j)%2){
	*(matr*)tempmat = *(matr*)m;
       	swap_pivot(tempmat,i,j);
	tmp = 0;
	for(int k=1; k < n; k++){
	  if((tempmat[k]%2)==1)
	    tmp += (((tempmat[0]>>1)&~(tempmat[k]>>1)) * 0x200040008001ULL & 0x111111111111111ULL) % 0xf;
	}
	if(tmp < new_zeros){
	  new_zeros = tmp;
	  count = elim(tempmat,n);
	  sort(tempmat,tempmat+n,std::greater<unsigned char>());
	  count += db[*(matr*)tempmat];
	  *ops=count;
	  *row=i;
	  *col=j;
	}
      }
    }
  }
}

void select_pivot2(mata m, char n, map& db, short* ops, char* row, char* col){
  mata tempmat = new unsigned char[8];
  int tmp;
  int new_zeros = 0x7fffffff;
  int count;
  for(int i=0; i < n; i++){
    for(int j=0; j < n; j++){
      if((m[i]>>j)%2){
	*(matr*)tempmat = *(matr*)m;
       	swap_pivot(tempmat,i,j);
	tmp = 0;
	for(int k=1; k < n; k++){
	  tmp += ((~(tempmat[0]|tempmat[k])>>1) * 0x200040008001ULL & 0x111111111111111ULL) % 0xf;
	}
	if(tmp < new_zeros){
	  new_zeros = tmp;
	  count = elim(tempmat,n);
	  sort(tempmat,tempmat+n,std::greater<unsigned char>());
	  count += db[*(matr*)tempmat];
	  *ops=count;
	  *row=i;
	  *col=j;
	}
      }
    }
  }
}

void write_result(string name, int inputs, matr* mats, char size, short* ops, char* prows, char*pcols){
  ofstream output(name+to_string(size),ios::app);
  for(int in = 0; in < inputs; in++){
    output.write((char*)(mats+in),8);
    output.write((char*)(ops+in),2);
    output.write((char*)(prows+in),1);
    output.write((char*)(pcols+in),1);
  }
  output.close();
}


int test_pivots8(const mata m, map2& db, unsigned short* ops){
  for(int i = 0; i < 8; i++){
      if(POPCNT(m[i])==1){
	int j=0;
	while(m[i]>>=1)
	  j++;
	*(matr*)m &= invcolmask[j];
	canonisation(m, m);
	values v = db[*(matr*)m];

	ops[0] = v.med_field;
	ops[1] = v.min_field;
	ops[2] = v.fillin_med_field;
	ops[3] = v.fillin_min_field;
	ops[4] = v.med_ring;
	ops[5] = v.min_ring;
	ops[6] = v.fillin_med_ring;
	ops[7] = v.fillin_min_ring;
	return 1;
      }
    }

    for(int j = 0; j < 8; j++){
      if(POPCNTLL((*(matr*)m)&colmask[j])==1){
	//cout << "found unit vector in column " << j;
	matr tempmat = *(matr*)m & colmask[j];
	int i = 0;
	while(tempmat >>= 8)
	  i++;
       	m[i] = 0;
	*(matr*)m &= invcolmask[j];

	//cout << " at pos " << i << endl;
	
	canonisation(m,m);
	values v = db[*(matr*)m];

	ops[0] = v.med_field;
	ops[1] = v.min_field;
	ops[2] = v.fillin_med_field;
	ops[3] = v.fillin_min_field;
	ops[4] = v.med_ring;
	ops[5] = v.min_ring;
	ops[6] = v.fillin_med_ring;
	ops[7] = v.fillin_min_ring;
	return 1;
      }
    }
    
    float count_field[64];
    float count_ring[64] = {};
    float count_fillin_field[64];
    float count_fillin_ring[64];
    int fillin;
    int min_fillin = INT_MAX;
    int count_both;
    int tmp;
    int k = 0;
    int num_fillin = 0;
    mata tempmat = new unsigned char[8];

    values v;
    ops[1] = v.min_field = USHRT_MAX;
    ops[3] = v.fillin_min_field = USHRT_MAX;
    ops[5] = v.min_ring = USHRT_MAX;
    ops[7] = v.fillin_min_ring = USHRT_MAX;

    for(int i=0; i<8; i++){
      for(int j=0; j<8; j++){
	if((m[i]>>j)&1){
	  *(matr*)tempmat = *(matr*)m;
	  count_both = 0;
	  for(int r=0; r<8; r++){
	    if(r!=i && ((m[r]>>j)&1)){
	      count_both += POPCNT(m[r]&m[i])-1;
	      count_ring[k] += POPCNT(m[r])-1;
	      tempmat[r]|=m[i];
	    }
	  }
	  tmp = POPCNTLL(colmask[j]&(*(matr*)m))-1;
	  fillin = (POPCNT(m[i])-1) * tmp;
	  count_field[k] = count_both + fillin + tmp;
	  count_ring[k] += count_both + fillin;

	  //cout << "piv:" << k << ", " << i << "," << j << ", f: " << count_field[k] << ", r: " << count_ring[k] << ", fillin: " << fillin << endl;
	  
	  tempmat[i] = 0;
	  *(matr*)tempmat &= invcolmask[j];
	  canonisation(tempmat,tempmat);
	  v = db[*(matr*)tempmat];

	  
	  if(ops[1] > v.min_field + count_field[k]){
	    ops[1] = v.min_field + count_field[k];
	  }
	  
	  if(ops[5] > v.min_ring + count_ring[k]){
	    ops[5] = v.min_ring + count_ring[k];
	  }

	  if(fillin < min_fillin){
	    min_fillin = fillin;
	    ops[3] = v.fillin_min_field + count_field[k];
	    ops[7] = v.fillin_min_ring + count_ring[k];

	    count_fillin_field[0] = v.fillin_med_field + count_field[k];
	    count_fillin_ring[0] = v.fillin_med_ring + count_ring[k];
	    num_fillin = 1;
	  }
	  else if(fillin == min_fillin){
	    count_fillin_field[num_fillin] = v.fillin_med_field + count_field[k];
	    count_fillin_ring[num_fillin] = v.fillin_med_ring + count_ring[k];
	    num_fillin++;
	    
	    if(ops[3] > v.fillin_min_field + count_field[k]){
	      ops[3] = v.fillin_min_field + count_field[k];
	    }
	    
	    if(ops[7] > v.fillin_min_ring + count_ring[k]){
	      ops[7] = v.fillin_min_ring + count_ring[k];
	    }
      	  }
	  
	  count_field[k] += v.med_field;
	  count_ring[k] += v.med_ring;
	  k++;
	}
      }
    }
    
    sort(count_field, count_field+k);
    sort(count_ring, count_ring+k);

    ops[0] = (count_field[k/2] + count_field[(k-1)/2])/2.0;
    ops[4] = (count_ring[k/2] + count_ring[(k-1)/2])/2.0;
    
    sort(count_fillin_field, count_fillin_field+num_fillin);
    sort(count_fillin_ring, count_fillin_ring+num_fillin);
    
    ops[2] = (count_fillin_field[num_fillin/2] + count_fillin_field[(num_fillin-1)/2])/2;
    ops[6] = (count_fillin_ring[num_fillin/2] + count_fillin_ring[(num_fillin-1)/2])/2;
    
    return 1;
}
