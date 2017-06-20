//
// Created by fatih on 6/12/17.
//

#pragma once

#include <malt_anim/joint.hpp>
#include <vector>
#include <memory>
#include <unordered_map>

namespace malt
{
    struct skeleton
    {
        std::vector<joint> joints;
        std::unordered_map<std::string, joint_id> joint_names;
    };

    constexpr auto joint_per_vertex = 4;
    constexpr uint8_t nulljnt = 0xFF;

    struct skinned_vertex
    {
        /*
         * indices of the joints this vertex is bound
         */
        std::array<joint_id, joint_per_vertex> joints {{nulljnt, nulljnt, nulljnt, nulljnt}};

        /*
         * stores the weights of each joint
         * they must add up to 1
         * thus, last weight can be calculated by exclusion:
         *     1 - w_0 - w_1 - ... - w_(n-1) = w_n
         */
        std::array<float, joint_per_vertex> weights;
    };

    /*
     * this struct stores the whole state of the skeleton for a pose
     */
    struct skeleton_pose
    {
        /*
         * stores each pose in sqt format
         * each of these sqts transforms a child joints space to its parents
         */
        std::unique_ptr<sqt[]> local_poses;

        /*
         * stores each pose as a matrix
         * these matrices transform the child joint space directly to model space
         */
        std::unique_ptr<glm::mat4[]> global_poses;
    };
}
