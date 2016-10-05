#include "TabReader.h"
#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

//reads size of the string and then reads the string
void gp_read_string(FILE* file, char**buf){
  char read;
  read = (char)fgetc(file);
  *buf = new char[(int)read];
  fgets(*buf, (int)read+1, file);
}

void gp_read_lyrics_string(FILE* file, char**buf, int len){
  *buf = new char[(int)len];
  fgets(*buf, len+1, file);
}

//reads single character and throws it away
void gp_read_null_string(FILE* file){
  fgetc(file);
}

//reads 4 characters and constructs the int
int gp_read_int(FILE* file){
  char read;
  int result = 0;
  for (int i=0; i<4; i++){
    result<<8;
    read = (char)fgetc(file);
    result += (int)read;
  }
  return result;
}

//reads an int, and depending on its value reads the string or reads a null string.
void gp_read_header_info(FILE* file, char**buf){
  if(gp_read_int(file)!=1){
    gp_read_string(file, buf);
    cout<<*buf<<endl; //just to check
  }
  else
    gp_read_null_string(file);
}

void Tab::load_tab(const char* filename){
  //open the file and check that it opened successfully
  FILE* file = fopen(filename, "r");
  if (file==NULL){
    cout<<"file reading error"<<endl;
    return;
  }

  /***** Read the header *****/
  
  char read; //character being read
  
  //read file version, fixed number of characters 
  read = (char)fgetc(file);
  file_version = new char[30]; //still set size to 30 because we read 30 characters
  fgets(file_version, 31, file);

  //read header info
  gp_read_header_info(file, &title);
  gp_read_header_info(file, &subtitle);
  gp_read_header_info(file, &interpret);
  gp_read_header_info(file, &album);
  gp_read_header_info(file, &author);
  gp_read_header_info(file, &copyright);
  gp_read_header_info(file, &tab_author);
  gp_read_header_info(file, &instructional);

  //read number of notices
  int n = gp_read_int(file);
  //create the notices and fill in the data
  notices = new char*[n];
  for (int i=0; i<n; i++)
    gp_read_header_info(file, &notices[i]);

  //read the triplet_feel
  triplet_feel = (int)(char)fgetc(file);
  cout<<triplet_feel<<endl;

  //read the lyrics
  gp_read_int(file); //number of the track corresponding to the lyrics. Don't care
  lyrics = new char*[5];
  
  for (int i=0;i<5;i++){
    n = gp_read_int(file);
    cout<<n<<endl;
    gp_read_lyrics_string(file, &lyrics[i], n);
    cout<<lyrics[i]<<endl;
  }

  //read the tempo
  //get weird values
  tempo = gp_read_int(file);
  cout<<tempo<<endl;
  
  //to see what the rest of the file looks like, uncomment
  /*while ( (read = (char)fgetc(file)) != (char)EOF){
    cout<<read<<endl;
    }*/
  fclose(file);
}
