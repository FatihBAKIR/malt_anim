//
// Created by fatih on 6/13/17.
//

#include <malt_anim/skeleton_loader.hpp>
#include <malt_anim/skinned_mesh_loader.hpp>
#include <assimp/Importer.hpp>
#include <postprocess.h>
#include <scene.h>
#include <rtk/asset/rtk_assimp.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <map>
#include <rtk/mesh_ops.hpp>

namespace malt
{
    std::vector<glm::vec3> joints;
    namespace assimp
    {
        glm::vec3 cvt_vec(const aiVector3D& vec)
        {
            return { vec[0], vec[1], vec[2] };
        }

        glm::quat cvt_quat(const aiQuaternion& q)
        {
            return { q.w, q.x, q.y, q.z };
        }

        glm::mat4x4 cvt_mat(const aiMatrix4x4& mat)
        {
            aiVector3D pos, scale;
            aiQuaternion rot;

            mat.Decompose(scale, rot, pos);

            return glm::translate(cvt_vec(pos)) * glm::toMat4(cvt_quat(rot)) * glm::scale(cvt_vec(scale));
        }

        struct skeleton_result
        {
            skeleton sk;
            std::vector<skinned_vertex> vs;
        };

        aiMatrix4x4 accum_transform(const aiNode* node)
        {
            if (node == nullptr)
            {
                return aiMatrix4x4{};
            }

            return accum_transform(node->mParent) * node->mTransformation;
        }

        skeleton_result proc_bones(const aiScene* scn, const aiMesh* mesh)
        {
            auto root = scn->mRootNode;
            auto mesh_node = scn->mRootNode->FindNode(mesh->mName);
            auto mesh_inv = glm::inverse(cvt_mat(accum_transform(mesh_node)));
            skeleton result;

            std::vector<skinned_vertex> verts;
            verts.resize(mesh->mNumVertices);

            result.joints.reserve(mesh->mNumBones);

            for (int j = 0; j < mesh->mNumBones; ++j)
            {
                auto& bone = mesh->mBones[j];

                if (bone->mName.length > 0)
                {
                    result.joint_names[bone->mName.C_Str()] = j;
                }

                joint jo;

                auto node = scn->mRootNode->FindNode(bone->mName);

                auto node_to_root = cvt_mat(accum_transform(node));
                glm::vec3 test = node_to_root * glm::vec4{0, 0, 0, 1};
                joints.push_back(test);

                jo.inverse_bind_pose = inverse(node_to_root);
                jo.bind_pose = node_to_root;
                if (j == 3)
                {
                    jo.bind_pose = jo.bind_pose * glm::rotate(glm::radians(90.f), glm::vec3{0, 1, 0});
                }

                result.joints.emplace_back(std::move(jo));

                for (int i = 0; i < bone->mNumWeights; ++i)
                {
                    auto& w = bone->mWeights[i];
                    auto& bind = verts[w.mVertexId];
                    int k;
                    for (k = 0; k < bind.joints.size(); ++k)
                    {
                        if (bind.joints[k] == nulljnt)
                        {
                            bind.joints[k] = j;
                            bind.weights[k] = w.mWeight;
                            break;
                        }
                    }
                }
            }

            return {result, verts};
        }
    }

    bool skinned_mesh_loader::check(meta::type<skinned_mesh>, const asset::asset_file&)
    {
        return true;
    }

    namespace
    {
        skinned_mesh read_skinned_mesh(const aiScene* scn)
        {
            using namespace rtk::assets::assimp;
            for (int i = 0; i < scn->mNumMeshes; ++i)
            {
                auto& m = scn->mMeshes[i];
                skinned_mesh res_mesh;

                res_mesh.set_vertices(read_positions(m));
                res_mesh.set_faces(read_faces(m));
                rtk::geometry::transform(res_mesh,inverse(assimp::cvt_mat(assimp::accum_transform(scn->mRootNode->FindNode(m->mName)))));

                if (m->HasTextureCoords(0))
                {
                    res_mesh.set_uvs(read_uvs(m));
                }

                auto sk = assimp::proc_bones(scn, m);
                res_mesh.set_skeleton(sk.sk);
                res_mesh.set_skinned_vertices(sk.vs);

                return res_mesh;
            }
        }
    }

    std::vector<glm::vec3> man_vertices()
    {
        constexpr auto leg_width = 0.3f;
        constexpr auto leg_height = 1.2f;
        constexpr auto man_depth = 0.3f;

        using v3 = glm::vec3;
        std::vector<v3> res(38);

        constexpr auto half_depth = man_depth / 2;
        res[0] = v3{-leg_width, -leg_height, half_depth};
        res[1] = v3{0, -1.2, half_depth};
        res[2] = v3{0, -1.2, half_depth};
        res[3] = v3{leg_width, -leg_height, half_depth};


    }

    skinned_mesh make_man()
    {
        skinned_mesh mesh;
    }

    skinned_mesh skinned_mesh_loader::load(meta::type<skinned_mesh>, asset::asset_file& file)
    {
        std::vector<uint8_t> data;
        data.resize(file.get_size());
        file.get_stream().read((char*)data.data(), data.size());

        Assimp::Importer imp;
        auto scn = imp.ReadFileFromMemory(data.data(), data.size(), aiProcess_JoinIdenticalVertices | aiProcess_Triangulate);

        return read_skinned_mesh(scn);
    }

    bool skeleton_loader::check(meta::type<skeleton>, const asset::asset_file&)
    {
        return true;
    }

    skeleton skeleton_loader::load(meta::type<skeleton>, asset::asset_file&)
    {
        return skeleton();
    }
}