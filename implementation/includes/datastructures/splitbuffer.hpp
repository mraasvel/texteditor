#pragma once

#include <string>

namespace DataStructures {

class SplitBuffer {
public:
	SplitBuffer();
	SplitBuffer(const std::string& post);
	void insert(char c);
	void erase();
	std::size_t moveleft(std::size_t n = 1);
	std::size_t moveright(std::size_t n = 1);
	std::size_t size() const;
	const std::string& getPre() const;
	const std::string& getPost() const;
	bool preEmpty() const;
	bool postEmpty() const;

/* DEBUG, REMOVE */
	void log() const;
private:
	std::size_t moveSplit(std::string& dest, std::string& src, std::size_t n);
private:
	std::string pre;
	std::string post;
};

}
