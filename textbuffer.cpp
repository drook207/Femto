#include "textbuffer.h"

#include <cstring>

namespace Femto {

TextBuffer::TextBuffer() { }

TextBuffer::TextBuffer(std::size_t startSize)
	: mSize(startSize)
	, mFreeSize(startSize)
{
	mData = new char[mSize];
}

TextBuffer::~TextBuffer()
{
	delete mData;
	mData = nullptr;
}

bool TextBuffer::append(const std::string& chars)
{
	if (nullptr == mData) {
		mData = new char[chars.size()];
		mSize = chars.size();
		mFreeSize = 0;
		mDataOffset = mSize;
		std::memcpy(mData, chars.data(), chars.size());
		return true;
	}

	if (chars.size() > mFreeSize) {
		char* c = new char[(mSize - mFreeSize) + chars.size()];

		std::memcpy(c, mData, mSize);
		std::memcpy(&c[mSize], chars.data(), chars.size());

		mSize += chars.size();
		mFreeSize = 0;
		mDataOffset = mSize;
		delete mData;

		mData = c;
		return true;
	}

	std::memcpy(&mData[mDataOffset], chars.data(), chars.size());

	mFreeSize -= chars.size();
	mDataOffset += chars.size();

	return true;
}

bool TextBuffer::append(char* c, std::size_t len)
{
	if (nullptr == mData) {
		mData = new char[len];
		mSize = len;
		mFreeSize = 0;
		mDataOffset = len - 1;
		std::memcpy(mData, c, len);
		return true;
	}

	if (len > mFreeSize) {
		char* c = new char[(mSize - mFreeSize) + len];

		std::memcpy(c, mData, mSize);
		std::memcpy(&c[mSize + 1], c, len);

		delete mData;

		mData = c;
		return true;
	}

	std::memcpy(&mData[mDataOffset], c, len);

	mDataOffset += len;

	return true;
}

bool TextBuffer::operator+=(const std::string& chars) { return append(chars); }

void TextBuffer::operator<<(const std::string& chars) { append(chars); }

char TextBuffer::operator[](uint32_t index)
{
	if (nullptr == mData) {
		throw;
	}

	if (index > mSize - 1) {
		throw;
	}

	return mData[index];
}

char* TextBuffer::data() { return mData; }

size_t TextBuffer::size() const { return mSize; }

void TextBuffer::release()
{
	delete mData;
	mData = nullptr;
	mSize = 0;
	mFreeSize = 0;
	mDataOffset = 0;
}

} // namespace Femto
