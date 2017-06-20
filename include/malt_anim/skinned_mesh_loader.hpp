//
// Created by fatih on 6/14/17.
//

#pragma once

#include <malt/list.hpp>
#include <malt_anim/skinned_mesh.hpp>
#include <malt_asset/asset_loader.hpp>

namespace malt
{
    class skinned_mesh_loader
    {
    public:
        using types = meta::list<skinned_mesh>;

        bool check(meta::type<skinned_mesh>, const asset::asset_file&);
        skinned_mesh load(meta::type<skinned_mesh>, asset::asset_file&);
    };


}

