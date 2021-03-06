/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.1.28
 *Email: 2117610943@qq.com
 */
#include "spine/spine-cocos2dx.h"

#include "GSPauseQuitLayer.h"
#include "GameScene.h"

#include "../SelectPlantsScene/SelectPlantsScene.h"
#include "../WorldScene/World_1.h"

using namespace spine;

Layer* GSPauseQuitLayer::addLayer()
{
	return GSPauseQuitLayer::create();
}

void GSPauseQuitLayer::pauseLayer()
{
	Director::getInstance()->getRunningScene()->getChildByName("backgroundLayer")->onExit();/* 暂停层 */
	Director::getInstance()->getRunningScene()->getChildByName("buttonLayer")->onExit();
	Director::getInstance()->getRunningScene()->getChildByName("animationLayer")->onExit();
	Director::getInstance()->getRunningScene()->getChildByName("controlLayer")->onExit();
	Director::getInstance()->getRunningScene()->getChildByName("informationLayer")->onExit();
	Director::getInstance()->getRunningScene()->getChildByName("sunLayer")->onExit();
	Global::getInstance()->stopMusic();
}

void GSPauseQuitLayer::resumeLayer()
{
	Director::getInstance()->getRunningScene()->getChildByName("backgroundLayer")->onEnter();/* 恢复层 */
	Director::getInstance()->getRunningScene()->getChildByName("buttonLayer")->onEnter();
	Director::getInstance()->getRunningScene()->getChildByName("animationLayer")->onEnter();
	Director::getInstance()->getRunningScene()->getChildByName("controlLayer")->onEnter();
	Director::getInstance()->getRunningScene()->getChildByName("informationLayer")->onEnter();
	Director::getInstance()->getRunningScene()->getChildByName("sunLayer")->onEnter();
	Global::getInstance()->resumeMusic();
}

bool GSPauseQuitLayer::init()
{
	if (!LayerColor::initWithColor(Color4B(0, 0, 0, 180)))return false;

	GameScene::setPauseGame(true);
	/* 创建对话框 */
	createDialog();

	return true;
}

void GSPauseQuitLayer::createDialog()
{
	_option = Sprite::createWithSpriteFrameName("LevelObjiectivesBg.png");
	_option->setPosition(_director->getWinSize() / 2);
	_option->setScale(0.9f);
	this->addChild(_option);

	auto PauseAnimation= SkeletonAnimation::createWithData(_global->userInformation->getAnimationData().find("PauseAnimation")->second);
	PauseAnimation->setAnimation(0, "animation", true);
	PauseAnimation->setPosition(Vec2(530, 650));
	_option->addChild(PauseAnimation);

	/* 触摸移动监听 */
	_touchListener = createTouchtListener(_option);

	/* 创建滑动条 */
	auto musicslider = createSlider(Vec2(600, 520), Vec2(150, 520), _global->userInformation->getGameText().find("音乐")->second, OptionScene_Slider::音乐,
		Sprite::createWithSpriteFrameName("bgFile.png"),
		Sprite::createWithSpriteFrameName("progressFile.png"),
		Sprite::createWithSpriteFrameName("thumbFile.png"),
		nullptr,
		true);
	auto SoundEffectslider = createSlider(Vec2(600, 450), Vec2(150, 450), _global->userInformation->getGameText().find("音效")->second, OptionScene_Slider::音效,
		Sprite::createWithSpriteFrameName("bgFile.png"),
		Sprite::createWithSpriteFrameName("progressFile.png"),
		Sprite::createWithSpriteFrameName("thumbFile.png"),
		nullptr,
		true);

	musicslider->setScale(1.2f);
	SoundEffectslider->setScale(1.2f);

	/* 创建复选框 */
	auto check    = createCheckBox(Vec2(350, 380), Vec2(150, 380), _global->userInformation->getGameText().find("信息")->second, OptionScene_CheckBox::显示信息, "CheckBox2", "CheckBox", true);
	auto check1   = createCheckBox(Vec2(800, 380), Vec2(600, 380), _global->userInformation->getGameText().find("全屏")->second, OptionScene_CheckBox::全屏, "CheckBox2", "CheckBox", true);
	auto check2   = createCheckBox(Vec2(350, 310), Vec2(150, 310), _global->userInformation->getGameText().find("高帧率")->second, OptionScene_CheckBox::高帧率, "CheckBox2", "CheckBox", true);
	auto check3   = createCheckBox(Vec2(800, 310), Vec2(600, 310), _global->userInformation->getGameText().find("鼠标显示")->second, OptionScene_CheckBox::鼠标隐藏, "CheckBox2", "CheckBox", true);
	auto check4   = createCheckBox(Vec2(350, 240), Vec2(150, 240), _global->userInformation->getGameText().find("拉伸显示")->second, OptionScene_CheckBox::拉伸显示, "CheckBox2", "CheckBox", true);

	check->setScale(0.6f);
	check1->setScale(0.6f);
	check2->setScale(0.6f);
	check3->setScale(0.6f);
	check4->setScale(0.6f);

	/* 创建按钮 */
	createButton(Vec2(210, 170), _global->userInformation->getGameText().find("查看图鉴")->second, PauseQuitLayer_Button::查看图鉴);
	createButton(Vec2(520, 170), _global->userInformation->getGameText().find("重新开始")->second, PauseQuitLayer_Button::从新开始);
	createButton(Vec2(830, 170), _global->userInformation->getGameText().find("返回游戏")->second, PauseQuitLayer_Button::返回游戏);
	createButton(Vec2(365, 70), _global->userInformation->getGameText().find("按键说明")->second, PauseQuitLayer_Button::按键说明);
	createButton(Vec2(665, 70), _global->userInformation->getGameText().find("退出游戏")->second, PauseQuitLayer_Button::退出游戏);
}

