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
    void setHeight(int x, int y, float z);
private:
    Color interpColor(const Color& base, const Color& max, float val);

    Transform transform;
    int m_row;
    int m_col;
    float m_squareSize;
};

class Object : public Mesh{
public:

    Object(Transform t, std::string filePath = "Test/floor.obj", vec3 objectColor = vec3(1,1,1));
    Transform& getTransform(){return transform;}
    void setTransform(const Transform& t){transform = t;}
private:
    Transform transform;
};


#endif //AR_MIRE_H
