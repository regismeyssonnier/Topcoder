// MM155.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <algorithm>
#include <list>
#include <string>
#include <vector>
#include <math.h>
#include <random>
#include <chrono>
#include <deque>
#include <map>
#include <tuple>
#include <queue>
#include <functional>
using namespace std::chrono;
using namespace std;
using namespace std;


#define ll long long int 

class Node {
public:
    int row;
    int col;
    double score=0;
    int m=0;
    list<string> path;
    vector<Node> path_i;
    vector<vector<bool>> used;

    Node():row(0), col(0) {}
    Node(int r, int c): row(r), col(c) {}




};

class CellList {
public:
    int row;
    int col;
    
    CellList() {}
    CellList(int r, int c) : row(r), col(c) {}

};


class ListPath {
public:
    Node head;
    vector<CellList> cell;
    int score = 0;
};


class Cell {
public:
    int row;
    int col;
    int dir;
    int mult;
    vector<Cell*> next, prec;

    Cell() {}
    Cell(int r, int c, int d, int m): row(r), col(c), dir(d), mult(m){
    
        
    
    }


};

class Grid {
public:
    int N;
    vector<vector<Cell>> body;
    Grid() {}
    Grid(int N, vector<vector<int>>& arrows, vector<vector<int>>& mults): N(N){
        body = vector<vector<Cell>>(N, vector<Cell>(N, Cell()));

        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                body[i][j] = Cell(i, j, arrows[i][j], mults[i][j]);
            }
        }

        int dr[] = { -1,-1, 0, 1, 1, 1, 0,-1 };
        int dc[] = { 0, 1, 1, 1, 0,-1,-1,-1 };

        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                int row = i, col = j;
                int dir = arrows[row][col];

                row += dr[dir];
                col += dc[dir];

                while (row >= 0 && row < N && col >= 0 && col < N) {
                    
                    body[i][j].next.push_back(&body[row][col]);
                    body[row][col].prec.push_back(&body[i][j]);

                    row += dr[dir];
                    col += dc[dir];

                }

            }

        }
    
    }


};

class MM155 {
public:
    int N;
    double P1, P2, P3;
    vector<vector<int>> arrows;
    vector<vector<int>> mults;
    list<Node> one_points, two_points, three_points, five_points;
    int _GLG = 2;
    int MAX_SCORE = 0;
    list<string> PATH;
    list<Node> path_init;
    ListPath anal_path;
    Grid grid;
    int MINUS_TIME = 0;

    MM155() {}
    MM155(int N, Grid &grid,vector<vector<int>> &arrows, vector<vector<int>> &mults,
         list<Node> &one_points, list<Node> &two_points, list<Node> &three_points, list<Node> &five_points) : 
         N(N), grid(grid), arrows(arrows), mults(mults), one_points(one_points), two_points(two_points), three_points(three_points), five_points(five_points){
    
                
    
    
    }

