//
// Created by PinkySmile on 02/10/2022.
//

#include <mutex>
#include <thread>
#include <SokuLib.hpp>
#include "Player.hpp"
#include "Socket.hpp"
#include "LobbyData.hpp"


class LobbyOptions : public SokuLib::IMenu {
private:
	enum Options {
		POST_TO_HOSTLIST,
		IS_RESERVED,
		//MAX_PING_ACCEPTABLE,
		//PING_IF_MENTION,
		IS_RANKED
	};

	struct OptionDefinition {
		std::string name;
		std::string description;
		bool isEnabled;
		SokuLib::DrawUtils::Sprite nameSprite;
		SokuLib::DrawUtils::Sprite descSprite;
		SokuLib::DrawUtils::Sprite enableSprite;
		SokuLib::DrawUtils::Sprite disableSprite;
		OptionDefinition* PreviousOption;
		OptionDefinition* NextOption;
	};

	SokuLib::DrawUtils::Sprite _title;
	std::map<Options, OptionDefinition> _options; 
	OptionDefinition* _currentOption; 
	LobbyData::Preferences _preferences; 
	void _loadData(); 
	void _setSize(SokuLib::DrawUtils::Sprite& item);
	void _savePreferences();

public:
	struct Preferences {
		bool isRanked;
		bool postToHostlist;
		bool isReserved;
	};

	LobbyOptions();
	~LobbyOptions();
	void _() override;
	int onProcess() override;
	int onRender() override;
};

