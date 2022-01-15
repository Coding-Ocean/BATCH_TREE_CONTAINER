#define _3D_MODEL
//#define _GORILLA_CAMERA
//#define _BATCH
//#define _TREE
//#define _CONTAINER

/*
シェーダとは
 ＧＰＵ（グラフィックボードのＣＰＵ）に対する命令書のこと。
 主に「座標１点の変換」と「ピクセル１点の描画」がコーディングされている。
 (以下のプログラムはシェーダに必要な値を設定しているだけで、
 シェーダプログラムではない！)
*/

#ifdef _3D_MODEL
#include"libOne.h"
#include"SHADER/LAMBERT.h"
#include"CONTAINER/CONTAINER.h"
#include"CONTAINER/BATCH.h"
void gmain()
{
    window(1920, 1080, full);
    //window(1280, 720);

    //シェーダに渡すデータを用意
    MATRIX world, view, proj;
    VECTOR lightPos;
    float ambient;
    COLOR diffuse;

    //カメラ行列
    VECTOR camPos(0, 0, 3), lookat(0, 0, 0), up(0, 1, 0);
    view.camera(camPos, lookat, up);
    //透視投影行列
    proj.pers(0.785f, width / height, 1, 100);
    //光源の位置
    lightPos.set(0, 0, 1);
    //環境光の明るさ（MAX１）
    ambient = 0;
    //頂点の色（MAX１）
    diffuse.set(1, 1, 1, 1);

    //シェーダにデータをセット
    SHADER* shader = new LAMBERT;
    shader->setProjView(proj * view);
    shader->setLightPosition(lightPos);
    shader->setLightAmbient(ambient);
    shader->setDiffuse(diffuse);
    setRasterizerCullNone();

    //３Ｄモデルデータ
    CONTAINER* container = new CONTAINER("assets\\assets.txt");
    BATCH* batch = container->batch("banana");

    while (notQuit)
    {
        clear(60, 120, 240);
        //world行列
        world.identity();
        //バッチモデルの表示
        batch->draw(shader, world);
    }

    delete container;
    delete shader;
}
#endif

#ifdef _GORILLA_CAMERA
#include"libOne.h"
#include"SHADER/LAMBERT.h"
#include"CONTAINER/CONTAINER.h"
#include"CONTAINER/BATCH.h"
void gmain()
{
    window(1000, 1000);

    SHADER* shader = new LAMBERT;
    MATRIX view;
    VECTOR camPos(0, 4, 1.5), lookat(0, 2, -1.5), up(0, 1, 0);
    view.camera(camPos, lookat, up);
    MATRIX proj;
    proj.pers(3.14f / 4, width / height, 1, 100);
    VECTOR lightPos(0, 0, 1);
    float ambient = 0;
    COLOR diffuse(1, 1, 1, 1);
    setRasterizerCullBack();

    CONTAINER* container = new CONTAINER("assets\\square.txt");
    BATCH* batch = container->batch("gorilla");
    MATRIX world;
    float angle = 0;

    while (notQuit)
    {
        angle += 0.017f;
        camPos.set(0, 3, -4 + sin(angle) * 5);
        lookat.set(0, 2, camPos.z - 1);
        view.camera(camPos, lookat, up);

        clear(60, 120, 240);

        shader->setProjView(proj * view);
        shader->setLightPosition(lightPos);
        shader->setLightAmbient(ambient);
        shader->setDiffuse(diffuse);

        for (int i = 0; i < 10; i++)
        {
            world.identity();
            world.mulTranslate(0, 1, -10 + i);
            world.mulRotateZ(sin(angle + i * 0.017 * 30) * 0.25f);
            batch->draw(shader, world);
        }
    }

    delete container;
    delete shader;
}
#endif

#ifdef _BATCH
#include"libOne.h"
#include"SHADER/LAMBERT.h"
#include"CONTAINER/CONTAINER.h"
#include"CONTAINER/BATCH.h"
void gmain()
{
    window(1000, 1000);

    SHADER* shader = new LAMBERT;
    MATRIX view;
    VECTOR camPos(0, 0, 5), lookat(0, 0, 0), up(0, 1, 0);
    view.camera(camPos, lookat, up);
    MATRIX proj;
    proj.pers(3.14f / 4, width / height, 1, 100);
    VECTOR lightPos(0, 0, 1);
    float ambient = 0;
    COLOR diffuse(1, 1, 1, 1);

    CONTAINER* container = new CONTAINER("assets\\elephant.txt");
    BATCH* batch = container->batch("elephant");
    float angle = 0;
    MATRIX world;

    while (notQuit)
    {
        clear(60, 120, 240);

        shader->setProjView(proj * view);
        shader->setLightPosition(lightPos);
        shader->setLightAmbient(ambient);
        shader->setDiffuse(diffuse);
        setRasterizerCullBack();

        world.identity();
        world.mulRotateY(angle);
        batch->draw(shader, world);
        angle += 0.01f;
    }

    delete batch;
    delete shader;
}
#endif

