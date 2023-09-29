// Frogs.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
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
using namespace std::chrono;
using namespace std;
//#define DEBUG
//#define DEBUG2
#define DEDGE 2

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

class LargentiereTreeSearch {
public:
    vector<Node*> garbage;
    Node* root;
    int nb_coins;
    char dirf;
    double max_coins;
    LargentiereTreeSearch(double coins, double H, double W, double PC) {
        root = new Node();
        this->garbage.push_back(root);
        this->nb_coins = 0;
        double ratio = .5;
        if (H >= 25 && W >= 25)ratio = 1.0;
        this->max_coins = ((H * coins / 30.0) * W / 30.0) *ratio;
    }

    void GetPossibleMove2(Grid grid, vector<Coord>& indice, Coord frog) {

        int m = rand() % 4 + 1;
        //cerr << "MMMMMMMMMMMMMMMMMMMMMMMM" << m << endl;
        if (m == 1)
            indice.push_back({ frog.x, frog.y-1 });
        else if (m == 2)
            indice.push_back({ frog.x+1, frog.y });
        else if (m == 3)
            indice.push_back({ frog.x, frog.y+1 });
        else if (m == 4)
            indice.push_back({ frog.x-1, frog.y });

    }

    void GetPossibleMoveSim(Grid grid, vector<Coord>& indice, vector<char>& vdir, Coord frog, int dirblock) {
#ifdef DEBUG
        cerr << "poss move sim " << frog.x << " " << frog.y << endl;
#endif
       

        int POS_REPLACE;
        if (grid.W < 15)
            POS_REPLACE = 2;
        else
            POS_REPLACE = 4;

        if (dirblock != 1 && frog.y - 1 >= 0) {
            if (grid.grid[frog.y - 1][frog.x] == State::log || grid.grid[frog.y - 1][frog.x] == State::coin) {
                indice.push_back({ frog.x, frog.y - 1 });
                vdir.push_back('U');
            }

            if (indice.size() == 0) {
                if (/*frog.x > POS_REPLACE &&*/ frog.x - 1 >= 0) {
                    if (grid.grid[frog.y][frog.x - 1] == State::log || grid.grid[frog.y][frog.x - 1] == State::coin) {
                        indice.push_back({ frog.x - 1, frog.y });
                        vdir.push_back('L');
                    }

                }
                if (/*frog.x < (grid.W - POS_REPLACE) && */ frog.x + 1 < grid.W) {
                    if (grid.grid[frog.y][frog.x + 1] == State::log || grid.grid[frog.y][frog.x + 1] == State::coin) {
                        indice.push_back({ frog.x + 1, frog.y });
                        vdir.push_back('R');
                    }

                }

            }

        }
        if (dirblock != 3 && frog.y + 1 < grid.H) {
            if (grid.grid[frog.y + 1][frog.x] == State::log || grid.grid[frog.y + 1][frog.x] == State::coin) {
                indice.push_back({ frog.x, frog.y + 1 });
                vdir.push_back('D');
            }

            if (indice.size() == 0) {
                if (/*frog.x > POS_REPLACE &&*/ frog.x - 1 >= 0) {
                    if (grid.grid[frog.y][frog.x - 1] == State::log || grid.grid[frog.y][frog.x - 1] == State::coin) {
                        indice.push_back({ frog.x - 1, frog.y });
                        vdir.push_back('L');
                    }


                    if (/*frog.x < (grid.W - POS_REPLACE) && */ frog.x + 1 < grid.W) {
                        if (grid.grid[frog.y][frog.x + 1] == State::log || grid.grid[frog.y][frog.x + 1] == State::coin) {
                            indice.push_back({ frog.x + 1, frog.y });
                            vdir.push_back('R');
                        }

                    }
                }

            }

        }


#ifdef DEBUG
        cerr << "end poss move" << endl;
#endif
    }

    void GetPossibleMove(Grid grid, vector<Coord>& indice, Coord frog, int dirblock) {
#ifdef DEBUG
        cerr << "poss move "<< frog.x << " " << frog.y << endl;
#endif
       /* if (frog.y == 0) {
            cerr << "Poss move y = 0" << endl;
            if (grid.grid[1][frog.x] == State::log || grid.grid[1][frog.x] == State::coin) {
                cerr << "coin or log row 1" << endl;
                indice.push_back({ frog.x, 1 });
            }
            
            if (frog.x <= grid.W/2) {
                cerr << "go right" << endl;
                indice.push_back({ frog.x+1, 0 });

            }
            else if (frog.x > grid.W / 2) {
                cerr << "go left" << endl;
                indice.push_back({ frog.x-1, 0 });

            }
            

        
            return;
        }*/

        /*if (dirblock != 4 && frog.x - 1 >= 0) {
            if (grid.grid[frog.y][frog.x - 1] == State::log || grid.grid[frog.y][frog.x - 1] == State::coin) {
                indice.push_back({ frog.x - 1, frog.y });
            }

        }
        if (dirblock != 2 && frog.x + 1 < grid.W) {
            if (grid.grid[frog.y][frog.x + 1] == State::log || grid.grid[frog.y][frog.x + 1] == State::coin) {
                indice.push_back({ frog.x + 1, frog.y });
            }

        }*/
        
        int POS_REPLACE;
        if (grid.W < 15)
            POS_REPLACE = 2;
        else
            POS_REPLACE = 4;

        if (dirblock != 1 && frog.y - 1 >= 0) {
            if (grid.grid[frog.y - 1][frog.x] == State::log || grid.grid[frog.y - 1][frog.x] == State::coin) {
                indice.push_back({ frog.x, frog.y - 1 });
            }

            if (indice.size() == 0) {
                if (/*frog.x > POS_REPLACE &&*/ frog.x - 1 >= 0) {
                    if (grid.grid[frog.y][frog.x - 1] == State::log || grid.grid[frog.y][frog.x - 1] == State::coin) {
                        indice.push_back({ frog.x - 1, frog.y });
                    }

                }
                if (/*frog.x < (grid.W - POS_REPLACE) && */ frog.x + 1 < grid.W) {
                    if (grid.grid[frog.y][frog.x + 1] == State::log || grid.grid[frog.y][frog.x + 1] == State::coin) {
                        indice.push_back({ frog.x + 1, frog.y });
                    }

                }

            }

        }
        if (dirblock != 3 && frog.y + 1 < grid.H) {
            if (grid.grid[frog.y + 1][frog.x] == State::log || grid.grid[frog.y + 1][frog.x] == State::coin) {
                indice.push_back({ frog.x, frog.y + 1 });
            }

            if (indice.size() == 0) {
                if (/*frog.x > POS_REPLACE &&*/ frog.x - 1 >= 0) {
                    if (grid.grid[frog.y][frog.x - 1] == State::log || grid.grid[frog.y][frog.x - 1] == State::coin) {
                        indice.push_back({ frog.x - 1, frog.y });
                    }


                    if (/*frog.x < (grid.W - POS_REPLACE) && */ frog.x + 1 < grid.W) {
                        if (grid.grid[frog.y][frog.x + 1] == State::log || grid.grid[frog.y][frog.x + 1] == State::coin) {
                            indice.push_back({ frog.x + 1, frog.y });
                        }

                    }
                }

            }

        }

        
#ifdef DEBUG
        cerr << "end poss move" << endl;
#endif
    }

