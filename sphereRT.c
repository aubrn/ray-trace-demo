#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Linux OpenGL Headers
/*
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
*/

// MacOS OpenGL Headers
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

// window size
#define SIZE 1024
#define nvalue 20

float lightX, lightY, lightZ, sphereR, sphereX, sphereY, sphereZ, ambientR, ambientG, ambientB, diffuseR, diffuseG, diffuseB, specularR, specularG, specularB = 0;

/* calculate the length of a vector */
float length(float *x, float *y, float *z)
{
   return (sqrtf((*x * *x) + (*y * *y) + (*z * *z)));
}

/* creates a unit vector */
/* divide a vector by its own length */
void normalize(float *xd, float *yd, float *zd)
{
   float len;
   len = length(xd, yd, zd);
   *xd = *xd / len;
   *yd = *yd / len;
   *zd = *zd / len;
}

void display()
{
   int i, j;
   float x, y;
   float r, g, b;

   float x0, y0, z0; // viewpoint origin
   float xd, yd, zd; // view ray direction

   glClearColor(0.0, 0.0, 0.0, 1.0);
   glClear(GL_COLOR_BUFFER_BIT);

   glPointSize(1.0);

   x0 = 0.0;
   y0 = 0.0;
   z0 = 0.0;

   glBegin(GL_POINTS);

   for (i = 0; i < SIZE; i++)
   {
      for (j = 0; j < SIZE; j++)
      {
         x = (float)(i - (SIZE / 2)) / (SIZE / 2);
         y = (float)(j - (SIZE / 2)) / (SIZE / 2);

         xd = ((float)(i - (SIZE / 2)) / (SIZE / 2));
         yd = ((float)(j - (SIZE / 2)) / (SIZE / 2));
         zd = -1.0;
         normalize(&xd, &yd, &zd);


         /* calculate B, C.  A == 1.0 */
         float B = 2 * (xd * (x0 - sphereX) + yd * (y0 - sphereY) + zd * (z0 - sphereZ));
         float C = (x0 - sphereX) * (x0 - sphereX) + (y0 - sphereY) * (y0 - sphereY) + (z0 - sphereZ) * (z0 - sphereZ) - (sphereR * sphereR);

         /* calculate the discriminant */

         float D = ((B * B) - (4 * C));

         float t0 = ((-1 * B) + sqrt(D)) / 2;
         float t1 = ((-1 * B) - sqrt(D)) / 2;
         float length = 0;
         float l0, l1 = 0;
         if (t0 > t1)
         {
            length = t1;
         }
         else
         {
            length = t0;
         }

         float intersectX = x0 + xd * length;
         float intersectY = y0 + yd * length;
         float intersectZ = z0 + zd * length;

         /* calculate normal vector (nx, ny, nz) to the intersection point */
         float nx, ny, nz = 0;
         nx = (intersectX - sphereX) / sphereR;
         ny = (intersectY - sphereY) / sphereR;
         nz = (intersectZ - sphereZ) / sphereR;
         normalize(&nx, &ny, &nz);

         /* calculate viewing vector (vx, vy, vz) */
         float vx, vy, vz = 0;
         vx = x0 - intersectX;
         vy = y0 - intersectY;
         vz = z0 - intersectZ;
         normalize(&vx, &vy, &vz);

         /* calculate the light vector (lx, ly, lz)  */
         float lx, ly, lz = 0;
         lx = lightX - intersectX;
         ly = lightY - intersectY;
         lz = lightZ - intersectZ;
         normalize(&lx, &ly, &lz);

         /* calculate the dot product N.L, using the normal vector
		   and the light vector */
         float NdotL = (nx * lx) + (ny * ly) + (nz * lz);

         /* calculate the reflection vector (rx, ry, rz) */
         float rx, ry, rz = 0;
         rx = 2 * (NdotL)*nx - lx;
         ry = 2 * (NdotL)*ny - ly;
         rz = 2 * (NdotL)*nz - lz;
         normalize(&rx, &ry, &rz);

         /* calculate the dot product R.V, using the reflection vector
		   and the viewing vector */
         float RdotV = ((rx * vx) + (ry * vy) + (rz * vz));
         if (RdotV < 0)
         {
            RdotV = 0;
         }
         RdotV = powf(RdotV, nvalue);

         r = ambientR + diffuseR * NdotL + specularR * RdotV;
         g = ambientG + diffuseG * NdotL + specularG * RdotV;
         b = ambientB + diffuseB * NdotL + specularB * RdotV;

         glColor3f(r, g, b);
         glVertex2f(x, y);
      } 
   }    

   glEnd();

   glFlush();
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key)
   {
   case 27:
   case 'q':
      exit(0);
      break;
   }
}

int main(int argc, char **argv)
{

   char line[128];
   printf("%s\n", argv[1]);
   FILE *file;
   char *temp;
   file = fopen(argv[1], "r");
   if (file)
   {
      fgets(line, sizeof(line), file);
      temp = strtok(line, " \t\n\r");
      if (strcmp(temp, "light") != 0)
      {
         printf("File not formatted correctly");
         return 0;
      }
      temp = strtok(NULL, " \t\n\r");
      lightX = strtof(temp, NULL);
      temp = strtok(NULL, " \t\n\r");
      lightY = strtof(temp, NULL);
      temp = strtok(NULL, " \t\n\r");
      lightZ = strtof(temp, NULL);

      fgets(line, sizeof(line), file);
      temp = strtok(line, " \t\n\r");
      if (strcmp(temp, "sphere") != 0)
      {
         printf("File not formatted correctly");
         return 0;
      }
      temp = strtok(NULL, " \t\n\r");
      sphereX = strtof(temp, NULL);
      temp = strtok(NULL, " \t\n\r");
      sphereY = strtof(temp, NULL);
      temp = strtok(NULL, " \t\n\r");
      sphereZ = strtof(temp, NULL);
      temp = strtok(NULL, " \t\n\r");
      sphereR = strtof(temp, NULL);
      temp = strtok(NULL, " \t\n\r");
      ambientR = strtof(temp, NULL);
      temp = strtok(NULL, " \t\n\r");
      ambientG = strtof(temp, NULL);
      temp = strtok(NULL, " \t\n\r");
      ambientB = strtof(temp, NULL);
      temp = strtok(NULL, " \t\n\r");
      diffuseR = strtof(temp, NULL);
      temp = strtok(NULL, " \t\n\r");
      diffuseG = strtof(temp, NULL);
      temp = strtok(NULL, " \t\n\r");
      diffuseB = strtof(temp, NULL);
      temp = strtok(NULL, " \t\n\r");
      specularR = strtof(temp, NULL);
      temp = strtok(NULL, " \t\n\r");
      specularG = strtof(temp, NULL);
      temp = strtok(NULL, " \t\n\r");
      specularB = strtof(temp, NULL);
   }
   else
   {
      printf("File could not be opened, sphere information mandatory\n");
      return 0;
   }

   /* Initialize OpenGL and GLUT */
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize(SIZE, SIZE);
   glutCreateWindow("Ray Tracing Demo - q key to quit");
   glutDisplayFunc(display);
   glutKeyboardFunc(keyboard);

   glutMainLoop();
   return 0;
}
