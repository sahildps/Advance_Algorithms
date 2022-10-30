#include <climits>
#include <algorithm>
#include <queue>
#include <math.h>
#include <vector>
#include <iostream>
#include <string>
#include <utility>
#include <map>
#include <iomanip>
#include <ctime>
#include <cmath>
using namespace std;

#ifndef PATTERN_H
#define PATTERN_H

class Pattern
{
public:
    Pattern();                 // default
    Pattern(const Pattern &S); // param
    void lsType(string s);     // arrange elements as L type and S type
    void printlsType();
    void buildSA(string s);                     // calling all fnction to build SA
    void match_check(string s, string pattern); // main function
    void markLMS(vector<string> db, string s);  // compute LMS string
    void printLMS();                            // to print LMS i.e. stringLMS
    void calBucket(string S);
    void sort_unique(string S); // sort, store unique elements and count of each char
    void SE(string S);          // to store starting and ending indices
    void sufffix_sort_T1(string s);
    static bool str_cmp(string s1, string s2); // place the indices in correct position
    vector<int> finalSuffix(string S);
    vector<int> patternCheckResult(string s, string p);

private:
    int size;
    vector<string> Type;          // store if L-type or S-type
    vector<int> typeLMS;          // LMS * type
    vector<string> stringLMS;     // store LMS string
    vector<char> sorted_elements; // elemnts stored in sorted order size = sorted_elements.size()
    map<char, int> elementCount;  // to store count of each char in string S
    // vector<int> startPos;         // to store start pos
    // vector<int> endPos;           // to store end pos
    map<char, int> startpos; // to store start pos from left to right
    map<char, int> endpos;   // to store end pos from right to left
    vector<int> T;
    vector<int> SA1;
    vector<int> finalSuffixarray; // store final suffix array
};

#endif
Pattern::Pattern()
{
}

// checek if L-type or S-type
void Pattern::lsType(string s)
{
    // int size = s.size();

    // intialize type vector with empty string
    for (int i = 0; i < size; i++)
    {
        Type.push_back(" ");
    }

    // intialize sentinel character with S-type
    Type[size - 1] = "S";
    string cur = "";
    for (int i = size - 1; i >= 0; i--)
    {
        if (i == 0)
        {
            Type[i] = cur;
        }
        else if (s[i] < s[i - 1])
        {
            Type[i - 1] = "L";
            cur = "L";
        }
        else if (s[i] > s[i - 1])
        {
            Type[i - 1] = "S";
            cur = "S";
        }
        else
        {
            Type[i - 1] = Type[i];
            cur = Type[i];
        }
    }

    for (int i = 0; i < size - 1; i++)
    {
        typeLMS.push_back(0);
    }

    for (int i = 0; i < size - 1; i++)
    {
        if (Type[i] == "L" && Type[i + 1] == "S")
        {
            typeLMS[i + 1] = 1;
        }
    }
}

void Pattern::printlsType()
{
    for (int i = 0; i < (int)Type.size(); i++)
    {
        cout << Type[i] << " ";
    }
    cout << endl;
    for (int i = 0; i < (int)Type.size(); i++)
    {
        cout << typeLMS[i] << " ";
    }

    cout << endl;
}

void Pattern::markLMS(vector<string> db, string s)
{
    stringLMS.resize(size);
    // cout << stringLMS.size()<<endl;

    int count = 0;
    // int cf = 0;
    int index = INT_MIN, flag = 0;
    string temp = "";
    stringLMS[size - 1] = "$";

    for (int i = 0; i < size; i++)
    {
        if (typeLMS[i] == 1)
        {

            flag = 1;
            count += 1;

            if (count == 2 && flag)
            {
                temp += s[i];
                stringLMS[index] = temp;
                temp = "";
                count = 1;
                flag = 1;
            }

            if (count == 1 && flag)
            {
                temp += s[i];
                index = i;
            }
        }
        else
        {
            if (flag)
            {
                temp += s[i];
            }
        }
    }
}

