//
//  Quaternion.cpp
//  OpenGLSetupTestWithSTB
//
//  Created by Forrest Miller on 1/16/18.
//  Copyright © 2018 Forrest Miller. All rights reserved.
//

#include "Quaternion.hpp"

#define FLOATING_POINT_TOLERANCE 0.0000001f //i just made this value up as a value close to 0 to check for cancelation error

//-----------------------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------------------

Quaternion::Quaternion() {
    this->theta = 0.0f;
    this->ux = this->uy = this->uz = this->real = this->i = this->j = this->k = 0.0f;
}

Quaternion::Quaternion(const float ux, const float uy, const float uz) {
    this->theta = 0.0f;
    this->ux = ux;
    this->uy = uy;
    this->uz = uz;
    this->computeSelf();
}

Quaternion::Quaternion(const aiVector3D p) {
    this->theta = 0.0f;
    this->ux = p.x;
    this->uy = p.y;
    this->uz = p.z;
    this->computeSelf();
}

Quaternion::Quaternion(aiVector3D p, float theta) {
    this->theta = theta;
    this->ux = p.x;
    this->uy = p.y;
    this->uz = p.z;
    this->computeSelf();
}

Quaternion::Quaternion(const float ux, const float uy, const float uz, const float theta) {
    this->theta = theta;
    this->ux = ux;
    this->uy = uy;
    this->uz = uz;
    this->computeSelf();
}

//Copy Constructor
Quaternion::Quaternion(const Quaternion & other) {
//    std::cout << "Quaternion Copy Constructor was called\n";
    this->real = other.real;
    this->i = other.i;
    this->j = other.j;
    this->k = other.k;
    this->theta = other.theta;
    this->ux = other.ux;
    this->uy = other.uy;
    this->uz = other.uz;
}

//Construct one of my quaternions from an assimp quaternion
Quaternion::Quaternion(const aiQuaternion & q) {
    //With w as the real part:
    this->real = (float) q.w;
    this->i = (float) q.x;
    this->j = (float) q.y;
    this->k = (float) q.z;
    
    this->theta = (float) 2.0f * acos(q.w);//Also could compute by: 2.0 * atan2(complexComponentNorm, real)
    
    
    float complexComponentNorm = sqrt(pow(this->i, 2.0f) + pow(this->j, 2.0f)
                      + pow(this->k, 2.0f));

    //Set the rotation axis
    if (abs(complexComponentNorm) <= FLOATING_POINT_TOLERANCE) { //Check for floating point error
        ux = uy = uz = 0.0f;
    }
    else {
        this->ux = this->i / complexComponentNorm;
        this->uy = this->j / complexComponentNorm;
        this->uz = this->k / complexComponentNorm;
    }
}

//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------

Quaternion::~Quaternion() {
    ; //Nothing currently to destroy
}

//-----------------------------------------------------------------------------
// Operators
//-----------------------------------------------------------------------------

//Note that the rotation from quaternion other will be applied first before this quaternions rotation
Quaternion Quaternion::operator*(const Quaternion & other) const {
    
    // Math to be computed:
    // (a1 + b1i + c1j + d1k)*(a2 + b2i + c2j + d2k)
    //                             =
    //                                a1a2 + a1b2i + a1c2j   + a1d2k
    //                                     + b1a2i + b1b2i^2 + b1c2ij  + b1d2ik
    //                                     + c1a2j + c1b2ji  + c1c2j^2 + c1d2jk
    //                                     + d1a2k + d1b2ki  + d1c2kj  + d1d2k^2
    //
    //                             =
    //                                a1a2 - b1b2 - c1c2 - d1d2
    //                                     + (a1b2 + b1a2 + c1d2 - d1c2)i
    //                                     + (a1c2 - b1d2 + c1a2 + d1b2)j
    //                                     + (a1d2 + b1c2 - c1b2 + d1a2)k
    //
    //
    
    Quaternion product(*this); //Create a new quaternion to be returned
    //Compute the Hamilton product as shown above
    product.real = (this->real * other.real) - (this->i * other.i) - (this->j * other.j) - (this->k * other.k);
    product.i = (this->real * other.i) + (this->i * other.real) + (this->j * other.k) - (this->k * other.j);
    product.j = (this->real * other.j) - (this->i * other.k) + (this->j * other.real) + (this->k * other.i);
    product.k = (this->real * other.k) + (this->i * other.j) - (this->j * other.i) + (this->k * other.real);
    
    
    //Calculate What the new axis of rotation is:
    float norm = sqrt(product.i*product.i + product.j*product.j + product.k*product.k);

    if (abs(norm) <= FLOATING_POINT_TOLERANCE) { //Don't divide by 0 (or anything close to 0 from floating-point error)
        product.ux = product.uy = product.uz = 0.0f;
    }
    else {
        product.ux = i / norm;
        product.uy = j / norm;
        product.uz = k / norm;
    }
    
    //For debug
    //std::cout << "\nAfter multiplication, the theta of the product is: ";
    product.theta = 2.0f * atan2(norm, product.real);
    //std::cout << product.theta << std::endl;

    //For debug:
    //std::cout << "\nNorm after Hamilton Product is: " << norm << std::endl;

    return product;
}

