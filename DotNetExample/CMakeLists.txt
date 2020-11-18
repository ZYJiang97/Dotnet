# Copyright (c) 2012-2019 ImFusion GmbH, Munich, Germany. All rights reserved.
#
# Main CMake file for ImFusion libraries and applications on Linux and Mac.
# Authors: Markus Mueller, Christian Schulte zu Berge
#
project(ImFusionDotNetExample)
cmake_minimum_required(VERSION 3.8.2)

set(CMAKE_MODULE_PATH C:/Users/zhiyi/Vibronix_dotnet)

set(Qt5_DIR C:/Qt/5.14.2/msvc2017_64/lib/cmake/Qt5)
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
set(ImFusionLib_DIR C:/Users/zhiyi/ImFusionLib/cmake)



if(NOT TARGET ImFusionLib)
	find_package(ImFusionLib COMPONENTS ImFusionUS ImFusionDicom REQUIRED)
endif()

add_subdirectory(ImFusionSuite_CLIWrapper)
add_subdirectory(ImFusionSuite_DotNetForm)


