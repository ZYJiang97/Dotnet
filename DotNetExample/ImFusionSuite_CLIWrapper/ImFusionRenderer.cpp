#include "ImFusionRenderer.h"

#include <ImFusion/Base/Log.h>
#include <ImFusion/GL/glad.h>
#include <ImFusion/Base/Utils/Platform.h>
#include <ImFusion/GL/GlUtils.h>
#include <ImFusion/GL/GlContext.h>
#include <ImFusion/GUI/DisplayWidgetMulti.h>

using namespace System;
using namespace System::Windows;
using namespace System::Windows::Forms;

namespace ImFusion
{
	namespace CLI
	{
		ImFusionRenderer::ImFusionRenderer(ImFusionApp^ app)
			: UserControl(),
			m_disp(app->mw()->get()->display()),
			m_glContext(app->GetGlContext())
		{

			// setup listener
			RenderDelegate^ fp = gcnew RenderDelegate(this, &ImFusionRenderer::Render);
			GCHandle gch = GCHandle::Alloc(fp);
			IntPtr ip = Marshal::GetFunctionPointerForDelegate(fp);
			renderFunction cb = static_cast<renderFunction>(ip.ToPointer());
			m_listener = gcnew ManagedObject<RenderEventListener>(new RenderEventListener(app->mw()->get()->display(), cb));
		}

		void ImFusionRenderer::Close()
		{
			m_listener->get()->close();
		}

		Void ImFusionRenderer::ActivateOpenGL(IntPtr WindowHandle)
		{
			HWND hwnd = static_cast<HWND>(WindowHandle.ToPointer());
			GLuint PixelFormat;	// pixel format of the drawing surface

			// get the Device Context associated to the hwnd
			if (!(m_deviceContext = GetDC(hwnd)))
			{
				LOG_ERROR(Utils::getLastWindowsErrorMessage());
				DeactivateOpenGL(WindowHandle);
				return;
			}

			static PIXELFORMATDESCRIPTOR pfd =
			{
				sizeof(PIXELFORMATDESCRIPTOR),
					1,
					PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    // Flags
					PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
					32,                   // Colordepth of the framebuffer.
					0, 0, 0, 0, 0, 0,
					0,
					0,
					0,
					0, 0, 0, 0,
					24,                   // Number of bits for the depthbuffer
					8,                    // Number of bits for the stencilbuffer
					0,                    // Number of Aux buffers in the framebuffer.
					PFD_MAIN_PLANE,
					0,
					0, 0, 0
			};

			// choose the nearest PixelFormat
			if (!(PixelFormat = ChoosePixelFormat(m_deviceContext, &pfd)))
			{
				LOG_ERROR(Utils::getLastWindowsErrorMessage());
				DeactivateOpenGL(WindowHandle);
				return;
			}

			if (!SetPixelFormat(m_deviceContext, PixelFormat, &pfd))
			{
				LOG_ERROR(Utils::getLastWindowsErrorMessage());
				DeactivateOpenGL(WindowHandle);
				return;
			}


			// we need to make the Device and also the RenderingContext current for the actual thread
			// this action is done just once, because each OpenGL component has its own rendering thread
			if (!wglMakeCurrent(m_deviceContext, m_glContext))
			{
				LOG_ERROR(Utils::getLastWindowsErrorMessage());
				DeactivateOpenGL(WindowHandle);
				return;
			}

		}

		Void ImFusionRenderer::DeactivateOpenGL(IntPtr WindowHandle)
		{
			wglMakeCurrent(NULL, NULL);

			if (m_deviceContext)
			{
				// release the device context
				ReleaseDC(static_cast<HWND>(WindowHandle.ToPointer()), m_deviceContext);
			}
		}

		void ImFusionRenderer::Render()
		{
			// perform the rendering
			IntPtr WindowHandle = this->Handle;

			try
			{
				ActivateOpenGL(WindowHandle);
				GLCHECK;

				ImFusion::GlUtils::resetOpenGlStateDefaults();
				GLCHECK;

				m_disp->render(false);
				SwapBuffers(m_deviceContext);
				GLCHECK;
			}
			catch (std::exception& e)
			{
				LOG_ERROR("Rendering error: " << e.what());
			}
		}

		Void ImFusionRenderer::Refresh()
		{
			__super::Refresh();
			Render();
		}

		Void ImFusionRenderer::OnPaint(PaintEventArgs^ e)
		{
			__super::OnPaint(e);
			Render();
		}

		Void ImFusionRenderer::OnPaintBackground(PaintEventArgs^ e)
		{
			__super::OnPaintBackground(e);
		}

		Void ImFusionRenderer::OnResize(EventArgs^ e)
		{
			__super::OnResize(e);
			m_disp->resize(QSize(this->Width, this->Height));
		}


		Void ImFusionRenderer::OnKeyDown(KeyEventArgs^ e)
		{
			__super::OnKeyDown(e);

			Qt::KeyboardModifiers modifiers;

			if (e->Alt) modifiers.setFlag(Qt::KeyboardModifier::AltModifier);
			if (e->Control) modifiers.setFlag(Qt::KeyboardModifier::ControlModifier);
			if (e->Shift) modifiers.setFlag(Qt::KeyboardModifier::ShiftModifier);
			QApplication::sendEvent(m_disp, new QKeyEvent(QEvent::Type::KeyPress, e->KeyValue, modifiers));
		}


