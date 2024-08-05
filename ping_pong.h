#include <iostream>
#include <string>
#include <vector>
#include <assert.h>
#include <time.h>
#include "ping_pong_const.h"

typedef unsigned short uint16_t;

class Team {
    public:
        std::string name;
        std::vector<std::pair<uint16_t, std::pair<uint16_t, uint16_t>>> each_game_point;
        uint16_t match_win_times;
        uint16_t match_lose_times;
        uint16_t current_round;

        /* 对于单打比赛积分并不重要， 为循环赛留一个计分接口 */
        uint16_t match_points;

        Team(std::string name) {
            this->name = name;
            this->each_game_point.clear();
            this->match_win_times = 0;
            this->match_lose_times = 0;
            this->current_round = 1;
            this->match_points = 0;
        }
};