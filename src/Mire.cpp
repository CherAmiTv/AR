//
// Created by julien on 10/01/18.
//

#include "Mire.h"

Mire::Mire(int row, int col, float squareSize, Transform t): Mesh(GL_TRIANGLES) {

    std::vector<vec3> tmp;
    std::vector<int> indices;

    for(int i = 0; i < row; ++i)
        for(int j = 0; j < col; ++j)
            tmp.push_back(vec3(j * squareSize, i * squareSize, 0.0));

    for(int i = 0; i < (col-1); i++){
        int cptY = 0 ;
        int cptZ = 0;
        for(int j = 0; j < (row-1); j++){
            int cptX = 0;
            cptZ++;
            for(int k = 0; k < 2; k++){
                indices.push_back(j * col + cptX + i);
                indices.push_back(col * cptZ + i);
                indices.push_back(col * cptY + i + 1);

                cptX++;
                if(k == 0)
                    cptY++;
            }
        }
    }

    for(int i = 0; i < indices.size(); i++) {
        vertex(tmp[indices[i]]);
        color(1,1,1);
    }



    transform = t;
}
