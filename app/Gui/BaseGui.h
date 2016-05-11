#pragma once

#include <MYGUI\MyGUI.h>

class BaseGui
{
public:
	BaseGui(std::string& layout);

	void setTitle(std::string& title);

	void setVisible(bool visible);
protected:
	MyGUI::VectorWidgetPtr _layoutRoot;
	MyGUI::Widget* _mainWidget;
};