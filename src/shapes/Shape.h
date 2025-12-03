#pragma once
#include "glm/ext/vector_float3.hpp"
#include <vector>
class Shape
{
public:
    void updateParams(int param1, int param2)
    {
        m_vertexData = std::vector<float>();
        m_param1 = param1;
        m_param2 = param2;
        setVertexData();
    };
    std::vector<float> generateShape() { return m_vertexData; }
    int getVertexCount() { return m_vertexData.size() / 6; };

protected:
    void insertVec3(std::vector<float> &data, glm::vec3 v)
    {
        data.push_back(v.x);
        data.push_back(v.y);
        data.push_back(v.z);
    };
    std::vector<float> m_vertexData;
    int m_param1;
    int m_param2;

private:
    virtual void setVertexData() = 0;
};
