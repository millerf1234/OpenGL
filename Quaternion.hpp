//
//  Quaternion.hpp
//
//  Quaternions are useful for doing rotations in 3D due to being able to
//  rotate freely without encountering gimbal lock (though they have to be used
//  in the correct way to do so, just creating 3 quaternions for rotations along
//  three axis's will still result in gimbal lock).
//  For more information see:
//                https://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation
//
//
//  Notes on how to use this class:
//    A quaternion is constructed with a vector that defines the axis of
//    rotation for this quaternion. Once constructed, call rotate(aiVector3D) to
//    do rotation
//
//  Created by Forrest Miller on 1/16/18.
//  Copyright © 2018 Forrest Miller. Not released under any liscense, because I
//  have not debugged this that much, and therefor should not be used for any reason
//  by anyone.
//
//  0.91 Added ability to convert Quaternion into a 3x3 rotaton matrix (aiMatrix3x3)
//  0.9 Have everything working except the rotation functions that take in arrays of vertices  1/23/2018
//  0.2 Added functionality to covert between this and aiQuaternion 1/20/2018
//  0.1 Basic Class functionality complete 1/19/2018
//  0.0 Wrote class header/outline   1/16/18

#ifndef Quaternion_hpp
#define Quaternion_hpp

#include "glad.h"
#include <assimp/Importer.hpp> //Gives access to the aiVector3D
#include <cmath>
#include <sstream>

#include <iostream> //For debug, can remove eventually


constexpr double e = 2.7182818284590452353602875;


class Quaternion {
private:
    float theta; //Angle to rotate on each call to rotate
    float real, i, j, k, ux, uy, uz; //Components, components of rotation axis
    
    //double   q    = e^((theta/2)*(u_x*i + u_y*j + u_z*k))
    //         q    = cos(theta/2) + (u_x*i + u_y*j + u_z*k)sin(theta/2)
    //
    //double q^(-1) = cos(theta/2) - (u_x*i + u_y*j + u_z*k)sin(theta/2)
    
    //Private member functions:
    void computeSelf(); //Computes self from parameters (the rotation vector u)
    void normalizeSelf();
    
public:
    //Constructors
    //ux,uy,uz represent the axis of rotation
    Quaternion();
    Quaternion(const float ux, const float uy, const float uz);
    Quaternion(const aiVector3D rotationAxis);
    Quaternion(const float ux, const float uy, const float uz, const float thetaStart);
    Quaternion(const aiVector3D rotationAxis, const float thetaStart);
    Quaternion(const Quaternion &); //Copy constructor
    
    Quaternion(const aiQuaternion &); //Construct from assimp Quaternion

    
    
    //Destructor
    ~Quaternion();
    
    //Public member functions
    aiVector3D computeRotation(aiVector3D p) const; //Uses internally stored angle
    aiVector3D computeRotation(aiVector3D p, float theta) const;
    aiVector3D computeRotation(float x, float y, float z) const;
    aiVector3D computeRotation(float x, float y, float z, float theta) const;
    
    //For an array of vertices:
    //void computeRotation(float * arrayOfVertices, int arraySize) const;
    //void computeRotation(float * arrayOfVertices, int startOffset, int arraySize) const;
    ////Stride is the distance between vertex data sets
    //void computeRotation(float * arrayOfVertices, int startOffset, int arraySize, int stride) const;
    //void computeRotation(float * arrayOfVertices, int startOffset, int arraySize, int stride, int componentsPerVert) const;

    
    void changeTheta(const float theta) {this->theta = theta; computeSelf();}
    
    void changeRotationAxis(float ux, float uy, float uz);
    void changeRotationAxis(const aiVector3D& u);
    
    float getRotationAxis_x() const {return this->ux;}
    float getRotationAxis_y() const {return this->uy;}
    float getRotationAxis_z() const {return this->uz;}
    
    aiVector3D getRotationAxis() const {return aiVector3D(ux, uy, uz);}
    aiVector3D getNormalizedRotationAxis() const;
    
    float getTheta() const {return this->theta;}
    float getRealPart() const {return this->real;} //i.e. float getRealPart() {return cos(theta / 2.0);}
    
    aiQuaternion to_aiQuaternion() const; //Haven't tested this extensivly yet
    
    aiMatrix3x3 toRotationMatrix() const;
    
    float geti() const {return i;}
    float getj() const {return j;}
    float getk() const {return k;}
    
//    //To use the next 2 methods, please handle creating the array for the argument
//    void getComponents(float *); //Requires a float array of size 4
//    void getComponentsInv(float *); //Requires a float array of size 4
    
    
    //Operators:
    Quaternion operator*(const Quaternion &) const;
    Quaternion operator=(const Quaternion &);
    bool operator==(const Quaternion &) const;
    bool operator==(const aiQuaternion &) const;
};

#endif /* Quaternion_hpp */
