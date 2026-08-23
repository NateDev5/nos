#pragma once
#include <boot/limine/limine.h>

namespace {
__attribute__((used, section(".limine_requests"))) volatile uint64_t limine_base_revision[] = LIMINE_BASE_REVISION(4);
}

namespace {

__attribute__((used, section(".limine_requests_start"))) volatile uint64_t limine_requests_start_marker[] = LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end"))) volatile uint64_t limine_requests_end_marker[] = LIMINE_REQUESTS_END_MARKER;
} // namespace