    void BSG_init(Node* root, int BWIDTH) {

        auto startm = high_resolution_clock::now();

        int GLG = 3;

        // max inf
        priority_queue<Node*, vector<Node*>, function<bool(Node*, Node*)>> beam(
            [](Node* a, Node* b) { return a->score < b->score; }
        );

        priority_queue<Node*, vector<Node*>, function<bool(Node*, Node*)>> TS(
            [](Node* a, Node* b) { return a->score < b->score; }
        );

        int dr[] = { -1,-1, 0, 1, 1, 1, 0,-1 };
        int dc[] = { 0, 1, 1, 1, 0,-1,-1,-1 };


        Node T0 = *root;
        Node* nsel = root;
        beam.push(root);

        int srow = nsel->row, scol = nsel->col;

        while (!beam.empty()) {

            for (int W = 0; W < BWIDTH && !beam.empty(); ++W) {
                //cerr << "beam size " << beam.size() << endl;
                nsel = beam.top();
                beam.pop();

                int row = nsel->row, col = nsel->col;
                //cerr << "gcell = " << row << " " << col << endl;
                int dir = arrows[row][col];

                int glg = 0;
                while (glg < GLG) {

                    row += dr[dir];
                    col += dc[dir];

                    //cerr << "cell = " << row << " " << col << endl;


                    if (row < 0 || row >= N || col < 0 || col >= N) {
                        //cerr << "break " << row << " " << col << endl;
                        break;
                    }
                    else {
                        if (nsel->used[row][col]) {
                            //cerr << "continue" << endl;
                            continue;

                        }
                    }



                    Node* new_node = new Node(row, col);
                    new_node->m = nsel->m + 1;
                    new_node->path_i = nsel->path_i;
                    new_node->path_i.push_back(Node(row, col));
                    new_node->path = nsel->path;
                    new_node->path.push_back(to_string(row) + " " + to_string(col));
                    //double e = exp(double(new_node->m) / 1000.0);
                    new_node->score = nsel->score + (double)new_node->m * (double)mults[row][col];
                    new_node->used = nsel->used;
                    new_node->used[row][col] = true;

                    //cerr << "add new node " << endl;
                    TS.push(new_node);

                    glg++;

                }
                //cerr << "end " << endl;

            }

            while (!beam.empty()) {
                delete beam.top();
                beam.pop();

            }
            beam.swap(TS);

            if (!beam.empty()) {
                if (beam.top()->score > T0.score)
                    T0 = *beam.top();
            }

            //cerr << "size = " << T0->path.size() << " " << T0->score << " " << maxt << endl;

        }

        anal_path.head = T0;
        for (Node m : T0.path_i) {
            anal_path.cell.push_back(CellList(m.row, m.col));
        }

        auto finalm = high_resolution_clock::now();;
        auto duration = duration_cast<milliseconds>(finalm - startm);
        //cerr << duration.count() << endl;
        MINUS_TIME = duration.count();

    }

    void BSG_initR(Node* root, int BWIDTH) {

        auto startm = high_resolution_clock::now();

        int GLG = 2;

        // max inf
        priority_queue<Node*, vector<Node*>, function<bool(Node*, Node*)>> beam(
            [](Node* a, Node* b) { return a->score < b->score; }
        );

        priority_queue<Node*, vector<Node*>, function<bool(Node*, Node*)>> TS(
            [](Node* a, Node* b) { return a->score < b->score; }
        );

        int dr[] = { -1,-1, 0, 1, 1, 1, 0,-1 };
        int dc[] = { 0, 1, 1, 1, 0,-1,-1,-1 };


        Node T0 = *root;
        Node* nsel = root;
        beam.push(root);

        int srow = nsel->row, scol = nsel->col;

        while (!beam.empty()) {

            for (int W = 0; W < BWIDTH && !beam.empty(); ++W) {
                //cerr << "beam size " << beam.size() << endl;
                nsel = beam.top();
                beam.pop();

                int row = nsel->row, col = nsel->col;
                //cerr << "gcell = " << row << " " << col << endl;
                int rs = row, cs = col;
                int dir = arrows[row][col];

                int ind_prec = 0;
                int glg = 0;
                while (glg < GLG && ind_prec < grid.body[rs][cs].prec.size()) {

                    //cerr << glg << "  " << ind_prec << " " << grid.body[rs][cs].prec.size() << endl;
                    row = grid.body[rs][cs].prec[ind_prec]->row;
                    col = grid.body[rs][cs].prec[ind_prec]->col;

                    //cerr << "cell = " << row << " " << col << endl;


                    if (row < 0 || row >= N || col < 0 || col >= N) {
                        //cerr << "break " << row << " " << col << endl;
                        break;
                    }
                    else {
                        if (nsel->used[row][col]) {
                            //cerr << "continue" << endl;
                            ind_prec++;
                            continue;

                        }
                    }



                    Node* new_node = new Node(row, col);
                    new_node->m = nsel->m + 1;
                    new_node->path_i = nsel->path_i;
                    new_node->path_i.push_back(Node(row, col));
                    new_node->path = nsel->path;
                    new_node->path.push_back(to_string(row) + " " + to_string(col));
                    //double e = exp(double(new_node->m) / 1000.0);
                    //new_node->score = nsel->score + (double)new_node->m * (double)mults[row][col];
                    new_node->score = nsel->score + (double)mults[row][col];// +(N - grid.body[row][col].next.size());
                    new_node->used = nsel->used;
                    new_node->used[row][col] = true;

                    //cerr << "add new node " << endl;
                    TS.push(new_node);

                    glg++;

                }
                //cerr << "end " << endl;

            }

            while (!beam.empty()) {
                delete beam.top();
                beam.pop();

            }
            beam.swap(TS);

            if (!beam.empty()) {
                if (beam.top()->score > T0.score)
                    T0 = *beam.top();
            }

            //cerr << "size = " << T0->path.size() << " " << T0->score << " " << maxt << endl;

        }

        anal_path.head = T0;
        for (int i = T0.path_i.size() - 1; i >= 0;--i) {
            anal_path.cell.push_back(CellList(T0.path_i[i].row, T0.path_i[i].col));
        }

        auto finalm = high_resolution_clock::now();;
        auto duration = duration_cast<milliseconds>(finalm - startm);
        //cerr << duration.count() << endl;
        MINUS_TIME = duration.count();


    }


