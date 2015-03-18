#pragma once

#include <cmath>

namespace OnceMoreWithFeeling
{
    const float PI = 3.14159265359f;

    struct Vector
    {
        float x, y, z, w;

        Vector() : x(0), y(0), z(0), w(0) { }
        Vector(float ax, float ay, float az) : x(ax), y(ay), z(az), w(1) { }
        Vector(const Vector &v) : x(v.x), y(v.y), z(v.z), w(v.w) { }

        Vector inline operator *(float scale)
        {
            Vector v;
            v.x = x * scale;
            v.y = y * scale;
            v.z = z * scale;
            return v;
        }

        Vector inline operator +(Vector b)
        {
            return Vector(x + b.x, y + b.y, z + b.z);
        }

        Vector inline operator -(const Vector b)
        {
            return Vector(x - b.x, y - b.y, z - b.z);
        }

        float *gl()
        {
            return &x;
        }

        static Vector Cross(Vector a, Vector b)
        {
            return Vector(
                a.y * b.z - a.z * b.y,
                a.z * b.x - a.x * b.z,
                a.x * b.y - a.y * b.x
            );
        }

        static float Dot(Vector a, Vector b)
        {
            return a.x * b.x + a.y * b.y + a.z * b.z;
        }

        Vector Normalise()
        {
            float length = ::sqrtf(x * x + y * y + z * z);
            if (length != 0)
            {
                x /= length;
                y /= length;
                z /= length;
            }
            return *this;
        }

        Vector Normalised()
        {
            Vector v(*this);
            v.Normalise();
            return v;
        }

        float Length()
        {
            return ::sqrtf(x * x + y * y + z * z);
        }

        float LengthSq()
        {
            return x * x + y * y + z * z;
        }

        Vector Scale(Vector s)
        {
            return Vector(
                x * s.x,
                y * s.y,
                z * s.z
            );
        }
    };

    struct Matrix
    {
        Matrix()
        {
            a[0][0] = 1.0;
            a[0][1] = 0.0;
            a[0][2] = 0.0;
            a[0][3] = 0.0;
            a[1][0] = 0.0;
            a[1][1] = 1.0;
            a[1][2] = 0.0;
            a[1][3] = 0.0;
            a[2][0] = 0.0;
            a[2][1] = 0.0;
            a[2][2] = 1.0;
            a[2][3] = 0.0;
            a[3][0] = 0.0;
            a[3][1] = 0.0;
            a[3][2] = 0.0;
            a[3][3] = 1.0;
        }

        Matrix(float *b)
        {
            a[0][0] = b[0];
            a[0][1] = b[1];
            a[0][2] = b[2];
            a[0][3] = b[3];
            a[1][0] = b[4];
            a[1][1] = b[5];
            a[1][2] = b[6];
            a[1][3] = b[7];
            a[2][0] = b[8];
            a[2][1] = b[9];
            a[2][2] = b[10];
            a[2][3] = b[11];
            a[3][0] = b[12];
            a[3][1] = b[13];
            a[3][2] = b[14];
            a[3][3] = b[15];
        }

        Matrix(const Matrix &b)
        {
            a[0][0] = b.a[0][0];
            a[0][1] = b.a[0][1];
            a[0][2] = b.a[0][2];
            a[0][3] = b.a[0][3];
            a[1][0] = b.a[1][0];
            a[1][1] = b.a[1][1];
            a[1][2] = b.a[1][2];
            a[1][3] = b.a[1][3];
            a[2][0] = b.a[2][0];
            a[2][1] = b.a[2][1];
            a[2][2] = b.a[2][2];
            a[2][3] = b.a[2][3];
            a[3][0] = b.a[3][0];
            a[3][1] = b.a[3][1];
            a[3][2] = b.a[3][2];
            a[3][3] = b.a[3][3];
        }

