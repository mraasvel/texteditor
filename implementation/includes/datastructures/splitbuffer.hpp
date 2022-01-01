#pragma once

#include <string>

namespace DataStructures {

class SplitBuffer {
public:
	SplitBuffer();
	SplitBuffer(const std::string& post);
	void insert(char c);
	void erase();
	void moveleft(std::size_t n = 1);
	void moveright(std::size_t n = 1);
	std::size_t size() const;
	const std::string& getPre() const;
	const std::string& getPost() const;
	bool preEmpty() const;
	bool postEmpty() const;

/* DEBUG, REMOVE */
	void log() const;
private:
	std::string pre;
	std::string post;
};

}
