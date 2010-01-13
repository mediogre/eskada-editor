#include "global.h"

irr::gui::IGUIToolBar* toolbar;
irr::gui::IGUIWindow *win_newmap;

irr::gui::IGUIStaticText* test1;
irr::gui::IGUIStaticText* test2;

std::vector <irr::gui::IGUIButton*> brushes;
int current_brush;

void win_newmap_open()
{
    //window newmap
    win_newmap = guienv->addWindow(irr::core::rect<irr::s32>(20, 80, 190, 320), false, L"New map", guienv->getRootGUIElement(), esk::GUI_ID_WIN_NEWMAP);
    guienv->addStaticText(L"Width", irr::core::rect<irr::s32>(20, 30, 60, 50), false, false, win_newmap);
    guienv->addStaticText(L"Height", irr::core::rect<irr::s32>(90, 30, 135, 50), false, false, win_newmap);
    guienv->addCheckBox(true, irr::core::rect<irr::s32>(10, 80, 70, 110), win_newmap, esk::GUI_ID_NEWMAP_64W, L"64");
    guienv->addCheckBox(true, irr::core::rect<irr::s32>(80, 80, 140, 110), win_newmap, esk::GUI_ID_NEWMAP_64H, L"64");
    guienv->addCheckBox(false, irr::core::rect<irr::s32>(10, 110, 70, 140), win_newmap, esk::GUI_ID_NEWMAP_128W, L"128");
    guienv->addCheckBox(false, irr::core::rect<irr::s32>(80, 110, 140, 140), win_newmap, esk::GUI_ID_NEWMAP_128H, L"128");
    guienv->addCheckBox(false, irr::core::rect<irr::s32>(10, 140, 70, 170), win_newmap, esk::GUI_ID_NEWMAP_192W, L"192");
    guienv->addCheckBox(false, irr::core::rect<irr::s32>(80, 140, 140, 170), win_newmap, esk::GUI_ID_NEWMAP_192H, L"192");
    guienv->addCheckBox(false, irr::core::rect<irr::s32>(10, 170, 70, 200), win_newmap, esk::GUI_ID_NEWMAP_256W, L"256");
    guienv->addCheckBox(false, irr::core::rect<irr::s32>(80, 170, 140, 200), win_newmap, esk::GUI_ID_NEWMAP_256H, L"256");
    guienv->addButton(irr::core::rect<irr::s32>(90, 200, 160, 230), win_newmap, esk::GUI_ID_NEWMAP_OK, L"OK");
    guienv->addButton(irr::core::rect<irr::s32>(10, 200, 80, 230), win_newmap, esk::GUI_ID_NEWMAP_CANCEL, L"Cancel");
}

void win_newmap_apply()
{
    int w, h;
    bool s;

    for(int i=esk::GUI_ID_NEWMAP_64W; i<=esk::GUI_ID_NEWMAP_256W; i++)
    {
        s = ((irr::gui::IGUICheckBox*)guienv->getRootGUIElement()->getElementFromId(i,true))->isChecked();
        if (s) { w = (i - esk::GUI_ID_NEWMAP_64W + 1) * 64; break; }
    }

    for(int i=esk::GUI_ID_NEWMAP_64H; i<=esk::GUI_ID_NEWMAP_256H; i++)
    {
        s = ((irr::gui::IGUICheckBox*)guienv->getRootGUIElement()->getElementFromId(i,true))->isChecked();
        if (s) { h = (i - esk::GUI_ID_NEWMAP_64H + 1) * 64; break; }
    }

    buildEmptyMap(w, h);
}

void win_newmap_close()
{
    irr::SEvent e;
    e.EventType = irr::EET_GUI_EVENT;
    e.GUIEvent.Caller = win_newmap;
    e.GUIEvent.Element = 0;
    e.GUIEvent.EventType = irr::gui::EGET_ELEMENT_CLOSED;
    guienv->getRootGUIElement()->OnEvent(e);
    win_newmap->remove();
}


