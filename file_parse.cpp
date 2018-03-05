

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


Parser::Parser()
{
//setup vars if none are passed in//
  numVars=0;
  debug=false;
  fileOpen=false;
  commentflag=false;
  delimiter='\0';
  fileLocation=NULL;
}
Parser::Parser(char *fileloc)
{
  debug=false;
  if(fileloc!=NULL)
  {
    int fileloclength=0;
    fileLocation = new char [fileloclength=std::char_traits<char>::length(fileloc)];
//    strcpy(fileLocaton, fileloc);
    std::char_traits<char>::copy(fileLocation, fileloc, fileloclength);
    file.open (fileLocation, std::ifstream::in);
    if(file.is_open())
    {
      fileOpen=true;
    }else
    {
      fileOpen=false;

    }

  } 
  commentflag=false;
  delimiter='\0';
  numVars=0;

}

int Parser::getnumlines()
{

  int numlines=0, missioncount=0;
  std::string line;
  if(file.is_open());
  else
  {
    file.open (fileLocation, std::ifstream::in);
    if(file.is_open())
    {
      fileOpen=true;
    }else
    {
      fileOpen=false;

    }
  }
if(debug)std::cout << "parsefileopen=" << file.is_open() << " pasefileloc=" << fileLocation << "\n";
  while(std::getline(file, line)) ++numlines;
  file.close();
  if(file.is_open());
  else
  {
    file.open (fileLocation, std::ifstream::in);
    if(file.is_open())
    {
      fileOpen=true;
    }else
    {
      fileOpen=false;

    }
  }
  return numlines;
}

bool Parser::getfileopen()
{
  return fileOpen;


}
bool Parser::setFileLocation(char *fileloc)
{
 
  if(fileloc!=NULL)
  {
    int fileloclength=0;
    fileLocation = new char [fileloclength=std::char_traits<char>::length(fileloc)];
    std::char_traits<char>::copy(fileLocation, fileloc, fileloclength);
    file.open (fileLocation, std::ifstream::in);
    if(file.is_open())
    {
      fileOpen=true;
    }else
    {
      fileOpen=false;

    }

  }
  return fileOpen; 

}
void Parser::dumpfile()
{
    if(fileOpen==true)
    {
       char c = file.get();
       while (file.good()) 
       {
          std::cout << c;
          c = file.get();
       }
    }else
    {


    }

}
void Parser::setDel(char del)
{
  delimiter=del;
}
void Parser::setCommentDel(char del)
{
  commentDelimter = del;
}
void Parser::setVarsPerLine(int* num)
{
  numVars=*num;
}

char **Parser::stripunalmemstr(char *inputstr, int *numofvars)
{
  char **item;
  int inputdelnum=0, inputstrlen=std::char_traits<char>::length(inputstr);
  //gets the number of vars to return and changes the delimiter to NULL
  for(int i=0;i<inputstrlen;i++)
  {
    if(inputstr[i]==delimiter) 
    {
      ++inputdelnum;
      inputstr[i]='\0';
    }
  }
  item = new char *[inputdelnum+1];
  int copylength=0,itemnum=0;
  for(int i=0; i<inputstrlen;i++)
  {
    if(i==0)
    {
      item[itemnum] = new char [((copylength=std::char_traits<char>::length(&inputstr[i]))+1)];
      std::char_traits<char>::copy(item[itemnum], &inputstr[i],copylength);
      ++itemnum;
    }else if (inputstr[i]=='\0')
    {
      item[itemnum] = new char [((copylength=std::char_traits<char>::length(&inputstr[i+1]))+1)];
      std::char_traits<char>::copy(item[itemnum], &inputstr[i+1],copylength);
      ++itemnum;
    }
  }
  *numofvars=itemnum;
  return item;
}

char **Parser::stripmemstr(char *inputstr, int *numofvars)
{
  char **item;
  int inputdelnum=0, inputstrlen=std::char_traits<char>::length(inputstr);
  //gets the number of vars to return and changes the delimiter to NULL
  for(int i=0;i<inputstrlen;i++)
  {
    if(inputstr[i]==delimiter) 
    {
      ++inputdelnum;
      inputstr[i]='\0';
    }
  }

  item = new char *[inputdelnum];
  int copylength=0,itemnum=0;
  for(int i=0; i<inputstrlen;i++)
  {

    if(i==0)
    {
      item[itemnum] = &inputstr[i];
      ++itemnum;

    }else if (inputstr[i]=='\0')
    {
      item[itemnum] = &inputstr[i+1];
      ++itemnum;
    }
  }
  *numofvars=itemnum;
  return item;
}


char **Parser::getline()
{
  static bool first=false;
if(debug)  std::cout << "parsefileopen=" << file.is_open() << " pasefileloc=" << fileLocation << "\n";

  while(true)
  {
    if(fileOpen)
    {
      char **item;
      std::string tmpitem;
 
      std::getline(file, tmpitem);
      if(file.eof()) return NULL;      
      item = new char *[numVars];
      for(int i=0,pos=0,oldpos=0; i<numVars;i++)
      {
        if(oldpos > tmpitem.length()) 
        {
    
        }else
        {
          pos=tmpitem.find(delimiter, oldpos);
        }
        //std::cout << "pos=" << pos << "\n";
        //std::cout << "oldpos=" << oldpos << "\n";
        if((i==0 && pos ==-1) || oldpos==-1)
        {
          item[i] = new char [1];
          item[i][0]='\0';
          oldpos=-1;
          
        }else if (oldpos==tmpitem.length()+1)
        {
          //std::cout << "( item[i]=NULL;)\n\n";
          item[i] = new char [1];
          item[i][0]='\0';
          //item[i][1]='\0';

        }else if( pos == -1)
        {
        //std::cout << "( pos == -1)\n\n";
            pos=(tmpitem.length() - oldpos);
            item[i] = new char [(pos+1)];
            int length = tmpitem.copy(item[i], pos, oldpos);
            item[i][length]='\0';
            oldpos=tmpitem.length()+1;
  
        }else
        {
        //std::cout << "else\n\n";
            item[i] = new char [((pos+1)-oldpos)];
            int length = tmpitem.copy(item[i], (pos-oldpos), oldpos);
            item[i][length]='\0';
            oldpos=pos+1;
        }

      }

      
      return item;


    }else
    {
      if(fileLocation==NULL) return NULL;
      else
      {
        file.open (fileLocation, std::ifstream::in);
        if(file.is_open())
        {
          fileOpen=true;
        }else
        {
          fileOpen=false;
        }
      }
    }
    if(first) return NULL;
    else first=true;
  }


}

char *Parser::getdel()
{
  bool first=false;
  while(true)
  {
    if(fileOpen)
    {
      char *item;
      std::string tmpitem;
      std::getline(file, tmpitem, delimiter);
      if(file.eof()) return NULL;
      item = new char [tmpitem.length()+1];
      int length = tmpitem.copy(item, tmpitem.length());
      item[length]='\0';
      return item;

    }else
    {
      if(fileLocation==NULL) return NULL;
      else
      {
        file.open (fileLocation, std::ifstream::in);
        if(file.is_open())
        {
          fileOpen=true;
        }else
        {
          fileOpen=false;
        }
      }
    }
    if(first) return NULL;
    else first=true;
  }


}

Parser::~Parser()
{
  if(fileOpen)
  {
    file.close();
    fileOpen=false;
  }
  if(fileLocation!=NULL)
  {
    delete[] fileLocation;
  }

}

