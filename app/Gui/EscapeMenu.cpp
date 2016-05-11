#include "EscapeMenu.h"

#include "GuiManager.h"

#include "Engine\GameEngine.h"

EscapeMenu::EscapeMenu() :
	BaseGui(std::string("Adit_Esc_Menu.layout"))
{
	_mainWidget->findWidget("exit")->eventMouseButtonClick += MyGUI::newDelegate(this, &EscapeMenu::onExitClicked);
	_mainWidget->findWidget("resume")->eventMouseButtonClick += MyGUI::newDelegate(this, &EscapeMenu::onResumeClicked);
	_mainWidget->findWidget("settings")->eventMouseButtonClick += MyGUI::newDelegate(this, &EscapeMenu::onSettingsClicked);
	_mainWidget->findWidget("main_menu")->eventMouseButtonClick += MyGUI::newDelegate(this, &EscapeMenu::onMainMenuClicked);
}

void EscapeMenu::onExitClicked(MyGUI::Widget *sender)
{
	GameEngine::inst().getViewer()->setDone(true);
}

void EscapeMenu::onResumeClicked(MyGUI::Widget *sender)
{
	GameEngine::inst().getGui()->removeGuiMode(GuiMode::GUI_Escape_Menu);
}

void EscapeMenu::onSettingsClicked(MyGUI::Widget *sender)
{
	GameEngine::inst().getGui()->addGuiMode(GuiMode::GUI_Settings_Menu);
}

void EscapeMenu::onMainMenuClicked(MyGUI::Widget *sender)
{
	GameEngine::inst().getGui()->removeGuiMode(GuiMode::GUI_Escape_Menu);
}