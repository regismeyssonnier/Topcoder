// Frogs2.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>   
#include <chrono>
#include <map>
#include <queue>
using namespace std;
using namespace std::chrono;


#define ll long long int

//#define MAX_COIN 150

namespace State
{
    typedef enum {
        beach = '#',
        water = '.',
        log = '=',
        frog = '@',
        coin = 'o'
    }Type;
}

namespace GameState {
    typedef enum {
        home = 1,
        search_coin,
        go_beach,
        replace


    }Type;

    typedef enum {
        go_down = 1,
        go_up


    }UnderType;

}


namespace Frog {
    typedef enum {
        one = 1,
        two,
        three,
        four,
        five


    }Type;
}

namespace FlowState {
    typedef enum {
        left = '<',
        right = '>'


    }Type;
}



struct Grid {
    int H;
    int W;
    int F;
    int KL;
    int KW;
    double PC;
    int nb_coins;
    ll score;
    int turn;
    bool down = false;

    vector<char> rowDir;
    vector<vector<char>> grid;

    void Init_grid(vector<vector<char>> g) {
        grid.swap(g);
    }

    void Init_rowDir(vector<char> r) {
        rowDir.swap(r);
    }

    void Copy_grid(vector<vector<char>>& cgrid) {

        for (int i = 0; i < H; ++i) {
            for (int j = 0; j < W; ++j) {
                cgrid[i][j] = grid[i][j];
            }
        }
    }

};

struct Coord {
    int x;
    int y;
};

struct Coord2 {
    int x;
    int y;
    int ind;
};

struct CoordF {
    int x;
    int y;
    int nb_coins;
};

struct Score {
    int nb_coins=0;
    int score=0;
    char dir='A';
};

class Node {
public:
    vector<Node*> child;
    Node* parent;
    int n;
    int ns;
    double ucb;
    Coord num;
    Coord frog;
    Grid grid;
    double score;
    int nbcoins;
    bool finalf;
    char dir;
    Node() {
        this->parent = nullptr;
        this->n = 0;
        this->ns = 0;
        this->ucb = 0;
        this->num = { 0,0 };
        this->score = 0;
        this->frog = { 0,0 };
        this->nbcoins = 0;
        this->finalf = false;

    }

    void Reset() {
        this->parent = nullptr;
        this->n = 0;
        this->ns = 0;
        this->ucb = 0;
        this->num = { 0,0 };
        this->score = 0;
        this->frog = { 0,0 };
        this->child.clear();
        this->nbcoins = 0;
        this->finalf = false;
        this->dir = 'x';
    }

};

class GameFrog {

public:
    vector<vector<bool>> vise;
    vector<char> rowDir;
    ll MAX_COIN;
    int MAX_TURN;
    int MAX_DEPTH;
    int TOTAL_COIN;
    bool DOWN;
   

    void Init_visE(int H, int W) {
        this->vise.clear();
        vise.resize(H);
        for (int i = 0; i < H; ++i)vise[i].resize(W, false);
    }

    
    GameFrog(int H, int W, vector<char> rdir) {
        this->Init_visE(H, W);
        this->rowDir = rdir;
        MAX_COIN = 5000;
        MAX_TURN = 870;
        MAX_DEPTH = H;
        TOTAL_COIN = 0;
        DOWN = true;
    }

    bool Get_up(Grid grid, Coord frog) {
        if (frog.y - 1 >= 0) {
            if (grid.grid[frog.y - 1][frog.x] == State::log || grid.grid[frog.y - 1][frog.x] == State::coin) {
                return true;
            }
        }

        return false;
    }

    bool Get_down(Grid grid, Coord frog) {
        if (frog.y + 1 < grid.H) {
            if (grid.grid[frog.y + 1][frog.x] == State::log || grid.grid[frog.y + 1][frog.x] == State::coin) {
                return true;
            }
        }

        return false;

    }

    bool Get_left(Grid grid, Coord frog) {
        if (frog.x - 1 >= 0) {
            if (grid.grid[frog.y ][frog.x- 1] == State::log || grid.grid[frog.y ][frog.x- 1] == State::coin) {
                return true;
            }
        }

        return false;
    }

    bool Get_right(Grid grid, Coord frog) {
        if (frog.x + 1 < grid.W) {
            if (grid.grid[frog.y][frog.x + 1] == State::log || grid.grid[frog.y][frog.x + 1] == State::coin) {
                return true;
            }
        }

        return false;
    }

    
    void GetPossibleMoveSim(Grid grid, vector<Coord>& indice, vector<char>& vdir, Coord frog, int dirblock) {



        if (dirblock == 3 && frog.y - 1 >= 0) {
            if (grid.grid[frog.y - 1][frog.x] == State::log || grid.grid[frog.y - 1][frog.x] == State::coin) {
                indice.push_back({ frog.x, frog.y - 1 });
                vdir.push_back('U');
            }

            if (indice.size() == 0) {
                if ( frog.x - 1 >= 0) {
                    if (grid.grid[frog.y][frog.x - 1] == State::log || grid.grid[frog.y][frog.x - 1] == State::coin) {
                        indice.push_back({ frog.x - 1, frog.y });
                        vdir.push_back('L');
                    }

                }
                if ( frog.x + 1 < grid.W) {
                    if (grid.grid[frog.y][frog.x + 1] == State::log || grid.grid[frog.y][frog.x + 1] == State::coin) {
                        indice.push_back({ frog.x + 1, frog.y });
                        vdir.push_back('R');
                    }

                }

            }

        }
        else if (dirblock == 1 && frog.y + 1 < grid.H) {
            if (grid.grid[frog.y + 1][frog.x] == State::log || grid.grid[frog.y + 1][frog.x] == State::coin) {
                indice.push_back({ frog.x, frog.y + 1 });
                vdir.push_back('D');
            }

            if (indice.size() == 0) {
                if (frog.x - 1 >= 0) {
                    if (grid.grid[frog.y][frog.x - 1] == State::log || grid.grid[frog.y][frog.x - 1] == State::coin) {
                        indice.push_back({ frog.x - 1, frog.y });
                        vdir.push_back('L');
                    }

                }

                if ( frog.x + 1 < grid.W) {
                    if (grid.grid[frog.y][frog.x + 1] == State::log || grid.grid[frog.y][frog.x + 1] == State::coin) {
                        indice.push_back({ frog.x + 1, frog.y });
                        vdir.push_back('R');
                    }

                    
                }

            }

        }


    }

    void GetPossibleMoveSim2(Grid grid, vector<Coord>& indice, vector<char>& vdir, Coord frog, int dirblock) {



        if (dirblock == 2 && frog.x - 1 >= 0) {
            if (grid.grid[frog.y][frog.x - 1] == State::log || grid.grid[frog.y][frog.x - 1] == State::coin) {
                indice.push_back({ frog.x - 1, frog.y });
                vdir.push_back('L');
            }

            if (indice.size() == 0) {
                if (frog.y - 1 >= 0) {
                    if (grid.grid[frog.y- 1][frog.x ] == State::log || grid.grid[frog.y- 1][frog.x ] == State::coin) {
                        indice.push_back({ frog.x, frog.y  - 1});
                        vdir.push_back('U');
                    }

                }
                if (frog.y + 1 < grid.H) {
                    if (grid.grid[frog.y+ 1][frog.x ] == State::log || grid.grid[frog.y+ 1][frog.x ] == State::coin) {
                        indice.push_back({ frog.x, frog.y  + 1});
                        vdir.push_back('D');
                    }

                }

            }

        }
        else if (dirblock == 4 && frog.x + 1 < grid.W) {
            if (grid.grid[frog.y ][frog.x+ 1] == State::log || grid.grid[frog.y][frog.x + 1] == State::coin) {
                indice.push_back({ frog.x+ 1, frog.y  });
                vdir.push_back('R');
            }

            if (indice.size() == 0) {
                if (frog.y - 1 >= 0) {
                    if (grid.grid[frog.y- 1][frog.x ] == State::log || grid.grid[frog.y- 1][frog.x ] == State::coin) {
                        indice.push_back({ frog.x , frog.y- 1 });
                        vdir.push_back('U');
                    }
                }

                if (frog.y + 1 < grid.H) {
                    if (grid.grid[frog.y+ 1][frog.x ] == State::log || grid.grid[frog.y+ 1][frog.x ] == State::coin) {
                        indice.push_back({ frog.x , frog.y+ 1 });
                        vdir.push_back('D');
                    }

                    
                }

            }

        }


    }

