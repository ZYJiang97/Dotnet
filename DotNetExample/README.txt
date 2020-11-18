-- Setting up the DotNet ImFusion Demo --

Run CMake
Add Qt5 5.13 path. Example: Qt5_DIR  C:\Qt\5.13.0\lib\cmake\Qt5
Check ImFusionLib_DIR points to correct installation.
Generate.

Projects:
ImFusionSuite_CLIWrapper
	- Wrapper around MainWindowBase. Point of entry for SDK applications.
	- ImFusionApp
		- Interaction between Form and Qt events. Creates and owns a MainWindow. Interaction between Form events and customer code (for example Open method)
	- ImFusionRenderer
		- Interaction between Form GL rendering and DisplayWidgetMulti rendering.
	- MainWindow
		- Point of entry of application. Where most customer code will happen, and extensions will be done.
ImFusionSuite_DotNetForm
	- Main form. Connects to ImFusionApp and ImFusionRenderer.
	- Docks the ImFusionRenderer UserControl on the Form.
	- Forwards the Open action from the menu to the ImFusionApp
	

To run, set ImFusionSuite_DotNetForm as Startup Project. Press Start. 