void buildGUI()
{
    guienv->getSkin()->setFont(guienv->getFont("res/font_16.xml"));
    for (irr::u32 i=0; i<irr::gui::EGDC_COUNT ; ++i)
    {
        irr::video::SColor col = guienv->getSkin()->getColor((irr::gui::EGUI_DEFAULT_COLOR)i);
        col.setAlpha(255);
        guienv->getSkin()->setColor((irr::gui::EGUI_DEFAULT_COLOR)i, col);
    }

    //tool bar
    toolbar = guienv->addToolBar();
    toolbar->addButton(esk::GUI_ID_NEW_MAP, L"", L"new map", driver->getTexture("res/gui_btn_new.jpg") );
    toolbar->addButton(esk::GUI_ID_OPEN_MAP, L"", L"open map", driver->getTexture("res/gui_btn_open.jpg") );
    toolbar->addButton(esk::GUI_ID_SAVE_MAP, L"", L"save map", driver->getTexture("res/gui_btn_save.jpg") );

    brushes.push_back(toolbar->addButton(esk::GUI_ID_BRUSH_DELETE, L"", L"brush clear", driver->getTexture("res/gui_btn_brush_delete.jpg"), 0, true ));
    brushes.push_back(toolbar->addButton(esk::GUI_ID_BRUSH_GROUND, L"", L"brush ground", driver->getTexture("res/gui_btn_brush_ground.jpg"), 0, true ));
    brushes.push_back(toolbar->addButton(esk::GUI_ID_BRUSH_SAND, L"", L"brush sand", driver->getTexture("res/gui_btn_brush_sand.jpg"), 0, true ));
    brushes.push_back(toolbar->addButton(esk::GUI_ID_BRUSH_GRASS, L"", L"brush grass", driver->getTexture("res/gui_btn_brush_grass.jpg"), 0, true ));
    brushes.push_back(toolbar->addButton(esk::GUI_ID_BRUSH_WATER, L"", L"brush water", driver->getTexture("res/gui_btn_brush_water.jpg"), 0, true ));
    brushes.push_back(toolbar->addButton(esk::GUI_ID_BRUSH_WALL, L"", L"brush wall", driver->getTexture("res/gui_btn_brush_wall.jpg"), 0, true ));
    brushes.push_back(toolbar->addButton(esk::GUI_ID_BRUSH_ROCK, L"", L"brush rock", driver->getTexture("res/gui_btn_brush_rock.jpg"), 0, true ));
    brushes.push_back(toolbar->addButton(esk::GUI_ID_BRUSH_TREE, L"", L"brush tree", driver->getTexture("res/gui_btn_brush_tree.jpg"), 0, true ));

    current_brush = esk::GUI_ID_BRUSH_DELETE;
    resetBrushes();
    brushes[0]->setPressed(true);

    test1 = guienv->addStaticText(L"test1", irr::core::rect<irr::s32>(35,35,200,50), true);
    test2 = guienv->addStaticText(L"test2", irr::core::rect<irr::s32>(35,55,200,70), true);
    //test1->setText(irr::core::stringw(1).c_str());
}

void resetBrushes()
{
    for(int i = 0, imax = brushes.size(); i<imax; i++)
    {
        brushes[i]->setPressed(false);
    }
}

void drawMapCursor(int x, int y)
{
    irr::core::line3d<irr::f32> ray = smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(irr::core::position2di(x, y), camera);
    irr::core::vector3df intersection;
    irr::core::triangle3df tri;
    const irr::scene::ISceneNode *nod;

    if ( smgr->getSceneCollisionManager()->getCollisionPoint(ray, greed_selector, intersection, tri, nod) )
    {

        irr::core::vector3df
            a(int(intersection.X), 0, int(intersection.Z)),
            b(a.X+1, a.Y, a.Z),
            c(a.X+1, a.Y, a.Z-1),
            d(a.X, a.Y, a.Z-1);

        mouse_3d = a;
    }
}