    void GetPossibleMove(Grid grid, vector<Coord>& indice, vector<char>& vdir, Coord frog) {

        if (frog.y - 1 >= 0) {
            if (grid.grid[frog.y - 1][frog.x] == State::log || grid.grid[frog.y - 1][frog.x] == State::coin) {
                indice.push_back({ frog.x, frog.y - 1 });
                vdir.push_back('U');
            }
        }

        if (frog.y + 1 < grid.H) {
            if (grid.grid[frog.y + 1][frog.x] == State::log || grid.grid[frog.y + 1][frog.x] == State::coin) {
                indice.push_back({ frog.x, frog.y + 1 });
                vdir.push_back('D');
            }
        }


        if (frog.x - 1 >= 0) {
            if (grid.grid[frog.y][frog.x - 1] == State::log || grid.grid[frog.y][frog.x - 1] == State::coin) {
                indice.push_back({ frog.x - 1, frog.y });
                vdir.push_back('L');
            }

        }
        if (frog.x + 1 < grid.W) {
            if (grid.grid[frog.y][frog.x + 1] == State::log || grid.grid[frog.y][frog.x + 1] == State::coin) {
                indice.push_back({ frog.x + 1, frog.y });
                vdir.push_back('R');
            }

        }

    }

    ll GetSafety(Grid &grid, Coord ind, Coord frog) {
        ll safety = 500;

        if (ind.x > grid.W / 5 && ind.x < grid.W * 4/5) {
            safety += 100;
        }
        else {
            safety -= 200;
        }

        int water = 0;
        if (ind.y - 1 >= 0 && (grid.grid[ind.y-1][ind.x] == State::water || grid.grid[ind.y - 1][ind.x] == State::frog)) {
            water++;
        }
        if (ind.y + 1 <grid.H && (grid.grid[ind.y+1][ind.x] == State::water || grid.grid[ind.y + 1][ind.x] == State::frog)) {
            water++;
        }
        if (ind.x - 1 >= 0 && (grid.grid[ind.y][ind.x-1] == State::water || grid.grid[ind.y ][ind.x- 1] == State::frog)) {
            water++;
        }
        if (ind.x + 1 < grid.W && (grid.grid[ind.y][ind.x + 1] == State::water || grid.grid[ind.y][ind.x+1] == State::frog)) {
            water++;
        }
        
        if (ind.y + 1 >= grid.H ) {
            water++;
        }
        if ( ind.x - 1 < 0 ) {
            water++;
        }
        if (ind.x + 1 >= grid.W) {
            water++;
        }

        if (water > 3)safety -= 200;
        //else if (water == 2) safety += 50;
        else safety += 100;

        return safety;


    }

    void GetScore(Grid &grid, int i, Score & score, vector<Coord>& indice, vector<char>& vdir, Coord frog) {

        if (grid.grid[indice[i].y][indice[i].x] == State::coin) {
            grid.nb_coins += indice[i].y;
            score.nb_coins += indice[i].y;
            score.score += indice[i].y;
        }
        else if (grid.grid[indice[i].y][indice[i].x] == State::log) {
            score.score += 1;
        }

        if (grid.nb_coins < this->MAX_COIN && indice[i].y > frog.y) {
            score.score += 1;
        }
        /*else if (grid.nb_coins < this->MAX_COIN && indice[i].y < frog.y) {
            score.score += 100000;
        }*/
        if (grid.nb_coins >= this->MAX_COIN && indice[i].y < frog.y) {
            score.score += 1;
        }

        if (indice[i].x >= grid.W / 2 && this->rowDir[indice[i].y] == FlowState::right) {
            score.score -= 1 ;
        }
        if (indice[i].x <= grid.W / 2 && this->rowDir[indice[i].y] == FlowState::left) {
            score.score -= 1 ;
        }

        if (indice[i].x <= grid.W / 2 && this->rowDir[indice[i].y] == FlowState::right) {
            score.score += 1 ;
        }
        if (indice[i].x >= grid.W / 2 && this->rowDir[indice[i].y] == FlowState::left) {
            score.score += 1 ;
        }

        if (indice[i].x < grid.W / 4) {
            score.score -= 1 ;
        }
        else if (indice[i].x > (grid.W - (grid.W / 4))) {
            score.score -= 1 ;
        }
        else {
            if (indice[i].x < grid.W / 2) {
                score.score += 2;
            }
            else {
                score.score += 2;
            }
        }

        int lc = 0;
        if (indice[i].y - 1 >= 0 && (grid.grid[indice[i].y - 1][indice[i].x] == State::log || grid.grid[indice[i].y - 1][indice[i].x] == State::coin)) {
            lc++;
        }
        if (indice[i].y + 1 < grid.H && (grid.grid[indice[i].y + 1][indice[i].x] == State::log || grid.grid[indice[i].y + 1][indice[i].x] == State::coin)) {
            lc++;
        }
        if (indice[i].x - 1 >= 0 && (grid.grid[indice[i].y][indice[i].x - 1] == State::log || grid.grid[indice[i].y ][indice[i].x- 1] == State::coin)) {
            lc++;
        }
        if (indice[i].x + 1 < grid.W && (grid.grid[indice[i].y][indice[i].x + 1] == State::log || grid.grid[indice[i].y ][indice[i].x+1] == State::coin)) {
            lc++;
        }

        score.score += lc;


    }

    string Simulate3(Grid grid, Coord frog, float time, Score& score) {

        auto start = high_resolution_clock::now();
        auto keepSolving = [&start, time]() -> bool
        {
            auto now = high_resolution_clock::now();
            auto d = duration_cast<microseconds>(now - start);
            return (d.count() < time);
        };
                

        Grid root;
        root.W = grid.W;
        root.H = grid.H;
        root.KL = grid.KL;
        root.KW = grid.KW;
        root.PC = grid.PC;
        root.nb_coins = grid.nb_coins;
        root.turn = grid.turn;

        vector<vector<char>>g(grid.H, vector<char>(grid.W));
        root.Init_grid(g);

        grid.Copy_grid(root.grid);

        Coord frog_curr = { frog.x, frog.y };
        char dir = 'X';
        string move;

        int bornl = grid.W / 4, bornr = grid.W *3/4;
        if (frog.x < bornl && this->rowDir[frog.y] == FlowState::right) {
            return "";
        }
        else if ((frog.x > bornr) && this->rowDir[frog.y] == FlowState::left) {
            return "";
        }



        if (frog.y + 1 < grid.W && frog.x < bornl && this->rowDir[frog.y + 1] == FlowState::right//){
            && (grid.grid[frog.y + 1][frog.x] == State::log || grid.grid[frog.y + 1][frog.x] == State::coin)) {
            return to_string(frog.y) + " " + to_string(frog.x) + " D";
        }
        else if (frog.y + 1 < grid.W && (frog.x > bornr) && this->rowDir[frog.y + 1] == FlowState::left//){
            && (grid.grid[frog.y + 1][frog.x] == State::log || grid.grid[frog.y + 1][frog.x] == State::coin)) {
            return to_string(frog.y) + " " + to_string(frog.x) + " D";
        }


        if (frog.y - 1 >= 1 && frog.x < bornl && this->rowDir[frog.y - 1] == FlowState::right
            && (grid.grid[frog.y - 1][frog.x] == State::log || grid.grid[frog.y - 1][frog.x] == State::coin)) {
            return to_string(frog.y) + " " + to_string(frog.x) + " U";
        }
        else if (frog.y - 1 >= 1 && (frog.x > bornr) && this->rowDir[frog.y - 1] == FlowState::left
            && (grid.grid[frog.y - 1][frog.x] == State::log || grid.grid[frog.y - 1][frog.x] == State::coin)) {
            return to_string(frog.y) + " " + to_string(frog.x) + " U";
        }
       
        

        vector<Coord> indice;
        vector<char> vdir;

        int dirblock = 1;

        /*if (root.turn > 900 && root.nb_coins > this->MAX_COIN / 2) {
            dirblock = 3;
        }
        else*/ if (frog.y > (root.H * 3 / 5) && root.nb_coins < this->MAX_COIN)dirblock = 2;
        else if (root.nb_coins > this->MAX_COIN)dirblock = 3;

        if (dirblock == 1) {
            GetPossibleMoveSim(root, indice, vdir, frog_curr, 1);
            if (indice.size() == 0) {
                if (this->Get_up(root, frog_curr)) {
                    indice.push_back({ frog_curr.x, frog_curr.y - 1 });
                    vdir.push_back('U');
                }
            }
        }
        else if (dirblock == 2) {
            if (frog.x <= root.W / 5) {
                GetPossibleMoveSim(root, indice, vdir, frog_curr, 3);
                if (indice.size() == 0) {
                    if (this->Get_down(root, frog_curr)) {
                        indice.push_back({ frog_curr.x , frog_curr.y+1 });
                        vdir.push_back('D');
                    }
                }
            }
            else if (frog.x >= root.W * 4 / 5) {
                GetPossibleMoveSim(root, indice, vdir, frog_curr, 3);
                if (indice.size() == 0) {
                    if (this->Get_down(root, frog_curr)) {
                        indice.push_back({ frog_curr.x, frog_curr.y+1 });
                        vdir.push_back('D');
                    }
                }
            }
            else {
               
                //int t[2] = { 1,3 };
                //GetPossibleMoveSim(root, indice, vdir, frog_curr, t[rand() % 2]);
                GetPossibleMove(root, indice, vdir, frog_curr);
            }
        }
        else if (dirblock == 3) {
            GetPossibleMoveSim(root, indice, vdir, frog_curr, 3);
            if (indice.size() == 0) {
                if (this->Get_down(root, frog_curr)) {
                    indice.push_back({ frog_curr.x, frog_curr.y + 1 });
                    vdir.push_back('D');
                }
            }
        }

            
        int indsel = -1;
        int maxscore = numeric_limits<int>::min();
        for (int i = 0; i < indice.size(); ++i) {
            Score score = { 0, 0, 'A' };
            this->GetScore(root, i, score, indice, vdir, frog_curr);
            ll safety = 500;
            if (frog_curr.x < root.W / 5 || frog_curr.x > grid.W * 4 / 5)
                safety = this->GetSafety(root, indice[i], frog);
            if (safety < 500)score.score -= safety / 10;


            cerr << "safety " << safety << " " << score.score << endl;
            if ( score.score > maxscore) {
                maxscore = score.score;
                indsel = i;
                dir = vdir[indsel];
            }

        }

        if (indsel != -1) {
            frog_curr = { indice[indsel].x, indice[indsel].y };
            move = to_string(frog.y) + " " + to_string(frog.x) + " " + dir;
        }
            


        


     
        return move;
      


    }

