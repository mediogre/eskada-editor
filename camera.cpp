#include "global.h"

irr::scene::ICameraSceneNode* camera;

bool isCameraChanged = false;
int maxZoom, minZoom;

void camSet(int mw, int mh)
{
    if ( !camera)
    {
        camera = smgr->addCameraSceneNode(0, irr::core::vector3df(-mw/2, mh , mh), irr::core::vector3df(-mw/2,0,mh/2));
        camera->setFOV(95);
    }

    maxZoom = 1400;
    minZoom = 5;

    isCameraChanged = true;
}

void camUpdate()
{
    if (isCameraChanged)
    {
        camera->updateAbsolutePosition();
        isCameraChanged = false;
    }
}//camUpdate

void camZoom(float z)
{
    irr::core::vector3df cam_offset = camera->getPosition() - camera->getTarget();

    float test = cam_offset.getLength() - z;

    if(test > maxZoom || test < minZoom) return;

    cam_offset.setLength(test);
    camera->setPosition(camera->getTarget() + cam_offset);

    isCameraChanged = true;
}//camZoom

void camRotate(float a)
{
    irr::core::vector3df cam_offset = camera->getPosition();
    cam_offset.rotateXZBy(a, camera->getTarget());
    camera->setPosition(cam_offset);

    isCameraChanged = true;
}//camRotate

void camShift(int sx, int sy)
{
    irr::core::vector3df
        cam_target = camera->getTarget(),
        cam_position = camera->getPosition(),
        cam_z_dir = cam_position - cam_target,
        cam_x_dir = cam_position - cam_target, tmp;

    //z shift
    cam_z_dir.Y = 0;
    cam_z_dir.normalize();

    cam_position -= cam_z_dir * sy * 0.3;
    cam_target -= cam_z_dir * sy * 0.3;

    //x shift
    cam_x_dir.normalize();
    tmp = cam_x_dir.crossProduct(irr::core::vector3df(0,1,0));
    cam_x_dir = tmp.normalize();

    cam_position -= cam_x_dir * sx * 0.3;
    cam_target -= cam_x_dir * sx * 0.3;

    //setup
    camera->setPosition(cam_position);
    camera->setTarget(cam_target);

       // test1->setText(irr::core::stringw(esk::GUI_ID_BRUSH_GROUND*15/16).c_str());
       // test2->setText(irr::core::stringw(esk::GUI_ID_BRUSH_GROUND).c_str());

    isCameraChanged = true;
}
