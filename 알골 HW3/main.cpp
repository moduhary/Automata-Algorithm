#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <ctime>

using namespace std;

void dfs_rev_mat(int a);
void dfs_rev_list(int a);
void dfs_rev_array(int a);
void dfs_orig_mat(int b);
void dfs_orig_list(int b);
void dfs_orig_array(int b);

bool *visited, *visited_2, *visited_3;
int *order, *leader, parent = 0;
int *order_2, *leader_2, *order_3, *leader_3;
int* adj_matrix, *adj_mat_rev;
vector<int> adj_arr_indexes, adj_arr_index_rev;
vector<int> adj_array, adj_array_rev, finish, finish_2, finish_3;
vector<vector<int>> adj_list, adj_list_rev;
int vertex_num = 0;

int main(int argc, char** argv) {

    int file_cnt = 0, line_cnt = 0, edge_cnt = 0, word_count = 0, index = 0, scc_cnt = 0, index2 = 0;
    int edge = 0;
    string line;
    stringstream args;

    clock_t start1, end1, start2, end2, start3, end3;

    vector<int> SCC_leader;

    if(argc == 1)
    {
        printf("usage: ./scc [input file lists]\n");
        printf("input file lists contains input files seperated with space bar.\n");
        exit(0);
    }

    while(file_cnt < argc - 1) {
        file_cnt++;

        ifstream file;

        file.open(argv[file_cnt], ifstream::in);

        if(file.fail())
            continue;

        //TODO: Read input file and construct graph.

        file >> vertex_num;
        getline(file, line);

        adj_mat_rev = (int*)calloc(vertex_num * vertex_num, sizeof(int));
        adj_matrix = (int*)calloc(vertex_num * vertex_num ,sizeof(int));
        adj_arr_indexes.clear(); //(int*)calloc(vertex_num, sizeof(int));
        adj_arr_index_rev.clear(); //(int*)calloc(vertex_num, sizeof(int));
        adj_list = vector<vector<int>>(vertex_num);
        adj_list_rev = vector<vector<int>>(vertex_num);
        adj_array.clear();
        adj_array_rev.clear();

        line_cnt = 0;
        word_count = 0;

        while(!file.eof())
        {
            getline(file, line);

            if(line == "")
                break;

            args = stringstream(line);
            edge_cnt = 0;

            args >> edge;

            if(edge == 0) {
                adj_list[line_cnt].push_back(-1);
                adj_arr_indexes.push_back(-1);
                line_cnt++;
                continue;
            }

            adj_list[line_cnt] = vector<int>(edge);
            adj_arr_indexes.push_back(word_count);

            while(args >> edge)
            {
                adj_list[line_cnt][edge_cnt] = edge-1;
                adj_list_rev[edge-1].push_back(line_cnt);
                adj_matrix[line_cnt * vertex_num + edge-1] = 1;
                adj_mat_rev[(edge-1)*vertex_num + line_cnt] = 1;
                adj_array.push_back(edge-1);

                edge_cnt++;
                word_count++;
            }

            line_cnt++;
        }

        adj_arr_index_rev.push_back(0);
        int curr = 0;

        for(index = 1; index < (int)adj_list_rev.size(); index++)
        {
            adj_arr_index_rev.push_back(adj_arr_index_rev[index-1] + adj_list_rev[index-1].size());

            if(adj_list_rev[index-1].empty())
                adj_arr_index_rev[index-1] = -1;
        }

        for(index = 0; index < adj_list_rev.size(); index++)
        {
            for(index2 = 0; index2 < adj_list_rev[index].size(); index2++)
                adj_array_rev.push_back(adj_list_rev[index][index2]);
        }

        /*
        for(index = 0; index < adj_array.size(); index++)
            cout << adj_array[index] << " ";
        cout << endl;

        for(index = 0; index < adj_array_rev.size(); index++)
            cout << adj_array_rev[index] << " ";
        cout << endl;

        for(index = 0; index < adj_list.size(); index++)
            for(index2 = 0; index2 < adj_list[index].size(); index2++)
                cout << adj_list[index][index2] << " ";
        cout << endl;

        for(index = 0; index < adj_list_rev.size(); index++)
            for(index2 = 0; index2 < adj_list_rev[index].size(); index2++)
                cout << adj_list_rev[index][index2] << " ";
        cout << endl;

        cout << endl;
         */

        //TODO: Implement algorithm.

        vector<vector<int>> check = adj_list;

        visited = (bool*)calloc(adj_list.size(), sizeof(bool));
        visited_2 = (bool*)calloc(adj_list.size(), sizeof(bool));
        visited_3 = (bool*)calloc(adj_list.size(), sizeof(bool));

        order = (int*)calloc(adj_list.size(), sizeof(int));
        order_2 = (int*)calloc(adj_list.size(), sizeof(int));
        order_3 = (int*)calloc(adj_list.size(), sizeof(int));

        leader = (int*)calloc(adj_list.size(), sizeof(int));
        leader_2 = (int*)calloc(adj_list.size(), sizeof(int));
        leader_3 = (int*)calloc(adj_list.size(), sizeof(int));

        //by Matrix.

        start1 = clock();

        memset(visited, false, sizeof(bool)*adj_list.size());

        for(index = 0; index < adj_list.size(); index++)
        {
            if(!visited[index])
                dfs_rev_mat(index);
        }

        memset(visited, false, sizeof(bool)*adj_list.size());

        /*
        for(index = 0; index < finish.size(); index++)
            cout << finish[index] << " ";
        cout << endl;
        */

        curr = 0;

        scc_cnt = 0;
        for(index = 0; index < (int)adj_list.size(); index++)
        {
            curr = finish.back();
            finish.pop_back();

            if(!visited[curr])
            {
                parent = curr;
                dfs_orig_mat(curr);
                scc_cnt++;
            }
        }

        end1 = clock();

        //by List.

        start2 = clock();
        memset(visited_2, false, sizeof(bool)*adj_list.size());
        parent = 0;

        for(index = 0; index < adj_list.size(); index++)
        {
            if(!visited_2[index])
                dfs_rev_list(index);
        }

        /*
        for(index = 0; index < finish_2.size(); index++)
            cout << finish_2[index] << " ";
        cout << endl;
        */

        memset(visited_2, false, sizeof(bool)*adj_list.size());

        scc_cnt = 0;
        for(index = 0; index < (int)adj_list.size(); index++)
        {
            curr = finish_2.back();
            finish_2.pop_back();

            if(!visited_2[curr])
            {
                parent = curr;
                dfs_orig_list(curr);
                scc_cnt++;
            }
        }

        end2 = clock();

        //by Array.
        start3 = clock();
        memset(visited_3, false, sizeof(bool)*adj_list.size());
        parent = 0;

        for(index = 0; index < adj_list.size(); index++)
        {
            if(!visited_3[index])
                dfs_rev_array(index);
        }

        /*
        for(index = 0; index < finish_3.size(); index++)
            cout << finish_3[index] << " ";
        cout << endl;
        */

        memset(visited_3, false, sizeof(bool)*adj_list.size());

        scc_cnt = 0;
        for(index = 0; index < (int)adj_list.size(); index++)
        {
            curr = finish_3.back();
            finish_3.pop_back();

            if(!visited_3[curr])
            {
                parent = curr;
                dfs_orig_array(curr);
                scc_cnt++;
            }
        }

        end3 = clock();

        cout << endl;

        //TODO: print result & time;
        int next;
        index = 0;
        memset(visited, false, sizeof(bool)*adj_list.size());

        cout << "result of " << argv[file_cnt] << " by Matrix."<< endl;

        while(true) {
            next = (int)adj_list.size();
            for (index2 = index; index2 < adj_list.size(); index2++) {
                if(leader[index] == leader[index2])
                {
                    visited[index2] = true;
                    cout << index2+1 << " ";
                }
                else
                {
                    if(next > index2 && !visited[index2])
                        next = index2;
                }
            }
            cout << endl;

            if(next == adj_list.size())
                break;

            index = next;
        }

        cout << "Elapsed time: " << double(end1-start1) / CLOCKS_PER_SEC * 1000 << " ms" << endl;

        cout << endl;

        memset(visited_2, false, sizeof(bool)*adj_list.size());
        index = 0;

        cout << "result of " << argv[file_cnt] << " by List."<< endl;

        while(true) {
            next = (int)adj_list.size();
            for (index2 = index; index2 < adj_list.size(); index2++) {
                if(leader_2[index] == leader_2[index2])
                {
                    visited_2[index2] = true;
                    cout << index2+1 << " ";
                }
                else
                {
                    if(next > index2 && !visited_2[index2])
                        next = index2;
                }
            }
            cout << endl;

            if(next == adj_list.size())
                break;

            index = next;
        }

        cout << "Elapsed time: " << double(end2-start2) / CLOCKS_PER_SEC * 1000 << " ms" << endl;

        cout << endl;

        memset(visited_3, false, sizeof(bool)*adj_list.size());
        index = 0;

        cout << "result of " << argv[file_cnt] << " by Array."<< endl;

        while(true) {
            next = (int)adj_list.size();
            for (index2 = index; index2 < adj_list.size(); index2++) {
                if(leader_3[index] == leader_3[index2])
                {
                    visited_3[index2] = true;
                    cout << index2+1 << " ";
                }
                else
                {
                    if(next > index2 && !visited_3[index2])
                        next = index2;
                }
            }
            cout << endl;

            if(next == adj_list.size())
                break;

            index = next;
        }

        cout << "Elapsed time: " << double(end3-start3) / CLOCKS_PER_SEC * 1000 << " ms" << endl;

        cout << endl;

        file.close();
        //free(adj_arr_indexes);
        //free(adj_arr_index_rev);
        free(adj_matrix);
        free(adj_mat_rev);
        free(visited);
        free(visited_2);
        free(visited_3);
        free(order);
        free(order_2);
        free(order_3);
        free(leader);
        free(leader_2);
        free(leader_3);
    }

    return 0;
}

