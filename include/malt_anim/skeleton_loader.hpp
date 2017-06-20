//
// Created by fatih on 6/13/17.
//

#pragma once

#include <malt/list.hpp>
#include <malt_anim/skeleton.hpp>
#include <malt_asset/asset_loader.hpp>

namespace malt
{
    class skeleton_loader
    {
    public:
        using types = meta::list<skeleton>;

        bool check(meta::type<skeleton>, const asset::asset_file&);
        skeleton load(meta::type<skeleton>, asset::asset_file&);
    };
}