    void GetPossibleMove2(Grid grid, vector<Coord>& indice, Coord frog, int dirblock) {
#ifdef DEBUG
        cerr << "poss move " << frog.x << " " << frog.y << endl;
#endif
        /* if (frog.y == 0) {
             cerr << "Poss move y = 0" << endl;
             if (grid.grid[1][frog.x] == State::log || grid.grid[1][frog.x] == State::coin) {
                 cerr << "coin or log row 1" << endl;
                 indice.push_back({ frog.x, 1 });
             }

             if (frog.x <= grid.W/2) {
                 cerr << "go right" << endl;
                 indice.push_back({ frog.x+1, 0 });

             }
             else if (frog.x > grid.W / 2) {
                 cerr << "go left" << endl;
                 indice.push_back({ frog.x-1, 0 });

             }



             return;
         }*/

         /*if (dirblock != 4 && frog.x - 1 >= 0) {
             if (grid.grid[frog.y][frog.x - 1] == State::log || grid.grid[frog.y][frog.x - 1] == State::coin) {
                 indice.push_back({ frog.x - 1, frog.y });
             }

         }
         if (dirblock != 2 && frog.x + 1 < grid.W) {
             if (grid.grid[frog.y][frog.x + 1] == State::log || grid.grid[frog.y][frog.x + 1] == State::coin) {
                 indice.push_back({ frog.x + 1, frog.y });
             }

         }*/

        if (frog.y - 1 >= 0) {
            if (grid.grid[frog.y - 1][frog.x] == State::log || grid.grid[frog.y - 1][frog.x] == State::coin) {
                indice.push_back({ frog.x, frog.y - 1 });
            }
        }

        if (frog.y + 1 < grid.H) {
            if (grid.grid[frog.y + 1][frog.x] == State::log || grid.grid[frog.y + 1][frog.x] == State::coin) {
                indice.push_back({ frog.x, frog.y + 1 });
            }
        }

       
        if (frog.x - 1 >= 0) {
            if (grid.grid[frog.y][frog.x - 1] == State::log || grid.grid[frog.y][frog.x - 1] == State::coin) {
                indice.push_back({ frog.x - 1, frog.y });
            }

        }
        if (frog.x + 1 < grid.W) {
            if (grid.grid[frog.y][frog.x + 1] == State::log || grid.grid[frog.y][frog.x + 1] == State::coin) {
                indice.push_back({ frog.x + 1, frog.y });
            }

        }


        
   


#ifdef DEBUG
        cerr << "end poss move" << endl;
#endif
    }


    void GetFrog(Grid grid, vector<char>& indice, Coord frog) {

        if (frog.x - 1 >= 0) {
            if (grid.grid[frog.y][frog.x - 1] != State::frog) {
                indice.push_back('L');
            }

        }
        if (frog.x + 1 < grid.W) {
            if (grid.grid[frog.y][frog.x + 1] != State::frog) {
                indice.push_back('R');
            }

        }

        if (frog.y - 1 >= 0) {
            if (grid.grid[frog.y - 1][frog.x] != State::frog) {
                indice.push_back('U');
            }

        }
        if (frog.y + 1 < grid.H) {
            if (grid.grid[frog.y + 1][frog.x] != State::frog) {
                indice.push_back('D');
            }

        }

    }

    void GetFrogLR(Grid grid, bool &l, bool &r, Coord frog) {

        if (frog.x - 1 >= 0) {
            if (grid.grid[frog.y][frog.x - 1] != State::frog) {
                l = true;
            }

        }
        if (frog.x + 1 < grid.W) {
            if (grid.grid[frog.y][frog.x + 1] != State::frog) {
                r = true;
            }

        }
               

    }

