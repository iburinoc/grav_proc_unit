#pragma once

#include "../buffer.hpp"

class Backend {
protected:
	// Use raw pointer so we can bind it later on
	StarBuffer *sb;
        int count;

	Backend(int count);

public:
	virtual ~Backend();

	virtual void set_buffer(StarBuffer *sb);
	virtual void update(float dt) = 0;
};

