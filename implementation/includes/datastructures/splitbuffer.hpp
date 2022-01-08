#pragma once

#include <string>

namespace DataStructures {

class SplitBuffer {
public:
	SplitBuffer();
	SplitBuffer(std::string&& post);
	void push(char c);
	void push(const std::string& s);
	void insert(char c);
	void insert(const std::string& s);
	void erase();
	void del();
	std::string&& movePost();
	std::size_t moveleft(std::size_t n = 1);
	std::size_t moveright(std::size_t n = 1);
	void moveTo(std::size_t index);
	std::size_t moveStart();
	std::size_t moveEnd();
	std::size_t size() const;
	const std::string& getPre() const;
	const std::string& getPost() const;
	bool preEmpty() const;
	bool postEmpty() const;

	char operator[](std::size_t n) const;

/* DEBUG, REMOVE */
	void log() const;
private:
	std::size_t moveSplit(std::string& dest, std::string& src, std::size_t n);
private:
	std::string pre;
	std::string post;
};

}
