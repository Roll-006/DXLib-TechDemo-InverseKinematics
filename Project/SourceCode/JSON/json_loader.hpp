#pragma once
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>

class JSONLoader final
{
public:
	/// @brief JSONデータを外部ファイルに保存する
	/// @brief ファイルがない場合は自動的に作成される
	/// @param file_path JSONファイルパス
	/// @param data 保存するJSONデータ
	/// @return true : 保存成功, false : 保存失敗
	bool Save(const std::string_view& file_path, const nlohmann::json& data);

	/// @brief 外部ファイルからJSONデータを読み込む
	/// @param file_path JSONファイルパス
	/// @param data 読み込むJSONデータ
	/// @return true : 読み込み成功, false : 読み込み失敗
	[[nodiscard]] bool Load(const std::string_view& file_path, nlohmann::json& data);
};
