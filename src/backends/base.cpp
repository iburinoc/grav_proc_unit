#include <memory>

#include "../backend.hpp"
#include "../buffer.hpp"
#include "../particle_gen.hpp"
#include "../options.hpp"

#include "base.hpp"
#include "gpu.hpp"

Backend::Backend(int count, std::unique_ptr<ParticleGen> p_gen, size_t stride) :
	count(count), p_gen(std::move(p_gen)), stride(stride) {}

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
	auto p_gen = std::make_unique<ParticleGen>(options.seed);
	return std::make_unique<GPUBackend>(options.particle_count,
		std::move(p_gen));
}

