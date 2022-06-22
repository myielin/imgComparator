#include <stdio.h>
#include <stdlib.h>
#include "dataGenerator.h"


void main();

void again(){
  char ans;

  printf("\n  Test another file? [y/n]: ");
  scanf("%s", &ans);

  (ans == 'y' || ans == 'Y') ? main(0) : exit(0);
}

// this function will compare both buffers with image information and return their percentage of difference
double compare(unsigned char * fst, unsigned char * scd, long int p){
  double c = 0.0;

  for (long int i = 0; i < p; i++){
    if(fst[i] != scd[i]){
      c++;
    }
  }

  printf("\nSpotted differences: %d\n", (int) c); // comment this line for csv stuff
  return c / p;
}

// this function corrects the path to find the images and adds the extention .bmp to the string
char * nameFixer(char * name){
  char path[] = "imgs/";
  char ext[] = ".bmp";
  char *fname;

  int c = 0, d = 0, sz = 0;

  while(name[sz] != 0) sz++; // this counts the size of the name vector, that corresponds to the filename

// 10 is the amount of characters in the variables path and ext
  fname = malloc((10 + sz)*sizeof(char));

  for(int i = 0; i <= (10 + sz); i++){
    if (i < 5) fname[i] = path[i];
    else if(i >= (5 + sz)) {
      fname[i] = ext[d];
      d++;
    } else {
      fname[i] = name[c];
      c++;
    }
  }

  return fname;
}

// here is where the image files are loaded and processed, generating two vectors corresponding to the two images that are going to be compared
double read(char *in1, char *in2){
  FILE *f1, *f2;

// here we check if the filenames are correct
  if( ((f1 = fopen(nameFixer(in1), "rb")) == NULL) || ((f2 = fopen(nameFixer(in2), "rb")) == NULL) ){
    printf("\nFile not found\n");
    again();
  }

  unsigned char header[54];
  unsigned char colortable1[1024];
  unsigned char colortable2[1024];

// since the analysis is supposed to compare bmp images following the same pattern, both headers should be equal, and it is good to store their information in a buffer just in case
  for(int i = 0; i < 54; i++){
    header[i] = getc(f1);
    getc(f2);
  }

  long int width = *(int*)&header[18];
  long int height = *(int*)&header[22];

// image colortables differ but they are not going to be included in the comparasion, so the information is just going to be stored in case it might be useful for some purpose later on.
  fread(colortable1, sizeof(unsigned char), 1024, f1);
  fread(colortable2, sizeof(unsigned char), 1024, f2);

  unsigned char buf1[height * width];
  unsigned char buf2[height * width];

  fread(buf1, sizeof(unsigned char), (height * width), f1);
  fread(buf2, sizeof(unsigned char), (height * width), f2);

  fclose(f1);
  fclose(f2);

  return 100*compare(buf1, buf2, (height * width));

}

void csvCreator(){
  //wtf works
  for(int i = 1; i <= 9; i++){
    char *ci = i+'0';
    for (int j = 9; j >= 1; j--){
      char *cj = j+'0';
      printf("%d -> %d,%f", i, j, read(&ci, &cj));
      printf("\n");
    }
  }
}

void main(int argc, char *argv[]){

 //  csvCreator(); // uncomment to make a csv-like copypaste text thing (or ignore (recommended))


// here we check if the filenames are entered as arguments before the execution of the program, and if not, we ask for them.
  if (argc < 2){
    char in1[10];
    char in2[10];

    printf("Input the filename of the first image:  ");
    scanf("%s", in1);
    printf("Input the filename of the second image: ");
    scanf("%s", in2);

    printf("Difference percentage: %lf\n",  read(in1, in2));
  } else {
    printf("Difference percentage: %lf\n",  read(argv[1], argv[2]));
  }
    again();
}
