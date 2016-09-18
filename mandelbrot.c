#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

typedef struct{
        double r, g, b;
}_color;

int process_point(double cx, double cy, int er, int bailout) {
    int    i;
    double zx , zxn , zy , zyn;
    double zx2, zxn2, zy2, zyn2;

    zx  = 0.0;
    zy  = 0.0;
    zx2 = 0.0;
    zy2 = 0.0;

    for ( i = 0; i < bailout; i++ ){
        zxn = zx * zx - zy * zy + cx;
        zyn = 2.0 * zx * zy + cy;
        zx  = zxn; zy  = zyn;

        zxn = zx * zx - zy * zy + cx;
        zyn = 2.0 * zx * zy + cy;
        zx  = zxn; zy  = zyn;

        zxn2 = zx2 * zx2 - zy2 * zy2 + cx;
        zyn2 = 2.0 * zx2 * zy2 + cy;
        zx2  = zxn2; zy2  = zyn2;

        if ( zx2 == zx && zy2 == zy )
            return 0;

        if ( zx * zx + zy * zy > er * er )
            return i;
    }

    return 0;
}

int main(){
    int     block_size;
    int     screenx, screeny,
            ix, iy,
            i, j,
            er, bailout;
    double  zx, zy, zxn, zyn, cx, cy,
            minx, maxx, miny, maxy,
            xcenter, ycenter, size;

    FILE *img = fopen("mandel.ppm","wt");
    _color col;
    int *escapetime;

    screenx  =  1600/2;
    screeny  =  1200/2;
    bailout  =  5000;
    er       =   2;
    minx     =  -2.5;
    maxx     =   1.5;
    miny     =  -2.0;
    maxy     =  miny * -1;

    printf("%f \t %f\n%f\t %f\n", minx, maxx, miny, maxy);

    escapetime  =  (int*)malloc(sizeof(int)*screenx*screeny);
    fprintf(img, "P3\n%d %d\n255\n", screenx, screeny);

    for(iy = 0; iy < screeny; iy++ ){
        cy = miny+iy*(maxy-miny)/screeny;
        for ( ix = 0; ix < screenx; ix++ ) {
            cx = minx + ix * (maxx - minx) / screenx;
            escapetime[iy * screenx + ix] = process_point(cx, cy, er, bailout);
        }

        if ( iy % ( screeny / 10 ) == 0 ) {
            fprintf(stderr," -- %.2f%%\n",(iy/(double)screeny)*100.0);
        }
    }

    fprintf(stderr," -- %.2f%%\n",100.0);
    fprintf(stderr," <---- DONE ---->\n");
    fprintf(stderr," Writing to disk!\n");

    for(i = 0;i<screeny;i++){
        for(j = 0;j<screenx;j++){
            col.r = escapetime[i*screenx+j];
            fprintf(img,"%d %d %d ",
                    (int)col.r * 7,
                    (int)col.r * 7,
                    (int)col.r * 7);
        }
        fputc('\n',img);
    }

    fclose(img);
    free(escapetime);

    fprintf(stderr," -- Bye\n");

    return EXIT_SUCCESS;
}
