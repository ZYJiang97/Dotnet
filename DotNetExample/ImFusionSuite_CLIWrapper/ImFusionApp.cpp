#include "ImFusionApp.h"
#include <ImFusion/Core/Log.h>
#include <ImFusion/Core/Log/ConsoleSink.h>
#include <ImFusion/Base/Framework.h>
#include <ImFusion/GL/GlContextManager.h>
#include <ImFusion/GUI/GlContextQt.h>
#include <ImFusion/Base/LicenseManagerSDK.h>
#include <msclr/marshal_cppstd.h>

using namespace msclr::interop;

namespace ImFusion
{
	namespace CLI
	{
		ImFusionApp::ImFusionApp()
		{
			char* m_argv = "ImFusionRenderer";
			int m_argc = 1;

			m_app = gcnew ManagedObject<QApplication>(new QApplication(m_argc, &m_argv));
		}

		bool ImFusionApp::Init()
		{
			try
			{
				// initialize MainWindow
				m_mw = gcnew ManagedObject<MainWindow>(new MainWindow());

				try
				{
					Log::addSink(std::make_unique<Log::ConsoleSink>(Log::ConsoleSink::UseColor));
				}
				catch (const std::exception& e)
				{
					System::Console::WriteLine("Error during logger initialization");
					System::Console::WriteLine(e.what());
				}

				m_currentContextId = (HGLRC)GlContextManager::get().currentContextId();
				return true;
			}
			catch (const MissingLicenseException& ex)
			{
				System::Console::WriteLine(ex.what());
				return false;
			}
		}

		Void ImFusionApp::Update()
		{
			QApplication::processEvents();
		}

		Void ImFusionApp::Close()
		{
			// deletes internal MainWindow. We must do this before the Form closes
			m_mw->reset();
		}

		bool ImFusionApp::Open(System::String^ filename)
		{
			std::string fn = marshal_as<std::string>(filename);
			return m_mw->get()->open(QString::fromStdString(fn));
		}
	}
}