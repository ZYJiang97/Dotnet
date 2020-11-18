# Copyright (c) 2012-2019 ImFusion GmbH, Munich, Germany. All rights reserved.
project(ImFusionSuite_DotNetForm CSharp)
cmake_minimum_required(VERSION 3.8.2)
include(CSharpUtilities)

# [Define and configure the CMake target]
set(Sources
	"Program.cs"
	"ImFusionForm.cs"
	"ImFusionForm.Designer.cs"
)

# Define target library
add_executable(ImFusionSuite_DotNetForm ${Sources})
target_include_directories(ImFusionSuite_DotNetForm PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}			# needed for the UI files
)
# Link agains the ImFusionLib and selected modules/plugins
target_link_libraries(ImFusionSuite_DotNetForm PRIVATE
	ImFusionLib
	ImFusionSuite_CLIWrapper
)

csharp_set_windows_forms_properties(
	"ImFusionForm.cs" 
	"ImFusionForm.Designer.cs" 
	"ImFusionForm.resx"
	"App.config" 
	"Properties/AssemblyInfo.cs" 
	"Properties/Resources.Designer.cs"
	"Properties/Resources.resx"
	"Properties/Settings.Designer.cs"
	"Properties/Settings.settings"
)

set_target_properties(${PROJECT_NAME} PROPERTIES VS_DOTNET_REFERENCES "System;System.Windows.Forms;System.Data;System.Drawing;")

# Copy all ImFusion DLLs to the target build directory
if(EXISTS ${ImFusionLib_DIR})
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND "${CMAKE_COMMAND}" -E copy_directory
		$<$<CONFIG:DEBUG>:${ImFusionLib_DIR}/../SuiteDev>
		$<$<NOT:$<CONFIG:DEBUG>>:${ImFusionLib_DIR}/../Suite>
		$<TARGET_FILE_DIR:${PROJECT_NAME}>
)
endif()