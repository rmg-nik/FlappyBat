#include "GameOverScene.h"
#include "GameScene.h"
#include "StartScene.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace ui;

int GameOverScene::m_high_score = 0;

Scene* GameOverScene::createScene(int new_score)
{
    auto scene = Scene::create();
    
    auto layer = GameOverScene::create();

    layer->setNewScore(new_score);

    scene->addChild(layer);

    return scene;
}

bool GameOverScene::init()
{    
    if ( !Layer::init() )
    {
        return false;
    }
    
    setupScene();

    return true;
}

void GameOverScene::setupScene() 
{

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto background = Sprite::create("background.jpg");
    background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(background);

    const float GROUND_HIGH = 100;
    {
        auto sprite_ground = Sprite::create("ground.png");
        sprite_ground->getTexture()->setTexParameters({ GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT });

        sprite_ground->setContentSize(Size(visibleSize.width, GROUND_HIGH));
        sprite_ground->setTextureRect(Rect(0, 0, visibleSize.width, GROUND_HIGH));
        sprite_ground->setAnchorPoint({ 0.0f, 0.0f });
        sprite_ground->setPosition(Vec2(origin.x, origin.y));
        this->addChild(sprite_ground);
    }

    {
        auto sprite_ground = Sprite::create("ground.png");
        sprite_ground->getTexture()->setTexParameters({ GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT });
        sprite_ground->setFlippedY(true);
        sprite_ground->setContentSize(Size(visibleSize.width, GROUND_HIGH));
        sprite_ground->setTextureRect(Rect(0, 0, visibleSize.width, GROUND_HIGH));
        sprite_ground->setAnchorPoint({ 0.0f, 1.0f });
        sprite_ground->setPosition(Vec2(origin.x, origin.y + visibleSize.height));
        this->addChild(sprite_ground);
    }

    auto menu = Menu::create();    

    {
        auto label = Label::createWithTTF("MENU", "PixelEmulator.ttf", 96);
        label->enableOutline(Color4B(255, 255, 255, 255), 4);
        label->setTextColor(Color4B(0, 0, 0, 255));

        auto menuItem = MenuItemLabel::create(label, [](Ref*)
        {
            auto scene = StartScene::createScene();
            Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
        });

        menu->addChild(menuItem);
    }

    {
        auto label = Label::createWithTTF("RETRY", "PixelEmulator.ttf", 96);
        label->enableOutline(Color4B(255, 255, 255, 255), 4);
        label->setTextColor(Color4B(0, 0, 0, 255));

        auto menuItem = MenuItemLabel::create(label, [](Ref*)
        {
            auto scene = GameScene::createScene();
            Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
        });

        menu->addChild(menuItem);
    }
    
    menu->alignItemsVerticallyWithPadding(50);
    menu->setPosition(Vec2(visibleSize.width / 2.0f + origin.x, 2.0f * visibleSize.height / 5.0f + origin.y));
    
    this->addChild(menu);  
}

void GameOverScene::setNewScore(int new_score)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    if (new_score > m_high_score)
        m_high_score = new_score;

    auto high_score = Label::createWithTTF("High Score: " + std::to_string(m_high_score), "PixelEmulator.ttf", 52);
    high_score->enableOutline(Color4B(255, 255, 128, 255), 2);
    high_score->setTextColor(Color4B(0, 0, 0, 255));
    high_score->setPosition(Vec2(visibleSize.width / 2 + origin.x, 3 * visibleSize.height / 4 + origin.y));

    addChild(high_score);

    auto score = Label::createWithTTF("Score: " + std::to_string(new_score), "PixelEmulator.ttf", 52);
    score->enableOutline(Color4B(128, 255, 128, 255), 2);
    score->setTextColor(Color4B(0, 0, 0, 255));
    score->setPosition(Vec2(visibleSize.width / 2 + origin.x, 3 * visibleSize.height / 4 + origin.y - high_score->getContentSize().height - 20));

    addChild(score);
}
