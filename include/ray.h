#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray{
public:
    ray() {};
    ray(const point3& origin, const vec3& direction): m_origin{origin}, m_direction{direction}{};      

    const point3& origin() const {return m_origin;}
    const vec3& direction() const {return m_direction;}

    point3 at(double t){
        return m_origin+m_direction*t; 
        // P(t) = origin + direction *t 
    }

    /**
     * Las funciones ray::origin() y ray::direction() regresan una referencia inmutable a sus memebers 
     */


private: 

    point3 m_origin; 
    vec3 m_direction; 

};


#endif