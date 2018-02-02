//
// Created by julien on 10/01/18.
//

#include "Mire.h"
#include "wavefront.h"

Mire::Mire(int row, int col, float squareSize, Transform t): Mesh(GL_TRIANGLES) {
    // squares black and white
    bool colorBlack = true;
    m_row = row+1;
    m_col = col+1;
    m_squareSize = squareSize;

//    std::vector<vec3> tmp;
//    std::vector<int> indices;
//
//    for(int i = 0; i < row; ++i)
//        for(int j = 0; j < col; ++j)
//            tmp.push_back(vec3(j * squareSize, i * squareSize, 0.0));
//
//    for(int i = 0; i < (col-1); i++){
//        int cptY = 0 ;
//        int cptZ = 0;
//        for(int j = 0; j < (row-1); j++){
//            int cptX = 0;
//            cptZ++;
//            for(int k = 0; k < 2; k++){
//                indices.push_back(j * col + cptX + i);
//                indices.push_back(col * cptZ + i);
//                indices.push_back(col * cptY + i + 1);
//
//                cptX++;
//                if(k == 0)
//                    cptY++;
//            }
//        }
//    }
//
//    for(int i = 0; i < indices.size(); i++) {
//        color(1,1,1);
//        vertex(tmp[indices[i]]);
//    }
//
//    transform = t;


    float height = row * squareSize;
    float width = col * squareSize;

    float z = 0.0;

    int count = 0;

    for(float h = -squareSize; h < height;h += squareSize) {
        if(count%2 == 0)
            colorBlack = true;
        else
            colorBlack = false;

        count++;
        for(float w = -squareSize;w < width;w += squareSize) {
            // change color
            if(colorBlack) {
                colorBlack = false;
                color(Color(0,0,0));
            }else {
                colorBlack = true;
                color(Color(1,1,1));
            }

            // triangle 1 (left bottom)
            Point a(w + squareSize, h, z);
            Point b(w, h, z);
            Point c(w, h + squareSize, z);

            // triangle 2 (right up)
            Point d = a;
            Point e = c;
            Point f(w + squareSize, h + squareSize, z);

            // create mesh vertices
            vertex(a.x, a.y, a.z);
            vertex(b.x, b.y, b.z);
            vertex(c.x, c.y, c.z);


            vertex(d.x, d.y, d.z);
            vertex(e.x, e.y, e.z);
            vertex(f.x, f.y, f.z);
        }
    }
    transform = t;
}

void Mire::setHeight(int x, int y, float z) {
    float X = (x-1) * (m_squareSize);
    float Y = (y-1) * (m_squareSize);

    for(int i = 0; i < m_positions.size(); ++i){
        if(m_positions[i].x == X && m_positions[i].y == Y){
            vec3 tmp = m_positions[i];
            tmp.z += z;
            vertex(i, tmp);
        }
    }
}

Object::Object(Transform t, std::string filename, vec3 objectColor) {

    Mesh mesh = read_mesh(filename.c_str());
    if(mesh == Mesh::error()) exit(0);

    std::vector<TriangleData> tmp_triangle;
    std::vector<vec3> tmp_vec3;

    for(int i= 0; i < mesh.triangle_count(); i++)
        tmp_triangle.push_back( mesh.triangle(i) );

    for(int i= 0; i < tmp_triangle.size(); i++) {
        tmp_vec3.push_back(tmp_triangle[i].a);
        tmp_vec3.push_back(tmp_triangle[i].b);
        tmp_vec3.push_back(tmp_triangle[i].c);
    }

    for(int i = 0; i < tmp_vec3.size(); i++) {
        color(objectColor.x, objectColor.y, objectColor.z);
        vertex(tmp_vec3[i]);
    }

    transform = t;
}