    string Simulate(Grid grid, Coord frog, int depth, int &r, int &c, int nbcoins, float time) {

        this->root->Reset();
       
        vector<vector<char>> gr(grid.H, vector<char>(grid.W));
        root->grid.W = grid.W;
        root->grid.H = grid.H;
        root->grid.KL = grid.KL;
        root->grid.KW = grid.KW;
        root->grid.PC = grid.PC;
        root->grid.Init_grid(gr);
        grid.Copy_grid(root->grid.grid);
        root->num = { frog.x, frog.y };
        root->nbcoins = nbcoins;
                       
        //cerr << ((float)(tact - timelcl)) / CLOCKS_PER_SEC << " " << (tact - timelcl) << " " << (elapsedTime - timelturn) << endl;
        clock_t timelcl;
        float timetot = 0.0f;
        //for (int d = 0; d < depth; ++d) {
      
        double num_sim = 0;
        
        auto start = high_resolution_clock::now();
        auto keepSolving = [&start, time]() -> bool
        {
            auto now = high_resolution_clock::now();
            auto d = duration_cast<microseconds>(now - start);
            //cerr << "---" << d.count() << endl;
            return (d.count() < time);
        };
        int x = 0;

        

        while(keepSolving()){
            //cerr << "sim1 " << timetot << endl;
            //timelcl = clock();
            //cerr << x << endl;

#ifdef DEBUG
            //cerr << "depth " << d << endl;
#endif
            
#ifdef DEBUG
            cerr << "Init root" << endl;
#endif
            this->root->n++;
           
            Node* node = this->root;
            double maxucb;
            num_sim = 1;
            while (node->child.size() > 0) {
                maxucb = numeric_limits<double>::min();
                int ind = 0;
                vector<Node*> tchild;
                vector<int>tind;

                for (int i = 0; i < node->child.size(); ++i) {
                    
                    node->child[i]->n++;
                    if (node->child[i]->n == 1)
                        node->child[i]->ucb = numeric_limits<double>::max();
                    else
                        node->child[i]->ucb = node->child[i]->score / node->child[i]->n + sqrt(2.0 * (log(node->n) / node->child[i]->n));
                   
                    if (node->child[i]->ucb > maxucb   && !node->child[i]->finalf) {
                        maxucb = node->child[i]->ucb;
                        tind = { i };
                    }
                    else if (node->child[i]->ucb == maxucb && !node->child[i]->finalf) {
                        tind.push_back(i);

                    }
                    //tchild.push_back(node->child[i]);
                }

                //sort(tchild.begin(), tchild.end(), [](Node* a, Node* b)->bool {
                //        return a->ucb > b->ucb;
                //    });
                ind = 0;
                if (tind.size() > 0)ind = rand() % tind.size();
                node = node->child[ind];
                node->n++;
                num_sim++;
            }

            

#ifdef DEBUG
            cerr << "Sorted child" << endl;
#endif
            vector<Coord> indice;
            int dirblock;
            
            vector<int>selnode;
            if (node->nbcoins < this->max_coins)dirblock = 1;
            else dirblock = 3;
            vector<char>vdir;
            GetPossibleMoveSim(node->grid, indice, vdir, node->num, dirblock);
            if (indice.size() > 0) {
                for (int i = 0; i < indice.size(); ++i) {
                    Node* n = new Node();
                    this->garbage.push_back(n);
                    n->parent = node;
                    n->num = { indice[i].x, indice[i].y };
                    vector<vector<char>> gr(grid.H, vector<char>(grid.W));
                    n->grid.W = grid.W;
                    n->grid.H = grid.H;
                    n->grid.KL = grid.KL;
                    n->grid.KW = grid.KW;
                    n->grid.PC = grid.PC;
                    n->grid.Init_grid(gr);
                    node->grid.Copy_grid(n->grid.grid);
                    n->grid.grid[node->num.y][node->num.x] = State::log;
                    n->grid.grid[indice[i].y][indice[i].x] = State::frog;
                    n->dir = vdir[i];
                    if (grid.rowDir[indice[i].y] == FlowState::right) {
                        n->num.x++;

                    }
                    else if (grid.rowDir[indice[i].y] == FlowState::left) {
                        n->num.x--;

                    }

                    //update grid
                    for (int i = 1; i < grid.rowDir.size(); ++i) {
                        vector<char> new_state;
                        if (grid.rowDir[i] == FlowState::right) {
                            int countw = 0, countl = 0;
                            if (n->grid.grid[i][0] == State::water) {
                                int j = 0;
                                while (j < n->grid.W && n->grid.grid[i][j] == State::water) {
                                    countw++;
                                    ++j;
                                }
                                if (countw == grid.KW) {
                                    double coin = rand() % 101;
                                    if (coin < 20)coin = 20;
                                    else if (coin > 60)coin = 60;
                                    new_state = { State::log };
                                    if (coin <= 100.0 * n->grid.PC)
                                        new_state.push_back(State::coin);
                                }
                                else {
                                    double coin = rand() % 101;
                                    if (coin < 20)coin = 20;
                                    else if (coin > 60)coin = 60;
                                    new_state = { State::water, State::log };
                                    if (coin <= 100.0 * n->grid.PC)
                                        new_state.push_back(State::coin);
                                }
                            }
                            else {
                                int j = 0;
                                while (j < n->grid.W && n->grid.grid[i][j] != State::water) {
                                    countl++;
                                    ++j;
                                }
                                if (countl == grid.KL) {
                                    new_state = { State::water };
                                }
                                else {
                                    double coin = rand() % 101;
                                    if (coin < 20)coin = 20;
                                    else if (coin > 60)coin = 60;
                                    new_state = { State::water, State::log };
                                    if (coin <= 100.0 * n->grid.PC)
                                        new_state.push_back(State::coin);
                                }
                            }

                            for (int j = grid.W - 1; j > 0; --j) {
                                n->grid.grid[i][j] = n->grid.grid[i][j - 1];
                            }
                            n->grid.grid[i][0] = new_state[rand()%new_state.size()];
                        }
                        else if (grid.rowDir[i] == FlowState::left) {
                            int countw = 0, countl = 0;
                            if (n->grid.grid[i][0] == State::water) {
                                int j = grid.W-1;
                                while (j >= 0 && n->grid.grid[i][j] == State::water) {
                                    countw++;
                                    --j;
                                }
                                if (countw == grid.KW) {
                                    double coin = rand() % 101;
                                    if (coin < 20)coin = 20;
                                    else if (coin > 60)coin = 60;
                                    new_state = { State::log };
                                    if (coin <= 100.0 * n->grid.PC)
                                        new_state.push_back(State::coin);
                                }
                                else {
                                    double coin = rand() % 101;
                                    if (coin < 20)coin = 20;
                                    else if (coin > 60)coin = 60;
                                    new_state = { State::water, State::log };
                                    if (coin <= 100.0 * n->grid.PC)
                                        new_state.push_back(State::coin);
                                }
                            }
                            else {
                                int j = grid.W - 1;
                                while (j >= 0 && n->grid.grid[i][j] != State::water) {
                                    countl++;
                                    --j;
                                }
                                if (countl == grid.KL) {
                                    new_state = { State::water };
                                }
                                else {
                                    double coin = rand() % 101;
                                    if (coin < 20)coin = 20;
                                    else if (coin > 60)coin = 60;
                                    new_state = { State::water, State::log };
                                    if (coin <= 100.0 * n->grid.PC)
                                        new_state.push_back(State::coin);
                                }
                            }

                            for (int j = 0; j < n->grid.W - 1; ++j) {
                                n->grid.grid[i][j] = n->grid.grid[i][j + 1];
                            }
                            n->grid.grid[i][n->grid.W - 1] = new_state[rand() % new_state.size()];
                        }
                    }

                    if (n->num.x >= 0 && n->num.x < grid.W)
                        node->child.push_back(n);
                  
                                       
                    
                }
#ifdef DEBUG
                cerr << "End expansion" << endl;
#endif

                //for (int nc = 0; nc < node->child.size(); ++nc) {
                if(node->child.size() > 0){
                    Node* ncsel = node->child[rand() % node->child.size()];
                    //Node* ncsel = node->child[nc];
                    float timesim = 500;
                    int score = this->Simulation(ncsel, grid.rowDir, ncsel->nbcoins, timesim);
                    int win = 1;
                    if (score < 0) {
                        score = 0;
                    }
#ifdef DEBUG
                    cerr << "End Simulation" << endl;
#endif
                    Node* child = ncsel;
                    while (child != nullptr) {
                        child->ns++;
                        child->n++;
                        child->score += (double)score * (1.0 / num_sim);
                        child = child->parent;
                    }
#ifdef DEBUG
                    cerr << "End backprop child" << endl;
#endif

                }
            }

            //clock_t tact = clock();
            //timetot += ((float)(tact - timelcl));
            ++x;
        }

        Node* best_child = nullptr;
        double score = 0;
        double maxscore = numeric_limits<double>::min();
        for (int i = 0; i < this->root->child.size(); ++i) {
            score = this->root->child[i]->score / (double)this->root->child[i]->ns;
#ifdef DEBUG
            cerr << "bchild " << this->root->child[i]->num.x << " " << this->root->child[i]->num.y << " "<<
                this->root->child[i]->score << endl;
#endif
            if (score > maxscore) {
                maxscore = score;
                best_child = this->root->child[i];
            }
        }

        

        string dir;
        if (best_child != nullptr) {
            if (best_child->dir == 'R') {
                dir = "R";
                r = frog.y;
                c = frog.x+1;
            }
            else if (best_child->dir == 'L') {
                dir = "L";
                r = frog.y;
                c = frog.x-1;
            }
            else if (best_child->dir == 'D') {
                dir = "D";
                r = frog.y+1;
                c = frog.x;
            }
            else if (best_child->dir == 'U') {
                dir = "U";
                r = frog.y-1;
                c = frog.x;
            }
        }
#ifdef DEBUG
        cerr << "End sel best child" << endl;
#endif
        if (dir.empty()) {
            //cerr << "dir empty" << endl;
            if (frog.y - 1 == 0) {
                r = frog.y - 1;
                c = frog.x;
                return "-1";

            }
            else {
                r = frog.y - 1;
                c = frog.x;
                return "-2";

            }
        }
#ifdef DEBUG
        cerr << "frog " << frog.x << " " << frog.y << ", bc " << best_child->num.x << " " << best_child->num.y << endl;
        cerr << "MOVE " << to_string(frog.y) << " " << to_string(frog.x) << " " << dir << endl;
#endif
        

        return to_string(frog.y) + " " + to_string(frog.x) + " " + dir;


    }

