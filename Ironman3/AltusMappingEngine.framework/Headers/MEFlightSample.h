//  Copyright (c) 2014 BA3, LLC. All rights reserved.
#import <Foundation/Foundation.h>
#import <CoreLocation/CoreLocation.h>

/** Represents a data sample from a flight simulator.*/
@interface MEFlightSample : NSObject
@property (assign) CLLocationCoordinate2D location;
/** Time in seconds since beginning of flight.*/
@property (assign) double timeSeconds;
@property (assign) double tempDegreeC;
/** Height above mean sea level.*/
@property (assign) double heightMSLFeet;
/** Height above mean sea level.*/
@property (assign) double heightMSLMeters;
/** Radio altimeter height.*/
@property (assign) double heightRAFeet;
/** -1 to 1 (left, right).*/
@property (assign) double aileronRatio;
/** -1 to 1 (nose down, nose up).*/
@property (assign) double elevatorRatio;
/** -1 to 1 (left, right.*/
@property (assign) double rudderRatio;
/** Positive up.*/
@property (assign) double pitchDegrees;
@property (assign) double rollDegrees;
@property (assign) double headingDegrees;
@property (assign) double speedIndicatedKnots;
@property (assign) double verticalSpeedIndicatedFeetPerMinute;
/** Positive nose right.*/
@property (assign) double slipIndicatedDegrees;
/** Positive right.*/
@property (assign) double turnSlipIndicatedDegrees;
@property (assign) double speedIndicatedMach;
@property (assign) double angleOfAttackInidcated;
@end
