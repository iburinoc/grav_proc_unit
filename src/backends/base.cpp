#include <memory>

#include "../backend.hpp"
#include "../buffer.hpp"
#include "../options.hpp"

Backend::Backend(int count) : sb(nullptr), count(count) {}

Backend::~Backend() {}

void Backend::set_buffer(StarBuffer *sb) {
	this->sb = sb;
}

std::unique_ptr<Backend> get_backend_from_opts() {
	return std::make_unique<BasicBackend>(options.particle_count);
}