    Score SimulationE2(Grid grid, Coord frog, int depth, int max_depth, Score score, vector<vector<bool>> vis, int direction, float time, std::chrono::steady_clock::time_point sttime) {

        if (max_depth != -1 && depth >= max_depth) {
            //score.score /= depth;
            return score;
        }
        if (frog.x < grid.W / 5 || frog.x >grid.W * 4 / 5) {
            //score.score /= depth;
            return score;
        }

       
      
        auto now = high_resolution_clock::now();
        auto d = duration_cast<microseconds>(now - sttime);
        if (d.count() > time) {
            return score;
        }
       
       
       

        cerr << "Simulation E start "  << depth<< endl;

        Coord frog_curr = { frog.x, frog.y };

        Score score_curr = { score.nb_coins, score.score, score.dir };

        //cerr << "score_start " << score_curr.score << endl;

        //cerr << "frog " << frog.x << " " << frog_curr.y <<  endl;

        /*for (int i = 0; i < grid.H; ++i) {
            for (int j = 0; j < grid.W; ++j) {
                cerr << grid.grid[i][j];
            }cerr << endl;
        }cerr << endl;*/


        vis[frog_curr.y][frog_curr.x] = true;

        /*for (int i = 0; i < grid.H; ++i) {
            for (int j = 0; j < grid.W; ++j) {
                cerr << this->vise[i][j];
            }cerr << endl;
        }*/

        //cerr <<"---"<< grid.score << endl;

        //ll score = grid.score;

        ll max_score = 0;

        //max_score = max(max_score, score);


        vector<Coord> indice;
        vector<char> vdir;

        int dirblock = 1;

        if (direction == 1) {
            //if (grid.nb_coins > this->MAX_COIN)dirblock = 3;
            GetPossibleMoveSim(grid, indice, vdir, frog_curr, direction);
            //GetPossibleMove(grid, indice, vdir, frog_curr);

            if (indice.size() == 0) {
                if (this->Get_up(grid, frog_curr)) {
                    indice.push_back({ frog_curr.x, frog_curr.y - 1 });
                    vdir.push_back('U');
                }
            }

        }
        else if (direction == 2) {
            GetPossibleMoveSim2(grid, indice, vdir, frog_curr, direction);
            /*if (indice.size() == 0) {
                if (this->Get_right(grid, frog_curr)) {
                    indice.push_back({ frog_curr.x + 1, frog_curr.y });
                    vdir.push_back('R');
                }
            }*/
        }
        else if (direction == 4) {
            GetPossibleMoveSim2(grid, indice, vdir, frog_curr, direction);
            /*if (indice.size() == 0) {
                if (this->Get_left(grid, frog_curr)) {
                    indice.push_back({ frog_curr.x - 1, frog_curr.y });
                    vdir.push_back('L');
                }
            }*/
        }
        else if (direction == 3) {
            GetPossibleMoveSim(grid, indice, vdir, frog_curr, direction);
            if (indice.size() == 0) {
                if (this->Get_down(grid, frog_curr)) {
                    indice.push_back({ frog_curr.x, frog_curr.y + 1 });
                    vdir.push_back('D');
                }
            }
        }

        /* }
        else if (frog_curr.x < (grid.W / 2.66)) {
            dirblock = 4;
            GetPossibleMoveSim2(grid, indice, vdir, frog_curr, 4);
        }
        else if (frog_curr.x > grid.W - (grid.W / 2.66)) {
            dirblock = 2;
            GetPossibleMoveSim2(grid, indice, vdir, frog_curr, 2);
        }*/

        vector<vector<bool>> visb(grid.H, vector<bool>(grid.W, false));
        /*for (int y = 0; y < grid.H; ++y) {
            for (int x = 0; x < grid.W; ++x) {
                visb[y][x] = vis[y][x];
            }
        }*/

        for (int i = 0; i < indice.size(); ++i) {
            if (!vis[indice[i].y][indice[i].x]) {

               
                this->GetScore(grid, i, score, indice, vdir, frog_curr);
                ll safety = this->GetSafety(grid, indice[i], frog);
                if (safety < 500) {
                    if (score_curr.score > 0)
                        score_curr.score /= 2;
                    else
                        score_curr.score *= 2;
                }
                else {
                    score_curr.score += 1;
                }

             
                grid.grid[frog.y][frog.x] = State::log;
                grid.grid[indice[i].y][indice[i].x] = State::frog;

                //vector<vector<bool>> visb2(grid.H, vector<bool>(grid.W, false));
                for (int y = 0; y < grid.H; ++y) {
                    for (int x = 0; x < grid.W; ++x) {
                        visb[y][x] = vis[y][x];
                    }
                }

                for (int k = 1; k < rowDir.size(); ++k) {


                    vector<char> new_state;
                    if (rowDir[k] == FlowState::right) {

                        for (int j = grid.W - 1; j > 0; --j) {
                            grid.grid[k][j] = grid.grid[k][j - 1];
                            visb[k][j] = visb[k][j - 1];
                        }
                        grid.grid[k][0] = State::water;
                        visb[k][0] = false;

                    }
                    else {
                        for (int j = 0; j < grid.W - 1; ++j) {
                            grid.grid[k][j] = grid.grid[k][j + 1];
                            visb[k][j] = visb[k][j + 1];
                        }
                        grid.grid[k][grid.W - 1] = State::water;
                        visb[k][grid.W - 1] = false;
                    }

                }

                frog_curr = { indice[i].x ,indice[i].y };
                if (rowDir[indice[i].y] == FlowState::right) {
                    frog_curr.x++;
                }
                else {
                    frog_curr.x--;
                }

                //if(depth > 0)
                 //   score.score /= depth;

                if (frog_curr.x >= 0 && frog_curr.x < grid.W) {
                    //cerr << "------------bef" << score_curr.score << endl;
                    //max_score = max(max_score, SimulationE(grid, frog_curr, depth+1, max_depth, score));
                    Score sc = SimulationE2(grid, frog_curr, depth + 1, max_depth, score_curr, visb, direction, 8000, sttime);
                    
                    score_curr.score += sc.score;
                    //cerr << "------------sc" << score_curr.score << " " << sc.score << endl;
                }
                else {
                    score_curr.score -= 1 * grid.nb_coins;
                }

                //max_score = max(max_score, score);
                //cerr << "------------" << score_curr.score << endl;

            }

        }

        //cerr << "------------fi" << score_curr.score << endl;

        return score_curr;

    }

