//
// Created by fatih on 6/12/17.
//

#pragma once

#include <malt_anim/sqt.hpp>
#include <glm/mat4x3.hpp>

namespace malt
{
    using joint_id = uint8_t;
    struct joint
    {
        /*
         * this matrix transforms a vertex from the bind pose directly to the joint space
         */
        glm::mat4x4 inverse_bind_pose;
        glm::mat4x4 bind_pose;

        joint_id parent;
    };
}
