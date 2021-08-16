#include "Player.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Math/MathUtil.h"
#include "Game.h"
#include <memory>
#include <vector>

Player::Player(const nc::Transform transform, std::shared_ptr<nc::Texture> texture, float speed, bool isEndless) : nc::Actor{ transform, texture }, speed{ speed }, isEndless{isEndless}
{
    std::unique_ptr locator = std::make_unique<Actor>();
    locator->transform.localPosition = nc::Vector2{ 0,4 };
    AddChild(std::move(locator));

    locator = std::make_unique<Actor>();
    locator->transform.localPosition = nc::Vector2{ 4,0 };
    AddChild(std::move(locator));

    locator = std::make_unique<Actor>();
    locator->transform.localPosition = nc::Vector2{ -4,0 };
    AddChild(std::move(locator));
}

void Player::OnCollision(Actor* actor)
{
    if ((dynamic_cast<Enemy*>(actor) && immunityFrameTimer <= 0) || ((dynamic_cast<Projectile*>(actor) && actor->tag == "enemy") && immunityFrameTimer <= 0))
    {
        nc::Event event;
        event.name = "PlayerDead";
        scene->engine->Get<nc::EventSystem>()->Notify(event);
        immunityFrameTimer = immunityFrames;
    }
}

void Player::Update(float dt)
{
    Actor::Update(dt);

    //movement
    float thrust = 0;
    if (scene->engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_A) == nc::InputSystem::eKeyState::Held) transform.rotation -= 5 * dt;
    if (scene->engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_D) == nc::InputSystem::eKeyState::Held) transform.rotation += 5 * dt;
    if (scene->engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_W) == nc::InputSystem::eKeyState::Held) thrust = speed;

    transform.position += nc::Vector2::Rotate(nc::Vector2::down, transform.rotation) * thrust * dt;
    transform.position.x = nc::Wrap(transform.position.x, 0.0f, 800.0f);
    transform.position.y = nc::Wrap(transform.position.y, 0.0f, 600.0f);

    //fire
    fireTimer -= dt;
    immunityFrameTimer -= dt;
    if (fireTimer <= 0 && (scene->engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == nc::InputSystem::eKeyState::Held))
    {
        std::shared_ptr<nc::Texture> texture = scene->engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("One_Alex_Churro.png", scene->engine->Get<nc::Renderer>());
        std::shared_ptr<nc::Texture> texture2 = scene->engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("Star.png", scene->engine->Get<nc::Renderer>());

        {
            nc::Transform t = children[0]->transform;
            if (isEndless) {
                t.scale = .02;
                std::unique_ptr<Projectile> projectile = std::make_unique<Projectile>(t, texture2, 300.0f);
                projectile->tag = "player";
                scene->AddActor(std::move(projectile));
            }
            else {
                t.scale = .15;
                t.position.x += 30;
                t.position.y += 40;
                std::unique_ptr<Projectile> projectile = std::make_unique<Projectile>(t, texture, 300.0f);
                projectile->tag = "player";
                scene->AddActor(std::move(projectile));
            }
        }

        scene->engine->Get<nc::AudioSystem>()->PlayAudio("playerShoot");

        fireTimer = fireRate;
    }

    transform.Update();
}
