#include <iostream>
#include <GL/glew.h>
#include <orbiter.h>
#include <CamCalibration.h>
#include <Mire.h>
#include <draw.h>
#include <pthread.h>
#include <Shader.h>
#include "app.h"


static void* cam(void* arg){
    CamCalibration* c = (CamCalibration*) arg;
    c->start();
}

class Framebuffer : public App {
protected:
    Orbiter m_camera;
    Mire m_mire;
    Mesh backGround;
    pthread_t m_threads;
    float camSpeed = 10;
    CamCalibration* m_calibration;
    GLuint tex = -1;
    Shader s;
public:
    // constructeur : donner les dimensions de l'image, et eventuellement la version d'openGL.
    Framebuffer() : App(640, 480), m_mire(4, 7, 35.0, Identity()), backGround(GL_TRIANGLE_STRIP) {}

    void moveCam(){
        int mx, my;
        unsigned int mb= SDL_GetRelativeMouseState(&mx, &my);
        if(mb & SDL_BUTTON(1))              // le bouton gauche est enfonce
            m_camera.rotation(mx, my);
        else if(mb & SDL_BUTTON(3))         // le bouton droit est enfonce
            m_camera.move(mx);
        if(key_state(SDLK_UP))         // le bouton du milieu est enfonce
            m_camera.translation((float) 0 / (float) window_width(), (float) camSpeed / (float) window_height());
        if(key_state(SDLK_DOWN))         // le bouton du milieu est enfonce
            m_camera.translation((float) 0 / (float) window_width(), (float) -camSpeed / (float) window_height());
        if(key_state(SDLK_LEFT))         // le bouton du milieu est enfonce
            m_camera.translation((float) camSpeed / (float) window_width(), (float) 0 / (float) window_height());
        if(key_state(SDLK_RIGHT))         // le bouton du milieu est enfonce
            m_camera.translation((float) -camSpeed / (float) window_width(), (float) 0 / (float) window_height());
    }

    void camInit(){
        m_calibration = new CamCalibration();
        pthread_create(&m_threads, NULL, cam, (void*)m_calibration);

//        m_threads.push_back(std::thread(&Framebuffer::panda, this));
    }

    int init() {

        double x = 10.0f;
        double y = 10.0f;
        camInit();
        s = Shader("data/mesh_color.glsl", 3);
//        Point min(0,0, -1);
//        Point max(7*3.5,4*3.5);
//        m_mire.bounds(min, max);
//        m_camera.lookat(Point(), -10.f);
        glClearColor(0.2, 0.2, 0.2, 1.f);
        glDepthFunc(GL_ALWAYS);
        glEnable(GL_DEPTH_TEST);
        glFrontFace(GL_CCW);
        return 0;   // ras, pas d'erreur
    }

    // destruction des objets de l'application
    int quit() {

        pthread_join(m_threads,NULL);
        return 0;
    }


    GLuint matToTexture(cv::Mat &mat, GLenum minFilter, GLenum magFilter, GLenum wrapFilter)
    {
        // Generate a number for our textureID's unique handle
        GLuint textureID;
        glGenTextures(1, &textureID);

        // Bind to our texture handle
        glBindTexture(GL_TEXTURE_2D, textureID);

        // Catch silly-mistake texture interpolation method for magnification
        if (magFilter == GL_LINEAR_MIPMAP_LINEAR  ||
            magFilter == GL_LINEAR_MIPMAP_NEAREST ||
            magFilter == GL_NEAREST_MIPMAP_LINEAR ||
            magFilter == GL_NEAREST_MIPMAP_NEAREST)
        {
            std::cout << "You can't use MIPMAPs for magnification - setting filter to GL_LINEAR" << std::endl;
            magFilter = GL_LINEAR;
        }

        // Set texture interpolation methods for minification and magnification
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

        // Set texture clamping method
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapFilter);

        // Set incoming texture format to:
        // GL_BGR       for CV_CAP_OPENNI_BGR_IMAGE,
        // GL_LUMINANCE for CV_CAP_OPENNI_DISPARITY_MAP,
        // Work out other mappings as required ( there's a list in comments in main() )
        GLenum inputColourFormat = GL_BGR;
        if (mat.channels() == 1)
        {
            inputColourFormat = GL_LUMINANCE;
        }

        // Create the texture
        glTexImage2D(GL_TEXTURE_2D,     // Type of texture
                     0,                 // Pyramid level (for mip-mapping) - 0 is the top level
                     GL_RGB,            // Internal colour format to convert to
                     mat.cols,          // Image width  i.e. 640 for Kinect in standard mode
                     mat.rows,          // Image height i.e. 480 for Kinect in standard mode
                     0,                 // Border width in pixels (can either be 1 or 0)
                     inputColourFormat, // Input image format (i.e. GL_RGB, GL_RGBA, GL_BGR etc.)
                     GL_UNSIGNED_BYTE,  // Image data type
                     mat.ptr());        // The actual image data itself

        // If we're using mipmaps then generate them. Note: This requires OpenGL 3.0 or higher
        if (minFilter == GL_LINEAR_MIPMAP_LINEAR  ||
            minFilter == GL_LINEAR_MIPMAP_NEAREST ||
            minFilter == GL_NEAREST_MIPMAP_LINEAR ||
            minFilter == GL_NEAREST_MIPMAP_NEAREST)
        {
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        return textureID;
    }


    // dessiner une nouvelle image
    int render() {
        moveCam();

        cv::Mat t = m_calibration->gettVec();
        cv::Mat img = m_calibration->getMat();

        glGenTextures(1, &tex);                  // Create The Texture

        glBindTexture(GL_TEXTURE_2D, tex);
        glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S , GL_REPEAT );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window_width(), window_height(), 0, GL_BGR, GL_UNSIGNED_BYTE, img.data);
        bool flag = m_calibration->getFlag();
        if(flag){
            flag = true;
            cv::Vec3d rot = m_calibration->getRot();
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if(key_state(' '))
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

//        draw(m_mire, m_mire.getTransform(), m_camera);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex);
        glGenerateMipmap(GL_TEXTURE_2D);

        glViewport(0, 0, window_width(), window_height());
        glClearColor(0.2f, 0.2f, 0.2f, 1.f);        // couleur par defaut de la fenetre
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        s.draw(m_calibration->getView(), m_calibration->getProjection(), tex);
        if(flag)
            draw(m_mire, m_calibration->getTransform(), m_calibration->getView(), m_calibration->getProjection());

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glDeleteTextures(1, &tex);
        return 1;
    }

};


int main(int argc, char **argv) {

    Framebuffer tp;
    tp.run();

//    CamCalibration c;
//    c.start();


    return 0;
}