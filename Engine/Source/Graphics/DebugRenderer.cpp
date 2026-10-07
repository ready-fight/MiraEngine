#include "Graphics/DebugRenderer.h"

#include "Graphics/Camera.h"

#include "Scene/Scene.h"
#include "Scene/GameObject.h"

#include "Collision/Collider.h"
#include "Collision/SphereCollider.h"
#include "Collision/BoxCollider.h"

#include <GL/glew.h>

#include <cmath>

namespace
{
    const char* DebugVertexShader = R"(
        #version 460 core

        layout(location = 0) in vec3 position;

        uniform mat4 view;
        uniform mat4 projection;

        void main()
        {
            gl_Position =
                projection *
                view *
                vec4(position, 1.0);
        }
    )";

    const char* DebugFragmentShader = R"(
        #version 460 core

        uniform vec3 color;

        out vec4 fragmentColor;

        void main()
        {
            fragmentColor =
                vec4(color, 1.0);
        }
    )";
}

namespace MiraEngine
{
    DebugRenderer::DebugRenderer()
        : m_shader(
            DebugVertexShader,
            DebugFragmentShader
        )
    {
        glGenVertexArrays(
            1,
            &m_vao
        );

        glGenBuffers(
            1,
            &m_vbo
        );

        glBindVertexArray(
            m_vao
        );

        glBindBuffer(
            GL_ARRAY_BUFFER,
            m_vbo
        );

        glEnableVertexAttribArray(
            0
        );

        glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            sizeof(glm::vec3),
            nullptr
        );

        glBindBuffer(
            GL_ARRAY_BUFFER,
            0
        );

        glBindVertexArray(
            0
        );
    }

    DebugRenderer::~DebugRenderer()
    {
        glDeleteBuffers(
            1,
            &m_vbo
        );

        glDeleteVertexArrays(
            1,
            &m_vao
        );
    }

    void DebugRenderer::Render(
        float aspectRatio,
        const Camera& camera,
        const Scene& scene
    )
    {
        std::vector<glm::vec3>
            vertices;

        for (
            const auto& object :
            scene.GetObjects()
            )
        {
            const Collider* collider =
                object->GetCollider();

            if (!collider)
            {
                continue;
            }

            const glm::vec3 position =
                object
                ->GetTransform()
                .GetPosition();

            if (
                const auto* sphere =
                dynamic_cast<
                const SphereCollider*
                >(collider)
                )
            {
                AddSphere(
                    vertices,
                    position,
                    sphere->GetRadius()
                );
            }

            else if (
                const auto* box =
                dynamic_cast<
                const BoxCollider*
                >(collider)
                )
            {
                AddBox(
                    vertices,
                    position,
                    box->GetHalfExtents()
                );
            }
        }

        if (vertices.empty())
        {
            return;
        }

        m_shader.Bind();

        m_shader.SetMatrix4(
            "view",
            camera.GetViewMatrix()
        );

        m_shader.SetMatrix4(
            "projection",
            camera.GetProjectionMatrix(
                aspectRatio
            )
        );

        const int colorLocation =
            glGetUniformLocation(
                m_shader.GetProgram(),
                "color"
            );

        glUniform3f(
            colorLocation,
            0.0f,
            1.0f,
            0.0f
        );

        glBindVertexArray(
            m_vao
        );

        glBindBuffer(
            GL_ARRAY_BUFFER,
            m_vbo
        );

        glBufferData(
            GL_ARRAY_BUFFER,
            vertices.size() *
            sizeof(glm::vec3),
            vertices.data(),
            GL_DYNAMIC_DRAW
        );

        glDrawArrays(
            GL_LINES,
            0,
            static_cast<GLsizei>(
                vertices.size()
                )
        );

        glBindVertexArray(
            0
        );

        m_shader.Unbind();
    }

    void DebugRenderer::AddBox(
        std::vector<glm::vec3>& vertices,
        const glm::vec3& position,
        const glm::vec3& halfExtents
    )
    {
        const glm::vec3 min =
            position - halfExtents;

        const glm::vec3 max =
            position + halfExtents;

        const glm::vec3 p000(
            min.x, min.y, min.z
        );

        const glm::vec3 p001(
            min.x, min.y, max.z
        );

        const glm::vec3 p010(
            min.x, max.y, min.z
        );

        const glm::vec3 p011(
            min.x, max.y, max.z
        );

        const glm::vec3 p100(
            max.x, min.y, min.z
        );

        const glm::vec3 p101(
            max.x, min.y, max.z
        );

        const glm::vec3 p110(
            max.x, max.y, min.z
        );

        const glm::vec3 p111(
            max.x, max.y, max.z
        );

        auto AddLine =
            [&](const glm::vec3& a,
                const glm::vec3& b)
            {
                vertices.push_back(a);
                vertices.push_back(b);
            };

        // Bottom
        AddLine(p000, p001);
        AddLine(p001, p101);
        AddLine(p101, p100);
        AddLine(p100, p000);

        // Top
        AddLine(p010, p011);
        AddLine(p011, p111);
        AddLine(p111, p110);
        AddLine(p110, p010);

        // Sides
        AddLine(p000, p010);
        AddLine(p001, p011);
        AddLine(p100, p110);
        AddLine(p101, p111);
    }

    void DebugRenderer::AddSphere(
        std::vector<glm::vec3>& vertices,
        const glm::vec3& position,
        float radius
    )
    {
        constexpr int segments = 32;

        constexpr float twoPi =
            6.28318530718f;

        for (
            int i = 0;
            i < segments;
            ++i
            )
        {
            const float angleA =
                twoPi *
                static_cast<float>(i) /
                segments;

            const float angleB =
                twoPi *
                static_cast<float>(i + 1) /
                segments;

            const float cosA =
                std::cos(angleA);

            const float sinA =
                std::sin(angleA);

            const float cosB =
                std::cos(angleB);

            const float sinB =
                std::sin(angleB);

            // XY circle
            vertices.push_back(
                position +
                glm::vec3(
                    cosA * radius,
                    sinA * radius,
                    0.0f
                )
            );

            vertices.push_back(
                position +
                glm::vec3(
                    cosB * radius,
                    sinB * radius,
                    0.0f
                )
            );

            // XZ circle
            vertices.push_back(
                position +
                glm::vec3(
                    cosA * radius,
                    0.0f,
                    sinA * radius
                )
            );

            vertices.push_back(
                position +
                glm::vec3(
                    cosB * radius,
                    0.0f,
                    sinB * radius
                )
            );

            // YZ circle
            vertices.push_back(
                position +
                glm::vec3(
                    0.0f,
                    cosA * radius,
                    sinA * radius
                )
            );

            vertices.push_back(
                position +
                glm::vec3(
                    0.0f,
                    cosB * radius,
                    sinB * radius
                )
            );
        }
    }
}