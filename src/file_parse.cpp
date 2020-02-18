

#include<iostream>
#include<fstream>
#include<string>

//define this to read NPAS mission / vis file;
#define __NPAS_FILE_READING__

#include"file_parse.h"

//check if file is open and if not open file;
bool Parser::check_fileopen()
{
  if(file.is_open()){
    return true;
  }
  else{
    file.open (fileLocation, std::ifstream::in);
    if(file.is_open())
    {
      fileOpen=true;
      return true;
    }else
    {
      fileOpen=false;
      return false;
    }
  }
}
//rewind to top of file;
void Parser::rewind_file()
{
  if(file.is_open()){
    //clear any file flags
    file.clear();
    //rewind to top of file;
    file.seekg(0);
  }
}


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
    fileLocation = new char [fileloclength=std::char_traits<char>::length(fileloc)+1];
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

  int numlines=0;
  std::string line;

  if(check_fileopen() == false){
    return -1;
  }

  if(debug)std::cout << "parsefileopen=" << file.is_open() << " pasefileloc=" << fileLocation << "\n";

  //find number of lines in file;
  while(std::getline(file, line)) ++numlines;

  rewind_file();

  return numlines;
}

int Parser::getnumdelcount()
{

  int numvars=0;
  std::string line;

  if(check_fileopen() == false){
    return -1;
  }

  std::getline(file, line);
  int inputstrlen=line.length();
  //gets the number of vars to return and changes the delimiter to NULL
  for(int i=0;i<inputstrlen;i++)
  {
    if(line[i]==delimiter){
      ++numvars;
    }
  }
  //rewind to top of file;
  rewind_file();

  return numvars;
}