        float inline Determinant()
        {
            float d = 0;

            d += a[0][0] * a[1][1] * a[2][2] * a[3][3];
            d += a[0][0] * a[2][1] * a[3][2] * a[1][3];
            d += a[0][0] * a[3][1] * a[1][2] * a[2][3];

            d += a[1][0] * a[0][1] * a[3][2] * a[2][3];
            d += a[1][0] * a[2][1] * a[0][2] * a[3][3];
            d += a[1][0] * a[3][1] * a[2][2] * a[0][3];

            d += a[2][0] * a[0][1] * a[1][2] * a[3][3];
            d += a[2][0] * a[1][1] * a[3][2] * a[0][3];
            d += a[2][0] * a[3][1] * a[0][2] * a[1][3];

            d += a[3][0] * a[0][1] * a[2][2] * a[1][3];
            d += a[3][0] * a[1][1] * a[0][2] * a[2][3];
            d += a[3][0] * a[2][1] * a[1][2] * a[0][3];

            d -= a[0][0] * a[1][1] * a[3][2] * a[2][3];
            d -= a[0][0] * a[2][1] * a[1][2] * a[3][3];
            d -= a[0][0] * a[3][1] * a[2][2] * a[1][3];

            d -= a[1][0] * a[0][1] * a[2][2] * a[3][3];
            d -= a[1][0] * a[2][1] * a[3][2] * a[0][3];
            d -= a[1][0] * a[3][1] * a[0][2] * a[2][3];

            d -= a[2][0] * a[0][1] * a[3][2] * a[1][3];
            d -= a[2][0] * a[1][1] * a[0][2] * a[3][3];
            d -= a[2][0] * a[3][1] * a[1][2] * a[0][3];

            d -= a[3][0] * a[0][1] * a[1][2] * a[2][3];
            d -= a[3][0] * a[1][1] * a[2][2] * a[0][3];
            d -= a[3][0] * a[2][1] * a[0][2] * a[1][3];

            return d;
        }

