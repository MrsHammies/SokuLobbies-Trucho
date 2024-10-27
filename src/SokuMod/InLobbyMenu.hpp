//
// Created by PinkySmile on 02/10/2022.
//

#ifndef SOKULOBBIES_INLOBBYMENU_HPP
#define SOKULOBBIES_INLOBBYMENU_HPP


#include <mutex>
#include <thread>
#include <queue>
#include <Socket.hpp>
#include <SokuLib.hpp>
#include "Connection.hpp"
#include "LobbyData.hpp"
#include "LobbyMenu.hpp"

class InLobbyMenu : public SokuLib::IMenu {
private:
	struct PlayerData {
		SokuLib::DrawUtils::Sprite name;
	};
	struct MessageText {
		SokuLib::DrawUtils::Sprite sprite;
		SokuLib::Vector2i realSize;
		SokuLib::Vector2i pos;
	};
	struct MessageEmote {
		unsigned id;
		unsigned cutRemain;
		SokuLib::Vector2i pos;
		SokuLib::Vector2i offset;
	};
	struct Message {
		std::vector<MessageEmote> emotes;
		std::list<MessageText> text;
		bool farDown = false;
		bool farUp = false;
	};
	struct ArcadeMachine {
		unsigned id;
		SokuLib::Vector2i pos;
		LobbyData::ArcadeAnimation *currentAnim;
		LobbyData::ArcadeSkin &skin;
		std::mutex mutex;
		unsigned skinAnimationCtr = 0;
		unsigned skinAnimation = 0;
		unsigned animationCtr = 0;
		unsigned animation = 0;
		unsigned playerCount = 0;
		bool animIdle = false;

		ArcadeMachine(unsigned id, SokuLib::Vector2i pos, LobbyData::ArcadeAnimation *currentAnim, LobbyData::ArcadeSkin &skin);
		ArcadeMachine(const ArcadeMachine &);
	};
	struct ElevatorMachine {
		unsigned id;
		SokuLib::Vector2i pos;
		LobbyData::ElevatorSkin &skin;
		LobbyData::ElevatorPlacement &links;
		std::mutex mutex;
		unsigned char skinAnimationCtr = 0;
		char skinAnimation = 0;
		char animation = 0;
		unsigned char state = 0;

		ElevatorMachine(unsigned id, SokuLib::Vector2i pos, LobbyData::ElevatorPlacement &links, LobbyData::ElevatorSkin &skin);
		ElevatorMachine(const ElevatorMachine &);
	};
	struct MessageBoxArgs {
		int sound;
		std::string text;
		std::string title;
		UINT type;
	};

	std::function<void (const std::string &ip, unsigned short port, bool spectate)> onConnectRequest;
	std::function<void (const std::string &msg)> onError;
	std::function<void (const std::string &msg)> onImpMsg;
	std::function<void (int32_t channel, int32_t player, const std::string &msg)> onMsg;
	std::function<void (const Player &)> onPlayerJoin;
	std::function<unsigned short ()> onHostRequest;
	std::function<void (const Lobbies::PacketOlleh &)> onConnect;
	std::function<void (const Player &, uint32_t id)> onArcadeEngage;
	std::function<void (const Player &, uint32_t id)> onArcadeLeave;
	SokuLib::Vector2i _translate{0, 0};
	SokuLib::Vector2i _translateStep{0, 0};
	SokuLib::Vector2i _translateTarget{0, 0};
	unsigned char _translateAnimation = 0;
	std::string _roomName;
	LobbyMenu *_menu;
	ArcadeMachine *_currentMachine = nullptr;
	ElevatorMachine *_currentElevator = nullptr;
	SokuLib::Vector2u _camera;
	float _zoom = 1;
	std::shared_ptr<Connection> _connection;
	SokuLib::MenuConnect *_parent;
	bool _wasConnected = false;
	bool _disconnected = false;
	std::vector<Player> _playersCopy;
	unsigned _chatTimer = 0;
	unsigned _chatOffset = 0;
	uint8_t _background = 0;
	std::string _music;
	SokuLib::DrawUtils::Sprite _chatSeat;
	SokuLib::DrawUtils::Sprite _loadingText;
	SokuLib::DrawUtils::Sprite _messageBox;
	SokuLib::DrawUtils::Sprite _loadingGear;
	SokuLib::DrawUtils::Sprite _battleStatus[3];
	std::list<Message> _chatMessages;
	std::mutex _chatMessagesMutex;
	std::map<uint32_t, PlayerData> _extraPlayerData;
	std::wstring _buffer;
	std::vector<ArcadeMachine> _machines;
	std::vector<ElevatorMachine> _elevators;
	std::unique_ptr<class SmallHostlist> _hostlist;
	std::vector<uint32_t> _insideElevator;
	std::thread _connectThread;
	unsigned char _elevatorCtr = 0;
	bool _elevatorOut = false;
	std::queue<MessageBoxArgs> _messageBoxQueue;
	std::mutex _messageBoxQueueMutex;
	std::thread _messageBoxThread;

	// Chat input box
	SokuLib::SWRFont _chatFont;
	SokuLib::DrawUtils::RectangleShape _textCursor;
	SokuLib::DrawUtils::Sprite _textSprite[2];
	std::mutex _textMutex;
	std::thread _hostThread;
	unsigned _timers[256];
	unsigned _textTimer = 0;
	unsigned _lastPressed = 0;
	unsigned _currentPlatform = 0;
	int _textCursorPosIndex = 0;
	int _textCursorPosSize = 0;
	bool _editingText = false;
	bool _returnPressed = false;
	std::map<unsigned, int> _textSize;

	void _updateMessageSprite(SokuLib::Vector2i pos, unsigned int remaining, SokuLib::Vector2i realSize, SokuLib::DrawUtils::Sprite &sprite, unsigned char alpha);
	void _addMessageToList(unsigned channel, unsigned player, const std::string &msg);
	void _inputBoxUpdate();
	void _initInputBox();
	void _updateTextCursor(int pos);
	void _sendMessage(const std::wstring &msg);
	void _unhook();
	void _renderMachineOverlay();
	void _startHosting();
	void _updateCompositionSprite();
	void _openMessageBox(int sound, const std::string &text, const std::string &title, UINT type);
	int _getTextSize(unsigned i);

	//custom commands

	struct Cmd 
	{
		std::string usage;
		std::string description;
		void (InLobbyMenu::*callback)(const std::vector<std::string> &msg);
	};
	std::vector<std::string> _parseCommand(const std::string &msg);
	void _processCommands(const std::string& msg);
	static const std::map<std::string, Cmd> _commands;
	void _helpCmd(const std::vector<std::string> &args);	

	//preferences
	LobbyData::Preferences _preferences; 
	void _updatePreferences(); 

public:
	char textChanged = 0;
	HIMC immCtx = nullptr;
	std::wstring immComposition;
	bool hasDeadkey = false;
	int compositionCursor = 0;
	int keyBufferUsed = 0;
	wchar_t keyBuffer[2] = {0, 0};
	bool wineWorkaroundNeeded = false;

	InLobbyMenu(LobbyMenu *menu, SokuLib::MenuConnect *parent, std::shared_ptr<Connection> &connection);
	~InLobbyMenu();
	void _() override;
	int onProcess() override;
	int onRender() override;

	void onKeyPressed(unsigned chr);
	void onKeyReleased();
	void addString(wchar_t *str, size_t size);
	void onCompositionResult();

	void updateChat(bool inGame);
	void renderChat();
	bool isInputing();
};

extern InLobbyMenu *activeMenu;


#endif //SOKULOBBIES_INLOBBYMENU_HPP
