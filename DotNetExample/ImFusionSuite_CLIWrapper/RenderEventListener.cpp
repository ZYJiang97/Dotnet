
#include <RenderEventListener.h>
#include <ImFusion/Core/Assert.h>

namespace ImFusion
{
	RenderEventListener::RenderEventListener(QObject* listentTo, renderFunction callback)
		:
		m_callback(callback),
		m_object(listentTo)
	{
		IMFUSION_ASSERT(listentTo, "Must be a valid object");
		listentTo->installEventFilter(this);
	}

	void RenderEventListener::close()
	{
		m_object->removeEventFilter(this);
	}

	bool RenderEventListener::eventFilter(QObject* obj, QEvent* e)
	{
		bool ok = QObject::eventFilter(obj, e);

		// update the quick view when the DisplayWidget was updated
		if (m_object != nullptr && obj == m_object && e->type() == QEvent::UpdateRequest)
		{
			m_callback(); // call render
		}

		return ok;
	}
}