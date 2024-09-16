#pragma once

#include "vertex_buffer_element.h"

#include <vector>

struct Layout {
    std::vector<struct Element> elements;
    int stride;
};

void layout_init(struct Layout *layout);

void layout_push_f32(struct Layout *self, unsigned int count);
void layout_push_u32(struct Layout *self, unsigned int count);
void layout_push_u8(struct Layout *self, unsigned int count);