Quaternion Quaternion::operator=(const Quaternion & other) {
    //For debug
//    std::cout << "\nQuaternion Copy Assignment operator was called" << std::endl;
    this->real = other.real;
    this->i = other.i;
    this->j = other.j;
    this->k = other.k;
    this->theta = other.theta;
    this->ux = other.ux;
    this->uy = other.uy;
    this->uz = other.uz;
    return *this;
}

bool Quaternion::operator==(const Quaternion & other) const{
    if (this->real == other.real) {
        if(this->i == other.i && this->j == other.j && this->k == other.k) {
            if (this->getNormalizedRotationAxis() == other.getNormalizedRotationAxis())
            return true;
        }
    }
    return false;
}

bool Quaternion::operator==(const aiQuaternion& q) const {
    return (this->real == q.w && this->i == q.x && this->j == q.y && this->k == q.z);
}

//-----------------------------------------------------------------------------
// Public Member Functions
//-----------------------------------------------------------------------------
aiVector3D Quaternion::computeRotation(aiVector3D p, float theta) const {
    //Will be computed using the Hamilton product
    //p' = qpq^(-1)
    
    //So it turns out I don't have any way to construct a quaternion from the components of
    //a quaternion, so I am going to construct 2 quaternions with garbage values and then manually change
    //them to the correct values. This means that these quaternions components outside of these values will
    //be incorrect (i.e. the rotation axis will be bogus, as will theta, etc.)
    
    Quaternion vectorAsPureQuaternion; //Construct a quaternion
    Quaternion qConjugate; //Construct another quaternion
    
    //Manually set the parts of this quaternion to represent the p as a quaternion with real part == 0
    vectorAsPureQuaternion.real = 0.0f;
    vectorAsPureQuaternion.i = p.x;
    vectorAsPureQuaternion.j = p.y;
    vectorAsPureQuaternion.k = p.z;
    
    //Construct qConjugate from q (q is this)
    qConjugate.real = this->real;
    qConjugate.i = -this->i; //To conjugate, just negate i,j and k
    qConjugate.j = -this->j;
    qConjugate.k = -this->k;
    
    //Now use the product operator to compute the product
    vectorAsPureQuaternion = (*this) * vectorAsPureQuaternion ; //Compute q*p
    vectorAsPureQuaternion = vectorAsPureQuaternion * qConjugate; //Compute (qp)*q^(-1)
    
    //Return the result as a 3vector
    return aiVector3D(vectorAsPureQuaternion.i, vectorAsPureQuaternion.j, vectorAsPureQuaternion.k);
}

aiVector3D Quaternion::computeRotation(aiVector3D p) const { //Uses internally stored angle
    return computeRotation(p, this->theta);
}

aiVector3D Quaternion::computeRotation(float x, float y, float z) const { //Uses internally stored angle
    return computeRotation(aiVector3D(x,y,z));
}

aiVector3D Quaternion::computeRotation(float x, float y, float z, float theta) const {
    return computeRotation(aiVector3D(x,y,z), theta);
}