    ~LargentiereTreeSearch() {
        for (int i = 0; i < this->garbage.size(); ++i) {
            delete this->garbage[i];
        }
    }
    
private:
    int Simulation(Node* node, vector<char> rowDir, int nbcoins, float time) {
        unsigned long long int score = 0;

        int POS_IDEAL = node->grid.W / 4;

        vector<vector<char>> gr(node->grid.H, vector<char>(node->grid.W));
        Grid new_grid;
        new_grid.W = node->grid.W;
        new_grid.H = node->grid.H;
        new_grid.KL = node->grid.KL;
        new_grid.KW = node->grid.KW;
        new_grid.PC = node->grid.PC;
        new_grid.Init_grid(gr);
        node->grid.Copy_grid(new_grid.grid);
#ifdef DEBUG
        cerr << "Init newgrid" << endl;
#endif
        Coord frog = { node->num.x, node->num.y };
        bool score_neg = false;
        int x = 0;
        this->nb_coins = nbcoins;

        
        clock_t timelcl;
        float timetot = 0.0f;

       
        auto start = high_resolution_clock::now();
        auto keepSolving = [&start, time]() -> bool
        {
            auto now = high_resolution_clock::now();
            auto d = duration_cast<microseconds>(now - start);
            return (d.count() < time);
        };

        while (x < 2) {
            //while (keepSolving()) {
                //cerr << "sim2 " << timetot << endl;

            timelcl = clock();


            vector<Coord> indice;
            int dirblock;

            if (this->nb_coins < this->max_coins)dirblock = 1;
            else dirblock = 3;
            GetPossibleMove(new_grid, indice, frog, dirblock);
#ifdef DEBUG
            cerr << "get possible move" << indice.size() << endl;
#endif
            if (indice.size() > 0) {
                int num = rand() % indice.size();

                /*cerr << "before " << endl;
                for (int i = 0; i < new_grid.H; ++i) {
                    for (int j = 0; j < new_grid.W; ++j) {
                        cerr << new_grid.grid[i][j];
                    }cerr << endl;
                }cerr << endl;*/

                /*
                else {
                    if (new_grid.grid[indice[num].y][indice[num].x] == State::water ||
                        new_grid.grid[indice[num].y][indice[num].x] == State::frog) {
                        score = -10000;
                        break;
                    }
                }*/

                if (new_grid.grid[indice[num].y][indice[num].x] == State::beach) {
                    node->finalf = true;
                    break;
                }

                //if (dirblock == 3)score += frog.y * 1000;
                //if (dirblock == 1)score += (new_grid.H - frog.y) * 1000;

                if (frog.y > 0)
                    new_grid.grid[frog.y][frog.x] = State::log;
                else
                    new_grid.grid[frog.y][frog.x] = State::beach;

                if (new_grid.grid[indice[num].y][indice[num].x] == State::coin) {
                    this->nb_coins += indice[num].y;
                    /*if (dirblock == 3)*/score += 10000 * this->nb_coins;

                }
                else {
                    if (this->nb_coins > 5 && (indice[num].x >= POS_IDEAL || indice[num].x <= node->grid.W - (POS_IDEAL + 1)))
                        score += 50 * this->nb_coins;
                    else if (indice[num].x >= POS_IDEAL || indice[num].x <= node->grid.W - (POS_IDEAL + 1))
                        score += 50;
                    /*else if (indice[num].x < POS_IDEAL || indice[num].x > node->grid.W - (POS_IDEAL + 1)) {
                        score = -100000*this->nb_coins;
                        node->finalf = true;
                        break;
                    }*/


                }


                //score += (new_grid.W - (new_grid.W - frog.y)) * this->nb_coins * 10000;

                if (this->nb_coins >= 5 && node->grid.grid[indice[num].y][indice[num].x] == State::beach)score += 100000 * this->nb_coins;
                if (this->nb_coins >= 5 && indice[num].y < frog.y
                    && (indice[num].x >= POS_IDEAL || indice[num].x <= node->grid.W - (POS_IDEAL + 1))
                    )score += 100000 * this->nb_coins;
                new_grid.grid[indice[num].y][indice[num].x] = State::frog;

                /*cerr << "after " << endl;
                for (int i = 0; i < new_grid.H; ++i) {
                    for (int j = 0; j < new_grid.W; ++j) {
                        cerr << new_grid.grid[i][j];
                    }cerr << endl;
                }cerr << endl;*/

#ifdef DEBUG
                cerr << "End score" << endl;
#endif
                frog = { indice[num].x, indice[num].y };
                if (rowDir[indice[num].y] == FlowState::right) {
                    frog.x++;
                    //node->num.x= frog.x;

                }
                else if (rowDir[indice[num].y] == FlowState::left) {
                    frog.x--;
                    //node->num.x= frog.x;

                }

                if (frog.x < 0 || frog.x >= node->grid.W) {
                    score = -10000 * this->nb_coins;
                    this->nb_coins = 0;
                    node->finalf = true;

                    break;
                }
#ifdef DEBUG
                cerr << "update grid" << endl;
#endif
                //update grid
                for (int i = 1; i < rowDir.size(); ++i) {
                    //cerr << "grid" << i << endl;
                    vector<char> new_state;
                    if (rowDir[i] == FlowState::right) {
                        int countw = 0, countl = 0;
                        if (new_grid.grid[i][0] == State::water) {
                            int j = 0;
                            while (j < new_grid.W && new_grid.grid[i][j] == State::water) {
                                countw++;
                                ++j;
                            }
                            if (countw == new_grid.KW) {
                                double coin = rand() % 101;
                                if (coin < 20)coin = 20;
                                else if (coin > 60)coin = 60;
                                new_state = { State::log };
                                if (coin <= 100.0 * new_grid.PC)
                                    new_state.push_back(State::coin);
                            }
                            else {
                                double coin = rand() % 101;
                                if (coin < 20)coin = 20;
                                else if (coin > 60)coin = 60;
                                new_state = { State::water, State::log};
                                if (coin <= 100.0 * new_grid.PC)
                                    new_state.push_back(State::coin);
                            
                            }
                        }
                        else {
                            int j = 0;
                            while (j < new_grid.W && new_grid.grid[i][j] != State::water) {
                                countl++;
                                ++j;
                            }
                            if (countl == new_grid.KL) {
                                new_state = { State::water };
                            }
                            else {
                                double coin = rand() % 101;
                                if (coin < 20)coin = 20;
                                else if (coin > 60)coin = 60;
                                new_state = { State::water, State::log };
                                if (coin <= 100.0 * new_grid.PC)
                                    new_state.push_back(State::coin);
                            }
                        }

                        for (int j = node->grid.W - 1; j > 0; --j) {
                            new_grid.grid[i][j] = new_grid.grid[i][j - 1];
                        }
                        new_grid.grid[i][0] = new_state[rand() % new_state.size()];
                    }
                    else if (rowDir[i] == FlowState::left) {
                        int countw = 0, countl = 0;
                        if (new_grid.grid[i][0] == State::water) {
                            int j = new_grid.W - 1;
                            while (j >= 0 && new_grid.grid[i][j] == State::water) {
                                countw++;
                                --j;
                            }
                            if (countw == new_grid.KW) {
                                double coin = rand() % 101;
                                if (coin < 20)coin = 20;
                                else if (coin > 60)coin = 60;
                                new_state = { State::log };
                                if (coin <= 100.0 * new_grid.PC)
                                    new_state.push_back(State::coin);
                            }
                            else {
                                double coin = rand() % 101;
                                if (coin < 20)coin = 20;
                                else if (coin > 60)coin = 60;
                                new_state = { State::water, State::log };
                                if (coin <= 100.0 * new_grid.PC)
                                    new_state.push_back(State::coin);
                            }
                        }
                        else {
                            int j = new_grid.W - 1;
                            while (j >= 0 && new_grid.grid[i][j] != State::water) {
                                countl++;
                                --j;
                            }
                            if (countl == new_grid.KL) {
                                new_state = { State::water };
                            }
                            else {
                                double coin = rand() % 101;
                                if (coin < 20)coin = 20;
                                else if (coin > 60)coin = 60;
                                new_state = { State::water, State::log };
                                if (coin <= 100.0 * new_grid.PC)
                                    new_state.push_back(State::coin);
                            }
                        }

                        for (int j = 0; j < node->grid.W - 1; ++j) {
                            new_grid.grid[i][j] = new_grid.grid[i][j + 1];
                        }
                        new_grid.grid[i][node->grid.W - 1] = new_state[rand() % new_state.size()];
                    }
                }

                /*for (int i = 0; i < new_grid.H; ++i) {
                    for (int j = 0; j < new_grid.W; ++j) {
                        cerr << new_grid.grid[i][j];
                    }cerr << endl;
                }cerr << endl;*/
#ifdef DEBUG
                cerr << "End update grid" << endl;
#endif
            }
            else {
                break;
            }


            clock_t tact = clock();
            timetot += ((float)(tact - timelcl));

            ++x;

        }
#ifdef DEBUG
        cerr << "Return score simulation " << score << endl;
#endif

        score += 10000 * this->nb_coins;
        node->nbcoins = this->nb_coins;

        return score;

    }


