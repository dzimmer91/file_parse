#include<iostream>

#include<fstream>
#include<string.h>

#include"file_parse.h"

using namespace std;

struct lldata{
  char
    satellite[40],
    station[20];
  tm
    aos_tm,
    los_tm;
  time_t
    aos_t,
    los_t;

  int duration;

  lldata *next;
};

int count_sats(lldata *dataptr, time_t time, lldata *rtnptr)
{
  bool ptrset = false;
  lldata *tmpptr = dataptr;
  int count = 0;
  while(tmpptr != NULL){
    if ( tmpptr->aos_t <= time && tmpptr->los_t >= time ) ++count;
    if ( tmpptr->aos_t > time && ptrset == false){
      rtnptr = tmpptr;
      ptrset = true;
    }
    if ( tmpptr->aos_t > ( time + 86400)) return count;
    tmpptr = tmpptr->next;
  }
  return count;
}

int main(int argc, char **argv)
{
  if(argc != 3){
    cout << "\nError: Invalid arguments\n";
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

    sprintf(&curpointer->satellite[0],"%s",tmpdata[0]);
    sprintf(&curpointer->station[1],"%s",tmpdata[1]);
    curpointer->aos_tm.tm_isdst=0;
    curpointer->los_tm.tm_isdst=0;
    strptime(tmpdata[2], "%m/%d/%Y %H:%M:%S", &curpointer->aos_tm);
    strptime(tmpdata[3], "%m/%d/%Y %H:%M:%S", &curpointer->los_tm);
    curpointer->aos_t = mktime(&curpointer->aos_tm);
    curpointer->los_t = mktime(&curpointer->los_tm);
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
    return -1;
  }
  fprintf(outputfile,"AOS,LOS,count\n");
  curpointer = headptr;

  for(time_t curtime=start_aos;curtime<end_los;curtime++){// = curtime + 60){
    lldata *rtnptr;
    currentcount = count_sats(curpointer, curtime, rtnptr);
    averagecount = ((double)(currentcount + averagecount) /2);
    if ( mincount > currentcount) mincount = currentcount;
    if ( maxcount < currentcount) maxcount = currentcount;
    if ( currentcount != prevouscount ) {
      ++totalchanges;
      tm *timeinfo, *preinfo;
      char predate[80], curdate[80];
      timeinfo =  localtime(&curtime);
      strftime(curdate,80,"%m/%d/%G %H:%M:%S",timeinfo);

      preinfo =  localtime(&prevoustime);
      strftime(predate,80,"%m/%d/%G %H:%M:%S",preinfo);
      fprintf(outputfile,"%s,%s,%d\n",predate,curdate,prevouscount);
      //cout << "\nTime=" << predate << " -> " << curdate<< " Day=" << ((curtime - start_aos) / 86400) << " count=" << currentcount;
      prevoustime = curtime;
      prevouscount = currentcount;

    }

  }
    cout << "\ntotalchanges =" << totalchanges;
    cout << "\nMin count =" << mincount;
    cout << "\nMax count =" << maxcount;
    cout << "\nAverage count =" << averagecount;


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
