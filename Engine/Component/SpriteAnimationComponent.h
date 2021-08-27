#pragma once
#include "SpriteComponent.h"
#include "SDL.h"
#include <map>

namespace nc
{
    class SpriteAnimationComponent : public SpriteComponent
    {
    private:
        struct Sequince {
            int fps{ 0 };
            int startFrame{ 0 };
            int endFrame{ 0 };
        };

    public:
        void Update() override;
        void Draw(Renderer* renderer) override;

        void SetSequence(const std::string& name);

        // Inherited via SpriteComponent
        virtual bool Write(const rapidjson::Value& value) const override;
        virtual bool Read(const rapidjson::Value& value) override;
    public:
        int frame = 0;
        int fps = 0;
        int numFramesX = 0;
        int numFramesY = 0;

        float frameTimer = 0;
        float frameTime = 0;

        int startFrame = 0;
        int endFrame = 0;

        SDL_Rect rect;

        std::map<std::string, Sequince> sequinces;

        std::string sequinceName;
    };
}