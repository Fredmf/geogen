//
//  main.cpp
//  hexahedron-sphere
//
//  Created by Grin on 01.03.15.
//  Copyright (c) 2015 FredMF. All rights reserved.
//

#include <iostream>
#include <cstdlib>
#include <string>
#include "hexahedronsphere.h"

int main(int argc, const char * argv[]) {
    unsigned int sublevel = 0;
    std::string file = "test.obj";
    if(argc==2){
        sublevel = atoi(argv[1]);
        if(sublevel == 0){
            sublevel=1;
        }
    }
    if(argc==3){
        sublevel = atoi(argv[1]);
        file = argv[2];
    }
    
    hexahedronsphere testSphere;
    testSphere.subdiv(sublevel);
    testSphere.save(file);
    
    return EXIT_SUCCESS;
}
