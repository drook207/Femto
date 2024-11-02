#include "editor.h"

#include <sys/ioctl.h>
#include <unistd.h>

#include <array>
#include <cerrno>

#include "terminal.h"

namespace Femto {

Editor::Editor()
{
	Terminal::enableRawMode();
	if (!getWindowSize()) {
		Utils::onError("Failed to get window size!");
	}
}

void Editor::run()
{
	while (true) {
		refreshScreen();
		processKeypress();
	}
}

char Editor::readKey()
{
	int nread;
	char c;
	while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
		char c = '\0';
		if ((nread == -1) && (errno != EAGAIN)) {
			Utils::onError("Failed to read input data!");
		}
	}
	return c;
}

void Editor::processKeypress()
{
	const auto c = readKey();

	switch (c) {
		case CTRL_KEY('q'):
			Utils::clearScreen();
			std::exit(0);
			break;
	}
}

void Editor::refreshScreen()
{
	hideCursor();
	drawRows();

	write(STDOUT_FILENO, mBuffer.data(), mBuffer.size());

	showCursor();
	mBuffer.release();
}

void Editor::moveCursorHome() { mBuffer.append("\x1b[H"); }

void Editor::moveCursor(const char key)
{
	switch (key) {
		case 'a':
			mCursor.x--;
			break;
		case 'd':
			mCursor.x++;
			break;
		case 'w':
			mCursor.y--;
			break;
		case 's':
			mCursor.y++;
			break;
	}
}

void Editor::hideCursor() { mBuffer.append("\x1b[?25l"); }

void Editor::showCursor() { mBuffer.append("\x1b[?25h"); }

void Editor::drawRows()
{
	for (uint16_t i = 0; i < mWindowSize.rows; i++) {
		if (i == mWindowSize.rows / 3) {
			std::string welcomeMessage = "Femto editor -- ";
			welcomeMessage += FEMTO_VERSION;
			mBuffer.append(welcomeMessage);
		} else {
			mBuffer.append("~");
		}
		clearLine();
		if (i < mWindowSize.rows) {
			mBuffer.append("\r\n");
		}
	}
}

void Editor::clearLine()
{
	// Set mode

	mBuffer.append("\x1b[K");
}

bool Editor::getWindowSize(Size& windowSize)
{
	struct winsize ws;

	if ((ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1) || (ws.ws_col == 0)) {
		return getWindowSizeWithCursor(windowSize);
	}

	windowSize.columns = ws.ws_col;
	windowSize.rows = ws.ws_row;
	return true;
}

bool Editor::getWindowSize()
{
	struct winsize ws;

	if ((ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1) || (ws.ws_col == 0)) {
		return getWindowSizeWithCursor();
	}

	mWindowSize.columns = ws.ws_col;
	mWindowSize.rows = ws.ws_row;
	return true;
}

bool Editor::getWindowSizeWithCursor()
{
	if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) {
		return false;
	}
	return getCursorPosition();
}

bool Editor::getWindowSizeWithCursor(Size& windowSize)
{
	if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) {
		return false;
	}
	return getCursorPosition(windowSize);
}

bool Editor::getCursorPosition()
{
	if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4) {
		return false;
	}
	std::array<char, 32> buffer {};

	for (auto& c : buffer) {
		if (read(STDIN_FILENO, &c, 1) != 1) {
			break;
		}
		if (c == 'R') {
			c = '\0';
			break;
		}
	}

	if (buffer[0] != '\x1b' || buffer[1] != '[') {
		return false;
	}

	if (std::sscanf(&buffer[2], "%hd;%hd", &mWindowSize.rows, &mWindowSize.columns) != 2) {
		return false;
	}

	return true;
}

bool Editor::getCursorPosition(Size& windowSize)
{
	if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4) {
		return false;
	}
	std::array<char, 32> buffer {};

	for (auto& c : buffer) {
		if (read(STDIN_FILENO, &c, 1) != 1) {
			break;
		}
		if (c == 'R') {
			c = '\0';
			break;
		}
	}

	if (buffer[0] != '\x1b' || buffer[1] != '[') {
		return false;
	}

	if (std::sscanf(&buffer[2], "%hd;%hd", &windowSize.rows, &windowSize.columns) != 2) {
		return false;
	}

	return true;
}

} // namespace Femto
