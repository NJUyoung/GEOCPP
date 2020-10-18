
#include "GEO.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>

void PREPROCESSING::PART(int Naxis_x,int Naxis_y,
        std::vector<std::vector<unsigned int> > &DATA,
        std::vector<std::vector<unsigned int> >ORIGIN_DATA)
{
    int Xmax = Naxis_x/200;
    int Ymax = Naxis_y/200;
    for(int i=0; i < Ymax; i++)
    {
        for(int j=0; j < Xmax; j++)
        {
            if(i==Ymax-1)
            {
                if(j==Xmax-1)
                {
                    std::vector<int> SORT;
                    for(int m=i*200; m<Naxis_y; m++)
                        for(int n=j*200; n<Naxis_x; n++)
                        {
                            SORT.push_back(ORIGIN_DATA[m][n]);
                        }

                    std::sort(SORT.begin(),SORT.end());
                        int Median = SORT[SORT.size()/2];
                        for(int m=i*200; m<Naxis_y; m++)
                            for(int n=j*200; n<Naxis_x; n++)
                            {
                                if(DATA[m][n]>Median)
                                    DATA[m][n] -= Median;
                                else
                                    DATA[m][n] = 1;
                            }
                }
                else
                {
                    std::vector<int> SORT;
                    for(int m=i*200; m<Naxis_y; m++)
                        for(int n=j*200; n<j*200+200; n++)
                        {
                            SORT.push_back(ORIGIN_DATA[m][n]);
                        }

                    std::sort(SORT.begin(),SORT.end());
                    int Median = SORT[SORT.size()/2];
                    for(int m=i*200; m<Naxis_y; m++)
                        for(int n=j*200; n<j*200+200; n++)
                        {
                            if(DATA[m][n]>Median)
                                DATA[m][n] -= Median;
                            else
                                DATA[m][n] = 1;
                        }
                }
            }
            else
            {
                if(j==Xmax-1)
                {
                    std::vector<int> SORT;
                    for(int m=i*200; m<i*200+200; m++)
                        for(int n=j*200; n<Naxis_x; n++)
                        {
                            SORT.push_back(ORIGIN_DATA[m][n]);
                        }

                    std::sort(SORT.begin(),SORT.end());
                    int Median = SORT[SORT.size()/2];
                    for(int m=i*200; m<i*200+200; m++)
                        for(int n=j*200; n<Naxis_x; n++)
                        {
                            if(DATA[m][n]>Median)
                                DATA[m][n] -= Median;
                            else
                                DATA[m][n] = 1;
                        }
                }
                else
                {
                    std::vector<int> SORT;
                    for(int m=i*200; m<i*200+200; m++)
                        for(int n=j*200; n<j*200+200; n++)
                        {
                            SORT.push_back(ORIGIN_DATA[m][n]);
                        }

                    std::sort(SORT.begin(),SORT.end());
                    int Median = SORT[SORT.size()/2];
                    for(int m=i*200; m<i*200+200; m++)
                        for(int n=j*200; n<j*200+200; n++)
                        {
                            if(DATA[m][n]>Median)
                                DATA[m][n] -= Median;
                            else
                                DATA[m][n] = 1;
                        }
                }
            }
        }
    }
};

void PREPROCESSING::ALL(int Naxis_x,int Naxis_y,
        std::vector<std::vector<unsigned int> > &DATA,
        std::vector<std::vector<unsigned int> > &DATA2,
        std::vector<std::vector<unsigned int> > ORIGIN_DATA,
        float &Average, float &Stdev)
{
    float SUM = 0;
    for(int i = 0; i < Naxis_y; i++)
    {
        float sum = std::accumulate(ORIGIN_DATA[i].begin(),ORIGIN_DATA[i].end(),0.0);
        SUM += sum;
    }


    Average = SUM/(Naxis_x*Naxis_y);
    float accum = 0;
    for(int i = 0; i < Naxis_y; i++)
    {
        for(int j = 0; j < Naxis_x; j++)
        {
            accum += (ORIGIN_DATA[i][j]-Average)*(ORIGIN_DATA[i][j]-Average);
        }
    }
    Stdev = sqrt(accum/(Naxis_x*Naxis_y));

    float k = log10(65535/Stdev) - 1;
    float thres = Average + k*Stdev;
    for(int i = 0; i < Naxis_y; i++)
    {
        for(int j = 0; j < Naxis_x; j++)
        {
            if(ORIGIN_DATA[i][j] < thres )
            {
                DATA2[i][j] = 0;
                DATA[i][j] = 0;
            }
            else
                DATA2[i][j] = 1;
        }
    }
}

