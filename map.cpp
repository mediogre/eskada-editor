#include "global.h"

int map_width, map_height;
irr::scene::IMeshSceneNode* sn_greed;
irr::scene::IMeshSceneNode* map_node;

//текстурные координаты для тайлов в ширину
const float tex_x_l[16] = {0.0000f, 0.064450, 0.126950, 0.189450, 0.251950, 0.314450, 0.376950, 0.439450, 0.501950, 0.564450, 0.626950, 0.689450, 0.751950, 0.814450, 0.876950, 0.939450};
const float tex_x_r[16] = {0.06250, 0.125000, 0.187500, 0.250000, 0.312500, 0.375000, 0.437500, 0.500000, 0.562500, 0.625000, 0.687500, 0.750000, 0.812500, 0.875000, 0.937500, 1.00000f};

//текстурные координаты для тайлов в высоту
const float tex_y_u[4]  = {0.000f, 0.25800, 0.5080, 0.7580};
const float tex_y_d[4]  = {0.2500, 0.50000, 0.7500, 1.000f};

struct tile
{
    int texIdx;
};
std::vector< std::vector<tile *> >tiles;

struct objTile
{
    irr::scene::IMeshSceneNode* n;
    int brush;
};
std::vector< std::vector<objTile *> > objtiles;

irr::core::vector3df mouse_3d;

irr::scene::ITriangleSelector* greed_selector = 0;

const int tailBaseTable[25][16] =
{
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},//0
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},//1
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},//2
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},//3
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},//4
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},//5
    { 9, 1, 6, 8, 4, 5, 6,15, 8, 9, 1,14, 4, 5,14,16},//6
    { 1, 1, 6,15, 5, 5, 6,15,15, 1, 1, 6, 5, 5, 6,16},//7
    {10, 1, 2, 7, 5, 5, 6, 7,15, 1,10, 2, 13,13, 6,16},//8
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},//9
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},//10
    { 4, 5,15, 8, 4, 5,15,15, 8, 4, 5, 8, 4, 5, 8,16},//11
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},//12
    { 2, 6, 2, 7,15,15, 6, 7,15, 6, 2, 2, 7, 7, 6,16},//13
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},//15---
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},//16
    {12, 5, 7, 3, 4, 5,15, 7, 8, 4,13, 3,12,13, 8,16},//17
    { 3,15, 7, 3, 8,15,15, 7, 8, 8, 7, 3, 3, 7, 8,16},//18
    {11, 6, 2, 3, 8,15, 6, 7, 8,14, 2,11, 3, 7,14,16},//19
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},//20
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},//21
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},//22
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},//23
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},//24
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16} //25
};

const int tailEqualTable[25][16] =
{
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},
    {16,10,16,16,12,13, 2,16, 3, 0,16,16,16,16,11, 7},
    {16, 0,11,16,12,12,11, 3, 3, 0, 0,16,16,12,11, 3},
    {16, 9,11,16,16, 4,14, 3,16,16, 0,16,16,12,16, 8},
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},
    {16,10,16,11, 0,10, 2, 2,11, 0,16,16, 0,10,11, 2},
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},
    {16, 9, 0,12,16, 4, 9,12, 4,16, 0, 0,16,12, 9, 4},
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},
    {16,16,16,11, 9, 1,16, 2,14,16,16,16, 0,10,16, 6},
    {16,16,10, 0, 9, 1, 1,10, 9,16,16, 0, 0,10, 9, 1},
    {16,16,10,12,16,16, 1,13, 4,16,16, 0,16,16, 9, 5},
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16}
};