void Pattern::printLMS()
{

    for (int i = 0; i < size; i++)
    {
        if (typeLMS[i] == 1)
        {
            cout << Type[i] << " - " << stringLMS[i] << " " << i << endl;
        }
    }
    cout << endl;
}

void Pattern::sort_unique(string S)
{
    map<char, int> unique_elements;

    for (int i = 0; i < size; i++)
    {
        if (unique_elements.empty())
        {
            // cout << i << endl;
            unique_elements.insert({S[i], i});
            elementCount.insert({S[i], 0});
        }
        else
        {
            // cout <<" dd " <<(db.find(s[i]) !=db.end()) <<endl;
            if (unique_elements.find(S[i]) == unique_elements.end())
            {
                // cout <<"d "<<i <<endl;
                unique_elements.insert({S[i], i});
                elementCount.insert({S[i], 0});
            }
        }
    }

    for (auto m : unique_elements)
    {
        sorted_elements.push_back(m.first);
    }

    // re-iterate to store count
    for (int i = 0; i < size; i++)
    {
        auto it = elementCount.find(S[i]);
        it->second += 1;
    }
}

void Pattern::SE(string S)
{
    // startPos.push_back(0);
    // auto sentinel = elementCount.find(S[size - 1]);
    int i = 0, j = 0;
    // startpos.insert({sentinel->first, i});

    for (auto a : elementCount)
    {
        //     startPos.push_back(startPos[i] + a.second);
        // cout << a.first<<" "<<a.second <<endl;
        startpos.insert({a.first, i});
        i += a.second;
        j++;
        if (j == sorted_elements.size())
        {
            break;
        }
    }
    // cout << startpos.size() << endl;
    // for (auto l : startpos)
    // {
    //     cout << l.first << " - " << l.second << endl;
    // }

    i = size - 1, j = 0;

    for (auto it = elementCount.rbegin(); it != elementCount.rend(); it++)
    {
        // endPos[j - 1] = endPos[j] - it->second;
        endpos.insert({it->first, i});
        i -= it->second;
        j++;
        if (j == sorted_elements.size())
        {
            break;
        }
    }

    // for (auto c:endpos)
    // {
    //     cout << c.first<<" "<<c.second <<endl;
    // }
}

