#include "datastructures/splitbuffer.hpp"
#include "util/mrlog.hpp" // REMOVE

namespace DataStructures {

SplitBuffer::SplitBuffer() {}

SplitBuffer::SplitBuffer(const std::string& post)
: post(post) {}

void SplitBuffer::insert(char c) {
	pre.push_back(c);
}

void SplitBuffer::erase() {
	pre.pop_back();
}

void SplitBuffer::moveleft(std::size_t n) {
	std::size_t index = pre.size() - n;
	post.append(pre, index, n);
	pre.erase(index);
}

void SplitBuffer::moveright(std::size_t n) {
	std::size_t index = post.size() - n;
	pre.append(post, index, n);
	post.erase(index);
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