    void BSG_between(Node* root, Node *end, int BWIDTH, ListPath &between_path) {

        auto startm = high_resolution_clock::now();;
        int maxt = -1;
        auto getTime = [&]()-> bool {
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(stop - startm);
            //cerr << duration.count() << endl;
            maxt = duration.count();
            return(duration.count() <= 1000);
        };


        int GLG = 3;

        // max inf
        priority_queue<Node*, vector<Node*>, function<bool(Node*, Node*)>> beam(
            [](Node* a, Node* b) { return a->score < b->score; }
        );

        priority_queue<Node*, vector<Node*>, function<bool(Node*, Node*)>> TS(
            [](Node* a, Node* b) { return a->score < b->score; }
        );

        int dr[] = { -1,-1, 0, 1, 1, 1, 0,-1 };
        int dc[] = { 0, 1, 1, 1, 0,-1,-1,-1 };


        Node T0 = *root;
        Node* nsel = root, *node_reach=nullptr;
        beam.push(root);
        bool reach_node = false;
        bool find = false;

        int srow = nsel->row, scol = nsel->col;

        while (!beam.empty() && getTime()) {

            for (int W = 0; W < BWIDTH && !beam.empty(); ++W) {
                //cerr << "beam size " << beam.size() << endl;
                nsel = beam.top();
                beam.pop();

                int row = nsel->row, col = nsel->col;
                int rs = row, cs = col;
                //cerr << "gcell = " << row << " " << col << " " << nsel->score << endl;
                int dir = arrows[row][col];

                int ind_prec = 0;
                int glg = 0;
                while (glg < GLG && ind_prec < grid.body[rs][cs].prec.size()) {

                    //cerr << glg << "  " << ind_prec << " " << grid.body[rs][cs].prec.size() << endl;
                    row = grid.body[rs][cs].prec[ind_prec]->row;
                    col = grid.body[rs][cs].prec[ind_prec]->col;
                    
                    //cerr << "cell = " << row << " " << col << endl;


                    if (row < 0 || row >= N || col < 0 || col >= N) {
                        //cerr << "break " << row << " " << col << endl;
                        break;
                    }
                    else {
                        if (row == end->row && col == end->col) {
                            reach_node = true;
                            find = true;
                        }
                        else if (nsel->used[row][col]) {
                            //cerr << "continue" << endl;
                            ind_prec++;
                            continue;

                        }
                    }



                    Node* new_node = new Node(row, col);
                    
                    new_node->path_i = nsel->path_i;
                    new_node->path_i.push_back(Node(row, col));
                    new_node->m = nsel->m + new_node->path_i.size();
                    //new_node->path = nsel->path;
                    //new_node->path.push_back(to_string(row) + " " + to_string(col));
                    //double e = exp(double(new_node->m) / 1000.0);

                    /*int score = 0;
                    int k = new_node->path_i.size();
                    for (int i = new_node->path_i.size() - 1; i >= 0; --i) {
                        score += k * (double)mults[new_node->path_i[i].row][new_node->path_i[i].col];
                        --k;
                    }*/
                    new_node->score = nsel->score + (double)mults[row][col];
                    //new_node->score = nsel->score + (double)new_node->m * (double)mults[row][col];
                    new_node->used = nsel->used;
                    new_node->used[row][col] = true;
                    if (reach_node) {
                        node_reach = new_node;
                        reach_node = false;
                        if (node_reach->score > T0.score) {
                            T0 = *node_reach;
                            find = true;
                        }
                    }
                    else {
                        TS.push(new_node);
                    }

                    glg++;
                    ind_prec++;

                }
                //cerr << "end " << endl;

            }

            while (!beam.empty()) {
                delete beam.top();
                beam.pop();

            }
            beam.swap(TS);

            /*if (!beam.empty() && reach_node) {
                if (node_reach->score > T0.score) {
                    T0 = *beam.top();
                    find = true;
                }
            }*/

            //cerr << "size = " << T0->path.size() << " " << T0->score << " " << maxt << endl;

        }

        if (find) {
            //cerr << "FIND " << T0.path_i.size()  << " t=" << maxt << endl;
            between_path.head = T0;
            between_path.score = T0.score;
            /*for (int i = T0.path_i.size() - 1; i >= 0; --i) {
                cerr << "--- " << T0.path_i[i].row << " " << T0.path_i[i].col << endl;
            }*/
            for (int i = T0.path_i.size() - 2; i > 0; --i) {
                between_path.cell.push_back(CellList(T0.path_i[i].row, T0.path_i[i].col));
            }
        }
        /*else {
            cerr << "NOT FIND " << " t=" << maxt << endl;
        }*/

    }

