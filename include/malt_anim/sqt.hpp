//
// Created by fatih on 6/12/17.
//

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace malt
{
    struct sqt
    {
        glm::quat rotation;
        glm::vec3 translation;
        float scale;
    };

    static_assert(alignof(sqt) == sizeof(float), "Alignment of sqt is wrong!");
    static_assert(sizeof(sqt) == sizeof(float) * 8, "Size of sqt is wrong!");
}
