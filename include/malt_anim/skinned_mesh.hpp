//
// Created by fatih on 6/14/17.
//

#pragma once

#include <malt/engine_defs.hpp>
#include <rtk/geometry/mesh.hpp>
#include <malt_anim/skeleton.hpp>

namespace malt
{
    class skinned_mesh : public rtk::geometry::mesh
    {
        std::shared_ptr<glm::vec4> m_binding;
        std::shared_ptr<glm::vec4> m_weight;

        skeleton m_skeleton;

        friend rtk::gl::mesh create(const skinned_mesh&);
    public:
        const skeleton& get_skeleton() const { return m_skeleton; }
        skeleton& get_skeleton() { return m_skeleton; }

        void set_skeleton(const skeleton& sk)
        {
            m_skeleton = sk;
        }

        void set_skinned_vertices(gsl::span<const skinned_vertex> verts);
    };

    MALT_PUBLIC rtk::gl::mesh create(const skinned_mesh&);
}