void PREPROCESSING::FILTER3(int Naxis_x,int Naxis_y,
        std::vector<std::vector<unsigned int> > &DATA,
        std::vector<std::vector<unsigned int> > ORIGIN_DATA)
{
    for(int i = 1; i < Naxis_y - 1; i++)
        for(int j = 1; j < Naxis_x - 1; j++)
        {
            DATA[i][j] = 0.25*ORIGIN_DATA[i][j] + 0.125 * (ORIGIN_DATA[i-1][j] + ORIGIN_DATA[i+1][j]
                    + ORIGIN_DATA[i][j-1] + ORIGIN_DATA[i][j+1]) + 0.0625 * (ORIGIN_DATA[i-1][j-1] +
                            ORIGIN_DATA[i+1][j-1] + ORIGIN_DATA[i+1][j+1] + ORIGIN_DATA[i-1][j+1]);
        }
}

void PREPROCESSING::NEWPART(int Naxis_x,int Naxis_y,
        std::vector<int> &X_label,std::vector<int> &Y_label,
        std::vector<std::vector<unsigned int> > ORIGIN_DATA)
{
    /*
    std::vector<int> ORIGIN_X;
    std::vector<int> ORIGIN_Y;
    for(int i=1; i < Naxis_y-1; i++)
    {
        float SUM = 0;
        SUM = std::accumulate(ORIGIN_DATA[i-1].begin(),ORIGIN_DATA[i-1].end(),0.0);
        SUM -= std::accumulate(ORIGIN_DATA[i+1].begin(),ORIGIN_DATA[i+1].end(),0.0);
        Y_label.push_back(abs(SUM));
        ORIGIN_Y.push_back(abs(SUM));
    }
    std::sort(Y_label.begin(),Y_label.end());
    int k = 2,l=3;
    std::vector<int>::iterator item1 = find(ORIGIN_Y.begin(),ORIGIN_Y.end(),Y_label[Naxis_y-1]);
    std::vector<int>::iterator item2 = find(ORIGIN_Y.begin(),ORIGIN_Y.end(),Y_label[Naxis_y-k]);
    while(abs(item1-item2)<20)
    {
        std::vector<int>::iterator item2 = find(ORIGIN_Y.begin(),ORIGIN_Y.end(),Y_label[Naxis_y-k]);
    }
    std::vector<int>::iterator item3 = find(ORIGIN_Y.begin(),ORIGIN_Y.end(),Y_label[Naxis_y-l]);
    while(abs(item2-item3)<20||abs(item1-item3)<20)
    {
        std::vector<int>::iterator item3 = find(ORIGIN_Y.begin(),ORIGIN_Y.end(),Y_label[Naxis_y-l]);
    }
    int index1 = std::distance(std::begin(ORIGIN_Y),item1);
    int index2 = std::distance(std::begin(ORIGIN_Y),item2);
    int index3 = std::distance(std::begin(ORIGIN_Y),item3);
    Y_label.clear();
    Y_label.push_back(0);
    Y_label.push_back(index1);
    Y_label.push_back(index2);
    Y_label.push_back(index3);
    std::sort(Y_label.begin(),Y_label.end());
    for(int j=1; j < Naxis_x-1; j++)
    {
        float SUM = 0;
        for(int i=0; i < Naxis_y; i++)
        {
            SUM += ORIGIN_DATA[i][j-1] - ORIGIN_DATA[i][j+1];
        }
        X_label.push_back(abs(SUM));
        ORIGIN_X.push_back(abs(SUM));
    }
    std::sort(X_label.begin(),X_label.end());
    std::vector<int>::iterator xtem1 = find(ORIGIN_X.begin(),ORIGIN_X.end(),X_label[Naxis_x-1]);
    int i = 2,j = 3;
    std::vector<int>::iterator xtem2 = find(ORIGIN_X.begin(),ORIGIN_X.end(),X_label[Naxis_x-i]);
    while(abs(xtem1-xtem2)<20)
    {
        std::vector<int>::iterator xtem2 = find(ORIGIN_X.begin(),ORIGIN_X.end(),X_label[Naxis_x-i]);
        i += 1;
    }
    std::vector<int>::iterator xtem3 = find(ORIGIN_X.begin(),ORIGIN_X.end(),X_label[Naxis_x-j]);
    while(abs(xtem2-xtem3)<20||abs(xtem1-xtem3)<20)
    {
        std::vector<int>::iterator xtem3 = find(ORIGIN_X.begin(),ORIGIN_X.end(),X_label[Naxis_x-j]);
        j += 1;

    }
    int index4 = std::distance(std::begin(ORIGIN_X),xtem1);
    int index5 = std::distance(std::begin(ORIGIN_X),xtem2);
    int index6 = std::distance(std::begin(ORIGIN_X),xtem3);
    X_label.clear();
    X_label.push_back(0);
    X_label.push_back(index4);
    X_label.push_back(index5);
    X_label.push_back(index6);
    std::sort(X_label.begin(),X_label.end());
    */
    X_label.push_back(0);
    X_label.push_back(Naxis_x/2);
    Y_label.push_back(0);
    Y_label.push_back(Naxis_y/2);
}

