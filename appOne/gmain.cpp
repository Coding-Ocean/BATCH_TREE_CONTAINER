#define _CONTAINER

#ifdef _SQUARE
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
    float ambient = 1;
    COLOR diffuse(1, 1, 1, 1);

    CONTAINER c("assets\\square.txt");
    BATCH* b = c.batch("square");
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
        setRasterizerCullBack();

        for (int i = 0; i < 10; i++)
        {
            world.identity();
            world.mulRotateZ(sin(angle + i * 0.017 * 30) * 0.25f);
            world.mulTranslate(0, 1, -10 + i);
            b->draw(shader, world);
        }
    }

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
    VECTOR camPos(0, 0, 6), lookat(0, 0, 0), up(0, 1, 0);
    view.camera(camPos, lookat, up);
    MATRIX proj;
    proj.pers(3.14f / 4, width / height, 1, 100);
    VECTOR lightPos(0, 0, 1);
    float ambient = 1;
    COLOR diffuse(1, 1, 1, 1);

    CONTAINER c("assets\\elephant.txt");
    BATCH* b = c.batch("elephant");
    MATRIX world;
    float angle = 0;

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
        b->draw(shader, world);
        angle += 0.01f;
    }

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

    CONTAINER c("assets\\explorer.txt");
    TREE* t = new TREE(c.treeOrigin("explorer"));
    ANIMATION* Animation[2];
    Animation[0] = c.animation("explorer_neutral");
    Animation[1] = c.animation("explorer_run");
    t->setAnimation(Animation[0]);

    MATRIX world;
    int animIdx = 0;

    while (notQuit)
    {
        if (isTrigger(KEY_SPACE))
        {
            ++animIdx %= 2;
            t->setNextAnimation(Animation[animIdx], 10);
        }

        world.identity();
        t->update(world);

        clear(60, 120, 240);

        shader->setProjView(proj * view);
        shader->setLightPosition(lightPos);
        shader->setLightAmbient(ambient);
        shader->setDiffuse(diffuse);
        setRasterizerCullBack();

        t->draw(shader);
    }

    delete t;
    delete shader;
}
#endif
#ifdef _CONTAINER
#include"libOne.h"
#include"SHADER/LAMBERT.h"
#include"CONTAINER/CONTAINER.h"
#include"CONTAINER/BATCH.h"
#include"CONTAINER/TREE.h"
void gmain()
{
    window(1920, 1080, full);
    hideCursor();
    //�V�F�[�_�[
    SHADER* shader = new LAMBERT;
    MATRIX view;
    VECTOR camPos(0, 6.3f, 6.0f), lookat(0, 1.0f, 0), up(0, 1, 0);
    view.camera(camPos, lookat, up);
    MATRIX proj;
    proj.pers(3.14f / 4, width / height, 1, 20);
    VECTOR lightPos(0, 1, 1);
    float ambient = 0.0f;
    COLOR diffuse(1, 1, 1, 1);
    //�A�Z�b�g�ǂݍ���
    CONTAINER* c = new CONTAINER("assets\\assets.txt");
    //��(����)
    BATCH* b1 = c->batch("floor");
    float pz = 0;
    //��
    BATCH* b2 = c->batch("elephant");
    float angle = 0;
    const int num = 15;
    float size[num];
    int dirOfRot[num];
    for (int i = 0; i < num; i += 1)
    {
        size[i] = 1.0f + random(1.0f);
        dirOfRot[i] = random() % 2;
        if (dirOfRot[i] == 0) { dirOfRot[i] = -1; }
    }
    //�`����
    TREE* t = new TREE(c->treeOrigin("explorer"));
    ANIMATION* Animation[2];
    Animation[0] = c->animation("explorer_neutral");
    Animation[1] = c->animation("explorer_run");
    t->setAnimation(Animation[1]);
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
            t->setNextAnimation(Animation[animIdx], 10);
        }
        //�`���҂̍s��
        world.identity();
        t->update(world);

        //�`��---------------------------------------------------------------
        clear(60, 120, 240);
        //�V�F�[�_�[
        shader->setProjView(proj * view);
        shader->setLightPosition(lightPos);
        shader->setLightAmbient(ambient);
        shader->setDiffuse(diffuse);
        setRasterizerCullBack();
        //���i�����j
        pz -= 0.02f;
        if (pz <= -40.0f)pz += 40.0f;
        world.identity();
        world.mulTranslate(0, 0, pz);
        world.mulScaling(4, 1, 4);
        setTextureSamplerWrap();
        b1->draw(shader, world);
        world.identity();
        world.mulTranslate(0, 0, pz+40);
        world.mulScaling(4, 1, 4);
        b1->draw(shader, world);
        setTextureSamplerClamp();
        //�ۂ��񂽂�
        for (int i = 0; i < num; i++) 
        {
            world.identity();
            world.mulTranslate(-7.0f + 1.5f * i, 0.1f, -2 - size[i] * 2);
            world.mulRotateZ(sin(angle)*0.04f*dirOfRot[i]);
            world.mulScaling(size[i], size[i], size[i]);
            b2->draw(shader, world);
        }
        angle += 0.1f;
        //�`����
        shader->setLightAmbient(0.5f);
        t->draw(shader);
        //�e�L�X�g
        textSize(200);
        text("Africa", 10, 210);
    }

    delete t;
    delete c;
    delete shader;
}
#endif
