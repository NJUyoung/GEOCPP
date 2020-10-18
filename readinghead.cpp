#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <numeric>
#include <vector>
#include "GEO.h"

int HEAD::TYPE(std::string File)
{
    std::string type("BITPIX");
    std::string::size_type position;
    int t = 0;
    position = File.find(type);
    if(position != File.npos)
    {
        std::string bitpix;
        bitpix = File.substr(position,80);
        for(int i = 0; i < 80; i++)
        {
            if((bitpix[i] < '0' || bitpix[i] > '9') && t > 0)
                break;
            if(bitpix[i] >= '0' && bitpix[i] <= '9')
                t = t * 10 + bitpix[i] - 48;
        }
    }
    return t;
}

void HEAD::CRPIX(std::string File, float &Crpix1, float &Crpix2)
{
    std::string c1("CRPIX1");
    std::string c2("CRPIX2");
    std::string::size_type position1;
    std::string::size_type position2;
    position1 = File.find(c1);
    position2 = File.find(c2);
    if(position1 != File.npos && position2 != File.npos)
    {
        std::string crpix1;
        std::string crpix2;
        crpix1 = File.substr(position1+9,71);
        crpix2 = File.substr(position2+9,71);
        Crpix1 = atof(crpix1.c_str());
        Crpix2 = atof(crpix2.c_str());
    }
}

void HEAD::CRVAL(std::string File, float &Crval1, float &Crval2)
{
    std::string c1("CRPIX1");
    std::string c2("CRPIX2");
    std::string::size_type position1;
    std::string::size_type position2;
    position1 = File.find(c1);
    position2 = File.find(c2);
    if(position1 != File.npos && position2 != File.npos)
    {
        std::string crval1;
        std::string crval2;
        crval1 = File.substr(position1+9,71);
        crval2 = File.substr(position2+9,71);
        Crval1 = atof(crval1.c_str());
        Crval2 = atof(crval2.c_str());
    }
}

void HEAD::NAXIS(std::string File,int &Naxis_x,int &Naxis_y)
{
    std::string n1("NAXIS1");
    std::string n2("NAXIS2");
    std::string::size_type position1;
    std::string::size_type position2;
    position1 = File.find(n1);
    position2 = File.find(n2);
    if(position1 != File.npos && position2 != File.npos)
    {
        std::string naxis1;
        std::string naxis2;
        naxis1 = File.substr(position1+sizeof(naxis1),80 - sizeof(naxis1));
        naxis2 = File.substr(position2+sizeof(naxis2),80 - sizeof(naxis2));
        for(int i = 0; i < 80 - sizeof(naxis1); i++)
        {
            if((naxis1[i] < '0' || naxis1[i] > '9' )&& Naxis_x > 0)
                break;
            if((naxis2[i] < '0' || naxis2[i] > '9' )&& Naxis_y > 0)
                break;
            if(naxis1[i] >= '0' && naxis1[i] <= '9')
                Naxis_x = Naxis_x * 10 + naxis1[i] - 48;
            if(naxis2[i] >= '0' && naxis2[i] <= '9')
                Naxis_y = Naxis_y * 10 + naxis2[i] - 48;
        }
    }
}

void HEAD::BVALUE(std::string File,int &Bscale,int &Bzero)
{
    std::string bs("BSCALE");
    std::string bz("BZERO");
    std::string::size_type position_bs;
    std::string::size_type position_bz;
    position_bs = File.find(bs);
    position_bz = File.find(bz);
    if(position_bs != File.npos)
    {
        std::string bscale;
        bscale = File.substr(position_bs,80 - sizeof(bs));
        for(int i = 0; i < 80 - sizeof(bs); i++)
        {
            if((bscale[i] < '0' || bscale[i] > '9') && Bscale > 0)
                break;
            if(bscale[i] > '0' && bscale[i] < '9')
                Bscale = Bscale * 10 + bscale[i] - 48;
        }
    }
    if(position_bz != File.npos)
    {
        std::string bzero;
        bzero = File.substr(position_bz,80 - sizeof(bz));
        for(int i = 0; i < 80 - sizeof(bz); i++)
        {
            if((bzero[i] < '0' || bzero[i] > '9') && Bzero > 0)
                break;
            if(bzero[i] > '0' && bzero[i] < '9')
                Bzero = Bzero * 10 + bzero[i] - 48;
        }
    }
}

int HEAD::DATA_POSITION(std::string File) {
    int unit = 2880;
    std::string::size_type end_position;
    end_position = File.rfind("END");
    int time = 0, pos = 0;
    while (pos < end_position) {
        time++;
        pos = time * unit;
    }
    return pos;
}
void DATAPART::READING16(int Data_Position,int Naxis_x,int Naxis_y,int Bscale,int Bzero,
        std::vector<std::vector<unsigned int> > &DATA,
        std::vector<std::vector<unsigned int> > &DATA2,
        std::vector<std::vector<unsigned int> > &ORIGIN_DATA,std::ifstream& file)
{
    unsigned char *tempcard = new unsigned char[Naxis_x*2];
    short *card = new short[Naxis_x];

    file.seekg(Data_Position, std::ios::beg);
    for(int i = 0; i < Naxis_y; i++)
    {
        file.read((char*)tempcard, Naxis_x * 2);
        for (int j = 0; j < Naxis_x; j++)
        {
            if (tempcard[2 * j] > 128)
            {
                card[j] = (unsigned short) (tempcard[2 * j] - 255) * 256 - (unsigned short) (256 - tempcard[2 * j + 1]);
                DATA[i][j] = Bzero + Bscale * card[j];
                ORIGIN_DATA[i][j] = DATA[i][j];
                DATA2[i][j] = ORIGIN_DATA[i][j];
            }else
                {
                card[j] = (unsigned short) (tempcard[2 * j]) * 256 + (unsigned short) (tempcard[2 * j + 1]);
                DATA[i][j] = Bzero + Bscale * card[j];
                ORIGIN_DATA[i][j] = DATA[i][j];
                DATA2[i][j] = ORIGIN_DATA[i][j];
                }
        }
    }
}

void DATAPART::READING32(int Data_Position, int Naxis_x, int Naxis_y, int Bscale, int Bzero,
                         std::vector<std::vector<unsigned int> > &DATA,
                         std::vector<std::vector<unsigned int> > &DATA2,
                         std::vector<std::vector<unsigned int> > &ORIGIN_DATA, std::ifstream &file)
                         {
    unsigned char *tempcard = new unsigned char[Naxis_x*4];
    long *card = new long[Naxis_x*4];

    file.seekg(Data_Position,std::ios::beg);
    for(int i=0; i<Naxis_y; i++)
    {
        file.read((char*)tempcard, Naxis_x*4);
        for(int j=0; j<Naxis_x; j++)
        {
            card[j] = 1;

            card[j] = (unsigned long)(tempcard[4*j])*16777216 + (unsigned long)(tempcard[4*j+1])*65536
                    + (unsigned long)(tempcard[4*j+2])*256 + (unsigned long)(tempcard[4*j+3]);
            DATA[i][j] = Bzero + Bscale * card[j];
            ORIGIN_DATA[i][j] = DATA[i][j];
            DATA2[i][j] = DATA[i][j];
        }
    }
}

int DATAPART::SUM(int Naxis_y,std::vector<std::vector<unsigned int> > ORIGIN_DATA)
{
    int SUM = 0;
    for(int i = 0; i < Naxis_y; i++)
    {
        int sum = std::accumulate(ORIGIN_DATA[i].begin(),ORIGIN_DATA[i].end(),0.0);
        SUM += sum;
    }
    return SUM;
}