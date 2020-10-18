#include <vector>
#include <math.h>
#include <queue>
#include "GEO.h"

bool SOLUTION::isvalid(int m, int n, int i, int j, std::vector<std::vector<unsigned int> > &matrix, std::vector<std::vector<bool> > &mask)
{
    return i >= 0 && i < m && j >= 0 && j < n && !mask[i][j] && matrix[i][j] == 1;
}

void SOLUTION::add(int m, int n, int i, int j, std::vector<std::vector<unsigned int> > &matrix, std::queue<Point> &q, std::vector<std::vector<bool> > &mask)
{
    if (isvalid(m, n, i, j, matrix, mask))
    {
        q.push(Point(i, j));
        mask[i][j] = true;
    }
}

std::vector<std::vector<Point> > bwlabel(int m, int n, std::vector<std::vector<unsigned int> > &matrix)
{
    m = matrix.size();
    n = matrix[0].size();
    std::vector<std::vector<Point> > res;
    std::vector<Point> tmp;
    std::vector<std::vector<bool> > mask(m, std::vector<bool>(n, false));
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (mask[i][j] || matrix[i][j] == 0)
                continue;
            tmp.clear();
            std::queue<Point> q;
            q.push(Point(i, j));
            mask[i][j] = true;
            SOLUTION sol;
            while (!q.empty())
            {
                Point t = q.front();
                q.pop();
                tmp.push_back(t);
                sol.add(m, n, t.r - 1, t.c, matrix, q, mask);
                sol.add(m, n, t.r + 1, t.c, matrix, q, mask);
                sol.add(m, n, t.r, t.c - 1, matrix, q, mask);
                sol.add(m, n, t.r, t.c + 1, matrix, q, mask);
            }
            res.push_back(tmp);
        }
    }
    return res;
}

std::vector<float> changex(std::vector<float> x, std::vector<float> y, float xc, float yc, float theta)
{
    std::vector<float> x2;
    for (size_t i = 0; i < x.size(); i++)
    {
        int x1, y1;
        x1 = 0;
        y1 = 0;
        x1 = x[i];
        y1 = y[i];
        x2.push_back((x1 - xc) * cos(theta) + (y1 - yc) * sin(theta));
    }
    return x2;
};

std::vector<float> changey(std::vector<float> x, std::vector<float> y, float xc, float yc, float theta)
{
    std::vector<float> y2;
    for (size_t i = 0; i < x.size(); i++)
    {
        int x1, y1;
        x1 = 0;
        y1 = 0;
        x1 = x[i];
        y1 = y[i];
        y2.push_back((y1 - yc) * cos(theta) - (x1 - xc) * sin(theta));
    }
    return y2;
};

int s(float x)
{
    if (x < 0)
    {
        return 0;
    }
    else
    {
        return x;
    }
};

std::vector<float>f_point(std::vector<float>x, std::vector<float>y, std::vector<float>A)
{
    int xmax, ymax, xmin, ymin;
    xmax = x[0];
    ymax = y[0];
    xmin = x[0];
    ymin = y[0];
    for (size_t i = 0; i < x.size(); i++)
    {
        if (x[i] > xmax)
        {
            xmax = x[i];
        }
        if (x[i] < xmin)
        {
            xmin = x[i];
        }
        if (y[i] > ymax)
        {
            ymax = y[i];
        }
        if (y[i] < ymin)
        {
            ymin = y[i];
        }
    }
    int h, v;
    h = xmax - xmin;
    v = ymax - ymin;
    float theta, w, l;
    if (h == 0)
    {
        theta = 3.14 / 2;
    }
    else
    {
        theta = atan(v / h);
    }
    l = (h * cos(theta) - v * sin(theta)) / (cos(theta) * cos(theta) - sin(theta) * sin(theta));
    if (v > ((v * cos(theta) - h * sin(theta)) / (cos(theta) * cos(theta) - sin(theta) * sin(theta))))
    {
        w = v;
    }
    else
    {
        w = (v * cos(theta) - h * sin(theta)) / (cos(theta) * cos(theta) - sin(theta) * sin(theta));
    }
    std::vector<float> r;
    for (size_t i = 0; i < x.size(); i++)
    {
        int x2, y2;
        x2 = 0;
        y2 = 0;
        x2 = x[i];
        y2 = y[i];
        r.push_back(sqrt(s(x2 - l / 2) * s(x2 - l / 2) + y2 * y2));
    }
    std::vector<float>F;
    for (size_t i = 0; i < r.size(); i++)
    {
        F.push_back(A[i] * exp(-r[i] * r[i] / (w * w) * 4 * log(2)));
    }
    return F;
};

std::vector<float>result(std::vector<float>x, std::vector<float>y, std::vector<float>F, std::vector<float>A)
{
    float I;
    I = 0;

    for (size_t i = 0; i < A.size(); i++)
    {
        I = I + A[i] * A[i];
    }

    float num;
    num = 0;
    int count;
    count = 0;

    for (size_t i = 0; i < A.size(); i++)
    {
        num = num + (A[i] - F[i]) * (A[i] - F[i]);
        count = count + 1;
    }


    if (num < I and count < 11)
    {
        return F;
    }
    else
    {
        std::vector<float>F1;
        F1 = f_point(x, y, F);
        return result(x,y,F1,A);
    }
};