    int Simulation2(Node* node, vector<char> rowDir, int nbcoins, float time) {
        unsigned long long int score = 0;

        int POS_IDEAL = node->grid.W / 4;
       
        /*vector<vector<char>> gr(node->grid.H, vector<char>(node->grid.W));
        Grid new_grid;
        new_grid.W = node->grid.W;
        new_grid.H = node->grid.H;
        new_grid.Init_grid(gr);
        node->grid.Copy_grid(new_grid.grid);*/
#ifdef DEBUG
        cerr << "Init newgrid" << endl;
#endif
        Coord frog = { node->num.x, node->num.y };
        bool score_neg = false;
        int x = 0;
        this->nb_coins = nbcoins;

        int max_coins = (((double)node->grid.H * 200.0 / 30.0) * (double)node->grid.H / 30.0);

        clock_t timelcl;
        float timetot = 0.0f;

        auto start = high_resolution_clock::now();
        auto keepSolving = [&start, time]() -> bool
        {
            auto now = high_resolution_clock::now();
            auto d = duration_cast<microseconds>(now - start);
            return (d.count() < time);
        };

        while (x < 2){
        //while (keepSolving()) {
            //cerr << "sim2 " << timetot << endl;

            timelcl = clock();
            

            vector<Coord> indice;
            int dirblock;
            
            if (this->nb_coins < max_coins)dirblock = 1;
            else dirblock = 3;
            GetPossibleMove(node->grid, indice, frog, dirblock);
#ifdef DEBUG
            cerr << "get possible move" << indice.size() <<  endl;
#endif
            if (indice.size() > 0) {
                int num = rand() % indice.size();

                /*cerr << "before " << endl;
                for (int i = 0; i < new_grid.H; ++i) {
                    for (int j = 0; j < new_grid.W; ++j) {
                        cerr << new_grid.grid[i][j];
                    }cerr << endl;
                }cerr << endl;*/
                
                /*
                else {
                    if (new_grid.grid[indice[num].y][indice[num].x] == State::water ||
                        new_grid.grid[indice[num].y][indice[num].x] == State::frog) {
                        score = -10000;
                        break;
                    }
                }*/

                if (node->grid.grid[indice[num].y][indice[num].x] == State::beach) {
                    node->finalf = true;
                    break;
                }

                //if (dirblock == 3)score += frog.y * 1000;
                //if (dirblock == 1)score += (new_grid.H - frog.y) * 1000;
                
                if(frog.y > 0)
                    node->grid.grid[frog.y][frog.x] = State::log;
                else
                    node->grid.grid[frog.y][frog.x] = State::beach;

                if (node->grid.grid[indice[num].y][indice[num].x] == State::coin) {
                    this->nb_coins+= indice[num].y;
                    /*if (dirblock == 3)*/score += 10000 * this->nb_coins;
                    
                }
                else {
                    if (this->nb_coins > 5 && (indice[num].x >= POS_IDEAL || indice[num].x <= node->grid.W - (POS_IDEAL + 1)))
                        score += 50 * this->nb_coins;
                    else if (indice[num].x >= POS_IDEAL || indice[num].x <= node->grid.W - (POS_IDEAL + 1))
                        score += 50;
                    /*else if (indice[num].x < POS_IDEAL || indice[num].x > node->grid.W - (POS_IDEAL + 1)) {
                        score = -100000*this->nb_coins;
                        node->finalf = true;
                        break;
                    }*/
                  

                }


                //score += (new_grid.W - (new_grid.W - frog.y)) * this->nb_coins * 10000;

                if (this->nb_coins >=5 && node->grid.grid[indice[num].y][indice[num].x] == State::beach)score += 100000*this->nb_coins;
                if (this->nb_coins >=5 && indice[num].y < frog.y
                    && (indice[num].x >= POS_IDEAL || indice[num].x <= node->grid.W - (POS_IDEAL + 1))
                    )score += 100000*this->nb_coins;
                node->grid.grid[indice[num].y][indice[num].x] = State::frog;

                /*cerr << "after " << endl;
                for (int i = 0; i < new_grid.H; ++i) {
                    for (int j = 0; j < new_grid.W; ++j) {
                        cerr << new_grid.grid[i][j];
                    }cerr << endl;
                }cerr << endl;*/

#ifdef DEBUG
                cerr << "End score" << endl;
#endif
                frog = { indice[num].x, indice[num].y };
                if (rowDir[indice[num].y] == FlowState::right) {
                    frog.x++;
                    //node->num.x= frog.x;
                    
                }
                else if (rowDir[indice[num].y] == FlowState::left) {
                    frog.x--;
                    //node->num.x= frog.x;
                    
                }

                if (frog.x < 0 || frog.x >= node->grid.W) {
                    score =  -10000 * this->nb_coins;
                    this->nb_coins = 0;
                    node->finalf = true;
                  
                    break;
                }

                //update grid
                for (int i = 1; i < rowDir.size(); ++i) {
                    if (rowDir[i] == FlowState::right) {
                        for (int j = node->grid.W - 1;j > 0; --j) {
                            node->grid.grid[i][j] = node->grid.grid[i][j - 1];
                        }
                        node->grid.grid[i][0] = State::water;
                    }
                    else if (rowDir[i] == FlowState::left) {
                        for (int j = 0;j < node->grid.W-1; ++j) {
                            node->grid.grid[i][j] = node->grid.grid[i][j + 1];
                        }
                        node->grid.grid[i][node->grid.W - 1] = State::water;
                    }
                }

                /*for (int i = 0; i < new_grid.H; ++i) {
                    for (int j = 0; j < new_grid.W; ++j) {
                        cerr << new_grid.grid[i][j];
                    }cerr << endl;
                }cerr << endl;*/
#ifdef DEBUG
                cerr << "End update grid" << endl;
#endif
            }
            else {
                break;
            }


            clock_t tact = clock();
            timetot += ((float)(tact - timelcl));

            ++x;

        }
#ifdef DEBUG
        cerr << "Return score simulation " << score << endl;
#endif
        
        score += 10000 * this->nb_coins;
        node->nbcoins = this->nb_coins;

        return score;
    
    }




};

