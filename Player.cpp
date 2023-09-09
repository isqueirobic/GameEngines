#include "Player.h"

Player::Player(Game& jogo, float x, float y, float angle, float senci, float fov, float velo) {
    px = x, py = y, pdx = 0, pdy = 0, pvelo = velo*100000, pfov = fov, psens = senci, this->angle = angle, this->jogo = &jogo, window = this->jogo->window;
}

void Player::Move() { // your angle will detern where u go
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            px += pdx;
            py += pdy;
        
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        angle += psens;
        if (angle > PI * 2) {
            angle -= 2 * PI;
        }
        pdx = cos(angle) * 2;
        pdy = sin(angle) * 2;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        px -= pdx;
        py -= pdy;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        angle -= psens;
        if (angle < 0) {
            angle += 2 * PI;
        }
        pdx = cos(angle) * 2;
        pdy = sin(angle) * 2;
    }
    


}

void Player::MoveSimple() { // x+ y+ x- y- 
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        py -= pvelo * jogo->deltaTimeSeconds;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        py += pvelo * jogo->deltaTimeSeconds;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        px += pvelo * jogo->deltaTimeSeconds;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        px -= pvelo * jogo->deltaTimeSeconds;
    }
}
