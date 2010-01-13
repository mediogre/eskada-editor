#include "global.h"

MyEventReceiver::MyEventReceiver()
{
    isMouseLeftDown = false;
    isMouseMiddleDown = false;
    isMouseRightDown = false;
    isMouseMoveWhileRight = false;
    isMouseMove = false;
    isMouseOnGUI = false;

    mx = 0;
    my = 0;
}

bool MyEventReceiver::OnEvent(const irr::SEvent& event)
{
    isMouseMove = false;

    if (event.EventType == irr::EET_KEY_INPUT_EVENT && event.KeyInput.Key == irr::KEY_F12 && event.KeyInput.PressedDown == false)
    {
        char screen_name_buffer[255];
        sprintf(screen_name_buffer, "screenshots/shot_%d.jpg", device->getTimer()->getTime());
        driver->writeImageToFile(driver->createScreenShot() , screen_name_buffer, 0);
        return false;
    }

    //обработчик гуев
    if (event.EventType == irr::EET_GUI_EVENT)
    {
        irr::s32 id = event.GUIEvent.Caller->getID();

        switch(event.GUIEvent.EventType)
		{
		    case irr::gui::EGET_CHECKBOX_CHANGED:
		    {
				switch(id)
				{
                    case esk::GUI_ID_NEWMAP_64W:
                    case esk::GUI_ID_NEWMAP_128W:
                    case esk::GUI_ID_NEWMAP_192W:
                    case esk::GUI_ID_NEWMAP_256W:
                            for(int i=esk::GUI_ID_NEWMAP_64W; i<=esk::GUI_ID_NEWMAP_256W; i++)
                               ((irr::gui::IGUICheckBox*)guienv->getRootGUIElement()->getElementFromId(i,true))->setChecked(false);
                            ((irr::gui::IGUICheckBox*)event.GUIEvent.Caller)->setChecked(true);
                        return true;

                    case esk::GUI_ID_NEWMAP_64H:
                    case esk::GUI_ID_NEWMAP_128H:
                    case esk::GUI_ID_NEWMAP_192H:
                    case esk::GUI_ID_NEWMAP_256H:
                            for(int i=esk::GUI_ID_NEWMAP_64H; i<=esk::GUI_ID_NEWMAP_256H; i++)
                               ((irr::gui::IGUICheckBox*)guienv->getRootGUIElement()->getElementFromId(i,true))->setChecked(false);
                            ((irr::gui::IGUICheckBox*)event.GUIEvent.Caller)->setChecked(true);
                        return true;
				}
		    }

            case irr::gui::EGET_BUTTON_CLICKED:
            {
				switch(id)
				{
                    case esk::GUI_ID_QUIT_BUTTON:
                        device->closeDevice();
                        return true;

                    case esk::GUI_ID_NEW_MAP:
                            win_newmap_open();
                        return true;

                    case esk::GUI_ID_NEWMAP_OK:
                            win_newmap_apply();
                            win_newmap_close();
                        return true;

                    case esk::GUI_ID_NEWMAP_CANCEL:
                            win_newmap_close();
                        return true;

                    case esk::GUI_ID_OPEN_MAP:
                            guienv->addFileOpenDialog(L"Please choose a map file.", true, 0, esk::GUI_ID_DLG_OPEN_MAP);
                        return true;

                    case esk::GUI_ID_SAVE_MAP:
                        saveMap();
                        return true;

                    case esk::GUI_ID_BRUSH_DELETE:
                        resetBrushes();
                        ((irr::gui::IGUIButton*)event.GUIEvent.Caller)->setPressed(true);
                        current_brush = esk::GUI_ID_BRUSH_DELETE;
                        isMouseOnGUI = false;
                        return false;

                    case esk::GUI_ID_BRUSH_GROUND:
                        resetBrushes();
                        ((irr::gui::IGUIButton*)event.GUIEvent.Caller)->setPressed(true);
                        current_brush = esk::GUI_ID_BRUSH_GROUND;
                        isMouseOnGUI = false;
                        return false;

                    case esk::GUI_ID_BRUSH_SAND:
                        resetBrushes();
                        ((irr::gui::IGUIButton*)event.GUIEvent.Caller)->setPressed(true);
                        current_brush = esk::GUI_ID_BRUSH_SAND;
                        isMouseOnGUI = false;
                        return false;

                    case esk::GUI_ID_BRUSH_WATER:
                        resetBrushes();
                        ((irr::gui::IGUIButton*)event.GUIEvent.Caller)->setPressed(true);
                        current_brush = esk::GUI_ID_BRUSH_WATER;
                        isMouseOnGUI = false;
                        return false;

                    case esk::GUI_ID_BRUSH_GRASS:
                        resetBrushes();
                        ((irr::gui::IGUIButton*)event.GUIEvent.Caller)->setPressed(true);
                        current_brush = esk::GUI_ID_BRUSH_GRASS;
                        isMouseOnGUI = false;
                        return false;

                    case esk::GUI_ID_BRUSH_WALL:
                        resetBrushes();
                        ((irr::gui::IGUIButton*)event.GUIEvent.Caller)->setPressed(true);
                        current_brush = esk::GUI_ID_BRUSH_WALL;
                        isMouseOnGUI = false;
                        return false;

                    case esk::GUI_ID_BRUSH_ROCK:
                        resetBrushes();
                        ((irr::gui::IGUIButton*)event.GUIEvent.Caller)->setPressed(true);
                        current_brush = esk::GUI_ID_BRUSH_ROCK;
                        isMouseOnGUI = false;
                        return false;

                    case esk::GUI_ID_BRUSH_TREE:
                        resetBrushes();
                        ((irr::gui::IGUIButton*)event.GUIEvent.Caller)->setPressed(true);
                        current_brush = esk::GUI_ID_BRUSH_TREE;
                        isMouseOnGUI = false;
                        return false;
				}
            }
            break;

            case irr::gui::EGET_ELEMENT_HOVERED:
            {
                return isMouseOnGUI = true;
            }
            break;

            case irr::gui::EGET_ELEMENT_CLOSED:
            case irr::gui::EGET_ELEMENT_LEFT:
            {
                return isMouseOnGUI = false;
            }
            break;

            case irr::gui::EGET_FILE_SELECTED:
            {
                switch(id)
                {
                    case esk::GUI_ID_DLG_OPEN_MAP:
                        irr::gui::IGUIFileOpenDialog* dialog = (irr::gui::IGUIFileOpenDialog*)event.GUIEvent.Caller;
                        //test1->setText(irr::core::stringw(dialog->getFileName()).c_str());
                        buildMap(irr::core::stringc(dialog->getFileName()).c_str());
                        return true;
                }
            }
            break;
		}
        return false;
    }

    if (event.EventType == irr::EET_MOUSE_INPUT_EVENT && !isMouseOnGUI)
    {
        switch(event.MouseInput.Event)
        {
            case irr::EMIE_MOUSE_WHEEL:
                camZoom(event.MouseInput.Wheel);
            break;

            case irr::EMIE_MMOUSE_LEFT_UP:
                isMouseMiddleDown = false;
            break;

            case irr::EMIE_MMOUSE_PRESSED_DOWN:
                isMouseMiddleDown = true;
            break;

            case irr::EMIE_LMOUSE_LEFT_UP:
                isMouseLeftDown = false;
                if ( current_brush >= esk::GUI_ID_BRUSH_GROUND && current_brush <= esk::GUI_ID_BRUSH_WATER)
                    setTile(mouse_3d, current_brush);
                else if ( current_brush >= esk::GUI_ID_BRUSH_WALL && current_brush <= esk::GUI_ID_BRUSH_TREE)
                    setObjTile(mouse_3d, current_brush);
            break;

            case irr::EMIE_LMOUSE_PRESSED_DOWN:
                isMouseLeftDown = true;
            break;

            case irr::EMIE_RMOUSE_PRESSED_DOWN:
                isMouseRightDown = true;
            break;

            case irr::EMIE_RMOUSE_LEFT_UP:
                isMouseRightDown = false;
                isMouseMoveWhileRight = false;
            break;

            case irr::EMIE_MOUSE_MOVED:

                isMouseMove = true;

                if (isMouseRightDown == true)
                {
                    isMouseMoveWhileRight = true;
                    float dir = 0; int angle = 3;
                    if (event.MouseInput.X > omx && event.MouseInput.Y < 300) dir = angle;
                    else if (event.MouseInput.X < omx && event.MouseInput.Y < 300) dir = -angle;
                    else if (event.MouseInput.X > omx && event.MouseInput.Y > 300) dir = -angle;
                    else if (event.MouseInput.X < omx && event.MouseInput.Y > 300) dir = angle;
                    if (dir != 0) camRotate(dir);
                }
                else if (isMouseMiddleDown == true)
                {
                    camShift(event.MouseInput.X-omx, event.MouseInput.Y-omy);
                }
                else if (isMouseLeftDown == true)
                {
                    if ( current_brush == esk::GUI_ID_BRUSH_DELETE)
                    {
                        setObjTile(mouse_3d, current_brush);
                    }
                    else if ( current_brush >= esk::GUI_ID_BRUSH_GROUND && current_brush <= esk::GUI_ID_BRUSH_WATER)
                    {
                        setTile(mouse_3d, current_brush);
                    }
                    else if ( current_brush >= esk::GUI_ID_BRUSH_WALL && current_brush <= esk::GUI_ID_BRUSH_TREE)
                    {
                        setObjTile(mouse_3d, current_brush);
                    }
                }


                mx = omx = event.MouseInput.X;
                my = omy = event.MouseInput.Y;
            break;
        }
        return false;
    }

    return false;
}
