#include "vvMainWindow.h"
#include "ui_vvMainWindow.h"

#include "pqActiveObjects.h"
#include "pqApplicationCore.h"
#include "pqAutoLoadPluginXMLBehavior.h"
#include "pqCommandLineOptionsBehavior.h"
#include "pqCrashRecoveryBehavior.h"
#include "pqInterfaceTracker.h"
#include "pqObjectBuilder.h"
#include "pqPersistentMainWindowStateBehavior.h"
#include "pqQtMessageHandlerBehavior.h"
#include "pqRenderView.h"
#include "pqStandardViewModules.h"
#include "vtkPVPlugin.h"
#include "vvLoadDataReaction.h"

// Declare the plugin to load.
PV_PLUGIN_IMPORT_INIT(VelodyneHDLPlugin);

class vvMainWindow::pqInternals
{
public:
  Ui::vvMainWindow Ui;
  pqInternals(vvMainWindow* window)
    {
    this->Ui.setupUi(window);
    this->paraviewInit(window);
    this->setupUi(window);
    }

private:
  void paraviewInit(vvMainWindow* window)
    {
    pqApplicationCore* core = pqApplicationCore::instance();

    // Register ParaView interfaces.
    pqInterfaceTracker* pgm = core->interfaceTracker();
    pgm->addInterface(new pqStandardViewModules(pgm));

    // Define application behaviors.
    new pqAutoLoadPluginXMLBehavior(window);
    new pqCommandLineOptionsBehavior(window);
    new pqCrashRecoveryBehavior(window);
    new pqPersistentMainWindowStateBehavior(window);
    new pqQtMessageHandlerBehavior(window);

    // Connect to builtin server.
    pqObjectBuilder* builder = core->getObjectBuilder();
    pqServer* server = builder->createServer(pqServerResource("builtin:"));
    pqActiveObjects::instance().setActiveServer(server);

    // Create a default view.
    pqView* view = builder->createView(pqRenderView::renderViewType(),
      server);
    pqActiveObjects::instance().setActiveView(view);
    window->setCentralWidget(view->getWidget());
    }

  void setupUi(vvMainWindow* window)
    {
    new vvLoadDataReaction(this->Ui.action_Open);
    }
};

//-----------------------------------------------------------------------------
vvMainWindow::vvMainWindow() : Internals (new vvMainWindow::pqInternals(this))
{
  PV_PLUGIN_IMPORT(VelodyneHDLPlugin);
}

//-----------------------------------------------------------------------------
vvMainWindow::~vvMainWindow()
{
  delete this->Internals;
  this->Internals = NULL;
}

//-----------------------------------------------------------------------------
