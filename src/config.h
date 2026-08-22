#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <random>
#include <cmath>

#define M_PI           3.14159265358979323846

struct Vec2 {
	float x;
	float y;
    
    float dot(Vec2& _argVec){
        return (x * _argVec.x) + (y * _argVec.y);
    }

    float magnitude(){
        return std::sqrt(x*x + y*y);
    }

    Vec2 operator+(const Vec2& rhs) const {
        return Vec2{x + rhs.x, y + rhs.y};
    }

    Vec2 operator+(const float& rhs) const {
        return Vec2{x + rhs, y + rhs};
    }

    Vec2 operator-(const Vec2& rhs) const {
        return Vec2{x - rhs.x, y - rhs.y};
    }

    Vec2 operator-(const float& rhs) const {
        return Vec2{x - rhs, y - rhs};
    }

    Vec2 operator*(const Vec2& rhs) const {
        return Vec2{x * rhs.x, y * rhs.y};
    }

    Vec2 operator*(const float& rhs) const {
        return Vec2{x * rhs, y * rhs};
    }

    Vec2 operator/(const Vec2& rhs) const {
        return Vec2{x / rhs.x, y / rhs.y};
    }

    Vec2 operator/(const float& rhs) const {
        return Vec2{x / rhs, y / rhs};
    }

    Vec2& operator*=(const Vec2& rhs){
        x *= rhs.x;
        y *= rhs.y;
        return *this;
    }
};

struct Vec3 {
    float x;
    float y;
    float z;

    Vec3 operator+(const Vec3& rhs) const {
        return Vec3{x + rhs.x, y + rhs.y, z + rhs.z};
    }

    Vec3 operator+(const float& rhs) const {
        return Vec3{x + rhs, y + rhs, z + rhs};
    }

    Vec3 operator-(const Vec3& rhs) const {
        return Vec3{x - rhs.x, y - rhs.y, z - rhs.z};
    }

    Vec3 operator-(const float& rhs) const {
        return Vec3{x - rhs, y - rhs, z - rhs};
    }

    Vec3 operator*(const Vec3& rhs) const {
        return Vec3{x * rhs.x, y * rhs.y, z * rhs.z};
    }

    Vec3 operator*(const float& rhs) const {
        return Vec3{x * rhs, y * rhs, z * rhs};
    }

    Vec3 operator/(const Vec3& rhs) const {
        return Vec3{x / rhs.x, y / rhs.y, z / rhs.z};
    }

    Vec3 operator/(const float& rhs) const {
        return Vec3{x / rhs, y / rhs, z / rhs};
    }

    Vec3& operator+=(const Vec3& rhs){
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    Vec3& operator*=(const Vec3& rhs){
        x *= rhs.x;
        y *= rhs.y;
        z *= rhs.z;
        return *this;
    }
};

struct Vec4 {
	float x;
	float y;
    float z;
    float w;

    Vec4 operator+(const Vec4& rhs) const {
        return Vec4{x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w};
    }

    Vec4 operator+(const float& rhs) const {
        return Vec4{x + rhs, y + rhs, z + rhs, w + rhs};
    }

    Vec4 operator-(const Vec4& rhs) const {
        return Vec4{x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w};
    }

    Vec4 operator-(const float& rhs) const {
        return Vec4{x - rhs, y - rhs, z - rhs, w - rhs};
    }

    Vec4 operator*(const Vec4& rhs) const {
        return Vec4{x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w};
    }

    Vec4 operator*(const float& rhs) const {
        return Vec4{x * rhs, y * rhs, z * rhs, w * rhs};
    }

    Vec4 operator/(const Vec4& rhs) const {
        return Vec4{x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w};
    }

    Vec4 operator/(const float& rhs) const {
        return Vec4{x / rhs, y / rhs, z / rhs, w / rhs};
    }

    Vec4& operator*=(const Vec4& rhs){
        x *= rhs.x;
        y *= rhs.y;
        z *= rhs.z;
        w *= rhs.w;
        return *this;
    }
};