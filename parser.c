#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"


/*======== void parse_file () ==========
Inputs:   char * filename 
          struct matrix * transform, 
          struct matrix * pm,
          screen s
Returns: 

Goes through the file named filename and performs all of the actions listed in that file.
The file follows the following format:
     Every command is a single character that takes up a line
     Any command that requires arguments must have those arguments in the second line.
     The commands are as follows:
         line: add a line to the edge matrix - 
	    takes 6 arguemnts (x0, y0, z0, x1, y1, z1)
	 ident: set the transform matrix to the identity matrix - 
	 scale: create a scale matrix, 
	    then multiply the transform matrix by the scale matrix - 
	    takes 3 arguments (sx, sy, sz)
	 translate: create a translation matrix, 
	    then multiply the transform matrix by the translation matrix - 
	    takes 3 arguments (tx, ty, tz)
	 rotate: create an rotation matrix,
	    then multiply the transform matrix by the rotation matrix -
	    takes 2 arguments (axis, theta) axis should be x y or z
	 apply: apply the current transformation matrix to the 
	    edge matrix
	 display: draw the lines of the edge matrix to the screen
	    display the screen
	 save: draw the lines of the edge matrix to the screen
	    save the screen to a file -
	    takes 1 argument (file name)
	 quit: end parsing

See the file script for an example of the file format


IMPORTANT MATH NOTE:
the trig functions int math.h use radian mesure, but us normal
humans use degrees, so the file will contain degrees for rotations,
be sure to conver those degrees to radians (M_PI is the constant
for PI)
====================*/
void parse_file ( char * filename, 
                  struct matrix * transform, 
                  struct matrix * edges,
                  screen s) {

  FILE *f;
  char line[256];
  clear_screen(s);
  int argstatus = 0;
  color c;
  c.red = MAX_COLOR;
  c.green = MAX_COLOR;
  c.blue = 0;

  /*
0 is null
1 is save
2 is line
3 is move
4 is scale
5 is rotate
  */

  if ( strcmp(filename, "stdin") == 0 ) 
    f = stdin;
  else
    f = fopen(filename, "r");
  
  while ( fgets(line, 255, f) != NULL ) {
    line[strlen(line)-1]='\0';
    printf(":%s:\n",line);
    //print_matrix(edges);
    if(!strcmp(line,"ident")){
      ident(transform);
    }
    else if(!strcmp(line,"apply")){
      matrix_mult(transform,edges);
    }
    else if(!strcmp(line,"display")){
      clear_screen(s);
      draw_lines(edges,s,c);
      display(s);
    }
    else if(!strcmp(line,"save")){
      argstatus = 1;
    }
    else if(!strcmp(line,"line")){
      argstatus = 2;
    }
    else if(!strcmp(line,"move")){
      argstatus = 3;
    }
    else if(!strcmp(line,"scale")){
      argstatus = 4;
    }
    else if(!strcmp(line,"rotate")){
      argstatus = 5;
    }
    else{
      char * line2 = (char*)malloc(200);
      char * line3 = strcpy(line2, line);
      char ** arr = (char**)malloc(6 * sizeof(char*));
      int i = 0;
      while(line3){
	arr[i] = strsep(&line3," ");
	i++;
      }
      arr[i] = NULL;
      if(argstatus == 1){
	save_extension(s,line);
      }
      else if(argstatus == 2){
	add_edge(edges,
		 atof(arr[0]),
		 atof(arr[1]),
		 atof(arr[2]),
		 atof(arr[3]),
		 atof(arr[4]),
		 atof(arr[5])
		 );
      }      
      else if(argstatus == 3){
	struct matrix * m = make_translate(
					   atof(arr[0]),
					   atof(arr[1]),
					   atof(arr[2]));
	matrix_mult(m,transform);
      }
      else if(argstatus == 4){
	struct matrix * m = make_scale(
				       atof(arr[0]),
				       atof(arr[1]),
				       atof( arr[2]));
	matrix_mult(m,transform);
      }
      else if(argstatus == 5){
	if(!strcmp(arr[0],"x")){
	  struct matrix * m = make_rotX(M_PI*atof(arr[1])/180);
	  matrix_mult(m,transform);
	}
	else if(!strcmp(arr[0],"y")){
	  struct matrix * m = make_rotY(M_PI*atof(arr[1])/180);
	  matrix_mult(m,transform);
	}
	else{
	  struct matrix * m = make_rotZ(M_PI*atof(arr[1])/180);
	  matrix_mult(m,transform);
	}
 
      }
    }
  }
}
  
