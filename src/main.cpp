#include <iostream>
#include <GL/glew.h>
#include <orbiter.h>
#include <CamCalibration.h>
#include <Mire.h>
#include <draw.h>
#include <pthread.h>
#include "app.h"
#include <fstream>


static void* cam(void* arg){
    CamCalibration* c = (CamCalibration*) arg;
    // Verifie si le fichier de calibration existe
    // Sinon en genere un nouveau
    std::ifstream calibrationFile(calibrationFileName);
    if(calibrationFile.good()) {
        calibrationFile.close();
        c->start();
    }else {
        c->start(calibrationFileName, true);
    }
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
    Framebuffer() : App(640, 480), m_mire(4, 7, 3.5, Identity()) {}

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

        Point min(0,0, -1);
        Point max(7*3.5,4*3.5, -1);
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

        cv::Mat t = m_calibration->gettVec();

        if(!t.empty()){

            cv::Vec3d rot = m_calibration->getRot();

            Vector transform = Vector(t.at<double>(0), -t.at<double>(1), -t.at<double>(2));

            Transform t2 = Translation(transform) * RotationX(rot[0]) * RotationY(-rot[1]) * RotationZ(-rot[2]);

            m_mire.setTransform(t2);

            Transform t3 = m_calibration->getTransform();
            std::cout << transform << std::endl;
            std::cout << rot[0] << " " << rot[1] << " " << rot[2] << std::endl;
            std::cout << t2 << std::endl;

        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if(key_state(' '))
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        draw(m_mire, m_calibration->getTransform(), m_camera.view(), m_calibration->getProjection());

        return 1;
    }

};


int main(int argc, char **argv) {

    Framebuffer tp;
    tp.run();

    return 0;
}