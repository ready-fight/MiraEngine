#pragma once

#include <memory>
#include <vector>

#include <glm/vec3.hpp>


namespace MiraEngine
{
    class GameObject;

    class Scene
    {
    public:
        void AddObject(std::unique_ptr<GameObject> object);
        void DrawUI();

        std::vector<std::unique_ptr<GameObject>>& GetObjects();
        const std::vector<std::unique_ptr<GameObject>>& GetObjects() const;

        bool IsColliding(
            const GameObject& a,
            const GameObject& b
        ) const;

        bool IsColliding(
            const GameObject& object
        ) const;

        bool TryMove(
            GameObject& object,
            const glm::vec3& movement
        );

        void Update(float deltaTime);

    private:
        std::vector<std::unique_ptr<GameObject>> m_objects;
    };
}