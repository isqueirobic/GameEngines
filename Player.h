#ifndef Player1
#define Player1
#include"Engine.h"
class Player {
public:
    char MAP[128] = {   '1','1','1','1','1','1','1','1',
                        '1',' ',' ',' ',' ',' ',' ','1',
                        '1',' ',' ',' ',' ',' ',' ','1',
                        '1',' ',' ',' ',' ',' ',' ','1',
                        '1',' ',' ','1',' ',' ',' ','1',
                        '1',' ',' ',' ',' ',' ',' ','1',
                        '1',' ','1',' ',' ',' ',' ','1',
                        '1',' ',' ',' ','2',' ',' ','1',
                        '1',' ','1',' ',' ',' ',' ','1',
                        '1',' ',' ',' ',' ',' ',' ','1',
                        '1',' ','1',' ',' ',' ',' ','1',
                        '1',' ',' ','1',' ',' ',' ','1',
                        '1',' ','1',' ',' ',' ',' ','1',
                        '1',' ',' ',' ',' ',' ',' ','1',
                        '1',' ',' ',' ',' ',' ',' ','1',
                        '1','1','1','1','1','1','1','1' };
    int MapWidth = 8, Maphight = 16;
    float px, py, pdx, pdy, pvelo, psens, pfov, angle;
    GLFWwindow* window;
    Game* jogo;
    Player* player;

    Player(Game& jogo, float x, float y, float angle=0.3, float senci=0.1, float fov=4, float velo=400);

    void Move();

    void MoveSimple();
};
#endif // !Player

