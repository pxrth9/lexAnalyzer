#include <iostream>
using namespace std;

class Cartoon
{
    string title;
    int minutes;

public:
    Cartoon(string t, int m = 30) : title(t), minutes(m) {}
    string getTitle() const { return title; }
    int getLength() const { return minutes; }
    virtual string type() const = 0;
    virtual string getSub() const = 0;
};

class Series : public Cartoon
{
    int numEpisodes;

public:
    Series(string t, int len = 45, int nE = 48)
        : Cartoon(t, len), numEpisodes(nE) {}
    string type() const { return "series"; }
    int getNumType() const { return numEpisodes; }
    virtual string getSub() const{ return "hello";};
};

class Anime : public Series
{
    string subtitles;

public:
    Anime(string title, int len, int numEpisodes, string subtitles) : Series(title, len, numEpisodes), subtitles(subtitles) {}
    string type() const { return "anime"; }
    virtual string getSub() const { return subtitles; }
};

main()
{
    Cartoon *t[] = {
        new Series("TT", 32),
        new Series("BB"),
        new Anime("FMA", 45, 48, "E"),
        0};
    int i = 0;
    while (t[i] != 0)
    {
        cout << t[i]->getTitle() << ": " << t[i]->type() << endl;
        i++;
    }

    //Cartoon *t = new Anime("FMA", 45, 48, "E");
    cout << t[1]->getLength();
}