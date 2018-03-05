

#include<iostream>
#include<fstream>
#include<string>
class Parser
{

  public:
    Parser();
    Parser(char*);
    Parser(int, char, char*, char**);
    ~Parser();
    void dumpfile(); 
    void setDel(char );
    void setCommentDel(char);
    void setVarsPerLine(int* );
    bool setFileLocation(char* );
    void setPassedVar(char**);
    char **getline();
    char *getdel();
    char **stripunalmemstr(char *, int *);
    char **stripmemstr(char *, int *);
    bool getfileopen();
    int getnumlines();
  private:
    bool debug;
    int numVars;
    bool fileOpen, commentflag;
    char delimiter;
    char commentDelimter;
    char *fileLocation;
    char **passedVar;
    std::ifstream file;    
};