        Matrix inline Inverse()
        {
            float d = Determinant();
            Matrix m;

            m.a[0][0] = 0;
            m.a[0][0] += a[1][1] * a[2][2] * a[3][3];
            m.a[0][0] += a[2][1] * a[3][2] * a[1][3];
            m.a[0][0] += a[3][1] * a[1][2] * a[2][3];
            m.a[0][0] -= a[1][1] * a[3][2] * a[2][3];
            m.a[0][0] -= a[2][1] * a[1][2] * a[3][3];
            m.a[0][0] -= a[3][1] * a[2][2] * a[1][3];

            m.a[1][0] = 0;
            m.a[1][0] += a[1][0] * a[3][2] * a[2][3];
            m.a[1][0] += a[2][0] * a[1][2] * a[3][3];
            m.a[1][0] += a[3][0] * a[2][2] * a[1][3];
            m.a[1][0] -= a[1][0] * a[2][2] * a[3][3];
            m.a[1][0] -= a[2][0] * a[3][2] * a[1][3];
            m.a[1][0] -= a[3][0] * a[1][2] * a[2][3];

            m.a[2][0] = 0;
            m.a[2][0] += a[1][0] * a[2][1] * a[3][3];
            m.a[2][0] += a[2][0] * a[3][1] * a[1][3];
            m.a[2][0] += a[3][0] * a[1][1] * a[2][3];
            m.a[2][0] -= a[1][0] * a[3][1] * a[2][3];
            m.a[2][0] -= a[2][0] * a[1][1] * a[3][3];
            m.a[2][0] -= a[3][0] * a[2][1] * a[1][3];

            m.a[3][0] = 0;
            m.a[3][0] += a[1][0] * a[3][1] * a[2][2];
            m.a[3][0] += a[2][0] * a[1][1] * a[3][2];
            m.a[3][0] += a[3][0] * a[2][1] * a[1][2];
            m.a[3][0] -= a[1][0] * a[2][1] * a[3][2];
            m.a[3][0] -= a[2][0] * a[3][1] * a[1][2];
            m.a[3][0] -= a[3][0] * a[1][1] * a[2][2];

            // yawn

            m.a[0][1] = 0;
            m.a[0][1] += a[0][1] * a[3][2] * a[2][3];
            m.a[0][1] += a[2][1] * a[0][2] * a[3][3];
            m.a[0][1] += a[3][1] * a[2][2] * a[0][3];
            m.a[0][1] -= a[0][1] * a[2][2] * a[3][3];
            m.a[0][1] -= a[2][1] * a[3][2] * a[0][3];
            m.a[0][1] -= a[3][1] * a[0][2] * a[2][3];

            m.a[1][1] = 0;
            m.a[1][1] += a[0][0] * a[2][2] * a[3][3];
            m.a[1][1] += a[2][0] * a[3][2] * a[0][3];
            m.a[1][1] += a[3][0] * a[0][2] * a[2][3];
            m.a[1][1] -= a[0][0] * a[3][2] * a[2][3];
            m.a[1][1] -= a[2][0] * a[0][2] * a[3][3];
            m.a[1][1] -= a[3][0] * a[2][2] * a[0][3];

            m.a[2][1] = 0;
            m.a[2][1] += a[0][0] * a[3][1] * a[2][3];
            m.a[2][1] += a[2][0] * a[0][1] * a[3][3];
            m.a[2][1] += a[3][0] * a[2][1] * a[0][3];
            m.a[2][1] -= a[0][0] * a[2][1] * a[3][3];
            m.a[2][1] -= a[2][0] * a[3][1] * a[0][3];
            m.a[2][1] -= a[3][0] * a[0][1] * a[2][3];

            m.a[3][1] = 0;
            m.a[3][1] += a[0][0] * a[2][1] * a[3][2];
            m.a[3][1] += a[2][0] * a[3][1] * a[0][2];
            m.a[3][1] += a[3][0] * a[0][1] * a[2][2];
            m.a[3][1] -= a[0][0] * a[3][1] * a[2][2];
            m.a[3][1] -= a[2][0] * a[0][1] * a[3][2];
            m.a[3][1] -= a[3][0] * a[2][1] * a[0][2];

            // OH GOD I'M SO BORED

            m.a[0][2] = 0;
            m.a[0][2] += a[0][1] * a[1][2] * a[3][3];
            m.a[0][2] += a[1][1] * a[3][2] * a[0][3];
            m.a[0][2] += a[3][1] * a[0][2] * a[1][3];
            m.a[0][2] -= a[0][1] * a[3][2] * a[1][3];
            m.a[0][2] -= a[1][1] * a[0][2] * a[3][3];
            m.a[0][2] -= a[3][1] * a[1][2] * a[0][3];

            m.a[1][2] = 0;
            m.a[1][2] += a[0][0] * a[3][2] * a[1][3];
            m.a[1][2] += a[1][0] * a[0][2] * a[3][3];
            m.a[1][2] += a[3][0] * a[1][2] * a[0][3];
            m.a[1][2] -= a[0][0] * a[1][2] * a[3][3];
            m.a[1][2] -= a[1][0] * a[3][2] * a[0][3];
            m.a[1][2] -= a[3][0] * a[0][2] * a[1][3];

            m.a[2][2] = 0;
            m.a[2][2] += a[0][0] * a[1][1] * a[3][3];
            m.a[2][2] += a[1][0] * a[3][1] * a[0][3];
            m.a[2][2] += a[3][0] * a[0][1] * a[1][3];
            m.a[2][2] -= a[0][0] * a[3][1] * a[1][3];
            m.a[2][2] -= a[1][0] * a[0][1] * a[3][3];
            m.a[2][2] -= a[3][0] * a[1][1] * a[0][3];

            m.a[3][2] = 0;
            m.a[3][2] += a[0][0] * a[3][1] * a[1][2];
            m.a[3][2] += a[1][0] * a[0][1] * a[3][2];
            m.a[3][2] += a[3][0] * a[1][1] * a[0][2];
            m.a[3][2] -= a[0][0] * a[1][1] * a[3][2];
            m.a[3][2] -= a[1][0] * a[3][1] * a[0][2];
            m.a[3][2] -= a[3][0] * a[0][1] * a[1][2];

            // MAKE IT STOP

            m.a[0][3] = 0;
            m.a[0][3] += a[0][1] * a[2][2] * a[1][3];
            m.a[0][3] += a[1][1] * a[0][2] * a[2][3];
            m.a[0][3] += a[2][1] * a[1][2] * a[0][3];
            m.a[0][3] -= a[0][1] * a[1][2] * a[2][3];
            m.a[0][3] -= a[1][1] * a[2][2] * a[0][3];
            m.a[0][3] -= a[2][1] * a[0][2] * a[1][3];

            m.a[1][3] = 0;
            m.a[1][3] += a[0][0] * a[1][2] * a[2][3];
            m.a[1][3] += a[1][0] * a[2][2] * a[0][3];
            m.a[1][3] += a[2][0] * a[0][2] * a[1][3];
            m.a[1][3] -= a[0][0] * a[2][2] * a[1][3];
            m.a[1][3] -= a[1][0] * a[0][2] * a[2][3];
            m.a[1][3] -= a[2][0] * a[1][2] * a[0][3];

            m.a[2][3] = 0;
            m.a[2][3] += a[0][0] * a[2][1] * a[1][3];
            m.a[2][3] += a[1][0] * a[0][1] * a[2][3];
            m.a[2][3] += a[2][0] * a[1][1] * a[0][3];
            m.a[2][3] -= a[0][0] * a[1][1] * a[2][3];
            m.a[2][3] -= a[1][0] * a[2][1] * a[0][3];
            m.a[2][3] -= a[2][0] * a[0][1] * a[1][3];

            m.a[3][3] = 0;
            m.a[3][3] += a[0][0] * a[1][1] * a[2][2];
            m.a[3][3] += a[1][0] * a[2][1] * a[0][2];
            m.a[3][3] += a[2][0] * a[0][1] * a[1][2];
            m.a[3][3] -= a[0][0] * a[2][1] * a[1][2];
            m.a[3][3] -= a[1][0] * a[0][1] * a[2][2];
            m.a[3][3] -= a[2][0] * a[1][1] * a[0][2];

            // ph'nglui mglw'nafh Cthulhu R'lyeh wgah'nagl fhtagn

            return m * (1.0f / d);
        }

