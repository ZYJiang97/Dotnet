#include <MainWindow.h>
#include <ImFusion/GUI/DisplayWidgetMulti.h>

#include <ImFusion/Base/Log.h>
#include <ImFusion/Base/Assert.h>
#include <ImFusion/Base/IoAlgorithmFactory.h>
#include <ImFusion/Base/SharedImageSet.h>
#include <ImFusion/GUI/DataController.h>

#include <ImFusion/GUI/ImageView3D.h>
#include <ImFusion/GUI/Animations.h>
#include <ImFusion/GL/GlContext.h>
#include <ImFusion/GL/GlVolumeView.h>
#include <ImFusion/GL/GlVolumeRendererGlobalIllum.h>
#include <ImFusion/Base/DataModel.h>

#include <QFileInfo>
#include <QVBoxLayout>

namespace ImFusion
{
	// Utility function to gather all available IoAlgorithmFactories 
		// (each loaded ImFusion plugin can have their own)
	std::vector<const IoAlgorithmFactory*> getIoAlgorithmFactories()
	{
		std::vector<const IoAlgorithmFactory*> factories;
		for (auto f : FactoryRegistry::get().getAlgorithmFactories())
			if (const IoAlgorithmFactory* ioaf = dynamic_cast<const IoAlgorithmFactory*>(f))
				factories.push_back(ioaf);

		return factories;
	}

	MainWindow::MainWindow()
		:
		MainWindowBase("DotNetExample"),
		m_disp(std::make_unique<DisplayWidgetMulti>())
	{

		m_disp->setBackgroundColor(QColor::fromRgbF(0.1, 0.1, 0.15));

		// per default, create 3 MPR views + 1 3D Volume Renderer View
		// the individual views can be shown/hidden with setViewVisibility()
		// add view 2D too
		m_disp->addView2D(false, true);
		m_disp->addViewGroup3D(false, true);
		IMFUSION_ASSERT(m_disp->numberSliceViews() == 3 && m_disp->number3DViews() == 1);

		m_disp->view3D()->view()->setVr(std::make_unique<GlVolumeRendererGlobalIllum>());
		for (auto* v : m_disp->views())
			v->setVisible(true);


		// initialize layout for widgets
		m_layout = new QVBoxLayout();
		setupWidgets();
	}

	MainWindow::~MainWindow()
	{
		setVisibleData({});
		m_disp.reset();
	}

	void MainWindow::setVisibleData(const ImFusion::DataList& dataList)
	{
		m_disp->showData(dataList);
	}

	QBoxLayout* MainWindow::algorithmDock() const
	{
		return m_layout;
	}

	QBoxLayout* MainWindow::scrollBarLayout() const
	{
		return m_layout;
	}

	DisplayWidgetMulti* MainWindow::display() const
	{
		return m_disp.get();
	}

	void MainWindow::postLoad(SharedImageSet* is)
	{
		DataList dl;
		dl.add(is);
		auto dataControllers = FactoryRegistry::get().compatibleDataControllers(dl);
		for (auto& id : dataControllers)
		{
			// Check if a controller with the given ID has already been created.
			// If so it will be notified by the data selection changed callback.
			bool addNewController = true;
			for (auto& dc : m_dataControllers)
			{
				if (dc->name() == Utils::parseAlgorithmName(id))
				{
					addNewController = false;
					break;
				}
			}
			if (!addNewController)
				continue;
			DataController* dc = FactoryRegistry::get().createDataController(id, dl);
			dc->setMainWindow(this);
			if (QWidget* w = dynamic_cast<QWidget*>(dc))
				w->installEventFilter(this);    // listens for close events
			m_dataControllers.push_back(dc);
		}

		m_disp->showData(is);
	}

}