struct Game {

    GameState::Type game_state = GameState::home;
    GameState::UnderType under_game_state = GameState::go_down;
    vector<int> path;
    int POS_REPLACE;
    int POS_WAIT;
    int NB_COINS = 0;
    int MAX_COIN_HARVEST = 5;
    int MAX_DOWN = 3;
    bool GO_BEACH = false;
    double max_coins;
    vector<bool> VWAIT;
    vector<int> VNB_COINS;
    vector< GameState::Type> vgame_state;
    vector<bool> vGO_BEACH;
    LargentiereTreeSearch *largts;

    void Start(Grid grid) {
        game_state = GameState::home;
        if (grid.W < 15) {
            POS_REPLACE = 2;
            
        }
        else {
            POS_REPLACE = 4;
            
        }

        if (grid.W <= 10)
            POS_WAIT = 3;
        else if (grid.W <= 15)
            POS_WAIT = 5;
        else if (grid.W <= 20)
            POS_WAIT = 5;
        else if (grid.W <= 25)
            POS_WAIT = 6;
        else if (grid.W <= 30)
            POS_WAIT = 6;
        
        NB_COINS = 0;

        VNB_COINS.resize(grid.F);
        vgame_state.resize(grid.F);
        vGO_BEACH.resize(grid.F);
        VWAIT.resize(grid.F);
        for (int i = 0; i < this->vGO_BEACH.size(); ++i) {
            this->vGO_BEACH[i] = false;
            this->VWAIT[i] = false;
        }
        

        this->max_coins = (((double)grid.H * 75.0 / 30.0) * (double)grid.H / 30.0);// *(W / (double)grid.H);
    }