        Matrix inline Transpose()
        {
            Matrix m;
            m.a[0][0] = a[0][0];
            m.a[0][1] = a[1][0];
            m.a[0][2] = a[2][0];
            m.a[0][3] = a[3][0];
            m.a[1][0] = a[0][1];
            m.a[1][1] = a[1][1];
            m.a[1][2] = a[2][1];
            m.a[1][3] = a[3][1];
            m.a[2][0] = a[0][2];
            m.a[2][1] = a[1][2];
            m.a[2][2] = a[2][2];
            m.a[2][3] = a[3][2];
            m.a[3][0] = a[0][3];
            m.a[3][1] = a[1][3];
            m.a[3][2] = a[2][3];
            m.a[3][3] = a[3][3];
            return m;
        }

        Matrix inline operator *(float scale)
        {
            Matrix m = Matrix(*this);
            m *= scale;
            return m;
        }

        void inline operator *=(float scale)
        {
            a[0][0] *= scale;
            a[0][1] *= scale;
            a[0][2] *= scale;
            a[0][3] *= scale;
            a[1][0] *= scale;
            a[1][1] *= scale;
            a[1][2] *= scale;
            a[1][3] *= scale;
            a[2][0] *= scale;
            a[2][1] *= scale;
            a[2][2] *= scale;
            a[2][3] *= scale;
            a[3][0] *= scale;
            a[3][1] *= scale;
            a[3][2] *= scale;
            a[3][3] *= scale;
        }

        Vector inline operator *(Vector &v)
        {
            Vector r;
            r.x = a[0][0] * v.x + a[1][0] * v.y + a[2][0] * v.z + a[3][0] * v.w;
            r.y = a[0][1] * v.x + a[1][1] * v.y + a[2][1] * v.z + a[3][1] * v.w;
            r.z = a[0][2] * v.x + a[1][2] * v.y + a[2][2] * v.z + a[3][2] * v.w;
            r.w = a[0][3] * v.x + a[1][3] * v.y + a[2][3] * v.z + a[3][3] * v.w;
            return r;
        }

