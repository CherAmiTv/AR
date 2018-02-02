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

//void Mire::setHeight(int x, int y, float z) {
//    //TODO
//    int cpt = x + y * m_col + 2;
//    m_positions[cpt].z += z;
//    vec3 tmp = vec3(m_positions[cpt].x, m_positions[cpt].y, m_positions[cpt].z);
//
//    vertex(cpt, tmp);

//    float X = (x-1) * (m_squareSize);
//    float Y = (y-1) * (m_squareSize);
//    int cpt = 0;
//    for(float h = -m_squareSize;h < m_row;h += m_squareSize) {
//        for(float w = -m_squareSize;w < m_col;w += m_squareSize) {
//
//            // triangle 1 (left bottom)
//            Point a(w + m_squareSize, h, z);
//            Point b(w, h, z);
//            Point c(w, h + m_squareSize, z);
//
//            // triangle 2 (right up)
//            Point d = a;
//            Point e = c;
//            Point f(w + m_squareSize, h + m_squareSize, z);
//
//
////            std::cout << m_positions[0].z << std::endl;
//
//            if(a.x == X && a.y == Y) {
////                std::cout << cpt << std::endl;
//                m_positions[cpt].z += z;
//                vec3 tmp = vec3(m_positions[cpt].x, m_positions[cpt].y, m_positions[cpt].z);
//                vertex(cpt, tmp);
//            }
//            if(b.x == X && b.y == Y){
//                std::cout << cpt << std::endl;
//
//                m_positions[cpt+1].z += z;
//                vec3 tmp = vec3(m_positions[cpt+1].x, m_positions[cpt+1].y, m_positions[cpt+1].z);
//                vertex(cpt+1, tmp);
//            }
//            if(c.x == X && c.y == Y){
//                std::cout << cpt << std::endl;
//
//                m_positions[cpt+2].z += z;
//                vec3 tmp = vec3(m_positions[cpt+2].x, m_positions[cpt+2].y, m_positions[cpt+2].z);
//                vertex(cpt+2, tmp);
//
//            }
//            if(d.x == X && d.y == Y){
//                std::cout << cpt << std::endl;
//
//                m_positions[cpt+3].z += z;
//                vec3 tmp = vec3(m_positions[cpt+3].x, m_positions[cpt+3].y, m_positions[cpt+3].z);
//                vertex(cpt+3, tmp);
//            }
//            if(e.x == X && e.y == Y){
//                std::cout << cpt << std::endl;
//
//                m_positions[cpt+4].z += z;
//                vec3 tmp = vec3(m_positions[cpt+4].x, m_positions[cpt+4].y, m_positions[cpt+4].z);
//                vertex(cpt+4, tmp);
//            }
//            if(f.x == X && f.y == Y){
//                std::cout << cpt << std::endl;
//
//                m_positions[cpt+5].z += z;
//                vec3 tmp = vec3(m_positions[cpt+5].x, m_positions[cpt+5].y, m_positions[cpt+5].z);
//                vertex(cpt+5, tmp);
//            }
//
//            cpt += 6;
//        }
//    }
//}

//Ne fonctionne pas
void Mire::setHeight(int x, int y, float z) {
    std::cout << x << " " << y << std::endl;
    // (0,0), origine en haut a gauche
    if(x == 0) {
        if(y == 0){
            // coin en haut a gauche
            int id = 1;
            vec3 p = m_positions[id];
            vertex(id, p.x, p.y, z);

        }else if(y == m_row) {
            // coin en bas a gauche
            int id1 = (6*m_col * (m_row-1)) + 2;
            int id2 = (6*m_col * (m_row-1)) + 4;
            vec3 p1 = m_positions[id1];
            vec3 p2 = m_positions[id2];
            vertex(id1, p1.x, p1.y, z);
            vertex(id2, p2.x, p2.y, z);

        }else {
            // bordure gauche
            int id1 = (6*m_col * (y-1)) + 2; // idem coin bas gauche
            int id2 = (6*m_col * (y-1)) + 4; // idem coin bas gauche
            int id3 = (6*m_col * y) + 1;
            vec3 p1 = m_positions[id1];
            vec3 p2 = m_positions[id2];
            vec3 p3 = m_positions[id3];
            vertex(id1, p1.x, p1.y, z);
            vertex(id2, p2.x, p2.y, z);
            vertex(id3, p3.x, p3.y, z);

        }
    }else if(x == m_col) {
        if(y == 0){
            // coin en haut a droite
            int id1 = 6*(m_col-1);
            int id2 = 6*(m_col-1) + 3;
            vec3 p1 = m_positions[id1];
            vec3 p2 = m_positions[id2];
            vertex(id1, p1.x, p1.y, z);
            vertex(id2, p2.x, p2.y, z);

        }else if(y == m_row) {
            // coin en bas a droite
            int id = (6*m_col * m_row) - 1;
            vec3 p = m_positions[id];
            vertex(id, p.x, p.y, z);

        }else {
            // bordure droite
            int id1 = (6*m_col * y) - 1;
            int id2 = (6*m_col * y) + (6*(m_col-1));
            int id3 = (6*m_col * y) + (6*(m_col-1)) + 3;
            vec3 p1 = m_positions[id1];
            vec3 p2 = m_positions[id2];
            vec3 p3 = m_positions[id3];
            vertex(id1, p1.x, p1.y, z);
            vertex(id2, p2.x, p2.y, z);
            vertex(id3, p3.x, p3.y, z);

        }
    }else {
        if(y == 0){
            // bordure en haut
            int id1 = 6*(x-1);
            int id2 = 6*(x-1) + 2;
            int id3 = 6*(x-1) + 7;
            vec3 p1 = m_positions[id1];
            vec3 p2 = m_positions[id2];
            vec3 p3 = m_positions[id3];
            vertex(id1, p1.x, p1.y, z);
            vertex(id2, p2.x, p2.y, z);
            vertex(id3, p3.x, p3.y, z);

        }else if(y == m_row) {
            // bordure en bas
            int id1 = 6*m_col * (m_row-1) + 6*x - 1;
            int id2 = 6*m_col * (m_row-1) + 6*x + 2;
            int id3 = 6*m_col * (m_row-1) + 6*x + 4;
            vec3 p1 = m_positions[id1];
            vec3 p2 = m_positions[id2];
            vec3 p3 = m_positions[id3];
            vertex(id1, p1.x, p1.y, z);
            vertex(id2, p2.x, p2.y, z);
            vertex(id3, p3.x, p3.y, z);

        }else {
            // milieu
            int id1 = 6*m_col*(y-1) + 6*x - 1;
            int id2 = 6*m_col*(y-1) + 6*x + 2;
            int id3 = 6*m_col*(y-1) + 6*x + 4;
            int id4 = 6*m_col*y + 6*(x-1);
            int id5 = 6*m_col*y + 6*(x-1) + 3;
            int id6 = 6*m_col*y + 6*(x-1) + 7;
            vec3 p1 = m_positions[id1];
            vec3 p2 = m_positions[id2];
            vec3 p3 = m_positions[id3];
            vec3 p4 = m_positions[id4];
            vec3 p5 = m_positions[id5];
            vec3 p6 = m_positions[id6];
            vertex(id1, p1.x, p1.y, z);
            vertex(id2, p2.x, p2.y, z);
            vertex(id3, p3.x, p3.y, z);
            vertex(id4, p4.x, p4.y, z);
            vertex(id5, p5.x, p5.y, z);
            vertex(id6, p6.x, p6.y, z);

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
