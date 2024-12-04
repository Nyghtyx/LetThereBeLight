#pragma once

#include <string>

class Action
{
    std::string m_name  = "NONE"; // Ex: "SHOOT", "LEFT", "JUMP"
    std::string m_type  = "NONE"; // "START" or "END"

public:

    Action() = default;

    Action(const std::string& name, const std::string& type)
        : m_name(name)
        , m_type(type) {}
    
    const std::string& name() const
    {
        return m_name;
    }

    const std::string& type() const
    {
        return m_type;
    }
};