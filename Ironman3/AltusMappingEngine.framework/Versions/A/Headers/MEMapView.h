//  Copyright (c) 2012 BA3, LLC. All rights reserved.

/**
 MEMapView displays maps, annotations and other content, and handles gestures (panning, zooming, scrolling, tapping) perfomed by the user.
 */
#pragma once
#import <MapKit/MapKit.h>
#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>
#import <CoreLocation/CoreLocation.h>
#import "MEProtocols.h"
#import "MECameraPlacement.h"
#import "MELocation3D.h"
#import "MELocationBounds.h"

@class MEMapViewController;

@interface MEMapView : GLKView <UIGestureRecognizerDelegate>

/**TBD*/
+ (void) forceLink; 

/** A simple convenience property for keeping track of multiple views.*/
@property (atomic, retain) NSString* name;

/** The an optional delegate that will receive map-related update messages */
@property (nonatomic, assign) id <MEMapViewDelegate> meMapViewDelegate;


/**---------------------------------------------------------------------------------------
 * @name Tile Selection
 *  ---------------------------------------------------------------------------------------
 */

/**The device independent point width and height for map tiles. The default is 380.0.*/
@property (nonatomic, assign) double tilePointSize;

/**The device dependent pixel width and height for a map tile.*/
@property (nonatomic, readonly) unsigned int tilePixelSize;

/**When set to 1.0, the mapping engine will force all levels to be coherent. In other words, if you have a raster street map, you can force it to always display a consistent. This results in more work for the engine and increased tile counts being displayed. Sometimes this can have a drastic impact on performance at certain viewing angles and distances. This should always be set to 0.0 when possible.*/
@property (nonatomic, assign) double tileLevelBias;

/**When tileLevelBias is greater than zero, setting this to YES limits level bias flip-flopping with smooth camera motion. NOTE: Enabling this setting can result in potentially more tiles being loaded for virtual maps than are necessary for the current camera position.*/
@property (nonatomic, assign) BOOL tileBiasSmoothingEnabled;


/**---------------------------------------------------------------------------------------
 * @name Gesture Recognition
 *  ---------------------------------------------------------------------------------------
 */

/**Scales the velocity of panning gestures up or down. Default is 1.0. If you increase this value, the user's gestures velocities will be multiplied by this value. Consider adjusting the panAcceleration before changing this value.*/
@property (nonatomic, assign) double panVelocityScale;

/**The MEMapViewController that is driving this view.*/
@property (nonatomic, assign) MEMapViewController* meMapViewController;

/**TBD*/
@property(nonatomic, readonly, retain) UIPanGestureRecognizer *panGestureRecognizer;
@property(nonatomic, readonly, retain) UIPinchGestureRecognizer *pinchGestureRecognizer;
@property(nonatomic, readonly, retain) UITapGestureRecognizer *tapGestureRecognizer;
@property(nonatomic, readonly, retain) UITapGestureRecognizer *tapZoomInGestureRecognizer;
@property(nonatomic, readonly, retain) UITapGestureRecognizer *tapZoomOutGestureRecognizer;

/**If set to YES, enables pan deceleration, othersie pan deceleration is disabled. Defaults to YES.*/
@property (nonatomic, assign, getter = isPanDecelerationEnabled) BOOL isPanDecelerationEnabled;

/**Controls the rate at which a panning gesture's velocity slows down once the user finishes the gesture. The default value is -10.0. If you increase this value, it has the affect of giving more momentum to panning gestures. If you set this value to 0, panning gestures will create a motion that will not stop and just continue.*/
@property (nonatomic, assign) double panAcceleration;

/**The color displayed when the screen is cleared with prior to drawing the map.*/
@property (nonatomic, retain) UIColor* clearColor;

/** Sets the minimum distance in meters from the camera/viewer to the map */
@property (nonatomic, assign) double minimumZoom;

/** Sets the maximum distance in meters from the camera/viewer to the map */
@property (nonatomic, assign) double maximumZoom;

/**If set to YES, the view can be panned, otherwise it panning is disabled. Defaults to YES.*/
@property (nonatomic, getter=isPanEnabled) BOOL panEnabled;

/**If set to YES, the view can be zoomed, otherwise it zooming is disabled. Defaults to YES.*/
@property (nonatomic, getter=isZoomEnabled) BOOL zoomEnabled;

/** Zoom in to the center of the screen */
- (void) zoomIn;

/** Zoom in from the center of the screen */
- (void) zoomOut;

/** TBD */
- (double) pointValueToPixelValue:(CGFloat)pointValue;
/** TBD */
- (IBAction) panGestureHandler:(UIGestureRecognizer *)sender;
- (IBAction) pinchGestureHandler:(UIGestureRecognizer *)sender;
- (IBAction) tapGestureHandler:(UIGestureRecognizer *)sender;
- (IBAction) tapZoomInGestureHandler:(UIGestureRecognizer *)sender;
- (IBAction) tapZoomOutGestureHandler:(UIGestureRecognizer *)sender;

/** Force tap getures handling at point */
- (void) tapGestureAtPoint:(CGPoint)point;

/**---------------------------------------------------------------------------------------
 * @name Placement - New
 *  ---------------------------------------------------------------------------------------
 */

/** The amount of time in seconds to animate placement changes (location, altitude, orientation).*/
@property (nonatomic, assign) double defaultAnimationDuration;

/** The geographic location at the center of the view. */
@property (nonatomic, assign) CLLocationCoordinate2D location;

/** Sets the geographic location smoothly over a period of time. */
- (void)setLocation:(CLLocationCoordinate2D) newLocation
  animationDuration:(CGFloat) animationDuration;

