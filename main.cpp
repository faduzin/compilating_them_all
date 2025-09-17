#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <map>
#include <cctype>

#define INICIO 0;

using namespace std;

const int state_matrix[4][5] =
{
    {1,2,3,0},//estado 0
    {1,4,4,4},//estado 1
    {4,2,4,4},//estado 2
    {4,4,3,4} //estado 3
};


const bool advance_matrix[5][6] =
{
    {true,true,true, true},//estado 0 (INICIAL)
    {true,true,true, true},//estado 1 (LETRAS)
    {true,true,true, true},//estado 2 (NUMEROS)
    {true, true, true, true},//estado 3 (ESPECIAIS)
    {false, false, false, false}//estado 4 (FINAL)
};


const bool accept_state[5] = {false, false, false, false, true};
int dfa_state = 0;
char previous_char;

/*
int isReserved(string word)
{
    map<string, int> reserved_words;

    reserved_words["int "] = 1;
    reserved_words["return "] = 1;
    reserved_words["if "] = 1;
    reserved_words["while "] = 1;
    reserved_words["void "] =1;

    if(reserved_words.find(word) != reserved_words.end())
    {
        return 1;

    }
    else
    {
        return -1;
    }
}*/

bool isReserved(string word)
{
    string reserved_words[6] = {"int", "return", "if", "while", "void"};
    for(int i = 0; i < 6; i++)
    {
        if(word == reserved_words[i]){return true;}
    }
    return false;
}


int charType(char c)
{
    regex letter("^[A-Za-z]$");
    regex number("^[0-9]$");

    string s(1, c);

    if (regex_match(s, letter))
        return 0;
    else if (regex_match(s, number))
        return 1;
    else if (string(";+\\-*/={}[]().,<>").find(c) != string::npos)
        return 2;
    else if (isspace(c))
        return 3;
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
        char_type = charType(c); // =2 //=0
        dfa_state = state_matrix[dfa_state][char_type]; // m[0][2] ESTADO 3 // sm[3][0]

        if(accept_state[dfa_state] == false)
        {
            previous_char = c;
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
                cout << c;
                break;
            }


        } else{
            //cout << "Buffer: " << buffer << "| ";
            switch(charType(previous_char))
            {
            case 0:
                if(isReserved(buffer) == true)
                {cout << buffer;}else
                {cout << "ID";}
                break;
            case 1:
                cout << buffer;
                break;
            case 2:
                //cout << c;
                break;

            }
            file.unget();
            dfa_state = INICIO;
            buffer="";
        }

    }

}

int main(void)
{
    ifstream file("sort.txt");
    dfa(file);

    file.close();

    return 0;
}