#ifdef _TREE
#include"libOne.h"
#include"SHADER/LAMBERT.h"
#include"CONTAINER/CONTAINER.h"
#include"CONTAINER/BATCH.h"
#include"CONTAINER/TREE.h"
void gmain()
{
    window(1000, 1000);

    SHADER* shader = new LAMBERT;
    MATRIX view;
    VECTOR camPos(0, 0, 6), lookat(0, 0, 0), up(0, 1, 0);
    view.camera(camPos, lookat, up);
    MATRIX proj;
    proj.pers(3.14f / 4, width / height, 1, 100);
    VECTOR lightPos(0, 0, 1);
    float ambient = 1;
    COLOR diffuse(1, 1, 1, 1);

    CONTAINER* container = new CONTAINER("assets\\explorer.txt");
    TREE* tree = new TREE(container->treeOrigin("explorer"));
    ANIMATION* Animation[2];
    Animation[0] = container->animation("explorer_neutral");
    Animation[1] = container->animation("explorer_run");
    tree->setAnimation(Animation[0]);

    MATRIX world;
    int animIdx = 0;

    while (notQuit)
    {
        if (isTrigger(KEY_SPACE))
        {
            ++animIdx %= 2;
            tree->setNextAnimation(Animation[animIdx], 10);
        }

        world.identity();
        tree->update(world);

        clear(60, 120, 240);

        shader->setProjView(proj * view);
        shader->setLightPosition(lightPos);
        shader->setLightAmbient(ambient);
        shader->setDiffuse(diffuse);
        setRasterizerCullBack();

        tree->draw(shader);
    }

    delete tree;
    delete container;
    delete shader;
}
#endif

#ifdef _CONTAINER
#include"libOne.h"
#include"SHADER/LAMBERT.h"
#include"CONTAINER/CONTAINER.h"
#include"CONTAINER/BATCH.h"
#include"CONTAINER/TREE.h"
void setCamPos(VECTOR& camPos)
{
    static float angleV = 0;
    static float angleH = 0;
    static float radius = 2.3f;
    if (isPress(KEY_I))angleV += 0.01f;
    if (isPress(KEY_K))angleV -= 0.01f;
    if (isPress(KEY_J))angleH -= 0.01f;
    if (isPress(KEY_L))angleH += 0.01f;
    if (isPress(KEY_U))radius -= 0.05f;
    if (isPress(KEY_O))radius += 0.05f;
    camPos.x = sin(angleH) * cos(angleV) * radius;
    camPos.y = sin(angleV) * radius;
    camPos.z = cos(angleH) * cos(angleV) * radius;
}
void gmain()
{
    window(1920, 1080, full);
    hideCursor();
    //シェーダー
    SHADER* shader = new LAMBERT;
    setRasterizerCullBack();
    MATRIX view;
    VECTOR camPos(0, 0.0f, 6.0f), lookat(0, 1.0f, 0), up(0, 1, 0);
    view.camera(camPos, lookat, up);
    MATRIX proj;
    proj.pers(3.14f / 4, width / height, 1, 20);
    VECTOR lightPos(0, 1, 1);
    float ambient = 0.0f;
    COLOR diffuse(1, 1, 1, 1);
    //アセット読み込み
    CONTAINER* container = new CONTAINER("assets\\assets.txt");
    //床(草原)
    BATCH* batchGrass = container->batch("floor");
    float pz = 0;
    //象
    BATCH* batchElephant = container->batch("elephant");
    float angle = 0;
    const int num = 12;
    float size[num];
    int dirOfRot[num];
    for (int i = 0; i < num; i += 1)
    {
        size[i] = 2.0f + random(1.0f);
        dirOfRot[i] = random() % 2;
        if (dirOfRot[i] == 0) { dirOfRot[i] = -1; }
    }
    //冒険者
    TREE* tree = new TREE(container->treeOrigin("explorer"));
    ANIMATION* Animation[2];
    Animation[0] = container->animation("explorer_neutral");
    Animation[1] = container->animation("explorer_run");
    tree->setAnimation(Animation[1]);
    int animIdx = 0;
    //world行列。このプログラムの場合、全モデルで共有できる
    MATRIX world;

    while (notQuit)
    {
        //更新---------------------------------------------------------------
        //冒険者のアニメーション切り替え
        if (isTrigger(KEY_SPACE))
        {
            ++animIdx %= 2;
            tree->setNextAnimation(Animation[animIdx], 10);
        }
        //冒険者の行列
        world.identity();
        tree->update(world);
        //カメラ
        setCamPos(camPos);
        camPos += lookat;
        view.camera(camPos, lookat, up);

        //描画---------------------------------------------------------------
        clear(60, 120, 240);
        //シェーダー
        shader->setProjView(proj * view);
        shader->setLightPosition(lightPos);
        shader->setLightAmbient(ambient);
        shader->setDiffuse(diffuse);
        //床（草原）
        pz -= 0.02f;
        if (pz <= -40.0f)pz += 40.0f;
        world.identity();
        world.mulTranslate(0, 0, pz);
        world.mulScaling(4, 1, 4);
        setTextureSamplerWrap();
        batchGrass->draw(shader, world);
        world.identity();
        world.mulTranslate(0, 0, pz+40);
        world.mulScaling(4, 1, 4);
        batchGrass->draw(shader, world);
        setTextureSamplerClamp();
        //象さんたち
        float startPx = num / 2.0f * -1.9f;
        for (int i = 0; i < num; i++)
        {
            world.identity();
            world.mulTranslate( startPx + 1.9f * i, 0.1f, -2 - size[i] * 2);
            world.mulRotateZ(sin(angle)*0.04f*dirOfRot[i]);
            world.mulScaling(size[i], size[i], size[i]);
            batchElephant->draw(shader, world);
        }
        angle += 0.1f;
        //冒険者
        shader->setLightAmbient(0.75f);
        tree->draw(shader);
        //テキスト
        textSize(200);
        text("Africa", 10, 210);
    }

    delete tree;
    delete container;
    delete shader;
}
#endif