    string Simulate2(Grid grid, Coord frog, int depth, float time, Score& score) {

        auto start = high_resolution_clock::now();
        auto keepSolving = [&start, time]() -> bool
        {
            auto now = high_resolution_clock::now();
            auto d = duration_cast<microseconds>(now - start);
            return (d.count() < time);
        };

        std::chrono::steady_clock::time_point sttime = high_resolution_clock::now();
        
        //cerr << "Simulate" << endl;

        Grid root;
        root.W = grid.W;
        root.H = grid.H;
        root.KL = grid.KL;
        root.KW = grid.KW;
        root.PC = grid.PC;
        root.nb_coins = grid.nb_coins;

        vector<vector<char>>g(grid.H, vector<char>(grid.W));
        root.Init_grid(g);
        //grid.Copy_grid(root.grid);

        Coord frog_curr = { frog.x, frog.y };

        char dir = 'X';
        vector<Coord> indice;
        vector<char> vdir;

        if (frog.x < grid.W / 3 && this->rowDir[frog.y] == FlowState::right) {
            return "";
        }
        else if (frog.x > (grid.W - (grid.W / 3)) && this->rowDir[frog.y] == FlowState::left) {
            return "";
        }

        bool special_right = false;
        if (this->rowDir[frog.y] == FlowState::right) {
            if ((frog.y - 1 >= 0 && this->rowDir[frog.y-1] == FlowState::right)||
                (frog.y + 1 < grid.W && this->rowDir[frog.y + 1] == FlowState::right) ){
                //if (frog.y + 1 < grid.W && this->rowDir[frog.y+1] == FlowState::right) {

                    special_right = true;

                //}
                
            }
        }

        bool special_left = false;
        if (this->rowDir[frog.y] == FlowState::left) {
            if ((frog.y - 1 >= 0 && this->rowDir[frog.y - 1] == FlowState::left) ||
               (frog.y + 1 < grid.W && this->rowDir[frog.y + 1] == FlowState::left) ){
                //if (frog.y + 1 < grid.W && this->rowDir[frog.y + 1] == FlowState::left) {

                    special_left = true;

                //}

            }
        }
        
        


        
        int dirblock = 1;


        /*if (frog.x < (grid.W / 5) && special_right) {
            return "";
        }
        else if (frog.x > (grid.W - (grid.W / 5)) && special_left) {
            return "";
        }*/
        
        if (frog.y > (grid.H * 3/5)  && grid.nb_coins < this->MAX_COIN)dirblock = 2;
        else if (grid.nb_coins > this->MAX_COIN)dirblock = 3;

        //if (frog_curr.x >= (grid.W / 2.66) && frog_curr.x <= grid.W - (grid.W / 2.66)) {
        //if (grid.nb_coins > this->MAX_COIN)dirblock = 3;

        if (dirblock == 1) {
            GetPossibleMoveSim(grid, indice, vdir, frog_curr, 1);
            if (indice.size() == 0) {
                if (this->Get_up(grid, frog_curr)) {
                    indice.push_back({ frog_curr.x, frog_curr.y - 1 });
                    vdir.push_back('U');
                }
            }
        }
        else if (dirblock == 2) {
            if (frog.x <= grid.W / 5) {
                GetPossibleMoveSim2(grid, indice, vdir, frog_curr, 4);
                if (indice.size() == 0) {
                    /*if (this->Get_left(grid, frog_curr)) {
                        indice.push_back({ frog_curr.x - 1, frog_curr.y });
                        vdir.push_back('L');
                    }*/
                }
            }
            else if (frog.x >= grid.W * 4/5) {
                GetPossibleMoveSim2(grid, indice, vdir, frog_curr, 2);
                if (indice.size() == 0) {
                    /*if (this->Get_right(grid, frog_curr)) {
                        indice.push_back({ frog_curr.x + 1, frog_curr.y });
                        vdir.push_back('R');
                    }*/
                }
            }
            else {
                //if(frog.y <= grid.W * 2 / 3)
                int t[2] = { 1,3 };
                    GetPossibleMoveSim(grid, indice, vdir, frog_curr, t[rand()%2]);
                //else if (frog.y >= grid.W * 7/8)
                    //GetPossibleMove(grid, indice, vdir, frog_curr);
            }
        }
        else if (dirblock == 3) {
            GetPossibleMoveSim(grid, indice, vdir, frog_curr, 3);
            if (indice.size() == 0) {
                if (this->Get_down(grid, frog_curr)) {
                    indice.push_back({ frog_curr.x, frog_curr.y + 1 });
                    vdir.push_back('D');
                }
            }
        }

        

         //cerr << "end poss move" << endl;
        bool out = false;
        ll max_score = numeric_limits<ll>::min();
        int max_coins = 0;
        Coord lcoord = { frog.x, frog.y };
        for (int s = 0; s < indice.size(); ++s) {

            grid.Copy_grid(root.grid);

            if (frog.y == 0)
                root.grid[frog.y][frog.x] = State::beach;
            else
                root.grid[frog.y][frog.x] = State::log;





            /*for (int i = 0; i < root.H; ++i) {
                for (int j = 0; j < root.W; ++j) {
                    cerr << root.grid[i][j];
                }cerr << endl;
            }cerr << endl;*/

            root.score = grid.score;
           
            this->GetScore(root, s, score, indice, vdir, frog);
            ll safety = this->GetSafety(root, indice[s], frog);
            if (safety < 500) {
                if (score.score > 0)
                    score.score /= 2;
                else
                    score.score *= 2;
            }
            else {
                score.score += 10;
            }

            root.grid[indice[s].y][indice[s].x] = State::frog;

            for (int k = 1; k < rowDir.size(); ++k) {


                vector<char> new_state;
                if (rowDir[k] == FlowState::right) {

                    for (int j = grid.W - 1; j > 0; --j) {
                        root.grid[k][j] = root.grid[k][j - 1];
                       
                    }
                    root.grid[k][0] = State::water;

                }
                else {
                    for (int j = 0; j < grid.W - 1; ++j) {
                        root.grid[k][j] = root.grid[k][j + 1];
                        
                    }
                    root.grid[k][grid.W - 1] = State::water;

                }

            }

            frog_curr = { indice[s].x ,indice[s].y };
            if (rowDir[indice[s].y] == FlowState::right) {
                frog_curr.x++;
            }
            else {
                frog_curr.x--;
            }

            lcoord = { frog_curr.x, frog_curr.y };
            out = false;
            //this->Init_visE(root.H, root.W);
            //for (int ind = 0; ind < indice.size(); ++ind)this->vise[indice[ind].y][indice[ind].y] = true;
            vector<vector<bool>>vis(grid.H, vector<bool>(grid.W, false));
            
            if (frog_curr.x >= 0 && frog_curr.x < grid.W ) {
                vis[frog_curr.y][frog_curr.x] = true;

                Score sc;
                if (dirblock == 1) {

                    sc = this->SimulationE2(root, { indice[s].x, indice[s].y }, 0, depth, score, vis, 1, time, sttime );
                }
                else if (dirblock == 3) {

                    sc = this->SimulationE2(root, { indice[s].x, indice[s].y }, 0, depth, score, vis, 3, time, sttime);
                }
                else if(dirblock == 2) {


                    if (vdir[s] == 'U')
                        sc = this->SimulationE2(root, { indice[s].x, indice[s].y }, 0, depth, score, vis, 3, time, sttime);
                    else if (vdir[s] == 'D')
                        sc = this->SimulationE2(root, { indice[s].x, indice[s].y }, 0, depth, score, vis, 1, time, sttime);
                    else if (vdir[s] == 'L') {

                        if(indice[s].x < grid.W / 3 )
                            sc = this->SimulationE2(root, { indice[s].x, indice[s].y }, 0, depth, score, vis, 4, time, sttime);
                        else if (indice[s].x > grid.W / 3 )
                            sc = this->SimulationE2(root, { indice[s].x, indice[s].y }, 0, depth, score, vis, 2, time, sttime);
                        else
                            sc = this->SimulationE2(root, { indice[s].x, indice[s].y }, 0, depth, score, vis, 3, time, sttime);
                    }
                    else if (vdir[s] == 'R') {
                        if (indice[s].x > (grid.W - (grid.W / 3)))
                            sc = this->SimulationE2(root, { indice[s].x, indice[s].y }, 0, depth, score, vis, 2, time, sttime);
                        else if(indice[s].x < grid.W / 3 )
                            sc = this->SimulationE2(root, { indice[s].x, indice[s].y }, 0, depth, score, vis, 4, time, sttime);
                        else
                            sc = this->SimulationE2(root, { indice[s].x, indice[s].y }, 0, depth, score, vis, 3, time, sttime);
                    }

                }

                cerr << "score " << sc.score << endl;
                //cerr << "----------********************************---------" << endl;
                if (sc.score > max_score) {
                    max_score = sc.score;
                    max_coins = sc.nb_coins;
                    score.score = sc.score;
                    score.nb_coins = sc.nb_coins;
                    dir = vdir[s];
                }
            }
            else {
                out = true;
            }
        }


        string move;

        if (max_score == numeric_limits<ll>::min()) {
            move = "";
        }
        else {
            cerr << "frog " << to_string(frog.y) << " " << to_string(frog.x) << endl;
            move = to_string(frog.y) + " " + to_string(frog.x) + " " + dir;
        }
        //cerr << move << endl;

        return move;

    }

