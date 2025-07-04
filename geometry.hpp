#pragma once

#include <cmath>
#include <ostream>

template <class t>
struct Vec2
{
    t x;
    t y;
    Vec2() {
        x = 0;
        y = 0;
    }
    Vec2(t x, t y) {
        this->x = x;
        this->y = y;
    }
    inline Vec2<t> operator+(const Vec2<t> &V) const { return Vec2<t>(x + V.x, y + V.y); }
    inline Vec2<t> operator-(const Vec2<t> &V) const { return Vec2<t>(x - V.x, y - V.y); }
    inline Vec2<t> operator*(float f) const { return Vec2<t>(x * f, y * f); }
    template <class>
    friend std::ostream &operator<<(std::ostream &s, Vec2<t> &v);
};

template <class t>
struct Vec3
{
    t x;
    t y;
    t z;
    Vec3() {
        x = 0;
        y = 0;
        z = 0;
    }
    Vec3(t x, t y, t z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    inline Vec3<t> cross(const Vec3<t> &v) const { return Vec3<t>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }
    inline Vec3<t> operator+(const Vec3<t> &v) const { return Vec3<t>(x + v.x, y + v.y, z + v.z); }
    inline Vec3<t> operator-(const Vec3<t> &v) const { return Vec3<t>(x - v.x, y - v.y, z - v.z); }
    inline Vec3<t> operator*(float f) const { return Vec3<t>(x * f, y * f, z * f); }
    inline t dot(const Vec3<t> &v) const { return x * v.x + y * v.y + z * v.z; }
    float norm() const { return std::sqrt(x * x + y * y + z * z); }
    Vec3<t> &normalize(t l = 1)
    {
        *this = (*this) * (l / norm());
        return *this;
    }
    Vec2<t> xy() {
        return Vec2<t>(x, y);
    }
    template <class>
    friend std::ostream &operator<<(std::ostream &s, Vec3<t> &v);
};

template <class t>
struct Vec4
{
    t x;
    t y;
    t z;
    t w;
    Vec4() {
        x = 0;
        y = 0;
        z = 0;
        w = 0;
    }
    Vec4(t x, t y, t z, t w) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }
    template <class>
    friend std::ostream &operator<<(std::ostream &s, Vec2<t> &v);
};

template <class t>
struct Mat2
{
    t m[2][2];
    
    Mat2() {
        for (int i = 0; i < 2; i++)
            for (int j = 0; j < 2; j++)
                m[i][j] = (i == j) ? 1 : 0;
    }
    
    Mat2(t m00, t m01, t m10, t m11) {
        m[0][0] = m00; m[0][1] = m01;
        m[1][0] = m10; m[1][1] = m11;
    }
    
    t* operator[](int i) { return m[i]; }
    const t* operator[](int i) const { return m[i]; }
    
    Mat2<t> operator*(const Mat2<t>& other) const {
        Mat2<t> result;
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                result[i][j] = 0;
                for (int k = 0; k < 2; k++) {
                    result[i][j] += m[i][k] * other[k][j];
                }
            }
        }
        return result;
    }
    
    Vec2<t> operator*(const Vec2<t>& v) const {
        return Vec2<t>(
            m[0][0] * v.x + m[0][1] * v.y,
            m[1][0] * v.x + m[1][1] * v.y
        );
    }
    
    Mat2<t> transpose() const {
        return Mat2<t>(m[0][0], m[1][0], m[0][1], m[1][1]);
    }
    
    t det() const {
        return m[0][0] * m[1][1] - m[0][1] * m[1][0];
    }
    
    Mat2<t> inverse() const {
        t d = det();
        if (std::abs(d) < 1e-9) return Mat2<t>(); // Return identity if singular
        return Mat2<t>(m[1][1]/d, -m[0][1]/d, -m[1][0]/d, m[0][0]/d);
    }
};

template <class t>
struct Mat3
{
    t m[3][3];
    
    Mat3() {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                m[i][j] = (i == j) ? 1 : 0;
    }
    
    Mat3(t m00, t m01, t m02,
         t m10, t m11, t m12,
         t m20, t m21, t m22) {
        m[0][0] = m00; m[0][1] = m01; m[0][2] = m02;
        m[1][0] = m10; m[1][1] = m11; m[1][2] = m12;
        m[2][0] = m20; m[2][1] = m21; m[2][2] = m22;
    }
    
    t* operator[](int i) { return m[i]; }
    const t* operator[](int i) const { return m[i]; }
    