/** Sets the geographic location smoothly over a period of time. */
- (void)setLocation3D:(MELocation3D) newLocation
  animationDuration:(CGFloat) animationDuration;

/** The altitude above sea level in meters. */
@property (nonatomic, assign) double altitude;

/** Sets the altitude smoothly over a period of time. */
- (void) setAltitude:(CGFloat) newAltitude
   animationDuration:(CGFloat) animationDuration;

/** Heading in degrees. */
@property (nonatomic, assign) double heading;

/** Sets the heading smoothly over a period of time. */
- (void) setHeading:(CGFloat) newHeading
  animationDuration:(CGFloat) animationDuration;

/** Roll in degrees. */
@property (nonatomic, assign) double roll;

/** Sets the roll smoothly over a period of time. */
- (void) setRoll:(CGFloat) newRoll animationDuration:(CGFloat) animationDuration;

/** Pitch in degrees. */
@property (nonatomic, assign) double pitch;

/** Sets the pitch smoothly over a period of time. */
- (void) setPitch:(CGFloat) newPitch animationDuration:(CGFloat) animationDuration;

/** Provides a convenient way to read or set all camera placement attributes.*/
@property (nonatomic, retain) MECameraPlacement* cameraPlacement;

/** Set to something you create that implementes the methods of the MECameraConstraintDelegate interface to respond to camera location changes. Set to nil to disable.*/
@property (nonatomic, retain) id <MECameraConstraintDelegate> meCameraConstraintDelegate;

/**Converts a geographic coordinate to a screen positiion.*/
- (CGPoint)convertCoordinate:(CLLocationCoordinate2D)coordinate;

/**Converts a geographic coordinate to a screen positiion.*/
- (CGPoint)convertCoordinate3D:(MELocation3D)coordinate;

- (GLKVector3)convertLocationToWorld:(MELocation3D)location;
- (MELocation3D)convertWorldToLocation:(GLKVector3)world;

/** Fit two points in view with some space (buffer) between the two points and the view edge. */
- (void)lookAtCoordinate:(CLLocationCoordinate2D) coordinate1 andCoordinate:(CLLocationCoordinate2D) coordinate2 withHorizontalBuffer:(double)horizontalBufferInPoints withVerticalBuffer:(double)verticalBufferInPoints animationDuration:(CGFloat) animationDuration;

/** Compute a 3D location from which two points (SW point, NE point) would be in view. Useful for looking at an area of interest.*/
- (MELocation3D) location3DThatFitsBounds:(MELocationBounds) bounds
					 withHorizontalBuffer:(double)horizontalBufferInPoints
					   withVerticalBuffer:(double)verticalBufferInPoints;

/** Compute a 3D location form which an array of points would be in view. Useful for looking at a route.*/
- (MELocation3D) location3DThatFitsPoints:(NSArray*) points
					 withHorizontalBuffer:(double)horizontalBufferInPoints
					   withVerticalBuffer:(double)verticalBufferInPoints;

/** Sets the orientation  of the camera.
 @param heading Number of degrees away from true north
 @param roll Number of degrees to roll away from vertical
 @param pitch Number of degrees to look up above the horizon
 @param animationDuration The duration in seconds to animate to this orientation from the previous orientation */
- (void) setCameraOrientation:(CGFloat) heading
						 roll:(CGFloat) roll
						pitch:(CGFloat) pitch
			animationDuration:(CGFloat) animationDuration;

/** Sets the location and orientation of the camera
@param animationDuration The duration in seconds to animate to this position from the previous position */
-(void) setCameraPlacement:(MECameraPlacement *) cameraPlacement
         animationDuration:(CGFloat) animationDuration;


/**---------------------------------------------------------------------------------------
 * @name Dealing with Points and Screen Coordinates
 *  ---------------------------------------------------------------------------------------
 */

/** The scaling factor used to convert points to pixels. This value is computed when the render target size changes. Until there is a valid render target (i.e. the engine is initialized and there is an OpenGL surface available) this value may be 0.*/
@property (readonly) CGFloat deviceScale;

/** If set to greater than 0, overrides the computed device scale. If you are sending cached marker images, or vector style assets to the mapping engine prior to the availability the actual OpenGL render target, you may need to override the deviceScale (which defaults to 1.0). Depending on how your application operates and on which platforms, this may be necessary to do, espeically on the iPhone 6 and higher since the device scale can often be different than the UIMainScreen scale.*/
@property (assign) CGFloat deviceScaleOverride;

/**Converts a screen position to a geographic coordinate.*/
- (CLLocationCoordinate2D)convertPoint:(CGPoint)point;

/**Returns a normalized (0 to 1) value indicating how the given screen coordinate relates to the horizon of the sphere. The value approaches 0 at the horizon and is 0 beyond the horizon. The value approaches the farther away from the horizon the point it. A value of 1.0 would be a point in the 'center' of the sphere equidistant from the horizon in all directions. A value less than 1.0 and greater than 0.0 is approaching the horizon. A value of 0.0 means the point is beyond the horizon.*/
- (double)distanceToHorizonFromPoint:(CGPoint)point;

/** Wraps a CLLocationCoordinate2D in an NSValue.  See also decodeCoodrinate. */
- (NSValue*) encodeCoordinate:(CLLocationCoordinate2D) clLocation;

/** Unwraps a CLLocationCoordinate2D from an NSValue.  See also encodeCoodrinate. */
- (CLLocationCoordinate2D) decodeCoordinate:(NSValue*) valueWrappedCoordinate;




/** TBD */
-(void) addSubscriber:(MEMapView*) subscriber;
/** TBD */
-(void) clearSubscribers;



@end