    Score SimulationE(Grid grid, Coord frog, int depth, int max_depth, Score score, vector<vector<bool>> &vis) {

        if (max_depth != -1 && depth >= max_depth)return score;

        //cerr << "Simulation E start "  << depth<< endl;
       
        Coord frog_curr = { frog.x, frog.y };

        //cerr << "frog " << frog.x << " " << frog_curr.y <<  endl;

        /*for (int i = 0; i < grid.H; ++i) {
            for (int j = 0; j < grid.W; ++j) {
                cerr << grid.grid[i][j];
            }cerr << endl;
        }cerr << endl;*/


        vis[frog_curr.y][frog_curr.x] = true;

        /*for (int i = 0; i < grid.H; ++i) {
            for (int j = 0; j < grid.W; ++j) {
                cerr << this->vise[i][j];
            }cerr << endl;
        }*/

        //cerr <<"---"<< grid.score << endl;
                    
        //ll score = grid.score;

        ll max_score = 0;

        //max_score = max(max_score, score);

       
        vector<Coord> indice;
        vector<char> vdir;

        int dirblock = 1;
                  
            if (grid.nb_coins > this->MAX_COIN)dirblock = 3;
            GetPossibleMoveSim(grid, indice, vdir, frog_curr, dirblock);
            //GetPossibleMove(grid, indice, vdir, frog_curr);

            if (indice.size() == 0 && dirblock == 1) {
                if (this->Get_up(grid, frog_curr)) {
                    indice.push_back({ frog_curr.x, frog_curr.y - 1 });
                    vdir.push_back('U');
                }
            }
            if (indice.size() == 0 && dirblock == 3) {
                if (this->Get_down(grid, frog_curr)) {
                    indice.push_back({ frog_curr.x, frog_curr.y + 1 });
                    vdir.push_back('D');
                }
            }

        /* }
        else if (frog_curr.x < (grid.W / 2.66)) {
            dirblock = 4;
            GetPossibleMoveSim2(grid, indice, vdir, frog_curr, 4);
        }
        else if (frog_curr.x > grid.W - (grid.W / 2.66)) {
            dirblock = 2;
            GetPossibleMoveSim2(grid, indice, vdir, frog_curr, 2);
        }*/
               
        for (int i = 0; i < indice.size(); ++i) {
            if (!vis[indice[i].y][indice[i].x]) {
                                                
                if (grid.grid[indice[i].y][indice[i].x] == State::coin) {
                    grid.nb_coins+=indice[i].y;
                    score.nb_coins += indice[i].y;
                    score.score += 10000* grid.nb_coins;
                }
                else if (grid.grid[indice[i].y][indice[i].x] == State::log) {
                    score.score += 5000;
                }

                if (grid.nb_coins < this->MAX_COIN && indice[i].y > frog.y) {
                    score.score += 100000;
                }
                /*else if (grid.nb_coins < this->MAX_COIN && indice[i].y < frog.y) {
                    score.score += 100000;
                }*/
                if (grid.nb_coins >= this->MAX_COIN && indice[i].y < frog.y) {
                    score.score += 100000;
                }

                if (indice[i].x >= grid.W / 2 && this->rowDir[indice[i].y] == FlowState::right) {
                    score.score -= 100000 * indice[i].x * indice[i].x;
                }
                if (indice[i].x <= grid.W / 2 && this->rowDir[indice[i].y] == FlowState::left) {
                    score.score -= 100000 * (grid.W - indice[i].x) * (grid.W - indice[i].x);
                }

                if (indice[i].x <= grid.W / 2 && this->rowDir[indice[i].y] == FlowState::right) {
                    score.score += 1000000 * indice[i].x * indice[i].x;
                }
                if (indice[i].x >= grid.W/2 && this->rowDir[indice[i].y] == FlowState::left) {
                    score.score += 1000000 * (grid.W - indice[i].x) * (grid.W - indice[i].x);
                }
                
                
                //score.score += ((grid.W / 2) - abs(indice[i].x - (grid.W / 2))) / (grid.W / 2) * 1000;
                /*if (indice[i].x > (grid.W / 2.66) && indice[i].x < grid.W - (grid.W / 2.66)) {
                    score.score += 1000;
                }*/
                /*if (indice[i].x < (grid.W / 2.66)) {
                    score.score -= 5000;
                }

                if (indice[i].x > grid.W - (grid.W / 2.66)) {
                    score.score -= 5000;
                }*/
                                
                //cerr << "------------" << score.score << " " << grid.score << " " << vdir[i] << endl;

                grid.grid[frog.y][frog.x] = State::log;
                grid.grid[indice[i].y][indice[i].x] = State::frog;

                for (int k = 1; k < rowDir.size(); ++k) {
                                        

                    vector<char> new_state;
                    if (rowDir[k] == FlowState::right) {
                        
                        for (int j = grid.W - 1; j > 0; --j) {
                            grid.grid[k][j] = grid.grid[k][j - 1];
                        }
                        grid.grid[k][0] = State::water;

                    }
                    else {
                        for (int j = 0; j < grid.W - 1; ++j) {
                            grid.grid[k][j] = grid.grid[k][j + 1];
                        }
                        grid.grid[k][grid.W - 1] = State::water;

                    }
                    
                }

                frog_curr = { indice[i].x ,indice[i].y };
                if (rowDir[indice[i].y] == FlowState::right) {
                    frog_curr.x++;
                }
                else {
                    frog_curr.x--;
                }

                
                
                
                if (frog_curr.x >= 0 && frog_curr.x < grid.W) {
                    //max_score = max(max_score, SimulationE(grid, frog_curr, depth+1, max_depth, score));
                    Score sc = SimulationE(grid, frog_curr, depth + 1, max_depth, score, vis);
                    score.score += sc.score;
                }
                else {
                    score.score -= 100000*grid.nb_coins;
                }
                
                //max_score = max(max_score, score);
                //cerr << "------------" << score.score << endl;

            }

        }

        

        return score;

    }