const int tailDiffTable[25][16] =
{
    { 9, 1, 6, 8, 4, 5, 6,15, 8, 9, 1,14, 4, 5,14,15},
    { 1, 1, 6,15, 5, 5, 6,15,15, 1, 1, 6, 5, 5, 6,15},
    { 1, 1, 6,15, 5, 5, 6,15,15, 1, 1, 6, 5, 5, 6,15},
    { 1, 1, 6,15, 5, 5, 6,15,15, 1, 1, 6, 5, 5, 6,15},
    {10, 1, 2, 7, 5, 5, 6, 7,15, 1,10, 2,13,13, 6,15},
    { 4, 5,15, 8, 4, 5,15,15, 8, 4, 5, 8, 4, 5, 8,15},
    {23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23},
    {19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19},
    {24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24},
    { 2, 6, 2, 7,15,15, 6, 7,15, 6, 2, 2, 7, 7, 6,15},
    { 4, 5,15, 8, 4, 5,15,15, 8, 4, 5, 8, 4, 5, 8,15},
    {18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18},
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},
    {20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20},
    { 2, 6, 2, 7,15,15, 6, 7,15, 6, 2, 2, 7, 7, 6,15},
    { 4, 5,15, 8, 4, 5,15,15, 8, 4, 5, 8, 4, 5, 8,15},
    {22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22},
    {17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17},
    {21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21},
    { 2, 6, 2, 7,15,15, 6, 7,15, 6, 2, 2, 7, 7, 6,15},
    {12, 5, 7, 3, 4, 5,15, 7, 8, 4,15,13,12,13, 8,15},
    { 3,15, 7, 3, 8,15,15, 7, 8, 8, 7, 3, 3, 7, 8,15},
    { 3,15, 7, 3, 8,15,15, 7, 8, 8, 7, 3, 3, 7, 8,15},
    { 3,15, 7, 3, 8,15,15, 7, 8, 8, 7, 3, 3, 7, 8,15},
    {11, 6, 2, 3,15,15, 6, 7, 8,14, 2,11, 3, 7,14,15}
};

int tileTmpTable[5][5];

int getTileBrush(int idx)
{
    return int(idx/16);
}

int getTileTex(int idx)
{
    int r = idx % 16;
    return r;
}

void clearTile(int x, int y)
{
   if ( tiles[y][x] )
   {
       delete tiles[y][x];
       tiles[y][x] = 0;
   }
}

void clearObjTile(int x, int y)
{
   if ( objtiles[y][x] )
   {
       objtiles[y][x]->n->remove();
       delete objtiles[y][x];
       objtiles[y][x] = 0;
   }
}


void clearTiles()
{
    for(int y=0, ysize=tiles.size(); y<ysize; y++)
    {
        for(int x=0, xsize=tiles[y].size(); x<xsize; x++)
        {
            clearTile(x,y);
            clearObjTile(x,y);
        }
    }
}

void setTileTex(int x, int z, int texType, int texNum)
{
    irr::scene::IMesh *mesh = map_node->getMesh();

    //расчитаем в каком буфере наш тайл и получим вершины этого буффера
    int buf_num = (map_width/64) * (z/64) + (x/64);
    irr::video::S3DVertex* vbuf = (irr::video::S3DVertex*)mesh->getMeshBuffer(buf_num)->getVertices();

    //найдем смещение первой вершины образующие тайл
    int base_x = x - (x/64)*64;
    int base_z = z - (z/64)*64;
    int vert_idx = base_z * 64 * 4 + base_x * 4;

    //определяем новые координаты текстуру для этого полигона
    vbuf[vert_idx  ].TCoords = irr::core::vector2df(tex_x_l[texNum], tex_y_u[texType]); //0,0
    vbuf[vert_idx+1].TCoords = irr::core::vector2df(tex_x_r[texNum], tex_y_u[texType]); //1,0
    vbuf[vert_idx+2].TCoords = irr::core::vector2df(tex_x_r[texNum], tex_y_d[texType]); //1,1
    vbuf[vert_idx+3].TCoords = irr::core::vector2df(tex_x_l[texNum], tex_y_d[texType]); //0,1

    mesh->getMeshBuffer(buf_num)->setDirty();
}

void createTile(int x, int z)
{
    tiles[z][x] = new tile;
    tiles[z][x]->texIdx = esk::GUI_ID_BRUSH_GROUND * 16 + 15;
    setTileTex(x, z, 0, 15); //земля - последняя текстура линейки воды
}

void dimTiles(int w, int h)
{
    clearTiles();
    tiles.resize(h);
    objtiles.resize(h);

    for(int i=0; i<h; i++)
    {
        tiles[i].resize(w);
        objtiles[i].resize(w);
    }

    for(int z=0; z<h; z++)
        for(int x=0; x<w; x++)
            createTile(x, z);
}