    string Get_Move(Grid grid, Coord frog, int depth, int &r, int &c, int nbcoins, float time) {
        largts = new LargentiereTreeSearch(75.0, grid.H, grid.W, grid.PC);
        string move = largts->Simulate(grid, frog, depth, r, c, nbcoins, time);
        return move;
    }

    char GetFrog(Grid grid, vector<char>& indice, Coord frog) {
        largts->GetFrog(grid, indice, frog);
        if (indice.size() == 0)return 'U';
        else return indice[0];

        return 'A';
    }

    void GetFrogLR(Grid grid, bool &l, bool &r, Coord frog) {
      
        largts->GetFrogLR(grid, l, r, frog);
        
    }

    

};

int main()
{
    srand(time(NULL));
    Game game;
    Grid grid;
    
    cin >> grid.H >> grid.W >> grid.F >> grid.KL >> grid.KW >> grid.PC;
    game.Start(grid);
    Coord frog;
    
    vector<char> rowDir(grid.H);
    for (int r = 1; r < grid.H; r++) cin >> rowDir[r];
    grid.Init_rowDir(rowDir);

    vector<vector<char>> gr(grid.H, vector<char>(grid.W));
    grid.Init_grid(gr);

    clock_t timelcl=clock();
    int timepass = 0;
    int timelturn = 0;
    int totaltime = 0;
    int acttime = 0;
    float add_time = 0;
    float past_time=0;
    for (int turn = 1; turn <= 1000; turn++)
    {
        vector<Coord>frogs;
        for (int r = 0; r < grid.H; r++) {
            for (int c = 0; c < grid.W; c++) {
                cin >> grid.grid[r][c];
                if (grid.grid[r][c] == State::frog) {
                    frog.x = c;
                    frog.y = r;
                    frogs.push_back({ c, r });
                }
            }

        }

        int elapsedTime;
        cin >> elapsedTime;
        //cerr << turn << " " << (elapsedTime - timelturn) << endl;
        timepass = (elapsedTime - timelturn) * 1000;
        //cerr << elapsedTime << endl;
        //clock_t tact = clock();
        //cerr << ((float)(tact - timelcl))/ CLOCKS_PER_SEC << " "<< (tact - timelcl)  << " " << (elapsedTime - timelturn) << endl;
        //timelcl = tact;
        timelturn = elapsedTime;

        if (turn > 1) {
            past_time += max(10000 - timepass, 0);
        }
        

#ifdef DEBUG2
        cerr << "before " << endl;

        for (int i = 0; i < grid.H; ++i) {
            for (int j = 0; j < grid.W; ++j) {
                cerr << grid.grid[i][j];
            }cerr << endl;
        }cerr << endl;
#endif
        
        vector<string> moves;

#ifdef DEBUG2
        cerr << "size " << frogs.size() << endl;
#endif
        for (int i = 0; i < frogs.size(); ++i) {

            string move;
            frog = { frogs[i].x, frogs[i].y };
#ifdef DEBUG2
            cerr << "frog " << frog.x << " " << frog.y << endl;
#endif
            //game.Analyze_Grid(grid, frog.x, frog.y, move);
            if (frog.y == 0) {
                game.vgame_state[i] = GameState::home;
                game.VNB_COINS[i] = 0;
                game.vGO_BEACH[i] = false;
            }
            else {
                game.vgame_state[i] = GameState::search_coin;
            }

            //int max_coins = (50.0 * (double)grid.H / 30.0) / 2;
            if (frog.y == 1 && game.VNB_COINS[i] > 0) {
                game.vGO_BEACH[i] = true;
            }
            else if (frog.y > 1) {
                game.vGO_BEACH[i] = false;

                game.VWAIT[i] = false;
                if (frog.x <= game.POS_WAIT || frog.x >= (grid.W - game.POS_WAIT)) {
                    game.VWAIT[i] = true;
                }



            }

            /* && game.VNB_COINS[i] > game.max_coins / 2.0*/
            if (game.VWAIT[i]) {
                if (frog.x <= game.POS_WAIT && grid.rowDir[frog.y] == FlowState::right) {
                    continue;
                }
                if (frog.x >= (grid.W - game.POS_WAIT) && grid.rowDir[frog.y] == FlowState::left) {
                    continue;
                }
            }


            if (game.vgame_state[i] == GameState::home) {

                if (grid.grid[1][frog.x] == State::log || grid.grid[1][frog.x] == State::coin) {
                    move = to_string(frog.y) + " " + to_string(frog.x) + " D";
#ifdef DEBUG2
                    cerr << "home1" << endl;
#endif
                    grid.grid[1][frog.x] = State::frog;
                    game.vgame_state[i] = GameState::search_coin;
                    if (grid.grid[1][frog.x] == State::coin) {
                        game.VNB_COINS[i]++;
                        //game.GO_BEACH = true;
                    }
                }
                else {
                    vector<char>indice;
                    bool l=false, r=false;
                    game.GetFrogLR(grid, l, r, frog);
                    if (frog.x < game.POS_REPLACE) {
                        if (r) {
#ifdef DEBUG2
                            cerr << "home r" << endl;
#endif
                            move = to_string(frog.y) + " " + to_string(frog.x) + " R";
                            grid.grid[frog.y][frog.x + 1] = State::frog;
                        }
                        else if (l) {
#ifdef DEBUG2
                            cerr << "home l" << endl;
#endif
                            move = to_string(frog.y) + " " + to_string(frog.x) + " L";
                            grid.grid[frog.y][frog.x - 1] = State::frog;
                        }
                        else {
#ifdef DEBUG2
                            cerr << "home n" << endl;
#endif
                            move = "";// to_string(frog.y) + " " + to_string(frog.x) + " D";
                            
                        }
                    }
                    else if (frog.x > grid.W - (game.POS_REPLACE + 1)) {
                        if (l) {
#ifdef DEBUG2
                            cerr << "home l2" << endl;
#endif
                            move = to_string(frog.y) + " " + to_string(frog.x) + " L";
                            grid.grid[frog.y][frog.x - 1] = State::frog;
                        }
                        else if (r) {
#ifdef DEBUG2
                            cerr << "home r2" << endl;
#endif
                            move = to_string(frog.y) + " " + to_string(frog.x) + " R";
                            grid.grid[frog.y][frog.x + 1] = State::frog;
                        }
                        else {
#ifdef DEBUG2
                            cerr << "home n2" << endl;
#endif
                            move = "";// to_string(frog.y) + " " + to_string(frog.x) + " D";

                        }
                    }
                }

                


                if (move.empty()) {
                    vector<char>indice;
                    bool l = false, r = false;
                    game.GetFrogLR(grid, l, r, frog);
                    if (frog.x < grid.W / 2) {
                        if (r) {
#ifdef DEBUG2
                            cerr << "home er1" << endl;
#endif
                            move = to_string(frog.y) + " " + to_string(frog.x) + " R";
                            grid.grid[frog.y][frog.x + 1] = State::frog;
                        }
                        else if (l) {
#ifdef DEBUG2
                            cerr << "home el1" << endl;
#endif
                            move = to_string(frog.y) + " " + to_string(frog.x) + " L";
                            grid.grid[frog.y][frog.x - 1] = State::frog;
                        }
                        else {
#ifdef DEBUG2
                            cerr << "home en1" << endl;
#endif
                            move = "";

                        }
                    }
                    else {
                        if (l) {
#ifdef DEBUG2
                            cerr << "home el2" << endl;
#endif
                            move = to_string(frog.y) + " " + to_string(frog.x) + " L";
                            grid.grid[frog.y][frog.x - 1] = State::frog;
                        }
                        else if (r) {
#ifdef DEBUG2
                            cerr << "home er2" << endl;
#endif
                            move = to_string(frog.y) + " " + to_string(frog.x) + " R";
                            grid.grid[frog.y][frog.x + 1] = State::frog;
                        }
                        else {
#ifdef DEBUG2
                            cerr << "home en2" << endl;
#endif
                            move = "";

                        }
                    }
                }

                if (!move.empty()) {
                    moves.push_back(move);
#ifdef DEBUG2
                    cerr << "---frog " << frogs[i].x << " " << frogs[i].y << endl;
#endif
                    grid.grid[frogs[i].y][frogs[i].x] = State::beach;
                }
            }
            else {
                int r, c;
                if (!game.vGO_BEACH[i]) {
                    /*int maxc = -1;
                    for (int j = 0; j < grid.F; ++j) {
                        maxc = max(maxc, game.VNB_COINS[i]);
                    }*/

                    float timef = 8000.0f / (float)grid.F;
                    if (turn > 1) {
                        //float atimef = max(add_time - elapsedTime * 1000.0f, 0.0f);
                        timef += add_time;
                    }
                    past_time = 0;
                    /*if (maxc > 0) {
                        timef = (9000.0f / (float)grid.F) * 0.75f + (float)add_time;
                        float add_t = (9000.0f / (float)grid.F) * 0.25f * (1.0f - (float)game.VNB_COINS[i] / (float)maxc);
                        timef += add_t;
                        add_time += ((9000.0f / (float)grid.F) * 0.25f) - add_t;
                    }*/
                    move = game.Get_Move(grid, frog, 5, r, c, game.VNB_COINS[i], timef);
                    acttime += timef;

                    if (grid.grid[r][c] == State::coin) {
                        game.VNB_COINS[i]+=r;
                        

                    }
                    if (move == "-2") {
                        //vector<char>indice;
                        //char dir = game.GetFrog(grid, indice, frog);
                        //move = to_string(frog.y) + " " + to_string(frog.x) + " " + dir;
#ifdef DEBUG2
                        cerr << -2 << endl;
#endif
                        move = "";
                        //grid.grid[frog.y-1][frog.x] = State::water;
                        //game.VNB_COINS[i] = 0;
                    }
                    else if (move == "-1") {
#ifdef DEBUG2
                        cerr << -1 << endl;
#endif
                        if (grid.grid[frog.y-1][frog.x] == State::frog) {
                            move = "";
                        }
                        else {
                            game.vgame_state[i] = GameState::home;
                            game.VNB_COINS[i] = 0;
                            move = to_string(frog.y) + " " + to_string(frog.x) + " U";
                            grid.grid[frog.y - 1][frog.x] = State::frog;
                        }
                    }
                    else {
#ifdef DEBUG2
                        cerr << "r " << r << ",  c " << c << endl;
#endif
                        //cerr << move << endl;
                        //if (grid.grid[r][c] != State::frog)
                            grid.grid[r][c] = State::frog;
                        //else
                         //   move = "";
                        /*if (r - 1 == 0 && game.VNB_COINS[i] > 0) {
                            game.vGO_BEACH[i] = true;
                        }*/
                    }

                    

                }
                else {
                    if (grid.grid[0][frog.x] == State::frog) {
                        move = "";
#ifdef DEBUG2
                        cerr << "go beach no" << endl;
#endif
                    }
                    else {
                        move = to_string(frog.y) + " " + to_string(frog.x) + " U";
                        grid.grid[0][frog.x] = State::frog;
                        game.vgame_state[i] = GameState::home;
#ifdef DEBUG2
                        cerr << "go beach yes" << endl;
#endif
                    }
                    
                }

                
                if (!move.empty()) {
                    moves.push_back(move);
#ifdef DEBUG2
                    cerr << "---frog2 " << frogs[i].x << " " << frogs[i].y << endl;
#endif
                    grid.grid[frogs[i].y][frogs[i].x] = State::log;
                }
            }
#ifdef DEBUG2
            cerr << "i" << i << " - "<< move << endl;
            for (int i = 0; i < grid.H; ++i) {
                for (int j = 0; j < grid.W; ++j) {
                    cerr << grid.grid[i][j];
                }cerr << endl;
            }cerr << endl;
#endif

            
            if (turn > 1) {
                add_time = max(10000 - timepass, 0) + past_time;
            }

            //cerr << turn << endl;
        }

        
        
        //cerr << "Analyze " << game.game_state << " " << game.under_game_state << endl;
        /*vector<string> moves;
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
        }*/

        
        
        cout << moves.size() << endl;
        for (string& mov : moves) {
            //cerr << "move " << mov << " = " << game.NB_COINS << endl;
           cout << mov << endl;
        }
        cout.flush();
        
       
    }
    return 0;
}

