#pragma once

#include <memory>
#include <glm/glm.hpp>

class ObjectImpl;
class EngineImpl;

namespace tinyrender {

    const char OBJECT_HIDDEN = 0x00;
    const char OBJECT_VISIBLE = 0x01;

    class Object {
    protected:
        std::unique_ptr<ObjectImpl> obj;

        friend EngineImpl;
    public:
        Object();
        virtual ~Object();

        virtual void setColor(glm::vec3 c);
        virtual void setTexture(std::string path);
        virtual void setScale(float s); 
        virtual void setTranslation(glm::vec3 t);
        virtual void setRotationX(float r); 
        virtual void setRotationY(float r); 
        virtual void setRotationZ(float r);
        void setVisibility(char);
    };
}