        Matrix operator *(const Matrix &m) const
        {
            Matrix r = Matrix();

            r.a[0][0] = a[0][0] * m.a[0][0] + a[1][0] * m.a[0][1] + a[2][0] * m.a[0][2] + a[3][0] * m.a[0][3];
            r.a[0][1] = a[0][1] * m.a[0][0] + a[1][1] * m.a[0][1] + a[2][1] * m.a[0][2] + a[3][1] * m.a[0][3];
            r.a[0][2] = a[0][2] * m.a[0][0] + a[1][2] * m.a[0][1] + a[2][2] * m.a[0][2] + a[3][2] * m.a[0][3];
            r.a[0][3] = a[0][3] * m.a[0][0] + a[1][3] * m.a[0][1] + a[2][3] * m.a[0][2] + a[3][3] * m.a[0][3];
            r.a[1][0] = a[0][0] * m.a[1][0] + a[1][0] * m.a[1][1] + a[2][0] * m.a[1][2] + a[3][0] * m.a[1][3];
            r.a[1][1] = a[0][1] * m.a[1][0] + a[1][1] * m.a[1][1] + a[2][1] * m.a[1][2] + a[3][1] * m.a[1][3];
            r.a[1][2] = a[0][2] * m.a[1][0] + a[1][2] * m.a[1][1] + a[2][2] * m.a[1][2] + a[3][2] * m.a[1][3];
            r.a[1][3] = a[0][3] * m.a[1][0] + a[1][3] * m.a[1][1] + a[2][3] * m.a[1][2] + a[3][3] * m.a[1][3];
            r.a[2][0] = a[0][0] * m.a[2][0] + a[1][0] * m.a[2][1] + a[2][0] * m.a[2][2] + a[3][0] * m.a[2][3];
            r.a[2][1] = a[0][1] * m.a[2][0] + a[1][1] * m.a[2][1] + a[2][1] * m.a[2][2] + a[3][1] * m.a[2][3];
            r.a[2][2] = a[0][2] * m.a[2][0] + a[1][2] * m.a[2][1] + a[2][2] * m.a[2][2] + a[3][2] * m.a[2][3];
            r.a[2][3] = a[0][3] * m.a[2][0] + a[1][3] * m.a[2][1] + a[2][3] * m.a[2][2] + a[3][3] * m.a[2][3];
            r.a[3][0] = a[0][0] * m.a[3][0] + a[1][0] * m.a[3][1] + a[2][0] * m.a[3][2] + a[3][0] * m.a[3][3];
            r.a[3][1] = a[0][1] * m.a[3][0] + a[1][1] * m.a[3][1] + a[2][1] * m.a[3][2] + a[3][1] * m.a[3][3];
            r.a[3][2] = a[0][2] * m.a[3][0] + a[1][2] * m.a[3][1] + a[2][2] * m.a[3][2] + a[3][2] * m.a[3][3];
            r.a[3][3] = a[0][3] * m.a[3][0] + a[1][3] * m.a[3][1] + a[2][3] * m.a[3][2] + a[3][3] * m.a[3][3];

            return r;
        }

        float *gl()
        {
            return &(a[0][0]);
        }

        static Matrix Rotate(float x, float y, float z)
        {
            Matrix m = Matrix();

            Matrix mx = Matrix();
            mx.a[0][0] = 1.0;
            mx.a[0][1] = 0.0;
            mx.a[0][2] = 0.0;
            mx.a[0][3] = 0.0;
            mx.a[1][0] = 0.0;
            mx.a[1][1] = cos(x);
            mx.a[1][2] = sin(x);
            mx.a[1][3] = 0.0;
            mx.a[2][0] = 0.0;
            mx.a[2][1] = -sin(x);
            mx.a[2][2] = cos(x);
            mx.a[2][3] = 0.0;
            mx.a[3][0] = 0.0;
            mx.a[3][1] = 0.0;
            mx.a[3][2] = 0.0;
            mx.a[3][3] = 1.0;

            Matrix my = Matrix();
            my.a[0][0] = cos(y);
            my.a[0][1] = 0.0;
            my.a[0][2] = -sin(y);
            my.a[0][3] = 0.0;
            my.a[1][0] = 0.0;
            my.a[1][1] = 1.0;
            my.a[1][2] = 0.0;
            my.a[1][3] = 0.0;
            my.a[2][0] = sin(y);
            my.a[2][1] = 0.0;
            my.a[2][2] = cos(y);
            my.a[2][3] = 0.0;
            my.a[3][0] = 0.0;
            my.a[3][1] = 0.0;
            my.a[3][2] = 0.0;
            my.a[3][3] = 1.0;

            Matrix mz = Matrix();
            mz.a[0][0] = cos(z);
            mz.a[0][1] = sin(z);
            mz.a[0][2] = 0.0;
            mz.a[0][3] = 0.0;
            mz.a[1][0] = -sin(z);
            mz.a[1][1] = cos(z);
            mz.a[1][2] = 0.0;
            mz.a[1][3] = 0.0;
            mz.a[2][0] = 0.0;
            mz.a[2][1] = 0.0;
            mz.a[2][2] = 1.0;
            mz.a[2][3] = 0.0;
            mz.a[3][0] = 0.0;
            mz.a[3][1] = 0.0;
            mz.a[3][2] = 0.0;
            mz.a[3][3] = 1.0;

            m = mx * my * mz;

            return m;
        }

