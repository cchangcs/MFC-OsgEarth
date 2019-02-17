#include "StdAfx.h"
#include "OSGObject.h"


COSGObject::COSGObject(HWND hWnd)
{
	m_hWnd = hWnd;
}


COSGObject::~COSGObject(void)
{
}

void COSGObject::InitOSG()
{
	InitSceneGraph();
	InitCameraConfig();
	InitOsgEarth();

}

void COSGObject::InitSceneGraph()
{
	mRoot = new osg::Group;
	osg::ref_ptr<osg::Node> mp = osgDB::readNodeFile("arcgisonline.earth");
	mRoot->addChild(mp);
	mapNode = dynamic_cast<osgEarth::MapNode*>(mp.get());
	//mRoot->addChild(osgDB::readNodeFile("H:/002.OpenSceneGraph/019.Earth/003.第三讲-VPB用法详解与常见问题处理/vpbtest/TestCommon10/output.ive"));
}

void COSGObject::InitCameraConfig()
{
	RECT rect;
	mViewer = new osgViewer::Viewer;
	::GetWindowRect(m_hWnd, &rect);
	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
	osg::ref_ptr<osg::Referenced> windata = new osgViewer::GraphicsWindowWin32::WindowData(m_hWnd);
	traits->x = 0;
	traits->y = 0;
	traits->width = rect.right - rect.left;
	traits->height = rect.bottom - rect.top;
	traits->windowDecoration = false;
	traits->doubleBuffer = true;
	traits->sharedContext = 0;
	traits->setInheritedWindowPixelFormat = true;
	traits->inheritedWindowData = windata;
	osg::GraphicsContext * gc = osg::GraphicsContext::createGraphicsContext(traits);

	osg::ref_ptr<osg::Camera> camera = new osg::Camera;
	camera->setGraphicsContext(gc);
	camera->setViewport(new osg::Viewport(traits->x, traits->y, traits->width, traits->height));
	camera->setProjectionMatrixAsPerspective(30.0f, static_cast<double>(traits->width) / static_cast<double>(traits->height), 1.0, 1000.0);

	mViewer->setCamera(camera);
	//mViewer->setCameraManipulator(new osgGA::TrackballManipulator);
	mViewer->setSceneData(mRoot);
	mViewer->realize();
	mViewer->getCamera()->setComputeNearFarMode(osg::CullSettings::COMPUTE_NEAR_FAR_USING_PRIMITIVES);
	mViewer->getCamera()->setNearFarRatio(0.000003f);
}

void COSGObject::PreFrameUpdate()
{

}

void COSGObject::PostFrameUpdate()
{

}

void COSGObject::Render(void * ptr)
{
	COSGObject *osg = (COSGObject*)ptr;
	osgViewer::Viewer * viewer = osg->getViewer();
	while (!viewer->done())
	{
		osg->PreFrameUpdate();
		viewer->frame();
		osg->PostFrameUpdate();
	}

	_endthread();
}

osgViewer::Viewer *COSGObject::getViewer()
{
	return mViewer;
}

void COSGObject::InitOsgEarth()
{
	//初始化操作器
	em = new osgEarth::Util::EarthManipulator;
	if (mapNode.valid())
	{
		em->setNode(mapNode);
	}
	em->getSettings()->setArcViewpointTransitions(true);
	mViewer->setCameraManipulator(em);

	////初始化天空
	//osgEarth::Config skyConf;
	//double hours = skyConf.value("hours", 12.0);
	//osg::ref_ptr<osgEarth::Util::SkyNode> sky_node = new osgEarth::Util::SkyNode(mapNode->getMap());
	//sky_node->setDateTime(2012, 1, 27, hours);
	//sky_node->attach(mViewer, 1);
	//sky_node->setAmbientBrightness(1.0, mViewer);
	//mRoot->addChild(sky_node);

}