    Mat3<t> operator*(const Mat3<t>& other) const {
        Mat3<t> result;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                result[i][j] = 0;
                for (int k = 0; k < 3; k++) {
                    result[i][j] += m[i][k] * other[k][j];
                }
            }
        }
        return result;
    }
    
    Vec3<t> operator*(const Vec3<t>& v) const {
        return Vec3<t>(
            m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z,
            m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
            m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z
        );
    }
    
    Mat3<t> transpose() const {
        return Mat3<t>(
            m[0][0], m[1][0], m[2][0],
            m[0][1], m[1][1], m[2][1],
            m[0][2], m[1][2], m[2][2]
        );
    }
    
    t det() const {
        return m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) -
               m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) +
               m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
    }
    
    Mat3<t> inverse() const {
        t d = det();
        if (std::abs(d) < 1e-9) return Mat3<t>(); // Return identity if singular
        
        Mat3<t> inv;
        inv[0][0] = (m[1][1] * m[2][2] - m[1][2] * m[2][1]) / d;
        inv[0][1] = (m[0][2] * m[2][1] - m[0][1] * m[2][2]) / d;
        inv[0][2] = (m[0][1] * m[1][2] - m[0][2] * m[1][1]) / d;
        inv[1][0] = (m[1][2] * m[2][0] - m[1][0] * m[2][2]) / d;
        inv[1][1] = (m[0][0] * m[2][2] - m[0][2] * m[2][0]) / d;
        inv[1][2] = (m[0][2] * m[1][0] - m[0][0] * m[1][2]) / d;
        inv[2][0] = (m[1][0] * m[2][1] - m[1][1] * m[2][0]) / d;
        inv[2][1] = (m[0][1] * m[2][0] - m[0][0] * m[2][1]) / d;
        inv[2][2] = (m[0][0] * m[1][1] - m[0][1] * m[1][0]) / d;
        
        return inv;
    }
};

template <class t>
struct Mat4
{
    t m[4][4];
    
    Mat4() {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                m[i][j] = (i == j) ? 1 : 0;
    }
    
    Mat4(t m00, t m01, t m02, t m03,
         t m10, t m11, t m12, t m13,
         t m20, t m21, t m22, t m23,
         t m30, t m31, t m32, t m33) {
        m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
        m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
        m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
        m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
    }
    
    t* operator[](int i) { return m[i]; }
    const t* operator[](int i) const { return m[i]; }
    
