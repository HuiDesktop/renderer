/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "WinMouseTracker.h"
#include "PointInPolygon.h"
#include <spine/spine.h>
#include <spine/spine-cocos2dx.h>
#include "../proj.win32/StartInfo.h"

USING_NS_CC;
using namespace spine;

Scene* HelloWorld::createScene()
{
	return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

	if (closeItem == nullptr ||
		closeItem->getContentSize().width <= 0 ||
		closeItem->getContentSize().height <= 0)
	{
		problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
	}
	else
	{
		float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
		float y = origin.y + closeItem->getContentSize().height / 2;
		closeItem->setPosition(Vec2(x, y));
	}

	/////////////////////////////
	// 3. add your codes below...

	MouseTracker::setWindow(Director::getInstance()->getOpenGLView()->getWin32Window());
	MouseTracker::setWindowPos(StartInfo::x, StartInfo::y);

	// events
	scheduleUpdate();
	mouseEventListener = EventListenerMouse::create();
	mouseEventListener->onMouseDown = CC_CALLBACK_1(HelloWorld::onMouseDown, this);
	mouseEventListener->onMouseUp = CC_CALLBACK_1(HelloWorld::onMouseUp, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseEventListener, this);

	// test spine sprite
	auto textureLoader = new Cocos2dTextureLoader();
	auto _atlas = new Atlas(StartInfo::atlasFile.c_str(), textureLoader, true); // TODO: Null check
	auto _attachmentLoader = new Cocos2dAtlasAttachmentLoader(_atlas);
	SkeletonBinary skel(_attachmentLoader);
	skel.setScale(StartInfo::scale);
	auto skeletonData = skel.readSkeletonDataFile(StartInfo::skelFile.c_str());
	skeletonAnimation = SkeletonAnimation::createWithData(skeletonData);
	skeletonAnimation->setAnimation(0, "Relax", true);
	skeletonAnimation->setPosition(StartInfo::x0, StartInfo::y0);
	this->addChild(skeletonAnimation, 1);

	//test bounding
	dnode = DrawNode::create();
	assert(dnode);
	this->addChild(dnode, 114514);
	return true;
}

void HelloWorld::update(float dt) {
	UNREFERENCED_PARAMETER(dt);
	spine::Vector<float> vec;
	spine::Vector<unsigned int> poly;
	int total = skeletonAnimation->getSkeleton()->getBounds1(vec, poly);
	int curX = MouseTracker::mouseX - MouseTracker::winX;
	int curY = StartInfo::h - (MouseTracker::mouseY - MouseTracker::winY);
	bool inPoly = false;

	dnode->clear();
	for (int i = 0; i < total; ++i) {
		std::vector<Point> ps;
		ps.reserve((poly[i + 1] - poly[i]) / 2);
		for (int j = poly[i], je = poly[i + 1]; j < je; j += 2) {
			ps.push_back(Point(vec[j] + skeletonAnimation->getPositionX(), vec[j + 1] + skeletonAnimation->getPositionY()));
		}
		if (!inPoly && Point_In_Polygon_2D(curX, curY, ps)) {
			inPoly = true;
			break;
		}
	}
	if (inPoly) {
		dnode->drawDot(Vec2(curX, curY), 1, Color4F(0, 1, 0, 1));
	}
	else {
		dnode->drawDot(Vec2(curX, curY), 5, Color4F(1, 0, 0, 1));
	}
	MouseTracker::state = inPoly;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();

	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);


}

void HelloWorld::onMouseDown(cocos2d::Event* event)
{
	prepareNewMessage(StartInfo::ipc, 2, 0);
	finishNewMessage(StartInfo::ipc);
	setForSend(StartInfo::ipc);
	MouseTracker::beginDrag();
}

void HelloWorld::onMouseUp(cocos2d::Event* event)
{
	MouseTracker::endDrag();
	int* pos = (int*)prepareNewMessage(StartInfo::ipc, 3, 8);
	pos[0] = MouseTracker::winX;
	pos[1] = MouseTracker::winY;
	finishNewMessage(StartInfo::ipc);
	setForSend(StartInfo::ipc);
}
