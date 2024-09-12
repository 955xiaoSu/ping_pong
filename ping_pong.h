#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <assert.h>
#include <time.h>
#include <math.h>
#include "ping_pong_const.h"

typedef unsigned short uint16_t;

class Team {
    public:
        std::string name;
        std::vector<std::pair<uint16_t, std::pair<uint16_t, uint16_t>>> each_game_point;
        uint16_t match_win_times;
        uint16_t match_lose_times;
        uint16_t current_round;
        STATUS status;

        /* 对于单打比赛积分并不重要， 为循环赛留一个计分接口 */
        uint16_t match_points;

        Team(std::string name) {
            this->name = name;
            this->each_game_point.clear();
            this->match_win_times = 0;
            this->match_lose_times = 0;
            this->status = WAITING;
            this->current_round = 1;
            this->match_points = 0;
        }

        Team() {
            this->name = "NULL";
            this->each_game_point.clear();
            this->match_win_times = 0;
            this->match_lose_times = 0;
            this->status = WAITING;
            this->current_round = 1;
            this->match_points = 0;
        }
};

void getOnePoint(uint16_t& team1_points, uint16_t& team2_points);
void matchOnce(Team& team1, Team& team2);
void match(Team& team1, Team& team2);

/* 64支 参赛队伍的名称 */
const std::string team_name[64] = {
    "China", "USA", "Japan", "Korea", "Russia", "Germany", "France", "UK",
    "Italy", "Spain", "Portugal", "Brazil", "Argentina", "Australia", "Canada", "Mexico",
    "India", "Pakistan", "Iran", "Iraq", "Egypt", "South Africa", "Nigeria", "Kenya",
    "Saudi Arabia", "UAE", "Turkey", "Greece", "Sweden", "Norway", "Finland", "Denmark",
    "Switzerland", "Netherlands", "Belgium", "Austria", "Poland", "Czech", "Slovakia", "Hungary",
    "Romania", "Bulgaria", "Croatia", "Serbia", "Slovenia", "Bosnia", "Montenegro", "Albania",
    "Macedonia", "Kosovo", "Ukraine", "Belarus", "Lithuania", "Latvia", "Estonia", "Moldova",
    "Georgia", "Armenia", "Azerbaijan", "Kazakhstan", "Uzbekistan", "Turkmenistan", "Kyrgyzstan", "Tajikistan"
}; 
