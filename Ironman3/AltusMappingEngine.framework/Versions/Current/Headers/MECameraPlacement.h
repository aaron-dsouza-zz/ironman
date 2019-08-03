//
//  MECameraPlacement.h
//  AltusMappingEngine
//
//  Created by Bruce Shankle on 2/2/15.
//  Copyright (c) 2015 BA3, LLC. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreLocation/CoreLocation.h>
#import <CoreGraphics/CoreGraphics.h>

@interface MECameraPlacement : NSObject
@property (assign) CLLocationCoordinate2D location;
@property (assign) CGFloat altitude;
@property (assign) CGFloat heading;
@property (assign) CGFloat roll;
@property (assign) CGFloat pitch;
@end
