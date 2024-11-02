#include <unistd.h>

#include "editor.h"
#include "formatstring.h"

int main()
{
	Femto::FormatString s("Hello World %0, %1");
	std::cout << s << "\r\n";
	s.arg("!");
	s.arg(1);
	std::cout << s << "\r\n";
	// Femto::Editor editor{};
	// editor.run();

	return 0;
}
