#include "datastructures/splitbuffer.hpp"
#include "util/mrlog.hpp" // REMOVE
#include <algorithm>
#include <stdexcept>

namespace DataStructures {

SplitBuffer::SplitBuffer() {}

SplitBuffer::SplitBuffer(std::string&& post) {
	std::swap(this->post, post);
}

int SplitBuffer::currentChar() const {
	if (pre.empty()) {
		return 0;
	}
	return pre.back();
}

int SplitBuffer::nextChar() const {
	if (post.empty()) {
		return 0;
	}
	return post.back();
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

// TODO: should be in reverse order
void SplitBuffer::insert(const std::string& s) {
	post.append(s);
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

void SplitBuffer::moveTo(std::size_t index) {
	if (index >= pre.size()) {
		moveright(index - pre.size());
	} else {
		moveleft(pre.size() - index);
	}
}

std::size_t SplitBuffer::moveStart() {
	auto size = pre.size();
	moveleft(pre.size());
	return size;
}

std::size_t SplitBuffer::moveEnd() {
	auto size = post.size();
	moveright(post.size());
	return size;
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

std::string& SplitBuffer::mutablePre() {
	return pre;
}

std::string& SplitBuffer::mutablePost() {
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

void SplitBuffer::writeToStream(std::ostream& os) const {
	std::string temp {post};
	std::reverse(temp.begin(), temp.end());
	os << pre << temp;
}

/*
Debug
*/
void SplitBuffer::log() const {
	std::string temp(post);
	std::reverse(temp.begin(), temp.end());
	mrlog::log("PRE({}) - POST({})", pre, temp);
}

}
