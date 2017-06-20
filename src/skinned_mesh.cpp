//
// Created by fatih on 6/14/17.
//

#include <malt_anim/skinned_mesh.hpp>
#include <rtk/gl/mesh.hpp>

namespace malt
{
    void skinned_mesh::set_skinned_vertices(gsl::span<const skinned_vertex> verts)
    {
        if (verts.size() != get_vertices_size())
        {
            throw std::runtime_error("must be equal!");
        }
        m_binding = std::shared_ptr<glm::vec4>(new glm::vec4[get_vertices_size()], [](auto* p){ delete[] p; });
        m_weight = std::shared_ptr<glm::vec4>(new glm::vec4[get_vertices_size()], [](auto* p) { delete[] p;});
        //std::fill(m_binding.get(), m_binding.get() + get_vertices_size(), 128);
        std::transform(verts.begin(), verts.end(), m_binding.get(), [](const skinned_vertex& v) -> glm::vec4
        {
            return {v.joints[0], v.joints[1], v.joints[2], v.joints[3]};
        });
        std::transform(verts.begin(), verts.end(), m_weight.get(), [](const skinned_vertex& v) -> glm::vec4
        {
            return {v.weights[0], v.weights[1], v.weights[2], v.weights[3]};
        });

        auto res = std::all_of(m_weight.get(), m_weight.get() + get_vertices_size(), [](const glm::vec4& w)
        {
            return std::abs(w[0] + w[1] + w[2] + w[3] - 1) <= 0.01f;
        });

        assert(res);
    }

    rtk::gl::mesh create(const skinned_mesh& m)
    {
        auto base = rtk::geometry::create(m);
        base.add_vertex_data<glm::vec4>(3, { m.m_binding.get(), (long)m.get_vertices_size() });
        base.add_vertex_data<glm::vec4>(4, { m.m_weight.get(), (long)m.get_vertices_size() });
        return base;
    }
}