void buildEmptyMap(int w, int h)
{
    map_width = w;
    map_height = h;

    //т.к. одна область у нас занимает поле 64х64 тайла
    //вычисляем сколько мешбуферов нам понадобится
    int buf_cnt_x = w/64;
    int buf_cnt_z = h/64;


    int v_cnt = 64*64*4; //число вершин в полигонах одного буффера 64*64*4
    int i_cnt = 64*64*6; //число индексов одного буффера 64*64*6
    int p_cnt = 0; //счетчик полигонов

    //создаем меш под буфера и указатель на текущий генерируемый буффер
    irr::scene::SMesh *dmes = new irr::scene::SMesh;
    irr::scene::SMeshBuffer *buffer;

    //цвет для вершин по умолчанию
    irr::video::SColor poly_col(255,255,255,255);

    //последовательно генерим каждый буфер
    for(int bcz=0; bcz<buf_cnt_z; bcz++)
    for(int bcx=0; bcx<buf_cnt_x; bcx++)
    {
        buffer = new irr::scene::SMeshBuffer;

        //вершины
        p_cnt = 0; //номер первой вершины полигона из buffer->Vertices
        buffer->Vertices.set_used(v_cnt);
        for(int z=bcz*64, bcz_max=bcz*64+64; z<bcz_max; z++)
            for(int x=bcx*64, bcx_max=bcx*64+64; x<bcx_max; x++) //за один шаг цикла описываем 4 вершины образующих полигон
            {
                buffer->Vertices[p_cnt]  = irr::video::S3DVertex(    -x,0,  z,   0,1,0, poly_col, 0, 0);//00
                buffer->Vertices[p_cnt+1]  = irr::video::S3DVertex(-x-1,0,  z,   0,1,0, poly_col, 1, 0);//10
                buffer->Vertices[p_cnt+2]  = irr::video::S3DVertex(-x-1,0,z+1,   0,1,0, poly_col, 1, 1);//11
                buffer->Vertices[p_cnt+3]  = irr::video::S3DVertex(  -x,0,z+1,   0,1,0, poly_col, 0, 1);//01
                p_cnt += 4;
            }

        //порядок обхода
        p_cnt = 0; //номер первой вершины полигона из buffer->Vertices
        buffer->Indices.set_used(i_cnt);
        for(int i=0; i<i_cnt; i+=6) //за один шаг цикла описываем 2 треугольника из которых состоит полигон
        {
            buffer->Indices[i] = p_cnt;
            buffer->Indices[i+1] = p_cnt+1;
            buffer->Indices[i+2] = p_cnt+2;
            buffer->Indices[i+3] = p_cnt;
            buffer->Indices[i+4] = p_cnt+2;
            buffer->Indices[i+5] = p_cnt+3;
            p_cnt += 4;
        }

        buffer->recalculateBoundingBox();
        dmes->addMeshBuffer(buffer);
        buffer->drop();
    }
    dmes->setHardwareMappingHint(irr::scene::EHM_STATIC);

    //создаем ноду на основе сгенереного меша
    if (map_node) map_node->remove();
    map_node = smgr->addMeshSceneNode(dmes);
    dmes->drop();
    map_node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
    map_node->setMaterialFlag(irr::video::EMF_BILINEAR_FILTER, false);

    map_node->setMaterialTexture(0, t_world_1);

    //создаем "подложку" для определения коллизий
    if (sn_greed) sn_greed->remove();
    sn_greed = smgr->addMeshSceneNode( m_greed, 0, -1, irr::core::vector3df(0, -0.009, 0) );
    sn_greed->setMaterialFlag( irr::video::EMF_LIGHTING, true );
    sn_greed->setMaterialTexture( 0, t_greed );
    sn_greed->setScale(irr::core::vector3df(map_width, 0, map_height));
    greed_selector = smgr->createTriangleSelector(m_greed, sn_greed);
    sn_greed->setTriangleSelector(greed_selector);

    dimTiles(map_width, map_height);
    camSet(map_width, map_height);
}//buildEmptyMap

