#pragma once

#include <Windows.h> // this has to be included before any "using namespace System" to avoid name clash
#include "ManagedObject.h"
#include "MainWindow.h"
#include <qapplication.h>
#include <ImFusion/GUI/MainWindowBase.h>

using namespace System;

namespace ImFusion
{
	namespace CLI
	{
		public ref class ImFusionApp
		{
		public:
			ImFusionApp();

			bool Init();

			Void Update();

			Void Close();

			HGLRC GetGlContext() { return m_currentContextId; }

			/// Forward calls to internal MainWindow
			bool Open(System::String^ filename);

			ManagedObject<MainWindow>^ mw() { return m_mw; };

		private:
			HGLRC m_currentContextId;

			ManagedObject<QApplication>^ m_app;
			ManagedObject<MainWindow>^ m_mw;
		};
	}
}