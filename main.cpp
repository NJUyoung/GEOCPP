#include <iostream>
#include <fstream>
#include <time.h>
#include <string>
#include <vector>
#include <math.h>
#include "GEO.h"
float  linefit(std::vector<float>x,std::vector<float>y)
{
    float t1=0,t2=0,t3=0,t4=0,a,b;
    for(int i=0;i<x.size();i++)
    {
        t1+=x[i]*x[i];
        t2+=x[i];
        t3+=x[i]*y[i];
        t4+=y[i];
    }
    a=(t3*x.size()-t2*t4)/(t1*x.size()-t2*t2);
    //b=(t4-a*t2)/x.size();
    return a;
}
int main(int argc,char* argv[]) {
    clock_t start,end;
    start = clock();
    std::ifstream file;
    file.open(argv[1],std::ios::binary|std::ios::beg);
    std::string File;
    getline(file,File);

    HEAD READHEAD;
    READHEAD.Naxis_x = 0;
    READHEAD.Naxis_y = 0;
    READHEAD.Bscale = 0;
    READHEAD.Bzero = 0;
    READHEAD.Data_Position = 0;
    READHEAD.Average = 0;
    READHEAD.Stdev = 0;
    READHEAD.Data_Position = READHEAD.DATA_POSITION(File);
    int type = READHEAD.TYPE(File);
    READHEAD.NAXIS(File,READHEAD.Naxis_x,READHEAD.Naxis_y);
    READHEAD.BVALUE(File,READHEAD.Bscale,READHEAD.Bzero);

    std::vector<std::vector<unsigned int> > DATA(READHEAD.Naxis_y,std::vector<unsigned int>(READHEAD.Naxis_x,0));
    std::vector<std::vector<unsigned int> > DATA2(READHEAD.Naxis_y,std::vector<unsigned int>(READHEAD.Naxis_x,0));
    std::vector<std::vector<unsigned int> > ORIGIN_DATA(READHEAD.Naxis_y,std::vector<unsigned int>(READHEAD.Naxis_x,0));
    std::vector<int> X_label,Y_label;

    DATAPART READDATA;
    if(type == 8){
        std::cout << "8" << std::endl;
    }
    if(type == 16){
        READDATA.READING16(READHEAD.Data_Position,READHEAD.Naxis_x,READHEAD.Naxis_y,READHEAD.Bscale,
                           READHEAD.Bzero,DATA,DATA2,ORIGIN_DATA,file);
    }
    if(type == 32) {
        std::cout << "32" << std::endl;
        READDATA.READING32(READHEAD.Data_Position,READHEAD.Naxis_x,READHEAD.Naxis_y,READHEAD.Bscale,
                           READHEAD.Bzero,DATA,DATA2,ORIGIN_DATA,file);
    }

    int SUMdata = READDATA.SUM(READHEAD.Naxis_y,ORIGIN_DATA);

    PREPROCESSING PRE;
    PRE.FILTER3(READHEAD.Naxis_x,READHEAD.Naxis_y,DATA,ORIGIN_DATA);
    PRE.NEWPART(READHEAD.Naxis_x,READHEAD.Naxis_y,X_label,Y_label,ORIGIN_DATA);
    //分块处理
    //PRE.PART2(READHEAD.Naxis_x,READHEAD.Naxis_y,DATA,X_label,Y_label);
    PRE.PART(READHEAD.Naxis_x,READHEAD.Naxis_y,DATA,ORIGIN_DATA);
    //全局阈值过滤
    PRE.ALL(READHEAD.Naxis_x,READHEAD.Naxis_y,DATA,DATA2,ORIGIN_DATA,READHEAD.Average,READHEAD.Stdev);
    //离散点过滤
    //PRE.DISCRETE(READHEAD.Naxis_x,READHEAD.Naxis_y,DATA,DATA2);
    //PRE.ADDNEW(READHEAD.Naxis_x,READHEAD.Naxis_y,DATA,DATA2);
    //PRE.TWOVALUE(READHEAD.Naxis_x,READHEAD.Naxis_y,DATA2);
    //PRE.POSITIVE(READHEAD.Naxis_x,READHEAD.Naxis_y,DATA);

    std::vector<std::vector<Point> >res = bwlabel(READHEAD.Naxis_x,READHEAD.Naxis_y,DATA2);
    std::vector<int> xmin, ymax;
    for(size_t i = 0; i < res.size(); i++)
    {
        if(res[i].size()>5)
        {
            int x = READHEAD.Naxis_x,y = 0;
            for(size_t j=0; j<res[i].size(); j++)
            {
                if(res[i][j].r >= y)
                {
                    y = res[i][j].r;
                    if(res[i][j].c < x)
                        x = res[i][j].c;
                }
            }
            xmin.push_back(x);
            ymax.push_back(y);
            std::cout << x << " " << y << " " << ";";
        }
    }


    int COUNT = 0;
    bool a = 0;
    bool b = 0;
    if(a == 0)
    {
        for (size_t i = 0; i < res.size(); i++) {
            if(res[i].size() > 5)
            {
                COUNT += 1;
                std::cout << "第" << COUNT + 1 << "个的位置";
                std::vector<float> x, y;
                std::vector<float> flux;
                for (size_t j = 0; j < res[i].size(); j++) {
                    //std::cout << "[" << res[i][j].r << "," << res[i][j].c << "] ";
                    x.push_back(res[i][j].c);
                    y.push_back(res[i][j].r);
                    flux.push_back(DATA[res[i][j].r][res[i][j].c]);
                }

                int xc,yc;
                xc=x[int(x.size()/2)];
                yc=y[int(y.size()/2)];
                float q;
                q=atan(linefit(x,y));


                int h,v,fluxmax,xmax,xmin,ymax,ymin;

                //int xmax, ymax, xmin, ymin;
                xmax = x[0];
                ymax = y[0];
                xmin = x[0];
                ymin = y[0];
                fluxmax=flux[0];
                for (size_t i = 0; i < x.size(); i++) {
                    if (x[i] > xmax) {
                        xmax = x[i];
                    }
                    if (x[i] < xmin) {
                        xmin = x[i];
                    }
                    if (y[i] > ymax) {
                        ymax = y[i];
                    }
                    if (y[i] < ymin) {
                        ymin = y[i];
                    }
                    if(flux[i]>fluxmax){
                        fluxmax=flux[i];
                    }
                }

                //before
                /*float xc, yc;
                xc = (xmax + xmin) / 2;
                yc = (ymax + ymin) / 2;
                int h, v;
                h = xmax - xmin;
                v = ymax - ymin;
                float theta = 0;
                if (h == 0) {
                    theta = 3.14 / 2;
                } else {
                    theta = atan2(v,h);
                }
                 */
                //before
                v=xmax-xmin;
                h=ymax-ymin;
                std::cout << "x: " << xc << " y: " << yc << std::endl;
                std::cout << "q: " << q << std::endl;
                std::cout<<"宽度"<<v<<std::endl;
                std::cout<<"高度"<<h<<std::endl;
                std::cout << "个数 " << res[i].size() << std::endl;
                std::cout << "中心亮度 " << ORIGIN_DATA[xc][yc] << std::endl;
                std::cout<<"最大亮度"<<fluxmax<<std::endl;
                /*
                if(h<20 and v<20 and abs(h - v) < 4  and res[i].size() > 10 )
                {
                    std::cout << "x " << xc << " y " << yc << " h " << h << " v " << v << std::endl;
                    std::cout << "夹角" << theta << std::endl;
                    std::cout << "个数 " << res[i].size() << std::endl;
                    std::cout << "符合碎片" << std::endl;
                }
                else if(v<20 and h<70 and h>20)
                {
                    std::cout << "x " << xc << " y " << yc << " h " << h << " v " << v << std::endl;
                    std::cout << "夹角" << theta << std::endl;
                    std::cout << "个数 " << res[i].size() << std::endl;
                    std::cout << "符合恒星" << std::endl;
                }
                else
                {
                    std::cout << "x " << xc << " y " << yc << " h " << h << " v " << v << std::endl;
                    std::cout << "夹角" << theta << std::endl;
                    std::cout << "个数 " << res[i].size() << std::endl;
                    std::cout << "不符合" << std::endl;
                }
                 */
                /*
                if (b == 1)
                {
                    std::vector<float> xnew, ynew, fnew;
                    xnew = changex(x, y, xc, yc, theta);
                    ynew = changey(x, y, xc, yc, theta);
                    fnew = f_point(xnew, ynew, flux);
                    std::vector<float> re;
                    //std::cout << i << " " << flux.size() << " " << flux[flux.size()-1]<< std::endl;
                    re = result(xnew, ynew, fnew, flux);

                    std::vector<int> xf, yf;
                    std::vector<float> ff;
                    for (size_t i = 0; i < re.size(); i++)
                    {
                        if (re[i] > 150) {
                            xf.push_back(x[i]);
                            yf.push_back(y[i]);
                            ff.push_back(re[i]);
                        }

                    }
                    if(xf.size()>0)
                        COUNT += 1;
                    for (size_t k = 0; k < xf.size(); k++)
                    {
                        std::cout << "第" << k << "个x坐标" << xf[k] << std::endl;
                        std::cout << "第" << k << "个y坐标" << yf[k] << std::endl;
                        std::cout << "第" << k << "个光度" << ff[k] << std::endl;
                    }

                    //std::cout << std::endl;
                }*/
            }

        }
    }

    end = clock();
    double time = double(end - start)/CLOCKS_PER_SEC;

    std::string str;
    while(std::cin >> str){

        if(str == "bscale")
            std::cout << READHEAD.Bscale << std::endl;
        if(str == "bzero")
            std::cout << READHEAD.Bzero << std::endl;
        if(str == "type")
            std::cout << type << std::endl;

        if(str == "sum")
        {
            int Sum = READDATA.SUM(READHEAD.Naxis_y,ORIGIN_DATA);
            std::cout << Sum << std::endl;
        }

        if(str == "value")
        {
            int x_label,y_label;
            std::cout << "x_label: ";
            std::cin >> x_label;
            std::cout << "y_label: ";
            std::cin >> y_label;
            std::cout << ORIGIN_DATA[x_label][y_label] << std::endl;
        }
        if(str == "newvalue")
        {
            int x_label,y_label;
            std::cout << "x_label: ";
            std::cin >> x_label;
            std::cout << "y_label: ";
            std::cin >> y_label;
            std::cout << DATA[x_label][y_label] << std::endl;
        }

        if(str == "count")
            std::cout << COUNT << std::endl;
        if(str == "time")
            std::cout << time << std::endl;

        if(str == "mean")
            std::cout << READHEAD.Average << std::endl;
        if(str == "stdev")
            std::cout << READHEAD.Stdev << std::endl;
        if(str == "naxis1")
            std::cout << READHEAD.Naxis_x << std::endl;
        if(str == "naxis2")
            std::cout << READHEAD.Naxis_y << std::endl;
        if(str == "addnew")
            PRE.ADDNEW(READHEAD.Naxis_x,READHEAD.Naxis_y,DATA,DATA2);//延展目标恢复，仅在连通域时使用
        if(str == "twovalue")
            PRE.TWOVALUE(READHEAD.Naxis_x,READHEAD.Naxis_y,DATA);//若需要可换成0-1值的矩阵
        if(str == "exit")
            break;
    }
    return 0;
}