void buildMap(const irr::c8* fn)
{
    /*
    smgr->clear();
    clearTiles();
    tiles.resize(64*64);
    objtiles.resize(64*64);

    irr::core::vector3df tmp_vec;
    irr::scene::IMeshSceneNode * tmp_msn;

    tmp_msn = smgr->addMeshSceneNode( m_greed, 0, -1, irr::core::vector3df(0, 0.1, 0) );
    tmp_msn->setMaterialFlag( irr::video::EMF_LIGHTING, false );
    tmp_msn->setMaterialTexture( 0, driver->getTexture("res/floor_greed.jpg") );

    greed_selector = smgr->createTriangleSelector(m_greed, tmp_msn);
    tmp_msn->setTriangleSelector(greed_selector);

    irr::io::IXMLReader* reader =  device->getFileSystem()->createXMLReader( "res/map_1.xml" )  ;
    if ( !reader ) return;

    while(reader && reader->read())
    {
        switch(reader->getNodeType())
        {
            case irr::io::EXN_ELEMENT:
                if ( irr::core::stringw(reader->getNodeName()) == irr::core::stringw("tile") )
                {
                    tmp_vec = irr::core::vector3df(
                            reader->getAttributeValueAsFloat(L"x"),
                            0,
                            reader->getAttributeValueAsFloat(L"z")
                        );
                    int tmp_brush = reader->getAttributeValueAsInt(L"brush");
                    setTile(tmp_vec, tmp_brush);
                }
                else if ( irr::core::stringw(reader->getNodeName()) == irr::core::stringw("obj") )
                {
                    tmp_vec = irr::core::vector3df(
                            reader->getAttributeValueAsFloat(L"x"),
                            0,
                            reader->getAttributeValueAsFloat(L"z")
                        );
                    int tmp_brush = reader->getAttributeValueAsInt(L"brush");
                    setObjTile(64, tmp_vec, tmp_brush);
                }
            break;
        }
    }
    reader->drop();
    reader = 0;

    camera = smgr->addCameraSceneNode(0, irr::core::vector3df(0,40,-10), irr::core::vector3df(0,0,0));
    camSet();
    */
}//buildMap

void saveMap()
{
/*
    irr::io::IXMLWriter* writer =  device->getFileSystem()->createXMLWriter( "res/map_1.xml" )  ;
    if ( !writer ) return;

   writer->writeXMLHeader();

   // write root node
   writer->writeElement( L"root",false);
   writer->writeLineBreak();


    irr::core::vector3df tmp;

    for(int i = 0, imax = tiles.size(); i<imax; i++)
    {
        if (tiles[i])
        {
            tmp = tiles[i]->n->getPosition();
            writer->writeElement(
                L"tile", true,
                L"x", irr::core::stringw(tmp.X).c_str(),
                L"z", irr::core::stringw(tmp.Z).c_str(),
                L"brush", irr::core::stringw(tiles[i]->brush).c_str(),
                L"speed", L"1"
            );
            writer->writeLineBreak();
        }
    }

    for(int i = 0, imax = objtiles.size(); i<imax; i++)
    {
        if (objtiles[i])
        {
            tmp = objtiles[i]->n->getPosition();
            writer->writeElement(
                L"obj", true,
                L"x", irr::core::stringw(tmp.X).c_str(),
                L"z", irr::core::stringw(tmp.Z).c_str(),
                L"brush", irr::core::stringw(objtiles[i]->brush).c_str(),
                L"speed", L"0"
            );
            writer->writeLineBreak();
        }
    }


   // close root node
   writer->writeClosingTag(L"root");
   writer->writeLineBreak();

   // dispose writer
   writer->drop();
   writer = 0;
*/
}//saveMap


void fillTileTmpTable(int x, int z)
{
    int sx = 0, sz = 0;

    for(int cz=0;cz<5;cz++)
    {
        sz = z-2+cz;
        for(int cx=0;cx<5;cx++)
        {
            sx = x+2-cx;
            if ( sx < 1 && sx > -map_width && sz > -1 && sz < map_height )
            {
                tileTmpTable[cz][cx] = tiles[sz][-sx]->texIdx;
            }
            else
            {
                tileTmpTable[cz][cx] = 0;
            }
        }
    }
}//fillTileTmpTable