void GSPauseQuitLayer::createButton(const Vec2& vec2, const std::string name, PauseQuitLayer_Button button_type)
{
	/* 创建返回主菜单按钮 */
	auto button = ui::Button::create("ButtonNew2.png", "ButtonNew.png", "", TextureResType::PLIST);
	auto label = Label::createWithTTF(name, "resources/fonts/GameFont.ttf", 35);
	label->enableShadow(Color4B(0, 0, 0, 200));//设置阴影
	label->setScale(2.0f);
	button->setTitleLabel(label);
	button->setTitleColor(Color3B::WHITE);
	button->setPosition(vec2);
	button->setScale(0.5f);
	_option->addChild(button);

	button->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case ui::Widget::TouchEventType::BEGAN:
				AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("gravebutton")->second), _global->userInformation->getSoundEffectVolume());
				break;
			case ui::Widget::TouchEventType::ENDED:
				switch (button_type)
				{
				case PauseQuitLayer_Button::查看图鉴:
					Application::getInstance()->openURL("https://share.weiyun.com/5TewoDc");
					break;
				case PauseQuitLayer_Button::从新开始:
					_director->getScheduler()->setTimeScale(1.0f);
					UserDefault::getInstance()->setIntegerForKey("BREAKTHROUGH", ++_global->userInformation->getBreakThroughnumbers());
					_director->replaceScene(TransitionFade::create(1.0f, SelectPlantsScene::createScene()));
					break;
				case PauseQuitLayer_Button::退出游戏:
					_director->getScheduler()->setTimeScale(1.0f);
					UserDefault::getInstance()->setIntegerForKey("BREAKTHROUGH", ++_global->userInformation->getBreakThroughnumbers());
					_director->replaceScene(TransitionFade::create(1.0f, World_1::createScene()));
					break;
				case PauseQuitLayer_Button::按键说明:
					showPrompt();
					break;
				case PauseQuitLayer_Button::返回游戏:
					resumeLayer();
					GameScene::setPauseGame(false);
					this->removeFromParent();
					break;
				default:
					break;
				}
			}
		});
}

void GSPauseQuitLayer::showPrompt()
{
	_promptLayer = LayerColor::create(Color4B(0, 0, 0, 180));
	createShieldLayer(_promptLayer);
	this->addChild(_promptLayer);

	auto prompt = Sprite::createWithSpriteFrameName("Prompt.png");
	prompt->setPosition(_director->getWinSize() / 2.0f);
	prompt->setScale(2.0f);
	prompt->setOpacity(200);
	_promptLayer->addChild(prompt);

	this->createTouchtListener(prompt);

	auto Close = ui::Button::create("CloseDown.png", "Close.png", "", TextureResType::PLIST);
	Close->setPosition(Vec2(750, 360));
	Close->setScale(0.6f);
	prompt->addChild(Close);

	Close->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case ui::Widget::TouchEventType::BEGAN:
				AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("tap")->second), _global->userInformation->getSoundEffectVolume());
				break;
			case ui::Widget::TouchEventType::ENDED:
				_promptLayer->removeFromParent();
				break;
			}
		});
}