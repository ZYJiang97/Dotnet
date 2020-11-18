#pragma once

#include <QEvent>
#include <QObject>

namespace ImFusion
{
	// from https://docs.microsoft.com/en-us/cpp/dotnet/how-to-marshal-callbacks-and-delegates-by-using-cpp-interop?view=vs-2019
	delegate void RenderDelegate();
	typedef void(__stdcall* renderFunction)();

	public class RenderEventListener : public QObject
	{
	public:
		RenderEventListener(QObject* listentTo, renderFunction callback);

		void close();

		renderFunction m_callback;
		QObject* m_object;

		bool eventFilter(QObject* obj, QEvent* e);
	};
}
