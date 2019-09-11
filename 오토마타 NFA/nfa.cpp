#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <set>
//#include "elements.cpp"

//struct next_list: a structure for nfa single state. can have at most 2 next states.
struct next_list{
    struct next_list* next_state;
    struct next_list* next_state2;
    char shift_char;
    char shift_char2;
    int final_state;
};

class nfa {

private:
    std::vector<next_list*> nfa_table;
    std::string regex_string;
    std::string postfix;
    struct next_list* start_state;
    struct next_list* end_state;


public:

    nfa()
    {
        regex_string = "";
        start_state = new next_list();
        end_state = new next_list();
        nfa_table.assign(sizeof(next_list*),nullptr);
    }

    explicit nfa(std::string regex) //constructor
    {
        regex_string = regex;
    }

    ~nfa()//destructor
    {
        delete start_state;
        delete end_state;

        start_state = NULL;
        end_state = NULL;
    }

    void set_regex_string(std::string in)
    {
        regex_string = in;
        postfix = "";
    }
    struct next_list* get_start_state()
    {
        return start_state;
    }

    std::string get_post_str()
    {
        return postfix;
    }

    void make_postfix() // construct a postfix expression of given regex string.
    {
        int i=0;
        std::string init("0");
        std::string temp_char; //used for temporary saving space for operator of postfix string.
        //std::string temp_postfix; //used for temporary saving space for postfix string.
        std::vector<std::string> opstack; //stack for postfix operator.

        if(regex_string.empty()) //for debug. check if regex string is empty.
        {
            std::cout << "regex string empty!\n";
            return;
        }

        for(i=0;i<regex_string.length();i++) //go through entire string.
        {
            switch(regex_string.at((unsigned long long)i))
            {
                case '(': //'(' just skip.
                    break;
                case ')': //')' end of parenthesis. need to get an operator.
                    temp_char = opstack.at(opstack.size()-1);
                    opstack.pop_back();
                    postfix += temp_char;
                    break;
                case '0': //'0' char 0. just append it to back of postfix.n
                    if(postfix.empty())
                        postfix = init;
                    else
                        postfix += "0";
                    break;
                case '1': //'1' char 1. just append it to back of postfix.
                    init = "1";
                    if(postfix.empty())
                        postfix = init;
                    else
                        postfix += "1";
                    break;
                case '.': //operators. Have '.','+','*'. need to be pushed into stack when encountered.
                    opstack.emplace_back(".");
                    break;
                case '+':
                    opstack.emplace_back("+");
                    break;
                case '*':
                    opstack.emplace_back("*");
                    break;
                default: // for debug. check if regex_string has undefined characters.
                    std::cout << "regex invalid string!\n";
                    break;
            }
        }

        postfix += '\0';
    }