    string Simulate(Grid grid, Coord frog, int depth, float time, Score &score) {

        auto start = high_resolution_clock::now();
        auto keepSolving = [&start, time]() -> bool
        {
            auto now = high_resolution_clock::now();
            auto d = duration_cast<microseconds>(now - start);
            return (d.count() < time);
        };

        //cerr << "Simulate" << endl;

        Grid root;
        root.W = grid.W;
        root.H = grid.H;
        root.KL = grid.KL;
        root.KW = grid.KW;
        root.PC = grid.PC;
        root.nb_coins = grid.nb_coins;
        
        vector<vector<char>>g(grid.H, vector<char>(grid.W));
        root.Init_grid(g);
        //grid.Copy_grid(root.grid);

        Coord frog_curr = { frog.x, frog.y };

        char dir = 'X';
        

        vector<Coord> indice;
        vector<char> vdir;
        int dirblock = 1;
        //if (frog_curr.x >= (grid.W / 2.66) && frog_curr.x <= grid.W - (grid.W / 2.66)) {
            if (grid.nb_coins > this->MAX_COIN)dirblock = 3;
            GetPossibleMoveSim(grid, indice, vdir, frog_curr, dirblock);
            //GetPossibleMove(grid, indice, vdir, frog_curr);

            if (indice.size() == 0 && dirblock == 1) {
                if (this->Get_up(grid, frog_curr)) {
                    indice.push_back({ frog_curr.x, frog_curr.y - 1 });
                    vdir.push_back('U');
                }
            }
            if (indice.size() == 0 && dirblock == 3) {
                if (this->Get_down(grid, frog_curr)) {
                    indice.push_back({ frog_curr.x, frog_curr.y + 1 });
                    vdir.push_back('D');
                }
            }

       /* }
        else if (frog_curr.x < (grid.W / 2.66)) {
            dirblock = 4;
            GetPossibleMoveSim2(grid, indice, vdir, frog_curr, 4);
        }
        else if (frog_curr.x > grid.W - (grid.W / 2.66)) {
            dirblock = 2;
            GetPossibleMoveSim2(grid, indice, vdir, frog_curr, 2);
        }*/


        

        //cerr << "end poss move" << endl;
        bool out = false;
        ll max_score = numeric_limits<ll>::min();
        int max_coins = 0;
        Coord lcoord = {frog.x, frog.y};
        for (int s = 0; s < indice.size(); ++s) {
            
            grid.Copy_grid(root.grid);

            if (frog.y == 0)
                root.grid[frog.y][frog.x] = State::beach;
            else
                root.grid[frog.y][frog.x] = State::log;
                        

            
            

            /*for (int i = 0; i < root.H; ++i) {
                for (int j = 0; j < root.W; ++j) {
                    cerr << root.grid[i][j];
                }cerr << endl;
            }cerr << endl;*/

            root.score = 0;


            if (root.grid[indice[s].y][indice[s].x] == State::coin) {
                root.nb_coins += indice[s].y;
                score.nb_coins += indice[s].y;
                score.score += 10000 * root.nb_coins;
            }
            else if (root.grid[indice[s].y][indice[s].x] == State::log) {
                score.score += 5000;
            }

            if (root.nb_coins < this->MAX_COIN && indice[s].y > frog.y) {
                score.score += 100000;
            }
            /*else if (grid.nb_coins < this->MAX_COIN && indice[i].y < frog.y) {
                score.score += 100000;
            }*/
            if (root.nb_coins >= this->MAX_COIN && indice[s].y < frog.y) {
                score.score += 100000;
            }

            if (indice[s].x >= root.W / 2 && this->rowDir[indice[s].y] == FlowState::right) {
                score.score -= 100000 * indice[s].x * indice[s].x;
            }
            if (indice[s].x <= root.W / 2 && this->rowDir[indice[s].y] == FlowState::left) {
                score.score -= 100000 * (root.W - indice[s].x) * (root.W - indice[s].x);
            }

            if (indice[s].x <= root.W / 2 && this->rowDir[indice[s].y] == FlowState::right) {
                score.score += 1000000 * indice[s].x * indice[s].x;
            }
            if (indice[s].x >= root.W / 2 && this->rowDir[indice[s].y] == FlowState::left) {
                score.score += 1000000 * (root.W - indice[s].x) * (root.W - indice[s].x);
            }

            root.grid[indice[s].y][indice[s].x] = State::frog;

            for (int k = 1; k < rowDir.size(); ++k) {


                vector<char> new_state;
                if (rowDir[k] == FlowState::right) {

                    for (int j = grid.W - 1; j > 0; --j) {
                        root.grid[k][j] = root.grid[k][j - 1];
                    }
                    root.grid[k][0] = State::water;

                }
                else {
                    for (int j = 0; j < grid.W - 1; ++j) {
                        root.grid[k][j] = root.grid[k][j + 1];
                    }
                    root.grid[k][grid.W - 1] = State::water;

                }

            }

            frog_curr = { indice[s].x ,indice[s].y };
            if (rowDir[indice[s].y] == FlowState::right) {
                frog_curr.x++;
            }
            else {
                frog_curr.x--;
            }

            lcoord = { frog_curr.x, frog_curr.y };
            out = false;
            //this->Init_visE(root.H, root.W);
            //for (int ind = 0; ind < indice.size(); ++ind)this->vise[indice[ind].y][indice[ind].y] = true;
            vector<vector<bool>>vis(grid.H, vector<bool>(grid.W, false));
            vis[indice[s].y][indice[s].x] = true;
            if (frog_curr.x >= 0 && frog_curr.x < grid.W) {
                Score sc = this->SimulationE(root, { indice[s].x, indice[s].y }, 0, depth, score, vis);
                cerr << "score " << sc.score << endl;
                //cerr << "----------********************************---------" << endl;
                if (sc.score > max_score) {
                    max_score = sc.score;
                    max_coins = sc.nb_coins;
                    score.score = sc.score;
                    score.nb_coins = sc.nb_coins;
                    dir = vdir[s];
                }
            }
            else {
                out = true;
            }
        }
              

        string move;

        if (max_score == numeric_limits<ll>::min()) {
            move = "";
        }
        else
            move = to_string(frog.y) + " " + to_string(frog.x) + " " + dir;

        //cerr << move << endl;

        return move;

    }

    

};


