#include "StartScene.h"
#include "GameScene.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace ui;

Scene* StartScene::createScene()
{
    auto scene = Scene::create();
    
    auto layer = StartScene::create();

    scene->addChild(layer);

    return scene;
}

bool StartScene::init()
{    
    if ( !Layer::init() )
    {
        return false;
    }
    
    setupScene();

    return true;
}

void StartScene::setupScene() 
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

    auto label = Label::createWithTTF("START", "PixelEmulator.ttf", 96);
    label->enableOutline(Color4B(255, 255, 255, 255), 4);
    label->setTextColor(Color4B(0, 0, 0, 255));

    auto menuItem = MenuItemLabel::create(label, [](Ref*)
    {
        auto scene = GameScene::createScene();
        Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
    });
    
    auto menu = Menu::createWithItem(menuItem);

    menu->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    
    this->addChild(menu);  
}
