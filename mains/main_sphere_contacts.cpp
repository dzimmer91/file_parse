#include<iostream>

#include<fstream>
#include<string.h>
#include<stdlib.h>
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
  int debug=0;
  if(argc == 4){
    debug = atoi(argv[3]);
  }else if(argc != 3){
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
    if(debug > 4)  cout << "\nstation=" << tmpdata[0] << " " << tmpdata[1] << " " << tmpdata[2];
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
  curpointer = headptr;

  char tmpdate[80], tmpedate[80], curdate[80], curedate[80];
  if(debug >4){
    cout <<"\n\ndata dump";
    while ( curpointer != NULL){
      strftime(curdate,80,"%m/%d/%G %H:%M:%S",&curpointer->aos_tm);
      strftime(curedate,80,"%m/%d/%G %H:%M:%S",&curpointer->los_tm);
      cout << "\ncurpointer station=" << curpointer->station;
      cout << "   curaos=" << curdate << "->" << curedate << " duration=" << curpointer->duration;
      curpointer = curpointer->next;
    }
  }
  curpointer = headptr;
  struct ptrlist{
    lldata *data;
    ptrlist *next;
  };

  ptrlist *bestlist, *bestlistend, *besthead;
  besthead = bestlist = bestlistend = NULL;
  if(debug)cout << "\nBest list generation";
  while (curpointer->next != NULL){// = curtime + 60){
    lldata *tmpptr = headptr;
    if(debug>2){
      cout << "\n\ncurpointer station=" << curpointer->station << "-";
      strftime(curdate,80,"%m/%d/%G %H:%M:%S",&curpointer->aos_tm);
      strftime(curedate,80,"%m/%d/%G %H:%M:%S",&curpointer->los_tm);
      cout << "   curaos=" << curdate << "->" << curedate << " duration=" << curpointer->duration;
    }
    while (tmpptr != NULL ){
       // cout << "\ntmp station=" << tmpptr->station;
       // cout << " aos=" << tmpptr->aos_t << "->" << tmpptr->los_t;
      //  cout << "\ncuraos=" << curpointer->aos_t << "->" << curpointer->los_t;
      if(debug>2){
        cout << "\ntmpptr while loop";
        cout << "   tmp station=" << tmpptr->station << "-";
        strftime(tmpdate,80,"%m/%d/%G %H:%M:%S",&tmpptr->aos_tm);
        strftime(tmpedate,80,"%m/%d/%G %H:%M:%S",&tmpptr->los_tm);
        cout << "  tmpaos=" << tmpdate << "->" << tmpedate << " duration=" << tmpptr->duration;
      }
      if( curpointer->los_t < tmpptr->aos_t){
        if(debug>2){
          cout << "\naos passed current LOS";
          cout << "\ntmp station=" << tmpptr->station << "-";
          strftime(tmpdate,80,"%m/%d/%G %H:%M:%S",&tmpptr->aos_tm);
          strftime(tmpedate,80,"%m/%d/%G %H:%M:%S",&tmpptr->los_tm);
          cout << "  tmpaos=" << tmpdate << "->" << tmpedate << " duration=" << tmpptr->duration;
        }
        if(bestlist->next != NULL) bestlist = bestlist->next;
        bestlistend = bestlistend->next;
        break;
      }


      if (curpointer->aos_t < tmpptr->los_t ){
        if(strcmp (curpointer->station, tmpptr->station)==0){
          if(debug>2){
            cout << "\ntmp station=" << tmpptr->station << "-";
            strftime(tmpdate,80,"%m/%d/%G %H:%M:%S",&tmpptr->aos_tm);
            strftime(tmpedate,80,"%m/%d/%G %H:%M:%S",&tmpptr->los_tm);
            cout << "  tmpaos=" << tmpdate << "->" << tmpedate << " duration=" << tmpptr->duration;
          }
          if(curpointer->los_t > tmpptr->aos_t && curpointer->aos_t < tmpptr->los_t ) {
            if(debug){
              cout << "\nchecking duration";
              strftime(curdate,80,"%m/%d/%G %H:%M:%S",&curpointer->aos_tm);
              cout << "\ncuraos=" << curdate << " duration=" << curpointer->duration;
            }
           if(bestlist != NULL){
             if(bestlistend == NULL){
               bestlist->next = new ptrlist;
               bestlistend = bestlist->next;
               bestlistend->data = tmpptr;
               bestlistend->next = NULL;
                   cout << "\ncreating new bestlist->next adding to bestlist station=" << tmpptr->station << "-";
                   strftime(tmpdate,80,"%m/%d/%G %H:%M:%S",&tmpptr->aos_tm);
                   strftime(tmpedate,80,"%m/%d/%G %H:%M:%S",&tmpptr->los_tm);
                   cout << "  tmpaos=" << tmpdate << "->" << tmpedate << " duration=" << tmpptr->duration;
              }else{
                if(tmpptr->duration > bestlistend->data->duration){
                   cout << "\nchanging to bestlist station=" << tmpptr->station << "-";
                   strftime(tmpdate,80,"%m/%d/%G %H:%M:%S",&tmpptr->aos_tm);
                   strftime(tmpedate,80,"%m/%d/%G %H:%M:%S",&tmpptr->los_tm);
                   cout << "  tmpaos=" << tmpdate << "->" << tmpedate << " duration=" << tmpptr->duration;
                  bestlistend->data = tmpptr;
                }
              }
            }else{
              bestlist = new ptrlist;
              bestlist->next = NULL;
              bestlistend = besthead = bestlist;
              bestlistend->data = tmpptr;
                   cout << "\ncreating new head bestlist station=" << tmpptr->station << "-";
                   strftime(tmpdate,80,"%m/%d/%G %H:%M:%S",&tmpptr->aos_tm);
                   strftime(tmpedate,80,"%m/%d/%G %H:%M:%S",&tmpptr->los_tm);
                   cout << "  tmpaos=" << tmpdate << "->" << tmpedate << " duration=" << tmpptr->duration;
            }


          }
        }else{
          if(debug)cout << "\nstation != station";
        }
      }else{
        if(debug)cout << "\ntmpptr los < cur aos";
      }
      tmpptr = tmpptr->next;
    }
    curpointer = curpointer->next;
  }
  bestlist = besthead;
  while(bestlist != NULL){
      strftime(curdate,80,"%m/%d/%G %H:%M:%S",&bestlist->data->aos_tm);
      strftime(curedate,80,"%m/%d/%G %H:%M:%S",&bestlist->data->los_tm);
      cout << "\n station=" << bestlist->data->station;
      cout << "   curaos=" << curdate << "->" << curedate << " duration=" << bestlist->data->duration;
    fprintf(outputfile,"%s,%s,%s\n",bestlist->data->station, curdate, curedate);
    bestlist = bestlist->next;

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
