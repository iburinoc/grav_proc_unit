#include <memory>

#include "../backend.hpp"
#include "../buffer.hpp"
#include "../options.hpp"

#include "base.hpp"
#include "gpu.hpp"

Backend::Backend(int count, size_t stride) : count(count), stride(stride) {}

Backend::~Backend() {}

void Backend::init() {

}

size_t Backend::pos_stride() const {
	return this->stride;
}

int Backend::get_count() const {
	return this->count;
}

void Backend::set_vbos(GLuint pvbo, GLuint svbo) {
	this->pvbo = pvbo;
	this->svbo = svbo;

	this->init();
}

std::unique_ptr<Backend> get_backend_from_opts() {
	return std::make_unique<GPUBackend>(options.particle_count);
}

