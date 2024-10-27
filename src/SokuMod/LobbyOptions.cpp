//MOD by Ham. Bugs expected!

#include <functional>
#include <iostream>
#include <utility>
#include "LobbyOptions.hpp"
#include <filesystem>
#include <../directx/dinput.h>
#include "LobbyData.hpp"
#include "data.hpp"

//Menu style
#define TEXT_SIZE 18
#define DESC_SIZE 10
#define RIGHT_MARGIN 40
#define TOP_MARGIN 150
#define OPTION_POSITION 388

void displaySokuCursor(SokuLib::Vector2i pos, SokuLib::Vector2u size);

LobbyOptions::LobbyOptions() 
{
	std::filesystem::path folder = profileFolderPath;
	std::map<std::string, std::pair<int, int>> scores;
	SokuLib::Vector2i size;
	this->_title.texture.loadFromFile((folder / "assets/menu/titleConfig.png").string().c_str());
	this->_title.setSize(this->_title.texture.getSize());
	this->_title.setPosition({ 23, 6 });
	this->_title.rect.width = this->_title.getSize().x;
	this->_title.rect.height = this->_title.getSize().y;
	this->_loadData();
}

LobbyOptions::~LobbyOptions()
{

}


int LobbyOptions::onRender()
{
	this->_title.draw();
	displaySokuCursor(_currentOption->nameSprite.getPosition() - SokuLib::Vector2i{ 4, 0 }, { 300, 16 });
	for (auto& item : _options) 
	{
		auto& option = item.second;
		auto tint = option.nameSprite.tint;
		option.nameSprite.draw();
		option.descSprite.draw();
		if (option.isEnabled) 
		{
			option.enableSprite.tint = tint;
			option.disableSprite.tint = SokuLib::Color{ 0xAA, 0xAA, 0xAA };
		}
		else 
		{
			option.enableSprite.tint = SokuLib::Color{ 0xAA, 0xAA, 0xAA };
			option.disableSprite.tint = tint;
		}
		option.disableSprite.draw();
		option.enableSprite.draw(); 
	}
	return 0;
}

void LobbyOptions::_()
{
	//I have no idea what this does, I'm just copypasting code
	puts("_ !");
	*(int*)0x882a94 = 0x16;
}
int LobbyOptions::onProcess()
{
	auto& selected = this->_currentOption;
	if ((SokuLib::inputMgrs.input.horizontalAxis) == 1 || (SokuLib::inputMgrs.input.horizontalAxis) == -1)
	{
		if (selected->isEnabled) 
		{
			selected->isEnabled = false;
		}
		else 
		{
			selected->isEnabled = true; 
		}
		playSound(0x27);
	}
	if ((SokuLib::inputMgrs.input.verticalAxis) == 1 || (SokuLib::inputMgrs.input.verticalAxis) == -1)
	{
		if(SokuLib::inputMgrs.input.verticalAxis == 1)
			selected = selected->NextOption;
		else
			selected = selected->PreviousOption;
		playSound(0x27);
	}
	if (SokuLib::checkKeyOneshot(DIK_ESCAPE, 0, 0, 0) || SokuLib::inputMgrs.input.b == 1)
	{
		this->_savePreferences();
		playSound(0x29);
		return false; 
	}
	return true;
}

void LobbyOptions::_loadData() 
{
	lobbyData->loadPreferences(this->_preferences);
	const std::string enableTxt = "Enable";
	const std::string disableTxt = "Disable";
	OptionDefinition postToHostlist, isReserved, isRanked;
	postToHostlist.name = "Post to Hostlist";
	postToHostlist.description = "Your host will be visible in Konni";
	postToHostlist.isEnabled = this->_preferences.postToHostlist;
	isReserved.name = "Set as reserved";
	isReserved.description = "Your host will be set as reserved";
	isReserved.isEnabled = this->_preferences.isReserved;
	isRanked.name = "Play as Ranked";
	isRanked.description = "If enabled, your game will be count as ranked";
	isRanked.isEnabled = this->_preferences.isRanked;
	//add to map
	_options[POST_TO_HOSTLIST] = postToHostlist;
	_options[IS_RESERVED] = isReserved;
	_options[IS_RANKED] = isRanked;
	int interline = 0;
	LobbyOptions::OptionDefinition* previousNode = NULL;
	LobbyOptions::OptionDefinition* head = NULL;

	for (auto &item : _options) 
	{
		auto& option = item.second;
		auto& optionName = option.nameSprite;
		auto& optionDesc = option.descSprite;
		auto& optionEnable = option.enableSprite;
		auto& optionDisable = option.disableSprite;
		optionName.texture.createFromText(option.name.c_str(), lobbyData->getFont(TEXT_SIZE), {400, 100});
		optionDesc.texture.createFromText(option.description.c_str(), lobbyData->getFont(DESC_SIZE), { 400, 100 });
		optionEnable.texture.createFromText(enableTxt.c_str(), lobbyData->getFont(TEXT_SIZE), {200, 100});
		optionDisable.texture.createFromText(disableTxt.c_str(), lobbyData->getFont(TEXT_SIZE), { 200, 100 });
		optionName.setPosition({RIGHT_MARGIN, TOP_MARGIN + interline});
		optionEnable.setPosition({ OPTION_POSITION, TOP_MARGIN + interline });
		optionDisable.setPosition({ OPTION_POSITION + 80, TOP_MARGIN + interline });
		optionDesc.setPosition({RIGHT_MARGIN, TOP_MARGIN + interline + TEXT_SIZE });
		_setSize(optionName);
		_setSize(optionDesc);
		_setSize(optionEnable);
		_setSize(optionDisable);
		if (option.isEnabled) 
		{
			optionDisable.tint = SokuLib::Color{ 0xAA, 0xAA, 0xAA };
		}
		else 
		{
			optionEnable.tint = SokuLib::Color{ 0xAA, 0xAA, 0xAA };
		}
		interline += TEXT_SIZE * 2 + DESC_SIZE;
		if (previousNode != NULL)
		{
			previousNode->NextOption = &option;
			option.PreviousOption = previousNode;
		}
		else
			head = &option; 
		previousNode = &option;
	}
	if (previousNode != NULL && head != NULL) 
	{
		previousNode->NextOption = head;
		head->PreviousOption = previousNode;
	}
	this->_currentOption = &_options.begin()->second;
}

void LobbyOptions::_setSize(SokuLib::DrawUtils::Sprite& item)
{
	auto size = item.texture.getSize();
	item.setSize(size);
	item.rect.width = size.x;
	item.rect.height = size.y;
}

void LobbyOptions::_savePreferences() 
{
	this->_preferences.isRanked = _options[IS_RANKED].isEnabled;
	this->_preferences.isReserved = _options[IS_RESERVED].isEnabled;
	this->_preferences.postToHostlist = _options[POST_TO_HOSTLIST].isEnabled;
	lobbyData->savePreferences(this->_preferences);
}
