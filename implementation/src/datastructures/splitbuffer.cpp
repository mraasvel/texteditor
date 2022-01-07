#include "datastructures/splitbuffer.hpp"
#include "util/mrlog.hpp" // REMOVE
#include <stdexcept>

namespace DataStructures {

SplitBuffer::SplitBuffer() {}

SplitBuffer::SplitBuffer(std::string&& post) {
	std::swap(this->post, post);
}

void SplitBuffer::push(char c) {
	pre.push_back(c);
}

void SplitBuffer::push(const std::string& s) {
	pre.append(s);
}

void SplitBuffer::insert(char c) {
	post.push_back(c);
}

void SplitBuffer::erase() {
	pre.pop_back();
}

void SplitBuffer::del() {
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

std::string&& SplitBuffer::movePost() {
	return std::move(post);
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

char SplitBuffer::operator[](std::size_t n) const {
	if (n >= size()) {
		throw std::out_of_range("SplitBuffer");
	}
	return n < pre.size() ? pre[n] : post[post.size() - (n - pre.size()) - 1];
}

/*
Debug
*/
void SplitBuffer::log() const {
	mrlog::info("PRE({}) - POST({})", pre, post);
}

}
