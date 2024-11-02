#include "terminal.h"

#include "unistd.h"
#include "utils.h"

struct termios Femto::Terminal::mOriginalTermios;

namespace Femto {

Terminal::Terminal() { }

void Terminal::enableRawMode()
{
	if (tcgetattr(STDIN_FILENO, &mOriginalTermios) == -1) {
		Utils::onError("Failed to get attributes! tcgettattr");
	}

	atexit(&disableRawMode);

	struct termios raw = mOriginalTermios;

	raw.c_iflag &= ~(BRKINT | ICRNL | ISTRIP | IXON);
	raw.c_oflag &= ~(OPOST);
	raw.c_cflag |= (CS8);
	raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

	// Timeout for the read oeration
	raw.c_cc[VMIN] = 0;
	raw.c_cc[VTIME] = 1;

	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw)) {
		Utils::onError("Failed to set attributes! tcsettattr");
	}
}

void Terminal::disableRawMode() { tcsetattr(STDIN_FILENO, TCSAFLUSH, &mOriginalTermios); }

} // namespace Femto
