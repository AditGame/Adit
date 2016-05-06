#include "GuiManager.h"

#include "Render\RenderBins.h"


void GuiManager::setupResources()
{
	MYGUIManager::setupResources();
	_platform->getDataManagerPtr()->addResourceLocation(_rootMedia + "/Demos/Demo_Themes", false);
	_platform->getDataManagerPtr()->addResourceLocation(_rootMedia + "/Common/Demos", false);
	_platform->getDataManagerPtr()->addResourceLocation(_rootMedia + "/Common/Themes", false);
}

void GuiManager::initializeControls()
{
	MyGUI::LayoutManager::getInstance().loadLayout("simple.layout");
}

void GuiManager::notifyComboAccept(MyGUI::ComboBox* sender, size_t index)
{
	createDemo(index);
}

void GuiManager::createDemo(int index)
{

}

void GuiManager::destroyDemo()
{
	if (_demoView)
		MyGUI::WidgetManager::getInstance().destroyWidget(_demoView);
	_demoView = NULL;
	_comboSkins = NULL;
}