    Mat4<t> operator*(const Mat4<t>& other) const {
        Mat4<t> result;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                result[i][j] = 0;
                for (int k = 0; k < 4; k++) {
                    result[i][j] += m[i][k] * other[k][j];
                }
            }
        }
        return result;
    }
    
    Vec4<t> operator*(const Vec4<t>& v) const {
        return Vec4<t>(
            m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w,
            m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w,
            m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w,
            m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w
        );
    }
    
    Vec3<t> transformPoint(const Vec3<t>& v) const {
        Vec4<t> temp(v.x, v.y, v.z, 1);
        Vec4<t> result = (*this) * temp;
        return Vec3<t>(result.x, result.y, result.z);
    }
    
    Vec3<t> transformVector(const Vec3<t>& v) const {
        Vec4<t> temp(v.x, v.y, v.z, 0);
        Vec4<t> result = (*this) * temp;
        return Vec3<t>(result.x, result.y, result.z);
    }
    
    Mat4<t> transpose() const {
        return Mat4<t>(
            m[0][0], m[1][0], m[2][0], m[3][0],
            m[0][1], m[1][1], m[2][1], m[3][1],
            m[0][2], m[1][2], m[2][2], m[3][2],
            m[0][3], m[1][3], m[2][3], m[3][3]
        );
    }
    
    t det() const {
        return m[0][3] * m[1][2] * m[2][1] * m[3][0] - m[0][2] * m[1][3] * m[2][1] * m[3][0] -
               m[0][3] * m[1][1] * m[2][2] * m[3][0] + m[0][1] * m[1][3] * m[2][2] * m[3][0] +
               m[0][2] * m[1][1] * m[2][3] * m[3][0] - m[0][1] * m[1][2] * m[2][3] * m[3][0] -
               m[0][3] * m[1][2] * m[2][0] * m[3][1] + m[0][2] * m[1][3] * m[2][0] * m[3][1] +
               m[0][3] * m[1][0] * m[2][2] * m[3][1] - m[0][0] * m[1][3] * m[2][2] * m[3][1] -
               m[0][2] * m[1][0] * m[2][3] * m[3][1] + m[0][0] * m[1][2] * m[2][3] * m[3][1] +
               m[0][3] * m[1][1] * m[2][0] * m[3][2] - m[0][1] * m[1][3] * m[2][0] * m[3][2] -
               m[0][3] * m[1][0] * m[2][1] * m[3][2] + m[0][0] * m[1][3] * m[2][1] * m[3][2] +
               m[0][1] * m[1][0] * m[2][3] * m[3][2] - m[0][0] * m[1][1] * m[2][3] * m[3][2] -
               m[0][2] * m[1][1] * m[2][0] * m[3][3] + m[0][1] * m[1][2] * m[2][0] * m[3][3] +
               m[0][2] * m[1][0] * m[2][1] * m[3][3] - m[0][0] * m[1][2] * m[2][1] * m[3][3] -
               m[0][1] * m[1][0] * m[2][2] * m[3][3] + m[0][0] * m[1][1] * m[2][2] * m[3][3];
    }
    
    Mat4<t> inverse() const {
        Mat4<t> inv;
        t d = det();
        if (std::abs(d) < 1e-9) return Mat4<t>(); // Return identity if singular
        
        inv[0][0] = (m[1][2]*m[2][3]*m[3][1] - m[1][3]*m[2][2]*m[3][1] + m[1][3]*m[2][1]*m[3][2] - m[1][1]*m[2][3]*m[3][2] - m[1][2]*m[2][1]*m[3][3] + m[1][1]*m[2][2]*m[3][3]) / d;
        inv[0][1] = (m[0][3]*m[2][2]*m[3][1] - m[0][2]*m[2][3]*m[3][1] - m[0][3]*m[2][1]*m[3][2] + m[0][1]*m[2][3]*m[3][2] + m[0][2]*m[2][1]*m[3][3] - m[0][1]*m[2][2]*m[3][3]) / d;
        inv[0][2] = (m[0][2]*m[1][3]*m[3][1] - m[0][3]*m[1][2]*m[3][1] + m[0][3]*m[1][1]*m[3][2] - m[0][1]*m[1][3]*m[3][2] - m[0][2]*m[1][1]*m[3][3] + m[0][1]*m[1][2]*m[3][3]) / d;
        inv[0][3] = (m[0][3]*m[1][2]*m[2][1] - m[0][2]*m[1][3]*m[2][1] - m[0][3]*m[1][1]*m[2][2] + m[0][1]*m[1][3]*m[2][2] + m[0][2]*m[1][1]*m[2][3] - m[0][1]*m[1][2]*m[2][3]) / d;
        inv[1][0] = (m[1][3]*m[2][2]*m[3][0] - m[1][2]*m[2][3]*m[3][0] - m[1][3]*m[2][0]*m[3][2] + m[1][0]*m[2][3]*m[3][2] + m[1][2]*m[2][0]*m[3][3] - m[1][0]*m[2][2]*m[3][3]) / d;
        inv[1][1] = (m[0][2]*m[2][3]*m[3][0] - m[0][3]*m[2][2]*m[3][0] + m[0][3]*m[2][0]*m[3][2] - m[0][0]*m[2][3]*m[3][2] - m[0][2]*m[2][0]*m[3][3] + m[0][0]*m[2][2]*m[3][3]) / d;
        inv[1][2] = (m[0][3]*m[1][2]*m[3][0] - m[0][2]*m[1][3]*m[3][0] - m[0][3]*m[1][0]*m[3][2] + m[0][0]*m[1][3]*m[3][2] + m[0][2]*m[1][0]*m[3][3] - m[0][0]*m[1][2]*m[3][3]) / d;
        inv[1][3] = (m[0][2]*m[1][3]*m[2][0] - m[0][3]*m[1][2]*m[2][0] + m[0][3]*m[1][0]*m[2][2] - m[0][0]*m[1][3]*m[2][2] - m[0][2]*m[1][0]*m[2][3] + m[0][0]*m[1][2]*m[2][3]) / d;
        inv[2][0] = (m[1][1]*m[2][3]*m[3][0] - m[1][3]*m[2][1]*m[3][0] + m[1][3]*m[2][0]*m[3][1] - m[1][0]*m[2][3]*m[3][1] - m[1][1]*m[2][0]*m[3][3] + m[1][0]*m[2][1]*m[3][3]) / d;
        inv[2][1] = (m[0][3]*m[2][1]*m[3][0] - m[0][1]*m[2][3]*m[3][0] - m[0][3]*m[2][0]*m[3][1] + m[0][0]*m[2][3]*m[3][1] + m[0][1]*m[2][0]*m[3][3] - m[0][0]*m[2][1]*m[3][3]) / d;
        inv[2][2] = (m[0][1]*m[1][3]*m[3][0] - m[0][3]*m[1][1]*m[3][0] + m[0][3]*m[1][0]*m[3][1] - m[0][0]*m[1][3]*m[3][1] - m[0][1]*m[1][0]*m[3][3] + m[0][0]*m[1][1]*m[3][3]) / d;
        inv[2][3] = (m[0][3]*m[1][1]*m[2][0] - m[0][1]*m[1][3]*m[2][0] - m[0][3]*m[1][0]*m[2][1] + m[0][0]*m[1][3]*m[2][1] + m[0][1]*m[1][0]*m[2][3] - m[0][0]*m[1][1]*m[2][3]) / d;
        inv[3][0] = (m[1][2]*m[2][1]*m[3][0] - m[1][1]*m[2][2]*m[3][0] - m[1][2]*m[2][0]*m[3][1] + m[1][0]*m[2][2]*m[3][1] + m[1][1]*m[2][0]*m[3][2] - m[1][0]*m[2][1]*m[3][2]) / d;
        inv[3][1] = (m[0][1]*m[2][2]*m[3][0] - m[0][2]*m[2][1]*m[3][0] + m[0][2]*m[2][0]*m[3][1] - m[0][0]*m[2][2]*m[3][1] - m[0][1]*m[2][0]*m[3][2] + m[0][0]*m[2][1]*m[3][2]) / d;
        inv[3][2] = (m[0][2]*m[1][1]*m[3][0] - m[0][1]*m[1][2]*m[3][0] - m[0][2]*m[1][0]*m[3][1] + m[0][0]*m[1][2]*m[3][1] + m[0][1]*m[1][0]*m[3][2] - m[0][0]*m[1][1]*m[3][2]) / d;
        inv[3][3] = (m[0][1]*m[1][2]*m[2][0] - m[0][2]*m[1][1]*m[2][0] + m[0][2]*m[1][0]*m[2][1] - m[0][0]*m[1][2]*m[2][1] - m[0][1]*m[1][0]*m[2][2] + m[0][0]*m[1][1]*m[2][2]) / d;
        
        return inv;
    }
};

