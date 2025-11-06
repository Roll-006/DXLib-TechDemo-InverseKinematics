#include "json_loader.hpp"

bool JSONLoader::Save(const std::string_view& file_path, const nlohmann::json& data)
{
    // 書き込み用ファイルを上書き指定で展開
    // ファイルがない場合自動作成
    std::ofstream ofs(std::string(file_path), std::ios::out);

    // 展開に失敗
    if (!ofs) { return false; }

    try
    {
        // 書き込み
        ofs << data.dump(4);
    }
    catch (...)
    {
        return false;
    }

    return true;
}

bool JSONLoader::Load(const std::string_view& file_path, nlohmann::json& data)
{
    // 読み込み用ファイルを展開
    std::string path = std::string(file_path);
    std::ifstream ifs(path);

    // 展開に失敗
    if (!ifs) return false;

    try
    {
        // 読み込み
        ifs >> data;
    }
    catch (...)
    {
        return false;
    }

    return true;
}
