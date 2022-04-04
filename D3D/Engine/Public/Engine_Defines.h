#pragma once

#include<d3d9.h>
#include<d3dx9.h>

#define DIRECTINPUT_VERSION 0x0800
#include<dinput.h>

#include<vector>
#include<list>
#include<map>
#include<queue>
#include<algorithm>
#include<unordered_map>
#include<process.h>
#include<typeinfo>


#include "Engine_Typedef.h"
#include "Engine_Function.h"
#include "Engine_Macro.h"
#include "Engine_Functor.h"
#include "Engine_Struct.h"

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

#pragma warning(disable : 4005)
#pragma warning(disable : 4251)
#pragma warning(disable : 4996)

/* ¸¯Ã¼Å© */
#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif

#endif // _DEBUG

using namespace std;
using namespace Engine;