void Pattern::calBucket(string S)
{
    sort_unique(S);
    SE(S);

    vector<int> cbuckSA; // mark c-buckets
    cbuckSA.resize(size);
    map<char, int> tempStartpos;
    tempStartpos = startpos;
    map<char, int> tempEndpos;
    tempEndpos = endpos;

    vector<int> nextStartPos;
    // nextStartPos.resize(startPos.size());

    for (int i = 0; i < size; i++)
    {
        cbuckSA[i] = -1;
    }

    for (int i = size - 1; i >= 0; i--)
    {
        // cout << Type[i] <<" "<<typeLMS[i] <<endl;
        // cout <<i<<endl;
        if (Type[i] == "S" && typeLMS[i] == 1)
        {
            auto ins = tempEndpos.find(S[i]);
            // cout <<i<<" "<< ins->second<<endl;
            cbuckSA[ins->second] = i;
            ins->second -= 1;
            // auto itr = tempStartpos.find(S[i]);
            // itr->second += 1;
        }
        // else
        // {
        //     cbuckSA[i] = -1;
        // }
    }

    // for (int i = 0; i < size; i++)
    // {
    //     cout<<i<<" " << cbuckSA[i]<<endl;
    // }

    for (int i = 0; i < size; i++)
    {
        if (cbuckSA[i] != -1)
        {
            int check = cbuckSA[i] - 1;
            if (check >= 0 && Type[check] == "L")
            {
                auto itr = tempStartpos.find(S[check]);
                cbuckSA[itr->second] = check;
                itr->second += 1;
            }
        }
    }
    tempEndpos = endpos;

    for (int i = size - 1; i >= 0; i--)
    {
        if (cbuckSA[i] > 0)
        {
            int check = cbuckSA[i] - 1;
            // cout<<check<<endl;
            if (Type[check] == "S")
            {
                // cout<<i<<endl;
                auto itr = tempEndpos.find(S[check]);
                cbuckSA[itr->second] = check;
                itr->second -= 1;
            }
        }
    }
    vector<int> temp_T;

    temp_T.resize(size);
    for (int i = 0; i < size; i++)
    {
        temp_T[i] = -1;
    }

    // int countCheck = 0;
    vector<int> tmpLMS;
    for (int i = (int)stringLMS.size() - 1; i >= 0; i--)
    {
        if (typeLMS[i] == 1)
        {
            tmpLMS.push_back(i);
            // countCheck += 1;
        }
    }

    // for (int i = 0; i < cbuckSA.size(); i++)
    // {
    //     cout << i <<" - "<<cbuckSA[i]<<endl;
    // }

    temp_T[size - 1] = 0;
    int cur = 0;
    for (int i = 0; i < tmpLMS.size() - 1; i++)
    {
        if (stringLMS[tmpLMS[i]] == stringLMS[tmpLMS[i + 1]])
        {
            // cout <<i<<endl;
            temp_T[cbuckSA[i + 1]] = cur;
        }
        else
        {
            // cout <<"else "<<i<<endl;
            cur += 1;
            temp_T[cbuckSA[i + 1]] = cur;
        }
    }

    // cout <<"------------- check -------------"<<endl;
    for (int i = 0; i < (int)temp_T.size(); i++)
    {
        if (temp_T[i] != -1)
        {
            T.push_back(temp_T[i]);
        }
    }

    // vector<int> SA1(T.size(), -1);
    cout << "printing out C - buckets" << endl;
    for (int i = 0; i < size; i++)
    {
        cout << S[i] << " ";
    }

    cout << endl;
    for (int i = 0; i < cbuckSA.size(); i++)
    {
        cout << cbuckSA[i] << " ";
    }

    cout << endl
         << "---------------------------------------------------" << endl;

} // end of calBucket

bool Pattern::str_cmp(string s1, string s2)
{
    if (s1.size() == s2.size())
    {
        return s1 < s2;
    }
    else
    {
        return s1.size() < s2.size();
    }
}

void Pattern::sufffix_sort_T1(string s)
{
    string c = "";
    for (int i = 0; i < T.size(); i++)
    {
        c += to_string(T[i]);
    }

    vector<string> storeage;
    vector<string> orig_storeage;

    for (int i = 0; i < c.size(); i++)
    {
        storeage.push_back(c.substr(i, size - i));
    }
    orig_storeage = storeage;
    sort(storeage.begin(), storeage.end(), str_cmp);

    for (int i = 0; i < c.size(); i++)
    {
        for (int j = 0; j < c.size(); j++)
        {
            if (storeage[i] == orig_storeage[j])
            {
                SA1.push_back(j);
            }
        }
    }

    int ind = 0;
    for (int i = 0; i < size; i++)
    {
        if (typeLMS[i] == 1)
        {
            T[ind] = i;
            ind++;
        }
    }
}

