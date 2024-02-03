#include "generic_ball.h"

#include "managers/asset_manager.h"

GenericBall::GenericBall(const glm::vec3& position)
    : PoolBall(position, *AssetManager::request<Model>(
                             "assets/models/red_ball.model.xml")) {}
