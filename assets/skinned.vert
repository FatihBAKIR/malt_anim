#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

layout (location = 3) in vec4 joint_ids;
layout (location = 4) in vec4 Weights;

out vec4 world_position;
out vec4 world_normal;
out vec2 frag_uv;

flat out vec4 joint_id;

uniform mat4 model;
uniform mat4 vp;
uniform mat4[64] palette;
uniform mat4[64] inverse_joint;

mat4 get_skinning_matrix()
{
    mat4 BoneTransform =
                     palette[uint(joint_ids[0])] * inverse_joint[uint(joint_ids[0])] * Weights[0];
    if (int(joint_ids[1]) != 255)
        BoneTransform += palette[uint(joint_ids[1])] * inverse_joint[uint(joint_ids[1])] * Weights[1];
    if (int(joint_ids[2]) != 255)
        BoneTransform += palette[uint(joint_ids[2])] * inverse_joint[uint(joint_ids[2])] * Weights[2];
    if (int(joint_ids[3]) != 255)
        BoneTransform += palette[uint(joint_ids[3])] * inverse_joint[uint(joint_ids[3])] * Weights[3];

    return BoneTransform;
}

void main()
{
    joint_id = joint_ids / 18.f;
    world_position = model * get_skinning_matrix() * vec4(position, 1.0);
    world_normal = vec4(normalize(vec3(transpose(inverse(model)) * vec4(normal, 0.0f))), 0);
	gl_Position = vp * world_position;
	frag_uv = uv;
}

