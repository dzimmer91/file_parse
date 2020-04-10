#include<iostream>

#include<fstream>
#include<string.h>

#include"file_parse.h"

using namespace std;

struct lldata{
  char
    //satellite[40],
    station[40];
  tm
    aos_tm,
    los_tm;
  time_t
    aos_t,
    los_t;

  int duration;

  lldata *next;
};

int main(int argc, char **argv)
{
  if(argc != 3){
    cout << "\nError: Invalid arguments:\n    sphere_contact input_file output_file\n";
    return -1;
  }
  Parser *in_file;
  if(argv[1] != NULL){
    in_file = new Parser(argv[1]);
  }else{
    cout << "\nError: Unable to open file\n";
    return -1;
  }
  in_file->setDel(',');

  int numlines = in_file->getnumlines();
  int numperline = in_file->getnumdelcount();

  time_t start_aos=4000000000, end_los=0;
  ++numperline;
  in_file->setVarsPerLine(&numperline);
  lldata *headptr, *curpointer;
  headptr = curpointer = NULL;

  cout << "\nNumber of lines =" << numlines;

  for(int k=0;k<numlines; k++){
    char **tmpdata = in_file->getline();
    if(k==0) {
      for(int i=0;i<numperline;i++)
      {
        delete[] tmpdata[i];
      }
      delete[] tmpdata;
      continue;
    }
    if(headptr == NULL){
        headptr = curpointer = new lldata;
    }else{
    curpointer->next = new lldata;
    curpointer = curpointer->next;
    }
    curpointer->next = NULL;

    //sprintf(&curpointer->satellite[0],"%s",tmpdata[0]);
    sprintf(&curpointer->station[0],"%s",tmpdata[0]);
    curpointer->aos_tm.tm_isdst=0;
    curpointer->los_tm.tm_isdst=0;
    strptime(tmpdata[1], "%m/%d/%Y %H:%M:%S", &curpointer->aos_tm);
    strptime(tmpdata[2], "%m/%d/%Y %H:%M:%S", &curpointer->los_tm);
    curpointer->aos_t = mktime(&curpointer->aos_tm);
    curpointer->los_t = mktime(&curpointer->los_tm);
    curpointer->duration = curpointer->los_t - curpointer->aos_t;
    if (curpointer->los_t > end_los) end_los = curpointer->los_t;
    if (curpointer->aos_t < start_aos) start_aos = curpointer->aos_t;

    for(int i=0;i<numperline;i++)
    {
      delete[] tmpdata[i];
    }
    delete[] tmpdata;
  }

  int numdays = (int)((end_los - start_aos) / 86400);

  cout << "\nStart_aos =" << start_aos;
  cout << "\nStart date =" << asctime(localtime(&headptr->aos_t));
  cout << "\nStart date =" << asctime(&headptr->aos_tm);

  cout << "\nEnd_los =" << end_los;
  cout << "\nNumber of days =" << numdays;
  int currentcount=0, prevouscount=0;
  int mincount=9999, maxcount=0, totalchanges=0;
  time_t prevoustime=start_aos;
  double averagecount=0;

  FILE *outputfile;
  outputfile = fopen(argv[2], "w");

  if(outputfile == NULL || end_los <= 0 || start_aos <= 0){
  //cleanup
    fclose(outputfile);
    delete in_file;
    curpointer = headptr;
    while(curpointer != NULL){
      lldata *tmp = curpointer->next;
      delete curpointer;
      curpointer = tmp;
    }
    cout << "\nError: unable to open output file";
    return -1;
  }
  fprintf(outputfile,"AOS,LOS,count\n");
  curpointer = headptr;

  struct ptrlist{
    lldata *data;
    ptrlist *next;
  };

  ptrlist *bestlist, *besthead;
  besthead = bestlist = NULL;
  cout << "\nBest list generation";
  while (curpointer->next != NULL){// = curtime + 60){
    lldata *tmpptr = headptr;

    while (tmpptr->next != NULL){
      while ( curpointer->aos_t > tmpptr->aos_t && tmpptr->next != NULL) tmpptr = tmpptr->next;
      if(curpointer->los_t > tmpptr->aos_t ) break;

      if(curpointer->station == tmpptr->station){
        if(curpointer->los_t < tmpptr->aos_t && curpointer->aos_t < tmpptr->los_t ) {
          char tmpdate[80], curdate[80];
          strftime(curdate,80,"%m/%d/%G %H:%M:%S",&curpointer->aos_tm);
          strftime(tmpdate,80,"%m/%d/%G %H:%M:%S",&tmpptr->aos_tm);

          cout << "\ncuraos=" << curdate << " duration=" << curpointer->duration;
          cout << "\ntmpaos=" << tmpdate << " duration=" << tmpptr->duration;
        }
      }
      tmpptr = tmpptr->next;
    }
    curpointer = curpointer->next;
  }


  //cleanup
  fclose(outputfile);
  delete in_file;
  curpointer = headptr;
  while(curpointer != NULL){
    lldata *tmp = curpointer->next;
    delete curpointer;
    curpointer = tmp;
  }

}
