#include "global.h"

irr::scene::IMesh* m_greed;
irr::video::ITexture* t_greed;

irr::scene::IMesh* m_wall;
irr::video::ITexture* t_wall;

irr::scene::IMesh* m_rock;
irr::video::ITexture* t_rock;

irr::scene::IMesh* m_tree;
irr::video::ITexture* t_tree;

irr::scene::IMesh* m_floor;

irr::video::ITexture* t_world_1;


void loadResourses()
{

    m_greed = smgr->getMesh("res/greed.3ds");
    t_greed = driver->getTexture("res/floor_greed.jpg");

    m_wall = smgr->getMesh("res/wall.3ds");
    t_wall = driver->getTexture("res/wall.jpg");

    m_rock = smgr->getMesh("res/obj_rock.3ds");
    t_rock = driver->getTexture("res/obj_rock.jpg");

    //m_wall = smgr->getMesh("res/test_home.3ds");
    //t_wall = driver->getTexture("res/test_home.jpg");

    m_tree = smgr->getMesh("res/obj_tree.3ds");
    t_tree = driver->getTexture("res/obj_tree_1.jpg");

    m_floor = smgr->getMesh("res/floor.3ds");

    t_world_1 = driver->getTexture("res/floor_world_1.jpg");

/*
    irr::video::IImage *tmp_img, *img = driver->createImageFromFile("res/floor_sand.jpg");
    for(int i=0; i<16; i++)
    {
        tmp_img = driver->createImage(img, irr::core::position2d<irr::s32>(i*32,0), irr::core::dimension2d<irr::u32>(32,32));
        t_sand16[i] = driver->addTexture("t", tmp_img);
    }
    img->drop();
*/

}

void unloadResourses()
{
    clearTiles();
}

