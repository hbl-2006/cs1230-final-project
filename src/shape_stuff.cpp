#include "realtime.h"
#include "settings.h"
void Realtime::initializeData(Shape &s, ShapeNames name)
{
    glBindBuffer(GL_ARRAY_BUFFER, vbos[name]);
    s.updateParams(settings.shapeParameter1, settings.shapeParameter2);
    std::vector<float> shapeData = s.generateShape();
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(GLfloat) * shapeData.size(),
                 shapeData.data(),
                 GL_STATIC_DRAW);

    // set up each vao by binding it to the proper vbo
    glBindVertexArray(vaos[name]);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void *) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(GLfloat) * 6,
                          (void *) (3 * sizeof(GLfloat)));

    // ================== Returning to Default State
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Realtime::drawShape(RenderShapeData &shape)
{
    Shape *s;
    ShapeNames name;
    auto mat = shape.primitive.material;
    switch (shape.primitive.type) {
    case PrimitiveType::PRIMITIVE_CUBE:
        s = &cube;
        name = CUBE;
        break;
    case PrimitiveType::PRIMITIVE_CONE:
        s = &cone;
        name = CONE;
        break;
    case PrimitiveType::PRIMITIVE_CYLINDER:
        s = &cylinder;
        name = CYLINDER;
        break;
    case PrimitiveType::PRIMITIVE_SPHERE:
        s = &sphere;
        name = SPHERE;
        break;
    case PrimitiveType::PRIMITIVE_MESH:
        break;
    }
    // Bind VAO
    glBindVertexArray(vaos[name]);
    // Pass in vertex matrix uniforms
    glUniformMatrix4fv(glGetUniformLocation(m_shader, "ctm"), 1, GL_FALSE, &shape.ctm[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(m_shader, "view"),
                       1,
                       GL_FALSE,
                       &camera.getViewMatrix()[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(m_shader, "proj"),
                       1,
                       GL_FALSE,
                       &camera.getProjectionMatrix()[0][0]);
    glm::mat3 normalMat = glm::inverse(glm::transpose(glm::mat3(shape.ctm)));
    glUniformMatrix3fv(glGetUniformLocation(m_shader, "normal_mat"), 1, GL_FALSE, &normalMat[0][0]);
    glUniform1f(glGetUniformLocation(m_shader, "shininess"), mat.shininess);
    glUniform3f(glGetUniformLocation(m_shader, "objAmbient"),
                mat.cAmbient.x,
                mat.cAmbient.y,
                mat.cAmbient.z);
    glUniform3f(glGetUniformLocation(m_shader, "objDiffuse"),
                mat.cDiffuse.x,
                mat.cDiffuse.y,
                mat.cDiffuse.z);
    glUniform3f(glGetUniformLocation(m_shader, "objSpecular"),
                mat.cSpecular.x,
                mat.cSpecular.y,
                mat.cSpecular.z);
    // Draw shape
    glDrawArrays(GL_TRIANGLES, 0, s->getVertexCount());
    // Unbind VAO
    glBindVertexArray(0);
}
