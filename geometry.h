//point in a plane
#ifndef GEOMETRY_H
#define GEOMETRY_H
#include <cmath>
#include <iostream>
using namespace std;

struct Vector{
  float x;
  float y;
  Vector(){x = 0; y = 0;}
  Vector(float xx, float yy){x = xx; y = yy;}

  //a few overloaded operators
  Vector operator=(const Vector& v){
    this->x = v.x;
    this->y = v.y;
  }
    
  Vector operator+(const Vector& v) {
    Vector v2(this->x+v.x, this->y+v.y);
    return v2;
  }

  Vector operator-(const Vector& v) {
    Vector v2(this->x-v.x, this->y-v.y);
    return v2;
  }
  
  //dot product
  float operator*(const Vector& v) {
    return this->x*v.x + this->y*v.y;
  }

  //scale
  Vector operator*(float k){
    this->x *= k;
    this->y *= k;
  }
  
  //get the norm
  float Norm(){
    return sqrt(x*x+y*y);
  }
  //function to normalize the vector
  void Normalize(){
    float norm = Norm();
    x = x/norm;
    y = y/norm;
  }
};

struct Point{
  float x;
  float y;
  Point(){x = 0; y = 0;}
  Point(float xx, float yy){x = xx; y = yy;}

  //a few overloaded operators
  Point operator=(const Point& p){
    this->x = p.x;
    this->y = p.y;
  }
    
  Point operator+(const Vector& v) {
    Point p(this->x+v.x, this->y+v.y);
    return p;
  }

  Vector operator-(const Point& p) {
    Vector v(this->x-p.x, this->y-p.y);
    return v;
  }
};



//Plane 
struct Plane{
  Point topleft;
  Vector xaxis;
  Vector yaxis;
  Plane(Point tl, Vector xa, Vector ya){topleft = tl; xaxis = xa; yaxis = ya;}
  
  //a few overloaded operators
  Plane operator=(const Plane& p){
    this->topleft = p.topleft;
    this->xaxis = p.xaxis;
    this->yaxis = p.yaxis;
  }
};

//function to scale a vector from one plane to another
Point scalePoint(Point p, Plane dest, Plane source);
Vector scaleVect(Vector v, Plane dest, Plane source);




//to print
inline ostream& operator<<(ostream& os, const Vector& v)
{
  os << "(" << v.x << " ; " << v.y << ")";
  return os;
}

inline ostream& operator<<(ostream& os, const Point& p)
{
  os << "(" << p.x << " ; " << p.y << ")";
  return os;
}

inline ostream& operator<<(ostream& os, const Plane& pl)
{
  os<<"Plane of origin " << pl.topleft << " and of x-axis " << pl.xaxis << " and of y-axis " << pl.yaxis<<endl;
  return os;
}

//what we need is intersection

//HAVE A WAY TO CONVERT BETWEEN POINTS IN SPACE AND POINT IN A PLANE!!!!
//create another layer of data abstraction, for 2D drawings, and for 3D points in space
//structures screenpoint and screen
//Have a way to get the projected plane from a distance of a source
//je me complique la vie, il suffirait de faire comme dans SICP, faire un plan defini par deux vecteurs, et puis apres il suffit de determiner la forme du deuxieme truc apres avoir scale 

#endif