vector<int> Pattern::finalSuffix(string S)
{
    vector<int> SA(size, -1);
    map<char, int> tempStartpos;
    tempStartpos = startpos;
    map<char, int> tempEndpos;
    tempEndpos = endpos;

    string t = "";
    int pos;
    for (int i = T.size() - 1; i >= 0; i--)
    {
        t = S[T[SA1[i]]];
        pos = T[SA1[i]];
        auto itr = tempEndpos.find(S[T[SA1[i]]]);
        SA[itr->second] = pos;
        itr->second -= 1;
        t = "";
    }

    tempEndpos = endpos;
    for (int i = 0; i < SA.size(); i++)
    {
        if (SA[i] != -1)
        {
            int ch = SA[i] - 1;
            if (ch >= 0)
            {
                string typeCheck = Type[ch];
                if (typeCheck == "L")
                {
                    auto itr = tempStartpos.find(S[ch]);
                    SA[itr->second] = ch;
                    itr->second += 1;
                }
            }
        }
    }

    for (int i = SA.size() - 1; i >= 0; i--)
    {
        if (SA[i] != -1)
        {
            int ch = SA[i] - 1;
            if (ch >= 0)
            {
                string typeCheck = Type[ch];
                if (typeCheck == "S")
                {
                    auto itr = tempEndpos.find(S[ch]);
                    SA[itr->second] = ch;
                    itr->second -= 1;
                }
            }
        }
    }

    cout << "SA1 - vector" << endl;
    for (int i = 0; i < SA.size(); i++)
    {
        cout << i << " ";
    }
    cout << endl;
    for (int i = 0; i < SA.size(); i++)
    {
        cout << SA[i] << " ";
    }

    cout << endl
         << "---------------------------------------------------" << endl;
    cout << "T - bucket" << endl;
    for (int i = 0; i < T.size(); i++)
    {
        cout << i << " ";
    }
    cout << endl;
    for (int i = 0; i < T.size(); i++)
    {
        cout << T[i] << " ";
    }

    cout << endl
         << "---------------------------------------------------" << endl;

    cout << "Final suffix array" << endl;
    cout << fixed << setw(5) << right << " " << setprecision(2) << "index"
         << setw(5) << right << " " << setprecision(2) << "SA Index"
         << endl;

    for (int i = 0; i < S.size(); i++)
    {
        cout << setw(7) << right << "" << setprecision(4) << i
             << setw(8) << right << "" << setprecision(4) << SA[i] << endl;
    }

    return SA;
}

void Pattern::buildSA(string s)
{
    string dev = s;
    lsType(dev);
    cout << " L-type and S- Type " << endl;
    printlsType();
    cout << "---------------------------------------------------";
    cout << endl
         << endl;
    markLMS(Type, dev);
    cout << "List all the LMS strings" << endl;
    printLMS();
    cout << "---------------------------------------------------";
    cout << endl
         << endl;
    calBucket(dev);

    sufffix_sort_T1(dev);

    finalSuffixarray = finalSuffix(dev);
}

vector<int> Pattern::patternCheckResult(string s, string p)
{
    vector<int> result;
    int z = (int)finalSuffixarray.size();

    for (int i = 0; i < z; i++)
    {
        if (s[finalSuffixarray[i]] == p[0])
        {
            result.push_back(finalSuffixarray[i]);
        }
    }

    return result;
}

// main function
void Pattern::match_check(string s, string pattern)
{
    clock_t time_req;
    time_req = clock();

    string S = s + '$';

    size = (int)S.size();
    for (int i = 0; i < size; i++)
    {
        cout << i << " ";
    }

    cout << endl;

    for (int i = 0; i < size; i++)
    {
        cout << S[i] << " ";
    }
    cout << endl;
    cout << "---------------------------------------------------" << endl;
    buildSA(S);
    cout << "---------------------------------------------------" << endl;
     cout<<"wait"<<endl;
    cout << "Pattern match starting from index : " << endl;
    vector<int> resStartpos;
    resStartpos = patternCheckResult(S, pattern);
   
    int posSize = resStartpos.size();
    int flag = 0;
    for (int i = 0; i < posSize; i++)
    {
        if (s.substr(resStartpos[i], pattern.size()) == pattern)
        {
            cout << resStartpos[i] << endl;
            flag = 1;
        }
    }

    if (!flag)
    {
        cout << "Pattern not found" << endl;
    }

    cout << "---------------------------------------------------" << endl;

    time_req = clock() - time_req;
    cout << "Time taken" << endl;
    cout << (float)time_req / CLOCKS_PER_SEC << endl;
}

int main()
{
    // string s = "sahiil";
    string s = "";
    string pattern = "";
    cin >> s;
    cin >> pattern;

    Pattern ob;
    ob.match_check(s, pattern);

    return 0;
}