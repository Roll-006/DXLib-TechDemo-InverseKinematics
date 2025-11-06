#pragma once
#include <nlohmann/json.hpp>
#include <unordered_map>
#include <DxLib.h>

#include "../JSON/json_loader.hpp"
#include "../Base/singleton_base.hpp"
#include "../Name/font_name.hpp"
#include "../Data/font_data.hpp"

class FontHandler final : public SingletonBase<FontHandler>
{
public:
	[[nodiscard]] int GetFontHandle(const std::string& font_name) const { return m_font_handle.at(font_name); }

private:
	FontHandler();
	~FontHandler() override;

	void CreateFontHandle();

private:
	std::unordered_map<std::string, int> m_font_handle;

	friend SingletonBase<FontHandler>;
};
