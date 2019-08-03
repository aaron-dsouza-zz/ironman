//
//  MEXPlaneFDRParser.h
//  AltusMappingEngine
//
//  Created by Bruce Shankle on 9/18/14.
//  Copyright (c) 2014 BA3, LLC. All rights reserved.
//

#import <Foundation/Foundation.h>

/**A utility for parsing XPlane .fdr files.*/
@interface MEXPlaneFDRParser : NSObject
+(NSArray*) parseFile:(NSString*) fileName;
@end

/**A utility for parsing XPlane Data.txt files.*/
@interface MEXPlaneDataTextParser : NSObject
+(NSArray*) parseFile:(NSString*) fileName;
@end