        static Matrix Scale(float s)
        {
            return Scale(s, s, s);
        }

        static Matrix Scale(float x, float y, float z)
        {
            Matrix m = Matrix();
            m.a[0][0] = x;
            m.a[1][1] = y;
            m.a[2][2] = z;
            m.a[3][3] = 1.0;
            return m;
        }

        static Matrix Scale(Vector v)
        {
            return Scale(v.x, v.y, v.z);
        }

        static Matrix Translate(float x, float y, float z)
        {
            Matrix m = Matrix();
            m.a[3][0] = x;
            m.a[3][1] = y;
            m.a[3][2] = z;
            m.a[3][3] = 1.0;
            return m;
        }

        static Matrix Translate(Vector v)
        {
            Matrix m;
            m.a[3][0] = v.x;
            m.a[3][1] = v.y;
            m.a[3][2] = v.z;
            m.a[3][3] = 1;
            return m;
        }

        static Matrix Camera(Vector eye, Vector target)
        {
            return Camera(eye, target, Vector(0, 1, 0));
        }

        static Matrix Camera(Vector eye, Vector target, Vector up)
        {
            Vector zaxis = (target - eye).Normalise();
            Vector xaxis = Vector::Cross(zaxis, up).Normalise();
            Vector yaxis = Vector::Cross(xaxis, zaxis).Normalise();

            Matrix m;
            m.a[0][0] = xaxis.x;
            m.a[1][0] = xaxis.y;
            m.a[2][0] = xaxis.z;
            m.a[3][0] = 0;
            m.a[0][1] = yaxis.x;
            m.a[1][1] = yaxis.y;
            m.a[2][1] = yaxis.z;
            m.a[3][1] = 0;
            m.a[0][2] = -zaxis.x;
            m.a[1][2] = -zaxis.y;
            m.a[2][2] = -zaxis.z;
            m.a[3][2] = 0;
            m.a[0][3] = 0;
            m.a[1][3] = 0;
            m.a[2][3] = 0;
            m.a[3][3] = 1;

            return m * Matrix::Translate(eye * -1);
        }

        static Matrix Projection(float nearClip, float farClip, float aspectRatio, float fov)
        {
            Matrix m;
            float s = 1.f / tan(fov / 2);
            m.a[0][0] = aspectRatio * s;
            m.a[1][1] = s;
            m.a[2][2] = (farClip + nearClip) / (nearClip - farClip);
            m.a[2][3] = -1;
            m.a[3][2] = 2 * (farClip * nearClip) / (nearClip - farClip);
            m.a[3][3] = 0;
            return m;
        }

        static Matrix Billboard(Vector eye, Vector target)
        {
            Vector up = Vector(0, 1, 0);
            Vector zaxis = (target - eye).Normalise();
            Vector xaxis = Vector::Cross(zaxis, up).Normalise();
            Vector yaxis = Vector::Cross(xaxis, zaxis).Normalise();

            Matrix m;
            m.a[0][0] = xaxis.x;
            m.a[1][0] = xaxis.y;
            m.a[2][0] = xaxis.z;
            m.a[3][0] = 0;
            m.a[0][1] = yaxis.x;
            m.a[1][1] = yaxis.y;
            m.a[2][1] = yaxis.z;
            m.a[3][1] = 0;
            m.a[0][2] = -zaxis.x;
            m.a[1][2] = -zaxis.y;
            m.a[2][2] = -zaxis.z;
            m.a[3][2] = 0;
            m.a[0][3] = 0;
            m.a[1][3] = 0;
            m.a[2][3] = 0;
            m.a[3][3] = 1;

            return m.Transpose();
        }

