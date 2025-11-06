#pragma once
#include <unordered_map>
#include <DxLib.h>

#include "../Path/bone_path.hpp"
#include "../JSON/json_loader.hpp"

namespace model_frame_debug
{
	static void DrawFrames(const int model_handle, const bool is_draw_joint = true, const bool is_draw_bone = true, const bool is_draw_axes = true, const bool is_fill = true)
	{
		JSONLoader json_loader;
		nlohmann::json data;
        if (!json_loader.Load("Data/JSON/mixamo_bone_hierarchy.json", data)) { return; }
		
		const auto  hips    = data.at("Armature").at(BonePath.HIPS);
        auto        hips_m  = MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, BonePath.HIPS));

        // ラムダ式（再帰関数）
        std::function<void(const nlohmann::json&, MATRIX&)> Traverse;

        Traverse = [&](const nlohmann::json& node, MATRIX& parent_matrix)
        {
            for (auto itr = node.begin(); itr != node.end(); ++itr)
            {
                const auto frame_name = itr.key();
                const auto frame_index = MV1SearchFrame(model_handle, frame_name.c_str());
                if (frame_index == -1) { continue; }

                auto       child_m      = MV1GetFrameLocalWorldMatrix(model_handle, frame_index);
                const auto child_pos    = MGetTranslateElem(child_m);
                const auto parent_pos   = MGetTranslateElem(parent_matrix);
                const auto distance     = VSize(child_pos - parent_pos);
                const auto radius       = distance * 0.2f;
                const auto parent_axes  = math::ConvertRotMatrixToAxes(parent_matrix);
                const auto axes_length  = distance * 0.6f;

                if (is_draw_bone ) { DrawCone3D(child_pos, parent_pos, radius, 6, 0xffffff, 0xffffff, is_fill); }
                if (is_draw_joint) { DrawSphere3D(parent_pos, radius, 6, 0xffffff, 0xffffff, is_fill); }
                if (is_draw_axes ) { axis::Draw(parent_axes, parent_pos, axes_length); }

                // 子がいないため再帰しない
                if (itr.value().empty())
                {
                    const auto child_axes = math::ConvertRotMatrixToAxes(child_m);

                    if (is_draw_joint) { DrawSphere3D(child_pos, radius, 6, 0xffffff, 0xffffff, is_fill); }
                    if (is_draw_axes ) { axis::Draw(child_axes, child_pos, axes_length); }
                    continue;
                }

                // 子がいるため再帰
                Traverse(itr.value(), child_m);
            }
        };

        Traverse(hips, hips_m);
	}
}