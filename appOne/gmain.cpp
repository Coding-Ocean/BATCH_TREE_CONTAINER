#define _3D_MODEL
//#define _GORILLA_CAMERA
//#define _BATCH
//#define _TREE
//#define _CONTAINER

/*
�V�F�[�_�Ƃ�
 �f�o�t�i�O���t�B�b�N�{�[�h�̂b�o�t�j�ɑ΂��閽�ߏ��̂��ƁB
 ��Ɂu���W�P�_�̕ϊ��v�Ɓu�s�N�Z���P�_�̕`��v���R�[�f�B���O����Ă���B
 (�ȉ��̃v���O�����̓V�F�[�_�ɕK�v�Ȓl��ݒ肵�Ă��邾���ŁA
 �V�F�[�_�v���O�����ł͂Ȃ��I)
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

    //�V�F�[�_�ɓn���f�[�^��p��
    MATRIX world, view, proj;
    VECTOR lightPos;
    float ambient;
    COLOR diffuse;

    //�J�����s��
    VECTOR camPos(0, 0, 3), lookat(0, 0, 0), up(0, 1, 0);
    view.camera(camPos, lookat, up);
    //�������e�s��
    proj.pers(0.785f, width / height, 1, 100);
    //�����̈ʒu
    lightPos.set(0, 0, 1);
    //�����̖��邳�iMAX�P�j
    ambient = 0;
    //���_�̐F�iMAX�P�j
    diffuse.set(1, 1, 1, 1);

    //�V�F�[�_�Ƀf�[�^���Z�b�g
    SHADER* shader = new LAMBERT;
    shader->setProjView(proj * view);
    shader->setLightPosition(lightPos);
    shader->setLightAmbient(ambient);
    shader->setDiffuse(diffuse);
    setRasterizerCullNone();

    //�R�c���f���f�[�^
    CONTAINER* container = new CONTAINER("assets\\assets.txt");
    BATCH* batch = container->batch("banana");

    while (notQuit)
    {
        clear(60, 120, 240);
        //world�s��
        world.identity();
        //�o�b�`���f���̕\��
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
    //�V�F�[�_�[
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
    //�A�Z�b�g�ǂݍ���
    CONTAINER* container = new CONTAINER("assets\\assets.txt");
    //��(����)
    BATCH* batchGrass = container->batch("floor");
    float pz = 0;
    //��
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
    //�`����
    TREE* tree = new TREE(container->treeOrigin("explorer"));
    ANIMATION* Animation[2];
    Animation[0] = container->animation("explorer_neutral");
    Animation[1] = container->animation("explorer_run");
    tree->setAnimation(Animation[1]);
    int animIdx = 0;
    //world�s��B���̃v���O�����̏ꍇ�A�S���f���ŋ��L�ł���
    MATRIX world;

    while (notQuit)
    {
        //�X�V---------------------------------------------------------------
        //�`���҂̃A�j���[�V�����؂�ւ�
        if (isTrigger(KEY_SPACE))
        {
            ++animIdx %= 2;
            tree->setNextAnimation(Animation[animIdx], 10);
        }
        //�`���҂̍s��
        world.identity();
        tree->update(world);
        //�J����
        setCamPos(camPos);
        camPos += lookat;
        view.camera(camPos, lookat, up);

        //�`��---------------------------------------------------------------
        clear(60, 120, 240);
        //�V�F�[�_�[
        shader->setProjView(proj * view);
        shader->setLightPosition(lightPos);
        shader->setLightAmbient(ambient);
        shader->setDiffuse(diffuse);
        //���i�����j
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
        //�ۂ��񂽂�
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
        //�`����
        shader->setLightAmbient(0.75f);
        tree->draw(shader);
        //�e�L�X�g
        textSize(200);
        text("Africa", 10, 210);
    }

    delete tree;
    delete container;
    delete shader;
}
#endif
