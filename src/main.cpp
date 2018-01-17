#include <iostream>
#include <GL/glew.h>
#include <orbiter.h>
#include <CamCalibration.h>
#include <Mire.h>
#include <draw.h>
#include <pthread.h>
#include "app.h"


static void* cam(void* arg){
    CamCalibration* c = (CamCalibration*) arg;
    c->calibrate();
}

class Framebuffer : public App {
protected:
    Orbiter m_camera;
    Mire m_mire;
    pthread_t m_threads;
    float camSpeed = 10;
    CamCalibration* m_calibration;
public:
    // constructeur : donner les dimensions de l'image, et eventuellement la version d'openGL.
    Framebuffer() : App(1024, 640), m_mire(6, 9, 1, Identity()) {}

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

        Point min, max;
        m_mire.bounds(min, max);
        m_camera.lookat(min, max);

        glClearColor(0.2, 0.2, 0.2, 1.f);
        glDepthFunc(GL_LESS);
        glEnable(GL_DEPTH_TEST);
        glFrontFace(GL_CW);
        return 0;   // ras, pas d'erreur
    }

    // destruction des objets de l'application
    int quit() {

        pthread_join(m_threads,NULL);
        return 0;
    }

    // dessiner une nouvelle image
    int render() {
        moveCam();

        cv::Mat tmp = m_calibration->getIntrinsicParametersMatrix();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if(key_state(' '))
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


        draw(m_mire, m_mire.getTransform(), m_camera);
        return 1;
    }

};


int main(int argc, char **argv) {

    Framebuffer tp;
    tp.run();

    return 0;
}