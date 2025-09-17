#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <map>
#include <cctype>

using namespace std;

const int state_matrix[4][5] = {{1,2,3,0},
    {1,4,4,4},
    {4,2,4,4},
    {4,4,3,4}
};

const bool advance_matrix[4][5] = {{true,true,true, true},
    {true,false,false, false},
    {false,true,false, false},
    {false, false, true, false}
};

const bool accept_state[5] = {false, false, false, false, true};

int dfa_state = 0;

int isReserved(string word)
{
    map<string, int> reserved_words;

    reserved_words["int"] = 1;
    reserved_words["return"] = 1;
    reserved_words["if"] = 1;
    reserved_words["while"] = 1;
    reserved_words["void"] =1;

    if(reserved_words.find(word) != reserved_words.end())
    {
        return 1;

    }
    else
    {
        return -1;
    }
}
/* programa para ordenação por seleção de
   uma matriz com dez elementos. */

int charType(char c)
{
    regex letter("^[A-Za-z]$");
    regex number("^[0-9]$");

    string s(1, c);

    if (isspace(c))
        return 3;
    else if (regex_match(s, letter))
        return 0;
    else if (regex_match(s, number))
        return 1;
    else if (string(";+\\-*/={}[]().").find(c) != string::npos)
        return 2;
    else
        return 4;



}

int dfa(ifstream &file)
{
    string buffer;
    int char_type;
    char c;

    while(file.get(c))
    {
        char_type = charType(c);
        dfa_state = state_matrix[dfa_state][char_type];
        cout << dfa_state;

        if(accept_state[dfa_state] == false)
        {
            switch(char_type)
            {
            case 0:
                buffer += c;
                break;
            case 1:
                buffer += c;
                break;
            case 2:
                cout << c;
                break;
            case 3:
                buffer += c;
                break;
            }
        }
        else
        {
            switch(char_type)
            {
            case 0:
                if(isReserved(buffer) == 1)
                {
                    cout << buffer;
                }
                else
                {
                    cout << "ID";
                }
                break;
            case 1:
                cout << "NUM ";
                break;
            case 2:
                cout << buffer;
                break;
            case 3:
                cout << buffer;
                break;

            }
            dfa_state = 0;
            buffer.clear();
            file.unget();
        }
    }

    /*
        while(file.get(c))
        {
            char_type = charType(c);

            switch(char_type)
            {
            case 0:
                if(advance_matrix[dfa_state][char_type] == true)
                {
                    dfa_state = state_matrix[dfa_state][char_type];
                    buffer+=c;
                }
                else
                {
                    dfa_state = 0;
                    if(isReserved(buffer) == 1)
                    {
                        cout << buffer;
                        buffer.clear();

                    }
                    else
                    {
                        cout << "ID ";
                        buffer.clear();

                    }

                }
                break;
            case 1:
                if(advance_matrix[dfa_state][char_type] == true)
                {
                    dfa_state = state_matrix[dfa_state][char_type];
                    cout << c;
                }
                else
                {
                    dfa_state = 0;
                }
                break;
            case 2:
                if(advance_matrix[dfa_state][char_type] == true)
                {
                    dfa_state = state_matrix[dfa_state][char_type];
                    cout << c;

                }
                else
                {
                    dfa_state = 0;
                    cout << c;
                }
                break;
            case 3:
                dfa_state = 0;
                cout << c;
                break;



            }



        }
    */


}








int main(void)
{
    ifstream file("sort.txt");
    dfa(file);

    file.close();

    return 0;
}