typedef Vec2<float> Vec2f;
typedef Vec2<int> Vec2i;
typedef Vec3<float> Vec3f;
typedef Vec3<int> Vec3i;
typedef Vec4<float> Vec4f;
typedef Vec4<int> Vec4i;
typedef Mat2<float> Mat2f;
typedef Mat2<int> Mat2i;
typedef Mat3<float> Mat3f;
typedef Mat3<int> Mat3i;
typedef Mat4<float> Mat4f;
typedef Mat4<int> Mat4i;

template <class t>
std::ostream &operator<<(std::ostream &s, Vec2<t> &v)
{
    s << "(" << v.x << ", " << v.y << ")\n";
    return s;
}

template <class t>
std::ostream &operator<<(std::ostream &s, Vec3<t> &v)
{
    s << "(" << v.x << ", " << v.y << ", " << v.z << ")\n";
    return s;
}

template <class t>
std::ostream &operator<<(std::ostream &s, Vec4<t> &v)
{
    s << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")\n";
    return s;
}

template <class t>
std::ostream &operator<<(std::ostream &s, Mat2<t> &m)
{
    s << "[" << m[0][0] << ", " << m[0][1] << "]\n";
    s << "[" << m[1][0] << ", " << m[1][1] << "]\n";
    return s;
}

template <class t>
std::ostream &operator<<(std::ostream &s, Mat3<t> &m)
{
    s << "[" << m[0][0] << ", " << m[0][1] << ", " << m[0][2] << "]\n";
    s << "[" << m[1][0] << ", " << m[1][1] << ", " << m[1][2] << "]\n";
    s << "[" << m[2][0] << ", " << m[2][1] << ", " << m[2][2] << "]\n";
    return s;
}

template <class t>
std::ostream &operator<<(std::ostream &s, Mat4<t> &m)
{
    s << "[" << m[0][0] << ", " << m[0][1] << ", " << m[0][2] << ", " << m[0][3] << "]\n";
    s << "[" << m[1][0] << ", " << m[1][1] << ", " << m[1][2] << ", " << m[1][3] << "]\n";
    s << "[" << m[2][0] << ", " << m[2][1] << ", " << m[2][2] << ", " << m[2][3] << "]\n";
    s << "[" << m[3][0] << ", " << m[3][1] << ", " << m[3][2] << ", " << m[3][3] << "]\n";
    return s;
}