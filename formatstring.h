#pragma once

#include <string>

namespace Femto {
class FormatString : public std::string {
public:
	FormatString(std::string s);

	template <typename T>

	void arg(const T& arg)
	{
		static_assert(std::is_arithmetic<T>() || std::is_class<std::string>(),
			"Argument must be an integral or arithmetic or floating "
			"point or string type");

		auto pos = this->find_first_of('%');
		if (pos != npos) {
			std::string replace = "";
			if (this->at(pos + 1) != '%') {
				if (std::is_arithmetic<T>() && !std::is_class<std::string>()) {
					this->replace(pos, 2, std::to_string(arg));
				} else {
					this->replace(pos, 2, arg);
				}
			}
		}
	}
};

} // namespace Femto
