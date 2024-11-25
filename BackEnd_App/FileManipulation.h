#ifndef __FILE_MANIPULATION_H
#define __FILE_MANIPULATION_H


#ifdef WIN32
#include <Windows.h>
#define ExportFunc(retType) __declspec( dllimport ) retType extern "C"
#else
#define ExportFunc(retType) retType
#endif


#include "curve.h"
#include "conv_model_exchange.h"
#include "assembly.h"
#include "plane_instance.h"
#include <iostream>
#include <string>
#include <filesystem>
#include <tool_cstring.h>
#include "ShipModel.h"



SPtr<MbCurve> GetSectionFromFile(const c3d::path_string& filePath);

#endif // __FILE_MANIPULATION_H