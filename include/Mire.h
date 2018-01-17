//
// Created by julien on 10/01/18.
//

#ifndef AR_MIRE_H
#define AR_MIRE_H


#include <vector>
#include <mat.h>
#include <mesh.h>

class Mire : public Mesh{

public:

    Mire(int row, int col, float squareSize, Transform t);
    Transform& getTransform(){return transform;}
    void setTransform(const Transform& t){transform = t;}
private:
    Transform transform;
};


#endif //AR_MIRE_H
