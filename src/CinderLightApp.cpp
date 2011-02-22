#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/Light.h"
#include "cinder/gl/Material.h"
#include "cinder/MayaCamUI.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class CinderLightApp : public AppBasic {
  public:
	void prepareSettings( Settings *settings );
	void setup();
	void setupCamera();
	void setupLights();
	
	// Keyboard
	bool _isOptionDown;
	void keyUp	( KeyEvent event);
	void keyDown( KeyEvent event);
	
	// Mouse
	void mouseMove( MouseEvent event );
	void mouseDown( MouseEvent event );
	void mouseDrag( MouseEvent event );
	void mouseUp  ( MouseEvent event );
	
	// Loop
	void update();
	void draw();
	
	
	MayaCamUI				mMayaCam;
	ci::CameraPersp			*_cameraPerspective;
	ci::Quatf				*_sceneRotation;
	
	// Light
	ci::gl::Light			*_light;
	ci::gl::Material		*_ribbonMaterial;
	
	// animation
	double				mTimePrevious;
	double				mTime;
	bool				bAnimate;
};

#define APP_INITIAL_WIDTH 1024
#define APP_INITIAL_HEIGHT 768

void CinderLightApp::prepareSettings( Settings* settings )
{
	settings->setWindowSize( APP_INITIAL_WIDTH, APP_INITIAL_HEIGHT );
	settings->setFrameRate( 60.0f );
}
void CinderLightApp::setup()
{
	setupCamera();
	setupLights();
	
	_isOptionDown = false;
	
	mTimePrevious = getElapsedSeconds();
	mTime = 0.0;
	bAnimate = false;
}

void CinderLightApp::setupCamera()
{
	// Create camera
	Vec3f p = Vec3f::one() * 10.0f;
	CameraPersp cam = CameraPersp( getWindowWidth(), getWindowHeight(), 45.0f );
	cam.setEyePoint( p );
	cam.setCenterOfInterestPoint( Vec3f::zero() );
	cam.setPerspective( 45.0f, getWindowAspectRatio(), 0.1f, 500.0f );
	
	// Set mayacamera
	mMayaCam.setCurrentCam( cam );
	
	// set up our projector (a special type of light)
	_light = new gl::Light( gl::Light::POINT, 0 );
	//mLight = new gl::Light( gl::Light::DIRECTIONAL, 0 );
	_light->lookAt( Vec3f( 0, 5, 5 ), Vec3f( 0, 0, 0 ) );
	_light->setAmbient( Color( 1.0f, 1.0f, 1.0f ) );
	_light->setDiffuse( Color( 1.0f, 1.0f, 1.0f ) );
	_light->setSpecular( Color( 1.0f, 1.0f, 1.0f ) );
//	_light->setShadowParams( 40.0f, 1.0f, 30.0f );
	_light->enable();
	
	// setup our scene (a simple gray cube)
	_ribbonMaterial = new gl::Material();
	_ribbonMaterial->setSpecular( Color::white() );
	_ribbonMaterial->setDiffuse( Color(0.9f, 0.1f, 0.6f) );
	_ribbonMaterial->setAmbient( Color( 0.6f, 0.2f, 0.2f ) );
	_ribbonMaterial->setShininess( 5.0f );
	

}

void CinderLightApp::setupLights()
{
	
}

void CinderLightApp::mouseMove( MouseEvent event )
{
}

void CinderLightApp::mouseDown( MouseEvent event )
{	
	// let the camera handle the interaction
	mMayaCam.mouseDown( event.getPos() );
}

void CinderLightApp::mouseDrag( MouseEvent event )
{
	// let the camera handle the interaction
	mMayaCam.mouseDrag( event.getPos(), event.isLeftDown(), event.isMetaDown(), event.isRightDown() );
}

void CinderLightApp::mouseUp( MouseEvent event )
{	
	// let the camera handle the interaction
	mMayaCam.mouseDown( event.getPos() );
}

void CinderLightApp::keyDown( KeyEvent event ) {
//	_isOptionDown = event.isMetaDown();
	if(event.getCode() == KeyEvent::KEY_a) {
		bAnimate = !bAnimate;
	}
}

void CinderLightApp::keyUp( KeyEvent event ) {
}


void CinderLightApp::update()
{
	double elapsed = getElapsedSeconds() - mTimePrevious;
	mTimePrevious = getElapsedSeconds();
	
	if(bAnimate) mTime += 0.25 * elapsed;
	
	/*// animate camera
	 if(bAnimate) {
	 Vec3f	eye = Vec3f( cosf(mTime), 1.0f, sinf(mTime) ) * 8.0f;
	 mCamera->lookAt( eye, Vec3f::zero() );
	 } //*/
	
	// animate light
	if(bAnimate) {
		Vec3f	p = Vec3f( cosf(mTime), 1.0f - 0.5f * sinf(0.1f * mTime), sinf(mTime) ) * 75.0f;
		_light->lookAt( -p, Vec3f::zero() );
	}//*/
}

void CinderLightApp::draw()
{
	// clear out the window with black
//	gl::clear( Color( 0, 0, 0 ) ); 
	gl::clear();
	gl::enableDepthWrite();
	gl::enableDepthRead();
	
	glEnable( GL_LIGHTING );
	gl::setMatrices( mMayaCam.getCamera() );

	_light->update( mMayaCam.getCamera() );
	_light->enable();
	_ribbonMaterial->apply();
	// DRAW
	glPushMatrix();
//		gl::rotate( getElapsedSeconds() * 0.01f );
		gl::drawSphere( ci::Vec3f(0.0f, sinf(0.01f * getElapsedFrames()) * 50, 0.0f) , 10, 32);
	glPopMatrix();
	// DRAW NO LIGHTING
	glDisable( GL_LIGHTING );
	
	// Draw the lighting frustum
	glColor3f( 1.0f, 1.0f, 1.0f );
	gl::drawFrustum( _light->getShadowCamera() );
}


CINDER_APP_BASIC( CinderLightApp, RendererGl )
