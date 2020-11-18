#pragma once
#include <ImFusion/GUI/MainWindowBase.h>

namespace ImFusion
{
	class DisplayWidgetMulti;

	public class MainWindow : public MainWindowBase
	{
	public:
		MainWindow();
		~MainWindow();

		// Inherited via MainWindowBase
		virtual QBoxLayout* algorithmDock() const override;
		virtual QBoxLayout* scrollBarLayout() const override;
		virtual DisplayWidgetMulti* display() const override;
		virtual void postLoad(SharedImageSet* is) override;

		void close();

	private:
		void setVisibleData(const ImFusion::DataList& dataList);

		std::unique_ptr<DisplayWidgetMulti> m_disp;

		// fake QBoxLayouts, not used
		QBoxLayout* m_layout;

	};
}