		Void ImFusionRenderer::OnKeyPress(KeyPressEventArgs^ e)
		{
			__super::OnKeyPress(e);
			// already sent keydown and keyup
		}


		Void ImFusionRenderer::OnKeyUp(KeyEventArgs^ e)
		{
			__super::OnKeyUp(e);

			Qt::KeyboardModifiers modifiers;

			if (e->Alt) modifiers.setFlag(Qt::KeyboardModifier::AltModifier);
			if (e->Control) modifiers.setFlag(Qt::KeyboardModifier::ControlModifier);
			if (e->Shift) modifiers.setFlag(Qt::KeyboardModifier::ShiftModifier);
			QApplication::sendEvent(m_disp, new QKeyEvent(QEvent::Type::KeyRelease, e->KeyValue, modifiers));
		}

		Void ImFusionRenderer::OnMouseClick(MouseEventArgs^ e)
		{
			__super::OnMouseClick(e);
			// already sending mouse down and up
		}


		Void ImFusionRenderer::OnMouseDoubleClick(MouseEventArgs^ e)
		{
			__super::OnMouseDoubleClick(e);

			Qt::MouseButton	button = Qt::MouseButton::NoButton;
			Qt::MouseButtons buttons;
			Qt::KeyboardModifiers modifiers;

			if (e->Button == Forms::MouseButtons::Right) button = Qt::MouseButton::RightButton;
			else if (e->Button == Forms::MouseButtons::Left) button = Qt::MouseButton::LeftButton;
			else if (e->Button == Forms::MouseButtons::Middle) button = Qt::MouseButton::MiddleButton;

			if (this->MouseButtons == Forms::MouseButtons::Left) buttons.setFlag(Qt::MouseButton::LeftButton);
			if (this->MouseButtons == Forms::MouseButtons::Right) buttons.setFlag(Qt::MouseButton::RightButton);
			if (this->MouseButtons == Forms::MouseButtons::Middle) buttons.setFlag(Qt::MouseButton::MiddleButton);

			if (this->ModifierKeys == Forms::Keys::Alt) modifiers.setFlag(Qt::KeyboardModifier::AltModifier);
			if (this->ModifierKeys == Forms::Keys::Control) modifiers.setFlag(Qt::KeyboardModifier::ControlModifier);
			if (this->ModifierKeys == Forms::Keys::Shift) modifiers.setFlag(Qt::KeyboardModifier::ShiftModifier);

			QApplication::sendEvent(m_disp, new QMouseEvent(QEvent::Type::MouseButtonDblClick, QPointF(e->X, e->Y), button, buttons, modifiers));
		}


		Void ImFusionRenderer::OnMouseDown(MouseEventArgs^ e)
		{
			__super::OnMouseDown(e);

			Qt::MouseButton	button = Qt::MouseButton::NoButton;
			Qt::MouseButtons buttons;
			Qt::KeyboardModifiers modifiers;

			if (e->Button == Forms::MouseButtons::Right) button = Qt::MouseButton::RightButton;
			else if (e->Button == Forms::MouseButtons::Left) button = Qt::MouseButton::LeftButton;
			else if (e->Button == Forms::MouseButtons::Middle) button = Qt::MouseButton::MiddleButton;

			if (this->MouseButtons == Forms::MouseButtons::Left) buttons.setFlag(Qt::MouseButton::LeftButton);
			if (this->MouseButtons == Forms::MouseButtons::Right) buttons.setFlag(Qt::MouseButton::RightButton);
			if (this->MouseButtons == Forms::MouseButtons::Middle) buttons.setFlag(Qt::MouseButton::MiddleButton);

			if (this->ModifierKeys == Forms::Keys::Alt) modifiers.setFlag(Qt::KeyboardModifier::AltModifier);
			if (this->ModifierKeys == Forms::Keys::Control) modifiers.setFlag(Qt::KeyboardModifier::ControlModifier);
			if (this->ModifierKeys == Forms::Keys::Shift) modifiers.setFlag(Qt::KeyboardModifier::ShiftModifier);

			QApplication::sendEvent(m_disp, new QMouseEvent(QEvent::Type::MouseButtonPress, QPointF(e->X, e->Y), button, buttons, modifiers));
		}


		Void ImFusionRenderer::OnMouseEnter(EventArgs^ e)
		{
			__super::OnMouseEnter(e);
			QApplication::sendEvent(m_disp, new QFocusEvent(QEvent::Type::FocusIn, Qt::FocusReason::MouseFocusReason));
		}


		Void ImFusionRenderer::OnMouseHover(EventArgs^ e)
		{
			__super::OnMouseHover(e);
			QApplication::sendEvent(m_disp, new QHoverEvent(QEvent::Type::HoverMove, QPointF(), QPointF())); // TODO 
		}


