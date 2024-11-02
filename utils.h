#pragma once

#include <unistd.h>

#include <iostream>
#include <string_view>

namespace Femto::Utils {

#define FEMTO_VERSION "0.0.1"

#define OUT_PARAMETER

#define DELETE_COPY_MOVE(X)                                                                                            \
                                                                                                                       \
	X(X& other) = delete;                                                                                              \
	X(X&& other) = delete;                                                                                             \
	X operator=(X& other) = delete;                                                                                    \
	X operator=(X&& other) = delete;

static inline void clearScreen()
{
	write(STDOUT_FILENO, "\x1b[2J", 4);
	write(STDOUT_FILENO, "\x1b[H", 3);
}

static inline void onError(std::string_view message)
{
	clearScreen();
	std::cerr << message;
	std::exit(1);
}

} // namespace Femto::Utils
