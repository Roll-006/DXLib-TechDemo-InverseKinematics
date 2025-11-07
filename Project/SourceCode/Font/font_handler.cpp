#include "font_handler.hpp"

FontHandler::FontHandler()
{
	CreateFontHandle();
}

FontHandler::~FontHandler()
{

}

void FontHandler::CreateFontHandle()
{
	nlohmann::json data;
	if (json_loader::Load("Data/JSON/font_data.json", data))
	{
		const auto font_json = data.at("font_data");
		for (const auto [font_name, value] : font_json.items())
		{
			FontData font_data = value.get<FontData>();

			m_font_handle[font_name] = CreateFontToHandle(
				font_data.font_path.c_str(),
				font_data.size,
				font_data.thick,
				font_data.type,
				font_data.char_set,
				font_data.edge_size,
				font_data.italic,
				font_data.handle);
		}
	}
}
