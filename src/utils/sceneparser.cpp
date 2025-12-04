#include "sceneparser.h"
#include "glm/gtx/string_cast.hpp"
#include "scenefilereader.h"
#include <glm/gtx/transform.hpp>
#include <iostream>

void traverseTree(SceneNode* node,
                  glm::mat4 ctm,
                  auto& shapes,
                  auto& lights,
                  glm::mat4 scale_matrix,
                  glm::mat4 rot_matrix);
bool SceneParser::parse(std::string filepath, RenderData& renderData)
{
    ScenefileReader fileReader = ScenefileReader(filepath);
    bool success = fileReader.readJSON();
    if (!success) {
        return false;
    }

    // Task 5: populate renderData with global data, and camera data;
    renderData.cameraData = fileReader.getCameraData();
    renderData.cameraData.pos.w = 1;
    renderData.globalData = fileReader.getGlobalData();

    // Task 6: populate renderData's list of primitives and their transforms.
    //         This will involve traversing the scene graph, and we recommend you
    //         create a helper function to do so!
    auto root = fileReader.getRootNode();
    renderData.shapes.clear();
    renderData.lights.clear();
    traverseTree(root,
                 glm::mat4(1.0),
                 renderData.shapes,
                 renderData.lights,
                 glm::mat4(1.0),
                 glm::mat4(1.0));
    return true;
}

void traverseTree(SceneNode* node,
                  glm::mat4 ctm,
                  auto& shapes,
                  auto& lights,
                  glm::mat4 scale_matrix,
                  glm::mat4 rot_matrix)
{
    if (!node)
        return;
    // First, update the ctm with our transformations
    for (auto* transformation : node->transformations) {
        switch (transformation->type) {
        case TransformationType::TRANSFORMATION_TRANSLATE:
            ctm = glm::translate(ctm, transformation->translate);
            break;
        case TransformationType::TRANSFORMATION_ROTATE:
            ctm = glm::rotate(ctm, transformation->angle, transformation->rotate);
            rot_matrix = glm::rotate(rot_matrix, transformation->angle, transformation->rotate);
            break;
        case TransformationType::TRANSFORMATION_SCALE:
            ctm = glm::scale(ctm, transformation->scale);
            scale_matrix = glm::scale(scale_matrix, transformation->scale);
            break;
        case TransformationType::TRANSFORMATION_MATRIX:
            ctm *= transformation->matrix;
            break;
        }
    }
    // If we have any primitives here, attach them to the shapes data with this ctm.
    for (auto* primitive : node->primitives) {
        auto shape = RenderShapeData(*primitive, ctm, glm::inverse(ctm));
        shape.body.position = glm::vec3(ctm[3]);
        shape.body.orientation = glm::normalize(glm::quat_cast(rot_matrix));
        shape.scale = scale_matrix;

        switch (primitive->type) {
        case PrimitiveType::PRIMITIVE_CUBE:
            shape.body.I_obj = (1.0f / 12.0f) * shape.body.mass
                               * glm::mat3(glm::vec3(1 + 1, 0, 0),
                                           glm::vec3(0, 1 + 1, 0),
                                           glm::vec3(0, 0, 1 + 1));
            break;
        case PrimitiveType::PRIMITIVE_CONE: {
            // formula taken from wolfram alpha
            float intermediate_xy = (3.0f / 80.0f) * shape.body.mass * (4 * (0.5f * 0.5f) + 1);
            shape.body.I_obj = glm::mat3(glm::vec3(intermediate_xy, 0, 0),
                                         glm::vec3(0, intermediate_xy, 0),
                                         glm::vec3(0, 0, 0.3f * shape.body.mass * 0.5f * 0.5f));
            break;
        }
        case PrimitiveType::PRIMITIVE_CYLINDER: {
            float intermediate_xy = (1.0f / 12.0f) * shape.body.mass * (3 * 0.5f * 0.5f + 1);
            shape.body.I_obj = glm::mat3(glm::vec3(intermediate_xy, 0, 0),
                                         glm::vec3(0, intermediate_xy, 0),
                                         glm::vec3(0, 0, 0.5f * shape.body.mass * 0.5f * 0.5f));
            break;
        }
        case PrimitiveType::PRIMITIVE_SPHERE: {
            float I = 0.4f * shape.body.mass * 0.5f * 0.5f;
            shape.body.I_obj = glm::mat3(I);
            break;
        }
        case PrimitiveType::PRIMITIVE_MESH:
            break;
        }

        shape.body.I_obj_inv = glm::inverse(shape.body.I_obj);

        // if our shape is static, give it 0 mass_inverse
        if (!shape.primitive.dynamic) {
            shape.body.mass_inv = 0.0f;
        }

        shapes.push_back(shape);
    }

    // If we have any lights here, attach them to the lights data.
    for (auto* light : node->lights) {
        auto pos = ctm * glm::vec4(0, 0, 0, 1);
        auto dir = ctm * light->dir;
        auto lightData = SceneLightData(light->id,
                                        light->type,
                                        light->color,
                                        light->function,
                                        pos,
                                        dir,
                                        light->penumbra,
                                        light->angle,
                                        light->width,
                                        light->height);
        lights.push_back(lightData);
    }

    // Pass the new CTM down to all the children
    for (auto* child : node->children) {
        traverseTree(child, ctm, shapes, lights, scale_matrix, rot_matrix);
    }
}