    void GetFinalPath() {

        int MAXT = 10000 - MINUS_TIME - 5;

        auto startm = high_resolution_clock::now();;
        int maxt = -1;
        auto getTime = [&]()-> bool {
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(stop - startm);
            //cerr << duration.count() << endl;
            maxt = duration.count();
            return(duration.count() <= MAXT);
        };

        // Random number generator
        std::random_device rd;  // Obtain a random number from hardware
        std::mt19937 gen(rd()); // Seed the generator

        //std::uniform_int_distribution<> distszb(1, anal_path.cell.size()/2);

        ListPath bet_path, rec_path;

        while (getTime()) {
            int sz_bet = 1; // distszb(gen);
            int MAXD = anal_path.cell.size() - (sz_bet+1);
            /*if (anal_path.cell.size() < 10) {
                sz_bet = anal_path.cell.size() / 2;
                MAXD = anal_path.cell.size() - anal_path.cell.size() / 2;
            }*/
           

            std::uniform_int_distribution<> distr(0, MAXD);
            int num = distr(gen);

            int r = anal_path.cell[num + sz_bet].row, c = anal_path.cell[num + sz_bet].col;
            Node* root = new Node(r, c);
            root->m = num + sz_bet;
            root->used = anal_path.head.used;
            root->used[r][c] = true;
            root->path_i.push_back(Node(r, c));

            int re = anal_path.cell[num].row, ce = anal_path.cell[num].col;
            Node* end = new Node(re, ce);


            int score_part = 0;

            for (int i = num; i <= num + sz_bet; ++i) {
                score_part += grid.body[anal_path.cell[i].row][anal_path.cell[i].col].mult;

            }

            root->score = arrows[r][c];
            end->score = score_part;

            bet_path.head = {};
            bet_path.cell = {};
            bet_path.score = 0;
            rec_path.head = {};
            rec_path.cell = {};

            //cerr << "sz " << anal_path.cell.size() << endl;
            //cerr << root->row << " " << root->col << " -- " << end->row << " " << end->col << endl;
            //cerr << "start between " << num << endl;
            BSG_between(root, end, 5, bet_path);
            //cerr << "end between" << endl;

            delete root;
            delete end;

            vector<vector<bool>> used = vector<vector<bool>>(N, vector<bool>(N, false));

            for (int i = 0; i <= num; ++i) {
                rec_path.cell.push_back(CellList(anal_path.cell[i].row, anal_path.cell[i].col));
                used[anal_path.cell[i].row][anal_path.cell[i].col] = true;
            }
            //rec_path.cell.push_back(CellList(end->row, end->col));

            if (bet_path.cell.size() > 0 && bet_path.score > score_part) {
                for (int i = 0; i < bet_path.cell.size(); ++i) {
                    rec_path.cell.push_back(CellList(bet_path.cell[i].row, bet_path.cell[i].col));
                    used[bet_path.cell[i].row][bet_path.cell[i].col] = true;
                }
            }
            else {
                for (int i = num + 1; i < num + sz_bet; ++i) {
                    rec_path.cell.push_back(CellList(anal_path.cell[i].row, anal_path.cell[i].col));
                    used[anal_path.cell[i].row][anal_path.cell[i].col] = true;
                }
            }

            for (int i = num + sz_bet; i < anal_path.cell.size(); ++i) {
                rec_path.cell.push_back(CellList(anal_path.cell[i].row, anal_path.cell[i].col));
                used[anal_path.cell[i].row][anal_path.cell[i].col] = true;
            }
            //rec_path.cell.push_back(CellList(root->row, root->col));

            anal_path.cell = rec_path.cell;
            anal_path.head.used = used;

        }

        cout << rec_path.cell.size() << endl;
        cerr << rec_path.cell.size() << endl;
        int I = 1;
        for (CellList c : rec_path.cell) {
            cerr << I << "= " << c.row << " " << c.col << endl;
            cout << c.row << " " << c.col << endl;
            ++I;
        }

        cout.flush();

        /*cout << anal_path.cell.size() << endl;
        for (CellList c : anal_path.cell) {
            cout << c.row << " " << c.col << endl;
        }

        cout.flush();*/


    }

