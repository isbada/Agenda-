//
//  main.cpp
//  Agenda
//
//  Created by fh on 2017/3/15.
//  Copyright © 2017年 付豪. All rights reserved.
//
#include <list>
#include <functional>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "AgendaUI.h"

using namespace std;

AgendaUI aui;

int main(int argc, const char * argv[]) {
    aui.OperationLoop();
    
    return 0;
}
