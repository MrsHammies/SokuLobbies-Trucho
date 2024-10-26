//
// Created by PinkySmile on 02/10/2022.
//

#include <mutex>
#include <thread>
#include <SokuLib.hpp>
#include "Player.hpp"
#include "Socket.hpp"

class LobbyOptions : public SokuLib::IMenu {
private:
	enum Options {
		POST_TO_HOSTLIST,
		IS_RESERVED,
		MAX_PING_ACCEPTABLE,
		PING_IF_MENTION,
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
	};

	SokuLib::DrawUtils::Sprite _title;
	std::map<unsigned, SokuLib::SWRFont> _fonts;
	std::map<Options, OptionDefinition> _options; 
	
	void _inCategoryUpdate();
	void _inCategoryRender();
	void _loadData(); 

public:
	LobbyOptions();
	~LobbyOptions();
	void _() override;
	int onProcess() override;
	int onRender() override;
};

