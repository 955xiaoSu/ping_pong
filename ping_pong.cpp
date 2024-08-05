#include "ping_pong.h"

void getOnePoint(uint16_t& team1_points, uint16_t& team2_points) {
    if (rand() % 2) {
        team1_points++;
    } else {
        team2_points++;
    }
}

void matchOnce(Team& team1, Team& team2) {
    static uint16_t team1_points = 0, team2_points = 0;

    /* 在 11 分制的情况下，确保胜负已分 */
    while ((team1_points < 11 && team2_points < 11)
            || abs(team1_points - team2_points) < 2) {
        getOnePoint(team1_points, team2_points);
    }

#ifdef DEBUG
    std::cout << team1.name << " vs " << team2.name << "\t" << 
        team1_points << " : " << team2_points << std::endl;
    assert((team1_points >= 11 || team2_points >= 11) 
            && abs(team1_points - team2_points) >= 2);
#endif

    if (team1_points > team2_points) {
        team1.match_win_times++;
        team2.match_lose_times++;
    } else {
        team2.match_win_times++;
        team1.match_lose_times++;
    }

    team1.each_game_point.push_back(std::make_pair(team1.current_round, 
                                    std::make_pair(team1_points, team2_points)));
    team2.each_game_point.push_back(std::make_pair(team2.current_round,
                                    std::make_pair(team2_points, team1_points)));
    team1.current_round++;
    team2.current_round++;

    team1_points = team2_points = 0;
}

void match(Team& team1, Team& team2) {
    matchOnce(team1, team2);
}
