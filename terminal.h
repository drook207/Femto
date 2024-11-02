#ifndef TERMINAL_H
#define TERMINAL_H

#include <termios.h>

namespace Femto {

class Terminal {
public:
	Terminal();

	static void enableRawMode();

	static void disableRawMode();

private:
	static struct termios mOriginalTermios;
};

} // namespace Femto

#endif // TERMINAL_H
