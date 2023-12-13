/*



*/


#define ASSIGN ;
#include "ourdefs.h"
double rnd()
{
    return (double)(rand()) / (double)(RAND_MAX);
}

int main(int argc,char *argv[]){
int r=0;
int i,j,k;
int c;
int count,size;
double maxval;
double minval;
int top;
int index;
int i1,j1;
int render;
int width,height,max;
int done;

double zt;
char* infilename;
char* filename;
char* string;

int*** pixel;
int* x;
int* y;
double* z;
double* x1;
double* y1;
double* z1;

FILE* infile;
FILE* outfile;

#include "vnum.c"


i=j=k=0;
infilename=malloc(BUFFER*sizeof(char));
filename=malloc(BUFFER*sizeof(char));
string=malloc(BUFFER*sizeof(char));

// open file
    if (argc !=2)
    {
    printf("usage:\n");
    printf("gcodefilter [filename]\n");
    exit(0);
    }

strcpy(infilename,argv[1]);
// count lines and check for %
if((infile=fopen(infilename,"r"))==NULL)
    {
    printf("(%s) bad filename?\n",argv[1]);
    exit(1);
    }
else
    printf("(%s) opened...\n",argv[1]);

/*
P6 128 128 255
[128 * 128 * 3 bytes of data go here, in row major order. The top-left 
 pixel is first, then the pixel to the right of it. When you get to the 
 end of a row, just write the first pixel of the next row. That's all 
 there is. No other header info, no terminators, etc.]

*/
if ((fgetc(infile)=='P')&&(fgetc(infile)=='6'))
	printf("magic P6 ok\n");
else
	{
	printf("bad magic\n");
	exit(1);//
	}
fgetc(infile);

while(isdigit(c=fgetc(infile)))
	string[i++]=(char)c;

string[i]=0;
width=atoi(string);

i=0;
while(isdigit(c=fgetc(infile)))
	string[i++]=(char)c;

string[i]=0;
height=atoi(string);

i=0;
while(isdigit(c=fgetc(infile)))
	string[i++]=(char)c;

string[i]=0;
max=atoi(string);
printf("image is %i x %i\n",width,height);



pixel=malloc(width*sizeof(int**));
for(i=0;i<width;i++)
	pixel[i]=malloc(height*sizeof(int*));

for(i=0;i<width;i++)
	for(j=0;j<height;j++)
		pixel[i][j]=malloc(3*sizeof(int));


double avval=0;
maxval=0;minval=99999999999;
for(j=height-1;j>=0;j--)
for(i=0;i<width;i++)
{
avval+=(double)(pixel[i][j][0]=(255-fgetc(infile)));
avval+=(double)(pixel[i][j][1]=(255-fgetc(infile)));
avval+=(double)(pixel[i][j][2]=(255-fgetc(infile)));
if((pixel[i][j][0]+pixel[i][j][1]+pixel[i][j][2])>maxval) 
	maxval=pixel[i][j][0]+pixel[i][j][1]+pixel[i][j][2];

if((pixel[i][j][0]+pixel[i][j][1]+pixel[i][j][2])<minval) 
	minval=pixel[i][j][0]+pixel[i][j][1]+pixel[i][j][2];

}
fclose(infile);
//printf("avval=%.20g\n",avval); 
avval/=(double)width;
avval/=(double)height;
//printf("maxval=%g  minval=%g  avval(byte)=%g\n\n",maxval,minval,avval/3.0);

count=atoi(readline("count?\n"));
size=atoi(readline("plot size?\n"));
render=atoi(readline("render type(0 straight,  1 old?\n"));
strcpy(filename,readline("filename?\n"));
strcat(filename,".ngc");

x=malloc(count*sizeof(int));
y=malloc(count*sizeof(int));
z=malloc(count*sizeof(double));
x1=malloc(count*sizeof(double));
y1=malloc(count*sizeof(double));
z1=malloc(count*sizeof(double));




// if pixel[i][j][0..2] = 255, it is white
// no, we have changed to -ve
double ra,square,sqmax,coef;
coef=log(avval/3)-log(maxval/3)/log(0.75);// av(rnd)=.75
sqmax=0;
i=0;
while (i<count)
	{
	x[i] = rand() % width;
	y[i] = rand() % height;
	if (pixel[x[i]][y[i]][0]>255)// check we have not already used this point
		done=1;
	else
		done=0;

	// now look a sum of pixels
	// the bigger they are the darker the pixel, the bigger the square
	// if zt > rnd(maxval)(preferably wighted(so 50% are over avval)) square zt*(size/50)/maxval, squares smaller than 2mm rejected
	// (weight is power(rnd,(log(av)-log(max)/log(0.5)))
	zt=(double)(pixel[x[i]][y[i]][0]+pixel[x[i]][y[i]][1]+pixel[x[i]][y[i]][2]);// total of rgb, reversed, darker is higher
	ra=(double)(rand()%((int)maxval));
	if ((zt>ra)&&(render==1)&&(done==0))
		// original render - good with clearly contrasted images
		{
		square=zt*((double)size/50)/maxval;
		if(square>1.5)// it was trying to print .2mm squares
			{
			if (square > (double)size/30.0)
				square=(double)size/30.0;
			z[i]=square;
			i++;
			pixel[x[i]][y[i]][0]=1023; // mark point as used
			}

		}
	else
		//  new render - better for tones / smooth transitions
		{
		zt=(double)(pixel[x[i]][y[i]][0]+pixel[x[i]][y[i]][1]+pixel[x[i]][y[i]][2]);
		square=zt/maxval; // between 0 and 1
		if (square>sqmax) sqmax=square;

		if (square>pow(rnd(),coef))// cutoff reflects brightness of image
		{
		//square=pow(square,1.2);
		square*=(double)size*((768.-avval)/768.0); // a darker picture gets smaller squares
		square/=100.0;// FIXME needs a log here
		if (square>1.5) // nothing too small and white is white
			{
			z[i]=square;
			i++;
			pixel[x[i]][y[i]][0]=1023; // mark point as used
			}
		}
		}
	}
//printf("sqmax=%g\n",sqmax);
//resize keep aspect ratio
for(i=0;i<count;i++)
	{
	x[i]*=size;x[i]/=width;
	y[i]*=size;y[i]/=width;}
int ysize=size*height/width;

//basic sort into 100 boxes / boxes with 40 points, whichever is faster

int istep;
int jstep;
istep=size/10;
jstep=ysize/10;
if (count > 4000)
{
istep=(int)((double)size/sqrt((double)count/100.0));
jstep=(int)((double)size/sqrt((double)count/100.0))*height/width;
}
top=count;
index=0;
for (i=0;i<size;i+=istep)
	for (j=0;j<ysize;j+=jstep)
		{
		i1=i+istep;
		j1=j+jstep;

		for (k=0;k<top;k++)
			if((x[k]>=i)&&(x[k]<i1)&&(y[k]>=j)&&(y[k]<j1))
				{
				x1[index]=(double)x[k];
				y1[index]=(double)y[k];
				z1[index++]=(double)z[k];
				x[k]=x[top];
				y[k]=y[top];
				z[k]=z[top--];
				if (index != count)
					k--; // so we can compare the square we have taken from the top
				}

		}

printf("swapped %i of %i\n",index,count);

//write colour depth as gcode squares
outfile=fopen(filename,"w");

fprintf(outfile,"%%\n");
fprintf(outfile,"(Header)\n");
fprintf(outfile,"(created by %s ver. %i.%i)\n",vname,vmaj/100,vmaj%100);
fprintf(outfile,"(from file %s)\n",infilename);
fprintf(outfile,"(size x:%i y:%i)\n",size,ysize);
fprintf(outfile,"G21 (All units in mm)\n");
fprintf(outfile,"G90 (Absolute positioning)\n");
fprintf(outfile,"F1000.0\n");
fprintf(outfile,"G0 Z 00.0\n");

for(i=0;i<count;i++)
{
fprintf(outfile,"G0 X %g Y%g\n",x1[i],y1[i]);
fprintf(outfile,"G0 Z -30.0\n");
fprintf(outfile,"G1 X %g Y%g\n",x1[i]+z1[i],y1[i]);
fprintf(outfile,"G1 X %g Y%g\n",x1[i]+z1[i],y1[i]+z1[i]);
fprintf(outfile,"G1 X %g Y%g\n",x1[i],y1[i]+z1[i]);
fprintf(outfile,"G1 X %g Y%g\n",x1[i],y1[i]);
fprintf(outfile,"G0 Z 00.0\n");
}
fprintf(outfile,"G0 X 00.0 Y0.00\n");
fprintf(outfile,"%%\n");

fclose(outfile);



return (r);
}


