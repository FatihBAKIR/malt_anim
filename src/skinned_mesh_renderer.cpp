//
// Created by fatih on 6/15/17.
//

#include <malt_anim/skinned_mesh.hpp>
#include <malt_anim/components/skinned_mesh_renderer.hpp>
#include <malt/component_mgr_impl.hpp>

namespace malt
{
    void skinned_mesh_renderer::Handle(render, const render_ctx& ctx)
    {
        auto mat = get_material();
        rtk::gl::program& prog = mat->get_program();
        for (int i = 0; i < m_skeleton->joints.size(); ++i)
        {
            prog.set_variable("inverse_joint[" + std::to_string(i) + "]", m_skeleton->joints[i].inverse_bind_pose);
            prog.set_variable("palette[" + std::to_string(i) + "]", m_skeleton->joints[i].bind_pose);
        }
        mesh_renderer::Handle(render{}, ctx);
    }
}

MALT_IMPLEMENT_COMP(malt::skinned_mesh_renderer);