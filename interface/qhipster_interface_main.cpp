//------------------------------------------------------------------------------
// Copyright 2017 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//------------------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <functional>
#include <stdexcept>
#include <cstdlib>

#include "qureg/qureg.hpp"
#include "interface_api_qasm.h"

using namespace std;


// Global variables related to Psi-function .malloc/.free routines.
using Type = ComplexDP;
NoisyQureg<Type> *psi1 = nullptr;
bool fPsiAllocated = false;

unsigned if_RNG_seed=12345;
double if_T1=100000, if_T2=100000;

int main(int argc, char*argv[]) {
    openqu::mpi::Environment env(argc, argv);
    string line = "";
    string token = "";

    if (argc > 1)
      if_RNG_seed = atoi(argv[1]);
    if (argc > 2)
      if_T1 = atof(argv[2]);
    if (argc > 3)
      if_T2 = atof(argv[3]);
    
    if (env.is_usefull_rank() == false) 
        return 0;

    int myid = env.rank();
    using Type = ComplexDP;

    while(true) {
        getline(cin,line);

        if(line.length() >= 1) {
            int token_end = line.find_first_of(' ');
            unsigned long result = 1;

            token = line.substr(0,token_end);
            if(!token.empty()) {
	/*
               function<long(string)> func = qufun_table[token];
               if(func) {
                  result = func(line.substr(token_end+1,line.length()));
               }
*/
	       result = ExecuteHandler(token,line.substr(token_end+1,line.length()));

               if (result > 0) {
                   cerr << "Qasm Op failed - ["<<token<<"]"<<endl;
               }
               if (fPsiAllocated)
                 psi1->apply_noise_gates_on_all_qubits();
            }
        } else
          break;
    }

return 0;
}
