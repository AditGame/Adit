#include "BaseGui.h"

#include <MYGUI\MyGUI.h>

BaseGui::BaseGui(std::string & layout)
{
	std::string mainWidgetName = "main";

	_layoutRoot = MyGUI::LayoutManager::getInstance().loadLayout(layout);
	_mainWidget = nullptr;
	for (MyGUI::VectorWidgetPtr::iterator it = _layoutRoot.begin(); it != _layoutRoot.end(); it++)
	{
		MyGUI::Widget* ptr = *it;
		if (ptr->getName() == mainWidgetName)
		{
			_mainWidget = ptr;
			return;
		}
	}
	std::cout << "Could not find widget named " + mainWidgetName + " in layout " + layout << std::endl;
}

void BaseGui::setTitle(std::string& title)
{
	static_cast<MyGUI::Window*>(_mainWidget)->setCaptionWithReplacing(title);
}

void BaseGui::setVisible(bool visible)
{
	_mainWidget->setVisible(visible);
}
