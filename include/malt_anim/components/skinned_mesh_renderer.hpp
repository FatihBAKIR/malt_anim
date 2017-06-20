//
// Created by fatih on 6/15/17.
//

#pragma once

#include <malt_render/components/mesh_renderer.hpp>

namespace malt
{
    class skinned_mesh_renderer : public mesh_renderer
    {
        const skeleton* m_skeleton;
    public:
        void Handle(render, const render_ctx&);
        void set_skeleton(const skeleton* sk) { m_skeleton = sk; }
    };
}