void dfs_rev_mat(int a)
{
    visited[a] = true;
    int index = 0;

    for(index = 0; index < vertex_num; index++)
    {
        if(!visited[index] && adj_mat_rev[a * vertex_num + index] == 1)
            dfs_rev_mat(index);
    }

    finish.push_back(a);
}

void dfs_rev_list(int a)
{
    visited_2[a] = true;
    int index = 0;

    for(index = 0; index < (int)adj_list_rev[a].size(); index++)
    {
        if(!visited_2[adj_list_rev[a][index]])
            dfs_rev_list(adj_list_rev[a][index]);
    }

    finish_2.push_back(a);
}

void dfs_rev_array(int a)
{
    visited_3[a] = true;
    int index = 0;

    for(index = 0; index < (int)adj_list_rev[a].size(); index++)
    {
        if(!visited_3[adj_array_rev[adj_arr_index_rev[a] + index]])
            dfs_rev_array(adj_array_rev[adj_arr_index_rev[a] + index]);
    }

    finish_3.push_back(a);
}

void dfs_orig_mat(int b)
{
    visited[b] = true;
    leader[b] = parent;

    int index = 0;

    for(index = 0; index < vertex_num; index++)
    {
        if(adj_matrix[b * vertex_num + index] == 1 && !visited[index])
            dfs_orig_mat(index);
    }
}

void dfs_orig_list(int b)
{
    visited_2[b] = true;
    leader_2[b] = parent;

    int index = 0;

    for(index = 0; index < adj_list[b].size(); index++)
    {
        if(adj_list[b][index] != -1)
            if(!visited_2[adj_list[b][index]])
                dfs_orig_list(adj_list[b][index]);
    }
}

void dfs_orig_array(int b)
{
    visited_3[b] = true;
    leader_3[b] = parent;

    int index = 0;

    if(adj_list[b][0] != -1)
    {
        for(index = 0; index < adj_list[b].size(); index++) {
            if (!visited_3[adj_array[adj_arr_indexes[b] + index]])
                dfs_orig_array(adj_array[adj_arr_indexes[b] + index]);
        }
    }
}