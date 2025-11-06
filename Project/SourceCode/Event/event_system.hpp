#pragma once
#include <functional>
#include <typeindex>

#include "../Base/singleton_base.hpp"
#include "../IncludeList/event.hpp"

class EventSystem final : public SingletonBase<EventSystem>
{
public:
    /// @brief イベントを登録
    template<typename EventT, typename ClassT>
    void Subscribe(ClassT* instance, void(ClassT::*method)(const EventT&))
    {
        const auto type_id = std::type_index(typeid(EventT));

        // インスタンスとメソッドポインタを組み合わせたキーを作成
        auto key = std::make_pair(static_cast<void*>(instance), reinterpret_cast<void*&>(method));

        m_method_listeners[type_id].emplace_back(key, [instance, method](const void* event_data)
        {
            const auto* event = static_cast<const EventT*>(event_data);
            (instance->*method)(*event);
        });
    }

    /// @brief イベントの登録を解除
    template<typename EventT, typename ClassT>
    void Unsubscribe(ClassT* instance, void(ClassT::*method)(const EventT&))
    {
        const auto type_id = std::type_index(typeid(EventT));
        auto itr = m_method_listeners.find(type_id);
        if (itr != m_method_listeners.end())
        {
            auto key = std::make_pair(static_cast<void*>(instance), reinterpret_cast<void*&>(method));

            auto& callbacks = itr->second;
            callbacks.erase(
                std::remove_if(callbacks.begin(), callbacks.end(),
                    [&key](const auto& pair) { return pair.first == key; }),
                callbacks.end()
            );
        }
    }

    /// @brief イベントを通知
    template<typename EventT>
    void Publish(const EventT& event)
    {
        const auto type_id = std::type_index(typeid(EventT));

        // 通常のリスナー
        auto itr = m_listeners.find(type_id);
        if (itr != m_listeners.end())
        {
            for (auto& callback : itr->second)
            {
                callback(&event);
            }
        }

        // メソッドリスナー
        auto method_itr = m_method_listeners.find(type_id);
        if (method_itr != m_method_listeners.end())
        {
            for (auto& [key, callback] : method_itr->second)
            {
                callback(&event);
            }
        }
    }

private:
    std::unordered_map<std::type_index, std::vector<std::function<void(const void*)>>> m_listeners;
    std::unordered_map<std::type_index, std::vector<std::pair<std::pair<void*, void*>, std::function<void(const void*)>>>> m_method_listeners;

    friend SingletonBase<EventSystem>;
};
