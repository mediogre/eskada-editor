#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED

#include <irrlicht.h>
#include <vector.h>

class MyEventReceiver : public irr::IEventReceiver
{
	private:
        bool isMouseLeftDown;
        bool isMouseMiddleDown;
		bool isMouseRightDown;
		bool isMouseMoveWhileRight;
		irr::s32 omx, omy; //страрое положение указателя

	public:
		bool isMouseMove;
        bool isMouseOnGUI;
		irr::s32 mx, my; //положение указателя

		MyEventReceiver();
		virtual bool OnEvent(const irr::SEvent& event);
};

extern irr::IrrlichtDevice *device;
extern irr::video::IVideoDriver* driver;
extern irr::scene::ISceneManager* smgr;
extern irr::gui::IGUIEnvironment* guienv;

//----------------MOUSE------------------

extern irr::core::vector3df mouse_3d;
extern irr::core::vector3df mouse_3dA;


//----------------GUI------------------
extern irr::gui::IGUIStaticText *test1;
extern irr::gui::IGUIStaticText *test2;

extern irr::gui::IGUIToolBar* toolbar;

extern irr::gui::IGUIWindow *win_newmap;
void win_newmap_open();
void win_newmap_apply();
void win_newmap_close();


extern std::vector <irr::gui::IGUIButton*> brushes;
extern int current_brush;

namespace esk
{
    enum
    {
        GUI_START = 1000,

        GUI_ID_WIN_NEWMAP,
            GUI_ID_NEWMAP_64W,
            GUI_ID_NEWMAP_128W,
            GUI_ID_NEWMAP_192W,
            GUI_ID_NEWMAP_256W,
            GUI_ID_NEWMAP_64H,
            GUI_ID_NEWMAP_128H,
            GUI_ID_NEWMAP_192H,
            GUI_ID_NEWMAP_256H,
            GUI_ID_NEWMAP_OK,
            GUI_ID_NEWMAP_CANCEL,

        GUI_ID_QUIT_BUTTON,
        GUI_ID_NEW_MAP,
        GUI_ID_OPEN_MAP,
        GUI_ID_SAVE_MAP,

        GUI_ID_DLG_OPEN_MAP,
        GUI_ID_DLG_SAVE_MAP,

            GUI_ID_BRUSH_DELETE,

            GUI_ID_BRUSH_GROUND,
            //insert new tile
            GUI_ID_BRUSH_SAND,
            GUI_ID_BRUSH_GRASS,
            GUI_ID_BRUSH_WATER,

            GUI_ID_BRUSH_WALL,
            GUI_ID_BRUSH_ROCK,
            //insert new obj
            GUI_ID_BRUSH_TREE,


        GUI_COUNT
    };
}

void buildGUI();
void resetBrushes();
void drawMapCursor(int x, int y);


//--------------------resources-----------------
void loadResourses();
void unloadResourses();

//--------------------camera--------------------
extern irr::scene::ICameraSceneNode* camera;
void camSet(int mw, int mh);
void camShift(int sx, int sy);
void camUpdate();
void camZoom(float z);
void camRotate(float a);


//---------------------объекты карты-------------
extern irr::scene::ITriangleSelector* greed_selector;

extern irr::scene::IMesh* m_greed;
extern irr::video::ITexture* t_greed;
extern irr::scene::IMeshSceneNode* sn_greed;

extern irr::scene::IMesh* m_wall;
extern irr::video::ITexture* t_wall;
extern irr::scene::IMesh* m_rock;
extern irr::video::ITexture* t_rock;
extern irr::scene::IMesh* m_tree;
extern irr::video::ITexture* t_tree;

extern irr::scene::IMesh* m_floor;

extern irr::video::ITexture* t_world_1;

void buildEmptyMap(int w, int h);
void buildMap(const irr::c8* fn);
void saveMap();
void setTile(const irr::core::vector3df p, int b);
void setObjTile(const irr::core::vector3df p, int b);
void clearTiles();


#endif // GLOBAL_H_INCLUDED
