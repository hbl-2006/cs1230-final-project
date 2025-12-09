#include "realtime.h"
#include "settings.h"
void Realtime::initializeData(ShapeNames name) {
    std::vector<float> vertexData;

    switch (name) {
    case SPHERE:
        sphere.updateParams(settings.shapeParameter1, settings.shapeParameter2);
        vertexData = sphere.generateShape();
        sphereVertexCount = vertexData.size() / 14;
        break;
    case CONE:
        cone.updateParams(settings.shapeParameter1, settings.shapeParameter2);
        vertexData = cone.generateShape();
        coneVertexCount = vertexData.size() / 14;
        break;
    case CUBE:
        cube.updateParams(settings.shapeParameter1, settings.shapeParameter2);
        vertexData = cube.generateShape();
        cubeVertexCount = vertexData.size() / 14;
        break;
    case CYLINDER:
        cylinder.updateParams(settings.shapeParameter1, settings.shapeParameter2);
        vertexData = cylinder.generateShape();
        cylinderVertexCount = vertexData.size() / 14;
        break;
    }

    glBindVertexArray(vaos[name]);
    glBindBuffer(GL_ARRAY_BUFFER, vbos[name]);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float),
                 vertexData.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float),
                          (void*)(3 * sizeof(float)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float),
                          (void*)(6 * sizeof(float)));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float),
                          (void*)(8 * sizeof(float)));

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float),
                          (void*)(11 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Realtime::drawShape(RenderShapeData &shape,
                         GLint useTextureLoc, GLint textureSamplerLoc,
                         GLint blendLoc, GLint repeatULoc, GLint repeatVLoc,
                         GLint useNormalMapLoc, GLint normalSamplerLoc,
                         GLint useBumpMapLoc, GLint bumpSamplerLoc, GLint bumpStrengthLoc,
                         GLint useParallaxMapLoc, GLint parallaxSamplerLoc, GLint parallaxHeightLoc) {
    // Set model matrix
    GLint modelLoc = glGetUniformLocation(m_shader, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &shape.ctm[0][0]);

    // Set material properties
    GLint cAmbientLoc = glGetUniformLocation(m_shader, "cAmbient");
    GLint cDiffuseLoc = glGetUniformLocation(m_shader, "cDiffuse");
    GLint cSpecularLoc = glGetUniformLocation(m_shader, "cSpecular");
    GLint shininessLoc = glGetUniformLocation(m_shader, "shininess");

    glUniform4fv(cAmbientLoc, 1, &shape.primitive.material.cAmbient[0]);
    glUniform4fv(cDiffuseLoc, 1, &shape.primitive.material.cDiffuse[0]);
    glUniform4fv(cSpecularLoc, 1, &shape.primitive.material.cSpecular[0]);
    glUniform1f(shininessLoc, shape.primitive.material.shininess);

    // Handle texture mapping
    bool hasTexture = settings.extraCredit3 && shape.primitive.material.textureMap.isUsed;
    glUniform1i(useTextureLoc, hasTexture ? 1 : 0);

    if (hasTexture) {
        GLuint textureID = loadTexture(shape.primitive.material.textureMap.filename);
        if (textureID != 0) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textureID);
            glUniform1i(textureSamplerLoc, 0);
            glUniform1f(blendLoc, shape.primitive.material.blend);
            glUniform1f(repeatULoc, shape.primitive.material.textureMap.repeatU);
            glUniform1f(repeatVLoc, shape.primitive.material.textureMap.repeatV);
        } else {
            glUniform1i(useTextureLoc, 0);
        }
    }

    // Handle normal mapping
    bool hasNormalMap = settings.extraCredit4 && shape.primitive.material.normalMap.isUsed;
    glUniform1i(useNormalMapLoc, hasNormalMap ? 1 : 0);

    if (hasNormalMap) {
        GLuint normalTexID = loadTexture(shape.primitive.material.normalMap.filename);
        if (normalTexID != 0) {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, normalTexID);
            glUniform1i(normalSamplerLoc, 1);
        } else {
            glUniform1i(useNormalMapLoc, 0);
        }
    }

    // Handle bump mapping
    bool hasBumpMap = settings.extraCredit2 && shape.primitive.material.bumpMap.isUsed;
    glUniform1i(useBumpMapLoc, hasBumpMap ? 1 : 0);

    if (hasBumpMap) {
        GLuint bumpTexID = loadTexture(shape.primitive.material.bumpMap.filename);
        if (bumpTexID != 0) {
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, bumpTexID);
            glUniform1i(bumpSamplerLoc, 2);
            glUniform1f(bumpStrengthLoc, 10.0f);
        } else {
            glUniform1i(useBumpMapLoc, 0);
        }
    }

    // Handle parallax/depth mapping
    bool hasParallaxMap = settings.extraCredit1 && shape.primitive.material.depthMap.isUsed;
    glUniform1i(useParallaxMapLoc, hasParallaxMap ? 1 : 0);

    if (hasParallaxMap) {
        GLuint parallaxTexID = loadTexture(shape.primitive.material.depthMap.filename);
        if (parallaxTexID != 0) {
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, parallaxTexID);
            glUniform1i(parallaxSamplerLoc, 3);
            glUniform1f(parallaxHeightLoc, 0.1f);
        } else {
            glUniform1i(useParallaxMapLoc, 0);
        }
    }

    // Bind the appropriate VAO and draw
    uint vaoIndex;
    int vertexCount;
    switch (shape.primitive.type) {
    case PrimitiveType::PRIMITIVE_SPHERE:
        vaoIndex = SPHERE;
        vertexCount = sphereVertexCount;
        break;
    case PrimitiveType::PRIMITIVE_CONE:
        vaoIndex = CONE;
        vertexCount = coneVertexCount;
        break;
    case PrimitiveType::PRIMITIVE_CUBE:
        vaoIndex = CUBE;
        vertexCount = cubeVertexCount;
        break;
    case PrimitiveType::PRIMITIVE_CYLINDER:
        vaoIndex = CYLINDER;
        vertexCount = cylinderVertexCount;
        break;
    default:
        return;
    }

    glBindVertexArray(vaos[vaoIndex]);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    glBindVertexArray(0);

    // Unbind all textures
    if (hasTexture) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    if (hasNormalMap) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    if (hasBumpMap) {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    if (hasParallaxMap) {
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