void PREPROCESSING::PART2(int Naxis_x,int Naxis_y,
        std::vector<std::vector<unsigned int> > &DATA,
        std::vector<int> X_label,std::vector<int> Y_label)
{
    std::vector<std::vector<unsigned int> > NEWDATA(Naxis_y,std::vector<unsigned int>(Naxis_x,0));
    for(int i=0; i < Naxis_y; i++)
    {
        for(int j=0; j < Naxis_y; j++)
        {
            NEWDATA[i][j] = DATA[i][j];
        }
    }
    X_label.push_back(Naxis_x);
    Y_label.push_back(Naxis_y);
    for(int i=0; i < Y_label.size()-1; i++)
    {
        for(int j=0; j < X_label.size()-1; j++)
        {
            std::vector<int> SORT;
            for(int m=Y_label[i]; m < Y_label[i+1]; m++)
            {
                for(int n=X_label[j]; n < X_label[j+1]; n++)
                {
                    SORT.push_back(NEWDATA[m][n]);
                }
            }

            std::sort(SORT.begin(),SORT.end());
            int median = SORT[SORT.size()/2];
            for(int m=Y_label[i]; m < Y_label[i+1]; m++)
            {
                for(int n=X_label[j]; n < X_label[j+1]; n++)
                {
                    if(DATA[m][n] < median)
                        DATA[m][n] = 0;
                    else
                        DATA[m][n] -= median;
                }
            }
        }
    }
}

void PREPROCESSING::DISCRETE(int Naxis_x,int Naxis_y,
        std::vector<std::vector<unsigned int> > &DATA,
        std::vector<std::vector<unsigned int> > &DATA2)
{
    for(int i = 2; i < Naxis_y - 2; i++)
        for(int j = 2; j < Naxis_x - 2; j++)
        {
            int count = 0;
            for(int m = i - 2; m < i + 3; m++)
                for(int n = j - 2; n < j + 3; n++)
                {
                    if(DATA[m][n] > 0)
                        count += 1;
                }
            if(count < 5)
            {
                for(int m = i - 2; m < i + 3; m++)
                    for(int n = j - 2; n < j + 3; n++)
                    {
                        DATA2[m][n] = 0;
                    }
            }
        }
    for(int i = 2; i < Naxis_y - 2; i++)
        for(int j = 2; j < Naxis_x - 2; j++)
            DATA[i][j] *= DATA2[i][j];

}

void PREPROCESSING::ADDNEW(int Naxis_x,int Naxis_y,
        std::vector<std::vector<unsigned int> > &DATA,
        std::vector<std::vector<unsigned int> > &DATA2)
{
    std::vector<std::vector<unsigned int> > NEWDATA(Naxis_y,std::vector<unsigned int>(Naxis_x,0));
    for(int i=0; i < Naxis_y; i++)
    {
        for(int j=0; j < Naxis_y; j++)
        {
            NEWDATA[i][j] = DATA[i][j];
        }
    }
    std::cout << NEWDATA[0][0] << std::endl;

    for(int i = 1; i < Naxis_y - 1; i++)
    {
        for(int j = 1; j < Naxis_x - 1; j++)
        {

            int count = 0;
            for(int m = i-1; m<i+2; m++)
            {
                for(int n = j-1; n<j+2; n++)
                {
                    if(NEWDATA[m][n]>0)
                        count += 1;
                }
            }
            if(count > 2)
            {
                for(int m = i-1; m<i+2; m++)
                {
                    for(int n = j-1; n<j+2; n++)
                    {
                        DATA2[m][n] = 1;
                    }
                }
            }

        }
    }
    for(int i = 2; i < Naxis_y - 2; i++)
    {
        for(int j = 2; j < Naxis_x - 2; j++)
        {
            if(NEWDATA[i][j] > 0)
            {
                int sum = 0;
                for(int m=i-2; m<i+3; m++)
                {
                    for(int n=j-2; n<j+3; n++)
                        sum+=NEWDATA[m][n];
                }
                for(int m=i-2; m<i-3; m++)
                    for(int n=j-2; n<j+3; n++)
                        DATA[m][n]+=sum;
            }
        }
    }


}

void PREPROCESSING::TWOVALUE(int Naxis_x,int Naxis_y,
        std::vector<std::vector<unsigned int> > &DATA)
{
    for(int i=0; i<Naxis_y; i++)
        for(int j=0; j<Naxis_x; j++)
        {
            if(DATA[i][j] > 0)
                DATA[i][j] = 1;
        }
}

int PREPROCESSING::POSITIVE(int Naxis_x, int Naxis_y,
        std::vector<std::vector<unsigned int> > DATA)
{
    int sum = 0;
    for(int i=0; i<Naxis_y; i++)
        for(int j=0; j<Naxis_x; j++)
        {
            if(DATA[i][j] > 1)
                sum += 1;
        }
    std::cout << sum << std::endl;
    return sum;
}