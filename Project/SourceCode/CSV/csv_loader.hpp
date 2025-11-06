#pragma once
#include <string>
#include <fstream>
#include <sstream>

class CSVLoader final
{
public:
    #pragma region 読み込み
    /// @brief 一次元のCSVファイルを読み込む
    /// @tparam Container データを格納する一次元コンテナ
    /// @tparam 使用可能コンテナ [std::vector<type>] [std::list<type>] [std::deque<type>]
    /// @tparam 使用可能な要素 [std::string] [int] [long] [long long] [float] [double] [long double] [bool]
    /// @param file_path ファイル名
    /// @param is_ignore_first_row 最初の行を無視するかを判定
    /// @return データ読み込み後のコンテナ
    template<typename ContainerT>
    [[nodiscard]] ContainerT Load1DCSV(const std::string& file_path, const bool is_ignore_first_row)
    {
        ContainerT container;
        std::ifstream ifile(file_path.c_str(), std::ios::in);
        std::string line_buffer;

        if (ifile.is_open())
        {
            // 最初の行を無視(任意)
            if (is_ignore_first_row) { getline(ifile, line_buffer); }

            while (getline(ifile, line_buffer))
            {
                AddData(container, line_buffer);
            }
            ifile.close();
        }
        return container;
    }

    /// @brief 二次元のCSVファイルを読み込む
    /// @tparam Container データを格納する二次元コンテナ
    /// @tparam 使用可能コンテナ [std::vector<std::vector<type>>] [std::list<std::list<type>>] [std::deque<std::deque<type>>]
    /// @tparam 使用可能な要素 [std::string] [int] [long] [long long] [float] [double] [long double] [bool]
    /// @param file_path ファイル名
    /// @param is_ignore_first_col 最初の列を無視するかを判定
    /// @return データ読み込み後のコンテナ
    template<typename ContainerT>
    [[nodiscard]] ContainerT Load2DCSV (std::string& file_path, bool is_ignore_first_col)
    {
        ContainerT container;
        std::ifstream ifile(file_path.c_str(), std::ios::in);
        std::string line_buffer;
        
        if (ifile.is_open())
        {
            while (getline(ifile, line_buffer))
            {
                // 二次元コンテナの要素(一次元コンテナ部分)を取得し宣言
                typename ContainerT::value_type split;

                Split(split, line_buffer, is_ignore_first_col);
                container.emplace_back(split);
            }
            ifile.close();
        }
        return container;
    }
    #pragma endregion

private:
    template<typename ContainerT>
    void AddData(ContainerT& container, const std::string& line)
    {
        // 空のセルは無視
        if (line.empty()) { return; }

        // コンテナの要素の型に合わせてデータを追加
        if constexpr (std::is_same_v<typename ContainerT::value_type, std::string>) { container.emplace_back(line); }                // std::string型      
        if constexpr (std::is_same_v<typename ContainerT::value_type, int>)         { container.emplace_back(std::stoi (line)); }    // int型     
        if constexpr (std::is_same_v<typename ContainerT::value_type, long>)        { container.emplace_back(std::stol (line)); }    // long型       
        if constexpr (std::is_same_v<typename ContainerT::value_type, long long>)   { container.emplace_back(std::stoll(line)); }    // long long型       
        if constexpr (std::is_same_v<typename ContainerT::value_type, float>)       { container.emplace_back(std::stof (line)); }    // float型        
        if constexpr (std::is_same_v<typename ContainerT::value_type, double>)      { container.emplace_back(std::stod (line)); }    // double型        
        if constexpr (std::is_same_v<typename ContainerT::value_type, long double>) { container.emplace_back(std::stold(line)); }    // long double型

        // bool型
        if constexpr (std::is_same_v<typename ContainerT::value_type, bool>)
        {
            if (line == "true"  || line == "TRUE"  || line == "1") { container.emplace_back(true); }
            if (line == "false" || line == "FALSE" || line == "0") { container.emplace_back(false); }
        }
    }

    /// @brief 二次元CSVの一行を分割
    template<typename ContainerT>
    void Split(ContainerT& container, const std::string& line, const bool is_ignore_first_col)
    {
        std::istringstream stream(line);
        std::string divided;

        // 行の最初の列を無視(任意)
        if (is_ignore_first_col) { getline(stream, divided, ','); }

        while (getline(stream, divided, ','))
        {
            AddData(container, divided);
        }
    }
};
