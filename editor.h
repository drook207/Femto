#pragma once

#include <cstdint>
#include <vector>

#include "textbuffer.h"
#include "utils.h"

namespace Femto {

class Editor {
public:
	struct Size {
		uint16_t rows {};
		uint16_t columns {};
	};

	struct Cursor {
		uint16_t x {};
		uint16_t y {};
	};

	Editor();

	static constexpr auto CTRL_KEY(const char& k) { return k & 0x1f; };

	void run();
	char readKey();
	void processKeypress();
	void refreshScreen();
	void moveCursorHome();
	void moveCursor(const char key);
	void hideCursor();
	void showCursor();
	void drawRows();
	void clearLine();
	[[nodiscard]] bool getWindowSize(OUT_PARAMETER Size& windowSize);

private:
	[[nodiscard]] bool getWindowSize();
	[[nodiscard]] bool getWindowSizeWithCursor();
	[[nodiscard]] bool getWindowSizeWithCursor(OUT_PARAMETER Size& windowSize);
	[[nodiscard]] bool getCursorPosition();
	[[nodiscard]] bool getCursorPosition(OUT_PARAMETER Size& windowSize);
	Size mWindowSize {};
	Cursor mCursor {};
	TextBuffer mBuffer { 256 };
};

} // namespace Femto
