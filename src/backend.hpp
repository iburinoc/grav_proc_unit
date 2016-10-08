#pragma once

#include <memory>

#include "backends/base.hpp"
#include "backends/basic.hpp"
#include "backends/gpu.hpp"

std::unique_ptr<Backend> get_backend_from_opts();

