#include "global.h"

irr::IrrlichtDevice* device;
irr::video::IVideoDriver* driver;
irr::scene::ISceneManager* smgr;
irr::gui::IGUIEnvironment* guienv;

int main(int argc, char** argv)
{
    MyEventReceiver receiver;
    device = irr::createDevice( irr::video::EDT_OPENGL, irr::core::dimension2d<irr::u32>(800, 600), 32, false, false, false, &receiver);
    device->setWindowCaption(L"Eskada");
    if (!device) return 1;

    driver = device->getVideoDriver();
    smgr = device->getSceneManager();
    guienv = device->getGUIEnvironment();

    smgr->addCameraSceneNode(0, irr::core::vector3df(0,30,-40), irr::core::vector3df(0,5,0));
    loadResourses();
    buildGUI();
    buildEmptyMap(64, 64);

    int lastFPS = -1, framelimit = 100;
    irr::s32 now = 0;
	irr::u32 sceneStartTime = 0;
	irr::u32 sceneSkipTime = 1000 / framelimit;

    while(device->run())
    {
        now = device->getTimer()->getTime();

        if (now - sceneStartTime > sceneSkipTime)
	{
            driver->beginScene(true, true, irr::video::SColor(0,100,100,100));
            camUpdate();
            smgr->drawAll();

            if ( receiver.isMouseMove && !receiver.isMouseOnGUI )
            {
                //отрисовка курсора над активным тайлом
                drawMapCursor(receiver.mx, receiver.my);
            }
            guienv->drawAll();

            driver->endScene();
	}

	int fps = driver->getFPS();
	if (lastFPS != fps)
        {
            test1->setText(irr::core::stringw(fps).c_str());
            lastFPS = fps;
        }

	device->yield();
    }

    unloadResourses();

    device->drop();

    return 0;
}