//Might want to rewrite this next function once I actually have a better idea of how I will be using this class (will rotations be on GPU?) (Should I add a function to turn product of quaternions into a 4x4 matrix [take in an array of quaternions plus an int for arraysize, and return a 4x4 (3x3?) matrix?]  (NOTE THAT I HAVE IMPLEMENTED THIS NOW))
//void Quaternion::computeRotation(float * arrayOfVertices, int arraySize) const {
//void const Quaternion::computeRotation(float * arrayOfVertices, int numVertices) {
//    std::stringstream ss;
//    if (0 != arraySize % 3) {
//        ss << "Error! The array size of " << arraySize << " given to this quaternions' computeRotation funcion is not a multiple of 3!";
//        throw ss.str();
//    }
//}

aiVector3D Quaternion::getNormalizedRotationAxis() const{
    aiVector3D temp(ux,uy,uz);
    return temp.NormalizeSafe(); //NormalizeSafe has extra checking in case vector is 0 vector
}

aiQuaternion Quaternion::to_aiQuaternion() const {
    return aiQuaternion(aiVector3D(ux, uy, uz), theta);
}


aiMatrix3x3 Quaternion::toRotationMatrix() const {
    /* Math to compute (not that are aiMatrices are constructed by column, not by row):
     With: a = real, b=i, c=j, d=k
            _                                                                       _
           /  a^2 + b^2 - c^2 - d^2         2bc-2ad                  2bd+2ac         \
     R =   |                                                                         |
           |       2bc + 2ad            a^2-b^2+c^2-d^2              2cd-2ab         |
           |                                                                         |
           |      2bd - 2ac                2cd+2ab              a^2-b^2-c^2+d^2      |
           \_                                                                       _/
     */
    
    return aiMatrix3x3(
                //col 1
    pow(real, 2.0f) + pow(i, 2.0f) - pow(j, 2.0f) - pow(k, 2.0f),
    2.0f*i*j + 2.0f*real*k,
    2.0f*i*k - 2.0f*real*j,
                                    //col 2
                            2.0f*i*j,
                            pow(real, 2.0f) + pow(i, 2.0f) - pow(j, 2.0f) - pow(k, 2.0f),
                            2.0f*j*k + 2.0f * real * i,
                                                            //col 3
                                                    2.0f*i*k + 2.0f*real*j,
                                                    2.0f*j*k - 2.0f*real*i,
                                                    pow(real, 2.0f) - pow(i, 2.0f) - pow(j, 2.0f) + pow(k, 2.0f)
                       );
}

void Quaternion::changeRotationAxis(float ux, float uy, float uz) {
    this->ux = ux;
    this->uy = uy;
    this->uz = uz;
    computeSelf();
}

void Quaternion::changeRotationAxis(const aiVector3D & u) {
    this->ux = u.x;
    this->uy = u.y;
    this->uz = u.z;
    computeSelf();
}


//-----------------------------------------------------------------------------
// Private Functions
//-----------------------------------------------------------------------------
void Quaternion::computeSelf() { //Computes self from parameters
    float halfTheta = theta/2.0f;
    
    //This next bit prevents theta from growing too big to try to keep floating point error in check
    while (theta > 2.0f*3.1415927f || theta < 0.0f) {
        if (theta > 2.0f*3.1415927f) {theta -= 2.0f*3.1415927f;}
        else if (theta < 0.0f) {theta += 2.0f*3.1415927f;}
    }
    
    //i = j = k = 0.0f;
    real = cos(halfTheta);
    i = ux*sin(halfTheta);
    j = uy*sin(halfTheta);
    k = uz*sin(halfTheta);
    normalizeSelf(); //Makes sure quaternion is a unit quaternion
}


void Quaternion::normalizeSelf() {
    //Do the math to normalize this quaternion
    // Calculate the 2-norm of the quaternion (same as 2-norm for a 4-component vector)
    float norm = sqrt(pow(this->real, 2.0f) + pow(this->i, 2.0f) + pow(this->j, 2.0f)
                      + pow(this->k, 2.0f));
    
    //If the norm is really tiny (but not exactly 0) then it probably should be 0
    if (abs(norm) <= FLOATING_POINT_TOLERANCE) {
        this->i = this->j = this->k = this->real = 0.0f; //Set components all to 0.0f
        return;
    }
    
    // else 
    //   Divide each component by the norm to normalize the quaternion
    this->real = this->real / norm;
    this->i = this->i / norm;
    this->j = this->j / norm;
    this->k = this->k / norm;
}
