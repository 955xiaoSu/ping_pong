#include "ping_pong.h"

void getOnePoint(uint16_t& team1_points, uint16_t& team2_points) {
    if (rand() % 2) {
        team1_points++;
    } else {
        team2_points++;
    }
}

void matchOnce(Team& team1, Team& team2) {
    static uint16_t game_round = 1;
    team1.status = GAMEING;
    team2.status = GAMEING;

    static uint16_t team1_points = 0, team2_points = 0;

    /* 在 11 分制的情况下，确保胜负已分 */
    while ((team1_points < 11 && team2_points < 11)
            || abs(team1_points - team2_points) < 2) {
        getOnePoint(team1_points, team2_points);
    }

    assert((team1_points >= 11 || team2_points >= 11) 
            && abs(team1_points - team2_points) >= 2);

    std::cout << "Game " << game_round++ << std::setw(15) << std::setfill(' ') 
        << team1.name << " vs " << team2.name << std::endl
        << std::setw(20) << std::setfill(' ') 
        << team1_points << " : " << team2_points << std::endl << std::endl;

    /* 根据胜负情况，调整对应变量 */
    if (team1_points > team2_points) {
        team1.match_win_times++;
        team1.status = WAITING;
        team1.current_round++;

        team2.match_lose_times++;
        team2.status = END;    
    } else {
        team2.match_win_times++;
        team2.status = WAITING;
        team2.current_round++;

        team1.match_lose_times++;
        team1.status = END;
    }

    team1.each_game_point.push_back(std::make_pair(team1.current_round, 
                                    std::make_pair(team1_points, team2_points)));
    team2.each_game_point.push_back(std::make_pair(team2.current_round,
                                    std::make_pair(team2_points, team1_points)));

    team1_points = team2_points = 0;
}

void match(Team& team1, Team& team2) {
    matchOnce(team1, team2);
}
