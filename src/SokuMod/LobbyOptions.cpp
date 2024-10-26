//MOD by Ham. Bugs expected!

#include <functional>
#include <iostream>
#include <utility>
#include "LobbyOptions.hpp"
#include <filesystem>
#include <../directx/dinput.h>
#include "LobbyData.hpp"
#include "InputBox.hpp"
#include "data.hpp"
#include "StatsMenu.hpp"

//Menu style
#define TEXT_SIZE 18
#define DESC_SIZE 8
#define RIGHT_MARGIN 40
#define TOP_MARGIN 150
#define OPTION_POSITION 600

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
	_loadData();

}

LobbyOptions::~LobbyOptions()
{

}


int LobbyOptions::onRender()
{
	this->_title.draw();
	for (auto& item : _options) 
	{
		item.second.nameSprite.draw();
		item.second.descSprite.draw();
		item.second.enableSprite.draw();
		item.second.disableSprite.draw();
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
	return true;
}

void LobbyOptions::_loadData() 
{
	//load preferences from json, not implemented yet
	// 
	//define options
	const std::string enableTxt = "Enable";
	const std::string disableTxt = "Disable";

	OptionDefinition postToHostlist, isReserved, isRanked;
	postToHostlist.name = "Post to Hostlist";
	postToHostlist.description = "Your host will be visible in Konni";
	postToHostlist.isEnabled = false; 
	isReserved.name = "Set as reserved";
	isReserved.description = "Your host will be set as reserved";
	postToHostlist.isEnabled = false; 
	isRanked.name = "Play as Ranked";
	isRanked.description = "If enabled, your game will be count as ranked";
	isRanked.isEnabled = false; 
	//add to map
	_options[POST_TO_HOSTLIST] = postToHostlist;
	_options[IS_RESERVED] = isReserved;
	_options[IS_RANKED] = isRanked;
	int interline = 0;

	for (auto &item : _options) 
	{
		item.second.nameSprite.texture.createFromText(item.second.name.c_str(), lobbyData->getFont(TEXT_SIZE), {400, 100});
		item.second.descSprite.texture.createFromText(item.second.description.c_str(), lobbyData->getFont(TEXT_SIZE), { 400, 100 });
		item.second.enableSprite.texture.createFromText(enableTxt.c_str(), lobbyData->getFont(TEXT_SIZE), {200, 100});
		item.second.disableSprite.texture.createFromText(disableTxt.c_str(), lobbyData->getFont(TEXT_SIZE), { 200, 100 });
		item.second.nameSprite.setSize(item.second.nameSprite.texture.getSize());
		item.second.descSprite.setSize(item.second.descSprite.texture.getSize());
		item.second.enableSprite.setSize(item.second.enableSprite.texture.getSize());
		item.second.disableSprite.setSize(item.second.disableSprite.texture.getSize());
		item.second.nameSprite.setPosition({RIGHT_MARGIN, TOP_MARGIN + interline});
		item.second.enableSprite.setPosition({ OPTION_POSITION, TOP_MARGIN + interline });
		item.second.disableSprite.setPosition({ OPTION_POSITION + 108, TOP_MARGIN + interline });
		item.second.descSprite.setPosition({RIGHT_MARGIN, TOP_MARGIN + interline + TEXT_SIZE });
		item.second.nameSprite.rect.width = item.second.nameSprite.texture.getSize().x;
		item.second.nameSprite.rect.height = item.second.nameSprite.texture.getSize().y;
		item.second.descSprite.rect.width = item.second.descSprite.texture.getSize().x;
		item.second.descSprite.rect.height = item.second.descSprite.texture.getSize().y;
		item.second.enableSprite.rect.width = item.second.enableSprite.texture.getSize().x;
		item.second.enableSprite.rect.height = item.second.enableSprite.texture.getSize().y;
		item.second.disableSprite.rect.width = item.second.disableSprite.texture.getSize().x;
		item.second.disableSprite.rect.height = item.second.disableSprite.texture.getSize().y;
		item.second.disableSprite.tint = SokuLib::Color{ 0x33, 0x33, 0x33 };
		interline += TEXT_SIZE * 2 + DESC_SIZE;
	}
}
