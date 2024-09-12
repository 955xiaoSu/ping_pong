/*
 * 乒乓比赛计分系统
 *
 * 从能实现某两个队伍之间的单盘比赛计分，到实现多盘比赛计分
 * 再到实现多个队伍的比赛计分、自动安排比赛（考虑并发安排），直至完善整个比赛计分系统
 * 
 * 预期处理规模为 64支 队伍，通过随机化输入，模拟比赛的结果（设定是输一把便出局）
 * 最终输出比赛的结果（包括冠亚军、每支队伍的比赛胜负场次等信息）
 */

#include "ping_pong.h"

#ifdef TEST
void check_two_teams() {
    Team team1("CHECK_TEAM_1");
    Team team2("CHECK_TEAM_2");

    for (unsigned short i = 0; i < TEST_TWOTEAMS_GAMES; i++) {
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
    Team teams[TEAMNUM];
    for (uint16_t i = 0; i < TEAMNUM; i++) {
        teams[i] = Team(team_name[i]);
    }

    /* 
     * 每安排一场比赛就会有一支队伍出局，所以只需要安排 TEAMNUM-1 场比赛
     * 每一场比赛结束后，随机抽两个队伍进行下一场比赛，所以整体队伍的比赛次数分布是不均匀的
     */
    for (uint16_t i = 0; i < TEAMNUM - 1; i++) {
        uint16_t team1_index = rand() % TEAMNUM;
        uint16_t team2_index = rand() % TEAMNUM;
        while (team1_index == team2_index 
               || teams[team1_index].status != WAITING || teams[team2_index].status != WAITING) {
            team1_index = rand() % TEAMNUM;
            team2_index = rand() % TEAMNUM;
        }
        match(teams[team1_index], teams[team2_index]);

        assert(teams[team1_index].status == END || teams[team2_index].status == END);
        assert(teams[team1_index].status == WAITING || teams[team2_index].status == WAITING);
        assert(teams[team1_index].match_lose_times == 1 || teams[team2_index].match_lose_times == 1);
        assert((teams[team1_index].current_round == teams[team1_index].match_win_times + 1)
                && (teams[team2_index].current_round == teams[team2_index].match_win_times + 1));
    }

    /* 验证比赛是否均已结束，处在 WAITING 状态的只可能是冠军队伍 */
    uint16_t waiting_team_num = 0;
    for (uint16_t i = 0; i < TEAMNUM; i++) {
        if (teams[i].status == WAITING) {
            waiting_team_num++;
        }
        assert((teams[i].status == END) || waiting_team_num <= 1);
    }

    return;
}

void check() {
    for (uint16_t i = 0; i < TESTNUM; i++) {
        check_two_teams();
        check_TEAMNUM_teams();
        std::cout << "Test " << i << " passed!" << std::endl;
    }
    std::cout << "All test passed!" << std::endl;
}
#endif

#ifdef DEMO
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
#endif

#ifdef RELEASE
void displayResult(const Team (&teams)[]) {
    /* 找出冠亚军 */
    uint16_t champion_index = TEAMNUM, runner_up_index = TEAMNUM;
    uint16_t champion_win_times = 0;
    
    uint16_t tmp = TEAMNUM; 
    while (tmp != 1) {
        tmp /= 2;
        champion_win_times++;
    }

    for (uint16_t i = 0; i < TEAMNUM; i++) {
        if (teams[i].status == WAITING) {
            champion_index = i;
        } else if (teams[i].match_win_times == champion_win_times - 1) {
            runner_up_index = i;
        }
        if (champion_index != TEAMNUM && runner_up_index != TEAMNUM) {
            break;
        }
    }

    std::cout << "Champion: " << teams[champion_index].name << std::endl;
    std::cout << "Runner-up: " << teams[runner_up_index].name << std::endl << std::endl;

    /* 输出每支队伍的比赛胜负场次 */
    for (uint16_t i = 0; i < TEAMNUM; i++) {
        std::cout << std::setw(15) << std::setfill(' ') 
            << teams[i].name << " win_times : " << teams[i].match_win_times << std::endl;
    }

    return;
}

void release() {
    Team teams[TEAMNUM];
    for (uint16_t i = 0; i < TEAMNUM; i++) {
        teams[i] = Team(team_name[i]);
    }

    /* 
     * 安排一场比赛就会有一支队伍出局，在有 TEAMNUM 支队伍的情况下需要安排 TEAMNUM-1 场比赛 
     * 每次安排最相邻且处于 WAITING 状态的两支队伍进行比赛，模拟分区赛制 
     */
    uint16_t team1_index = 0, team2_index = 0;
    uint16_t team1_index_boundary = TEAMNUM - 2;
    for (uint16_t i = 0; i < TEAMNUM - 1; i++) {
        while (teams[team1_index].status != WAITING) {
            team1_index++;
            if (team1_index > team1_index_boundary) {
                team1_index = 0;
            }
        }
        team2_index = team1_index + 1;
        while (teams[team2_index].status != WAITING) {
#ifdef DEBUG
            std::cout << std::endl << "team1_index: " << team1_index
                << std::endl << "team2_index: " << team2_index << std::endl;
#endif
            team2_index++;
            assert(team2_index < TEAMNUM);
        }

#ifdef DEBUG
        std::cout << std::endl << "team1_index: " << team1_index
            << std::endl << "team2_index: " << team2_index << std::endl;
#endif

        match(teams[team1_index], teams[team2_index]);

        /* 保证一轮比赛中，一支队伍只比一次，即便它胜了也要等到下一轮再比 */
        team1_index = team2_index + 1;
        if (team1_index > team1_index_boundary) {
            team1_index = 0;
        }
    }
    
    displayResult(teams);
    return;
}
#endif

 
int main() {
    srand(time(NULL));

#ifdef TEST
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