    void BS_init(Node* root, int BWIDTH) {

        cerr << "init" << endl; 
        int GLG = 2;

        // max inf
        priority_queue<Node*, vector<Node*>, function<bool(Node*, Node*)>> beam(
            [](Node* a, Node* b) { return a->score < b->score; }
        );

        priority_queue<Node*, vector<Node*>, function<bool(Node*, Node*)>> TS(
            [](Node* a, Node* b) { return a->score < b->score; }
        );

        int dr[] = { -1,-1, 0, 1, 1, 1, 0,-1 };
        int dc[] = { 0, 1, 1, 1, 0,-1,-1,-1 };


        Node T0 = *root;
        Node* nsel = root;
        beam.push(root);

        int srow = nsel->row, scol = nsel->col;

        while (!beam.empty()) {

            for (int W = 0; W < BWIDTH && !beam.empty(); ++W) {
                //cerr << "beam size " << beam.size() << endl;
                nsel = beam.top();
                beam.pop();

                int row = nsel->row, col = nsel->col;
                //cerr << "gcell = " << row << " " << col << endl;
                int dir = arrows[row][col];

                int glg = 0;
                while ( glg < GLG) {

                    row += dr[dir];
                    col += dc[dir];

                    //cerr << "cell = " << row << " " << col << endl;


                    if (row < 0 || row >= N || col < 0 || col >= N) {
                        //cerr << "break " << row << " " << col << endl;
                        break;
                    }
                    else {
                        if (nsel->used[row][col]) {
                            //cerr << "continue" << endl;
                            continue;

                        }
                    }



                    Node* new_node = new Node(row, col);
                    new_node->m = nsel->m + 1;
                    new_node->path_i = nsel->path_i;
                    new_node->path_i.push_back(Node(row, col));
                    new_node->path = nsel->path;
                    new_node->path.push_back(to_string(row) + " " + to_string(col));
                    //double e = exp(double(new_node->m) / 1000.0);
                    new_node->score = nsel->score + (double)new_node->m * (double)mults[row][col];
                    new_node->used = nsel->used;
                    new_node->used[row][col] = true;

                    //cerr << "add new node " << endl;
                    TS.push(new_node);

                    glg++;

                }
                //cerr << "end " << endl;

            }

            while (!beam.empty()) {
                delete beam.top();
                beam.pop();

            }
            beam.swap(TS);

            if (!beam.empty()) {
                if (beam.top()->score > T0.score)
                    T0 = *beam.top();
            }

            //cerr << "size = " << T0->path.size() << " " << T0->score << " " << maxt << endl;

        }

        cout << T0.path.size() << endl;

        for (string m : T0.path) {
            cout << m << endl;
            //cerr << m << endl;
        }
        cout.flush();

    }
     