    void make_nfa() // construct an nfa from a postfix regex string.
    {
        int i=0;
        struct next_list *curr, *next; //used for '*','+'. start, end state.
        struct next_list* temp_start; //used for all cases. have 1 transition's start state.
        struct next_list* temp_end; //used for all cases. have 1 transition's end state.
        struct next_list* temp_start2; //used for many cases. have state 1 another transition's start state.
        struct next_list* temp_end2; //used for many cases. have 1 another transition's end state.

        struct next_list* arr[2];

        if(postfix.empty()&&regex_string.empty()) //for debug. check if no prerequsites are satisfied.
        {
            std::cout << "No string to make regular language!\n";
            return;
        }

        for(i=0;i<postfix.size();i++) //go through entire string.
        {
            if(postfix[i]=='0')
            {
                temp_end = new next_list();
                temp_start = new next_list();

                temp_start -> shift_char = '0';
                temp_start -> next_state = temp_end;

                temp_start -> final_state = 0;
                temp_end -> final_state = 1;

                if(nfa_table.empty()) {
                    nfa_table.insert(nfa_table.begin(), temp_start);
                    nfa_table.emplace_back(temp_end);
                }
                else
                {
                    nfa_table.emplace_back(temp_start);
                    nfa_table.emplace_back(temp_end);//constructed transition has to be remebered before finishing making nfa.
                }
            }
            else if(postfix[i]=='1')
            {
                temp_end = new next_list();
                temp_start = new next_list();

                temp_start -> shift_char = '1';
                temp_start -> next_state = temp_end;

                temp_start -> final_state = 0;
                temp_end -> final_state = 1;

                if(nfa_table.empty()) {
                    nfa_table.insert(nfa_table.begin(), temp_start);
                    nfa_table.emplace_back(temp_end);
                }
                else
                {
                    nfa_table.emplace_back(temp_start);
                    nfa_table.emplace_back(temp_end);//constructed transition has to be remebered before finishing making nfa.
                }
            }
            else if(postfix[i]=='*')
            {
                //saved transition.
                temp_start = nfa_table.at(nfa_table.size()-2);
                temp_end = nfa_table.at(nfa_table.size()-1);
                nfa_table.pop_back();
                nfa_table.pop_back();

                //curr is the new start state, next is the new final state.
                curr = new next_list();
                next = new next_list();

                next -> final_state = 1;

                //curr's next state can be both next or the transition's starting state.
                curr -> next_state = temp_start;
                curr -> shift_char ='e';
                curr -> next_state2 = next;
                curr -> shift_char2 ='e';
                curr -> final_state = 0;

                //saved transition's final state has 2 next states which are curr and next.
                temp_end -> next_state = curr;
                temp_end -> shift_char = 'e';
                temp_end -> next_state2 = next;
                temp_end -> shift_char2 = 'e';
                temp_end -> final_state = 0;

                nfa_table.emplace_back(curr);
                nfa_table.emplace_back(next);//constructed transition has to be remebered before finishing making nfa.

            }
            else if(postfix[i]=='+')
            {
                //transition 1
                temp_start = nfa_table.at(nfa_table.size()-2);
                temp_end = nfa_table.at(nfa_table.size()-1);
                nfa_table.pop_back();
                nfa_table.pop_back();

                //transition 2
                temp_start2 = nfa_table.at(nfa_table.size()-2);
                temp_end2 = nfa_table.at(nfa_table.size()-1);
                nfa_table.pop_back();
                nfa_table.pop_back();

                //constructing new transition using curr as starting point. curr has 2 next states, each of them is starting state of transition 1 and 2.
                curr = new next_list();
                curr -> shift_char = 'e';
                curr -> shift_char2 = 'e';
                curr -> next_state = temp_start;
                curr -> next_state2 = temp_start2;
                curr -> final_state = 0;

                // 2 saved transitions' final state should remap the next state has next.
                next = new next_list();
                temp_end -> shift_char = 'e';
                temp_end2 -> shift_char = 'e';
                temp_end -> next_state = next;
                temp_end2 -> next_state = next;

                temp_end -> final_state = 0;
                temp_end2 -> final_state = 0;

                next -> final_state = 1;

                nfa_table.emplace_back(curr);
                nfa_table.emplace_back(next);//constructed transition has to be remebered before finishing making nfa.

            }
            else if(postfix[i] == '.')
            {
                //transition 1
                temp_start = nfa_table.at(nfa_table.size()-2);
                temp_end = nfa_table.at(nfa_table.size()-1);
                nfa_table.pop_back();
                nfa_table.pop_back();

                //transition 2
                temp_start2 = nfa_table.at(nfa_table.size()-2);
                temp_end2 = nfa_table.at(nfa_table.size()-1);
                nfa_table.pop_back();
                nfa_table.pop_back();

                //concatenate transition 2 at the back of transition 1 with shift character 'epsilon'.
                temp_end2 -> next_state = temp_start;
                temp_end2 -> shift_char = 'e';

                temp_end2 -> final_state = 0;
                temp_end -> final_state = 1;

                nfa_table.emplace_back(temp_start2);
                nfa_table.emplace_back(temp_end);//constructed transition has to be remebered before finishing making nfa.

            }

        }

        //finished constructing nfa, saved the starting state and end state to each member variables.
        start_state = nfa_table.at(nfa_table.size()-2);
        end_state = nfa_table.at(nfa_table.size()-1);
    }

    //check if the input string satisfies the regular expression rule. (Recursive function)
    bool accept(std::string input, struct next_list* check, int index)
    {
        char c;
        bool res = false;
        struct next_list *j = start_state, *k;
        std::set <next_list*> currset, tempset, nextset;
        struct next_list* tempstate = start_state;
        int i=0, i2=0;

        currset.insert(start_state);

        for (i=0;i<input.size();i++)
        {
            for(j: currset)
            {
                tempset.insert(j);

                if(j -> shift_char == 'e' || j -> shift_char2 == 'e')
                {
                    get_all_set(j,&tempset);
                }
            }

            currset.clear();
            currset = tempset;
            tempset.clear();

            for(j: currset) {

                if (j->next_state == nullptr)
                {
                    continue;
                }
                else
                {
                    if(j->shift_char == input[i])
                    {
                        nextset.insert(j->next_state);
                    }

                    if(j->shift_char2 == input[i])
                    {
                        nextset.insert(j->next_state2);
                    }
                }
            }

            if(nextset.empty())
            {
                return false;
            }

            currset.clear();
            currset = nextset;
            nextset.clear();
        }

        for(j: currset)
        {
            tempset.insert(j);

            if(j -> shift_char == 'e' || j -> shift_char2 == 'e')
            {
                get_all_set(j,&tempset);
            }
        }

        currset.clear();
        currset = tempset;
        tempset.clear();

        for(j: currset)
        {
            if(j->final_state == 1)
                return true;
        }

        return false;
    }

    void get_all_set(struct next_list* curr, std::set<next_list*>* temp)
    {

        if(curr->shift_char=='e')
        {
            temp->insert(curr->next_state);
            get_all_set(curr->next_state,temp);
        }

        if(curr->shift_char2=='e')
        {
            temp->insert(curr->next_state2);
            get_all_set(curr->next_state2,temp);
        }

    }
};