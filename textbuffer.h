#pragma once

#include <cstddef>
#include <string>

#include "utils.h"

namespace Femto {

class TextBuffer {
public:
	TextBuffer();
	explicit TextBuffer(std::size_t startSize);
	~TextBuffer();
	DELETE_COPY_MOVE(TextBuffer)

	bool append(const std::string& chars);
	bool append(char* c, std::size_t len);

	bool operator+=(const std::string& chars);

	void operator<<(const std::string& chars);

	char operator[](std::uint32_t index);

	char* data();

	size_t size() const;

	void release();

private:
	char* mData = nullptr;
	std::size_t mDataOffset = 0;
	std::size_t mSize = 0;
	std::size_t mFreeSize = 0;
};

} // namespace Femto
