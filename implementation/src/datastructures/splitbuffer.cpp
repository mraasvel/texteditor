#include "datastructures/splitbuffer.hpp"
#include "util/mrlog.hpp" // REMOVE

namespace DataStructures {

SplitBuffer::SplitBuffer() {}

SplitBuffer::SplitBuffer(const std::string& post)
: post(post) {}

void SplitBuffer::insert(char c) {
	pre.push_back(c);
}

void SplitBuffer::insert(const std::string& s) {
	pre.append(s);
}

void SplitBuffer::erase() {
	pre.pop_back();
}

void SplitBuffer::erasePost() {
	post.pop_back();
}

/*
Returns amount of bytes the line was able to move left
*/
std::size_t SplitBuffer::moveleft(std::size_t n) {
	return moveSplit(post, pre, n);
}

std::size_t SplitBuffer::moveright(std::size_t n) {
	return moveSplit(pre, post, n);
}

std::size_t SplitBuffer::moveSplit(std::string& dest, std::string& src, std::size_t n) {
	n = std::min(src.size(), n);
	// append in reverse order
	dest.append(src.crbegin(), src.crbegin() + n);
	src.erase(src.size() - n);
	return n;
}

std::size_t SplitBuffer::size() const {
	return pre.size() + post.size();
}

const std::string& SplitBuffer::getPre() const {
	return pre;
}

const std::string& SplitBuffer::getPost() const {
	return post;
}

bool SplitBuffer::preEmpty() const {
	return pre.empty();
}

bool SplitBuffer::postEmpty() const {
	return post.empty();
}

/*
Debug
*/
void SplitBuffer::log() const {
	mrlog::info("PRE(%s) - POST(%s)", pre.c_str(), post.c_str());
}

}
