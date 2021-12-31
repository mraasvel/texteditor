#pragma once

#define CTRL_KEY(c) ((c) & 0x1F)

namespace TextEditor {

namespace Keys {
	enum {
		NEWLINE = 13,
		ESCAPE = 27,
	};
}

}
