#pragma once

#include <memory>

#include "backends/base.hpp"

std::unique_ptr<Backend> get_backend_from_opts();