    void BS(Node *root, int BWIDTH) {

        auto startm = high_resolution_clock::now();;
        int maxt = -1;
        auto getTime = [&]()-> bool {
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(stop - startm);
            //cerr << duration.count() << endl;
            maxt = duration.count();
            return(duration.count() <= 9500);
        };

        int GLG = 3;
        int NB_MAX_EXEC = N * N;
        int NB_EXEC = 0;

        // max inf
        priority_queue<Node*, vector<Node*>, function<bool(Node*, Node*)>> beam(
            [](Node* a, Node* b) { return a->score < b->score; }
        );

        priority_queue<Node*, vector<Node*>, function<bool(Node*, Node*)>> TS(
            [](Node* a, Node* b) { return a->score < b->score; }
        );

        int dr[] = { -1,-1, 0, 1, 1, 1, 0,-1 };
        int dc[] = { 0, 1, 1, 1, 0,-1,-1,-1 };

       
        Node T0 = *root;
        Node* nsel = root;
        beam.push(root);

        int srow = nsel->row, scol = nsel->col;

        //cerr << srow << " " << scol << endl;

        while (getTime()) {

            while (!beam.empty() && getTime()) {
                                
                for (int W = 0;getTime() &&  W < BWIDTH && !beam.empty(); ++W) {
                    //cerr << "beam size " << beam.size() << endl;
                    nsel = beam.top();
                    beam.pop();

                    int row = nsel->row, col = nsel->col;
                    //cerr << "gcell = " << row << " " << col << endl;
                    int dir = arrows[row][col];

                    int glg = 0;
                    while (getTime() && glg < GLG) {

                        row += dr[dir];
                        col += dc[dir];

                        //cerr << "cell = " << row << " " << col << endl;


                        if (row < 0 || row >= N || col < 0 || col >= N) {
                            //cerr << "break " << row << " " << col << endl;
                            break;
                        }
                        else {
                            if (nsel->used[row][col]) {
                                //cerr << "continue" << endl;
                                continue;

                            }
                        }



                        Node* new_node = new Node(row, col);
                        new_node->m = nsel->m + 1;
                        new_node->path = nsel->path;
                        new_node->path.push_back(to_string(row) + " " + to_string(col));
                        //double e = exp(double(new_node->m) / 1000.0);
                        new_node->score = nsel->score + (double)new_node->m * (double)mults[row][col];
                        new_node->used = nsel->used;
                        new_node->used[row][col] = true;

                        //cerr << "add new node " << endl;
                        TS.push(new_node);

                        glg++;

                    }
                    //cerr << "end " << endl;

                }
                
                while (!beam.empty()) {
                    delete beam.top();
                    beam.pop();

                }
                beam.swap(TS);
                
                if (!beam.empty()) {
                    if (beam.top()->score > T0.score)
                        T0 = *beam.top();
                }

                //cerr << "size = " << T0->path.size() << " " << T0->score << " " << maxt << endl;

            }

            NB_EXEC++;
            if (NB_EXEC == NB_MAX_EXEC)break; 
            scol++;
            if (scol == N) {
                scol = 0;
                srow++;
                if (srow == N) {
                    srow = 0;
                }
            }
            //cerr << srow << " " << scol << endl;

            Node* root2 = new Node(srow, scol);
            root2->m = 1;
            root2->used = vector<vector<bool>>(N, vector<bool>(N, false));
            root2->used[srow][scol] = true;
            root2->path.push_back(to_string(srow) + " " + to_string(scol));
            root2->score = arrows[srow][scol];

            //priority_queue<Node*, vector<Node*>, function<bool(Node*, Node*)>> beamsw(
            //    [](Node* a, Node* b) { return a->score < b->score; }
            //);

            while (!beam.empty()) {
                delete beam.top();
                beam.pop();

            }
            //beam.swap(beamsw);
            beam.push(root2);

            //cerr << "action" << endl;



        }

        //cerr << "size = " << T0->path.size() << " " << T0->score << endl;
        cout << T0.path.size() << endl;
        
        for (string m : T0.path) {
            cout << m << endl;
            //cerr << m << endl;
        }
        cout.flush();


    }

