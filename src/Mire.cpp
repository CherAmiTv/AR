//
// Created by julien on 10/01/18.
//

#include "Mire.h"
#include "wavefront.h"

Mire::Mire(int row, int col, float squareSize, Transform t): Mesh(GL_TRIANGLES) {
    // squares black and white
    bool colorBlack = true;

    int m_row = row;
    int m_col = col;
    float m_squareSize = squareSize;
    float height = row;//*squareSize;
    float width = col;//*squareSize;

    float z = 0.0;

    int count = 0;

    for(float h = -squareSize;h < height;h += squareSize) {
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
    // (-1,-1), origine en haut a gauche
    if(x == -1) {
        if(y == -1){
            // coin en haut a gauche
            int id = 1;
            vec3 p = m_positions[id];
            vertex(id, p.x, p.y, z);

        }else if(y == m_row-1) {
            // coin en bas a gauche
            int id1 = (6*m_col * (m_row-1)) + 2;
            int id2 = (6*m_col * (m_row-1)) + 4;
            vec3 p1 = m_positions[id1];
            vec3 p2 = m_positions[id2];
            vertex(id1, p1.x, p1.y, z);
            vertex(id2, p2.x, p2.y, z);

        }else {
            // bordure gauche
            int id1 = (6*m_col * y) + 2; // idem coin bas gauche
            int id2 = (6*m_col * y) + 4; // idem coin bas gauche
            int id3 = (6*m_col * (y+1)) + 1;
            vec3 p1 = m_positions[id1];
            vec3 p2 = m_positions[id2];
            vec3 p3 = m_positions[id3];
            vertex(id1, p1.x, p1.y, z);
            vertex(id2, p2.x, p2.y, z);
            vertex(id3, p3.x, p3.y, z);

        }
    }else if(x == m_col-1) {
        if(y == -1){
            // coin en haut a droite
            int id1 = 6*(m_col-1);
            int id2 = 6*(m_col-1) + 3;
            vec3 p1 = m_positions[id1];
            vec3 p2 = m_positions[id2];
            vertex(id1, p1.x, p1.y, z);
            vertex(id2, p2.x, p2.y, z);

        }else if(y == m_row-1) {
            // coin en bas a droite
            int id = (6*m_col * m_row) + 1;
            vec3 p = m_positions[id];
            vertex(id, p.x, p.y, z);

        }else {
            // bordure droite
            int id1 = (6*m_col * (y+1)) - 1;
            int id2 = (6*m_col * (y+1)) + (6*(m_col-1));
            int id3 = (6*m_col * (y+1)) + (6*(m_col-1)) + 3;
            vec3 p1 = m_positions[id1];
            vec3 p2 = m_positions[id2];
            vec3 p3 = m_positions[id3];
            vertex(id1, p1.x, p1.y, z);
            vertex(id2, p2.x, p2.y, p2.z);
            vertex(id3, p3.x, p3.y, p3.z);

        }
    }else {
        if(y == -m_squareSize){
            // bordure en haut

        }else if(y == m_squareSize * (m_row-1)) {
            // bordure en bas

        }else {
            // milieu

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
