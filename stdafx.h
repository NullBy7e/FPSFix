#pragma once

#define WIN32_LEAN_AND_MEAN		
#include <windows.h>

#include <fstream>
#include <ostream>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <strsafe.h>
#include <TlHelp32.h>
#include <comdef.h>
#include <string.h>
#include "d3d9.h"
#include <sstream> 
#include <string>
#include <direct.h>
#include "Pointer.h"
typedef std::basic_string<TCHAR> tstring;
#pragma comment(lib,"ws2_32.lib")