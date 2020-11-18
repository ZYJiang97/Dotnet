# Copyright (c) 2012-2019 ImFusion GmbH, Munich, Germany. All rights reserved.
project(ImFusionSuite_CLIWrapper)
cmake_minimum_required(VERSION 3.8.2)

find_package(OpenGL REQUIRED)

set(Sources
	ImFusionApp.cpp
	ImFusionRenderer.cpp
	MainWindow.cpp
	RenderEventListener.cpp
)
set(Headers
	ImFusionApp.h
	ImFusionRenderer.h
	MainWindow.h
	ManagedObject.h
	RenderEventListener.h
)

# Define target library
add_library(ImFusionSuite_CLIWrapper SHARED ${Sources} ${Headers})
target_include_directories(ImFusionSuite_CLIWrapper PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}			# needed for the UI files
)
# Link agains the ImFusionLib and selected modules/plugins
target_link_libraries(ImFusionSuite_CLIWrapper PRIVATE
	ImFusionLib
	OpenGL::GL
)

set_target_properties(${PROJECT_NAME} PROPERTIES COMMON_LANGUAGE_RUNTIME "" VS_DOTNET_REFERENCES "System;System.Windows.Forms;System.Windows.Forms.Control;")
set_property(TARGET ${PROJECT_NAME} PROPERTY VS_GLOBAL_ROOTNAMESPACE ${PROJECT_NAME})
set_property(TARGET ${PROJECT_NAME} PROPERTY VS_GLOBAL_KEYWORD "ManagedCProj")
set_property(TARGET ${PROJECT_NAME} PROPERTY VS_GLOBAL_CLRSupport "true")
set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DOTNET_TARGET_FRAMEWORK_VERSION "v4.0")