    void BS2(Node* root, int BWIDTH) {

        auto startm = high_resolution_clock::now();;
        int maxt = -1;
        auto getTime = [&]()-> bool {
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(stop - startm);
            //cerr << duration.count() << endl;
            maxt = duration.count();
            return(duration.count() <= 9995);
        };

        priority_queue<Node*, vector<Node*>, function<bool(Node*, Node*)>> beam(
            [](Node* a, Node* b) { return a->score < b->score; }
        );

        int dr[] = { -1,-1, 0, 1, 1, 1, 0,-1 };
        int dc[] = { 0, 1, 1, 1, 0,-1,-1,-1 };

        Node* T0 = root;
        Node* nsel = root;
        beam.push(root);

        while (getTime() && !beam.empty()) {

            priority_queue<Node*, vector<Node*>, function<bool(Node*, Node*)>> TS(
                [](Node* a, Node* b) { return a->score < b->score; }
            );

            for (int W = 0; W < BWIDTH && !beam.empty(); ++W) {
                //cerr << "beam size " << beam.size() << endl;
                nsel = beam.top();
                beam.pop();

                int row = nsel->row, col = nsel->col;
                //cerr << "gcell = " << row << " " << col << endl;
                int dir = arrows[row][col];
                while (1) {

                    row += dr[dir];
                    col += dc[dir];

                    //cerr << "cell = " << row << " " << col << endl;


                    if (row < 0 || row >= N || col < 0 || col >= N) {
                        //cerr << "break " << row << " " << col << endl;
                        break;
                    }
                    else {
                        if (nsel->used[row][col]) {
                            //cerr << "continue" << endl;
                            continue;

                        }
                    }



                    Node* new_node = new Node(row, col);
                    new_node->m = nsel->m + 1;
                    new_node->path = nsel->path;
                    new_node->path.push_back(to_string(row) + " " + to_string(col));
                    new_node->score = nsel->score + new_node->m * mults[row][col];
                    new_node->used = nsel->used;
                    new_node->used[row][col] = true;

                    //cerr << "add new node " << endl;
                    TS.push(new_node);

                }


            }


            beam.swap(TS);


            if (!beam.empty()) {
                if (beam.top()->score > T0->score)
                    T0 = beam.top();
            }

        }


        cout << T0->path.size() << endl;
        //cerr <<"size = " << T0->path.size() << " " << T0->score << endl;
        for (string m : T0->path) {
            cout << m << endl;
            //cerr << m << endl;
        }
        cout.flush();


    }

    void rec(int r, int c, double score, int k, list<string> path, vector<vector<bool>> used, int lvl) {

        cerr << r << " " << c << " " << score << " " << lvl <<  endl;
        int dr[] = { -1,-1, 0, 1, 1, 1, 0,-1 };
        int dc[] = { 0, 1, 1, 1, 0,-1,-1,-1 };

        int glg = 0;
        int row = r, col = c;
        int dir = arrows[row][col];

        /*// Random number generator
        std::random_device rd;  // Obtain a random number from hardware
        std::mt19937 gen(rd()); // Seed the generator

        std::uniform_int_distribution<> distr(0, _GLG-1);
        int nsel = distr(gen);*/

        while (glg < _GLG) {

            row += dr[dir];
            col += dc[dir];

            //cerr << "cell = " << row << " " << col << endl;


            if (row < 0 || row >= N || col < 0 || col >= N) {
                //cerr << "break " << row << " " << col << endl;
                break;
            }
            else {
                if (used[row][col]) {
                    //cerr << "continue" << endl;
                    continue;

                }
            }

            //if (nsel == glg) {

                list<string> pth = path;
                pth.push_back(to_string(row) + " " + to_string(col));
                vector<vector<bool>> usd = used;
                usd[row][col] = true;
                double nscore = score + (k + 1) * mults[row][col];
                if (nscore > MAX_SCORE) {
                    MAX_SCORE = nscore;
                    PATH = pth;
                }
                rec(row, col, nscore, k + 1, pth, usd, lvl + 1);
            //}

            glg++;
        }

    }

