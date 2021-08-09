#include <iostream>
#include <random>
#include <chrono>

using namespace std;
using namespace chrono;

int whish(int w) {
    switch (w) {
        case 1:
            cout << "角色池up五星" << endl;
            break;
        case 2:
            cout << "武器池指定up五星" << endl;
            break;
        case 3:
            cout << "常驻池特定五星角色" << endl;
            break;
        case 4:
            cout << "常驻池特定五星武器" << endl;
            break;
        case 5:
            cout << "角色池指定up四星" << endl;
            break;
        case 6:
            cout << "武器池指定up四星" << endl;
            break;
        case 7:
            cout << "常驻池特定四星角色" << endl;
            break;
        case 8:
            cout << "常驻池特定四星武器" << endl;
            break;
    }
    return 0;
}

int main() {
    // 随机引擎
    unsigned long seed = system_clock::now().time_since_epoch().count();
    mt19937 e(seed);
    uniform_real_distribution<double> ri(0.0, 1.0);
    while (1){
        // 循环变量
        int i, j;

        // 什么池子，抽几个目标，预算抽数
        int wish = 0, goal = 0, budget = 0, pgeo, fate, money;
        cout << "原神抽卡预算与出货期望计算器" << endl;
        cout << "1 角色池 五星 唯一up角色" << endl;
        cout << "2 武器池 五星 指定up武器" << endl;
        cout << "3 常驻池 五星 某一角色" << endl;
        cout << "4 常驻池 五星 某一武器" << endl;
        cout << "5 角色池 四星 指定up角色" << endl;
        cout << "6 武器池 四星 指定up武器" << endl;
        cout << "7 常驻池 四星 某一角色" << endl;
        cout << "8 常驻池 四星 某一武器" << endl;
        cout << "输入数字选择项目（随时可以使用Ctrl+C退出程序）：";
        cin >> wish;
        system("clear");

        whish(wish);
        cout << "\n想要抽到几个：";
        cin >> goal;
        cout << "\n填入抽卡预算。原石、之缘、钱数分别填入，会自动合计。\n";
        cout << "原石数：";
        cin >> pgeo;
        cout << "单抽之缘：";
        cin >> fate;
        cout << "氪金钱数：";
        cin >> money;
        budget = (int) ((double) money / 648 * 8080 + pgeo) / 160 + fate;
        system("clear");

        // 当前保底进度
        int bd4 = 0, up4 = 0, bd5 = 0, up5 = 0;
        whish(wish);
        printf("目标抽出%d个，预算%d抽。\n", goal, budget);
        cout << "\n该祈愿池历史记录中，" << endl;
        if (wish > 4) {
            cout << "上次出紫后垫了几抽：";
            cin >> bd4;
            if (wish < 7) {
                cout << "上次出的紫卡是UP填0，歪了填1：";
                cin >> up4;
            }
        }
        cout << "上次出金后垫了几抽：";
        cin >> bd5;
        if (wish < 3) {
            cout << "上次出的金卡是UP填0，歪了填1：";
            cin >> up5;
        }
        system("clear");

        // 尝试次数
        int gachan = 0;
        cout << "将要模拟N人按如上设定抽卡\nN越高，结果越精准，但耗时越长\n可设1万~10亿，推荐100000" << endl;
        cout << "请输入想设定的N值(纯数字)：";
        cin >> gachan;
        system("clear");

        cout << "条件已设定，计算中……" << endl;
        cout << "总次数：" << gachan << endl;
        auto stime = system_clock::now();

        // 金紫出货率与抽数
        double b5[91] = {0}, b4[21] = {0};
        if (wish == 2 || wish == 6) { // 武器池
            for (i = 1; i < 81; i++) {
                if (i < 63)b5[i] = 0.007;
                else if (i < 74)b5[i] = 0.007 + 0.07 * (i - 62);
                else if (i < 80)b5[i] = 0.777 + 0.035 * (i - 73);
                else b5[i] = 1;
            }
            for (i = 1; i < 21; i++) {
                if (i < 8)b4[i] = 0.06;
                else if (i == 8)b4[i] = 0.66;
                else if (i == 9)b4[i] = 0.96;
                else b4[i] = 1;
            }
        } else { // 常驻池、角色池
            for (i = 1; i < 91; i++) {
                if (i < 74)b5[i] = 0.006;
                else if (i < 90)b5[i] = 0.006 + 0.06 * (i - 73);
                else b5[i] = 1;
            }
            for (i = 1; i < 21; i++) {
                if (i < 9)b4[i] = 0.051;
                else if (i == 9)b4[i] = 0.561;
                else b4[i] = 1;
            }
        }

        // 统计
        long long int sum = 0; // 总抽数
        int get = 0; // 预算以内实现目标的人数

        // 状态变量
        double temp; // 概率临时值
        int pbd, pup, gbd, gup; // 保底进度
        int tot; // 抽到目标的个数

        // 主计算
        for (i = 0; i < gachan; i++) {
            if (i % (gachan / 100) == 0)
                printf("计算进度：%.0lf%%\r", (double) i / gachan * 100);
            pbd = bd4 + 1, pup = up4;
            gbd = bd5 + 1, gup = up5;
            tot = 0;
            int fatePoint = 0;
            for (j = 1; j < 999999; j++) {
                temp = ri(e);
                if (temp < b5[gbd]) // 出金
                    {
                    gbd = 1;
                    if (wish > 4)pbd++;
                    else if (wish == 1) {
                        if (gup || ri(e) < 0.5) tot++, gup = 0;
                        else gup = 1;
                    } else if (wish == 2) {
                        //出金之后的情况分类：
                        // 1. 上一次歪了||在0.75区间内||命定值满了：gup归零
                        // 1.1 在0.5区间内，中了，命定值归零
                        // 1.2 歪了，命定值+1
                        // 2. 献祭命定值救回来了，命定值归零
                        // 2.1 命定值不够，还是歪了，命定值+1
                        // 3. 在0.75区间外，完全歪了，命定值+1并且下一次必定是情况1
                        if (gup || ri(e) < 0.75 || fatePoint >= 2) { //1. 上一次歪了||在0.75区间内||命定值满了：gup归零
                            gup = 0;
                            if (ri(e) < 0.5) { // 1.1 再roll一次，在二分之一可能性里中了，命定值归零
                                tot++;
                                fatePoint = 0;
                            } else { // 1.2 歪了，命定值+1
                                if (fatePoint >= 2) {// 2. 献祭命定值救回来了，命定值归零
                                    tot++;
                                    fatePoint = 0;
                                } else { // 2.1 命定值也不够，又没有钞能力，拿来吧你（指648）
                                    fatePoint++;
                                }
                            }
                        } else { // 3. gup=0，在0.75区间外，命定值不满2：命定值+1并且下一次必定是情况1
                            gup = 1;
                            fatePoint++;
                        }
                    } else if (wish == 3) { // 常驻池5个五星角色
                        if (ri(e) < (double) 1 / 2 / 5) tot++;
                    } else if (wish == 4) { // 常驻池10个五星武器
                        if (ri(e) < (double) 1 / 2 / 10) tot++;
                    }
                    } else if (temp < b4[pbd] + b5[gbd] && wish > 4) // 出紫
                        {
                    gbd++, pbd = 1;
                    if (wish == 5) {
                        if (pup || ri(e) < 0.5) {
                            if (ri(e) < (double) 1 / 3)tot++;
                            pup = 0;
                        } else pup = 1;
                    } else if (wish == 6) {
                        if (pup || ri(e) < 0.75) {
                            if (ri(e) < (double) 1 / 5)tot++;
                            pup = 0;
                        } else pup = 1;
                    } else if (wish == 7) { // 常驻池17个四星角色
                        if (ri(e) < (double) 1 / 17) tot++;
                    } else if (wish == 8) { // 常驻池18个四星武器
                        if (ri(e) < (double) 1 / 18) tot++;
                    }
                        } else {
                    gbd++;
                    if (wish > 4)pbd++;
                }

                // 数量达标后
                if (tot == goal) {
                    sum += j;
                    if (j <= budget)get++;
                    break;
                }
            }
        }

        auto etime = system_clock::now();
        auto duration = duration_cast<microseconds>(etime - stime);
        system("clear");
        printf("计算完毕！耗时 %.3lf 秒\n\n", double(duration.count()) * microseconds::period::num / microseconds::period::den);

        // 结果输出
        whish(wish);
        if (wish > 4) {
            printf("上次出紫");
            if (wish < 7) {
                if (up4)printf("歪了，");
                else printf("没歪，");
            }
            printf("之后又垫了%d抽\n", bd4);
        }
        printf("上次出金");
        if (wish < 3) {
            if (up5)printf("歪了，");
            else printf("没歪，");
        }
        printf("之后又垫了%d抽\n\n", bd5);

        cout << "出货目标：" << goal << endl;
        cout << "预算抽数：" << budget << endl;

        printf("\n总%d人所需的平均抽数 %.2lf 次\n", gachan, (double) sum / gachan);
        printf("%d人在预算以内达成，占比 %.3lf%%\n", get, (double) get / gachan * 100);
        cout << "按Enter开始下一次（ctrl+C退出）……" << endl;
        system("read");
        system("clear");
    }
}
