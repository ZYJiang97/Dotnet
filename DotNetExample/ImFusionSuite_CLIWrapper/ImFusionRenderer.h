#pragma once
#include <Windows.h> // this has to be included before any "using namespace System" to avoid name clash
#include "ImFusionApp.h"
#include "ManagedObject.h"
#include "MainWindow.h"
#include <RenderEventListener.h>

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Windows::Forms;

namespace ImFusion
{
	namespace CLI
	{
		public ref class ImFusionRenderer : public UserControl
		{
		public:
			ImFusionRenderer(ImFusionApp^ app);
			void Close();

			// methods
			virtual Void Refresh() override;	// the refresh method is not available (empty) for this control to avoid painting 

			// catch the events user events	
			virtual Void OnPaint(PaintEventArgs^ e) override;	// this and the next function was overridden, to avoid painting collisions
			virtual Void OnPaintBackground(PaintEventArgs^ e) override;
			virtual Void OnResize(EventArgs^ e) override;
			virtual Void OnKeyDown(KeyEventArgs^ e) override;
			virtual Void OnKeyPress(KeyPressEventArgs^ e) override;
			virtual Void OnKeyUp(KeyEventArgs^ e) override;
			virtual Void OnMouseClick(MouseEventArgs^ e) override;
			virtual Void OnMouseDoubleClick(MouseEventArgs^ e) override;
			virtual Void OnMouseDown(MouseEventArgs^ e) override;
			virtual Void OnMouseEnter(EventArgs^ e) override;
			virtual Void OnMouseHover(EventArgs^ e) override;
			virtual Void OnMouseLeave(EventArgs^ e) override;
			virtual Void OnMouseMove(MouseEventArgs^ e) override;
			virtual Void OnMouseUp(MouseEventArgs^ e) override;
			virtual Void OnMouseWheel(MouseEventArgs^ e) override;
			virtual Void OnLoad(EventArgs^ e) override;
			virtual Void CreateHandle() override;
			virtual Void DestroyHandle() override;
			virtual Void WndProc(Message% m) override;

		private:
			Void Render();
			Void ActivateOpenGL(IntPtr WindowHandle);
			Void DeactivateOpenGL(IntPtr WindowHandle);

			HGLRC m_glContext;
			HDC m_deviceContext;
			ManagedObject<RenderEventListener>^ m_listener;
			DisplayWidgetMulti* m_disp = nullptr;
		};
	}
}