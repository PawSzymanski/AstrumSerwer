#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

const float EPSILON = 0.0001f;

inline float abs_f(float x)
{
    return(x*(1 - (2 * (x < 0))));
}

inline sf::Vector2f crossVS(const sf::Vector2f & v, float a)
{
    return sf::Vector2f(v.y*a,-v.x*a);
}

inline sf::Vector2f crossSV(float a, const sf::Vector2f & v)
{
    return sf::Vector2f(-v.y*a, v.x*a);
}

inline float crossVV(const sf::Vector2f & a, const sf::Vector2f & b)
{
    return a.x * b.y - a.y * b.x;
}

inline float dot(const sf::Vector2f & a, const sf::Vector2f & b)
{
    return a.x * b.x + a.y * b.y;
}

inline float vecLenght(const sf::Vector2f & a)
{
    return sqrt(dot(a, a));
}

inline float vecLenghtSq(const sf::Vector2f & a)
{
    return dot(a,a);
}

inline sf::Vector2f vecNormalize(sf::Vector2f & a)
{
    return a / vecLenght(a);
}

inline float distance(const sf::Vector2f & a, const sf::Vector2f & b)
{
    sf::Vector2f c = b - a;
    return sqrt(dot(c,c));
}

inline float distanceSq(const sf::Vector2f & a, const sf::Vector2f & b)
{
    sf::Vector2f c = b - a;
    return dot(c,c);
}

inline bool equal(float a, float b)
{
    return std::abs(a - b) <= EPSILON;
}

inline float sqr(float a)
{
    return a*a;
}

inline bool BiasGreaterThan(float a, float b)
{
    const float k_biasRelative = 0.95f;
    const float k_biasAbsolute = 0.01f;
    return a >= b * k_biasRelative + a * k_biasAbsolute;
}
