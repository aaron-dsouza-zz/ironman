//  Copyright (c) 2012 BA3, LLC. All rights reserved.
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <CoreLocation/CoreLocation.h>

/**Useful math functions.*/
@interface MEMath : NSObject

/**Given a CGPoint where x represents longitude, and y represents latitude, returns a new CGPoint in EPSG:3857 coordinates where x represents longitude and y represents latitude.*/
+(CGPoint) mercatorTo3857:(CGPoint) sourceCoordinate;

/**Convert degrees to radians.*/
+(double) toRadians:(double)degrees;

/**Convert radians to degrees.*/
+(double) toDegrees:(double)radians;

/**Returns the distance in radians between two points specified in radians.*/
+(double) distanceBetween:(double) lon1Radians
			  lat1Radians:(double) lat1Radians
			  lon2Radians:(double) lon2Radians
			  lat2Radians:(double) lat2Radians;

/**Returns the great circle distance between two points in radians between two points.*/
+(double) distanceBetween:(CGPoint) point1
				   point2:(CGPoint) point2;

/**Returns the great circle distance in nautical miles between two points.*/
+(double) nauticalMilesBetween:(CGPoint) point1
						point2:(CGPoint) point2;

/**Returns the great circle distance in nautical miles between two locations.*/
+(double) nauticalMilesBetween:(CLLocationCoordinate2D) location1
					 location2:(CLLocationCoordinate2D) location2;


/**Returns the great circle distance in nautical miles of a route.*/
+(double) nauticalMilesInRoute:(NSArray*) wayPoints;

/**Given an array of way points, return a new array of points evenly spaced along that route.*/
+(NSArray*) tesselateRoute:(NSArray*) wayPoints pointCount:(uint) pointCount;

/**Converts radians to nautical miles.*/
+(double) radiansToNauticalMiles:(double) radians;

/**Converts nautical miles to radians.*/
+(double) nauticalMilesToRadians:(double) nauticalMiles;

/**Converts nautical miles to meters.*/
+(double) nauticalMilesToMeters:(double) nauticalMiles;

/**Converts meters to nautical miles.*/
+(double) metersToNauticalMiles:(double) meters;

/**Returns a point on a radial a given distance from an origin point.
 @param point Where x = longitude, y = latitude
 @param radial Angle in degrees.
 @param distance Distance in nautical miles.*/
+(CGPoint) pointOnRadial:(CGPoint) point
				  radial:(double) radial
				distance:(double) distance;

/**Returns a location on a radial a given distance from an origin location.
 @param location The point from which to measure.
 @param radial Angle in degrees.
 @param distance Distance in nautical miles.*/
+(CLLocationCoordinate2D) locationOnRadial:(CLLocationCoordinate2D) location
									radial:(double) radial
								  distance:(double) distance;

/**Returns the degrees heading you would need to take to go from point1 to point2.*/
+(double) courseFromPoint:(CGPoint) point1
				  toPoint:(CGPoint) point2;

/**Returns the degrees heading you would need to take to go from location1 to location2.*/
+(double) courseFromLocation:(CLLocationCoordinate2D) location1
				  toLocation:(CLLocationCoordinate2D) location2;


//(vec2d origin, double radial, double distance)
/**Tesselates a route between two geographic points into an array of points of nodeCount size.*/
+(NSArray*) tesselateRoute:(CGPoint) point1
					point2:(CGPoint) point2
				 nodeCount:(int) nodeCount;

/**Tesselates a route between two geographic points into an array of points along the route separating each node by milesPerNode nautical miles.*/
+(NSArray*) tesselateRoute:(CGPoint) point1
					point2:(CGPoint) point2
			  milesPerNode:(double) milesPerNode;

/**Returns the point that is f/1 the distance between point1 and point2. When f = 0, point 1 will be returned, when f = 1, point 2 will be returned. This is used for route-line point tesselation. The units of point1 and point 2 should be degrees x=lon/y=lat. Returned point will be degrees x=lon/y=lat.*/
+(CGPoint) pointBetween:(CGPoint) point1
				 point2:(CGPoint) point2
			   fraction:(double) f;

/**Returns the location that is f percent the distance between locations */
+(CLLocationCoordinate2D) locationBetween:(CLLocationCoordinate2D)a
							  andLocation:(CLLocationCoordinate2D)b
								 fraction:(double) f;

/**Returns YES if x is a power of 2.*/
+(BOOL) isPowerOfTwo:(uint) x;

/**Returns the next power of 2.*/
+(int) nextPowerOfTwo:(int) x;

/**Returns Earth's radius in meters.*/
+(double) getEarthRadiusInMeters;

/**Returns Earth's circumference in meters.*/
+(double) getEarthCircumferenceInMeters;

/**Returns Earth's circumference in nautical miles.*/
+(double) getEarthCircumferenceInNauticalMiles;

/**Returns the internal 'epsilon' value used for determining if certain floating point results are equivalent to zero (0).*/
+(double) getEpsilon;

/**Returns the maximum spherical mercator projection latitude.*/
+(double) getMercatorMax;

/**Returns the minimum spherical mercator projection latitude.*/
+(double) getMercatorMin;

/**Returns the internal height scale divisor constant.*/
+(double) getHeightScaleDivisor;

/**Returns the current spherical mercator max level supported by the engine.*/
+(unsigned int) getMaxLevel;

/**
 Returns an array points that form a circle around a location.
 The returned array contains NSValue objects that 'wrap' a CGPoint struct where the x value is longitude and the y value is latitude.
 @param location The geographic center of the circle.
 @param radius The radius in nautical miles of the circle.
 @param segmentCount The number of segments to break the circle into.
 */
+(NSMutableArray*) createCircleAroundLocation:(CLLocationCoordinate2D) location
										radius:(double) radius
								 segmentCount:(int) segmentCount;

@end
