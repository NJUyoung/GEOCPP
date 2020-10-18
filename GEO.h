

#ifndef GEOCPP2_GEO_H
#define GEOCPP2_GEO_H

#include <vector>
#include <fstream>
#include <queue>

class HEAD
{
public:
    int Naxis_x;
    int Naxis_y;
    int Bscale;
    int Bzero;
    int Data_Position;
    float Average;
    float Stdev;
    int TYPE(std::string File);
    void NAXIS(std::string File,int &Naxis_x,int &Naxis_y);
    void BVALUE(std::string File,int &Bscale,int &Bzero);
    int DATA_POSITION(std::string File);
    void CRPIX(std::string File, float &Crpix1, float &Crpix2);
    void CRVAL(std::string File, float &Crval1, float &Crval2);

};

class DATAPART{
public:
    void READING16(int Data_Position,int Naxis_x,int Naxis_y,int Bscale,int Bzero,
            std::vector<std::vector<unsigned int> > &DATA,
            std::vector<std::vector<unsigned int> > &DATA2,
            std::vector<std::vector<unsigned int> > &ORIGIN_DATA,std::ifstream& file);
    void READING32(int Data_Position,int Naxis_x,int Naxis_y,int Bscale,int Bzero,
            std::vector<std::vector<unsigned int> > &DATA,
            std::vector<std::vector<unsigned int> > &DATA2,
            std::vector<std::vector<unsigned int> > &ORIGIN_DATA,std::ifstream& file);
    int SUM(int Naxis_y,std::vector<std::vector<unsigned int> > ORIGIN_DATA);
};

class PREPROCESSING
{
public:
    void PART(int Naxis_x,int Naxis_y,
              std::vector<std::vector<unsigned int> > &DATA,
              std::vector<std::vector<unsigned int> >ORIGIN_DATA);
    void ALL(int Naxis_x,int Naxis_y,
             std::vector<std::vector<unsigned int> > &DATA,
             std::vector<std::vector<unsigned int> > &DATA2,
             std::vector<std::vector<unsigned int> > ORIGIN_DATA,
             float &Average, float &Stdev);
    void DISCRETE(int Naxis_x,int Naxis_y,
                  std::vector<std::vector<unsigned int> > &DATA,
                  std::vector<std::vector<unsigned int> > &DATA2);
    void ADDNEW(int Naxis_x,int Naxis_y,
                std::vector<std::vector<unsigned int> > &DATA,
                std::vector<std::vector<unsigned int> > &DATA2);
    void TWOVALUE(int Naxis_x,int Naxis_y,
            std::vector<std::vector<unsigned int> > &DATA);
    int POSITIVE(int Naxis_x, int Naxis_y,
            std::vector<std::vector<unsigned int> > DATA);
    void FILTER3(int Naxis_x,int Naxis_y,
            std::vector<std::vector<unsigned int> > &DATA,
            std::vector<std::vector<unsigned int> > ORIGIN_DATA);
    void NEWPART(int Naxis_x,int Naxis_y,
            std::vector<int> &X_label,std::vector<int> &Y_label,
            std::vector<std::vector<unsigned int> > ORIGIN_DATA);
    void PART2(int Naxis_x,int Naxis_y,
          std::vector<std::vector<unsigned int> > &DATA,
          std::vector<int> X_label,std::vector<int> Y_label);
};

struct Point
{
    int r;
    int c;
    Point(int r_, int c_) : r(r_), c(c_) {}
    Point(const Point& p) : r(p.r), c(p.c) {}
};

class SOLUTION
{
public:
    int m;
    int n;
    bool isvalid(int m, int n, int i, int j, std::vector<std::vector<unsigned int> > &matrix,
            std::vector<std::vector<bool> > &mask);
    void add(int m, int n, int i, int j, std::vector<std::vector<unsigned int> > &matrix,
            std::queue<Point> &q, std::vector<std::vector<bool> > &mask);
};

std::vector<std::vector<Point> > bwlabel(int m, int n, std::vector<std::vector<unsigned int> > &matrix);
std::vector<float> changex(std::vector<float> x, std::vector<float> y, float xc, float yc, float theta);
std::vector<float> changey(std::vector<float> x, std::vector<float> y, float xc, float yc, float theta);
int s(float x);
std::vector<float>f_point(std::vector<float>x, std::vector<float>y, std::vector<float>A);
std::vector<float>result(std::vector<float>x, std::vector<float>y, std::vector<float>F, std::vector<float>A);



#endif //GEOCPP2_GEO_H
