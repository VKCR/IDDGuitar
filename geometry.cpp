#include "geometry.h"

//this function takes the vector v in the plane source, and scales it to fit in the plane dest
Point scalePoint(Point p, Plane dest, Plane source){
  Vector v = p-source.topleft;
  v = scaleVect(v, dest, source);
  Point result = dest.topleft+v;
  return result;
}

Vector scaleVect(Vector v, Plane dest, Plane source){
  if (source.xaxis*source.yaxis !=0){
    Vector nul;
    cout<<"scaleVect only implemented for orthogonal source plane!";
    return nul;
  }
  
  v.x /= source.xaxis.x;
  v.y /= source.yaxis.y;
  Vector result;
  result.x = v.x * dest.xaxis.x + v.y * dest.yaxis.x;
  result.y = v.x * dest.xaxis.y + v.y * dest.yaxis.y;
  return result;
}
