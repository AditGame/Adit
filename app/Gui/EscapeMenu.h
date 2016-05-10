#pragma once

#include "BaseGui.h"

class EscapeMenu : public BaseGui
{
public:
	EscapeMenu();

protected:
	void onResumeClicked(MyGUI::Widget *sender);
	void onExitClicked(MyGUI::Widget *sender);
	void onSettingsClicked(MyGUI::Widget *sender);
	void onMainMenuClicked(MyGUI::Widget *sender);
};