void prepareTileTmpTable(int b)
{
    int b_idx = 0, t_idx = 0, cnt = 0;

    if ( b == esk::GUI_ID_BRUSH_GROUND )
    {
        tileTmpTable[2][2] = b*16+15;
        for (int z=0; z<5; z++)
            for (int x=0; x<5; x++)
            {
                if ( tileTmpTable[z][x] > 0 )
                {
                    b_idx = getTileBrush( tileTmpTable[z][x] );
                    t_idx = getTileTex( tileTmpTable[z][x] );
                    if ( tailBaseTable[cnt][t_idx] != 16 )
                        tileTmpTable[z][x] = b_idx*16 + tailBaseTable[cnt][t_idx];
                }
                cnt++;
            }
    }
    else
    {
        tileTmpTable[2][2] = b * 16;
        for (int z=0; z<5; z++)
            for (int x=0; x<5; x++)
            {
                if ( tileTmpTable[z][x] > 0 )
                {
                    b_idx = getTileBrush( tileTmpTable[z][x] );
                    t_idx = getTileTex( tileTmpTable[z][x] );

                    if ( b_idx == b) // Если прежняя земля имеет тот же тип, что и активная
                    {
                        if ( tailEqualTable[cnt][t_idx] != 16 ) tileTmpTable[z][x] = b_idx*16 + tailEqualTable[cnt][t_idx];
                    }
                    else // Если заменяемая и замещающая земля имеют разные типы
                    {
                        // Если число матрици = 16, то ничего не делаем
                        // Если < 16, то рисуем заменяемой землей
                        // Если > 16, то рисуем замещающей (с хитрым индексом секрет которого не разгадан мной)

                        if (tailDiffTable[cnt][t_idx] < 16)
                        {
                            tileTmpTable[z][x] = b_idx*16 + tailDiffTable[cnt][t_idx];
                        }
                        else if (tailDiffTable[cnt][t_idx] > 16)
                        {
                            tileTmpTable[z][x] = b*16 + tailDiffTable[cnt][t_idx]-16;
                        }
                    }
                }
                cnt++;
            }
    }
}//prepareTileTmpTable

void placeTileTmpTable(int x, int z)
{
    int sx=0, sz=0, b_idx = 0, t_idx = 0;

    for(int cz=0;cz<5;cz++)
    {
        sz = z-2+cz;
        for(int cx=0;cx<5;cx++)
        {
            sx = x+2-cx;
            if ( sx < 1 && sx > -map_width && sz > -1 && sz < map_height )
            {
                tiles[sz][-sx]->texIdx = tileTmpTable[cz][cx];
                b_idx = getTileBrush( tileTmpTable[cz][cx] );
                t_idx = getTileTex( tileTmpTable[cz][cx] );

                if ( b_idx == esk::GUI_ID_BRUSH_GROUND )
                    setTileTex(-sx, sz, 0, 15);
                else if ( b_idx == esk::GUI_ID_BRUSH_WATER )
                    setTileTex(-sx, sz, 0, t_idx);
                else if ( b_idx == esk::GUI_ID_BRUSH_SAND )
                    setTileTex(-sx, sz, 1, t_idx);
                else if ( b_idx == esk::GUI_ID_BRUSH_GRASS )
                    setTileTex(-sx, sz, 2, t_idx);
            }
        }
    }
}//placeTileTmpTable

void setTile(const irr::core::vector3df p, int b)
{
    fillTileTmpTable(p.X, p.Z);
    prepareTileTmpTable(b);
    placeTileTmpTable(p.X, p.Z);

    if ( b == esk::GUI_ID_BRUSH_WATER )
    {
        clearObjTile(-p.X, p.Z);
    }
}//setTile

void setObjTile(const irr::core::vector3df p, int b)
{
    int x = -p.X, z = p.Z;

    clearObjTile(x, z);

    if (b == esk::GUI_ID_BRUSH_DELETE) return;

    if (b == esk::GUI_ID_BRUSH_WALL)
    {
        objtiles[z][x] = new objTile;
        objtiles[z][x]->n = smgr->addMeshSceneNode( m_wall, 0, -1, p );
        objtiles[z][x]->n->setMaterialFlag( irr::video::EMF_LIGHTING, false );
        objtiles[z][x]->n->setMaterialTexture(0, t_wall);
        setTile(p, esk::GUI_ID_BRUSH_SAND);
    }
    else if (b == esk::GUI_ID_BRUSH_ROCK)
    {
        objtiles[z][x] = new objTile;
        objtiles[z][x]->n = smgr->addMeshSceneNode( m_rock, 0, -1, p );
        objtiles[z][x]->n->setMaterialFlag( irr::video::EMF_LIGHTING, false );
        objtiles[z][x]->n->setMaterialTexture(0, t_rock);
        setTile(p, esk::GUI_ID_BRUSH_SAND);

    }
    else if (b == esk::GUI_ID_BRUSH_TREE)
    {
        objtiles[z][x] = new objTile;
        objtiles[z][x]->n = smgr->addMeshSceneNode( m_tree, 0, -1, p );
        objtiles[z][x]->n->setMaterialFlag( irr::video::EMF_LIGHTING, false );
        objtiles[z][x]->n->setMaterialTexture(0, t_tree);
        setTile(p, esk::GUI_ID_BRUSH_GRASS);
    }
}//setObjTile