		Void ImFusionRenderer::OnMouseLeave(EventArgs^ e)
		{
			__super::OnMouseLeave(e);
			QApplication::sendEvent(m_disp, new QFocusEvent(QEvent::Type::FocusOut, Qt::FocusReason::MouseFocusReason));
		}


		Void ImFusionRenderer::OnMouseMove(MouseEventArgs^ e)
		{
			__super::OnMouseMove(e);

			Qt::MouseButton	button = Qt::MouseButton::NoButton;
			Qt::MouseButtons buttons;
			Qt::KeyboardModifiers modifiers;

			if (e->Button == Forms::MouseButtons::Right) button = Qt::MouseButton::RightButton;
			else if (e->Button == Forms::MouseButtons::Left) button = Qt::MouseButton::LeftButton;
			else if (e->Button == Forms::MouseButtons::Middle) button = Qt::MouseButton::MiddleButton;

			if (this->MouseButtons == Forms::MouseButtons::Left) buttons.setFlag(Qt::MouseButton::LeftButton);
			if (this->MouseButtons == Forms::MouseButtons::Right) buttons.setFlag(Qt::MouseButton::RightButton);
			if (this->MouseButtons == Forms::MouseButtons::Middle) buttons.setFlag(Qt::MouseButton::MiddleButton);

			if (this->ModifierKeys == Forms::Keys::Alt) modifiers.setFlag(Qt::KeyboardModifier::AltModifier);
			if (this->ModifierKeys == Forms::Keys::Control) modifiers.setFlag(Qt::KeyboardModifier::ControlModifier);
			if (this->ModifierKeys == Forms::Keys::Shift) modifiers.setFlag(Qt::KeyboardModifier::ShiftModifier);

			QApplication::sendEvent(m_disp, new QMouseEvent(QEvent::Type::MouseMove, QPointF(e->X, e->Y), button, buttons, modifiers));
		}


		Void ImFusionRenderer::OnMouseUp(MouseEventArgs^ e)
		{
			__super::OnMouseUp(e);

			Qt::MouseButton	button;
			Qt::MouseButtons buttons;
			Qt::KeyboardModifiers modifiers;

			if (e->Button == Forms::MouseButtons::Right) button = Qt::MouseButton::RightButton;
			else if (e->Button == Forms::MouseButtons::Left) button = Qt::MouseButton::LeftButton;
			else if (e->Button == Forms::MouseButtons::Middle) button = Qt::MouseButton::MiddleButton;

			if (this->MouseButtons == Forms::MouseButtons::Left) buttons.setFlag(Qt::MouseButton::LeftButton);
			if (this->MouseButtons == Forms::MouseButtons::Right) buttons.setFlag(Qt::MouseButton::RightButton);
			if (this->MouseButtons == Forms::MouseButtons::Middle) buttons.setFlag(Qt::MouseButton::MiddleButton);

			if (this->ModifierKeys == Forms::Keys::Alt) modifiers.setFlag(Qt::KeyboardModifier::AltModifier);
			if (this->ModifierKeys == Forms::Keys::Control) modifiers.setFlag(Qt::KeyboardModifier::ControlModifier);
			if (this->ModifierKeys == Forms::Keys::Shift) modifiers.setFlag(Qt::KeyboardModifier::ShiftModifier);

			QApplication::sendEvent(m_disp, new QMouseEvent(QEvent::Type::MouseButtonRelease, QPointF(e->X, e->Y), button, buttons, modifiers));
		}


		Void ImFusionRenderer::OnMouseWheel(MouseEventArgs^ e)
		{
			__super::OnMouseWheel(e);

			Qt::MouseButtons buttons;
			Qt::KeyboardModifiers modifiers;

			if (this->MouseButtons == Forms::MouseButtons::Left) buttons.setFlag(Qt::MouseButton::LeftButton);
			if (this->MouseButtons == Forms::MouseButtons::Right) buttons.setFlag(Qt::MouseButton::RightButton);
			if (this->MouseButtons == Forms::MouseButtons::Middle) buttons.setFlag(Qt::MouseButton::MiddleButton);

			if (this->ModifierKeys == Forms::Keys::Alt) modifiers.setFlag(Qt::KeyboardModifier::AltModifier);
			if (this->ModifierKeys == Forms::Keys::Control) modifiers.setFlag(Qt::KeyboardModifier::ControlModifier);
			if (this->ModifierKeys == Forms::Keys::Shift) modifiers.setFlag(Qt::KeyboardModifier::ShiftModifier);


			QApplication::sendEvent(m_disp, new QWheelEvent(QPointF(e->X, e->Y), e->Delta, buttons, modifiers));
		}
		Void ImFusionRenderer::OnLoad(EventArgs^ e)
		{
			__super::OnLoad(e);

			return Void();
		}
		Void ImFusionRenderer::CreateHandle()
		{
			__super::CreateHandle();

			return Void();
		}
		Void ImFusionRenderer::DestroyHandle()
		{
			__super::DestroyHandle();

			return Void();
		}
		Void ImFusionRenderer::WndProc(Message% m)
		{
			__super::WndProc(m);

			return Void();
		}
	}
}