#pragma once
#include <string>

struct FontData
{
	std::string font_path	= "";
	int			size		= 0;
	int			thick		= 0;
	int			type		= -1;
	int			char_set	= -1;
	int			edge_size	= -1;
	bool		italic		= false;
	int			handle		= -1;
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& data, FontData& font_data)
{
	data.at("font_path").get_to(font_data.font_path);
	data.at("size")		.get_to(font_data.size);
	data.at("thick")	.get_to(font_data.thick);
	data.at("type")		.get_to(font_data.type);
	data.at("char_set")	.get_to(font_data.char_set);
	data.at("edge_size").get_to(font_data.edge_size);
	data.at("italic")	.get_to(font_data.italic);
	data.at("handle")	.get_to(font_data.handle);
}

inline void to_json(nlohmann::json& data, const FontData& font_data)
{
	data = nlohmann::json
	{
		{ "font_path",	font_data.font_path },
		{ "size",		font_data.size },
		{ "thick",		font_data.thick },
		{ "type",		font_data.type },
		{ "char_set",	font_data.char_set },
		{ "edge_size",	font_data.edge_size },
		{ "italic",		font_data.italic },
		{ "handle",		font_data.handle }
	};
}
#pragma endregion