int main()
{
    srand(time(NULL));
  
    Grid grid;
    
    cin >> grid.H >> grid.W >> grid.F >> grid.KL >> grid.KW >> grid.PC;
   
    Coord frog;

    vector<char> rowDir(grid.H);
    for (int r = 1; r < grid.H; r++) cin >> rowDir[r];
    grid.Init_rowDir(rowDir);

    vector<vector<char>> gr(grid.H, vector<char>(grid.W));
    grid.Init_grid(gr);

    cerr << "Init grid" << endl;
    grid.nb_coins = 0;
    vector<ll>VNB_COINS(grid.F, 0);

    GameFrog *game = new GameFrog(grid.H, grid.W, rowDir);
    grid.down = game->DOWN;

    vector<CoordF>frogs_pos;
    vector<Coord2>frog_bis;
    vector<Coord2>frogs, nfrogs;
    vector<int>frog_lose;
    map<pair<int, int>,int> coins_frog, coin_frbis, temp;

    ll somme = 0;

    for (int turn = 1; turn <= 1000; turn++)
    {
        cerr << "turn " << turn << endl;

        frog_bis.swap(frogs);

        frogs = {};

        coins_frog.clear();

        int nb_frog = 0;
        int nb_flose = 0;
        for (int r = 0; r < grid.H; r++) {
            for (int c = 0; c < grid.W; c++) {
                cin >> grid.grid[r][c];
                if (grid.grid[r][c] == State::frog) {
                    frog.x = c;
                    frog.y = r;
                    frogs.push_back({ c, r, nb_frog });
                    nfrogs.push_back({ c, r, nb_frog });

                    if (r == 0)
                        coins_frog[{c, r}] = 0;
                    else {
                        coins_frog[{c, r}] = coin_frbis[{c, r}];
                    }
                        
                    nb_frog++;
                    cerr << nb_frog << " {x,y}={" <<c<<","<<r << "} = " << coins_frog[{c, r}] << endl;
                }
            }

        }

        coin_frbis.clear();
        temp.clear();

        grid.score = 0;
        grid.turn = turn;
              
        
        cerr << grid.score << endl;

        int elapsedTime;
        cin >> elapsedTime;

        vector<string> moves;

        //if (turn > 800)game->MAX_COIN = 50;

        if (turn > 1) {
            int t;
            t = turn % 100;
            if (t == 0) {
                game->DOWN = !game->DOWN;
                grid.down = game->DOWN;
            }
        }

        for (int fr = 0; fr < grid.F; ++fr) {

            
            string move;
            if (frogs[fr].y == 0 && frogs[fr].x < grid.W /4 
                && grid.grid[0][frogs[fr].x+1] != State::frog) {
                move = to_string(frogs[fr].y) + " " + to_string(frogs[fr].x) + " R";
                moves.push_back(move);
                
            }
            else if (frogs[fr].y == 0 && frogs[fr].x > grid.W * 3 / 4
                && grid.grid[0][frogs[fr].x-1] != State::frog) {
                move = to_string(frogs[fr].y) + " " + to_string(frogs[fr].x) + " L";
                moves.push_back(move);

            }
            else if (frogs[fr].y == 0 &&
                grid.grid[0][frogs[fr].x + 1] == State::frog&&
                frogs[fr].x < grid.W / 4) {
                //nothing
            }
            else if (frogs[fr].y == 0 &&
                    grid.grid[0][frogs[fr].x - 1] == State::frog &&
                    frogs[fr].x > grid.W * 3 / 4) {
                //nothing
            }
            else if (frogs[fr].y == 1  && coins_frog[{frogs[fr].x, frogs[fr].y}] > game->MAX_COIN /* && !game->DOWN*/&& grid.grid[0][frogs[fr].x] != State::frog) {
                moves.push_back(to_string(frogs[fr].y) + " " + to_string(frogs[fr].x) + " U");
                move = to_string(frogs[fr].y) + " " + to_string(frogs[fr].x) + " U";
                            
            }
            else {
                grid.nb_coins = coins_frog[{frogs[fr].x, frogs[fr].y}];
                Score score;
                score.score = 0;
                score.nb_coins = 0;
                //string m = game->Simulate2(grid, { frogs[fr].x, frogs[fr].y }, 1, 8000, score);
                string m = game->Simulate3(grid, { frogs[fr].x, frogs[fr].y }, 8000, score);
                move = m;
                if (!m.empty()) {
                    moves.push_back(m);
                    if (m[m.length() - 1] == 'U' && grid.grid[frogs[fr].y - 1][frogs[fr].x] == State::coin) {
                        VNB_COINS[fr] += frogs[fr].y - 1;
                        game->TOTAL_COIN += frogs[fr].y - 1;

                                                
                    }
                    if (m[m.length() - 1] == 'D' && grid.grid[frogs[fr].y + 1][frogs[fr].x] == State::coin) {
                        VNB_COINS[fr] += frogs[fr].y + 1;
                        game->TOTAL_COIN += frogs[fr].y + 1;

                        
                        
                    }
                    if (m[m.length() - 1] == 'L' && grid.grid[frogs[fr].y][frogs[fr].x - 1] == State::coin) {
                        VNB_COINS[fr] += frogs[fr].y;
                        game->TOTAL_COIN += frogs[fr].y;

                       
                        
                    }
                    if (m[m.length() - 1] == 'R' && grid.grid[frogs[fr].y][frogs[fr].x + 1] == State::coin) {
                        VNB_COINS[fr] += frogs[fr].y;
                        game->TOTAL_COIN += frogs[fr].y;

                        
                    }

                }

            }

            int X = frogs[fr].x, Y=frogs[fr].y;
            bool tmp = false;

            /*if (move.empty()) {

                if (frogs[fr].y == 0 ) {
                    if (grid.grid[1][frogs[fr].x] != State::frog && grid.grid[1][frogs[fr].x] != State::water) {
                        move = to_string(frogs[fr].y) + " " + to_string(frogs[fr].x) + " D";
                        moves.push_back(move);
                    }
                }
             
            }*/
            if (!move.empty()) {
                if (move[move.length() - 1] == 'U') {
                    /*int addc = coins_frog[{frogs[fr].x, frogs[fr].y}];
                    coins_frog[{frogs[fr].x, frogs[fr].y}] = 0;
                    coins_frog[{frogs[fr].x, frogs[fr].y - 1}] = addc;
                    if (grid.grid[frogs[fr].y - 1][frogs[fr].x] == State::coin)
                        coins_frog[{frogs[fr].x, frogs[fr].y - 1}] += frogs[fr].y - 1;*/

                    map<pair<int, int>, int>* m;
                    if (fr == 0) {
                        m = &coins_frog;
                    }
                    else {
                        m = &coin_frbis;
                    }

                    /*if (coin_frbis.count({frogs[fr].x, frogs[fr].y - 1}) > 0) {
                        temp[{frogs[fr].x, frogs[fr].y - 1}] = coins_frog[{frogs[fr].x, frogs[fr].y}];
                        if (grid.grid[frogs[fr].y - 1][frogs[fr].x] == State::coin)
                            temp[{frogs[fr].x, frogs[fr].y - 1}] += frogs[fr].y - 1;
                        tmp = true;
                    }
                    else {*/
                        cerr << "U " << coin_frbis[{frogs[fr].x, frogs[fr].y - 1}] << " " <<  coins_frog[{frogs[fr].x, frogs[fr].y}]<<endl;
                        coin_frbis[{frogs[fr].x, frogs[fr].y - 1}] = coins_frog[{frogs[fr].x, frogs[fr].y}];
                        if (grid.grid[frogs[fr].y - 1][frogs[fr].x] == State::coin)
                            coin_frbis[{frogs[fr].x, frogs[fr].y - 1}] += frogs[fr].y - 1;
                    //}

                    grid.grid[frogs[fr].y][frogs[fr].x] = State::log;
                    grid.grid[frogs[fr].y - 1][frogs[fr].x] = State::frog;

                    

                    nfrogs[fr].y--;
                    frogs[fr].y--;
                }
                if (move[move.length() - 1] == 'D') {
                    /*int addc = coins_frog[{frogs[fr].x, frogs[fr].y}];
                    cerr << "addc " << addc << endl;
                    coins_frog[{frogs[fr].x, frogs[fr].y}] = 0;
                    coins_frog[{frogs[fr].x, frogs[fr].y + 1}] = addc;
                    if (grid.grid[frogs[fr].y + 1][frogs[fr].x] == State::coin) {
                        coins_frog[{frogs[fr].x, frogs[fr].y + 1}] += frogs[fr].y + 1;
                        cerr << "down1 " << frogs[fr].y + 1 << endl;
                    }
                    cerr << "down " << coins_frog[{frogs[fr].x, frogs[fr].y + 1}] << endl;
                    cerr << game->TOTAL_COIN << " " << grid.grid[frogs[fr].y + 1][frogs[fr].x] << endl;
                    cerr << endl;*/

                    map<pair<int, int>, int>* m;
                    if (fr == 0) {
                        m = &coins_frog;
                    }
                    else {
                        m = &coin_frbis;
                    }

                    /*if (coin_frbis.count({frogs[fr].x, frogs[fr].y + 1}) > 0) {
                        temp[{frogs[fr].x, frogs[fr].y + 1}] = coins_frog[{frogs[fr].x, frogs[fr].y}];
                        if (grid.grid[frogs[fr].y + 1][frogs[fr].x] == State::coin)
                            temp[{frogs[fr].x, frogs[fr].y + 1}] += frogs[fr].y + 1;
                        tmp = true;
                    }
                    else {*/
                    cerr << "D " << coin_frbis[{frogs[fr].x, frogs[fr].y + 1}] << " " << coins_frog[{frogs[fr].x, frogs[fr].y}] << endl;
                        coin_frbis[{frogs[fr].x, frogs[fr].y + 1}] = coins_frog[{frogs[fr].x, frogs[fr].y}];
                        if (grid.grid[frogs[fr].y + 1][frogs[fr].x] == State::coin)
                            coin_frbis[{frogs[fr].x, frogs[fr].y + 1}] += frogs[fr].y + 1;
                    //}

                    if (frogs[fr].y == 0)
                        grid.grid[frogs[fr].y][frogs[fr].x] = State::beach;
                    else
                        grid.grid[frogs[fr].y][frogs[fr].x] = State::log;
                    grid.grid[frogs[fr].y + 1][frogs[fr].x] = State::frog;

                    nfrogs[fr].y++;
                    frogs[fr].y++;
                }
                if (move[move.length() - 1] == 'L') {
                    /*int addc = coins_frog[{frogs[fr].x, frogs[fr].y}];
                    coins_frog[{frogs[fr].x, frogs[fr].y}] = 0;
                    coins_frog[{frogs[fr].x - 1, frogs[fr].y }] = addc;
                    if (grid.grid[frogs[fr].y][frogs[fr].x - 1] == State::coin)
                        coins_frog[{frogs[fr].x - 1, frogs[fr].y }] += frogs[fr].y;*/

                    map<pair<int, int>, int>* m;
                    if (fr == 0) {
                        m = &coins_frog;
                    }
                    else {
                        m = &coin_frbis;
                    }

                    /*if (coin_frbis.count({frogs[fr].x - 1, frogs[fr].y}) > 0) {
                        temp[{frogs[fr].x - 1, frogs[fr].y}] = coins_frog[{frogs[fr].x, frogs[fr].y}];
                        if (grid.grid[frogs[fr].y][frogs[fr].x - 1] == State::coin)
                            temp[{frogs[fr].x - 1, frogs[fr].y}] += frogs[fr].y;
                        tmp = true;
                    }
                    else {*/
                    cerr << "L " << coin_frbis[{frogs[fr].x- 1, frogs[fr].y }] << " " << coins_frog[{frogs[fr].x, frogs[fr].y}] << endl;
                        coin_frbis[{frogs[fr].x - 1, frogs[fr].y}] = coins_frog[{frogs[fr].x, frogs[fr].y}];
                        if (grid.grid[frogs[fr].y][frogs[fr].x - 1] == State::coin)
                            coin_frbis[{frogs[fr].x - 1, frogs[fr].y}] += frogs[fr].y;
                    //}

                    grid.grid[frogs[fr].y][frogs[fr].x] = State::log;
                    grid.grid[frogs[fr].y][frogs[fr].x-1] = State::frog;

                    
                    nfrogs[fr].x--;
                    frogs[fr].x--;
                }
                if (move[move.length() - 1] == 'R') {
                    /*int addc = coins_frog[{frogs[fr].x, frogs[fr].y}];
                    coins_frog[{frogs[fr].x, frogs[fr].y}] = 0;
                    coins_frog[{frogs[fr].x + 1, frogs[fr].y }] = addc;
                    if (grid.grid[frogs[fr].y][frogs[fr].x + 1] == State::coin)
                        coins_frog[{frogs[fr].x + 1, frogs[fr].y }] += frogs[fr].y;*/

                    map<pair<int, int>, int>* m;
                    if (fr == 0) {
                        m = &coins_frog;
                    }
                    else {
                        m = &coin_frbis;
                    }

                    /*if (coin_frbis.count({frogs[fr].x + 1, frogs[fr].y}) > 0) {
                        temp[{frogs[fr].x + 1, frogs[fr].y}] = coins_frog[{frogs[fr].x, frogs[fr].y}];
                        if (grid.grid[frogs[fr].y][frogs[fr].x + 1] == State::coin)
                            temp[{frogs[fr].x + 1, frogs[fr].y}] += frogs[fr].y;
                        tmp = true;
                    }
                    else {*/
                    cerr << "R " << coin_frbis[{frogs[fr].x+1, frogs[fr].y }] << " " << coins_frog[{frogs[fr].x, frogs[fr].y}] << endl;
                        coin_frbis[{frogs[fr].x + 1, frogs[fr].y}] = coins_frog[{frogs[fr].x, frogs[fr].y}];
                        if (grid.grid[frogs[fr].y][frogs[fr].x + 1] == State::coin)
                            coin_frbis[{frogs[fr].x + 1, frogs[fr].y}] += frogs[fr].y;
                    //}

                    grid.grid[frogs[fr].y][frogs[fr].x] = State::log;
                    grid.grid[frogs[fr].y][frogs[fr].x+1] = State::frog;

                    nfrogs[fr].x++;
                    frogs[fr].x++;

                    
                }
                              

            }

            if (!move.empty()) {


                if (grid.rowDir[frogs[fr].y] == FlowState::right) {
                    cerr << "> " << coin_frbis[{frogs[fr].x + 1, frogs[fr].y}] << " " << coin_frbis[{frogs[fr].x, frogs[fr].y}] << endl;
                    if (coin_frbis.count({ frogs[fr].x + 1, frogs[fr].y }) > 0) {
                        temp[{ frogs[fr].x + 1, frogs[fr].y }] = coin_frbis[{frogs[fr].x, frogs[fr].y}];
                    }
                    else {
                        coin_frbis[{frogs[fr].x + 1, frogs[fr].y}] = coin_frbis[{frogs[fr].x, frogs[fr].y}];
                        coin_frbis.erase({ frogs[fr].x, frogs[fr].y });
                    }
                }
                else if (grid.rowDir[frogs[fr].y] == FlowState::left) {
                    cerr << frogs[fr].x - 1 << " " << frogs[fr].y << ", " << frogs[fr].x << " " << frogs[fr].y << endl;
                    cerr << "< " << coin_frbis[{frogs[fr].x - 1, frogs[fr].y}] << " " << coin_frbis[{frogs[fr].x, frogs[fr].y}] << endl;

                    if (coin_frbis.count({ frogs[fr].x - 1, frogs[fr].y }) > 0) {
                        temp[{ frogs[fr].x - 1, frogs[fr].y }] = coin_frbis[{frogs[fr].x, frogs[fr].y}];
                    }
                    else {
                        coin_frbis[{frogs[fr].x - 1, frogs[fr].y}] = coin_frbis[{frogs[fr].x, frogs[fr].y}];
                        coin_frbis.erase({ frogs[fr].x, frogs[fr].y });
                    }
                }
            }
            else {
                if (grid.rowDir[frogs[fr].y] == FlowState::right) {
                    cerr << "> " << coin_frbis[{frogs[fr].x + 1, frogs[fr].y}] << " " << coin_frbis[{frogs[fr].x, frogs[fr].y}] << endl;
                    
                    if (coin_frbis.count({ frogs[fr].x + 1, frogs[fr].y }) > 0) {
                        temp[{ frogs[fr].x + 1, frogs[fr].y }] = coins_frog[{frogs[fr].x, frogs[fr].y}];
                    }
                    else {
                        coin_frbis[{frogs[fr].x + 1, frogs[fr].y}] = coins_frog[{frogs[fr].x, frogs[fr].y}];
                        //coin_frbis.erase({ frogs[fr].x, frogs[fr].y });
                    }

                }
                else if (grid.rowDir[frogs[fr].y] == FlowState::left) {
                    cerr << frogs[fr].x - 1 << " " << frogs[fr].y << ", " << frogs[fr].x << " " << frogs[fr].y << endl;
                    cerr << "< " << coin_frbis[{frogs[fr].x - 1, frogs[fr].y}] << " " << coins_frog[{frogs[fr].x, frogs[fr].y}] << endl;

                    if (coin_frbis.count({ frogs[fr].x - 1, frogs[fr].y }) > 0) {
                        temp[{ frogs[fr].x - 1, frogs[fr].y }] = coins_frog[{frogs[fr].x, frogs[fr].y}];
                    }
                    else {
                        coin_frbis[{frogs[fr].x - 1, frogs[fr].y}] = coins_frog[{frogs[fr].x, frogs[fr].y}];
                        //coin_frbis.erase({ frogs[fr].x, frogs[fr].y });
                    }

                }


            }
                   

         
        }

        for (auto t : temp) {
            if(grid.rowDir[t.first.second] == FlowState::right)
                coin_frbis[{t.first.first, t.first.second}] = t.second;
            else
                coin_frbis[{t.first.first, t.first.second}] = t.second;
        }

       

        /*for (int y = 0; y < grid.H; ++y) {
            for (int x = 0; x < grid.W; ++x) {
                cerr << coin_frbis[{x, y}] << " ";
            }cerr << endl;
        }*/


        /*

        
        for (int r = 0; r <= 1; r++)
        {
            for (int c = 0; c < grid.W; c++)
            {
                //collect coin
                if (r == 0 && grid.grid[r][c] == '@' && grid.grid[r + 1][c] == 'o')
                    moves.push_back(to_string(r) + " " + to_string(c) + " D");
                else if(r == 0 && grid.grid[r][c] == '@' && c > 0)
                    moves.push_back(to_string(r) + " " + to_string(c) + " L");
                else if (r == 0 && grid.grid[r][c] == '@' )
                    moves.push_back(to_string(r) + " " + to_string(c) + " R");
                //return coin
                else if (r == 1 && grid.grid[r][c] == '@' && grid.grid[r - 1][c] == '#')
                    moves.push_back(to_string(r) + " " + to_string(c) + " U");

            }
        }
        */


        cout << moves.size() << endl;
        for (string& mov : moves) {
            cerr << "move " << mov  << endl;
            cout << mov << endl;
        }
        cout.flush();

    }
    
}
