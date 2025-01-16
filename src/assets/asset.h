#pragma once

class Asset {
protected:
    std::string path;
public:
    virtual bool load() = 0;
    virtual ~Asset() = default;
    explicit Asset(const std::string& path) : path(path) {}
};