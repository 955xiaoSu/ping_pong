/*
 * 乒乓比赛计分系统
 *
 * 从能实现某两个队伍之间的单盘比赛计分，到实现多盘比赛计分
 * 再到实现多个队伍的比赛计分、自动安排比赛，直至完善整个比赛计分系统
 * 
 * 预期处理规模为 64支 队伍，通过随机化输入，模拟比赛的结果
 * 最终输出比赛的结果（包括冠亚军、每支队伍的比赛胜负场次等信息）
 */

#include "ping_pong.cpp"

#ifdef DEBUG
void check_two_teams() {
    Team team1("CHECK_TEAM_1");
    Team team2("CHECK_TEAM_2");

    for (unsigned short i = 0; i < TESTNUM; i++) {
        match(team1, team2);

        /* 分别验证比赛次数、得分是否符合规范 */
        assert(team1.match_win_times + team1.match_lose_times == i + 1);
        assert(team2.match_win_times + team2.match_lose_times == i + 1);
        assert(team1.match_win_times == team2.match_lose_times);
        assert(team1.each_game_point[i].second.first == team2.each_game_point[i].second.second);
        assert(team1.each_game_point[i].second.second == team2.each_game_point[i].second.first);
        assert(team1.each_game_point[i].second.first >= 11 || team1.each_game_point[i].second.second >= 11);
        assert(abs(team1.each_game_point[i].second.first - team1.each_game_point[i].second.second) >= 2);
    }
}

void check_TEAMNUM_teams() {
   
}

void check() {
    check_two_teams();
    check_TEAMNUM_teams();
    std::cout << "All test passed!" << std::endl;
}
#endif

void only_for_demo() {
    Team team1("Kawayi");
    Team team2("Hello Kitty");

    for (unsigned short i = 0; i < MATCHNUM; i++) {
        match(team1, team2);
    }

    for (auto& each_game : team1.each_game_point) {
        std::cout << team1.name << " vs " << team2.name << "\t" << 
            each_game.second.first << " : " << each_game.second.second << std::endl;
    }

    if (team1.match_win_times > team2.match_win_times) {
        std::cout << team1.name << " is winner! " <<  std::endl;
    } else {
        std::cout << team2.name << " is winner! "  << std::endl;
    }

    return;
}

void release() {

}

 
int main() {
    srand(time(NULL));

#ifdef DEBUG
    check();
#endif  

#ifdef DEMO
    only_for_demo();
#endif

#ifdef RELEASE
    release();
#endif

    return 0;
}