char *Parser::getline_nodel()
{

  if(fileOpen){
      char *item;
      std::string tmpitem;

      std::getline(file, tmpitem);
      if(file.eof()) return NULL;
      item = new char [tmpitem.length()+1];
      strcpy(&item[0],tmpitem.c_str());

      return item;
  }else{
    return NULL;
  }
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

    check_fileopen();

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
  int itemnum=0;
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
        check_fileopen();
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
        check_fileopen();
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
#ifdef __NPAS_FILE_READING__

struct Misheader *Parser::readvis()
{
  debug = false;

  Parser::numVars=7;

  Misheader *FirstFileHead, *FirstFileCurrent;
  FirstFileHead = new Misheader;
  FirstFileHead->next = NULL;

  bool first = true;


  char **test;
  while ((test = Parser::getline())!=NULL)
  {
    if(debug) std::cout << "test[0]=" << test[0] << std::endl;
    if(strcmp(test[0],"Mission")==0)
    {
      for(int i=0;i<Parser::numVars;i++)
      {
        if(debug) std::cout << "removing mission header" << std::endl;
        delete test[i];
      }
      delete test;
      continue;
    }


    if(!first)
    {
       if(debug) std::cout << "not first" << std::endl;
       FirstFileCurrent = FirstFileHead;
       while(FirstFileCurrent!=NULL)
       {
         if(strcmp(FirstFileCurrent->misname,test[0])==0)
         {
            if(debug) std::cout << "found Mission" << std::endl;
            break;
         }
         FirstFileCurrent = FirstFileCurrent->next;
       }
       if(FirstFileCurrent!=NULL)
       {
          if(debug) std::cout << "FirstFileCurrent!=NULL" << std::endl;
          FirstFileHead->data->next = new Misdata;
          FirstFileHead->data = FirstFileHead->data->next;
          FirstFileHead->data->next = NULL;
          FirstFileHead->data->data = test;
          FirstFileCurrent->data->station = test[1];
          FirstFileCurrent->data->aos = test[3];
          FirstFileCurrent->data->los = test[4];
       }else
       {
          if(debug) std::cout << "FirstFileCurrent==NULL" << std::endl;
          FirstFileCurrent = FirstFileHead;
          while(FirstFileCurrent->next != NULL) FirstFileCurrent = FirstFileCurrent->next;
          FirstFileCurrent->next = new Misheader;
          FirstFileCurrent = FirstFileCurrent->next;
          FirstFileCurrent->next = NULL;

          FirstFileCurrent->data = new Misdata;
          FirstFileCurrent->data->next = NULL;
          FirstFileCurrent->data->data = test;
          strcpy(FirstFileCurrent->misname, test[0]);
          FirstFileCurrent->data->data = test;
          FirstFileCurrent->data->station = test[1];
          FirstFileCurrent->data->aos = test[3];
          FirstFileCurrent->data->los = test[4];
          FirstFileCurrent->datahead = FirstFileCurrent->data;
       }

    }else
    {
       if(debug) std::cout << "first" << std::endl;
       first = false;
       strcpy(FirstFileHead->misname, test[0]);
       FirstFileHead->data = new Misdata;
       FirstFileHead->data->next = NULL;
       FirstFileHead->data->data = test;
       FirstFileHead->data->station = test[1];
       FirstFileHead->data->aos = test[3];
       FirstFileHead->data->los = test[4];
       FirstFileHead->datahead = FirstFileHead->data;

    }


  }
  FirstFileCurrent = FirstFileHead;
  while(FirstFileCurrent != NULL )
  {


     Misdata *tmpcur = FirstFileCurrent->datahead;
     while(tmpcur != NULL)
     {
        for(int i=0; i< strlen(tmpcur->station);i++) tmpcur->station[i] = toupper(tmpcur->station[i]);
        tmpcur->aostm.tm_isdst=1;
        tmpcur->lostm.tm_isdst=1;
        tmpcur->changed = false;
        strptime(tmpcur->aos, "%m/%d/%Y %H:%M:%S", &tmpcur->aostm);
        strptime(tmpcur->los, "%m/%d/%Y %H:%M:%S", &tmpcur->lostm);
        tmpcur->aos_t = mktime(&tmpcur->aostm);
        tmpcur->los_t = mktime(&tmpcur->lostm);

        tmpcur = tmpcur->next;
     }

     FirstFileCurrent = FirstFileCurrent->next;

  }

  FirstFileCurrent = FirstFileHead;
  while(FirstFileCurrent != NULL )
  {

     int staNum=1;
     Misdata *tmpcur = FirstFileCurrent->datahead;
     while(tmpcur != NULL)
     {
       Misdata *tmpcursec = FirstFileCurrent->datahead;
       if(tmpcur->stanum == 0)
       {
          while(tmpcursec != NULL)
          {
             if(strcmp(tmpcursec->station,tmpcur->station)==0)
             tmpcursec->stanum=staNum;
             tmpcursec = tmpcursec->next;
          }
          ++staNum;
        }
        tmpcur = tmpcur->next;
     }
     FirstFileCurrent = FirstFileCurrent->next;

  }

  return FirstFileHead;

}


struct Misheader *Parser::readsch()
{
  debug = false;

  Parser::numVars=12;

  Misheader *FirstFileHead, *FirstFileCurrent;
  FirstFileHead = new Misheader;
  FirstFileHead->next = NULL;

  bool first = true;


  char **test;
  while ((test = Parser::getline())!=NULL)
  {
    if(debug) std::cout << "test[0]=" << test[0] << std::endl;
    if(strcmp(test[0],"Mission")==0)
    {
      for(int i=0;i<Parser::numVars;i++)
      {
        if(debug) std::cout << "removing mission header" << std::endl;
        delete test[i];
      }
      delete test;
      continue;
    }


    if(!first)
    {
       if(debug) std::cout << "not first" << std::endl;
       FirstFileCurrent = FirstFileHead;
       while(FirstFileCurrent!=NULL)
       {
         if(strcmp(FirstFileCurrent->misname,test[0])==0)
         {
            if(debug) std::cout << "found Mission" << std::endl;
            break;
         }
         FirstFileCurrent = FirstFileCurrent->next;
       }
       if(FirstFileCurrent!=NULL)
       {
          if(debug) std::cout << "FirstFileCurrent!=NULL" << std::endl;
          FirstFileHead->data->next = new Misdata;
          FirstFileHead->data = FirstFileHead->data->next;
          FirstFileHead->data->next = NULL;
          FirstFileHead->data->data = test;
          FirstFileCurrent->data->station = test[1];
          FirstFileCurrent->data->aos = test[4];
          FirstFileCurrent->data->los = test[5];
       }else
       {
          if(debug) std::cout << "FirstFileCurrent==NULL" << std::endl;
          FirstFileCurrent = FirstFileHead;
          while(FirstFileCurrent->next != NULL) FirstFileCurrent = FirstFileCurrent->next;
          FirstFileCurrent->next = new Misheader;
          FirstFileCurrent = FirstFileCurrent->next;
          FirstFileCurrent->next = NULL;

          FirstFileCurrent->data = new Misdata;
          FirstFileCurrent->data->next = NULL;
          FirstFileCurrent->data->data = test;
          strcpy(FirstFileCurrent->misname, test[0]);
          FirstFileCurrent->data->data = test;
          FirstFileCurrent->data->station = test[1];
          FirstFileCurrent->data->aos = test[4];
          FirstFileCurrent->data->los = test[5];
          FirstFileCurrent->datahead = FirstFileCurrent->data;
       }

    }else
    {
       if(debug) std::cout << "first" << std::endl;
       first = false;
       strcpy(FirstFileHead->misname, test[0]);
       FirstFileHead->data = new Misdata;
       FirstFileHead->data->next = NULL;
       FirstFileHead->data->data = test;
       FirstFileHead->data->station = test[1];
       FirstFileHead->data->aos = test[4];
       FirstFileHead->data->los = test[5];
       FirstFileHead->datahead = FirstFileHead->data;

    }


  }
  FirstFileCurrent = FirstFileHead;
  while(FirstFileCurrent != NULL )
  {


     Misdata *tmpcur = FirstFileCurrent->datahead;
     while(tmpcur != NULL)
     {
        tmpcur->stanum=0;
        tmpcur->aostm.tm_isdst=1;
        tmpcur->lostm.tm_isdst=1;
        tmpcur->changed = false;
        strptime(tmpcur->aos, "%m/%d/%Y %H:%M:%S", &tmpcur->aostm);
        strptime(tmpcur->los, "%m/%d/%Y %H:%M:%S", &tmpcur->lostm);
        tmpcur->aos_t = mktime(&tmpcur->aostm);
        tmpcur->los_t = mktime(&tmpcur->lostm);
        tmpcur = tmpcur->next;
     }

     FirstFileCurrent = FirstFileCurrent->next;

  }

  return FirstFileHead;

}


Misdata *Parser::Misdata_remove(Misheader *head, Misdata *delinput)
{

     Misdata *preinput;
     bool resethead = false;
     if(delinput != head->datahead)
     {
        preinput = head->datahead;
        while(preinput->next != delinput && preinput->next != NULL) preinput = preinput->next;

     }else
     {

        resethead = true;
     }

     Misdata *tmpcur = delinput;
     char **tmpdata = tmpcur->data;

     if(!resethead)  preinput->next = tmpcur->next;
     else  preinput = head->datahead = tmpcur->next;

     delete tmpcur;
     for(int i=0; i<Parser::numVars;i++) delete tmpdata[i];
     delete tmpdata;

     return preinput;

}

void Parser::MisHeaderCleanup(Misheader *delinput)
{

  Misheader *FirstFileHead, *FirstFileCurrent;


  FirstFileCurrent = delinput;
  while(FirstFileCurrent != NULL )
  {

     Misdata *tmpcur = FirstFileCurrent->datahead;
     while(tmpcur != NULL)
     {

        char **tmpdata = tmpcur->data;
        Misdata *tmphead = tmpcur;
        tmpcur = tmpcur->next;
        delete tmphead;
        for(int i=0; i<Parser::numVars;i++) delete tmpdata[i];
        delete tmpdata;
     }
     Misheader *tmpheader = FirstFileCurrent;
     FirstFileCurrent = FirstFileCurrent->next;
     delete tmpheader;

  }

}

#endif