        static Matrix Ortho(float width, float height)
        {
            Matrix m;
            m.a[0][0] = 2.f / width;
            m.a[1][1] = 2.f / height;
            m.a[2][2] = -1; // -2 / (far - near)
            m.a[3][0] = -1; // -(right + left) / (right - left)
            m.a[3][1] = -1; // -(top + bottom) / (top - bottom)
            m.a[3][2] = 0; // (far + near) / (far - near);
            return m;
        }

        float a[4][4];		// Columns, Rows
    };

    struct Quaternion
    {
        float w, x, y, z;

        Quaternion() : w(1), x(0), y(0), z(0)
        {

        }

        Quaternion(const Vector &b) : x(b.x), y(b.y), z(b.z), w(0.0)
        {
        
        }

        Quaternion(const Vector &axis, const float angle)
        {
            float a = angle / 2;
            w = cos(a);
            x = sin(a) * axis.x;
            y = sin(a) * axis.y;
            z = sin(a) * axis.z;
        }

        void Normalise()
        {
            float n = sqrt(x * x + y * y + z * z + w * w);
            x /= n;
            y /= n;
            z /= n;
            w /= n;
        }

        Quaternion Inverse() const
        {
            Quaternion r;
            float n = x * x + y * y + z * z + w * w;
            r.x = x / -n;
            r.y = y / -n;
            r.z = z / -n;
            r.w = w / n;
            return r;
        }

        Quaternion operator *(const Quaternion &b) const
        {
            float rw = w * b.w - x * b.x - y * b.y - z * b.z;
            float rx = w * b.x + x * b.w + y * b.z - z * b.y;
            float ry = w * b.y - x * b.z + y * b.w + z * b.x;
            float rz = w * b.z + x * b.y - y * b.x + z * b.w;

            Quaternion q;
            q.x = rx;
            q.y = ry;
            q.z = rz;
            q.w = rw;

            return q;
        }

        Vector operator *(const Vector &b) const
        {
            Quaternion a(b);
            Quaternion r = *this * a;
            Quaternion i = Quaternion(*this).Inverse();
            r = r * i;
            return Vector(r.x, r.y, r.z);
        }

        static Quaternion FromEuler(const float pitch, const float yaw, const float roll)
        {
            Quaternion q;

            float cx = cos(pitch / 2);
            float cy = cos(yaw / 2);
            float cz = cos(roll / 2);

            float sx = sin(pitch / 2);
            float sy = sin(yaw / 2);
            float sz = sin(roll / 2);

            q.w = cx * cy * cz + sx * sy * sz;
            q.x = sx * cy * cz - cx * sy * sz;
            q.y = cx * sy * cz + sx * cy * sz;
            q.z = cx * cy * sz - sx * sy * cz;
            
            q.Normalise();

            return q;
        }

        Matrix ToMatrix() const
        {
            Matrix m;

            m.a[0][0] = w * w + x * x - y * y - z * z;
            m.a[0][1] = 2 * x * y + 2 * z * w;
            m.a[0][2] = 2 * x * z - 2 * y * w;

            m.a[1][0] = 2 * x * y - 2 * z * w;
            m.a[1][1] = w * w - x * x + y * y - z * z;
            m.a[1][2] = 2 * y * z + 2 * x * w;

            m.a[2][0] = 2 * x * z + 2 * y * w;
            m.a[2][1] = 2 * y * z - 2 * x * w;
            m.a[2][2] = w * w - x * x - y * y + z * z;

            return m;
        }
    };

    static float RandF(float min, float max)
    {
        return (static_cast<float>(rand()) / RAND_MAX) * (max - min) + min;
    }

    static int RandI(int min, int max)
    {
        // This is a horrible hack. But it's not like we're doing crypto
        float maxf = max + (max >= 0 ? 0.999f : -0.999f);
        return static_cast<int>(floor(RandF(static_cast<float>(min), maxf)));
    }
}