    void run() {
        int r = 0, c = 0, k = 1;
        double score = arrows[r][c];
        MAX_SCORE = score;
        list<string> path;
        vector<vector<bool>> used = vector<vector<bool>>(N, vector<bool>(N, false));
        used[r][c] = true;
        path.push_back(to_string(r) + " " + to_string(c));
        PATH = path;

        rec(r, c, score, k, path, used, 0);

        cout << PATH.size() << endl;

        for (string m : PATH) {
            cout << m << endl;
            //cerr << m << endl;
        }
        cout.flush();

    }

};


int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    cin >> N;

    
    vector<vector<int>> arrows = vector<vector<int>>(N, vector<int>(N, 0));
    vector<vector<int>> mults = vector<vector<int>>(N, vector<int>(N, 0));
    vector<vector<bool>> used = vector<vector<bool>>(N, vector<bool>(N, false));
    list<Node> one_points, two_points, three_points, five_points;
    for (int r = 0; r < N; r++)
        for (int c = 0; c < N; c++)
        {
            cin >> arrows[r][c] >> mults[r][c];
            if (mults[r][c] == 1)one_points.push_back(Node(r, c));
            else if (mults[r][c] == 2)two_points.push_back(Node(r, c));
            else if (mults[r][c] == 3)three_points.push_back(Node(r, c));
            else if (mults[r][c] == 5)five_points.push_back(Node(r, c));

        }

    cerr << "grid start" << endl;
    Grid grid = Grid(N, arrows, mults);
    cerr << "grid end" << endl;

    int r = N/2, c = N/2;
    /*if (five_points.size() > 0) {
        r = five_points.begin()->row;
        c = five_points.begin()->col;
    }
    else if (three_points.size() > 0) {
        r = three_points.begin()->row;
        c = three_points.begin()->col;
    }
    else*/ if (two_points.size() > 0) {
        r = two_points.begin()->row;
        c = two_points.begin()->col;
    }

    Node* root = new Node(r, c);
    root->m = 1;
    root->used = vector<vector<bool>>(N, vector<bool>(N, false));
    root->used[r][c] = true;
    root->path.push_back(to_string(r) + " " + to_string(c));
    root->score = arrows[r][c];

    MM155 mm155(N, grid, arrows, mults, one_points, two_points, three_points, five_points);
    //mm155.BS(root, 10);
    //mm155.BS2(root, 2);
    cerr << "start " << endl;
    //mm155.BS_init(root, 2);
    mm155.BSG_initR(root, 2);
    mm155.GetFinalPath();

    /*

    list<string> moves;
    int dr[] = { -1,-1, 0, 1, 1, 1, 0,-1 };
    int dc[] = { 0, 1, 1, 1, 0,-1,-1,-1 };

    int r = N / 2;
    int c = N / 2;
    int dir = -1;

    while (true)
    {
        if (r < 0 || r >= N || c < 0 || c >= N) break;

        if (used[r][c])
        {
            r += dr[dir];
            c += dc[dir];
            continue;
        }

        string s = to_string(r) + " " + to_string(c);
        moves.push_back(s);
        used[r][c] = true;
        dir = arrows[r][c];
    }

    cout << moves.size() << endl;
    for (string m : moves) cout << m << endl;
    cout.flush();*/

    return 0;
}


// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Astuces pour bien démarrer : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtre Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.
