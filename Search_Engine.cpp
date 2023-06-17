#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <vector>
#include "WebGraph.h"
#include "Webpage.h"
using namespace std;
int webNum = 30;
int Webpage::count = -1;
double Webpage::max = -1;
double Webpage::min = 1.5;
vector <Webpage> websites(webNum);
enum condition { AND, OR, none, error, one };
void Process_Input(string query, string& res1, string& res2, condition& cond);
vector<string> MatchStrings(string res1, string res2, condition c);
void ChooseWebsite(vector<Webpage>& webpages, string, string);
void DisplayRPage(vector<Webpage>& webpages, string, string);
void Options2(vector<Webpage>& webpages, string, string);
void displayWebPages(vector<Webpage> webpages);
void Menu();
void NotAvailable();
void Search(); 
void UpdateClicks(Webpage& w);
void Exit();
int main()
{
    cout << "\n\n\n\t\t\tWelcome to my Search Engine: \n\n";
    Menu();
    return 0;
}
void Menu()
{
    string choice;
    cout << "\n\t\t\tWhat would you like to do?\n\t\t\t1. New Search\n\t\t\t2. Exit\n";
    cout << "\n\t\t\tType your choice: ";
    cin >> choice;

    if (choice == "1")
    {
        Search();
    }
    else if (choice == "2")
    {
        Exit();
    }
    else
    {
        system("cls");
        NotAvailable();
        Menu();
    }
}
void Exit()
{
    system("cls");
    cout << "\n\t\t\tThank you for using My Search Engine\n\n";
    system("pause");
}
void NotAvailable()
{
    cout << "\n\n\t\t\tSorry this option is not available. Please Try Again.\n\n";
}
void Search()
{
    system("cls");
    cout << "\n\t\t\tPlease Type Your Search Query\n";
    string searchquery, res1, res2, dummy;
    condition cond;
    WebGraph G;
    cout << "\n\t\t\tYour Query: "; 
    getline(cin, dummy);
    getline(cin, searchquery);
    Process_Input(searchquery, res1, res2, cond);
    vector <string> r = MatchStrings(res1, res2, cond);
    if (r[0] == "No matching results found. Please check your spelling or try another search query.\n")
    {
        cout << "\n\t\t\t" << r[0] << endl;
        Menu();
    }
    else
    {
        G.SetGraphImpressionsClicks(websites);
        G.PageRank();
        G.FilterResults(r);
        vector <Webpage> wp = G.GetWebPages();
        wp = G.UpdateImpressions();
        DisplayRPage(wp, res1, res2);
    }
}
void DisplayRPage(vector<Webpage>& wp, string res1, string res2)
{
    cout << "\n\t\t\tSearch Results: \n";
    displayWebPages(wp);
    string choice;
    cout << "\t\tWould you like to\n\t\t\t1. Choose a webpage to open\n\t\t\t2. New Search\n\t\t\t3. Exit\n";
    cout << "\nType in your choice: ";
    cin >> choice;
    if (choice == "1")
    {
        cout << "\n\t\t\tWhich website would you like to visit?\n";
        ChooseWebsite(wp, res1, res2);
    }
    else if (choice == "2")
    {
        system("cls");
        Search();
    }
    else if (choice == "3")
        Exit();
    else
    {
        system("cls");
        NotAvailable();
        DisplayRPage(wp, res1, res2);
    }
}
void ChooseWebsite(vector<Webpage>& webpages, string res1, string res2)
{
    string choice;
    cout << "\t\t\tType in your choice: ";
    cin >> choice;
    if (stoi(choice) <= webpages.size() && stoi(choice) > 0)
    {
        cout << "\n\t\t\tYou are now viewing " << webpages[stoi(choice) - 1].name << ".";
        UpdateClicks(webpages[stoi(choice) - 1]);

        Options2(webpages, res1, res2);
    }
    else
    {
        system("cls");
        NotAvailable();
        ChooseWebsite(webpages, res1, res2);
    }
}
void Options2(vector<Webpage>& webpages, string res1, string res2)
{
    string choice;
    cout << "\n\t\t\tWould you like to\n\t\t\t1. Back to search results\n\t\t\t2. New Search\n\t\t\t3. Exit\n";
    cout << "\n\t\t\tType in your choice: ";
    cin >> choice;
    system("cls");
    if (choice == "1")
    {
        DisplayRPage(webpages, res1, res2);
    }
    else if (choice == "2")
    {
        Search();
    }
    else if (choice == "3")
    {
        Exit();
    }
    else
    {
        system("cls");
        NotAvailable();
        Options2(webpages, res1, res2);
    }

}
void displayWebPages(vector<Webpage> webpages)
{
    sort(webpages.begin(), webpages.end(), [](Webpage a, Webpage b) {return a.score > b.score; });
    for (int i = 0; i < webpages.size(); i++)
        cout << "\t\t\t" << setw(3) << i + 1 << setw(25) << webpages[i].name << endl;// << setw(15) << webpages[i].score << setw(10) << endl;
}
vector<string> MatchStrings(string res1, string res2, condition c)
{
    vector <string> results(webNum);
    int i = 0;
    int j = 0;
    string test, website, space = " ";
    ifstream keywords;
    keywords.open("Keywords file.csv");
    if (c == error)
    {
        results.resize(1);
        results[0] = "No matching results found. Please check your spelling or try another search query.\n";
    }
    else
    {
        while (keywords >> website)
        {
            websites[j].SetName(website);
            j++;
            getline(keywords, test);
            if (c == AND)
            {
                if (test.find(space + res1) < test.length() && test.find(space + res2) < test.length())
                {
                    results[i] = website;
                    i++;
                }
            }
            else if (c == OR)
            {
                if (test.find(space + res1) < test.length() || test.find(space + res2) < test.length())
                {
                    results[i] = website;
                    i++;
                }
            }
            else if (c == none)
            {
                if (test.find(space + res1) < test.length())
                {
                    results[i] = website;
                    i++;
                }
            }
            else if (c == one)
            {
                if (test.find(space + res1) < test.length())
                {
                    results[i] = website;
                    i++;
                }
            }
        }
    }
    keywords.close();
    results.resize(i);
    if (results.empty())
    {
        results.resize(1);
        results[0] = "No matching results found. Please check your spelling or try another search query.\n";
    }
    return results;
}
void Process_Input(string query, string& res1, string& res2, condition& cond)
{
    if (query[0] == '\"' && query[query.length() - 1] == '\"')
    {
        cond = none;
        res1 = query.substr(1, query.length() - 2);
        res2 = "";
    }
    else if (query.find(" ") < query.length())
    {
        if (query.find(" AND ") < query.length())
        {
            cond = AND;
            res1 = query.substr(0, query.find(" AND "));
            res2 = query.substr(query.find_last_of(" AND ") + 1, query.length() - query.find_last_of(" AND ") - 1);
        }
        else if (query.find(" OR ") < query.length())
        {            
            cond = OR;
            res1 = query.substr(0, query.find(" OR "));
            res2 = query.substr(query.find_last_of(" OR ") + 1, query.length() - query.find_last_of(" OR ") - 1);
        }
        else
        {
            cond = OR;
            res1 = query.substr(0, query.find(" "));
            res2 = query.substr(query.find(" ") + 1, query.length() - query.find(" ") - 1);
        }
    }
    else if (query.find(" ") > query.length())
    {
        res1 = query;
        res2 = "";
        cond = one;
    }
    else
        cond = error;
}
void UpdateClicks(Webpage& w)
{
    w.clicks++;
    w.setScore();
    ifstream cFile;
    cFile.open("Number of Clicks file.csv");
    vector<string> names(webNum);
    vector<int> values(webNum);
    string comma, word1, word2;
    int i = 0;
    while (cFile >> word1)
    {
        cFile >> comma >> word2;
        names[i] = word1;
        values[i] = stoi(word2);
        i++;
    }
    cFile.close();
    ofstream oFile;
    oFile.open("Number of Clicks file.csv");    
    for (int ik = 0; ik < i; ik++)
    {
        if (names[ik] == w.name)
        {
            values[ik] = values[ik] + 1;
            break;
        }
    }
    for (int t = 0; t < webNum; t++)
    {
        oFile << names[t] << " , " << values[t] << endl;
    }
    oFile.close();
}